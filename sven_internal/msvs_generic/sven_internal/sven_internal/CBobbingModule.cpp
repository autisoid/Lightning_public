#include "StdAfx.h"
#include "CBobbingModule.hpp"

//Credits go to somebody from some Half-Life forums, I dunno.
//Snark gave me this code - xWhitey

#define HL2_BOB_CYCLE_MIN 1.0f
#define HL2_BOB_CYCLE_MAX 0.45f
#define HL2_BOB_UP 0.5f

CBobbingModule* g_pBobbingModule;

float RemapVal(float val, float A, float B, float C, float D) {
	return C + (D - C) * (val - A) / (B - A);
}

float g_lateralBob;
float g_verticalBob;

float V_CalcNewBob(struct ref_params_s* pparams)
{
	static double bobtime;
	static double lastbobtime;
	float cycle;

	if (!g_pBobbingModule) return 0.0f;
	if (!g_pdblClientTime) return 0.f;
	if (!g_pPlayerMove) return 0.0f;

	vec3_t vel = g_pPlayerMove->velocity;
	vel[2] = 0;

	if (pparams->onground == -1 || *g_pdblClientTime == lastbobtime) {
		return 0.0f;
	}

	float speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1]) * (!g_p_fGameSpeed ? 1.f : float(*g_p_fGameSpeed / 1000.0));

	if (!g_pBobbingModule->m_pBobStrengthIsUncapped->Get()) {
		int cap = g_pBobbingModule->m_pBobStrengthSpeedCap->Get();
		speed = CUtils::Clamp(-cap, cap, speed);
	}

	float bob_offset = RemapVal(speed, 0, 320, 0.0f, 1.0f);

	bobtime += float((*g_pdblClientTime) - lastbobtime) * bob_offset;
	lastbobtime = *g_pdblClientTime;

	cycle = bobtime - (int)(bobtime / HL2_BOB_CYCLE_MAX) * HL2_BOB_CYCLE_MAX;
	cycle /= HL2_BOB_CYCLE_MAX;

	if (cycle < HL2_BOB_UP) {
		cycle = M_PI * cycle / HL2_BOB_UP;
	} else {
		cycle = M_PI + M_PI * (cycle - HL2_BOB_UP) / (1.0 - HL2_BOB_UP);
	}

	g_verticalBob = speed * 0.004f;
	g_verticalBob = g_verticalBob * 0.3 + g_verticalBob * 0.7 * sin(cycle);

	g_verticalBob = CUtils::Clamp(-7.0f, 4.0f, g_verticalBob);

	cycle = bobtime - (int)(bobtime / HL2_BOB_CYCLE_MAX * 2) * HL2_BOB_CYCLE_MAX * 2;
	cycle /= HL2_BOB_CYCLE_MAX * 2;

	if (cycle < HL2_BOB_UP) {
		cycle = M_PI * cycle / HL2_BOB_UP;
	} else {
		cycle = M_PI + M_PI * (cycle - HL2_BOB_UP) / (1.0 - HL2_BOB_UP);
	}

	g_lateralBob = speed * 0.004f;
	g_lateralBob = g_lateralBob * 0.3 + g_lateralBob * 0.7 * sin(cycle);

	g_lateralBob = CUtils::Clamp(-7.0f, 4.0f, g_lateralBob);

	return 0.0f;
}

void VectorMA(const float *veca, float scale, const float *vecb, float *vecc) {
	vecc[0] = veca[0] + scale * vecb[0];
	vecc[1] = veca[1] + scale * vecb[1];
	vecc[2] = veca[2] + scale * vecb[2];
}

CBobbingModule::CBobbingModule() : CModule("Bobbing", "Render") {
	m_pVecValues->push_back(m_pBobStrengthIsUncapped = Q_new(CBoolValue)("Bob strength is uncapped", "sc_bobbing_bob_strength_is_uncapped", false));
	m_pVecValues->push_back(m_pBobStrengthSpeedCap = Q_new(CIntegerValue)("Bob strength speed cap", "sc_bobbing_bob_strength_speed_cap", 320, 4096, 320));
	m_pBobStrengthSpeedCap->RegisterOnceValueChangedCallback([](void* _UserData, void* _Value, void* _PreviousValue) {
		CBobbingModule* thiz = (CBobbingModule*)_UserData;
		int* value = (int*)_Value;
		if ((*value) < 320) {
			*value = 320;
		}
	}, this);
	m_pBobStrengthSpeedCap->GetConVar()->RegisterCallback([](void* _UserData, char** _Value) {
		CBobbingModule* thiz = (CBobbingModule*)_UserData;
		int value = 0;
		sscanf_s(*_Value, "%d", &value);
		if (value < 320) {
			value = 320;
			*_Value = "320";
		}
		thiz->m_pBobStrengthSpeedCap->Set(value);
	}, this);

	g_pBobbingModule = this;

	DefineConCmd("sc_lightning_hl2_bobbing", CBobbingModule);
}

void CBobbingModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CBobbingModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CBobbingModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kMoveEvent) {
		if (!g_pRefParams) return;

		cl_entity_t* view = g_pEngfuncs->GetViewModel();
		Vector forward, right;
		g_pEngfuncs->pfnAngleVectors(view->angles, forward, right, NULL);

		V_CalcNewBob(g_pRefParams);

		VectorMA(view->origin, g_verticalBob * 0.1f, forward, view->origin);

		view->origin[2] += g_verticalBob * 0.1f;

		VectorMA(view->origin, g_lateralBob * 0.8f, right, view->origin);
	}
}