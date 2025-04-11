/**
 * Copyright - xWhitey, 2023.
 * CAutoTertiaryAttackGlitchModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CAUTOTERTIARYATTACKGLITCHMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAutoTertiaryAttackGlitchModule.hpp
#else //CAUTOTERTIARYATTACKGLITCHMODULE_HPP_RECURSE_GUARD

#define CAUTOTERTIARYATTACKGLITCHMODULE_HPP_RECURSE_GUARD

#ifndef CAUTOTERTIARYATTACKGLITCHMODULE_HPP_GUARD
#define CAUTOTERTIARYATTACKGLITCHMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAutoTertiaryAttackGlitchModule : CModule {
	CAutoTertiaryAttackGlitchModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CIntegerValue* m_pWaitTicksBeforeDoingGlitch;

	bool m_bAllowedToDoGlitch, m_bHasSwitchedWeaponsUsingLastinv, m_bPreviousWeaponWasGauss, m_bTotallySure, m_bWaitingForWeaponAnimation;
	int m_iWaitCount, m_iTicksWaitedAfterWeaponHasBeenSelected;
} CAutoTertiaryAttackGlitchModule;

using CAutoTertiaryAttackGlitchModule = struct CAutoTertiaryAttackGlitchModule;

#else //!__cplusplus
#error C++ compiler required to compile CAutoTertiaryAttackGlitchModule.hpp
#endif //__cplusplus

#endif //CAUTOTERTIARYATTACKGLITCHMODULE_HPP_GUARD

#undef CAUTOTERTIARYATTACKGLITCHMODULE_HPP_RECURSE_GUARD
#endif //CAUTOTERTIARYATTACKGLITCHMODULE_HPP_RECURSE_GUARD