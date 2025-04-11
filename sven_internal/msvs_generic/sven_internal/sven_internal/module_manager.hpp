/**
 * Copyright - xWhitey, 2023.
 * module_manager.hpp - description
 */

#ifdef MODULE_MANAGER_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in module_manager.hpp
#else //MODULE_MANAGER_HPP_RECURSE_GUARD

#define MODULE_MANAGER_HPP_RECURSE_GUARD

#ifndef MODULE_MANAGER_HPP_GUARD
#define MODULE_MANAGER_HPP_GUARD
#pragma once

#ifdef __cplusplus

//typedef struct CModule CModule;

typedef struct CModuleManager {
	CModuleManager();
	~CModuleManager();

	void RegisterModule(_In_ CModule*);
	void RegisterScriptModule(_In_ CModule*);

	void RegisterModules();

	void UnregisterModules();

	template<class _Ty> _Ty* GetModule() {
		CVector<CModule*>::iterator beg = m_pVecModules->begin();
		for (beg; beg != m_pVecModules->end(); ++beg) {
			CModule* lpModule = *beg;
			if (lpModule->IsScriptModule()) continue; //CBaseScriptModule doesn't have RTTI
			if (auto m = dynamic_cast<functional::remove_pointer_t<_Ty>*>(lpModule)) {
				return m;
			}
		}

		return nullptr;
	}

	CModule* GetModuleByName(_In_z_ const char* _Name);

	bool DoesCategoryExist(_In_z_ const char* _Which);

	CVector<CModule*>* GetModulesByCategory(_In_z_ _Pre_z_ const char* _Category);

	CVector<const char*>* GetCategories() const;

	void EnableModulesFromConfigFile();
	void SaveModulesStatesToConfigFile();

	void LoadValuesFromConfigFile();
	void SaveValuesToConfigFile();

	void LoadKeybindsFromConfigFile();
	void SaveKeybindsToConfigFile();

	void SortModules();

	CConfigMgr* m_pConfigManager;
	CConfigMgr* m_pValuesManager;
	CVector<CModule*>* m_pVecModules;
	CVector<CModule*>* m_pVecScriptModules;
	CVector<const char*>* m_pVecCategories;
	CVector<CModule*>* m_pVecSortedModules;
	CConfigMgr* m_pKeybindsManager;
} CModuleManager;

#else //__cplusplus
#error C++ compiler required to compile module_manager.hpp.
#endif //__cplusplus

#endif //MODULE_MANAGER_HPP_GUARD

#undef MODULE_MANAGER_HPP_RECURSE_GUARD
#endif //MODULE_MANAGER_HPP_RECURSE_GUARD