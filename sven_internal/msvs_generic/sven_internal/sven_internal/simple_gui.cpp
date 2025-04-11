#include "StdAfx.h"
#include "simple_gui.hpp"
#include "CClickGUIModule.hpp"

CModuleTab::CModuleTab(CModule* _Module) {
	m_pTheModule = _Module;
	m_bOpen = false;
}

CCategoryPanel::CCategoryPanel(const char* _Name) {
	m_bOpen = false;
	m_pszName = _Name;
	m_pModules = Q_new(CVector<CModuleTab*>)({});
	CVector<CModule*>* pModules = CCheat::GetCheat()->m_pModuleManager->GetModulesByCategory(_Name);
	for (int nIndex = 0; nIndex < pModules->size(); nIndex++) {
		CModule* pModule = pModules->at(nIndex);
		m_pModules->push_back(Q_new(CModuleTab)(pModule));
	}
}

CSimpleGui::CSimpleGui() {
	m_pPanels = Q_new(CVector<CCategoryPanel*>)({});
	CModuleManager* pModuleManager = CCheat::GetCheat()->m_pModuleManager;
	for (int nIndex = 0; nIndex < pModuleManager->m_pVecCategories->size(); nIndex++) {
		const char* pszCategory = pModuleManager->m_pVecCategories->at(nIndex);
		m_pPanels->push_back(Q_new(CCategoryPanel)(pszCategory));
	}
}

