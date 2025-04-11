#include "StdAfx.h"

#ifndef BYTE_SIZE_IN_BITS
#define BYTE_SIZE_IN_BITS 8
#endif //BYTE_SIZE_IN_BITS

#ifndef CHAR_BIT
#define CHAR_BIT (sizeof(char) * BYTE_SIZE_IN_BITS)
#endif //CHAR_BIT

#ifndef FLOAT_STR_SIZE
#define FLOAT_STR_SIZE (sizeof(float) * CHAR_BIT / 3 + 18)
#endif //FLOAT_STR_SIZE

#ifndef INT_STR_SIZE
#define INT_STR_SIZE (sizeof(int) * CHAR_BIT / 3 + 3)
#endif //INT_STR_SIZE

CValue::CValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_ EValueType _ValueType) : m_pszName(_Name), m_pszConvarName(_ConvarName) {
	m_pfnOnceValueChangedCB = nullptr;
	m_pConVar = nullptr;
	m_pOnceValueChangedCBUserData = nullptr;
	m_eFlags = kValueFlagNone;
	m_eType = _ValueType;
}

void CValue::AddFlag(_In_ EValueFlag _Flag) {
	m_eFlags |= _Flag;
}

void CValue::TakeFlag(_In_ EValueFlag _Flag) {
	m_eFlags &= ~_Flag;
}

bool CValue::HasFlag(_In_ EValueFlag _Flag) {
	return (m_eFlags & _Flag) != 0;
}

CVirtualConVar* CValue::GetConVar() {
	return m_pConVar;
}

bool& CBoolValue::Get() {
	return m_bCurrentValue;
}

float& CFloatValue::Get() {
	return m_fCurrentValue;
}

int& CIntegerValue::Get() {
	return m_iCurrentValue;
}

int CListValue::Get() {
	return m_nCurrentIdx;
}

const char* CTextValue::Get() {
	return m_pszText;
}

float* CColourValue::Get() {
	return m_aflRGBA;
}

void CValue::RegisterOnceValueChangedCallback(_In_ _tOnceChangedCallback _Callback, _In_opt_ void* _UserData) {
	m_pfnOnceValueChangedCB = _Callback;
	m_pOnceValueChangedCBUserData = _UserData;
}

CBoolValue::CBoolValue(_In_ const char* _Name, _In_z_ char* _ConvarName, _In_ bool _DefaultValue) : CValue(_Name, _ConvarName, kValueTypeBool) {
	m_bDefaultValue = _DefaultValue;
	m_bCurrentValue = _DefaultValue;

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (!strcmp(config_value, "Default")) {
			m_bCurrentValue = _DefaultValue;
		} else if (!_strnicmp(config_value, "true", 4)) m_bCurrentValue = true; else m_bCurrentValue = false;
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, _DefaultValue ? "1" : "0"));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CBoolValue* thiz = reinterpret_cast<CBoolValue*>(_UserArg);

		bool value = *_Value[0] == '1';

		if (!value && !_strnicmp(*_Value, "true", 4)) value = true;

		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &value, &thiz->m_bCurrentValue);

		thiz->m_bCurrentValue = value;
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

void CBoolValue::Set(_In_ const bool& _Value) {
	m_bCurrentValue = _Value;

	m_pConVar->DirectSet(_Value ? "1" : "0");
	CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
}

CFloatValue::CFloatValue(_In_ const char* _Name, _In_z_ char* _ConvarName, _In_ float _Min, _In_ float _Max, _In_ float _DefaultValue) : CValue(_Name, _ConvarName, kValueTypeFloat) {
	m_fMinValue = _Min;
	m_fMaxValue = _Max;
	m_fDefaultValue = _DefaultValue;
	m_fCurrentValue = _DefaultValue;

	char* buffer = indirect_cast<char*>(Q_malloc(FLOAT_STR_SIZE + 1));
	sprintf_s(buffer, FLOAT_STR_SIZE + 1, "%f", _DefaultValue);

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (!strcmp(config_value, "Default")) {
			m_fCurrentValue = _DefaultValue;
		} else m_fCurrentValue = atof(config_value);
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, buffer));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CFloatValue* thiz = reinterpret_cast<CFloatValue*>(_UserArg);

		float realValue = atof(*_Value);

		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &realValue, &thiz->m_fCurrentValue);

		thiz->m_fCurrentValue = realValue;
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

