#include "StdAfx.h"
#include "CInstantRespawnModule.hpp"

CInstantRespawnModule::CInstantRespawnModule() : CModule("InstantRespawn", "Exploit") {
	m_pVecValues->push_back(m_pBlockMovements = Q_new(CBoolValue)("Block movements", "sc_instantrespawn_block_movements", true));
	m_pVecValues->push_back(m_pChargeFor = Q_new(CIntegerValue)("Charge for", "sc_instantrespawn_charge_for", 128, 1024, 512));

	m_iHeldPacketsCount = 0;
	m_iAfterDeathJumpedCount = 0;

	DefineConCmd("sc_lightning_instantrespawn", CInstantRespawnModule);
}

void CInstantRespawnModule::OnEnable() {
	CModule::OnEnable();
	g_pEngfuncs->pfnClientCmd(";stuck_kill;\n");
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CInstantRespawnModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	*g_pfNextCmdTime = -1.f;

	m_iHeldPacketsCount = 0;

	g_pEngfuncs->pfnClientCmd(";w 5;-jump;\n");
	CModule::OnDisable();
}

void CInstantRespawnModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_p_fGameSpeed) return;

		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);

		if (*g_p_fGameSpeed)
			*g_p_fGameSpeed = 100000.0;

		if (m_pBlockMovements->Get()) {
			e->m_pCmd->forwardmove = 0;
			e->m_pCmd->sidemove = 0;
		}

		if (m_iHeldPacketsCount < 60) {
			*g_pfNextCmdTime = FLT_MAX;
			m_iHeldPacketsCount++;
		}
		else {
			*g_pfNextCmdTime = -1.f;
			m_iHeldPacketsCount = 0;
			//TODO: fix me
			g_pEngfuncs->pfnClientCmd(";+jump;\n");
			m_bState = false;
			this->OnDisable();
		}
	}
}