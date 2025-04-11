#include "StdAfx.h"
#include "CApplicationSpeedModule.hpp"

CApplicationSpeedModule* g_pApplicationSpeedModule = nullptr;

//LTFX speedhack implementation but done rather without using 'realtime'

//FIXME: Has issues when m_pSpeed is set to 0
//HINT: Do not speedhack the game when m_pSpeed is 0?
BOOL WINAPI HOOKED_QueryPerformanceCounter(LARGE_INTEGER* _PerformanceCount) {
	static LONGLONG oldfakevalue = 0;
	static LONGLONG oldrealvalue = 0;

	LONGLONG newvalue;

	if (oldfakevalue == 0 || oldrealvalue == 0) {
		oldfakevalue = _PerformanceCount->QuadPart;
		oldrealvalue = _PerformanceCount->QuadPart;
	}

	BOOL result = ORIG_QueryPerformanceCounter(_PerformanceCount);

	newvalue = _PerformanceCount->QuadPart;
	if (g_flBaseApplicationSpeed < 0.01f) {
		g_bForceImGuiToNotDrawAnything = true;
		g_flBaseApplicationSpeed = 0.01f;
		g_bForceImGuiToNotDrawAnything = false;
	}
	newvalue = oldfakevalue + (LONGLONG)((newvalue - oldrealvalue) * (static_cast<double>(g_flBaseApplicationSpeed)));

	oldrealvalue = _PerformanceCount->QuadPart;
	oldfakevalue = newvalue;

	_PerformanceCount->QuadPart = newvalue;

	return result;
}

CApplicationSpeedModule::CApplicationSpeedModule() : CModule("ApplicationSpeed", "Misc") {
	g_flBaseApplicationSpeed = 1.0f;

	m_pVecValues->push_back(m_pSpeed = Q_new(CFloatValue)("Speed", "sc_application_speed_speed", 0.05f, 100.f, 1.0f));
	m_pSpeed->RegisterOnceValueChangedCallback([](void* _UserData, void* _Value, void* _PreviousValue) {
		g_bForceImGuiToNotDrawAnything = true;
		CApplicationSpeedModule* thiz = (CApplicationSpeedModule*)_UserData;
		float* value = (float*)_Value;
		if ((*value) < 0.05f) {
			*value = 0.05f;
		}
		if (thiz->m_bState)
			g_flBaseApplicationSpeed = *value;
		g_bForceImGuiToNotDrawAnything = false;
	}, this);
	m_pSpeed->GetConVar()->RegisterCallback([](void* _UserData, char** _Value) {
		g_bForceImGuiToNotDrawAnything = true;
		CApplicationSpeedModule* thiz = (CApplicationSpeedModule*)_UserData;
		float value = 0.0f;
		sscanf_s(*_Value, "%f", &value);
		if (value < 0.05f) {
			value = 0.05f;
			char* pszNewValue = (char*)Q_malloc(sizeof("0.05")); //it will be freed in next Set call
			memset(pszNewValue, 0, sizeof("0.05"));
			strncpy_s(pszNewValue, sizeof("0.05"), "0.05", sizeof("0.05") - 1);
			*_Value = pszNewValue;
		}
		thiz->m_pSpeed->Set(value);
		//since we're setting speed in m_pSpeed's callback, this thing here is unnecessary
		//if (thiz->m_bState)
			//g_flBaseApplicationSpeed = value;
		g_bForceImGuiToNotDrawAnything = false;
	}, this);

	g_pApplicationSpeedModule = this;

	MH_STATUS status = MH_CreateHook(QueryPerformanceCounter, HOOKED_QueryPerformanceCounter, reinterpret_cast<void**>(&ORIG_QueryPerformanceCounter));
	if (status == MH_OK) {
		status = MH_EnableHook(MH_ALL_HOOKS);
		if (status != MH_OK) {
			CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CApplicationSpeedModule::CApplicationSpeedModule(): failed to enable hook for QueryPerformanceCounter! Reason: %s\n", MH_StatusToString(status));
		}
	} else {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CApplicationSpeedModule::CApplicationSpeedModule(): failed to enable hook for QueryPerformanceCounter! Reason: %s\n", MH_StatusToString(status));
	}

	DefineConCmd("sc_lightning_application_speed", CApplicationSpeedModule);
}

void CApplicationSpeedModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CApplicationSpeedModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	g_flBaseApplicationSpeed = 1.f;
	CModule::OnDisable();
}

void CApplicationSpeedModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		g_flBaseApplicationSpeed = m_pSpeed->Get();
	}
}