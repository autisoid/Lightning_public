/**
 * Copyright - xWhitey, 2024.
 * command_hooks.cpp - CCommandHooks impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "command_hooks.hpp"
#include "HwDLL.hpp"

//CCommandHooks::_m_tOriginalConCmdFunction CCommandHooks::_ms_pfnOriginalSay;

void CCommandHooks::Initialise() {
	//CEngineHooks::SetupEngineCommandHook("say", CCommandHooks::SayCommand, reinterpret_cast<void**>(&_ms_pfnOriginalSay));
}

/*void CCommandHooks::SayCommand() {
	printf("Hey, \"say\" command was called with %d args!\n", g_pEngfuncs->Cmd_Argc());

	return _ms_pfnOriginalSay();
}*/