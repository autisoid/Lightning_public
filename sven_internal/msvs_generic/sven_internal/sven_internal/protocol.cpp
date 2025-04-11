/**
 * Copyright - xWhitey, 2024.
 * protocol.cpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "protocol.hpp"
#include "CAntiSlowhackModule.hpp"
#include "CDisablerModule.hpp"

CProtocolHooks::_m_tProtocolMessage CProtocolHooks::_ms_pfnStuffText = nullptr;
CProtocolHooks::_m_tProtocolMessage CProtocolHooks::_ms_pfnDirector = nullptr;
CProtocolHooks::_m_tProtocolMessage CProtocolHooks::_ms_pfnDisconnect = nullptr;
CProtocolHooks::_m_tProtocolMessage CProtocolHooks::_ms_pfnServerInfo = nullptr;
CProtocolHooks::_m_tProtocolMessage CProtocolHooks::_ms_pfnSignonNum = nullptr;
int* CProtocolHooks::_ms_piReadCount = nullptr;
int CProtocolHooks::_ms_iSavedReadCount = 0;
CProtocolHooks::_m_tMSG_ReadString CProtocolHooks::_ms_pfnMSG_ReadString = nullptr;
CProtocolHooks::_m_tMSG_ReadByte CProtocolHooks::_ms_pfnMSG_ReadByte = nullptr;
CProtocolHooks::_m_tMSG_ReadLong CProtocolHooks::_ms_pfnMSG_ReadLong = nullptr;
const char* CProtocolHooks::_ms_rglpszPredefinedAllowedList[] = { "npc_findcover", "fullserverinfo", "retry", "reconnect" };
cvar_t* CProtocolHooks::_ms_pConVarIgnoreDisconnect = nullptr;
cvar_t* CProtocolHooks::_ms_pConVarIgnoreInvalidSignon = nullptr;

CVector<char*>* Split(_In_z_ const char* _Command, _In_ char _Delimiter) {
	if (!_Command) {
		return nullptr;
	}

	CVector<char*>* lpVecOutput = Q_new(CVector<char*>)({});
	const char* szStart = _Command;

	while (*_Command) {
		if (*_Command == _Delimiter) {
			int iLength = _Command - szStart;
			char* szPart = (char*)Q_malloc(sizeof(char*) * (iLength + 1));
			strncpy_s(szPart, iLength + 1, szStart, iLength);
			szPart[iLength] = '\0';
			lpVecOutput->push_back(szPart);
			szStart = _Command + 1;
		}
		_Command++;
	}

	int iLength = _Command - szStart;
	if (iLength > 0) {
		char* szPart = (char*)Q_malloc(sizeof(char*) * (iLength + 1));
		strncpy_s(szPart, iLength + 1, szStart, iLength);
		szPart[iLength] = '\0';
		lpVecOutput->push_back(szPart);
	}

	return lpVecOutput;
}

void CProtocolHooks::Initialise() {
	FindAsync(g_pHwDll, patterns::engine::MSG_BeginReading_mov_dword_ptr_msg_readcount, _ms_piReadCount, [](const char* _Version) {
		_ms_piReadCount = *reinterpret_cast<int**>(reinterpret_cast<DWORD>(_ms_piReadCount) + 2);
		printf("Found msg_readcount at 0x%p.\n", _ms_piReadCount);
	}, []() { printf("Couldn't find msg_readcount! Anti-slowhack is unavailable.\n"); });
	FindAsync(g_pHwDll, patterns::engine::MSG_ReadString, _ms_pfnMSG_ReadString, [](const char* _Version) {
		printf("Found MSG_ReadString at 0x%p.\n", _ms_pfnMSG_ReadString);
	}, []() { printf("Couldn't find MSG_ReadString! Anti-slowhack is unavailable.\n"); });
	FindAsync(g_pHwDll, patterns::engine::MSG_ReadByte, _ms_pfnMSG_ReadByte, [](const char* _Version) {
		printf("Found MSG_ReadByte at 0x%p.\n", _ms_pfnMSG_ReadByte);
	}, []() { printf("Couldn't find MSG_ReadByte! Anti-slowhack for svc_director is unavailable.\n"); });
	FindAsync(g_pHwDll, patterns::engine::MSG_ReadLong, _ms_pfnMSG_ReadLong, [](const char* _Version) {
		printf("Found MSG_ReadLong at 0x%p.\n", _ms_pfnMSG_ReadLong);
	}, []() { printf("Couldn't find MSG_ReadLong! Some features will be unavailable.\n"); });
	if (_ms_piReadCount && _ms_pfnMSG_ReadString) {
		FindAsync(g_pHwDll, patterns::engine::CL_Parse_StuffText, _ms_pfnStuffText, [](const char* _Version) {
			printf("Found CL_Parse_StuffText at 0x%p.\n", _ms_pfnStuffText);
			MH_STATUS status = MH_CreateHook(_ms_pfnStuffText, CProtocolHooks::StuffText, reinterpret_cast<void**>(&_ms_pfnStuffText));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for CL_Parse_StuffText: %s\n", MH_StatusToString(status));
			} else printf("Couldn't create hook for CL_Parse_StuffText: %s\n", MH_StatusToString(status));
		}, []() { printf("Couldn't find CL_Parse_StuffText! Anti-slowhack is unavailable.\n"); });
		FindAsync(g_pHwDll, patterns::engine::CL_Parse_Director, _ms_pfnDirector, [](const char* _Version) {
			printf("Found CL_Parse_Director at 0x%p.\n", _ms_pfnDirector);
			MH_STATUS status = MH_CreateHook(_ms_pfnStuffText, CProtocolHooks::Director, reinterpret_cast<void**>(&_ms_pfnDirector));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for CL_Parse_Director: %s\n", MH_StatusToString(status));
			}
			else printf("Couldn't create hook for CL_Parse_Director: %s\n", MH_StatusToString(status));
		}, []() { printf("Couldn't find CL_Parse_Director! Anti-slowhack for svc_director is unavailable.\n"); });
		FindAsync(g_pHwDll, patterns::engine::CL_Parse_Disconnect, _ms_pfnDisconnect, [](const char* _Version) {
			printf("Found CL_Parse_Disconnect at 0x%p.\n", _ms_pfnDisconnect);
			MH_STATUS status = MH_CreateHook(_ms_pfnDisconnect, CProtocolHooks::Disconnect, reinterpret_cast<void**>(&_ms_pfnDisconnect));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status == MH_OK) {
					_ms_pConVarIgnoreDisconnect = g_pEngfuncs->pfnRegisterVariable("sc_lightning_ignore_disconnect", "0", 0);
				}
				else {
					printf("Couldn't enable hook for CL_Parse_Disconnect: %s\n", MH_StatusToString(status));
				}
			}
			else printf("Couldn't create hook for CL_Parse_Disconnect: %s\n", MH_StatusToString(status));
			}, []() { printf("Couldn't find CL_Parse_Disconnect! svc_disconnect ignoring is unavailable.\n"); });
	}
	if (_ms_pfnMSG_ReadLong) {
		FindAsync(g_pHwDll, patterns::engine::CL_Parse_ServerInfo, _ms_pfnServerInfo, [](const char* _Version) {
			printf("Found CL_Parse_ServerInfo at 0x%p.\n", _ms_pfnServerInfo);
			MH_STATUS status = MH_CreateHook(_ms_pfnServerInfo, CProtocolHooks::ServerInfo, reinterpret_cast<void**>(&_ms_pfnServerInfo));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status == MH_OK) {
					//aaaa
				} else {
					printf("Couldn't enable hook for CL_Parse_ServerInfo: %s\n", MH_StatusToString(status));
				}
			}
			else printf("Couldn't create hook for CL_Parse_ServerInfo: %s\n", MH_StatusToString(status));
		}, []() { printf("Couldn't find CL_Parse_ServerInfo! Some features will be unavailable.\n"); });
	}
	if (_ms_pfnMSG_ReadByte) {
		FindAsync(g_pHwDll, patterns::engine::CL_Parse_SignonNum, _ms_pfnSignonNum, [](const char* _Version) {
			printf("Found CL_Parse_SignonNum at 0x%p.\n", _ms_pfnSignonNum);
			MH_STATUS status = MH_CreateHook(_ms_pfnSignonNum, CProtocolHooks::SignonNum, reinterpret_cast<void**>(&_ms_pfnSignonNum));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status == MH_OK) {
					_ms_pConVarIgnoreInvalidSignon = g_pEngfuncs->pfnRegisterVariable("sc_lightning_ignore_invalid_signon", "1", 0);
				} else {
					printf("Couldn't enable hook for CL_Parse_SignonNum: %s\n", MH_StatusToString(status));
				}
			}
			else printf("Couldn't create hook for CL_Parse_SignonNum: %s\n", MH_StatusToString(status));
		}, []() { printf("Couldn't find CL_Parse_SignonNum! Some features will be unavailable.\n"); });
	}
}

void CProtocolHooks::SignonNum() {
	SaveReadCount();
#ifdef XWHITEY_PROPRIETARY
	if (g_pDisablerModule && g_pDisablerModule->m_bState && g_pDisablerModule->m_pMode->Get() == 0 /* Clockwindow */) {
		(void)_ms_pfnMSG_ReadByte();
		return;
	}
