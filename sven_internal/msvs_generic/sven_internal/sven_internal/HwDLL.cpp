#include "StdAfx.h"
#include "HwDLL.hpp"
#include "CFrameSkipperModule.hpp"
 
void __cdecl HOOKED_SPR_Set(HSPRITE_HL _Sprite, int _Red, int _Green, int _Blue) {
	if (!sc_game_hud_color_same_as_client_color || sc_game_hud_color_same_as_client_color->value == 0) return ORIG_SPR_Set(_Sprite, _Red, _Green, _Blue);

	return ORIG_SPR_Set(_Sprite, (int) (g_DefaultClientColor.Value.x * 255.f), (int)(g_DefaultClientColor.Value.y * 255.f), (int)(g_DefaultClientColor.Value.z * 255.f));
}

void __cdecl HOOKED_Draw_FillRGBA(int _X, int _Y, int _Width, int _Height, int _Red, int _Green, int _Blue, int _Alpha) {
	if (!sc_game_hud_color_same_as_client_color || sc_game_hud_color_same_as_client_color->value == 0) return ORIG_Draw_FillRGBA(_X, _Y, _Width, _Height, _Red, _Green, _Blue, _Alpha);

	return ORIG_Draw_FillRGBA(_X, _Y, _Width, _Height, (int)(g_DefaultClientColor.Value.x * 255.f), (int)(g_DefaultClientColor.Value.y * 255.f), (int)(g_DefaultClientColor.Value.z * 255.f), (int)(g_DefaultClientColor.Value.w * 255.f));
}

void __cdecl HOOKED_Netchan_TransmitBits(netchan_s* _Chan, int _Size, char* _Data) {
	CPacketEvent* e = Q_new(CPacketEvent)(_Chan, _Size, _Data);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	if (e->m_bIsCancelled) {
		Q_delete(e);
		return ORIG_Netchan_TransmitBits(_Chan, 0, nullptr);
	}
	Q_delete(e);

	return ORIG_Netchan_TransmitBits(_Chan, _Size, _Data);
}

int __cdecl HOOKED_SCR_UpdateScreen() {
	static int s_nSkippedFramesCnt = 0;
	
	if (g_pFrameSkipperModule && g_pFrameSkipperModule->m_bState == true) {
		if (s_nSkippedFramesCnt <= 1) {
			s_nSkippedFramesCnt++;
		}

		if (s_nSkippedFramesCnt > 1) {
			s_nSkippedFramesCnt = 0;

			return 1;
		}
	}

	return ORIG_SCR_UpdateScreen();
}

void __cdecl HOOKED_MSG_WriteUsercmd(sizebuf_t* _Buffer, usercmd_s* _To, usercmd_s* _From) {
	CNetworkUserCommandSubmitEvent* e = Q_new(CNetworkUserCommandSubmitEvent)(_To, _From);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	if (e->m_bIsCancelled) {
		Q_delete(e);
		return;
	}
	Q_delete(e);

	return ORIG_MSG_WriteUsercmd(_Buffer, _To, _From);
}

void HOOKED_Cbuf_AddText(const char* _Text) {
	auto e = Q_new(CCommandIssuedEvent)(_Text);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	if (e->m_bIsCancelled) {
		Q_delete(e);
		return;
	}

	ORIG_Cbuf_AddText(e->m_pszText);

	auto post_e = Q_new(CPostCommandIssuedEvent)(e->m_pszText);
	CCheat::GetCheat()->m_pEventBus->Post(post_e);
	Q_delete(post_e);
	Q_delete(e);
}

int HOOKED_R_StudioDrawPlayer(int _Flags, entity_state_t* _State) {
	CStudioDrawPlayerEvent* e = Q_new(CStudioDrawPlayerEvent)(_Flags, _State);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	Q_delete(e);

	int result = ORIG_R_StudioDrawPlayer(_Flags, _State);

	CPostStudioDrawPlayerEvent* post_e = Q_new(CPostStudioDrawPlayerEvent)(_Flags, _State);
	CCheat::GetCheat()->m_pEventBus->Post(post_e);
	Q_delete(post_e);

	return result;
}

