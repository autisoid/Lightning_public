/**
 * Copyright - xWhitey, 2025.
 * int_packet_dump.hpp - packet logging
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef INT_PACKET_DUMP_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in int_packet_dump.hpp
#else //INT_PACKET_DUMP_HPP_RECURSE_GUARD

#define INT_PACKET_DUMP_HPP_RECURSE_GUARD

#ifndef INT_PACKET_DUMP_HPP_GUARD
#define INT_PACKET_DUMP_HPP_GUARD
#pragma once

#ifdef __cplusplus

//Used entirely by me to packetlog data incoming from the server, and outgoing from the client when needed. (Was used to fix rehlds for sven)
typedef struct CPacketLogger {
private:
	typedef void(__cdecl* _CL_ParseServerMessage)(qboolean normal_message);
	typedef void(__cdecl* _SV_ExecuteClientMessage)(void* /* client_t* */ cl);
public:
	static void Initialise();
	static void TryInitialisingServerSideLogger();
	static void __cdecl CL_ParseServerMessage_Hook(/*qboolean _NormalMessage*/);
	static void __cdecl SV_ExecuteClientMessage_Hook(/*void* _Client*/);

	static _CL_ParseServerMessage ms_pfnCL_ParseServerMessage;
	static _SV_ExecuteClientMessage ms_pfnSV_ExecuteClientMessage;

	static cvar_t* ms_pConVarDoClientLogging;
	static cvar_t* ms_pConVarDoServerLogging;
} CPacketLogger;

using CPacketLogger = struct CPacketLogger;

#else //!__cplusplus
#error C++ compiler required to compile int_packet_dump.hpp
#endif //__cplusplus

#endif //INT_PACKET_DUMP_HPP_GUARD

#undef INT_PACKET_DUMP_HPP_RECURSE_GUARD
#endif //INT_PACKET_DUMP_HPP_RECURSE_GUARD