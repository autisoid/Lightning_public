/**
 * Copyright - xWhitey, 2023.
 * CTimeMachineModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CTIMEMACHINEMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CTimeMachineModule.hpp
#else //CTIMEMACHINEMODULE_HPP_RECURSE_GUARD

#define CTIMEMACHINEMODULE_HPP_RECURSE_GUARD

#ifndef CTIMEMACHINEMODULE_HPP_GUARD
#define CTIMEMACHINEMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CTimeMachineModule : CModule {
	CTimeMachineModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CIntegerValue* m_pStrength = nullptr;
	CIntegerValue* m_pChargeFor = nullptr;
	CBoolValue* m_pBlockMovements = nullptr;
	CBoolValue* m_pDontSendNormalCmds = nullptr;
	CBoolValue* m_pRandomLerpMsec = nullptr;

	int m_iChargeCounter = 0;
} CTimeMachineModule;

using CTimeMachineModule = struct CTimeMachineModule;

#else //!__cplusplus
#error C++ compiler required to compile CTimeMachineModule.hpp
#endif //__cplusplus

#endif //CTIMEMACHINEMODULE_HPP_GUARD

#undef CTIMEMACHINEMODULE_HPP_RECURSE_GUARD
#endif //CTIMEMACHINEMODULE_HPP_RECURSE_GUARD