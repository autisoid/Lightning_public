/**
 * Copyright - xWhitey, 2023.
 * CSpeedModule.hpp - description
 */

#ifdef CSPEEDMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CSpeedModule.hpp
#else //CSPEEDMODULE_HPP_RECURSE_GUARD

#define CSPEEDMODULE_HPP_RECURSE_GUARD

#ifndef CSPEEDMODULE_HPP_GUARD
#define CSPEEDMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CSpeedModule : CModule {
	CSpeedModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;
	virtual const char* GetTag() override;

	CListValue* m_pMode = nullptr;
	CFloatValue* m_pGameSpeed = nullptr;
	CIntegerValue* m_pGameSpeedChargeFor = nullptr;
	CFloatValue* m_pAnarchyGameSpeed = nullptr;
	CFloatValue* m_pAnarchyBaseSpeed = nullptr;
	CIntegerValue* m_pAnarchyChargeFor = nullptr;

	int m_iGameSpeedChargeForCounter = 0;
} CSpeedModule;

using CSpeedModule = struct CSpeedModule;

#else //__cplusplus
#error C++ compiler required to compile CSpeedModule.hpp.
#endif //__cplusplus

#endif //CSPEEDMODULE_HPP_GUARD

#undef CSPEEDMODULE_HPP_RECURSE_GUARD
#endif //CSPEEDMODULE_HPP_RECURSE_GUARD