#include "StdAfx.h"
#include "as_callroutines.hpp"

void CASCallRoutines::CallStub() {
	// TODO: Rewrite with args support
	asIScriptFunction* pFunction = g_Callstack.Pop<asIScriptFunction*>();
	if (!pFunction) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CASCallRoutines::CallStub: pFunction is nullptr\n");
		return;
	}

	asIScriptEngine* pEngine = CCheat::GetCheat()->m_pTheScripts->m_pScriptEngine;

	asIScriptContext* pContext = pEngine->CreateContext();

	pContext->Prepare(pFunction);
	int r = pContext->Execute();
	if (r != asEXECUTION_FINISHED) {
		if (r == asEXECUTION_EXCEPTION) {
			CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CASCallRoutines::CallStub: an exception has occurred during execution of %s in AS module %s: %s\n", pFunction->GetName(), pFunction->GetModuleName(), pContext->GetExceptionString());
		}
	}
	pContext->Release();
}