/**
 * Copyright - xWhitey, 2024.
 * CMovementCommandRateLimiterModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CMovementCommandRateLimiterModule.hpp
#else //CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_RECURSE_GUARD

#define CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_RECURSE_GUARD

#ifndef CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_GUARD
#define CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CMovementCommandRateLimiterModule : CModule {
	CMovementCommandRateLimiterModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CListValue* m_pMode = nullptr;
	CFloatValue* m_pLimitBurst = nullptr;
	CListValue* m_pSpeedMode = nullptr;
	CFloatValue* m_pSpeed = nullptr;

	struct CTimer* m_pTimer;

	double m_dblNextPossibleCommandSubmitTime;
	double m_dblLastCommandSendTime;
} CMovementCommandRateLimiterModule;

using CMovementCommandRateLimiterModule = struct CMovementCommandRateLimiterModule;

#else //!__cplusplus
#error C++ compiler required to compile CMovementCommandRateLimiterModule.hpp
#endif //__cplusplus

#endif //CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_GUARD

#undef CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_RECURSE_GUARD
#endif //CMOVEMENTCOMMANDRATELIMITERMODULE_HPP_RECURSE_GUARD