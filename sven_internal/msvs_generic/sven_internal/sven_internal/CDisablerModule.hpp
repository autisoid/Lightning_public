/**
 * Copyright - xWhitey, 2024.
 * CDisablerModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CDISABLERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CDisablerModule.hpp
#else //CDISABLERMODULE_HPP_RECURSE_GUARD

#define CDISABLERMODULE_HPP_RECURSE_GUARD

#ifndef CDISABLERMODULE_HPP_GUARD
#define CDISABLERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CDisablerModule : CModule {
	CDisablerModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;
	virtual const char* GetTag() override;

	CListValue* m_pMode = nullptr;
	CFloatValue* m_pDelayBetweenUpdates;
	CIntegerValue* m_pCommandAmount = nullptr;
	double m_dblLastCommandSendTime;
	unsigned long long m_ullModuleEnableTime;
	Vector m_vecModuleEnableViewangles;
	bool m_bHasDucked;
} CDisablerModule;

using CDisablerModule = struct CDisablerModule;

#else //!__cplusplus
#error C++ compiler required to compile CDisablerModule.hpp
#endif //__cplusplus

#endif //CDISABLERMODULE_HPP_GUARD

#undef CDISABLERMODULE_HPP_RECURSE_GUARD
#endif //CDISABLERMODULE_HPP_RECURSE_GUARD