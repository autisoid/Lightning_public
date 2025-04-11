/**
 * Copyright - xWhitey, 2023.
 * CBobbingModule.hpp - Source-like bobbing
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CBOBBINGMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CBobbingModule.hpp
#else //CBOBBINGMODULE_HPP_RECURSE_GUARD

#define CBOBBINGMODULE_HPP_RECURSE_GUARD

#ifndef CBOBBINGMODULE_HPP_GUARD
#define CBOBBINGMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CBobbingModule : CModule {
	CBobbingModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CBoolValue* m_pBobStrengthIsUncapped = nullptr;
	CIntegerValue* m_pBobStrengthSpeedCap = nullptr;
} CBobbingModule;

using CBobbingModule = struct CBobbingModule;

#else //!__cplusplus
#error C++ compiler required to compile CBobbingModule.hpp
#endif //__cplusplus

#endif //CBOBBINGMODULE_HPP_GUARD

#undef CBOBBINGMODULE_HPP_RECURSE_GUARD
#endif //CBOBBINGMODULE_HPP_RECURSE_GUARD