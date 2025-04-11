/**
 * Copyright - xWhitey, 2024.
 * as_value_api.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef AS_VALUE_API_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in as_value_api.hpp
#else //AS_VALUE_API_HPP_RECURSE_GUARD

#define AS_VALUE_API_HPP_RECURSE_GUARD

#ifndef AS_VALUE_API_HPP_GUARD
#define AS_VALUE_API_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CScriptValue : CValue {
	CScriptValue(const std::string& _Name, const std::string& _ConvarName);

	void AddRef();
	void Release();

	int GetFlags() {
		return (int)m_eFlags;
	}

	int m_nRefCount;
} CScriptValue;

using CScriptValue = struct CScriptValue;

typedef struct CScriptBoolValue : CBoolValue {
	CScriptBoolValue(const std::string& _Name, const std::string& _ConvarName, const bool& _DefaultValue);

	void AddRef();
	void Release();

	int m_nRefCount;
	void* m_pfnScriptOnceValueChangedCB;
	void* m_pScriptOnceValueChangedCBUserData;
} CScriptBoolValue;

using CScriptBoolValue = struct CScriptBoolValue;

#else //!__cplusplus
#error C++ compiler required to compile as_value_api.hpp
#endif //__cplusplus

#endif //AS_VALUE_API_HPP_GUARD

#undef AS_VALUE_API_HPP_RECURSE_GUARD
#endif //AS_VALUE_API_HPP_RECURSE_GUARD