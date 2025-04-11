/**
 * Copyright - xWhitey, 2024.
 * CAutoFreezeOnChatModule.cpp - CAutoFreezeOnChatModule impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CAutoFreezeOnChatModule.hpp"
#include "CAirStuckModule.hpp"

//the name of the module explains itself

CAutoFreezeOnChatModule::CAutoFreezeOnChatModule() : CModule("AutoFreezeOnChat", "Misc") {
	m_pVecValues->push_back(m_pUnfreezeAfter = Q_new(CFloatValue)("Unfreeze after (in ms)", "sc_autofreezeonchat_unfreeze_after", 0.0f, 2000.0f, 500.f));

	m_ui64LastNotPlayingTime = 0;
	m_pAirStuckModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAirStuckModule>(); //Maybe create g_pAirStuckModule so we don't have to iterate over every module HERE?
	m_iState = 0;

	DefineConCmd("sc_lightning_autofreezeonchat", CAutoFreezeOnChatModule);
}

void CAutoFreezeOnChatModule::OnEnable() {
	CModule::OnEnable();

	if (!m_pAirStuckModule)
		m_pAirStuckModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAirStuckModule>();

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CAutoFreezeOnChatModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CAutoFreezeOnChatModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_pPlayerMove) return;
		if (!g_pRefParams) return;
		if (!g_pEngfuncs->GetLocalPlayer()) return;

		if (!m_pAirStuckModule) {
			m_pAirStuckModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAirStuckModule>();
			return;
		}

		if (m_pAirStuckModule->m_bState == true && m_iState == 0) return; //module was turned on by the user itself

		if ((g_bIsPlaying == false || (*g_piKeyDest) == LIGHTNING_REHL_CONST_KEYDEST_KEY_MESSAGE)) {
			m_ui64LastNotPlayingTime = g_pTimer->m_nTimeInMilliseconds;
			if (m_pAirStuckModule->m_bState == false) {
				m_pAirStuckModule->Toggle();
			}
			if (m_iState == 0) {
				m_iState = 1;
			}
		}

		switch (m_iState) {
		case 1: {
			if (g_bIsPlaying == true && (*g_piKeyDest) == LIGHTNING_REHL_CONST_KEYDEST_KEY_GAME) {
				m_iState = 2;
			}
		}
			break;
		case 2: {
			double dblTimeDifference = TimeDifference(m_ui64LastNotPlayingTime, g_pTimer->m_nTimeInMilliseconds);
			if (dblTimeDifference > static_cast<double>(m_pUnfreezeAfter->Get() / 1000.0f)) {
				m_iState = 3;
			}
		}
			break;
		case 3: {
			if (m_pAirStuckModule->m_bState == true) {
				m_pAirStuckModule->Toggle();
			}
			m_iState = 0;
		}
			break;
		}
	}
}