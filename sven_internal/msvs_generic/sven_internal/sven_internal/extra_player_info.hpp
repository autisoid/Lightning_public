/**
 * Copyright - xWhitey, 2024.
 * extra_player_info.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: Sw1ft747, xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef EXTRA_PLAYER_INFO_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in extra_player_info.hpp
#else //EXTRA_PLAYER_INFO_HPP_RECURSE_GUARD

#define EXTRA_PLAYER_INFO_HPP_RECURSE_GUARD

#ifndef EXTRA_PLAYER_INFO_HPP_GUARD
#define EXTRA_PLAYER_INFO_HPP_GUARD
#pragma once

#ifdef __cplusplus

//https://github.com/sw1ft747/svenmod/blob/main/public/hl_sdk/cl_dll/hud.h#L446

typedef struct CExtraPlayerInfo {
	float m_flScore; //0x0000
	int m_iDeaths; //0x0004
	short m_sPlayerClass; //0x0008
	short m_sTeamNumber; //0x000A
	char m_acTeamName[64]; //0x000C
	float m_flHealth; //0x004C
	float m_flArmour; //0x0050
	char pad_0054[0x4]; //0x0054
} CExtraPlayerInfo; //Size: 0x0058

using CExtraPlayerInfo = struct CExtraPlayerInfo;

extern CExtraPlayerInfo* g_pExtraPlayerInfo;

// Created with ReClass.NET 1.2 by KN4CK3R

typedef struct CExtraPlayerInfo_526
{
	float m_flScore; //0x0000
	int m_nDeaths; //0x0004
	int m_eClassification; //0x0008
	char m_rgcPadding[64]; //0x000C
	float m_flHealth; //0x004C
	float m_flArmour; //0x0050
	char pad_0054[8]; //0x0054
} CExtraPlayerInfo_526; //Size: 0x005C

using CExtraPlayerInfo_526 = struct CExtraPlayerInfo_526;

extern CExtraPlayerInfo_526* g_pExtraPlayerInfo_526;

#else //!__cplusplus
#error C++ compiler required to compile extra_player_info.hpp
#endif //__cplusplus

#endif //EXTRA_PLAYER_INFO_HPP_GUARD

#undef EXTRA_PLAYER_INFO_HPP_RECURSE_GUARD
#endif //EXTRA_PLAYER_INFO_HPP_RECURSE_GUARD