#include "StdAfx.h"
#include "click_gui.hpp"
#include "CClickGUIModule.hpp"

void DrawStringWithShadow(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text) {
	for (int idx = 1; idx <= 1; idx++) {
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(_X + idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(_X - idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(_X, _Y + idx), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(_X, _Y - idx), ImColor(0, 0, 0), _Text);
	}
	ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(_X, _Y), _Color, _Text);
}

void DrawGlowingString(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text, _In_opt_ float _FontSize = 25.f) {
	float flCurrentIntensity = 0.02f;
	for (float idx = 0.1f; idx <= 3.f; idx += 0.1f) {
		if (flCurrentIntensity == 0.f) break;
		ImColor colCurrentColor = ImColor(_Color.Value.x, _Color.Value.y, _Color.Value.z, flCurrentIntensity);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y), colCurrentColor, _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y), colCurrentColor, _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y + idx), colCurrentColor, _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y - idx), colCurrentColor, _Text);

		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y - idx), colCurrentColor, _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y + idx), colCurrentColor, _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y - idx), colCurrentColor, _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y + idx), colCurrentColor, _Text);
		flCurrentIntensity = max(flCurrentIntensity - 0.0005f, 0.0f);
	}
	ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y), _Color, _Text);
}

CModuleButton::CModuleButton(_In_ CModule* _Module, _In_ CPanel* _Owner) {
	m_pModule = _Module;
	m_bValuesOpen = false;
	m_vecSize.m_flY = g_pClickGUIModule->m_pPanelSize->Get();
	m_pOwner = _Owner;
	m_bListeningForKeybind = false;
	m_vecPosition.m_flX = 0.f;
	m_vecPosition.m_flY = 0.f;
	m_bHovered = false;
}

void CModuleButton::Draw() {
	//float flScale = g_pClickGUIModule->m_pScale->Get();

	if (m_bHovered == true) {
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(m_vecPosition.m_flX - 2.f, m_vecPosition.m_flY), ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 72.f, m_vecPosition.m_flY + m_vecSize.m_flY + 6), ImColor(30, 30, 30));
	}

	if (m_bListeningForKeybind == false) {
		if (m_pModule->m_bState == true) {
			DrawGlowingString(m_vecPosition.m_flX + 4, m_vecPosition.m_flY, g_DefaultClientColor, m_pModule->m_pszName);
		} else {
			if (g_pClickGUIModule->m_pTheme->Get() == 1 /* Light */) {
				ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(m_vecPosition.m_flX + 4, m_vecPosition.m_flY), ImColor(95, 115, 105), m_pModule->m_pszName);
			} else {
				DrawStringWithShadow(m_vecPosition.m_flX + 4, m_vecPosition.m_flY, ImColor(160, 180, 170), m_pModule->m_pszName);
			}
		}
	} else {
		if (g_pClickGUIModule->m_pTheme->Get() == 1 /* Light */) {
			ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(m_vecPosition.m_flX + 4, m_vecPosition.m_flY), ImColor(95, 115, 105), "Listening...");
		} else {
			DrawStringWithShadow(m_vecPosition.m_flX + 4, m_vecPosition.m_flY, ImColor(160, 180, 170), "Listening...");
		}
	}

	if (!m_pModule->m_pVecValues->empty()) {
		if (m_bValuesOpen == true) {
			if (g_pClickGUIModule->m_pTheme->Get() == 1 /* Light */) {
				ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 55, m_vecPosition.m_flY), ImColor(95, 115, 105), "-");
			} else {
				DrawStringWithShadow(m_vecPosition.m_flX + m_vecSize.m_flX + 55, m_vecPosition.m_flY, ImColor(160, 180, 170), "-");
			}
		} else {
			if (g_pClickGUIModule->m_pTheme->Get() == 1 /* Light */) {
				ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 55, m_vecPosition.m_flY), ImColor(95, 115, 105), "+");
			} else {
				DrawStringWithShadow(m_vecPosition.m_flX + m_vecSize.m_flX + 55, m_vecPosition.m_flY, ImColor(160, 180, 170), "+");
			}
		}
	}
}

