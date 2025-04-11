/**
 * Copyright - xWhitey, 2024.
 * CDiscordRPCModule.hpp - discord rpc hooks
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CDISCORDRPCMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CDiscordRPCModule.hpp
#else //CDISCORDRPCMODULE_HPP_RECURSE_GUARD

#define CDISCORDRPCMODULE_HPP_RECURSE_GUARD

#ifndef CDISCORDRPCMODULE_HPP_GUARD
#define CDISCORDRPCMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CDiscordRPCModule : CModule {
	CDiscordRPCModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	HMODULE m_hDiscordRPC;
	time_t m_tStartTime;
} CDiscordRPCModule;

using CDiscordRPCModule = struct CDiscordRPCModule;

extern struct CDiscordRPCModule* g_pDiscordRPCModule;

#else //!__cplusplus
#error C++ compiler required to compile CDiscordRPCModule.hpp
#endif //__cplusplus

#endif //CDISCORDRPCMODULE_HPP_GUARD

#undef CDISCORDRPCMODULE_HPP_RECURSE_GUARD
#endif //CDISCORDRPCMODULE_HPP_RECURSE_GUARD