void CFloatValue::Set(_In_ const float& _Value) {
	m_fCurrentValue = _Value;

	char* buffer = indirect_cast<char*>(Q_malloc(FLOAT_STR_SIZE + 1));
	sprintf_s(buffer, FLOAT_STR_SIZE + 1, "%f", _Value);

	m_pConVar->DirectSet(buffer, true);
	CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	//Q_free(buffer);
}

CIntegerValue::CIntegerValue(_In_ const char* _Name, _In_z_ char* _ConvarName, _In_ int _Min, _In_ int _Max, _In_ int _DefaultValue) : CValue(_Name, _ConvarName, kValueTypeInteger) {
	m_iMinValue = _Min;
	m_iMaxValue = _Max;
	m_iDefaultValue = _DefaultValue;
	m_iCurrentValue = _DefaultValue;

	char* buffer = indirect_cast<char*>(Q_malloc(INT_STR_SIZE + 1));
	sprintf_s(buffer, INT_STR_SIZE + 1, "%d", _DefaultValue);

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (!strcmp(config_value, "Default")) {
			m_iCurrentValue = _DefaultValue;
		} else m_iCurrentValue = atoi(config_value);
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, buffer));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CIntegerValue* thiz = reinterpret_cast<CIntegerValue*>(_UserArg);

		int realValue = atoi(*_Value);

		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &realValue, &thiz->m_iCurrentValue);

		thiz->m_iCurrentValue = realValue;
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

void CIntegerValue::Set(_In_ const int& _Value) {
	m_iCurrentValue = _Value;

	char* buffer = indirect_cast<char*>(Q_malloc(INT_STR_SIZE + 1));
	sprintf_s(buffer, INT_STR_SIZE + 1, "%d", _Value);

	m_pConVar->DirectSet(buffer, true);
	CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	//Q_free(buffer);
}

bool Items_SingleStringGetter(void* data, int idx, const char** out_text) {
	// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

CListValue::CListValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_ const char* _Modes, _In_ int _DefaultMode) : CValue(_Name, _ConvarName, kValueTypeList), m_pszModes(std::move(_Modes)) {
	m_nCurrentIdx = _DefaultMode;
	m_rgpszModes = Q_new(CVector<const char*>)({});

	int items_count = 0;
	const char* p = _Modes; 
	while (*p) {
		p += strlen(p) + 1;
		items_count++;
	}

	const char* preview_value = NULL;
	for (int idx = 0; idx >= 0 && idx < items_count; idx++) {
		Items_SingleStringGetter((void*)_Modes, idx, &preview_value);
		m_rgpszModes->push_back(preview_value);
	}

	char* buffer = indirect_cast<char*>(Q_malloc(INT_STR_SIZE + 1));
	sprintf_s(buffer, INT_STR_SIZE + 1, "%d", _DefaultMode);

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (!strcmp(config_value, "Default")) {
			m_nCurrentIdx = _DefaultMode;
		} else m_nCurrentIdx = atoi(config_value);
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, buffer));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CListValue* thiz = reinterpret_cast<CListValue*>(_UserArg);

		int realValue = atoi(*_Value);

		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &realValue, &thiz->m_nCurrentIdx);

		thiz->m_nCurrentIdx = realValue;
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

void CListValue::SetConVarValue(_In_ const int& _Value) {
	char* buffer = indirect_cast<char*>(Q_malloc(INT_STR_SIZE + 1));
	sprintf_s(buffer, INT_STR_SIZE + 1, "%d", _Value);

	m_pConVar->DirectSet(buffer, true);
	CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();

	//Q_free(buffer);
}