void CEngineHooks::SetupEngineCommandHook(_In_z_ const char* _CommandName, _In_ void(__cdecl* _Detour)(), _In_ void** _Original) {
	if (!g_pCmdFunctions) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CEngineHooks::SetupEngineCommandHook: attempted to create concmd hook whilst g_pCmdFunctions is null!\n");

		return;
	}

	cmd_function_t* target = nullptr;
	cmd_function_t* cmd_functions = g_pCmdFunctions;
	while (cmd_functions != nullptr) {
		if (!cmd_functions->name || cmd_functions->name[0] == '\0') continue;

		if (!strcmp(cmd_functions->name, _CommandName)) {
			target = cmd_functions;
			break;
		}
		cmd_functions = cmd_functions->next;
	}

	if (target) {
		if (_Original) {
			*_Original = target->function;
		}
		target->function = _Detour;
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CEngineHooks::SetupEngineCommandHook: successfully set up a hook for \"%s\" concmd\n", _CommandName);
	} else {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CEngineHooks::SetupEngineCommandHook: couldn't find specified cmd: \"%s\"\n", _CommandName);
	}
}

void CEngineHooks::Initialize() {
	if (g_pHwDll) {
		if (ORIG_Netchan_TransmitBits) {
			MH_STATUS status = MH_CreateHook(ORIG_Netchan_TransmitBits, HOOKED_Netchan_TransmitBits, reinterpret_cast<void**>(&ORIG_Netchan_TransmitBits));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for Netchan_TransmitBits: %s\n", MH_StatusToString(status));
			} else {
				printf("Failed to create hook for Netchan_TransmitBits: %s\n", MH_StatusToString(status));
			}
		}
		if (ORIG_SCR_UpdateScreen) {
			MH_STATUS status = MH_CreateHook(ORIG_SCR_UpdateScreen, HOOKED_SCR_UpdateScreen, reinterpret_cast<void**>(&ORIG_SCR_UpdateScreen));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for SCR_UpdateScreen: %s\n", MH_StatusToString(status));
			} else {
				printf("Failed to create hook for SCR_UpdateScreen: %s\n", MH_StatusToString(status));
			}
		}
		FindAsync(g_pHwDll, patterns::engine::Cbuf_InsertText, ORIG_Cbuf_InsertText, [](const char* _Version) {
			printf("Found Cbuf_InsertText at 0x%p.\n", ORIG_Cbuf_InsertText);
		}, [] { printf("Failed to find Cbuf_InsertText!\n");  });
		FindAsync(g_pHwDll, patterns::engine::Cbuf_AddText, ORIG_Cbuf_AddText, [](const char* _Version) {
			if (!_strnicmp(_Version, "Sven-5.26", sizeof("Sven-5.26"))) {
				g_bIsSven5_26 = true;
			}

			printf("Found Cbuf_AddText at 0x%p.\n", ORIG_Cbuf_AddText);
			MH_STATUS status = MH_CreateHook(ORIG_Cbuf_AddText, HOOKED_Cbuf_AddText, reinterpret_cast<void**>(&ORIG_Cbuf_AddText));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for Cbuf_AddText: %s\n", MH_StatusToString(status));
			} else {
				printf("Failed to create hook for Cbuf_AddText: %s\n", MH_StatusToString(status));
			}
		}, [] { printf("Failed to find Cbuf_AddText!\n");  });
		FindAsync(g_pHwDll, patterns::engine::Draw_FillRGBA, ORIG_Draw_FillRGBA, [](const char* _Version) {
			printf("Found Draw_FillRGBA at 0x%p.\n", ORIG_Draw_FillRGBA);
		}, []() { printf("Couldn't find Draw_FillRGBA! Some features won't be available.\n"); });
		FindAsync(g_pHwDll, patterns::engine::SPR_Set, ORIG_SPR_Set, [](const char* _Version) {
			printf("Found SPR_Set at 0x%p.\n", ORIG_SPR_Set);
		}, []() { printf("Couldn't find SPR_Set! Some features won't be available.\n"); });
		FindAsync(g_pHwDll, patterns::engine::Cmd_AddCommandWithFlags_mov_dword_ptr_ds_cmd_functions_eax, g_pCmdFunctions, [](const char* _Version) {
			g_pCmdFunctions = *reinterpret_cast<cmd_function_t**>(reinterpret_cast<uintptr_t>(g_pCmdFunctions) + 1);
			g_pCmdFunctions = g_pCmdFunctions->next;
			printf("Found cmd_functions at 0x%p.\n", g_pCmdFunctions);
		}, []() { printf("Couldn't find cmd_functions! Some features won't be available.\n"); });
		FindAsync(g_pHwDll, patterns::engine::MSG_WriteUsercmd, ORIG_MSG_WriteUsercmd, [](const char* _Version) {
			printf("Found MSG_WriteUsercmd at 0x%p.\n", ORIG_MSG_WriteUsercmd);
			MH_STATUS status = MH_CreateHook(ORIG_MSG_WriteUsercmd, HOOKED_MSG_WriteUsercmd, reinterpret_cast<void**>(&ORIG_MSG_WriteUsercmd));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for MSG_WriteUsercmd: %s\n", MH_StatusToString(status));
			} else {
				printf("Failed to create hook for MSG_WriteUsercmd: %s\n", MH_StatusToString(status));
			}
		}, []() { printf("Couldn't find MSG_WriteUsercmd! Some features won't be available.\n"); });
		FindAsync(g_pHwDll, patterns::engine::R_StudioDrawPlayer, ORIG_R_StudioDrawPlayer, [](const char* _Version) {
			printf("Found R_StudioDrawPlayer at 0x%p.\n", ORIG_R_StudioDrawPlayer);
			MH_STATUS status = MH_CreateHook(ORIG_R_StudioDrawPlayer, HOOKED_R_StudioDrawPlayer, reinterpret_cast<void**>(&ORIG_R_StudioDrawPlayer));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) printf("Couldn't enable hook for R_StudioDrawPlayer: %s\n", MH_StatusToString(status));
			}
			else {
				printf("Failed to create hook for R_StudioDrawPlayer: %s\n", MH_StatusToString(status));
			}
		}, []() { printf("Couldn't find R_StudioDrawPlayer! Some features won't be available.\n"); });
		if (FindAsync(g_pHwDll, patterns::engine::SCR_EndLoadingPlaque, ORIG_SCR_EndLoadingPlaque)) {
			printf("Found SCR_EndLoadingPlaque at 0x%p.\n", ORIG_SCR_EndLoadingPlaque);
		} else {
			printf("Couldn't find SCR_EndLoadingPlaque!\n");
		}
		if (g_bIsSven5_26) {
			if (g_pMoveVars = reinterpret_cast<movevars_s*>(reinterpret_cast<uintptr_t>(g_pHwDll) + 0x685B4C8)) {
				printf("Found g_pMoveVars at 0x%p (Sven-5.26)\n", g_pMoveVars);
			}
		}
		FindAsync(g_pHwDll, patterns::engine::CL_ParseClientdata_g_flLatency, g_pdblLatency, [](const char* _Version)
		{
			DWORD dwOffset = reinterpret_cast<DWORD>(g_pdblLatency) + 2;
			if (!_strnicmp(_Version, "Sven-5.26", sizeof("Sven-5.26") - 1)) {
				g_pdblLatency = *reinterpret_cast<double**>(dwOffset);
			} else {
				g_pdblLatency = reinterpret_cast<double*>(dwOffset);
			}
			printf("Found g_flLatency at 0x%p\n", g_pdblLatency);
		}, []()
		{
			printf("Couldn't find g_flLatency!\n");
		});
		if (ORIG_SPR_Set && ORIG_Draw_FillRGBA) {
			MH_STATUS status = MH_CreateHook(ORIG_SPR_Set, HOOKED_SPR_Set, reinterpret_cast<void**>(&ORIG_SPR_Set));
			if (status == MH_OK) {
				status = MH_EnableHook(MH_ALL_HOOKS);
				if (status != MH_OK) {
					printf("Couldn't enable hook for SPR_Set: %s\n", MH_StatusToString(status));
				} else if (status == MH_OK) {
					status = MH_CreateHook(ORIG_Draw_FillRGBA, HOOKED_Draw_FillRGBA, reinterpret_cast<void**>(&ORIG_Draw_FillRGBA));
					if (status == MH_OK) {
						status = MH_EnableHook(MH_ALL_HOOKS);
						if (status != MH_OK) {
							printf("Couldn't enable hook for Draw_FillRGBA: %s\n", MH_StatusToString(status));
						}
					} else {
						printf("Failed to create hook for Draw_FillRGBA: %s\n", MH_StatusToString(status));
					}
				}
			} else {
				printf("Failed to create hook for SPR_Set: %s\n", MH_StatusToString(status));
			}
		}
	}
}