#endif //XWHITEY_PROPRIETARY
	if (_ms_pConVarIgnoreInvalidSignon->value != 0 && g_bIsPlaying && g_pEngfuncs->GetLocalPlayer() && g_pPlayerMove && g_pRefParams) {
		(void)_ms_pfnMSG_ReadByte();
		return;
	}
	return _ms_pfnSignonNum();
}

void CProtocolHooks::ServerInfo() {
	SaveReadCount();

	int lProtocolVersion = _ms_pfnMSG_ReadLong();
	(void)lProtocolVersion; //We aren't dealing with this one so we just void cast it
	g_iServerSpawnCount = _ms_pfnMSG_ReadLong();

	RestoreReadCount();

	return _ms_pfnServerInfo();
}

void CProtocolHooks::Disconnect() {
	if (_ms_pConVarIgnoreDisconnect->value == 0) {
		return _ms_pfnDisconnect();
	}

	(void)_ms_pfnMSG_ReadString(); //skip the reason

	return;
}

bool CProtocolHooks::IsCommandInPredefinedAllowedList(_In_z_ const char* _Command) {
	for (int idx = 0; idx < Q_ARRAYSIZE(_ms_rglpszPredefinedAllowedList); idx++) {
		if (!_strnicmp(_Command, _ms_rglpszPredefinedAllowedList[idx], strlen(_ms_rglpszPredefinedAllowedList[idx])))
			return true;
	}

	return false;
}