void CModuleButton::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kMouseMoveEvent) {
		auto e = static_cast<const CMouseMoveEvent*>(_Event);

		m_bHovered = IsHovered(e->m_iX, e->m_iY) ? true : false;
		m_bHovered = (g_pClickGUIModule->m_bState && m_bHovered) ? true : false;
	}
	if (!m_pModule) return;
	if (!g_pClickGUIModule->m_bState) {
		m_bHovered = m_bListeningForKeybind = false;
		return;
	}

	if (_Event->GetType() == EEventType::kKeyEvent) {
		if (m_bListeningForKeybind == false) return;

		auto e = static_cast<const CKeyEvent*>(_Event);
		if (e->m_eAction != EKeyAction::kDown) return;
		if (e->m_iButton >= UCHAR_MAX) return;
		if (e->m_iButton == VK_ESCAPE) {
			if (m_pModule == g_pClickGUIModule) return;
			m_pModule->m_nKeyBind = 0;
			m_bListeningForKeybind = false;
			m_pModule->m_pszKeybind = "None";
			CCheat::GetCheat()->m_pModuleManager->SaveKeybindsToConfigFile();

			return;
		}

		m_pModule->m_nKeyBind = e->m_iButton;

		char* szBuffer = (char*)(Q_malloc(sizeof(char) * 128));
		int iTotalKeynameLength = 0;
		unsigned int uiScancode = MapVirtualKey(e->m_iButton, MAPVK_VK_TO_VSC);
		switch (e->m_iButton) {
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
			const char* pszPreviousBuffer = m_pModule->m_pszKeybind;
			m_pModule->m_pszKeybind = szBuffer;
			if (strcmp(pszPreviousBuffer, "Unknown") && strcmp(pszPreviousBuffer, "None"))
				Q_free((void*)pszPreviousBuffer);
		} else {
			Q_free(szBuffer);
			m_pModule->m_pszKeybind = "Unknown";
		}
		m_bListeningForKeybind = false;
		CCheat::GetCheat()->m_pModuleManager->SaveKeybindsToConfigFile();
	}

	if (_Event->GetType() == EEventType::kMouseBtnDownEvent) {
		auto e = static_cast<const CMouseBtnDownEvent*>(_Event);

		if (m_bHovered) {
			if (e->m_iButton == 0 /* Left mouse button */) {
				if (m_pModule == g_pClickGUIModule) return;

				m_pModule->Toggle();
			} else if (e->m_iButton == 1 /* Right mouse button */) {
				if (!m_pModule->m_pVecValues->empty()) {
					m_bValuesOpen ^= true;
				}
			} else if (e->m_iButton == 2 /* Middle mouse button */) {
				for (int i = 0; i < m_pOwner->m_pOwner->m_pVecPanels->size(); i++) {
					CPanel* lpPanel = m_pOwner->m_pOwner->m_pVecPanels->at(i);
					if (lpPanel->m_bDragging == true) return;
					for (int idx = 0; idx < lpPanel->m_pVecElements->size(); idx++) {
						CModuleButton* pModuleBtn = lpPanel->m_pVecElements->at(idx);
						if (pModuleBtn == this) continue;
						if (pModuleBtn->m_bListeningForKeybind) return;
					}
				}
				m_bListeningForKeybind ^= true;
			}

			e->CancelEvent();
		}
	}
}

