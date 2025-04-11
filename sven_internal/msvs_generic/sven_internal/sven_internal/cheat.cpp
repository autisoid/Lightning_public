#include "StdAfx.h"
#include "Windows_Hooks.hpp"

#include "HwDLL.hpp"
#include "ClientDLL.hpp"

#include "command_hooks.hpp"
#include "protocol.hpp"
#include "extra_player_info.hpp"

#include "int_packet_dump.hpp"

#include "CClickGUIModule.hpp"

const char* const CCheat::ms_pszVersion = "0.5g";

CCheat::CCheat() {
	_ms_pTheCheat = this;

	g_pTimer = Q_new(CTimer)();
	g_pTimer->Initialise();

	m_pConsole = Q_new(CVirtualConsole)();
	m_pEventBus = Q_new(CEventManager)();
	m_pNotifications = Q_new(CNotifications)();
	m_pModuleManager = Q_new(CModuleManager)();

	m_pTheScripts = Q_new(CTheScripts)();

	m_pMovementRecorder = Q_new(CMovementRecorder)();

	g_pEntityDescriptions = Q_new(CEntityDescriptions)();
	g_pEntityList = Q_new(CEntityList)();
}

CCheat* CCheat::GetCheat() {
	return _ms_pTheCheat;
}

void CCheat::LoadUpScripts() {
	CSimpleIniA::TNamesDepend values;
	m_pTheScriptsMgr->m_pIni->GetAllKeys("Scripts", values);
	CSimpleIniA::TNamesDepend::const_iterator it;
	for (it = values.begin(); it != values.end(); ++it) {
		const char* pszModuleName = it->pItem;
		const char* pszScriptName = m_pTheScriptsMgr->GetValue("Scripts", pszModuleName);
		if (!pszScriptName || !pszScriptName[0]) {
			m_pConsole->Printf("[FATAL] CCheat::LoadUpScripts: script name wasn't specified for \"%s\" module.\n", pszModuleName);
			continue;
		}
		int iSize = sizeof("lightning.tech/scripts/") - 1 + strlen(pszScriptName) + sizeof(".as") - 1 + 1;
		char* pszPath = (char*)Q_malloc(iSize);
		if (!pszPath) {
			m_pConsole->Printf("[FATAL] CCheat::LoadUpScripts: failure allocating memory for pszPath.\n");
			continue;
		}
		sprintf_s(pszPath, iSize, "lightning.tech/scripts/%s.as", pszScriptName);
		m_pTheScripts->LoadThisScriptFile(pszPath, pszModuleName);
		Q_free(pszPath);
	}
}

