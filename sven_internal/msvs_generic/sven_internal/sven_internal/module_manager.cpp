#include "StdAfx.h"

#include "CDoubletapModule.hpp"
#include "CSpeedModule.hpp"
#include "CClickGUIModule.hpp"
#include "CFakeLagModule.hpp"
#ifdef XWHITEY_PROPRIETARY
#include "CServerCrasherModule.hpp"
#endif //XWHITEY_PROPRIETARY
#include "CBreakLCModule.hpp"
#include "CAirStuckModule.hpp"
#include "CFrameSkipperModule.hpp"
#include "CAutoJumpModule.hpp"
#include "CApplicationSpeedModule.hpp"
#include "CFastCrowbarModule.hpp"
#include "CDebugModule.hpp"
#include "CAutoJumpbugModule.hpp"
#include "CTimeMachineModule.hpp"
#include "CAntiAimModule.hpp"
#include "CInstantRespawnModule.hpp"
#include "CBobbingModule.hpp"
#include "CAutoTertiaryAttackGlitchModule.hpp"
#ifdef XWHITEY_PROPRIETARY
#include "CSlideWalkModule.hpp"
#endif //XWHITEY_PROPRIETARY
#include "CSelfSinkModule.hpp"
#include "CESPModule.hpp"
#include "CAutoFreezeOnChatModule.hpp"
#include "CBacktrackModule.hpp"
#include "CFakeLatencyModule.hpp"
#include "CForwardtrackModule.hpp"
#ifdef XWHITEY_PROPRIETARY
#include "CRagebotModule.hpp"
#endif //XWHITEY_PROPRIETARY
#include "CAntiSlowhackModule.hpp"
#include "CDiscordRPCModule.hpp"
#ifdef XWHITEY_PROPRIETARY
#include "CMovementCommandRateLimiterModule.hpp"
#endif //XWHITEY_PROPRIETARY
#include "CStrafeModule.hpp"
#include "CMessageStealerModule.hpp"
#ifdef XWHITEY_PROPRIETARY
#include "CDisablerModule.hpp"
#endif //XWHITEY_PROPRIETARY
#include "CConsoleSpammerModule.hpp"
#ifdef XWHITEY_PROPRIETARY
#include "CStresserModule.hpp"
#include "CKickerModule.hpp"
#endif //XWHITEY_PROPRIETARY

#include "CHUDModule.hpp"

#include "CClientDesignerModule.hpp"
#include "script_module.hpp"

CModuleManager::CModuleManager() {
	//Creating config files if they don't exist - xWhitey
	bool bModulesFileExisted = false, bConVarsFileExisted = false, bKeybindsFileExisted = false;
	
	if (!CFileMgr::DoesFileExist("./lightning.tech/modules.ini"))
		CFileMgr::CloseFile(CFileMgr::OpenFile("./lightning.tech/modules.ini", "a+"));
	else 
		bModulesFileExisted = true;

	if (!CFileMgr::DoesFileExist("./lightning.tech/convars.ini"))
		CFileMgr::CloseFile(CFileMgr::OpenFile("./lightning.tech/convars.ini", "a+"));
	else 
		bConVarsFileExisted = true;

	if (!CFileMgr::DoesFileExist("./lightning.tech/keybinds.ini"))
		CFileMgr::CloseFile(CFileMgr::OpenFile("./lightning.tech/keybinds.ini", "a+"));
	else
		bKeybindsFileExisted = true;

	m_pConfigManager = Q_new(CConfigMgr)("./lightning.tech/modules.ini");
	m_pConfigManager->m_bExistedOnLaunch = bModulesFileExisted;
	m_pValuesManager = Q_new(CConfigMgr)("./lightning.tech/convars.ini");
	m_pValuesManager->m_bExistedOnLaunch = bConVarsFileExisted;
	m_pKeybindsManager = Q_new(CConfigMgr)("./lightning.tech/keybinds.ini");
	m_pKeybindsManager->m_bExistedOnLaunch = bKeybindsFileExisted;

	m_pVecModules = Q_new(CVector<CModule*>)({});
	m_pVecScriptModules = Q_new(CVector<CModule*>)({});
	m_pVecCategories = Q_new(CVector<const char*>)({});

	m_pVecSortedModules = nullptr;
}