void CCustomFontsGui::Draw(_In_ bool& _Active) {
	if (ImGui::Begin("Custom fonts", reinterpret_cast<bool*>(&_Active), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::Button("Reset client font")) {
			g_pClientFont = g_pFirstLoadedFont->m_pFont;
			CCustomFonts::ms_pConfig->SetValue("Data", "ClientFont", "Default");
			CCustomFonts::ms_pConfig->Save();
		}
		CCustomFont* font = g_pFirstLoadedFont;

		while (font->m_pNext) {
			if (ImGui::Button(font->m_pszName)) {
				m_pSelectedFont = font;
				m_bHasSelected = true;
			}

			font = font->m_pNext;
		}

		if (m_pSelectedFont && m_bHasSelected == true) {
			if (ImGui::Begin(m_pSelectedFont->m_pszName, reinterpret_cast<bool*>(&m_bHasSelected), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("Font size: %d", m_pSelectedFont->m_iFontSize);
				ImGui::Text("Font size in pixels: %f", m_pSelectedFont->m_flFontSizeInPixels);
				if (ImGui::Button("Use as client font")) {
					g_pClientFont = m_pSelectedFont->m_pFont;
					CCustomFonts::ms_pConfig->SetValue("Data", "ClientFont", m_pSelectedFont->m_pszName);
					CCustomFonts::ms_pConfig->Save();
				}

				ImGui::End();
			}
		}

		ImGui::End();
	}
}

CPanel::CPanel(_In_ CClickGUI* _Owner, _In_z_ _Pre_z_ const char* _Title, _In_ int _X, _In_ int _Y, _In_ int _Width, _In_ int _Height, _In_opt_ bool _Open /* = true */) : m_pszTitle(_Title) {
	m_pOwner = _Owner;
	m_bOpen = _Open;
	m_bDragging = false;

	m_vecPosition.m_flX = _X;
	m_vecPosition.m_flY = _Y;
	m_vecSize.m_flX = _Width;
	m_vecSize.m_flY = _Height;

	m_vecPosition2.m_flX = 0.f;
	m_vecPosition2.m_flY = 0.f;

	m_iMouseX = m_iMouseY = 0;

	m_pVecElements = Q_new(CVector<CModuleButton*>)({});
}

CPanel::~CPanel() {
	for (int nIndex = 0; nIndex < m_pVecElements->size(); nIndex++) {
		CModuleButton* pModuleButton = m_pVecElements->at(nIndex);
		if (!pModuleButton) continue;
		Q_delete(pModuleButton);
	}
	Q_delete(m_pVecElements);
}

void CPanel::Draw() {
	if (!g_pClickGUIModule) return;

	if (m_bDragging) {
		m_vecPosition.m_flX = m_vecPosition2.m_flX + m_iMouseX;
		m_vecPosition.m_flY = m_vecPosition2.m_flY + m_iMouseY;
	}

	ImColor colBeginning = (g_pClickGUIModule->m_pTheme->Get() == 1 ? ImColor(230, 230, 230) : ImColor(64, 65, 67));
	ImColor colModBtn = (g_pClickGUIModule->m_pTheme->Get() == 1 ? ImColor(210, 210, 210) : ImColor(60, 60, 60));
	ImColor colText = (g_pClickGUIModule->m_pTheme->Get() == 0 ? ImColor(230, 230, 230) : ImColor(150, 150, 150));

	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(m_vecPosition.m_flX, m_vecPosition.m_flY), ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 70, m_vecPosition.m_flY + m_vecSize.m_flY), colBeginning, 15.f, g_pClickGUIModule->m_pRounding->Get() ? (m_bOpen == true ? ImDrawFlags_RoundCornersTop : ImDrawFlags_RoundCornersAll) : ImDrawFlags_RoundCornersNone);
	ImVec2 vecTextSize = g_pClientFont->CalcTextSizeA(25.f, FLT_MAX, 0.f, m_pszTitle);
	DrawStringWithShadow(m_vecPosition.m_flX + (((m_vecSize.m_flX + 70.f) / 2.f) - vecTextSize.x / 2.f), m_vecPosition.m_flY, colText, m_pszTitle);

	if (m_bOpen == true && !m_pVecElements->empty()) {
		int delta = m_vecPosition.m_flY + m_vecSize.m_flY;
		int iLastBtnHeight = 0;

		for (int i = 0; i < m_pVecElements->size(); i++) {
			CModuleButton* btn = m_pVecElements->at(i);
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(m_vecPosition.m_flX, delta), ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 70, delta + btn->m_vecSize.m_flY + 6), colModBtn);

			//TODO: Add gradient at first element

			btn->m_vecPosition.m_flX = m_vecPosition.m_flX + 2;
			btn->m_vecPosition.m_flY = delta;
			btn->m_vecSize.m_flX = m_vecSize.m_flX - 4;
			btn->Draw();
			delta += btn->m_vecSize.m_flY + 6;
			iLastBtnHeight = btn->m_vecSize.m_flY;
		}

		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(m_vecPosition.m_flX, delta), ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 70, delta + iLastBtnHeight - 10), g_DefaultClientColor, g_pClickGUIModule->m_pRounding->Get() ? 15.f : 0.f, g_pClickGUIModule->m_pRounding->Get() ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersNone);
		ImGui::GetForegroundDrawList()->AddShadowRect(ImVec2(m_vecPosition.m_flX, delta), ImVec2(m_vecPosition.m_flX + m_vecSize.m_flX + 70, delta + iLastBtnHeight - 10), g_DefaultClientColor, 40.f, ImVec2(0.f, 0.f), g_pClickGUIModule->m_pRounding->Get() ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersNone, g_pClickGUIModule->m_pRounding->Get() ? 15.f : 0.f);
	}
}

