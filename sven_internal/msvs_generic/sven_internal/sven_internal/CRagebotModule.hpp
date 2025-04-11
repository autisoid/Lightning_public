/**
 * Copyright - xWhitey, 2024.
 * CRagebotModule.hpp - ragebot
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CRAGEBOTMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CRagebotModule.hpp
#else //CRAGEBOTMODULE_HPP_RECURSE_GUARD

#define CRAGEBOTMODULE_HPP_RECURSE_GUARD

#ifndef CRAGEBOTMODULE_HPP_GUARD
#define CRAGEBOTMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus
typedef struct CRagebotModule : CModule {
	CRagebotModule();

	typedef enum struct EUnableReason : unsigned char {
		kNone = 0,
		kNoState,
		kNeedsReloading,
		kReloading,
		kBusy,
		kResolvingError
	} EUnableReason;

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	bool CanEntityBeSeen(_In_ Vector _CurrentOrigin, _In_ Vector _TargetOrigin, _In_ int _PlrIdx);
	bool CanReload();
	bool IsZoomed();
	int GetAttackButtonForWeapon(_In_ float _DistanceToTarget);
	EUnableReason CanShoot(_In_ float _DistanceToTarget);
	bool CanDoPrimaryAttack();
	bool CanDoSecondaryAttack();
	bool ShouldKeepAimAfterFiring();
	bool DoesDistanceAllowUsToUseTheWeapon(_In_ float _DistanceToTarget);
	bool IsWeaponStillFiring(_In_ int _CurrentButtons);
	bool IsWeaponSuitable();
	bool CanGenerallyAttack();
	Vector CalculateRotations(_In_ const Vector& _Point, _In_ const Vector& _EyePos);
	CEntity* GetBestTarget(_In_ const Vector& _CurrentOrigin, _Out_ float* _Distance);
	void ClampSpeed(_In_ float _MaxSpeed, _In_ Vector _CurrentVelocity, _In_ usercmd_s* _UserCommand);

	CFloatValue* m_pMaxDistance;
	CBoolValue* m_pPerfectSilent;
	CListValue* m_pAutoStop;
	CFloatValue* m_pAutoStopMaxVelocity;
	CBoolValue* m_pAutoShoot;
	CListValue* m_pBacktrackPreference;
	CListValue* m_pForwardtrackPreference;

	local_state_s* m_pLocalState;
	int m_iWeaponId;
	int m_iPreviousButtons;
	Vector m_vecLastRotations;
	bool m_bHasDonePerfectSilentShot;
	bool m_bHasDoneSpecialWeaponShot;
} CRagebotModule;

using CRagebotModule = struct CRagebotModule;

#else //!__cplusplus
#error C++ compiler required to compile CRagebotModule.hpp
#endif //__cplusplus

#endif //CRAGEBOTMODULE_HPP_GUARD

#undef CRAGEBOTMODULE_HPP_RECURSE_GUARD
#endif //CRAGEBOTMODULE_HPP_RECURSE_GUARD