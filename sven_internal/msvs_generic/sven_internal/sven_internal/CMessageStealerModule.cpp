#include "StdAfx.h"
#include "CMessageStealerModule.hpp"

CMessageStealerModule* g_pMessageStealerModule;

char* UTIL_GenerateRandomString(const int& _Length, CTrustedRandom* _RandomDevice) {
	static const char lpszCharset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMOPQRSTUWXYZ0123456789";

	char* szResult = indirect_cast<char*>(Q_malloc(_Length + 1));

	for (int idx = 0; idx < _Length; idx++) {
		szResult[idx] = lpszCharset[_RandomDevice->Rand(0, sizeof(lpszCharset) - 1)];
	}

	szResult[_Length] = '\0';

	return szResult;
}

int __cdecl HOOKED_SayText_UserMsg(const char* pszName, int iSize, void* pbuf) {
	if (!g_pMessageStealerModule) {
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}
	if (g_pMessageStealerModule->m_bState == false) {
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}

	int iReadCount = 0;
	byte* buf = (byte*)pbuf;

	int iSenderId = ((unsigned char)buf[iReadCount]);
	if (iSenderId < 1 || iSenderId > 32) {
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}
	cl_entity_t* pLocalPlayer = g_pEngfuncs->GetLocalPlayer();
	static bool bRecursionGuard = false;

	if (!pLocalPlayer || iSenderId == pLocalPlayer->index) {
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}

	/*if (bRecursionGuard) {
		bRecursionGuard = false;
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}

	bRecursionGuard = true;*/

	iReadCount++;
	char* szMessage = indirect_cast<char*>(&buf[iReadCount]);
	if (!szMessage) {
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}
	if (szMessage[0] != '\x02') {
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}
	szMessage++;
	if (szMessage[0] == '"') {
		szMessage++;
	}
	auto iLength = strlen(szMessage);
	if (szMessage[iLength - 1] == '"') {
		szMessage[iLength - 1] = '\0';
	}

	char szBuffer[512];
	if (g_pMessageStealerModule->m_pConcatenateRandomCharsAtTheBeginning->Get() && g_pMessageStealerModule->m_pConcatenateRandomCharsAtTheEnd->Get()) {
		int iMin = Q_labsi(g_pMessageStealerModule->m_pRandomCharactersLengthMin->Get());
		int iMax = Q_labsi(g_pMessageStealerModule->m_pRandomCharactersLengthMax->Get());
		if (iMax < iMin) {
			int iTemp = iMax;
			iMax = iMin;
			iMin = iTemp;
		}

		char* szBeginning = UTIL_GenerateRandomString(g_pMessageStealerModule->m_pRandomDevice->Rand(iMin, iMax), g_pMessageStealerModule->m_pRandomDevice);
		char* szEnd = UTIL_GenerateRandomString(g_pMessageStealerModule->m_pRandomDevice->Rand(iMin, iMax), g_pMessageStealerModule->m_pRandomDevice);

		sprintf_s(szBuffer, ";say <%s> %s <%s>;\n", szBeginning, szMessage, szEnd);
		g_pEngfuncs->pfnClientCmd(szBuffer);
		Q_free(szBeginning);
		Q_free(szEnd);
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}
	if (!g_pMessageStealerModule->m_pConcatenateRandomCharsAtTheBeginning->Get() && g_pMessageStealerModule->m_pConcatenateRandomCharsAtTheEnd->Get()) {
		int iMin = Q_labsi(g_pMessageStealerModule->m_pRandomCharactersLengthMin->Get());
		int iMax = Q_labsi(g_pMessageStealerModule->m_pRandomCharactersLengthMax->Get());
		if (iMax < iMin) {
			int iTemp = iMax;
			iMax = iMin;
			iMin = iTemp;
		}

		char* szEnd = UTIL_GenerateRandomString(g_pMessageStealerModule->m_pRandomDevice->Rand(iMin, iMax), g_pMessageStealerModule->m_pRandomDevice);

		sprintf_s(szBuffer, ";say %s <%s>;\n", szMessage, szEnd);
		g_pEngfuncs->pfnClientCmd(szBuffer);
		Q_free(szEnd);
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}
	if (g_pMessageStealerModule->m_pConcatenateRandomCharsAtTheBeginning->Get() && !g_pMessageStealerModule->m_pConcatenateRandomCharsAtTheEnd->Get()) {
		int iMin = Q_labsi(g_pMessageStealerModule->m_pRandomCharactersLengthMin->Get());
		int iMax = Q_labsi(g_pMessageStealerModule->m_pRandomCharactersLengthMax->Get());
		if (iMax < iMin) {
			int iTemp = iMax;
			iMax = iMin;
			iMin = iTemp;
		}

		char* szBeginning = UTIL_GenerateRandomString(g_pMessageStealerModule->m_pRandomDevice->Rand(iMin, iMax), g_pMessageStealerModule->m_pRandomDevice);

		sprintf_s(szBuffer, ";say <%s> %s;\n", szBeginning, szMessage);
		g_pEngfuncs->pfnClientCmd(szBuffer);
		Q_free(szBeginning);
		return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
	}

	sprintf_s(szBuffer, ";say %s;\n", szMessage);
	g_pEngfuncs->pfnClientCmd(szBuffer);

	return ORIG_SayText_UserMsg(pszName, iSize, pbuf);
}

CMessageStealerModule::CMessageStealerModule() : CModule("MessageStealer", "Misc") {
	m_pRandomDevice = Q_new(CTrustedRandom)();
	g_pMessageStealerModule = this;

	m_pVecValues->push_back(m_pConcatenateRandomCharsAtTheEnd = Q_new(CBoolValue)("Concatenate random chars at the end", "sc_msgstealer_concat_random_chars_at_the_end", true));
	m_pVecValues->push_back(m_pConcatenateRandomCharsAtTheBeginning = Q_new(CBoolValue)("Concatenate random chars at the beginning", "sc_msgstealer_concat_random_chars_at_the_beg", false));
	m_pVecValues->push_back(m_pRandomCharactersLengthMin = Q_new(CIntegerValue)("Random characters length min", "sc_msgstealer_random_chars_len_min", 5, 15, 10));
	m_pVecValues->push_back(m_pRandomCharactersLengthMax = Q_new(CIntegerValue)("Random characters length max", "sc_msgstealer_random_chars_len_max", 6, 20, 15));

	DefineConCmd("sc_lightning_messagestealer", CMessageStealerModule);
}

void CMessageStealerModule::OnEnable() {
	CModule::OnEnable();
	m_pRandomDevice->FeedRandWithTime(time(NULL));
}

void CMessageStealerModule::OnDisable() {
	m_pRandomDevice->FeedRandWithTime(time(NULL));
	CModule::OnDisable();
}

void CMessageStealerModule::OnEvent(_In_ const ISimpleEvent* _Event) {

}