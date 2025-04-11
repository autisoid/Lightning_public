/**
 * Copyright - xWhitey, 2025.
 * custom_hud.hpp - move hud elements (watermark, arraylist)
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CUSTOM_HUD_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in custom_hud.hpp
#else //CUSTOM_HUD_HPP_RECURSE_GUARD

#define CUSTOM_HUD_HPP_RECURSE_GUARD

#ifndef CUSTOM_HUD_HPP_GUARD
#define CUSTOM_HUD_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CHUDElement {
	CHUDElement(const char* _Name);

	virtual void Draw() = 0;

	const char* m_pszName;
protected:
	virtual ~CHUDElement() = default;
} CHUDElement;

using CHUDElement = struct CHUDElement;

typedef struct CCustomHUD {
	CCustomHUD();
	~CCustomHUD();

	void SaveElements();
	void LoadElements();

	void Draw();

	CConfigMgr* m_pElementsMgr = nullptr;
	CLinkedList<CHUDElement*>* m_pElements;
} CCustomHUD;

using CCustomHUD = struct CCustomHUD;

#else //!__cplusplus
#error C++ compiler required to compile custom_hud.hpp
#endif //__cplusplus

#endif //CUSTOM_HUD_HPP_GUARD

#undef CUSTOM_HUD_HPP_RECURSE_GUARD
#endif //CUSTOM_HUD_HPP_RECURSE_GUARD