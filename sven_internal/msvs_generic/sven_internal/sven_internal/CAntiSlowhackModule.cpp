/**
 * Copyright - xWhitey, 2024.
 * CAntiSlowhackModule.cpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CAntiSlowhackModule.hpp"

//Look into protocol.hpp and protocol.cpp for the implementation.

CAntiSlowhackModule::CAntiSlowhackModule() : CModule("AntiSlowhack", "Safety", 0x0, false) {
	m_pVecValues->push_back(m_pConfigureWnd = Q_new(CExtraValues)("Help", "sc_antislowhack_help", [](void* _UserData) {
		ImGui::Text("Additional help\n");
		ImGui::Text("This module can block commands which are sent by the server you're playing on, but blocking EVERY command is impossible: some of them are required to maintain the gameplay.");
		ImGui::Text("You can specify your own whitelist of allowed commands, but the \"special\" commands which cannot be really blocked are there by default and cannot be removed.");
		ImGui::Text("List of the \"special commands\" as follows: any npc_* command, fullserverinfo, reconnect, retry, stopsound, cl_stopsound, spk, speak, play, mp3.\n");
		ImGui::Text("For safety, we block any gl_* command, any cl_* command, demos, gg, rcon, cmd *, exit, quit, setinfo, name and any r_* commands.");
		ImGui::Text("Notify players checkbox determines whether this module should send a chat message about the slowhack incident, so other players know about that.");
	}, this));
	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_antislowhack_mode", "Block everything, except gameplay important commands\0Whitelist\0\0", 1));
	m_pVecValues->push_back(m_pWhitelist = Q_new(CTextValue)("Whitelist", "sc_antislowhack_whitelist", "speak,spk,play,mp3,stopsound,cl_stopsound"));
	m_pVecValues->push_back(m_pNotifyPlayers = Q_new(CBoolValue)("Notify players", "sc_antislowhack_notify_players", true));
	m_pVecValues->push_back(m_pNotificationText = Q_new(CTextValue)("Notify players - Notification text", "sc_antislowhack_notification_text", "Whoosh! Seems like this server tried to slowhack me. ~ lightning.tech"));

	//Toggle(); //do NOT call Toggle at least in module constructor as this will INSTANTLY override modules.ini
	if (!m_bState)
		CModule::SetState(true, true);
}

void CAntiSlowhackModule::OnEnable() {
}

void CAntiSlowhackModule::OnDisable() {
	Toggle(); //it is PROBABLY ok to call Toggle here
}

void CAntiSlowhackModule::OnEvent(_In_ const ISimpleEvent* _Event) {}