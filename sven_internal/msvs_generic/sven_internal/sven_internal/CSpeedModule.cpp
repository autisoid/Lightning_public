#include "StdAfx.h"
#include "CSpeedModule.hpp"
#include "CApplicationSpeedModule.hpp"

CSpeedModule* g_pSpeedModule;

//TODO: get rid of useless modes
CSpeedModule::CSpeedModule() : CModule("Speed", "Movement") {
	m_iGameSpeedChargeForCounter = 0;

	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_speed_mode", "GameSpeed\0Anarchy\0\0", 0)); //Vision must be here too

	m_pVecValues->push_back(m_pGameSpeed = Q_new(CFloatValue)("GameSpeed mode - Game speed", "sc_speed_game_spd_speed", 0.1f, 100000.f, 20000.f));
	m_pVecValues->push_back(m_pGameSpeedChargeFor = Q_new(CIntegerValue)("GameSpeed mode - Charge for", "sc_speed_game_spd_charge_for", 1, 256, 1));

	m_pVecValues->push_back(m_pAnarchyGameSpeed = Q_new(CFloatValue)("Anarchy mode - Game speed", "sc_speed_anarchy_speed", 0.1f, 100000.f, 12.f));
	m_pVecValues->push_back(m_pAnarchyBaseSpeed = Q_new(CFloatValue)("Anarchy mode - Base speed", "sc_speed_anarchy_base_speed", 0.1f, 1.0f, 0.2f));
	m_pVecValues->push_back(m_pAnarchyChargeFor = Q_new(CIntegerValue)("Anarchy mode - Charge for", "sc_speed_anarchy_charge_for", 1, 256, 1));

	DefineConCmd("sc_lightning_speed", CSpeedModule);
}

void CSpeedModule::OnEnable() {
	CModule::OnEnable();
	m_iGameSpeedChargeForCounter = 0;

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CSpeedModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	m_iGameSpeedChargeForCounter = 0;
	g_flBaseApplicationSpeed = 1.0f;
	
	//if (g_p_fGameSpeed)
		//*g_p_fGameSpeed = 1000.0;

	*g_pfNextCmdTime = -1.f;
	CModule::OnDisable();
}

const char* CSpeedModule::GetTag() {
	return m_pMode->GetModeByIndex(m_pMode->Get());
}

void CSpeedModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		//const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);

		if (m_pMode->Get() == 0 /* GameSpeed */) {
			if (!g_p_fGameSpeed) return;

			if (m_iGameSpeedChargeForCounter < m_pGameSpeedChargeFor->Get()) {
				*g_pfNextCmdTime = FLT_MAX;
				m_iGameSpeedChargeForCounter++;
			} else {
				*g_pfNextCmdTime = -1.f;
				m_iGameSpeedChargeForCounter = 0;
			}

			*g_p_fGameSpeed = static_cast<double>(m_pGameSpeed->Get()) * 1000.0;
		}

		if (m_pMode->Get() == 1 /* Anarchy */) {
			if (!g_p_fGameSpeed) return;
			g_flBaseApplicationSpeed = m_pAnarchyBaseSpeed->Get();

			if (m_iGameSpeedChargeForCounter < m_pAnarchyChargeFor->Get()) {
				*g_pfNextCmdTime = FLT_MAX;
				m_iGameSpeedChargeForCounter++;
			}
			else {
				*g_pfNextCmdTime = -1.f;
				m_iGameSpeedChargeForCounter = 0;
			}

			*g_p_fGameSpeed = static_cast<double>(m_pAnarchyGameSpeed->Get()) * 1000.0;
		}
	}
}