CModuleManager::~CModuleManager() {
	m_pConfigManager->Save();
	m_pValuesManager->Save();
	m_pKeybindsManager->Save();
	UnregisterModules();
	Q_delete(m_pVecModules);
	Q_delete(m_pVecScriptModules);
	Q_delete(m_pVecCategories);
	if (m_pVecSortedModules) {
		Q_delete(m_pVecSortedModules);
	}
}

CModule* CModuleManager::GetModuleByName(_In_z_ const char* _Name) {
	for (int idx = 0; idx < m_pVecModules->size(); idx++) {
		CModule* lpModule = m_pVecModules->at(idx);

		if (!_stricmp(lpModule->m_pszName, _Name))
			return lpModule;
	}
	for (int idx = 0; idx < m_pVecScriptModules->size(); idx++) {
		CModule* lpModule = m_pVecScriptModules->at(idx);

		if (!_stricmp(lpModule->m_pszName, _Name))
			return lpModule;
	}

	return nullptr;
} 

void CModuleManager::EnableModulesFromConfigFile() {
	CSimpleIniA::TNamesDepend values;
	m_pConfigManager->m_pIni->GetAllKeys("States", values);
	for (CSimpleIniA::TNamesDepend::const_iterator it = values.begin(); it != values.end(); ++it) {
		CModule* pModule = GetModuleByName(it->pItem);
		if (!pModule) continue;

		const char* pszValue = m_pConfigManager->GetValue("States", it->pItem);

		if (!Q_strnicmp_s(pszValue, sizeof("true"), "true", sizeof("true") - 1)) {
			pModule->SetState(true, true);
			CCheat::GetCheat()->m_pModuleManager->SortModules();
		}
	}
}

void CModuleManager::SaveModulesStatesToConfigFile() {
	for (int idx = 0; idx < m_pVecModules->size(); idx++) {
		CModule* lpModule = m_pVecModules->at(idx);

		m_pConfigManager->SetValue("States", lpModule->m_pszName, lpModule == g_pClickGUIModule ? "false" : lpModule->m_bState == true ? "true" : "false");
	}
	for (int idx = 0; idx < m_pVecScriptModules->size(); idx++) {
		CModule* lpModule = m_pVecScriptModules->at(idx);

		m_pConfigManager->SetValue("States", lpModule->m_pszName, lpModule == g_pClickGUIModule ? "false" : lpModule->m_bState == true ? "true" : "false");
	}

	m_pConfigManager->Save();
}

void CModuleManager::LoadValuesFromConfigFile() {
	CSimpleIniA::TNamesDepend values;
	m_pValuesManager->m_pIni->GetAllKeys("ConVars", values);

	size_t cbBufferLength = sizeof(char) * 256;

	for (CSimpleIniA::TNamesDepend::const_iterator it = values.begin(); it != values.end(); ++it) {
		const char* pszValue = m_pValuesManager->GetValue("ConVars", it->pItem);

		char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferLength));
		memset(szBuffer, 0, cbBufferLength);
		_snprintf_s(szBuffer, cbBufferLength, _TRUNCATE, "%s %s", it->pItem, pszValue);

		CCheat::GetCheat()->m_pConsole->InsertText(szBuffer, false);

		Q_free(szBuffer);
	}
}

void CModuleManager::LoadKeybindsFromConfigFile() {
	CSimpleIniA::TNamesDepend values;
	m_pKeybindsManager->m_pIni->GetAllKeys("Keybinds", values);
	for (CSimpleIniA::TNamesDepend::const_iterator it = values.begin(); it != values.end(); ++it) {
		CModule* pModule = GetModuleByName(it->pItem);
		if (!pModule) continue;
		
		const char* pszValue = m_pKeybindsManager->GetValue("Keybinds", it->pItem);
		int iKeyBind = 0;
		int cNumScanned = sscanf_s(pszValue, "%d", &iKeyBind);
		if (cNumScanned != 1) continue;

		pModule->m_nKeyBind = iKeyBind;
	}
}

