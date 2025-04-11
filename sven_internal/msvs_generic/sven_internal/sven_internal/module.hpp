/**
 * Copyright - xWhitey, 2023.
 * module.hpp - A really simple "Module" implementation.
 */

#ifdef MODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in module.hpp
#else //MODULE_HPP_RECURSE_GUARD

#define MODULE_HPP_RECURSE_GUARD

#ifndef MODULE_HPP_GUARD
#define MODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define DefineConCmd(_ConCmd, _Typename) do { if (_ConCmd) g_pEngfuncs->pfnAddCommand(_ConCmd, []() { CCheat::GetCheat()->m_pModuleManager->GetModule<_Typename>()->Toggle(); }); } while(0)

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif //_MSC_VER

typedef struct CModule : CAbstractEventListener {
	const char* m_pszName = "Module";
	const char* m_pszCategory = "Category";
	bool m_bState = false, m_bShownInArrayList = true;
	bool m_bInitialized = false;
	CVector<CValue*>* m_pVecValues;
	int m_nKeyBind;
	const char* m_pszKeybind;
protected:
	CModule(_In_z_ const char*, _In_z_ const char*, _In_opt_ int _KeyBind = 0x0, _In_opt_ bool _ShownInArrayList = true);
public:
	virtual const char* GetTag() { return ""; }
	virtual void SetState(_In_ bool, _In_opt_ bool _PreLoading = false);
	void Toggle();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual bool IsScriptModule();
} CModule;

#ifdef _MSC_VER
#pragma pack(pop)
#endif //_MSC_VER

#else //__cplusplus
#error C++ compiler required to compile module.hpp.
#endif //__cplusplus

#endif //MODULE_HPP_GUARD

#undef MODULE_HPP_RECURSE_GUARD
#endif //MODULE_HPP_RECURSE_GUARD