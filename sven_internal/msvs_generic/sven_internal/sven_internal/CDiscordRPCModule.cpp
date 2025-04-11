/**
 * Copyright - xWhitey, 2024.
 * CDiscordRPCModule.cpp - discord rpc hooks
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CDiscordRPCModule.hpp"

struct CDiscordRPCModule* g_pDiscordRPCModule;

extern "C" {
	typedef struct DiscordRichPresence {
		const char* state;   /* max 128 bytes */
		const char* details; /* max 128 bytes */
		int64_t startTimestamp;
		int64_t endTimestamp;
		const char* largeImageKey;  /* max 32 bytes */
		const char* largeImageText; /* max 128 bytes */
		const char* smallImageKey;  /* max 32 bytes */
		const char* smallImageText; /* max 128 bytes */
		const char* partyId;        /* max 128 bytes */
		int partySize;
		int partyMax;
		int partyPrivacy;
		const char* matchSecret;    /* max 128 bytes */
		const char* joinSecret;     /* max 128 bytes */
		const char* spectateSecret; /* max 128 bytes */
		int8_t instance;
	} DiscordRichPresence;

	typedef struct DiscordUser {
		const char* userId;
		const char* username;
		const char* discriminator;
		const char* avatar;
	} DiscordUser;

	typedef struct DiscordEventHandlers {
		void(*ready)(const DiscordUser* request);
		void(*disconnected)(int errorCode, const char* message);
		void(*errored)(int errorCode, const char* message);
		void(*joinGame)(const char* joinSecret);
		void(*spectateGame)(const char* spectateSecret);
		void(*joinRequest)(const DiscordUser* request);
	} DiscordEventHandlers;

	void (*Discord_Initialize)(const char* applicationId,
		DiscordEventHandlers* handlers,
		int autoRegister,
		const char* optionalSteamId);

	void(*Discord_Shutdown)(void);

	void (*Discord_UpdatePresence)(const DiscordRichPresence* presence);

	decltype(Discord_UpdatePresence) ORIG_Discord_UpdatePresence;
}

CDiscordRPCModule::CDiscordRPCModule() : CModule("DiscordRPC", "Designer") {
	g_pDiscordRPCModule = this;

	DefineConCmd("sc_lightning_discordrpc", CDiscordRPCModule);
}

void Discord_Ready(const DiscordUser* request) {
	CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CDiscordRPCModule: ready!\n");
}

void Discord_Error(int errorCode, const char* message) {
	CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CDiscordRPCModule: error: %s\n", message);
}

void Discord_Disconnected(int errorCode, const char* message) {
	CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CDiscordRPCModule: disconnected: %s\n", message);
}

void Discord_JoinGame(const char* joinSecret) {}
void Discord_SpectateGame(const char* spectateSecret) {}
void Discord_JoinRequest(const DiscordUser* request) {}

void __cdecl HOOKED_Discord_UpdatePresence(const DiscordRichPresence* presence) {
	if (!presence) return;

	//if (strcmp(presence->state, g_pDiscordRPCModule->m_pTextToDisplay->Get())) return;
	//if (strcmp(presence->details, g_pDiscordRPCModule->m_pDetails->Get())) return;

	return ORIG_Discord_UpdatePresence(presence);
}

void CDiscordRPCModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);

	m_tStartTime = time(NULL);

	m_hDiscordRPC = GetModuleHandleA("discord-rpc");
	if (m_hDiscordRPC) {
		Discord_Initialize = (decltype(Discord_Initialize))(GetProcAddress(m_hDiscordRPC, "Discord_Initialize"));
		if (Discord_Initialize) {
			DiscordEventHandlers handlers;
			memset(&handlers, 0, sizeof(handlers));
			handlers.ready = Discord_Ready;
			handlers.errored = Discord_Error;
			handlers.disconnected = Discord_Disconnected;
			handlers.joinGame = Discord_JoinGame;
			handlers.spectateGame = Discord_SpectateGame;
			handlers.joinRequest = Discord_JoinRequest;

			Discord_Initialize("1271088812109598750", &handlers, 0, NULL); //"allah client" application :D
		}
	}
}

void CDiscordRPCModule::OnDisable() {
	if (m_hDiscordRPC) {
		Discord_Shutdown = (decltype(Discord_Shutdown))(GetProcAddress(m_hDiscordRPC, "Discord_Shutdown"));
		if (Discord_Shutdown) {
			Discord_Shutdown();
		}
	}

	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CDiscordRPCModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kRenderEvent) {
		if (m_hDiscordRPC) {
			if (!Discord_UpdatePresence) {
				Discord_UpdatePresence = (decltype(Discord_UpdatePresence))(GetProcAddress(m_hDiscordRPC, "Discord_UpdatePresence"));
				if (!ORIG_Discord_UpdatePresence && Discord_UpdatePresence) {
					MH_CreateHook(Discord_UpdatePresence, HOOKED_Discord_UpdatePresence, reinterpret_cast<void**>(&ORIG_Discord_UpdatePresence));
					MH_EnableHook(MH_ALL_HOOKS);
				}
			} else {
				DiscordRichPresence discordPresence;
				memset(&discordPresence, 0, sizeof(discordPresence));
                //FIXME: 27/09/2024 (DD/MM/YYYY) - There's a problem with getting the current player state in the OG game (HL) so I left this as placeholders. - xWhitey
				discordPresence.state = "In the main menu";
				discordPresence.details = "Doing nothing";
				discordPresence.startTimestamp = m_tStartTime;
				discordPresence.largeImageKey = "logo";
				Discord_UpdatePresence(&discordPresence);
			}
		}
	}
}