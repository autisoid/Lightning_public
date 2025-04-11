#include "StdAfx.h"
#include "scriptany.h"
#include "as_value_api.hpp"

CScriptValue* CScriptValueFactory(const std::string& _Name, const std::string& _ConvarName) {
	return Q_new(CScriptValue)(_Name, _ConvarName);
}

CScriptBoolValue* CScriptBoolValueFactory(const std::string& _Name, const std::string& _ConvarName, const bool& _DefaultValue) {
	return Q_new(CScriptBoolValue)(_Name, _ConvarName, _DefaultValue);
}

//TODO: add once value changed callback wrapper
CScriptValue::CScriptValue(const std::string& _Name, const std::string& _ConvarName) : CValue(_strdup(_Name.c_str()), _strdup(_ConvarName.c_str()), kValueTypeNone) {
	m_nRefCount = 0;
}

void CScriptValue::AddRef() {
	m_nRefCount++;
}

void CScriptValue::Release() {
	if (--m_nRefCount == 0) Q_free(this);
}

void ScriptBoolValueOnceChangedCallbackHandler(void* _UserData, void* _Value, void* _PreviousValue) {
	CScriptBoolValue* pThisInstance = (CScriptBoolValue*)_UserData;
	if (!pThisInstance || !pThisInstance->m_pfnScriptOnceValueChangedCB) return;

	asIScriptEngine* pEngine = CCheat::GetCheat()->m_pTheScripts->m_pScriptEngine;

	asIScriptContext* pContext = pEngine->CreateContext();

	asIScriptFunction* pFunction = (asIScriptFunction*)pThisInstance->m_pfnScriptOnceValueChangedCB;

	pContext->Prepare(pFunction);

	pContext->SetArgObject(0, pThisInstance->m_pScriptOnceValueChangedCBUserData);
	pContext->SetArgAddress(1, _Value);
	pContext->SetArgAddress(2, _PreviousValue);

	int r = pContext->Execute();
	if (r != asEXECUTION_FINISHED) {
		if (r == asEXECUTION_EXCEPTION) {
			CCheat::GetCheat()->m_pConsole->Printf("[FATAL] ScriptBoolValueOnceChangedCallbackHandler: an exception has occurred during execution of %s in AS module %s: %s\n", pFunction->GetName(), pFunction->GetModuleName(), pContext->GetExceptionString());
		}
	}
	pContext->Release();
}

void RegisterScriptBoolOnceChangedCallback(CScriptBoolValue* _Value, asIScriptFunction* _Function, CScriptAny* _UserData) {
	if (!_Value) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] RegisterScriptBoolOnceChangedCallback: _Value was nullptr\n");
		return;
	}
	if (!_Function) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] RegisterScriptBoolOnceChangedCallback: _Function was nullptr\n");
		return;
	}
	_Value->m_pfnScriptOnceValueChangedCB = _Function;
	_Value->m_pScriptOnceValueChangedCBUserData = _UserData;
	_Value->RegisterOnceValueChangedCallback(ScriptBoolValueOnceChangedCallbackHandler, _Value);
}

CScriptBoolValue::CScriptBoolValue(const std::string& _Name, const std::string& _ConvarName, const bool& _DefaultValue) : CBoolValue(_strdup(_Name.c_str()), _strdup(_ConvarName.c_str()), _DefaultValue) {
	m_nRefCount = 0;
}

void CScriptBoolValue::AddRef() {
	m_nRefCount++;
}

void CScriptBoolValue::Release() {
	if (--m_nRefCount == 0) Q_free(this); // Q_delete(this);
}