void CModuleManager::SaveKeybindsToConfigFile() {
	size_t cbBufferSize = sizeof(char) * 64;

	for (int idx = 0; idx < m_pVecModules->size(); idx++) {
		CModule* lpModule = m_pVecModules->at(idx);

		char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
		memset(szBuffer, 0, cbBufferSize);
		sprintf_s(szBuffer, cbBufferSize, "%d", lpModule->m_nKeyBind);
		m_pKeybindsManager->SetValue("Keybinds", lpModule->m_pszName, szBuffer);
		Q_free(szBuffer);
	}
	m_pKeybindsManager->Save();
	for (int idx = 0; idx < m_pVecScriptModules->size(); idx++) {
		CModule* lpModule = m_pVecScriptModules->at(idx);

		char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
		memset(szBuffer, 0, cbBufferSize);
		sprintf_s(szBuffer, cbBufferSize, "%d", lpModule->m_nKeyBind);
		m_pKeybindsManager->SetValue("Keybinds", lpModule->m_pszName, szBuffer);
		Q_free(szBuffer);
	}
	m_pKeybindsManager->Save();
}

void CModuleManager::SaveValuesToConfigFile() {
	size_t cbBufferSize;

	for (int idx = 0; idx < m_pVecModules->size(); idx++) {
		CModule* pModule = m_pVecModules->at(idx);

		for (CValue** pBegin = pModule->m_pVecValues->begin(); pBegin != pModule->m_pVecValues->end(); ++pBegin) {
			if (!pBegin) {
				break;
			}

			CValue* pValue = *pBegin;
			if (!pValue) continue;

			if (pValue->HasFlag(kValueFlagDontSave)) continue;

			if (pValue->m_eType == kValueTypeBool) {
				CBoolValue* pBool = static_cast<CBoolValue*>(pValue);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, pBool->Get() ? "true" : "false");
			} else if (pValue->m_eType == kValueTypeFloat) {
				CFloatValue* pFloat = static_cast<CFloatValue*>(pValue);

				cbBufferSize = sizeof(char) * 64;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "%f", pFloat->Get());

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeInteger) {
				CIntegerValue* pInteger = static_cast<CIntegerValue*>(pValue);

				cbBufferSize = sizeof(char) * 64;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "%d", pInteger->Get());

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeList) {
				CListValue* pCombo = static_cast<CListValue*>(pValue);

				cbBufferSize = sizeof(char) * 64;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "%d", pCombo->Get());

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeText) {
				CTextValue* pText = static_cast<CTextValue*>(pValue);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, pText->Get());
			} else if (pValue->m_eType == kValueTypeColour) {
				CColourValue* picker = static_cast<CColourValue*>(pValue);

				cbBufferSize = sizeof(char) * 128;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "raw %f %f %f %f", picker->m_aflRGBA[0], picker->m_aflRGBA[1], picker->m_aflRGBA[2], picker->m_aflRGBA[3]);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeExtra) {
				CExtraValues* pExtra = static_cast<CExtraValues*>(pValue);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, pExtra->Get() ? "true" : "false");
			}
		}
	}

	for (int idx = 0; idx < m_pVecScriptModules->size(); idx++) {
		CModule* pModule = m_pVecScriptModules->at(idx);

		for (CValue** pBegin = pModule->m_pVecValues->begin(); pBegin != pModule->m_pVecValues->end(); ++pBegin) {
			if (!pBegin) {
				break;
			}

			CValue* pValue = *pBegin;
			if (!pValue) continue;

			if (pValue->HasFlag(kValueFlagDontSave)) continue;

			if (pValue->m_eType == kValueTypeBool) {
				CBoolValue* pBool = static_cast<CBoolValue*>(pValue);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, pBool->Get() ? "true" : "false");
			} else if (pValue->m_eType == kValueTypeFloat) {
				CFloatValue* pFloat = static_cast<CFloatValue*>(pValue);

				cbBufferSize = sizeof(char) * 64;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "%f", pFloat->Get());

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeInteger) {
				CIntegerValue* pInteger = static_cast<CIntegerValue*>(pValue);

				cbBufferSize = sizeof(char) * 64;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "%d", pInteger->Get());

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeList) {
				CListValue* pCombo = static_cast<CListValue*>(pValue);

				cbBufferSize = sizeof(char) * 64;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "%d", pCombo->Get());

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeText) {
				CTextValue* pText = static_cast<CTextValue*>(pValue);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, pText->Get());
			} else if (pValue->m_eType == kValueTypeColour) {
				CColourValue* picker = static_cast<CColourValue*>(pValue);

				cbBufferSize = sizeof(char) * 128;

				char* szBuffer = indirect_cast<char*>(Q_malloc(cbBufferSize));
				memset(szBuffer, 0, cbBufferSize);
				_snprintf_s(szBuffer, cbBufferSize, _TRUNCATE, "raw %f %f %f %f", picker->m_aflRGBA[0], picker->m_aflRGBA[1], picker->m_aflRGBA[2], picker->m_aflRGBA[3]);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, szBuffer);

				Q_free(szBuffer);
			} else if (pValue->m_eType == kValueTypeExtra) {
				CExtraValues* pExtra = static_cast<CExtraValues*>(pValue);

				m_pValuesManager->SetValue("ConVars", pValue->m_pszConvarName, pExtra->Get() ? "true" : "false");
			}
		}
	}

	m_pValuesManager->Save();
}

