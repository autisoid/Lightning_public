/**
 * Copyright - xWhitey, 2023.
 * CHUDModule.hpp - description
 */

#ifdef CHUDMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CHUDModule.hpp
#else //CHUDMODULE_HPP_RECURSE_GUARD

#define CHUDMODULE_HPP_RECURSE_GUARD

#ifndef CHUDMODULE_HPP_GUARD
#define CHUDMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CHUDModule : CModule {
	CHUDModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CBoolValue* m_pDrawWatermark = nullptr;
	CBoolValue* m_pDrawArrayList = nullptr;
	CBoolValue* m_pArrayListBackground = nullptr;
	CBoolValue* m_pArrayListRect = nullptr;
	CFloatValue* m_pArrayListTextHeight = nullptr;
	CTextValue* m_pWatermarkText = nullptr;
	CBoolValue* m_pGlow = nullptr;
	CFloatValue* m_pRainbowStep = nullptr;
} CHUDModule;

using CHUDModule = struct CHUDModule;

#else //__cplusplus
#error C++ compiler required to compile CHUDModule.hpp.
#endif //__cplusplus

#endif //CHUDMODULE_HPP_GUARD

#undef CHUDMODULE_HPP_RECURSE_GUARD
#endif //CHUDMODULE_HPP_RECURSE_GUARD