bool CProtocolHooks::IsCommandDangerous(_In_z_ const char* _Command) {
	CAntiSlowhackModule* lpAntiSlowHackModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAntiSlowhackModule>();
	if (!lpAntiSlowHackModule)
		return true;

	if (lpAntiSlowHackModule->m_pMode->Get() > 1) { //some retard changed the value of "Mode" in AntiSlowhack module by editing our configs directly
		return !IsCommandInPredefinedAllowedList(_Command);
	}

	switch (lpAntiSlowHackModule->m_pMode->Get()) {
	case 0: /* Block everything, except gameplay important commands */ {
		return !IsCommandInPredefinedAllowedList(_Command);
	}
		break;
	case 1: /* Whitelist */ {
		if (IsCommandInPredefinedAllowedList(_Command))
			return false;

		CVector<char*>* lpSplit = Split(lpAntiSlowHackModule->m_pWhitelist->Get(), ',');
		bool bResult = true;
		for (int idx = 0; idx < lpSplit->size(); idx++) {
			if (!_strnicmp(_Command, lpSplit->at(idx), strlen(lpSplit->at(idx)))) {
				bResult = false; //Yeah, it is slower now.
			}
		}
		for (int idx = 0; idx < lpSplit->size(); idx++) {
			Q_delete(lpSplit->at(idx));
		}
		Q_delete(lpSplit);

		return bResult;
	}
		break;
	}

	return false;
}

