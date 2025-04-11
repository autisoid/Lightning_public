/**
 * Copyright - xWhitey, 2024.
 * CFakeLagModule.cpp - CFakeLagModule impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CFakeLagModule.hpp"

CFakeLagModule* g_pFakeLagModule;

CFakeLagModule::CFakeLagModule() : CModule("FakeLag", "Movement") {
	m_pVecValues->push_back(m_pChokeCmdsCount = Q_new(CIntegerValue)("Choked commands count", "sc_lightning_fakelag_ccmds_cnt", 1, 63, 17));

	g_pFakeLagModule = this;

	DefineConCmd("sc_lightning_fakelag", CFakeLagModule);
}

void CFakeLagModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CFakeLagModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	*g_pfNextCmdTime = -1.f;
	CModule::OnDisable();
}

//TODO: fix walking gaitsequences - make them broken (latched.prevframe)
void CFakeLagModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	static int s_iChokedCmds = 0;

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_pPlayerMove) {
			*g_pfNextCmdTime = -1.f;
			return;
		}
		if (g_bIsPlaying == false) {
			*g_pfNextCmdTime = -1.f;
			return;
		}

		if (s_iChokedCmds < m_pChokeCmdsCount->Get()) {
			*g_pfNextCmdTime = FLT_MAX;

			s_iChokedCmds++;
		} else {
			*g_pfNextCmdTime = -1.f;

			s_iChokedCmds = 0;
		}
	}
}