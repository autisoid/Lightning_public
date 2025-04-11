#include "StdAfx.h"
#include "script_module.hpp"

CAbstractEventListener* CBaseScriptModuleToCAbstractEventListener(CBaseScriptModule* _Mod) {
	return static_cast<CAbstractEventListener*>(_Mod);
}

CBaseScriptModule* CBaseScriptModuleFactory(const std::string& _Name, const std::string& _Category, int _KeyBind, bool _ShownInArrayList) {
	return Q_new(CBaseScriptModule)(_Name, _Category, _KeyBind, _ShownInArrayList);
}

CBaseScriptModule::CBaseScriptModule(std::string _Name, std::string _Category, int _KeyBind, bool _ShownInArrayList) : CModule(_strdup(_Name.c_str()), _strdup(_Category.c_str()), _KeyBind, static_cast<bool>(_ShownInArrayList)) {
	m_nRefCount = 0;
	m_pfnOnDisable = m_pfnOnEnable = m_pfnOnEvent = nullptr;
}

bool CBaseScriptModule::IsScriptModule() {
	return true;
}

void CBaseScriptModule::AddRef() {
	m_nRefCount++;
}

void CBaseScriptModule::Release() {
	if (--m_nRefCount == 0) Q_delete(this);
}

void CBaseScriptModule::SetOnEnable(void* _Func) {
	if (m_pfnOnEnable) {
		asIScriptFunction* pOnEnable = (asIScriptFunction*)m_pfnOnEnable;
		pOnEnable->Release();
	}
	m_pfnOnEnable = _Func;
}

void CBaseScriptModule::SetOnDisable(void* _Func) {
	if (m_pfnOnDisable) {
		asIScriptFunction* pOnDisable = (asIScriptFunction*)m_pfnOnDisable;
		pOnDisable->Release();
	}
	m_pfnOnDisable = _Func;
}

void CBaseScriptModule::SetOnEvent(void* _Func) {
	if (m_pfnOnEvent) {
		asIScriptFunction* pOnEvent = (asIScriptFunction*)m_pfnOnEvent;
		pOnEvent->Release();
	}
	m_pfnOnEvent = _Func;
}

void CBaseScriptModule::AddValue(CScriptValue* _Value) {
	_Value->AddRef();
	m_pVecValues->push_back(_Value);
}

void CBaseScriptModule::OnEnable() {
	CModule::OnEnable();
	if (m_pfnOnEnable) {
		asIScriptFunction* pOnEnable = (asIScriptFunction*)m_pfnOnEnable;

		asIScriptEngine* pEngine = CCheat::GetCheat()->m_pTheScripts->m_pScriptEngine;

		asIScriptContext* ctx = pEngine->CreateContext();

		ctx->Prepare(pOnEnable);
		int r = ctx->Execute();
		if (r != asEXECUTION_FINISHED) {
			if (r == asEXECUTION_EXCEPTION) {
				CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CBaseScriptModule::OnEnable: an exception has occured during execution of %s#OnEnable: %s\n", m_pszName, ctx->GetExceptionString());
			}
		}
		ctx->Release();
	} else {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CBaseScriptModule::OnEnable: m_pfnOnEnable is nullptr in %s\n", m_pszName);
	}
}

void CBaseScriptModule::OnDisable() {
	if (m_pfnOnDisable) {
		asIScriptFunction* pOnDisable = (asIScriptFunction*)m_pfnOnDisable;

		asIScriptEngine* pEngine = CCheat::GetCheat()->m_pTheScripts->m_pScriptEngine;

		asIScriptContext* ctx = pEngine->CreateContext();

		ctx->Prepare(pOnDisable);
		int r = ctx->Execute();
		if (r != asEXECUTION_FINISHED) {
			if (r == asEXECUTION_EXCEPTION) {
				CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CBaseScriptModule::OnDisable: an exception has occured during execution of %s#OnDisable: %s\n", m_pszName, ctx->GetExceptionString());
			}
		}
		ctx->Release();
	} else {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CBaseScriptModule::OnDisable: m_pfnOnDisable is nullptr in %s\n", m_pszName);
	}
	CModule::OnDisable();
}

void CBaseScriptModule::OnEvent(const ISimpleEvent* _Event) {
	if (m_pfnOnEvent) {
		asIScriptFunction* pOnEvent = (asIScriptFunction*)m_pfnOnEvent;

		asIScriptEngine* pEngine = CCheat::GetCheat()->m_pTheScripts->m_pScriptEngine;

		asIScriptContext* ctx = pEngine->CreateContext();

		ctx->Prepare(pOnEvent);
		ctx->SetArgObject(0, (void*) _Event);
		int r = ctx->Execute();
		if (r != asEXECUTION_FINISHED) {
			if (r == asEXECUTION_EXCEPTION) {
				CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CBaseScriptModule::OnEvent: an exception has occured during execution of %s#OnEvent: %s\n", m_pszName, ctx->GetExceptionString());
			}
		}
		ctx->Release();
	}
}