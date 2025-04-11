/**
 * Copyright - xWhitey, 2024.
 * player_sp.hpp - singleplayer base player entity "imagination"
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef PLAYER_SP_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in player_sp.hpp
#else //PLAYER_SP_HPP_RECURSE_GUARD

#define PLAYER_SP_HPP_RECURSE_GUARD

#ifndef PLAYER_SP_HPP_GUARD
#define PLAYER_SP_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "input_buttons.hpp"

typedef struct CEntityPlayerSP {
	CEntityPlayerSP();
	~CEntityPlayerSP();

	CInputButtons* m_pInputButtons;
	CVirtualInputButtons* m_pVirtualInputButtons;
} CEntityPlayerSP;

using CEntityPlayerSP = struct CEntityPlayerSP;

#else //!__cplusplus
#error C++ compiler required to compile player_sp.hpp
#endif //__cplusplus

#endif //PLAYER_SP_HPP_GUARD

#undef PLAYER_SP_HPP_RECURSE_GUARD
#endif //PLAYER_SP_HPP_RECURSE_GUARD