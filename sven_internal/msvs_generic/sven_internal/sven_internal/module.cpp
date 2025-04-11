#include "StdAfx.h"

CModule::CModule(_In_z_ const char* _Name, _In_z_ const char* _Category, _In_opt_ int _KeyBind /* = 0x0 */, _In_opt_ bool _ShownInArrayList /* = true */) {
	m_bState = false;
	m_pszName = _Name;
	m_pszCategory = _Category;
	m_nKeyBind = _KeyBind;
	m_bShownInArrayList = _ShownInArrayList;
	m_pVecValues = Q_new(CVector<CValue*>)({});
	m_pszKeybind = "";
	if (m_nKeyBind) {
		char* szBuffer = (char*) (Q_malloc(sizeof(char) * 128));
		int iTotalKeynameLength = 0;
		unsigned int uiScancode = MapVirtualKey(m_nKeyBind, MAPVK_VK_TO_VSC);
		switch (m_nKeyBind) {
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
		case VK_RCONTROL: case VK_RMENU:
		case VK_LWIN: case VK_RWIN: case VK_APPS:
		case VK_PRIOR: case VK_NEXT:
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE:
		case VK_NUMLOCK:
			uiScancode |= KF_EXTENDED;
		}
		if ((iTotalKeynameLength = GetKeyNameTextA(uiScancode << 16, szBuffer, 128)) != 0) {
			const char* pszPreviousBuffer = m_pszKeybind;
			m_pszKeybind = szBuffer;
			if (strcmp(pszPreviousBuffer, "Unknown") && strcmp(pszPreviousBuffer, "None"))
				Q_free((void*)pszPreviousBuffer);
		} else {
			Q_free(szBuffer);
			m_pszKeybind = "Unknown";
		}
	}

	m_bInitialized = true;
}

bool CModule::IsScriptModule() {
	return false;
}

void CModule::SetState(_In_ bool _bState, _In_opt_ bool _PreLoading /* = false */) {
	if (!_PreLoading && !m_bInitialized) {
		_PreLoading = true; //super ctor hasn't been called yet so we're preloading
	}

	m_bState = _bState;

	if (_bState == true) {
		this->OnEnable();
	} else {
		this->OnDisable();
	}

	if (_PreLoading == false) {
		CCheat::GetCheat()->m_pModuleManager->SaveModulesStatesToConfigFile();
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}
}

void CModule::Toggle() {
	this->SetState(m_bState ? false : true);
}

void CModule::OnEnable() {
	CCheat::GetCheat()->m_pModuleManager->SortModules();
}

void CModule::OnDisable() {
	CCheat::GetCheat()->m_pModuleManager->SortModules();
}