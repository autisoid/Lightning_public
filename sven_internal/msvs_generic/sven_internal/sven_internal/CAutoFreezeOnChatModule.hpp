/**
 * Copyright - xWhitey, 2024.
 * CAutoFreezeOnChatModule.hpp - automatically turn on airstuck whenever the player opens chat/console
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CAUTOFREEZEONCHATMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAutoFreezeOnChatModule.hpp
#else //CAUTOFREEZEONCHATMODULE_HPP_RECURSE_GUARD

#define CAUTOFREEZEONCHATMODULE_HPP_RECURSE_GUARD

#ifndef CAUTOFREEZEONCHATMODULE_HPP_GUARD
#define CAUTOFREEZEONCHATMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAutoFreezeOnChatModule : CModule {
	CAutoFreezeOnChatModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CFloatValue* m_pUnfreezeAfter;
	unsigned long long m_ui64LastNotPlayingTime;
	struct CAirStuckModule* m_pAirStuckModule;
	int m_iState;
} CAutoFreezeOnChatModule;

using CAutoFreezeOnChatModule = struct CAutoFreezeOnChatModule;

#else //!__cplusplus
#error C++ compiler required to compile CAutoFreezeOnChatModule.hpp
#endif //__cplusplus

#endif //CAUTOFREEZEONCHATMODULE_HPP_GUARD

#undef CAUTOFREEZEONCHATMODULE_HPP_RECURSE_GUARD
#endif //CAUTOFREEZEONCHATMODULE_HPP_RECURSE_GUARD