void CPanel::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kMouseMoveEvent) {
		auto e = static_cast<const CMouseMoveEvent*>(_Event);

		m_iMouseX = e->m_iX;
		m_iMouseY = e->m_iY;
	}

	if (_Event->GetType() == EEventType::kMouseBtnUpEvent) {
		auto e = static_cast<const CMouseBtnUpEvent*>(_Event);

		if (e->m_iButton == 0 && m_bDragging)
			m_bDragging = false;
	}

	if (_Event->GetType() == EEventType::kMouseBtnDownEvent) {
		auto e = static_cast<const CMouseBtnDownEvent*>(_Event);

		if (e->m_iButton == 0 && IsHovered(m_iMouseX, m_iMouseY)) {
			for (int i = 0; i < m_pOwner->m_pVecPanels->size(); i++) {
				CPanel* lpPanel = m_pOwner->m_pVecPanels->at(i);
				if (lpPanel->m_bDragging == true) return;
			}

			m_vecPosition2.m_flX = m_vecPosition.m_flX - m_iMouseX;
			m_vecPosition2.m_flY = m_vecPosition.m_flY - m_iMouseY;
			m_bDragging = true;

			e->CancelEvent();
		}
		else if (e->m_iButton == 1 && IsHovered(m_iMouseX, m_iMouseY)) {
			m_bOpen = m_bOpen ? false : true;
			for (int i = 0; i < m_pVecElements->size(); i++) {
				CModuleButton* modBtn = m_pVecElements->at(i);
				modBtn->m_bListeningForKeybind = false;
				modBtn->m_bValuesOpen = false;
			}

			e->CancelEvent();
		}
	}

	if (m_bOpen == true) {
		for (int i = 0; i < m_pVecElements->size(); i++) {
			CModuleButton* modBtn = m_pVecElements->at(i);
			modBtn->OnEvent(_Event);
		}
	}
}

CClickGUI::CClickGUI() {
	InitialisePanels();
	m_bHasInitialised = false;
}

CClickGUI::~CClickGUI() {
	m_bHasInitialised = false;
	for (int nIndex = 0; nIndex < m_pVecPanels->size(); nIndex++) {
		CPanel* pPanel = m_pVecPanels->at(nIndex);
		if (!pPanel) continue;
		Q_delete(pPanel);
	}
	Q_delete(m_pVecPanels);
	if (m_pCustomFontsGui)
		Q_delete(m_pCustomFontsGui);
}

void CClickGUI::InitialisePanels() {
	if (!CCheat::GetCheat()->m_pModuleManager) {
		m_bHasInitialised = false;
		return;
	}

	CVector<CPanel*>* pPanels = Q_new(CVector<CPanel*>)({});
	int iWidth = 205, iHeight = g_pClickGUIModule->m_pPanelSize->Get() + 6.f, iX = 10, iY = 100;

	CVector<const char*>* lpVecCategories = CCheat::GetCheat()->m_pModuleManager->GetCategories();

	for (int idx = 0; idx < lpVecCategories->size(); idx++) {
		const char* szCategory = lpVecCategories->at(idx);
		CVector<CModule*>* pVecMods = CCheat::GetCheat()->m_pModuleManager->GetModulesByCategory(szCategory);

		CPanel* lpPanel = Q_new(CPanel)(this, szCategory, iX, iY, iWidth, iHeight);

		CVector<CModule*>::iterator beg = pVecMods->begin();
		for (beg; beg != pVecMods->end(); beg++) {
			CModule* mod = *beg;
			lpPanel->m_pVecElements->push_back(Q_new(CModuleButton)(mod, lpPanel));
		}
		Q_delete(pVecMods);

		pPanels->push_back(lpPanel);

		iX += iWidth + 90;
	}

	CVector<CPanel*>* pTemp = m_pVecPanels;
	m_pVecPanels = pPanels;
	if (pTemp) {
		{
			CPanel** pBegin = pTemp->begin();
			for (pBegin; pBegin != pTemp->end(); pBegin++) {
				if (!pBegin || !*pBegin) continue;
				CModuleButton** pButton = (*pBegin)->m_pVecElements->begin();
				for (pButton; pButton != (*pBegin)->m_pVecElements->end(); pButton++) {
					if (!pButton || !*pButton) continue;
					Q_delete(*pButton);
				}
				Q_delete(*pBegin);
			}
		}
		Q_delete(pTemp);
	}

	m_bHasInitialised = true;
}

