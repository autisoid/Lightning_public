/**
 * Copyright - xWhitey, 2023.
 * CServerCrasherModule.hpp - description
 */

#ifdef CSERVERCRASHERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CServerCrasherModule.hpp
#else //CSERVERCRASHERMODULE_HPP_RECURSE_GUARD

#define CSERVERCRASHERMODULE_HPP_RECURSE_GUARD

#ifndef CSERVERCRASHERMODULE_HPP_GUARD
#define CSERVERCRASHERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CServerCrasherModule : CModule {
	CServerCrasherModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;
	virtual const char* GetTag() override;

	CListValue* m_pMode = nullptr;

	//CustomVision settings
	CIntegerValue* m_pCVInterpolationTime = nullptr;
	CIntegerValue* m_pCVMsecTime = nullptr;

	CFloatValue* m_pCVYaw = nullptr;
	CFloatValue* m_pCVPitch = nullptr;
	CFloatValue* m_pCVRoll = nullptr;

	CFloatValue* m_pCVForwardMove = nullptr;
	CFloatValue* m_pCVSideMove = nullptr;
	CFloatValue* m_pCVUpMove = nullptr;

	CIntegerValue* m_pCVLightLevel = nullptr;

	CIntegerValue* m_pCVButtons = nullptr;
	CIntegerValue* m_pCVImpulse = nullptr;

	CIntegerValue* m_pCVWeaponSelect = nullptr;

	CIntegerValue* m_pCVImpactIdx = nullptr;

	CFloatValue* m_pCVImpactX = nullptr;
	CFloatValue* m_pCVImpactY = nullptr;
	CFloatValue* m_pCVImpactZ = nullptr;

	CBoolValue* m_pVisionBlockMovements = nullptr;

	CIntegerValue* m_pRPRXOffset = nullptr;
	CIntegerValue* m_pEditOffset = nullptr;
} CServerCrasherModule;

using CServerCrasherModule = struct CServerCrasherModule;

#else //__cplusplus
#error C++ compiler required to compile CServerCrasherModule.hpp.
#endif //__cplusplus

#endif //CSERVERCRASHERMODULE_HPP_GUARD

#undef CSERVERCRASHERMODULE_HPP_RECURSE_GUARD
#endif //CSERVERCRASHERMODULE_HPP_RECURSE_GUARD