#include "StdAfx.h"
#include "CBreakLCModule.hpp"

CBreakLCModule::CBreakLCModule() : CModule("BreakLC", "Exploit") {
	m_pVecValues->push_back(m_pTest = Q_new(CIntegerValue)("Test", "sc_break_lc_test", -256, 256, -16));
	m_pVecValues->push_back(m_pFlipTicks = Q_new(CBoolValue)("Flip ticks", "sc_break_lc_flip_ticks", true));

	DefineConCmd("sc_lightning_break_lc", CBreakLCModule);
}

void CBreakLCModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CBreakLCModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CBreakLCModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	static bool s_bFlipTicks = false;

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (g_piOutgoingSequence) {
			if (m_pFlipTicks->Get()) {
				if (!s_bFlipTicks) {
					(*g_piOutgoingSequence) += m_pTest->Get();
					s_bFlipTicks = true;
				} else {
					s_bFlipTicks = false;
				}
			} else {
				(*g_piOutgoingSequence) += m_pTest->Get();
			}
		}
	}
}