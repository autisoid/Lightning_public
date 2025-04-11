/**
 * Copyright - xWhitey, 2025.
 * hud_arraylist_element.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef HUD_ARRAYLIST_ELEMENT_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in hud_arraylist_element.hpp
#else //HUD_ARRAYLIST_ELEMENT_HPP_RECURSE_GUARD

#define HUD_ARRAYLIST_ELEMENT_HPP_RECURSE_GUARD

#ifndef HUD_ARRAYLIST_ELEMENT_HPP_GUARD
#define HUD_ARRAYLIST_ELEMENT_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "custom_hud.hpp"

typedef struct CArrayListHUDElement : CHUDElement {
	CArrayListHUDElement();

	virtual void Draw() override;
} CArrayListHUDElement;

using CArrayListHUDElement = struct CArrayListHUDElement;

#else //!__cplusplus
#error C++ compiler required to compile hud_arraylist_element.hpp
#endif //__cplusplus

#endif //HUD_ARRAYLIST_ELEMENT_HPP_GUARD

#undef HUD_ARRAYLIST_ELEMENT_HPP_RECURSE_GUARD
#endif //HUD_ARRAYLIST_ELEMENT_HPP_RECURSE_GUARD