bool CModuleManager::DoesCategoryExist(_In_z_ const char* _Which) {
	for (int idx = 0; idx < m_pVecCategories->size(); idx++) {
		const char* szCategory = m_pVecCategories->at(idx);
		if (!szCategory) continue;

		size_t cbCategoryLength = Q_strlen(szCategory);

		if (!Q_strncmp_s(szCategory, cbCategoryLength, _Which, cbCategoryLength)) return true;
	}

	return false;
}

void CModuleManager::RegisterModule(_In_ CModule* _Module) {
	if (!DoesCategoryExist(_Module->m_pszCategory)) m_pVecCategories->push_back(_Module->m_pszCategory);

	m_pVecModules->push_back(_Module);
}

void CModuleManager::RegisterScriptModule(_In_ CModule* _Module) {
	if (!DoesCategoryExist(_Module->m_pszCategory)) m_pVecCategories->push_back(_Module->m_pszCategory);

	m_pVecScriptModules->push_back(_Module);
}

void CModuleManager::RegisterModules() {
	this->RegisterModule(Q_new(CBreakLCModule)());
	this->RegisterModule(Q_new(CDoubletapModule)());
	this->RegisterModule(Q_new(CFakeLagModule)());
	this->RegisterModule(Q_new(CSpeedModule)());
	this->RegisterModule(Q_new(CAutoJumpModule)());
	this->RegisterModule(Q_new(CFrameSkipperModule)());
#ifdef XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CServerCrasherModule)());
#endif //XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CAirStuckModule)());
	this->RegisterModule(Q_new(CApplicationSpeedModule)());
	this->RegisterModule(Q_new(CFastCrowbarModule)());
	this->RegisterModule(Q_new(CDebugModule)());
	this->RegisterModule(Q_new(CAutoJumpbugModule)());
	this->RegisterModule(Q_new(CTimeMachineModule)());
	this->RegisterModule(Q_new(CAntiAimModule)());
	this->RegisterModule(Q_new(CInstantRespawnModule)());
	this->RegisterModule(Q_new(CBobbingModule)());
	this->RegisterModule(Q_new(CAutoTertiaryAttackGlitchModule)());
#ifdef XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CSlideWalkModule)());
#endif //XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CSelfSinkModule)());
	this->RegisterModule(Q_new(CESPModule)());
	this->RegisterModule(Q_new(CAutoFreezeOnChatModule)());
	this->RegisterModule(Q_new(CBacktrackModule)());
	this->RegisterModule(Q_new(CFakeLatencyModule)());
	this->RegisterModule(Q_new(CForwardtrackModule)());
	this->RegisterModule(Q_new(CClientDesignerModule)());
	this->RegisterModule(Q_new(CHUDModule)());
	this->RegisterModule(Q_new(CClickGUIModule)());
#ifdef XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CRagebotModule)());
#endif //XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CAntiSlowhackModule)());
	this->RegisterModule(Q_new(CDiscordRPCModule)());
#ifdef XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CMovementCommandRateLimiterModule)());
#endif //XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CStrafeModule)());
	this->RegisterModule(Q_new(CMessageStealerModule)());
#ifdef XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CDisablerModule)());
#endif //XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CConsoleSpammerModule)());
#ifdef XWHITEY_PROPRIETARY
	this->RegisterModule(Q_new(CStresserModule)());
	this->RegisterModule(Q_new(CKickerModule)());
#endif //XWHITEY_PROPRIETARY
}

