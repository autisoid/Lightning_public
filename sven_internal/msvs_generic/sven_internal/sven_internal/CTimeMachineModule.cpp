#include "StdAfx.h"
#include "CTimeMachineModule.hpp"

//LTFX2 speed as it is
CTimeMachineModule::CTimeMachineModule() : CModule("TimeMachine", "Exploit") {
	m_pVecValues->push_back(m_pStrength = Q_new(CIntegerValue)("Strength", "sc_timemachine_strength", -256, 256, 16));
	m_pVecValues->push_back(m_pChargeFor = Q_new(CIntegerValue)("Charge for (in ticks)", "sc_timemachine_charge_for", 1, 256, 15));
	m_pVecValues->push_back(m_pBlockMovements = Q_new(CBoolValue)("Block movements", "sc_timemachine_block_movements", true));
	m_pVecValues->push_back(m_pDontSendNormalCmds = Q_new(CBoolValue)("Don't send normal cmds", "sc_timemachine_choke_normal_cmds", false));
	m_pVecValues->push_back(m_pRandomLerpMsec = Q_new(CBoolValue)("Bypass detections", "sc_timemachine_bypass_detections", true));

	m_iChargeCounter = 0;

	DefineConCmd("sc_lightning_timemachine", CTimeMachineModule);
}

void CTimeMachineModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CTimeMachineModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	*g_pfNextCmdTime = -1.f;
	CModule::OnDisable();
}

void CTimeMachineModule::OnEvent(const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kMoveEvent) {
		auto e = static_cast<const CMoveEvent*>(_Event);

		if (!m_pRandomLerpMsec->Get())
			e->m_pCmd->lerp_msec = 0;
		else
			e->m_pCmd->lerp_msec = rand() % (SHRT_MAX - SHRT_MIN + 1) + SHRT_MIN;
		*g_piLastOutgoingCmd = m_pStrength->Get();
	}

	if (_Event->GetType() == EEventType::kPreUpdateEvent) {
		auto e = static_cast<const CPreUpdateEvent*>(_Event);

		//e->m_pCmd->lerp_msec = 0;
		*g_piLastOutgoingCmd = m_pStrength->Get();
	}

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		auto e = static_cast<const CUpdateEvent*>(_Event);

		if (m_pBlockMovements->Get()) {
			e->m_pCmd->forwardmove = 0;
			e->m_pCmd->sidemove = 0;
		}

		if (m_iChargeCounter < m_pChargeFor->Get()) {
			*g_pfNextCmdTime = FLT_MAX;
			m_iChargeCounter++;
		} else {
			*g_pfNextCmdTime = -1.f;
			m_iChargeCounter = 0;
		}

		if (m_pDontSendNormalCmds->Get()) {
			*g_pfNextCmdTime = FLT_MAX;
		}

		if (!m_pRandomLerpMsec->Get())
			e->m_pCmd->lerp_msec = 0;
		else
			e->m_pCmd->lerp_msec = rand() % (SHRT_MAX - SHRT_MIN + 1) + SHRT_MIN;
		*g_piLastOutgoingCmd = m_pStrength->Get();
	}
}