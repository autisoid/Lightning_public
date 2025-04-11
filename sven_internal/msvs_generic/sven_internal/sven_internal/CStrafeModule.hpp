/**
 * Copyright - xWhitey, 2024.
 * CStrafeModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CSTRAFEMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CStrafeModule.hpp
#else //CSTRAFEMODULE_HPP_RECURSE_GUARD

#define CSTRAFEMODULE_HPP_RECURSE_GUARD

#ifndef CSTRAFEMODULE_HPP_GUARD
#define CSTRAFEMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "strafe.hpp"

typedef struct CStrafeModule : CModule {
	CStrafeModule();

	void OnEnable() override;
	void OnDisable() override;

	void OnEvent(_In_ const ISimpleEvent*) override;

	void UpdateStrafeData(Strafe::StrafeData& _StrafeData, bool _ReduceWishSpeed, bool _Strafe, Strafe::StrafeDir _Dir, Strafe::StrafeType _Type, float _Yaw, float _PointX, float _PointY);

	CBoolValue* m_pStopWhenNoInput;
	CIntegerValue* m_pWaitNTicksBeforeFlippingDirection;
	CFloatValue* m_pTargetX;
	CFloatValue* m_pTargetY;
	CListValue* m_pTargetDirection;
	CListValue* m_pStrafeType;
	CBoolValue* m_pReduceWishSpeed;
	CBoolValue* m_pSilentViewangles;
	CBoolValue* m_pPerfectSilentViewangles;
	CBoolValue* m_pInstantStopOnBackwards;

	int m_nTicksPassedSinceLastFlip;
} CStrafeModule;

using CStrafeModule = struct CStrafeModule;

#else //!__cplusplus
#error C++ compiler required to compile CStrafeModule.hpp
#endif //__cplusplus

#endif //CSTRAFEMODULE_HPP_GUARD

#undef CSTRAFEMODULE_HPP_RECURSE_GUARD
#endif //CSTRAFEMODULE_HPP_RECURSE_GUARD