void CModuleManager::UnregisterModules() {
	for (CModule** ppBegin = m_pVecModules->begin(); ppBegin != NULL && ppBegin != m_pVecModules->end(); ++ppBegin) {
		if (!ppBegin) continue;

		CModule* pModule = *ppBegin;
		if (!pModule) continue;

		if (pModule->m_bState) {
			pModule->Toggle();
		}

		CCheat::GetCheat()->m_pEventBus->UnregisterListener(pModule);
		Q_delete(pModule);
	}

	for (CModule** ppBegin = m_pVecScriptModules->begin(); ppBegin != NULL && ppBegin != m_pVecModules->end(); ++ppBegin) {
		if (!ppBegin) continue;

		CModule* pModule = *ppBegin;
		if (!pModule) continue;

		if (pModule->m_bState) {
			pModule->Toggle();
		}

		CCheat::GetCheat()->m_pEventBus->UnregisterListener(pModule);
		CBaseScriptModule* pScriptModule = static_cast<CBaseScriptModule*>(pModule);
		pScriptModule->Release(); //notify garbage collector (is this even needed?)
		Q_delete(pModule);
	}
	m_pVecModules->clear();
	m_pVecScriptModules->clear();
}

void CModuleManager::SortModules() {
	if (m_pVecSortedModules) {
		Q_delete(m_pVecSortedModules);
		m_pVecSortedModules = nullptr;
	}
	m_pVecSortedModules = Q_new(CVector<CModule*>)({});

	for (int idx = 0; idx < m_pVecModules->size(); idx++) {
		CModule* pModule = m_pVecModules->at(idx);
		if (!pModule) continue;
		if (pModule->m_bShownInArrayList == false || pModule->m_bState == false) continue;

		m_pVecSortedModules->push_back(pModule);
	}

	for (int idx = 0; idx < m_pVecScriptModules->size(); idx++) {
		CModule* pModule = m_pVecScriptModules->at(idx);
		if (!pModule) continue;
		if (pModule->m_bShownInArrayList == false || pModule->m_bState == false) continue;

		m_pVecSortedModules->push_back(pModule);
	}

	std::sort(m_pVecSortedModules->begin(), m_pVecSortedModules->end(), [](CModule* _First, CModule* _Second) {
		char szBuffer1[512];
		char szBuffer2[512];
		sprintf_s(szBuffer1, "%s%s", _First->m_pszName, _First->GetTag());
		sprintf_s(szBuffer2, "%s%s", _Second->m_pszName, _Second->GetTag());
		ImVec2 vecFirstSize = g_pClientFont->CalcTextSizeA(25.f, FLT_MAX, 0.0f, szBuffer1);
		ImVec2 vecSecondSize = g_pClientFont->CalcTextSizeA(25.f, FLT_MAX, 0.0f, szBuffer2);
		return vecFirstSize.x > vecSecondSize.x;
	});
}

CVector<CModule*>* CModuleManager::GetModulesByCategory(_In_z_ _Pre_z_ const char* _Category) {
	CVector<CModule*>* result = Q_new(CVector<CModule*>)({});

	if (!DoesCategoryExist(_Category)) {
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CModuleManager::GetModulesByCategory(char const*): _Category (%s) doesn't exist!\n", _Category);

		return result;
	}

	for (int idx = 0; idx < m_pVecModules->size(); idx++) {
		CModule* pModule = m_pVecModules->at(idx);
		if (!pModule) continue;
		if (!pModule->m_pszCategory) continue;

		size_t cbCategoryLength = Q_strlen(pModule->m_pszCategory);

		if (!Q_strncmp_s(pModule->m_pszCategory, cbCategoryLength, _Category, cbCategoryLength))
			result->push_back(pModule);
	}
	for (int idx = 0; idx < m_pVecScriptModules->size(); idx++) {
		CModule* pModule = m_pVecScriptModules->at(idx);
		if (!pModule) continue;
		if (!pModule->m_pszCategory) continue;

		size_t cbCategoryLength = Q_strlen(pModule->m_pszCategory);

		if (!Q_strncmp_s(pModule->m_pszCategory, cbCategoryLength, _Category, cbCategoryLength))
			result->push_back(pModule);
	}

	if (result->empty()) {
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CModuleManager::GetModulesByCategory(char const*): Something strange happened: specified category (%s) exists, but no modules inside it!\n", _Category);
	}

	return result;
}

CVector<const char*>* CModuleManager::GetCategories() const {
	return m_pVecCategories;
}
