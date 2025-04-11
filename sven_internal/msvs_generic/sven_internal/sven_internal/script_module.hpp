/**
 * Copyright - xWhitey, 2024.
 * script_module.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef SCRIPT_MODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in script_module.hpp
#else //SCRIPT_MODULE_HPP_RECURSE_GUARD

#define SCRIPT_MODULE_HPP_RECURSE_GUARD

#ifndef SCRIPT_MODULE_HPP_GUARD
#define SCRIPT_MODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "as_value_api.hpp"

typedef struct CBaseScriptModule : CModule {
	CBaseScriptModule(std::string _Name, std::string _Category, int _KeyBind, bool _ShownInArrayList);

	void AddRef();
	void Release();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent* _Event) override;
	virtual bool IsScriptModule() override;

	void SetOnEnable(void* _Func);
	void SetOnDisable(void* _Func);
	void SetOnEvent(void* _Func);
	
	void AddValue(CScriptValue* _Value);

	unsigned int m_nRefCount;
	void* m_pfnOnEnable;
	void* m_pfnOnDisable;
	void* m_pfnOnEvent;
} CBaseScriptModule;

using CBaseScriptModule = struct CBaseScriptModule;

#else //!__cplusplus
#error C++ compiler required to compile script_module.hpp
#endif //__cplusplus

#endif //SCRIPT_MODULE_HPP_GUARD

#undef SCRIPT_MODULE_HPP_RECURSE_GUARD
#endif //SCRIPT_MODULE_HPP_RECURSE_GUARD