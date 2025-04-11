/**
 * Copyright - xWhitey, 2024.
 * simple_gui.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef SIMPLE_GUI_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in simple_gui.hpp
#else //SIMPLE_GUI_HPP_RECURSE_GUARD

#define SIMPLE_GUI_HPP_RECURSE_GUARD

#ifndef SIMPLE_GUI_HPP_GUARD
#define SIMPLE_GUI_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CModuleTab {
	CModuleTab(CModule* _Module);
	void Draw();

	bool m_bOpen;
	CModule* m_pTheModule;
} CModuleTab;

using CModuleTab = struct CModuleTab;

typedef struct CCategoryPanel {
	CCategoryPanel(const char* _Name);
	void Draw();
	
	bool m_bOpen;
	const char* m_pszName;
	CVector<CModuleTab*>* m_pModules;
} CCategoryPanel;

using CCategoryPanel = struct CCategoryPanel;

typedef struct CSimpleGui {
	CSimpleGui();
	void Draw();

	CVector<CCategoryPanel*>* m_pPanels;
} CSimpleGui;

using CSimpleGui = struct CSimpleGui;

#else //!__cplusplus
#error C++ compiler required to compile simple_gui.hpp
#endif //__cplusplus

#endif //SIMPLE_GUI_HPP_GUARD

#undef SIMPLE_GUI_HPP_RECURSE_GUARD
#endif //SIMPLE_GUI_HPP_RECURSE_GUARD