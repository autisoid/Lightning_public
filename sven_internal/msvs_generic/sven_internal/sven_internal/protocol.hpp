/**
 * Copyright - xWhitey, 2024.
 * protocol.hpp - hookin' proto messages here haha
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef PROTOCOL_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in protocol.hpp
#else //PROTOCOL_HPP_RECURSE_GUARD

#define PROTOCOL_HPP_RECURSE_GUARD

#ifndef PROTOCOL_HPP_GUARD
#define PROTOCOL_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CProtocolHooks {
	static void Initialise();
	static void StuffText();
	static void Director();
	static void Disconnect();
	static void ServerInfo();
	static void SignonNum();
	static CVector<char*>* SanitizeCommand(_In_z_ const char* _Command);
	static bool IsCommandDangerous(_In_z_ const char* _Command);
	static bool IsCommandInPredefinedAllowedList(_In_z_ const char* _Command);

	static void SaveReadCount();
	static void RestoreReadCount();
private:
	typedef void(__cdecl* _m_tProtocolMessage)();
	typedef const char*(__cdecl* _m_tMSG_ReadString)();
	typedef int(__cdecl* _m_tMSG_ReadByte)();
	typedef int(__cdecl* _m_tMSG_ReadLong)();

	static int _ms_iSavedReadCount;
	static int* _ms_piReadCount;
	static _m_tProtocolMessage _ms_pfnStuffText;
	static _m_tProtocolMessage _ms_pfnDirector;
	static _m_tProtocolMessage _ms_pfnDisconnect;
	static _m_tProtocolMessage _ms_pfnServerInfo;
	static _m_tProtocolMessage _ms_pfnSignonNum;
	static const char* _ms_rglpszPredefinedAllowedList[];
	static cvar_t* _ms_pConVarIgnoreDisconnect;
	static cvar_t* _ms_pConVarIgnoreInvalidSignon;

public:
	//MUST BE PRIVATE!!! Made public for int_packet_dump.
	static _m_tMSG_ReadString _ms_pfnMSG_ReadString;
	static _m_tMSG_ReadByte _ms_pfnMSG_ReadByte;
	static _m_tMSG_ReadLong _ms_pfnMSG_ReadLong;
} CProtocolHooks;

using CProtocolHooks = struct CProtocolHooks;

#else //!__cplusplus
#error C++ compiler required to compile protocol.hpp
#endif //__cplusplus

#endif //PROTOCOL_HPP_GUARD

#undef PROTOCOL_HPP_RECURSE_GUARD
#endif //PROTOCOL_HPP_RECURSE_GUARD