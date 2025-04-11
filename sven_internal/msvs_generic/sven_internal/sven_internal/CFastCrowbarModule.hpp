/**
 * Copyright - xWhitey, 2023.
 * CFastCrowbarModule.hpp - fast crowbar exploit (a.k.a charging crowbar)
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CFASTCROWBARMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CFastCrowbarModule.hpp
#else //CFASTCROWBARMODULE_HPP_RECURSE_GUARD

#define CFASTCROWBARMODULE_HPP_RECURSE_GUARD

#ifndef CFASTCROWBARMODULE_HPP_GUARD
#define CFASTCROWBARMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CFastCrowbarModule : CModule {
	CFastCrowbarModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CIntegerValue* m_pChargeFor = nullptr;
	CBoolValue* m_pBlockMovements = nullptr;
	int m_iChargedFor = 0;
	bool m_bAllowedToCharge = false, m_bHasSwitchedWeaponsUsingLastinv, m_bPreviousWeaponWasCrowbar;
} CFastCrowbarModule;

using CFastCrowbarModule = struct CFastCrowbarModule;

#else //!__cplusplus
#error C++ compiler required to compile CFastCrowbarModule.hpp
#endif //__cplusplus

#endif //CFASTCROWBARMODULE_HPP_GUARD

#undef CFASTCROWBARMODULE_HPP_RECURSE_GUARD
#endif //CFASTCROWBARMODULE_HPP_RECURSE_GUARD