void CProtocolHooks::SaveReadCount() {
	_ms_iSavedReadCount = *_ms_piReadCount;
}

void CProtocolHooks::RestoreReadCount() {
	*_ms_piReadCount = _ms_iSavedReadCount;
}

template<class... _Ts> char* UTIL_VarArgs(_In_z_ const char* const _Format, _In_opt_ _Ts... _Args) {
	static char cs_acBuffer[1024];

	sprintf_s(cs_acBuffer, _Format, _Args...);

	return cs_acBuffer;
}

CVector<char*>* CProtocolHooks::SanitizeCommand(_In_z_ const char* _Command) {
	if (!_Command) {
		return nullptr;
	}

	CVector<char*>* lpVecOutput = Q_new(CVector<char*>)({});

	int iCommandLength = strlen(_Command);

	for (int idx = 0; idx < iCommandLength; idx++) {
		if (_Command[idx] == '\n' || _Command[idx] == ';') {
			int nCurrentIdx = idx + 1;
			int iStringSize = 0;

			for (int i = nCurrentIdx; i < iCommandLength + 1; i++) {
				if (_Command[i] == '\n' || _Command[i] == ';' || _Command[i] == '\0') {
					iStringSize = i - nCurrentIdx;
					idx = i - 1;
					break;
				}
			}

			if (iStringSize == 0) continue;

			char* lpszBuffer = (char*)Q_malloc(sizeof(char) * (iStringSize + 1));
			lpszBuffer[iStringSize - 1] = '\0';
			strncpy_s(lpszBuffer, iStringSize + 1, &_Command[nCurrentIdx], iStringSize);
			lpVecOutput->push_back(lpszBuffer);
		}
	}

	return lpVecOutput;
}

void CProtocolHooks::Director() {
	SaveReadCount();

	int cCmdType = _ms_pfnMSG_ReadByte();

	if (cCmdType == 10 /* DRC_CMD_STUFFTEXT */) {
		const char* szSanitizedCommand = _ms_pfnMSG_ReadString();
		CVector<char*>* lpVecParts = SanitizeCommand(szSanitizedCommand);
		if (!lpVecParts->empty()) {
			for (int idx = 0; idx < lpVecParts->size(); idx++) {
				char* szTheCommand = lpVecParts->at(idx);
				if (IsCommandDangerous(szTheCommand)) {
					g_pEngfuncs->Con_Printf("[Lightning] Blocked malicious console command, which server tried to execute: \"%s\"\n", szTheCommand);

					CAntiSlowhackModule* lpAntiSlowHackModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAntiSlowhackModule>();
					if (!lpAntiSlowHackModule)
						continue;

					if (lpAntiSlowHackModule->m_pNotifyPlayers->Get()) {
						g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";say \"%s\";\n", lpAntiSlowHackModule->m_pNotificationText->Get()));
					}
				} else {
					g_pEngfuncs->Con_Printf("[Lightning] Allowed execution of console command: \"%s\"\n", szTheCommand);
					g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";%s;\n", szTheCommand));
				}
			}
			for (int idx = 0; idx < lpVecParts->size(); idx++) {
				Q_delete(lpVecParts->at(idx));
			}
			Q_delete(lpVecParts);
		} else {
			for (int idx = 0; idx < lpVecParts->size(); idx++) {
				Q_delete(lpVecParts->at(idx));
			}
			Q_delete(lpVecParts);
			if (IsCommandDangerous(szSanitizedCommand)) {
				CAntiSlowhackModule* lpAntiSlowHackModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAntiSlowhackModule>();
				if (lpAntiSlowHackModule && lpAntiSlowHackModule->m_pNotifyPlayers->Get()) {
					g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";say \"%s\";\n", lpAntiSlowHackModule->m_pNotificationText->Get()));
				}

				g_pEngfuncs->Con_Printf("[Lightning] Blocked malicious console command, which server tried to execute: \"%s\"\n", szSanitizedCommand);
				return;
			}

			g_pEngfuncs->Con_Printf("[Lightning] Allowed execution of console command: \"%s\"\n", szSanitizedCommand);
			g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";%s;\n", szSanitizedCommand));
		}

		return; //don't let the engine execute the command, we do it on our own
	}

	RestoreReadCount();
}