void CCheat::SetupHooks() {
	ORIG_wglSwapBuffers = reinterpret_cast<decltype(ORIG_wglSwapBuffers)>(GetProcAddress(GetModuleHandleA("opengl32"), "wglSwapBuffers"));
	ORIG_wglGetCurrentContext = reinterpret_cast<decltype(ORIG_wglGetCurrentContext)>(GetProcAddress(GetModuleHandleA("opengl32"), "wglGetCurrentContext"));
	ORIG_wglCreateContext = reinterpret_cast<decltype(ORIG_wglCreateContext)>(GetProcAddress(GetModuleHandleA("opengl32"), "wglCreateContext"));
	ORIG_wglMakeCurrent = reinterpret_cast<decltype(ORIG_wglMakeCurrent)>(GetProcAddress(GetModuleHandleA("opengl32"), "wglMakeCurrent"));

	g_pHook = SetWindowsHookExA(WH_MOUSE_LL, AAL_LowLevelMouseHook, NULL, 0);

	if (auto hwDll = GetModuleHandleA("hw")) {
		FindAsync(hwDll, patterns::engine::SZ_GetSpace, ORIG_SZ_GetSpace, [](const char* _Version) {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found SZ_GetSpace at 0x%p\n", ORIG_SZ_GetSpace);
		}, [] {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find SZ_GetSpace :(\n");

			ORIG_SZ_GetSpace = nullptr;
		});

		FindAsync(hwDll, patterns::engine::ClientConnect__MSG_WriteByte_cls_netchan_message, g_pNetchanMessage, [](const char* _Version) {
			uintptr_t msg = (uintptr_t)g_pNetchanMessage;
			
			g_pNetchanMessage = *(void**)(msg + 1);

			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found &cls.netchan.message at 0x%p\n", g_pNetchanMessage);
		}, [] {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find &cls.netchan.message. :(\n"); 
			
			g_pNetchanMessage = nullptr;
		});
		FindAsync(hwDll, patterns::engine::g_fGameSpeed, g_p_fGameSpeed, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_p_fGameSpeed) + 2;
			double** ppdblGameSpeed = reinterpret_cast<double**>(dwOffset);
			g_p_fGameSpeed = *ppdblGameSpeed;
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(g_p_fGameSpeed, &mbi, sizeof(mbi));
			if ((mbi.AllocationProtect & PAGE_READWRITE) == 0) {
				//Unprotect it NOW because we will crash later if we try to access it.
				DWORD dwOld; //Maybe save that to a global variable to we can restore that later?..
				if (!VirtualProtect(g_p_fGameSpeed, sizeof(double), PAGE_READWRITE, &dwOld)) {
					printf("[FATAL] Failure unprotecting g_p_fGameSpeed! Expect big troubles...\n");
				}
				//We set PAGE_READWRITE and not PAGE_EXECUTE_READWRITE because this can lead to RCE's.
				//Actually we can just specify "1" as dwSize because the whole page protection will be changed anyway, with any size specified.
			}
			printf("[DEBUG] Found fGameSpeed at 0x%p, current game speed is %f\n", g_p_fGameSpeed, *g_p_fGameSpeed);
		}, []() {
			g_p_fGameSpeed = &g_dblStubGameSpeed; //do that so we don't crash if we try to set it
			printf("[DEBUG] Couldn't find fGameSpeed.\n");
		});
		FindAsync(hwDll, patterns::engine::g_fNextCmdTime, g_pfNextCmdTime, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_pfNextCmdTime) + 2;
			float** ppfNextCmdTime = reinterpret_cast<float**>(dwOffset);
			g_pfNextCmdTime = *ppfNextCmdTime;
			g_piLastOutgoingCmd = reinterpret_cast<int*>(reinterpret_cast<DWORD>(g_pfNextCmdTime) + sizeof(float));
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found fNextCmdTime at 0x%p\n", g_pfNextCmdTime);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find fNextCmdTime.\n");
		});
		FindAsync(hwDll, patterns::engine::SCR_NetGraph_mov_ebx_cls_netchan_incoming_sequence, g_piIncomingSequence, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_piIncomingSequence) + 2;
			if (_Version == "Sven-5.25" || _Version == "Sven-5.26") {
				int** ppiIncomingSequence = reinterpret_cast<int**>(dwOffset);
				g_piIncomingSequence = *ppiIncomingSequence;
				g_piIncomingReliableSequence = reinterpret_cast<int*>(reinterpret_cast<DWORD>(g_piIncomingSequence) - sizeof(int));
			} else if (_Version == "HL-8684" /*|| _Version == "HL-9920"*/) {
				g_piIncomingSequence = reinterpret_cast<int*>((uintptr_t)g_pHwDll + 0x1059B20);
			}
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found cls.netchan.incoming_sequence at 0x%p\n", g_piIncomingSequence);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find cls.netchan.incoming_sequence.\n");
		});
		FindAsync(hwDll, patterns::engine::CL_Move_mov_esi_cls_netchan_outgoing_sequence, g_piOutgoingSequence, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_piOutgoingSequence) + 2;
			if (_Version == "Sven-5.25" || _Version == "Sven-5.26") {
				int** ppiOutgoingSequence = reinterpret_cast<int**>(dwOffset);
				g_piOutgoingSequence = *ppiOutgoingSequence;
			}
			else if (_Version == "HL-8684" /*|| _Version == "HL-9920"*/) {
				g_piOutgoingSequence = reinterpret_cast<int*>((uintptr_t)g_pHwDll + 0x1059B30);//reinterpret_cast<int*>(offset);                                                                                                  |
			}
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found cls.netchan.outgoing_sequence at 0x%p\n", g_piOutgoingSequence);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find cls.netchan.outgoing_sequence.\n");
		});
		FindAsync(hwDll, patterns::engine::V_SetRefParams_fld_st0_cl_time, g_pdblClientTime, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_pdblClientTime) + 2;

			double** ppdblClientTime = reinterpret_cast<double**>(dwOffset);
			g_pdblClientTime = *ppdblClientTime;
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found cl.time at 0x%p\n", g_pdblClientTime);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find cl.time.\n");
		});
		FindAsync(hwDll, patterns::engine::ClientDLL_CheckStudioInterface_push_engine_studio_api, g_pEngineStudioAPI, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_pEngineStudioAPI) + 1;
			uintptr_t* address = *reinterpret_cast<uintptr_t**>(dwOffset);
			g_pEngineStudioAPI = reinterpret_cast<engine_studio_api_s*>(address);
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found g_pEngineStudioAPI at 0x%p\n", g_pEngineStudioAPI);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find g_pEngineStudioAPI.\n");
		});
		FindAsync(hwDll, patterns::engine::R_StudioDrawPlayer_mov_eax_currententity, g_pCurrentEntity, [](const char* _Version) {
			DWORD dwOffset = reinterpret_cast<DWORD>(g_pCurrentEntity) + 1;
			uintptr_t* address = *reinterpret_cast<uintptr_t**>(dwOffset);
			g_pCurrentEntity = reinterpret_cast<cl_entity_s**>(address);
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found currententity at 0x%p\n", g_pCurrentEntity);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find currententity.\n");
		});
		FindAsync(hwDll, patterns::engine::g_alpUserMsgs, g_alpUserMsgs, [](const char* _Version) {
			uintptr_t uiOffset = reinterpret_cast<uintptr_t>(g_alpUserMsgs) + 2;
			usermsg_t*** ppapUserMsgs = reinterpret_cast<usermsg_t***>(uiOffset);
			g_alpUserMsgs = *ppapUserMsgs;
			usermsg_t *pUserMsg = *g_alpUserMsgs;

			MH_STATUS status = MH_UNKNOWN;

			while (pUserMsg) {
				if (!_stricmp("CurWeapon", pUserMsg->name)) {
					status = MH_CreateHook(pUserMsg->function, HOOKED_CurWeapon_UserMsg, reinterpret_cast<void**>(&ORIG_CurWeapon_UserMsg));
				}
				if (!_stricmp("SayText", pUserMsg->name)) {
					status = MH_CreateHook(pUserMsg->function, HOOKED_SayText_UserMsg, reinterpret_cast<void**>(&ORIG_SayText_UserMsg));
				}

				pUserMsg = pUserMsg->next;
			}

			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) CCheat::GetCheat()->m_pConsole->Printf("[FATAL] Failed to enable the hook for CurWeapon_UserMsg! Reason: %s\n", MH_StatusToString(status));
			}
			else CCheat::GetCheat()->m_pConsole->Printf("[FATAL] Failed to create a hook for CurWeapon_UserMsg! Reason: %s\n", MH_StatusToString(status));
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found g_alpUserMsgs at 0x%p\n", g_alpUserMsgs);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find g_alpUserMsgs.\n");
		});
		FindAsync(hwDll, patterns::engine::Cvar_RegisterVariable, ORIG_Cvar_RegisterVariable, [](const char* _Version) {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found Cvar_RegisterVariable at 0x%p.\n", ORIG_Cvar_RegisterVariable);
		}, [] { CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find Cvar_RegisterVariable.\n"); });
		char* lpszSysTimescale;
		if (FindAsync(hwDll, (unsigned char*) "sys_timescale", "xxxxxxxxxxxxx", lpszSysTimescale)) {
			unsigned char* lpSysTimeScaleStringPtr = (unsigned char*) &lpszSysTimescale;
			cvar_t* sys_timescale;
			unsigned char aucSecondTry[4] = { lpSysTimeScaleStringPtr[0], lpSysTimeScaleStringPtr[1], lpSysTimeScaleStringPtr[2], lpSysTimeScaleStringPtr[3] };
			if (FindAsync(hwDll, aucSecondTry, "xxxx", sys_timescale)) {
				if (ORIG_Cvar_RegisterVariable)
					ORIG_Cvar_RegisterVariable(sys_timescale);
				CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found sys_timescale at 0x%p\n", sys_timescale);
			} else {
				CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find sys_timescale =(\n");
			}
		}
	}
	m_pConsole->RegisterCommand(Q_new(CVirtualConCmd)("hello", [](CVirtualConsole* _Console) {
		_Console->Printf("Hello, World! Argc: %d\n", _Console->Cmd_Argc());
	}));
	m_pConsole->RegisterCommand(Q_new(CVirtualConCmd)("echo", [](CVirtualConsole* _Console) {
		if (_Console->Cmd_Argc() < 2) {
			_Console->Printf("[info] Usage: echo <text> - Output specified text into this console.\n");

			return;
		}

		int nArgsSize = _Console->m_pVecArgs->size();
		for (int idx = 1; idx < nArgsSize; idx++) {
			_Console->Printf("%s%c", _Console->m_pVecArgs->at(idx), idx == nArgsSize - 1 ? '\n' : ' ');
		}
	}));
	m_pConsole->RegisterCommand(Q_new(CVirtualConCmd)("clear", [](CVirtualConsole* console) {
		console->Clear();
	}));
	m_pConsole->RegisterCommand(Q_new(CVirtualConCmd)("cls", [](CVirtualConsole* console) {
		console->Clear();
	}));
	m_pConsole->RegisterCommand(Q_new(CVirtualConCmd)("resetvar", [](CVirtualConsole* _Console) {
		if (_Console->Cmd_Argc() < 2) {
			_Console->Printf("[info] Usage: resetvar <convar> - Reset a variable's value; reverts it to default one.\n");

			return;
		}

		const char* szConVarName = _Console->Cmd_Argv(1);
		for (int idx = 0; idx < _Console->m_pVecConVars->size(); idx++) {
			CVirtualConVar* lpConVar = _Console->m_pVecConVars->at(idx);
			if (!_strnicmp(lpConVar->m_pszName, szConVarName, strlen(szConVarName))) {
				lpConVar->DirectSet(lpConVar->m_pszDefaultValue, true);
				_Console->Printf("resetvar: successfully reset \"%s\" convar value to \"%s\".\n", lpConVar->m_pszName, lpConVar->m_pszDefaultValue);

				return;
			}
		}

		_Console->Printf("[error] resetvar: no convar found with name \"%s\"\n", szConVarName);
	}));
	m_pConsole->RegisterCommand(Q_new(CVirtualConCmd)("reload_clickgui", [](CVirtualConsole* console) {
		CClickGUIModule* clickGui = CCheat::GetCheat()->m_pModuleManager->GetModule<CClickGUIModule>();
		Q_delete(clickGui->m_pClickGUI);
		clickGui->m_pClickGUI = Q_new(CClickGUI)();
	}));
	m_pConsole->RegisterCommand(new CVirtualConCmd("add_notification", [](CVirtualConsole* _Console) {
		if (_Console->Cmd_Argc() < 5) {
			_Console->Printf("[info] Usage: add_notification <text> <text2> <text3> <duration>\n");

			return;
		}

		const char* szTextOne = _Console->Cmd_Argv(1);
		const char* szTextTwo = _Console->Cmd_Argv(2);
		const char* szTextThree = _Console->Cmd_Argv(3);
		const char* szDuration = _Console->Cmd_Argv(4);
		float flDuration = atof(szDuration);
		CCheat::GetCheat()->m_pNotifications->PushNotification(szTextOne, szTextTwo, szTextThree, flDuration);
	}));
	m_pConsole->RegisterCommand(new CVirtualConCmd("as_reloadplugins", [](CVirtualConsole* _Console) {
		g_bForceImGuiToNotDrawAnything = true;
		CCheat* pCheat = CCheat::GetCheat();
		CModuleManager* pModuleManager = pCheat->m_pModuleManager;
		CModule** pBegin = pModuleManager->m_pVecScriptModules->begin();
		for (pBegin; pBegin != pModuleManager->m_pVecScriptModules->end(); pBegin++) {
			if (!pBegin || !*pBegin) continue;
			if ((*pBegin)->m_bState == true)
				(*pBegin)->Toggle();
			pCheat->m_pEventBus->UnregisterListener(*pBegin);

			CValue** pValues = (*pBegin)->m_pVecValues->begin();
			for (pValues; pValues != (*pBegin)->m_pVecValues->end(); pValues++) {
				free((void*) (*pValues)->m_pszConvarName);
				free((void*) (*pValues)->m_pszName);
			}
			Q_delete((*pBegin)->m_pVecValues);

			free((void*) (*pBegin)->m_pszName); //Since we're doing strdup for name and category, we need to free them.
			free((void*) (*pBegin)->m_pszCategory);
			Q_delete(*pBegin);
		}
		pModuleManager->m_pVecScriptModules->clear();
		//Q_delete(pModuleManager->m_pVecScriptModules);
		//pModuleManager->m_pVecScriptModules = Q_new(CVector<CModule*>)({});
		pCheat->m_pTheScripts->Reload();
		pCheat->LoadUpScripts();
		CClickGUIModule* pClickGUI = pModuleManager->GetModule<CClickGUIModule>();
		if (pClickGUI) {
			if (pClickGUI->m_pClickGUI) {
				pClickGUI->m_pClickGUI->InitialisePanels();
			}
		}
		g_bForceImGuiToNotDrawAnything = false;
	}));
	CHooks::Initialise();
	CEngineHooks::Initialize();
	CClientHooks::Initialize();
	m_pLocalPlayer = Q_new(CEntityPlayerSP)();
	CCommandHooks::Initialise();
	CProtocolHooks::Initialise();
	CPacketLogger::Initialise();
	m_pTheScripts->Initialise();
	if (g_pEngfuncs) {
		g_pEngfuncs->pfnAddCommand("sc_lightning_record_movements", []() {
			if (g_pEngfuncs->Cmd_Argc() < 2) {
				g_pEngfuncs->Con_Printf("Not enough args\n");
				return;
			}

			CCheat::GetCheat()->m_pMovementRecorder->StartRecording(g_pEngfuncs->Cmd_Argv(1));
		});
		g_pEngfuncs->pfnAddCommand("sc_lightning_stop_recording", []() {
			CCheat::GetCheat()->m_pMovementRecorder->StopRecording();
		});
		g_pEngfuncs->pfnAddCommand("sc_lightning_start_playback", []() {
			if (g_pEngfuncs->Cmd_Argc() < 2) {
				g_pEngfuncs->Con_Printf("Not enough args\n");
				return;
			}

			CCheat::GetCheat()->m_pMovementRecorder->StartPlayback(g_pEngfuncs->Cmd_Argv(1));
		});
		g_pEngfuncs->pfnAddCommand("sc_lightning_stop_playback", []() {
			CCheat::GetCheat()->m_pMovementRecorder->StopPlayback();
		});
	}

	bool bScriptsFileExisted = false;

	if (!CFileMgr::DoesFileExist("./lightning.tech/scripts/scripts.ini"))
		CFileMgr::CloseFile(CFileMgr::OpenFile("./lightning.tech/scripts/scripts.ini", "a+"));
	else
		bScriptsFileExisted = true;

	m_pTheScriptsMgr = Q_new(CConfigMgr)("./lightning.tech/scripts/scripts.ini");
	m_pTheScriptsMgr->m_bExistedOnLaunch = bScriptsFileExisted;

	MH_STATUS status = MH_CreateHook(ORIG_wglSwapBuffers, HOOKED_wglSwapBuffers, reinterpret_cast<void**>(&ORIG_wglSwapBuffers));
	if (status == MH_OK) {
		status = MH_CreateHook(GetMessageA, HOOKED_GetMessageA, reinterpret_cast<void**>(&ORIG_GetMessageA));
		if (status != MH_OK) {
			printf("Couldn't set hook for GetMessageA: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(GetMessageW, HOOKED_GetMessageW, reinterpret_cast<void**>(&ORIG_GetMessageW));
		if (status != MH_OK) {
			printf("Couldn't set hook for GetMessageW: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(PeekMessageA, HOOKED_PeekMessageA, reinterpret_cast<void**>(&ORIG_PeekMessageA));
		if (status != MH_OK) {
			printf("Couldn't set hook for PeekMessageA: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(PeekMessageW, HOOKED_PeekMessageW, reinterpret_cast<void**>(&ORIG_PeekMessageW));
		if (status != MH_OK) {
			printf("Couldn't set hook for PeekMessageW: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(PostMessageA, HOOKED_PostMessageA, reinterpret_cast<void**>(&ORIG_PostMessageA));
		if (status != MH_OK) {
			printf("Couldn't set hook for PostMessageA: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(PostMessageW, HOOKED_PostMessageW, reinterpret_cast<void**>(&ORIG_PostMessageW));
		if (status != MH_OK) {
			printf("Couldn't set hook for PostMessageW: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(ClipCursor, HOOKED_ClipCursor, reinterpret_cast<void**>(&ORIG_ClipCursor));
		if (status != MH_OK) {
			printf("Couldn't set hook for ClipCursor: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(SetCursorPos, HOOKED_SetCursorPos, reinterpret_cast<void**>(&ORIG_SetCursorPos));
		if (status != MH_OK) {
			printf("Couldn't set hook for SetCursorPos: %s\n", MH_StatusToString(status));
		}
		status = MH_CreateHook(GetCursorPos, HOOKED_GetCursorPos, reinterpret_cast<void**>(&ORIG_GetCursorPos));
		if (status != MH_OK) {
			printf("Couldn't set hook for GetCursorPos: %s\n", MH_StatusToString(status));
		}
		status = MH_EnableHook(MH_ALL_HOOKS);
		if (status == MH_OK) printf("[DEBUG] Successfully placed all the needed hooks!\n");
		if (status != MH_OK) {
			printf("Couldn't enable hooks: %s\n", MH_StatusToString(status));
		}
	} else {
		printf("Couldn't create hook for SwapBuffers: %s\n", MH_StatusToString(status));
	}
}

void CCheat::MainLoop() {
	m_pEventBus->Post(new CTickEvent());
}