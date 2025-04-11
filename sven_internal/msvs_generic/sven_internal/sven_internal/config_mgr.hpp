/**
 * Copyright - xWhitey, 2023.
 * config_mgr.hpp - description
 */

#ifdef CONFIG_MGR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in config_mgr.hpp
#else //CONFIG_MGR_HPP_RECURSE_GUARD

#define CONFIG_MGR_HPP_RECURSE_GUARD

#ifndef CONFIG_MGR_HPP_GUARD
#define CONFIG_MGR_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "SimpleIni.h"

typedef struct CConfigMgr {
	CConfigMgr(_In_z_ char* _FileName);

	const char* GetValue(_In_z_ const char* _Section, _In_z_ const char* _Key);
	void SetValue(_In_z_ const char* _Section, _In_z_ const char* _Key, _In_z_ const char* _Value);
	void Save();

	CSimpleIniA* m_pIni;
	const char* m_pszFileName;
	bool m_bExistedOnLaunch, m_bLoadFailed;
} CConfigMgr;

using CConfigMgr = struct CConfigMgr;

#else //!__cplusplus
#error C++ compiler required to compile config_mgr.hpp
#endif //__cplusplus

#endif //CONFIG_MGR_HPP_GUARD

#undef CONFIG_MGR_HPP_RECURSE_GUARD
#endif //CONFIG_MGR_HPP_RECURSE_GUARD