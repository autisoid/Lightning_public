#include "StdAfx.h"
#include "script_console.hpp"

void CScriptConsole::Print(const std::string& _Message) {
	CCheat::GetCheat()->m_pConsole->Printf(_Message.c_str());
}