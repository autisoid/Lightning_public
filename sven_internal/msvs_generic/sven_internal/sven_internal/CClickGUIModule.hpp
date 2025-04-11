/**
 * Copyright - xWhitey, 2023.
 * CClickGUIModule.hpp - description
 */

#ifdef CCLICKGUIMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CClickGUIModule.hpp
#else //CCLICKGUIMODULE_HPP_RECURSE_GUARD

#define CCLICKGUIMODULE_HPP_RECURSE_GUARD

#ifndef CCLICKGUIMODULE_HPP_GUARD
#define CCLICKGUIMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "click_gui.hpp"
#include "simple_gui.hpp"

typedef struct CClickGUIModule : CModule {
	CClickGUIModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CListValue* m_pTheme;
	CFloatValue* m_pScale;
	CListValue* m_pStyle;

	CBoolValue* m_pRounding;
	CFloatValue* m_pPanelSize;

	CClickGUI* m_pClickGUI;
	CSimpleGui* m_pSimpleGUI;

	bool m_bDebugConsoleActive, m_bIsInCustomFontsGui;
	char* m_acInputField;
	int m_nCurrentInputFieldIdx;
	cvar_t* m_pMouseRawInputConVar;
	POINT m_pntLastCursorPos;
} CClickGUIModule;

using CClickGUIModule = struct CClickGUIModule;

#else //__cplusplus
#error C++ compiler required to compile CClickGUIModule.hpp.
#endif //__cplusplus

#endif //CCLICKGUIMODULE_HPP_GUARD

#undef CCLICKGUIMODULE_HPP_RECURSE_GUARD
#endif //CCLICKGUIMODULE_HPP_RECURSE_GUARD