void CModuleTab::Draw() {
	if (!m_bOpen) return;

	if (ImGui::Begin(m_pTheModule->m_pszName, &m_bOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		if (m_pTheModule != g_pClickGUIModule) {
			char rgszBuffer[256];
			_snprintf_s(rgszBuffer, Q_ARRAYSIZE(rgszBuffer), "Enabled##%s", m_pTheModule->m_pszName);
			bool bValue = m_pTheModule->m_bState;
			if (ImGui::Checkbox(rgszBuffer, &bValue)) {
				m_pTheModule->Toggle();
			}
		}
		if (m_pTheModule->m_nKeyBind != 0 && m_pTheModule->m_pszKeybind) {
			ImGui::Text("Keybind: %s\n", m_pTheModule->m_pszKeybind);
		}
		ImGui::Separator();
		CVector<CValue*>::iterator beg = m_pTheModule->m_pVecValues->begin();
		for (beg; beg != m_pTheModule->m_pVecValues->end(); beg++) {
			CValue* value = *beg;
			if (value->HasFlag(kValueFlagDontRender)) continue;
			auto name = value->m_pszName;

			if (value->m_eType == kValueTypeBool) {
				CBoolValue* bl = static_cast<CBoolValue*>(value);
				ImGui::PushID(value->m_pszConvarName);
				bool bValue = bl->Get();
				if (ImGui::Checkbox(name, &bValue)) {
					if (bl->m_pfnOnceValueChangedCB) bl->m_pfnOnceValueChangedCB(bl->m_pOnceValueChangedCBUserData, &bValue, &bl->m_bCurrentValue);
					bl->Set(bValue);
				}
				ImGui::PopID();
			}
			else if (value->m_eType == kValueTypeFloat) {
				CFloatValue* flt = static_cast<CFloatValue*>(value);
				ImGui::PushID(value->m_pszConvarName);
				float flValue = flt->Get();
				if (ImGui::SliderFloat(flt->m_pszName, &flValue, flt->m_fMinValue, flt->m_fMaxValue)) {
					if (flt->m_pfnOnceValueChangedCB) flt->m_pfnOnceValueChangedCB(flt->m_pOnceValueChangedCBUserData, &flValue, &flt->m_fCurrentValue);
					flt->Set(flValue);
				}
				ImGui::PopID();
			}
			else if (value->m_eType == kValueTypeInteger) {
				CIntegerValue* itg = static_cast<CIntegerValue*>(value);
				ImGui::PushID(value->m_pszConvarName);
				int iValue = itg->Get();
				if (ImGui::SliderInt(itg->m_pszName, &iValue, itg->m_iMinValue, itg->m_iMaxValue)) {
					if (itg->m_pfnOnceValueChangedCB) itg->m_pfnOnceValueChangedCB(itg->m_pOnceValueChangedCBUserData, &iValue, &itg->m_iCurrentValue);
					itg->Set(iValue);
				}
				ImGui::PopID();
			}
			else if (value->m_eType == kValueTypeList) {
				CListValue* combo = static_cast<CListValue*>(value);
				ImGui::PushID(value->m_pszConvarName);
				int nIdx = combo->m_nCurrentIdx;
				if (ImGui::Combo(combo->m_pszName, &nIdx, combo->m_pszModes)) {
					if (combo->m_pfnOnceValueChangedCB) combo->m_pfnOnceValueChangedCB(combo->m_pOnceValueChangedCBUserData, &nIdx, &combo->m_nCurrentIdx);
					combo->SetConVarValue(nIdx);
					combo->m_nCurrentIdx = nIdx;
				}
				ImGui::PopID();
			}
			else if (value->m_eType == kValueTypeText) {
				CTextValue* text = static_cast<CTextValue*>(value);
				ImGui::PushID(value->m_pszConvarName);
				ImGui::InputText(value->m_pszName, text->m_pszText, sizeof(char) * 256);
				ImGui::PopID();
			}
			else if (value->m_eType == kValueTypeColour) {
				CColourValue* picker = static_cast<CColourValue*>(value);
				ImGui::PushID(value->m_pszConvarName);
				float* copy = (float*)Q_malloc(sizeof(float) * 4);
				copy[0] = picker->m_aflRGBA[0];
				copy[1] = picker->m_aflRGBA[1];
				copy[2] = picker->m_aflRGBA[2];
				copy[3] = picker->m_aflRGBA[3];
				if (ImGui::ColorPicker4(value->m_pszName, copy)) {
					if (picker->m_pfnOnceChangedCB) {
						picker->m_pfnOnceChangedCB(picker->m_pCallbackUserData, copy);
					}
					if (picker->m_pfnOnceValueChangedCB) {
						picker->m_pfnOnceValueChangedCB(picker->m_pOnceValueChangedCBUserData, &copy, &picker->m_aflRGBA);
					}
					picker->m_aflRGBA[0] = copy[0];
					picker->m_aflRGBA[1] = copy[1];
					picker->m_aflRGBA[2] = copy[2];
					picker->m_aflRGBA[3] = copy[3];
				}
				Q_free(copy);
				ImGui::PopID();
			}
			else if (value->m_eType == kValueTypeExtra) {
				CExtraValues* extra = static_cast<CExtraValues*>(value);
				ImGui::PushID(value->m_pszConvarName);
				if (ImGui::Button(value->m_pszName)) {
					extra->Set(!extra->Get());
				}
				ImGui::PopID();
				if (extra->Get()) {
					char* buffer = (char*)Q_malloc(sizeof(char) * 128);
					buffer[(sizeof(char) * 128) - 1] = '\0';
					sprintf_s(buffer, sizeof(char) * 128, "Extra settings for %s module", m_pTheModule->m_pszName);
					if (ImGui::Begin(buffer, &extra->m_bActive, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
						if (extra->m_pfnWindowRenderCB) extra->m_pfnWindowRenderCB(extra->m_pCallbackUserData);
						ImGui::End();
					}
					Q_free(buffer);
				}
			}
		}

		ImGui::End();
	}
}

void CCategoryPanel::Draw() {
	if (!m_bOpen) return;

	if (!ImGui::Begin(m_pszName, &m_bOpen, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) return;

	for (int nIndex = 0; nIndex < m_pModules->size(); nIndex++) {
		CModuleTab* pTab = m_pModules->at(nIndex);
		if (ImGui::Button(pTab->m_pTheModule->m_pszName)) {
			pTab->m_bOpen ^= true;
		}
		pTab->Draw();
	}

	ImGui::End();
}

void CSimpleGui::Draw() {
	if (!ImGui::Begin("Lightning##SimpleGui", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) return;

	for (int nIndex = 0; nIndex < m_pPanels->size(); nIndex++) {
		CCategoryPanel* pPanel = m_pPanels->at(nIndex);
		if (ImGui::Button(pPanel->m_pszName)) {
			pPanel->m_bOpen ^= true;
		}
		pPanel->Draw();
	}

	ImGui::End();
}