void CProtocolHooks::StuffText() {
	SaveReadCount();

	const char* szSanitizedCommand = _ms_pfnMSG_ReadString();
	CVector<char*>* lpVecParts = SanitizeCommand(szSanitizedCommand);
	if (!lpVecParts->empty()) {
		for (int idx = 0; idx < lpVecParts->size(); idx++) {
			char* szTheCommand = lpVecParts->at(idx);
			if (IsCommandDangerous(szTheCommand)) {
				g_pEngfuncs->Con_Printf("[Lightning] Blocked malicious console command, which server tried to execute: \"%s\"\n", szTheCommand);

				CAntiSlowhackModule* lpAntiSlowHackModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAntiSlowhackModule>();
				if (!lpAntiSlowHackModule)
					continue;

				if (lpAntiSlowHackModule->m_pNotifyPlayers->Get()) {
					g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";say \"%s\";\n", lpAntiSlowHackModule->m_pNotificationText->Get()));
				}
			} else {
				g_pEngfuncs->Con_Printf("[Lightning] Allowed execution of console command: \"%s\"\n", szTheCommand);
				g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";%s;\n", szTheCommand));
			}
		}
		for (int idx = 0; idx < lpVecParts->size(); idx++) {
			Q_delete(lpVecParts->at(idx));
		}
		Q_delete(lpVecParts);
	} else {
		for (int idx = 0; idx < lpVecParts->size(); idx++) {
			Q_delete(lpVecParts->at(idx));
		}
		Q_delete(lpVecParts);
		if (IsCommandDangerous(szSanitizedCommand)) {
			CAntiSlowhackModule* lpAntiSlowHackModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CAntiSlowhackModule>();
			if (lpAntiSlowHackModule && lpAntiSlowHackModule->m_pNotifyPlayers->Get()) {
				g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";say \"%s\";\n", lpAntiSlowHackModule->m_pNotificationText->Get()));
				/*g_pEngfuncs->pfnClientCmd(";say \"");
				g_pEngfuncs->pfnClientCmd(const_cast<char*>(lpAntiSlowHackModule->m_pNotificationText->Get()));
				g_pEngfuncs->pfnClientCmd("\";\n");*/
			}

			g_pEngfuncs->Con_Printf("[Lightning] Blocked malicious console command, which server tried to execute: \"%s\"\n", szSanitizedCommand);
			return;
		}

		g_pEngfuncs->Con_Printf("[Lightning] Allowed execution of console command: \"%s\"\n", szSanitizedCommand);
		g_pEngfuncs->pfnClientCmd(UTIL_VarArgs(";%s;\n", szSanitizedCommand));
		/*g_pEngfuncs->pfnClientCmd(";");
		g_pEngfuncs->pfnClientCmd(const_cast<char*>(szSanitizedCommand));
		g_pEngfuncs->pfnClientCmd(";\n");*/

		return; //don't let the engine execute the command, we do it on our own
		//RestoreReadCount();
		//return _ms_pfnStuffText();
	}

	//return _ms_pfnStuffText();
}