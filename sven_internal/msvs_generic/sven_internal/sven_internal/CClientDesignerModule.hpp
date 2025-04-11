/**
 * Copyright - xWhitey, 2023.
 * CClientDesignerModule.hpp - A module that is able to change the accent color of our client :)
 *
 * Lightning a.k.a lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CCLIENTDESIGNERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CClientDesignerModule.hpp
#else //CCLIENTDESIGNERMODULE_HPP_RECURSE_GUARD

#define CCLIENTDESIGNERMODULE_HPP_RECURSE_GUARD

#ifndef CCLIENTDESIGNERMODULE_HPP_GUARD
#define CCLIENTDESIGNERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CClientDesignerModule : CModule {
	CClientDesignerModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	Vector GetRainbowColour32(double _Offset, float _Saturation, float _Lightness);

	CBoolValue* m_pRainbow = nullptr;
	CFloatValue* m_pRainbowSaturation = nullptr;
	CFloatValue* m_pRainbowLightness = nullptr;
	CColourValue* m_pClientColor = nullptr;
	CListValue* m_pRainbowMode = nullptr;
	CBoolValue* m_pNotifications = nullptr;

	bool m_bUseFastAccessToTime;
	bool m_bHasTriedAccessingFastTime;

	float m_flGlowRGB_HUE = 0.0f;
	double m_dblNextUpdateTime = 0.0;
} CClientDesignerModule;

using CClientDesignerModule = struct CClientDesignerModule;

#else //!__cplusplus
#error C++ compiler required to compile CClientDesignerModule.hpp
#endif //__cplusplus

#endif //CCLIENTDESIGNERMODULE_HPP_GUARD

#undef CCLIENTDESIGNERMODULE_HPP_RECURSE_GUARD
#endif //CCLIENTDESIGNERMODULE_HPP_RECURSE_GUARD