const char* CListValue::GetModeByIndex(_In_ const int& _Index) {
	if (_Index > m_rgpszModes->size()) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CListValue::GetModeByIndex(const int&): index is out of range!\n");

		return "";
	}

	return m_rgpszModes->at(_Index);
}

CTextValue::CTextValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_z_ const char* _DefaultValue) : CValue(_Name, _ConvarName, kValueTypeText) {
	m_pszText = indirect_cast<char*>(Q_malloc(sizeof(char) * 256));
	m_pszText[sizeof(char) * 256] = '\0';
	strncpy_s(m_pszText, sizeof(char) * 256, _DefaultValue, sizeof(char) * 256 - 1);

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (strcmp(config_value, "Default")) {
			strncpy_s(m_pszText, sizeof(char) * 256, config_value, sizeof(char) * 256 - 1);
			auto iConfigValLen = strlen(config_value);
			m_pszText[iConfigValLen] = '\0';
		}
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, m_pszText));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CTextValue* thiz = reinterpret_cast<CTextValue*>(_UserArg);

		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, _Value, &thiz->m_pszText);

		strncpy_s(thiz->m_pszText, sizeof(char) * 256, *_Value, sizeof(char) * 256 - 1);
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

void CTextValue::Set(_In_z_ char* _Text) {
	strncpy_s(m_pszText, sizeof(char) * 256, _Text, sizeof(char) * 256 - 1);
}

CColourValue::CColourValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_opt_ _tOnChangedCallback _pfnCB, _In_opt_ void* _CBUserData, _In_ float _Red, _In_ float _Green, _In_ float _Blue, _In_opt_ float _Alpha /* = 1.f */) : CValue(_Name, _ConvarName, kValueTypeColour) {
	m_aflRGBA[0] = _Red;
	m_aflRGBA[1] = _Green;
	m_aflRGBA[2] = _Blue;
	m_aflRGBA[3] = _Alpha;
	m_pfnOnceChangedCB = _pfnCB;
	m_pCallbackUserData = _CBUserData;

	char* buffer = indirect_cast<char*>(Q_malloc(sizeof(char) * 256));
	buffer[sizeof(char) * 256] = '\0';

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (!strcmp(config_value, "Default")) {
			sprintf_s(buffer, sizeof(char) * 256, "raw %f %f %f %f", _Red, _Green, _Blue, _Alpha);
			m_aflRGBA[0] = _Red;
			m_aflRGBA[1] = _Green;
			m_aflRGBA[2] = _Blue;
			m_aflRGBA[3] = _Alpha;
		} else {
			strcpy_s(buffer, sizeof(char) * 256, config_value);

			float r = 0, g = 0, b = 0;
			float a = 255;

			if (!_strnicmp(config_value, "raw", 3)) {
				float r = 0.f, g = 0.f, b = 0.f;
				float a = 1.f;

				sscanf_s(config_value, "raw %f %f %f %f", &r, &g, &b, &a);

				m_aflRGBA[0] = r;
				m_aflRGBA[1] = g;
				m_aflRGBA[2] = b;
				m_aflRGBA[3] = a;
				if (m_pfnOnceChangedCB) m_pfnOnceChangedCB(m_pCallbackUserData, m_aflRGBA);

				CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
			}
			else {
				int r = 0, g = 0, b = 0;
				int a = 255;

				sscanf_s(config_value, "%d %d %d %d", &r, &g, &b, &a);

				ImColor transition = ImColor(r, g, b, a);

				m_aflRGBA[0] = transition.Value.x;
				m_aflRGBA[1] = transition.Value.y;
				m_aflRGBA[2] = transition.Value.z;
				m_aflRGBA[3] = transition.Value.w;
				if (m_pfnOnceChangedCB) m_pfnOnceChangedCB(m_pCallbackUserData, m_aflRGBA);
			}
		}
	} else {
		sprintf_s(buffer, sizeof(char) * 256, "raw %f %f %f %f", _Red, _Green, _Blue, _Alpha);
		m_aflRGBA[0] = _Red;
		m_aflRGBA[1] = _Green;
		m_aflRGBA[2] = _Blue;
		m_aflRGBA[3] = _Alpha;
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, buffer));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CColourValue* thiz = reinterpret_cast<CColourValue*>(_UserArg);

		if (!_strnicmp(*_Value, "raw", 3)) {
			float r = 0.f, g = 0.f, b = 0.f;
			float a = 1.f;

			sscanf_s(*_Value, "raw %f %f %f %f", &r, &g, &b, &a);
			float copy[4] = { r, g, b, a };

			if (thiz->m_pfnOnceChangedCB) thiz->m_pfnOnceChangedCB(thiz->m_pCallbackUserData, copy);
			if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &copy, &thiz->m_aflRGBA);

			thiz->m_aflRGBA[0] = copy[0];
			thiz->m_aflRGBA[1] = copy[1];
			thiz->m_aflRGBA[2] = copy[2];
			thiz->m_aflRGBA[3] = copy[3];

			CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
			return;
		}

		int r = 0, g = 0, b = 0;
		int a = 255;
		
		sscanf_s(*_Value, "%d %d %d %d", &r, &g, &b, &a);

		ImColor transition = ImColor(r, g, b, a);

		float copy[4] = { 0.f, 0.f, 0.f, 0.f };
		copy[0] = transition.Value.x;
		copy[1] = transition.Value.y;
		copy[2] = transition.Value.z;
		copy[3] = transition.Value.w;

		if (thiz->m_pfnOnceChangedCB) thiz->m_pfnOnceChangedCB(thiz->m_pCallbackUserData, copy);
		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &copy, &thiz->m_aflRGBA);

		thiz->m_aflRGBA[0] = copy[0];
		thiz->m_aflRGBA[1] = copy[1];
		thiz->m_aflRGBA[2] = copy[2];
		thiz->m_aflRGBA[3] = copy[3];

		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

