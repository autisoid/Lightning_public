/*
	Lightning (Sven Co-op multihack) source file
	Authors: xWhitey. Refer to common.hpp for dependencies and their authors.
	Do not delete this comment block. Respect others' work!
*/

#include "StdAfx.h"

void* ImGuiWrapper_MemAlloc(functional_size_t _Size, void* _UserData) {
	return Q_malloc(_Size);
}

void ImGuiWrapper_MemFree(void* _Pointer, void* _UserData) {
	Q_free(_Pointer);
}

void Main() {
	bool bHasGlobalConfigFileExistedOnLaunch = CFileMgr::DoesFileExist("./lightning.tech/internals.ini");
	bool bSpawnDebugConsole = false;

	g_pGlobalInternals = Q_new(CConfigMgr)("./lightning.tech/internals.ini");
	g_pGlobalInternals->m_bExistedOnLaunch = bHasGlobalConfigFileExistedOnLaunch;

	if (bHasGlobalConfigFileExistedOnLaunch) {
		CSimpleIniA::TNamesDepend values;
		g_pGlobalInternals->m_pIni->GetAllKeys("Internals", values);
		CSimpleIniA::TNamesDepend::const_iterator it;
		for (it = values.begin(); it != values.end(); ++it) {
			const char* pszInternal = it->pItem;
			const char* pszInternalValue = g_pGlobalInternals->GetValue("Internals", pszInternal);
			if (!Q_strncmp_s(pszInternal, Q_strlen(pszInternal) + 1, "bSpawnDebugConsole", sizeof("bSpawnDebugConsole") - 1)) {
				if (!Q_strnicmp_s(pszInternalValue, Q_strlen(pszInternalValue) + 1, "true", sizeof("true") - 1)) {
					bSpawnDebugConsole = true;
				}
			}
		}
	} else {
		g_pGlobalInternals->SetValue("Internals", "bSpawnDebugConsole", "false");
		g_pGlobalInternals->Save();
	}

	if (bSpawnDebugConsole) {
		AllocConsole();

		freopen_s(&g_pfStdInput, "conin$", "r", stdin);
		freopen_s(&g_pfStdOutput, "conout$", "w+", stdout);
	}

	ImGui::SetAllocatorFunctions(ImGuiWrapper_MemAlloc, ImGuiWrapper_MemFree);

	if (bSpawnDebugConsole) {
		SetConsoleTitleA("lightning.tech ~ get fascinated! ~ xWhitey");
	}

	MH_STATUS eStatus = MH_Initialize();

	printf("51squad. (Version: %s)\nTrying to initialize MinHook... ", CCheat::ms_pszVersion);
	if (eStatus == MH_OK) {
		printf("Success!\n");
	} else {
		printf("Failure: %s\n", MH_StatusToString(eStatus));
	}
	
	if (g_pHwDll = GetModuleHandleA("hw")) {
		FindAsync(g_pHwDll, patterns::engine::Netchan_TransmitBits, ORIG_Netchan_TransmitBits, [](const char* _Version) {
			printf("Found Netchan_TransmitBits at 0x%p.\n", ORIG_Netchan_TransmitBits);
		}, []() { printf("Couldn't find Netchan_TransmitBits! Some features won't be available.\n"); });
		FindAsync(g_pHwDll, patterns::engine::SCR_UpdateScreen, ORIG_SCR_UpdateScreen, [](const char* _Version) {
			printf("Found SCR_UpdateScreen at 0x%p.\n", ORIG_SCR_UpdateScreen);
		}, []() { printf("Couldn't find SCR_UpdateScreen! Some features won't be available.\n"); });
		FindAsync(g_pHwDll, patterns::engine::Cmd_Messagemode2_f_mov_dword_ptr_ds_key_dest, g_piKeyDest, [](const char* _Version) {
			g_piKeyDest = *reinterpret_cast<int**>(reinterpret_cast<DWORD>(g_piKeyDest) + 2);
			printf("Found key_dest at 0x%p. Current key_dest is %d.\n", g_piKeyDest, *g_piKeyDest);
		}, []() { printf("Couldn't find key_dest! Some features may be broken.\n"); });
	}
	if (g_pClientDll = GetModuleHandleA("client")) {
		g_pEngfuncs = reinterpret_cast<cl_enginefunc_t*>(GetProcAddress(reinterpret_cast<HMODULE>(g_pClientDll), "gEngfuncs"));
		//^^^^^ Why is this here? I don't know. Probably because this whole thing was pasted from BXT and BXT uses dlsym to see if gEngfuncs is exported.

		if (g_pEngfuncs)
			g_pEngfuncs->Con_Printf("[client dll] pEngfuncs is %p.\n", g_pEngfuncs);
		else {
			FARPROC pInitialize = GetProcAddress(reinterpret_cast<HMODULE>(g_pClientDll), "Initialize");

			if (pInitialize) {
				printf("[client dll] Found Initialize at %p.\n", pInitialize);

				FindAsync(g_pClientDll, patterns::client::Initialize_mov_edi_g_pEngfuncs, g_pEngfuncs, [](const char* _Version) {
					if (!_strnicmp(_Version, "HL-Compat", sizeof("HL-Compat") - 1)) {
						g_pEngfuncs = reinterpret_cast<cl_enginefunc_t*>(*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(g_pEngfuncs) + 2));
					} else {
						g_pEngfuncs = *reinterpret_cast<cl_enginefunc_t**>(((char*)(g_pEngfuncs)) + 1);
					}

					MEMORY_BASIC_INFORMATION mbi;
					VirtualQuery(g_pEngfuncs, &mbi, sizeof(mbi));
					if ((mbi.AllocationProtect & PAGE_READWRITE) == 0) {
						//Unprotect it NOW because we will crash later if we try to access it.
						DWORD dwOld; //Maybe save that to a global variable to we can restore that later?..
						if (!VirtualProtect(g_pEngfuncs, sizeof(cl_enginefunc_t), PAGE_READWRITE, &dwOld)) {
							printf("[client dll] Failure unprotecting g_pEngfuncs! Expect big troubles...\n");
						}
						//We set PAGE_READWRITE and not PAGE_EXECUTE_READWRITE because this can lead to RCE's.
						//Actually we can just specify "1" as dwSize because the whole page protection will be changed anyway, with any size specified.
					}

					printf("[client dll] g_pEngfuncs is %p.\n", g_pEngfuncs);
				}, []() { printf("[client dll] Couldn't find g_pEngfuncs. Some features will be unavailable.\n"); });
			} else {
				printf("[client dll] Couldn't get the address of Initialize.\nCustom HUD is unavailable.\nClient-side logging is unavailable.\n");
			}
		}

		static CCheat* cheat = Q_new(CCheat)();
		cheat->SetupHooks();
	} else {
		printf("Couldn't find client.dll module!\n");
	}
}

DWORD WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void** _Unused) {
	if (_Reason == DLL_PROCESS_ATTACH) {
		g_pSelfHandle = _DllHandle;

		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), NULL, NULL, NULL);

		return 1;
	}
	
	return 0;
}