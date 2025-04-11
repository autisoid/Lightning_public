/**
 * Copyright - xWhitey, 2023.
 * CApplicationSpeedModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CAPPLICATIONSPEEDMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CApplicationSpeedModule.hpp
#else //CAPPLICATIONSPEEDMODULE_HPP_RECURSE_GUARD

#define CAPPLICATIONSPEEDMODULE_HPP_RECURSE_GUARD

#ifndef CAPPLICATIONSPEEDMODULE_HPP_GUARD
#define CAPPLICATIONSPEEDMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

extern float g_flBaseApplicationSpeed;

typedef struct CApplicationSpeedModule : CModule {
	CApplicationSpeedModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CFloatValue* m_pSpeed = nullptr;
} CApplicationSpeedModule;

using CApplicationSpeedModule = struct CApplicationSpeedModule;

#else //!__cplusplus
#error C++ compiler required to compile CApplicationSpeedModule.hpp
#endif //__cplusplus

#endif //CAPPLICATIONSPEEDMODULE_HPP_GUARD

#undef CAPPLICATIONSPEEDMODULE_HPP_RECURSE_GUARD
#endif //CAPPLICATIONSPEEDMODULE_HPP_RECURSE_GUARD