void CClickGUI::Draw() {
	if (!m_bHasInitialised) {
		InitialisePanels();
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
	if (ImGui::Begin("##ClickGui_Background", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav)) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, ImColor(0, 0, 0, 120));

		ImGui::End();
	}

	for (int idx = 0; idx < m_pVecPanels->size(); idx++) {
		CPanel* lpPanel = m_pVecPanels->at(idx);
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
		if (ImGui::Begin(lpPanel->m_pszTitle, nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing)) {
			lpPanel->Draw();

			if (lpPanel->m_bOpen == true && !lpPanel->m_pVecElements->empty()) {
				for (int i = 0; i < lpPanel->m_pVecElements->size(); i++) {
					CModuleButton* modBtn = lpPanel->m_pVecElements->at(i);
					if (modBtn->m_bValuesOpen) {
						ImGui::SetNextWindowPos(ImVec2(modBtn->m_vecPosition.m_flX + modBtn->m_vecSize.m_flX + 72, modBtn->m_vecPosition.m_flY), ImGuiCond_Always);
						ImGui::SetNextWindowBgAlpha(255.f);
						if (ImGui::Begin(modBtn->m_pModule->m_pszName, nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
							ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize)) {
							if (modBtn->m_pModule->m_nKeyBind != 0 && modBtn->m_pModule->m_pszKeybind) {
								ImGui::Text("Keybind: %s\n", modBtn->m_pModule->m_pszKeybind);
							}
							CVector<CValue*>::iterator beg = modBtn->m_pModule->m_pVecValues->begin();
							for (beg; beg != modBtn->m_pModule->m_pVecValues->end(); beg++) {
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
								} else if (value->m_eType == kValueTypeFloat) {
									CFloatValue* flt = static_cast<CFloatValue*>(value);
									ImGui::PushID(value->m_pszConvarName);
									float flValue = flt->Get();
									if (ImGui::SliderFloat(flt->m_pszName, &flValue, flt->m_fMinValue, flt->m_fMaxValue)) {
										if (flt->m_pfnOnceValueChangedCB) flt->m_pfnOnceValueChangedCB(flt->m_pOnceValueChangedCBUserData, &flValue, &flt->m_fCurrentValue);
										flt->Set(flValue);
									}
									ImGui::PopID();
								} else if (value->m_eType == kValueTypeInteger) {
									CIntegerValue* itg = static_cast<CIntegerValue*>(value);
									ImGui::PushID(value->m_pszConvarName);
									int iValue = itg->Get();
									if (ImGui::SliderInt(itg->m_pszName, &iValue, itg->m_iMinValue, itg->m_iMaxValue)) {
										if (itg->m_pfnOnceValueChangedCB) itg->m_pfnOnceValueChangedCB(itg->m_pOnceValueChangedCBUserData, &iValue, &itg->m_iCurrentValue);
										itg->Set(iValue);
									}
									ImGui::PopID();
								} else if (value->m_eType == kValueTypeList) {
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
								} else if (value->m_eType == kValueTypeColour) {
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
								} else if (value->m_eType == kValueTypeExtra) {
									CExtraValues* extra = static_cast<CExtraValues*>(value);
									ImGui::PushID(value->m_pszConvarName);
									if (ImGui::Button(value->m_pszName)) {
										extra->Set(!extra->Get());
									}
									ImGui::PopID();
									if (extra->Get()) {
										char* buffer = (char*)Q_malloc(sizeof(char) * 128);
										buffer[(sizeof(char) * 128) - 1] = '\0';
										sprintf_s(buffer, sizeof(char) * 128, "Extra settings for %s module", modBtn->m_pModule->m_pszName);
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
				}
			}

			ImGui::End();
		}
	}
}

void CClickGUI::OnEvent(_In_ const ISimpleEvent* _Event) {
	for (int idx = 0; idx < m_pVecPanels->size(); idx++) {
		CPanel* lpPanel = m_pVecPanels->at(idx);
		lpPanel->OnEvent(_Event);
	}
}