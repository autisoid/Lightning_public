/**
 * Copyright - xWhitey, 2023.
 * CAntiAimModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CANTIAIMMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAntiAimModule.hpp
#else //CANTIAIMMODULE_HPP_RECURSE_GUARD

#define CANTIAIMMODULE_HPP_RECURSE_GUARD

#ifndef CANTIAIMMODULE_HPP_GUARD
#define CANTIAIMMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAntiAimModule : CModule {
	CAntiAimModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CBoolValue* m_pJitter = nullptr;
	CIntegerValue* m_pJitterMin = nullptr;
	CIntegerValue* m_pJitterMax = nullptr;
	CFloatValue* m_pRollAngle = nullptr;

	Vector m_vecAntiAimAngles;

	bool m_bStop = false;
} CAntiAimModule;
 
using CAntiAimModule = struct CAntiAimModule;

#else //!__cplusplus
#error C++ compiler required to compile CAntiAimModule.hpp
#endif //__cplusplus

#endif //CANTIAIMMODULE_HPP_GUARD

#undef CANTIAIMMODULE_HPP_RECURSE_GUARD
#endif //CANTIAIMMODULE_HPP_RECURSE_GUARD