CExtraValues::CExtraValues(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_ _tWindowRenderCallback _pfnCB, _In_opt_ void* _CBUserData) : CValue(_Name, _ConvarName, kValueTypeExtra) {
	m_pfnWindowRenderCB = _pfnCB;
	m_pCallbackUserData = _CBUserData;

	CConfigMgr* convars = CCheat::GetCheat()->m_pModuleManager->m_pValuesManager;

	if (convars->m_bExistedOnLaunch == true) {
		const char* config_value = convars->GetValue("ConVars", _ConvarName);
		if (!strcmp(config_value, "Default")) {
			m_bActive = false;
		} else {
			if (!_strnicmp(config_value, "true", 4)) m_bActive = true; else m_bActive = false;
		}
	} else {
		m_bActive = false;
	}

	CCheat::GetCheat()->m_pConsole->RegisterConVar(m_pConVar = Q_new(CVirtualConVar)(_ConvarName, "0"));
	m_pConVar->RegisterCallback([](_In_ void* _UserArg, _In_ char** _Value) {
		CExtraValues* thiz = reinterpret_cast<CExtraValues*>(_UserArg);

		bool value = *_Value[0] == '1';

		if (!value && !_strnicmp(*_Value, "true", 4)) value = true;

		if (thiz->m_pfnOnceValueChangedCB) thiz->m_pfnOnceValueChangedCB(thiz->m_pOnceValueChangedCBUserData, &value, &thiz->m_bActive);

		thiz->m_bActive = value;
		CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
	}, this);
}

bool CExtraValues::Get() {
	return m_bActive;
}

void CExtraValues::Set(_In_ const bool& _Value) {
	m_bActive = _Value;

	m_pConVar->DirectSet(_Value ? "1" : "0");
	CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
}