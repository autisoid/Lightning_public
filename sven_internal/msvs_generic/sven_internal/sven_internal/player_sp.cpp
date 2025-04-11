/**
 * Copyright - xWhitey, 2024.
 * player_sp.cpp - singleplayer base player entity "implementation"
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "player_sp.hpp"

CEntityPlayerSP::CEntityPlayerSP() {
	m_pInputButtons = Q_new(CInputButtons)();
	m_pVirtualInputButtons = Q_new(CVirtualInputButtons)();
	CVirtualInputButtons::Initialise();
}

CEntityPlayerSP::~CEntityPlayerSP() {
	Q_delete(m_pInputButtons);
	Q_delete(m_pVirtualInputButtons);
}
