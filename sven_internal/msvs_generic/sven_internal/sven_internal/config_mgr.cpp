#include "StdAfx.h"
#include "config_mgr.hpp"

CConfigMgr::CConfigMgr(_In_z_ char* _FileName) : m_pszFileName(_FileName) {
	m_pIni = Q_new(CSimpleIniA)();
	m_bExistedOnLaunch = false;

	if (m_pIni->LoadFile(_FileName) < 0) {
		if (CCheat::GetCheat())
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CConfigMgr::CConfigMgr(char*): Failed to load %s file: %d\n", _FileName, errno);
		m_bLoadFailed = true;
	}
}

const char* CConfigMgr::GetValue(_In_z_ const char* _Section, _In_z_ const char* _Key) {
	return m_pIni->GetValue(_Section, _Key, "Default");
}

void CConfigMgr::SetValue(_In_z_ const char* _Section, _In_z_ const char* _Key, _In_z_ const char* _Value) {
	m_pIni->SetValue(_Section, _Key, _Value, nullptr, true);
}

void CConfigMgr::Save() {
	m_pIni->SaveFile(m_pszFileName);
}