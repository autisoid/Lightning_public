/**
 * Copyright - xWhitey, 2025.
 * int_packet_dump.cpp - packet logging
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "int_packet_dump.hpp"
#include "protocol.hpp"

decltype(CPacketLogger::ms_pfnCL_ParseServerMessage) CPacketLogger::ms_pfnCL_ParseServerMessage = nullptr;
decltype(CPacketLogger::ms_pfnSV_ExecuteClientMessage) CPacketLogger::ms_pfnSV_ExecuteClientMessage = nullptr;
cvar_t* CPacketLogger::ms_pConVarDoClientLogging = nullptr;
cvar_t* CPacketLogger::ms_pConVarDoServerLogging = nullptr;

const char* g_rgpszS2CMessages[] = {
	"svc_bad",
	"svc_nop",
	"svc_disconnect",
	"svc_event",
	"svc_version",
	"svc_setview",
	"svc_sound",
	"svc_time",
	"svc_print",
	"svc_stufftext",
	"svc_setangle",
	"svc_serverinfo",
	"svc_lightstyle",
	"svc_updateuserinfo",
	"svc_deltadescription",
	"svc_clientdata",
	"svc_stopsound",
	"svc_pings",
	"svc_particle",
	"svc_weaponanimex",
	"svc_spawnstatic",
	"svc_event_reliable",
	"svc_spawnbaseline",
	"svc_temp_entity",
	"svc_setpause",
	"svc_signonnum",
	"svc_centerprint",
	"svc_killedmonster",
	"svc_eventlist",
	"svc_spawnstaticsound",
	"svc_intermission",
	"svc_finale",
	"svc_cdtrack",
	"svc_restore",
	"svc_cutscene",
	"svc_weaponanim",
	"svc_decalname",
	"svc_roomtype",
	"svc_addangle",
	"svc_newusermsg",
	"svc_packetentities",
	"svc_deltapacketentities",
	"svc_choke",
	"svc_resourcelist",
	"svc_newmovevars",
	"svc_resourcerequest",
	"svc_customization",
	"svc_crosshairangle",
	"svc_soundfade",
	"svc_filetxferfailed",
	"svc_hltv",
	"svc_director",
	"svc_voiceinit",
	"svc_voicedata",
	"svc_sendextrainfo",
	"svc_timescale",
	"svc_resourcelocation",
	"svc_sendcvarvalue",
	"svc_sendcvarvalue2",
	"End of List",
};

const char* g_rgpszC2SMessages[] = {
	"clc_bad",
	"clc_nop",
	"clc_move",
	"clc_stringcmd",
	"clc_delta",
	"clc_resourcelist",
	"clc_tmove",
	"clc_fileconsistency",
	"clc_voicedata",
	"clc_hltv",
	"clc_cvarvalue",
	"clc_cvarvalue2",
	"End of List",
};

void __cdecl CL_ParseServerMessage_Gateway() {
	if (!CPacketLogger::ms_pConVarDoClientLogging)
		return;

	if ((int) CPacketLogger::ms_pConVarDoClientLogging->value != 1)
		return;

	CProtocolHooks::SaveReadCount();

	int nMessageNum = CProtocolHooks::_ms_pfnMSG_ReadByte();
	if (nMessageNum == -1) {
		CProtocolHooks::RestoreReadCount();

		return;
	}

	if (nMessageNum < (Q_ARRAYSIZE(g_rgpszS2CMessages) - 1)) {
		g_pEngfuncs->Con_Printf("[Lightning] Server -> Client Message received: %s\n", g_rgpszS2CMessages[nMessageNum]);
	}

	CProtocolHooks::RestoreReadCount();
}

void __declspec(naked) __cdecl CPacketLogger::CL_ParseServerMessage_Hook() {
	__asm {
		pushfd
		pushad
		call CL_ParseServerMessage_Gateway
		popad
		popfd
		jmp ms_pfnCL_ParseServerMessage
	}
}

void __cdecl SV_ExecuteClientMessage_Gateway() {
	if (!CPacketLogger::ms_pConVarDoServerLogging)
		return;

	if ((int)CPacketLogger::ms_pConVarDoServerLogging->value != 1)
		return;

	CProtocolHooks::SaveReadCount();

	int nMessageNum = CProtocolHooks::_ms_pfnMSG_ReadByte();
	if (nMessageNum == -1) {
		CProtocolHooks::RestoreReadCount();

		return;
	}

	if (nMessageNum < (Q_ARRAYSIZE(g_rgpszC2SMessages) - 1)) {
		g_pEngfuncs->Con_Printf("[Lightning] Client -> Server Message received: %s\n", g_rgpszC2SMessages[nMessageNum]);
	}

	CProtocolHooks::RestoreReadCount();
}

void __declspec(naked) __cdecl CPacketLogger::SV_ExecuteClientMessage_Hook() {
	__asm {
		pushfd
		pushad
		call SV_ExecuteClientMessage_Gateway
		popad
		popfd
		jmp ms_pfnSV_ExecuteClientMessage
	}
}

void CPacketLogger::Initialise() {
	if (!g_pEngfuncs) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CPacketLogger::Initialise(): g_pEngfuncs was nullptr\n");

		return;
	}

	MH_STATUS eStatus;

	if (FindAsync(g_pHwDll, patterns::engine::CL_ParseServerMessage_call_MSG_ReadByte, ms_pfnCL_ParseServerMessage)) {
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found CL_ParseServerMessage's call insn at 0x%p\n", ms_pfnCL_ParseServerMessage);

		eStatus = MH_CreateHook(ms_pfnCL_ParseServerMessage, CPacketLogger::CL_ParseServerMessage_Hook, reinterpret_cast<void**>(&ms_pfnCL_ParseServerMessage));

		if (eStatus == MH_OK) {
			eStatus = MH_EnableHook(MH_ALL_HOOKS);
			if (eStatus == MH_OK) {
				ms_pConVarDoClientLogging = g_pEngfuncs->pfnRegisterVariable("sc_lightning_cl_log_incoming_packets", "0", 0);
			} else {
				CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CPacketLogger::Initialise(): Failure enabling the hook for CL_ParseServerMessage's call insn: %s\n", MH_StatusToString(eStatus));
			}
		} else {
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CPacketLogger::Initialise(): Failure creating a hook for CL_ParseServerMessage's call insn: %s\n", MH_StatusToString(eStatus));
		}
	}

	if (FindAsync(g_pHwDll, patterns::engine::SV_ExecuteClientMessage_call_MSG_ReadByte, ms_pfnSV_ExecuteClientMessage)) {
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found SV_ExecuteClientMessage's call insn at 0x%p\n", ms_pfnSV_ExecuteClientMessage);

		eStatus = MH_CreateHook(ms_pfnSV_ExecuteClientMessage, CPacketLogger::SV_ExecuteClientMessage_Hook, reinterpret_cast<void**>(&ms_pfnSV_ExecuteClientMessage));

		if (eStatus == MH_OK) {
			eStatus = MH_EnableHook(MH_ALL_HOOKS);
			if (eStatus == MH_OK) {
				ms_pConVarDoServerLogging = g_pEngfuncs->pfnRegisterVariable("sc_lightning_sv_log_incoming_packets", "0", 0);
			}
			else {
				CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CPacketLogger::Initialise(): Failure enabling the hook for SV_ExecuteClientMessage's call insn: %s\n", MH_StatusToString(eStatus));
			}
		}
		else {
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CPacketLogger::Initialise(): Failure creating a hook for SV_ExecuteClientMessage's call insn: %s\n", MH_StatusToString(eStatus));
		}
	}
}