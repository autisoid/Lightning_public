/**
 * Copyright - xWhitey, 2023.
 * CESPModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CESPMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CESPModule.hpp
#else //CESPMODULE_HPP_RECURSE_GUARD

#define CESPMODULE_HPP_RECURSE_GUARD

#ifndef CESPMODULE_HPP_GUARD
#define CESPMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "custom_fonts.hpp"

typedef struct CESPModule : CModule {
	CESPModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CBoolValue* m_pDrawPlayers;
	CBoolValue* m_pDrawItems;
	CBoolValue* m_pDrawOtherEntities;
	CBoolValue* m_pDrawUnknown;
	CBoolValue* m_pDrawFurniture;
	CBoolValue* m_pConcatUnknownEntityModel;
	CFloatValue* m_pFontSize;

	CColourValue* m_pEnemyColour;
	CColourValue* m_pFriendColour;
	CColourValue* m_pUnknownColour;
	CColourValue* m_pItemColour;

	CExtraValues* m_pExtraValues;

	ImColor m_ColorEnemy;
	ImColor m_ColorFriend;
	ImColor m_ColorUnknown;
	ImColor m_ColorItem;

	CCustomFont* m_pDrawingFont;
} CESPModule;

using CESPModule = struct CESPModule;

extern CEntityList* g_pEntityList;

#else //!__cplusplus
#error C++ compiler required to compile CESPModule.hpp
#endif //__cplusplus

#endif //CESPMODULE_HPP_GUARD

#undef CESPMODULE_HPP_RECURSE_GUARD
#endif //CESPMODULE_HPP_RECURSE_GUARD