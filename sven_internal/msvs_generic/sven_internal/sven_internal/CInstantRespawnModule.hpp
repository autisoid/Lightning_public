/**
 * Copyright - xWhitey, 2023.
 * CInstantRespawnModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CINSTANTRESPAWNMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CInstantRespawnModule.hpp
#else //CINSTANTRESPAWNMODULE_HPP_RECURSE_GUARD

#define CINSTANTRESPAWNMODULE_HPP_RECURSE_GUARD

#ifndef CINSTANTRESPAWNMODULE_HPP_GUARD
#define CINSTANTRESPAWNMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CInstantRespawnModule : CModule {
	CInstantRespawnModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CBoolValue* m_pBlockMovements = nullptr;
	CIntegerValue* m_pChargeFor = nullptr;

	int m_iHeldPacketsCount = 0, m_iAfterDeathJumpedCount = 0;
} CInstantRespawnModule;

using CInstantRespawnModule = struct CInstantRespawnModule;

#else //!__cplusplus
#error C++ compiler required to compile CInstantRespawnModule.hpp
#endif //__cplusplus

#endif //CINSTANTRESPAWNMODULE_HPP_GUARD

#undef CINSTANTRESPAWNMODULE_HPP_RECURSE_GUARD
#endif //CINSTANTRESPAWNMODULE_HPP_RECURSE_GUARD