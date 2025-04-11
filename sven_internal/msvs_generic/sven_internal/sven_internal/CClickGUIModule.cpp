#include "StdAfx.h"
#include "CClickGUIModule.hpp"
#include "custom_fonts.hpp"
#include "CClientDesignerModule.hpp"

CClickGUIModule* g_pClickGUIModule;

//Default toggle key is VK_RSHIFT. Don't modify unless required.
CClickGUIModule::CClickGUIModule() : CModule("ClickGUI", "Render", VK_RSHIFT) {
	m_pVecValues->push_back(m_pTheme = Q_new(CListValue)("Theme", "sc_clickgui_theme", "Dark\0Light\0\0", 0));
	m_pVecValues->push_back(m_pScale = Q_new(CFloatValue)("Scale", "sc_clickgui_scale", 0.1f, 3.f, 1.f));
	m_pScale->RegisterOnceValueChangedCallback([](void* _UserData, void* _Value, void* _PreviousValue) {
		CClickGUIModule* thiz = (CClickGUIModule*)_UserData;
		float* pflValue = (float*)_Value;
		float flValue = *pflValue;
		if (flValue < 0.5f) {
			*pflValue = 0.5f;
		}
		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(*pflValue);
	}, this);
	m_pScale->GetConVar()->RegisterCallback([](void* _UserData, char** _Value) {
		CClickGUIModule* thiz = (CClickGUIModule*)_UserData;
		char* pszValue = *_Value;
		float flValue = 0.0f;
		sscanf_s(pszValue, "%f", &flValue);
		if (flValue < 0.5f) {
			flValue = 0.5f;
			char* pszNewValue = (char*) Q_malloc(sizeof("0.1"));
			memset(pszNewValue, 0, sizeof("0.1"));
			strncpy_s(pszNewValue, sizeof("0.1"), "0.1", sizeof("0.1") - 1);
			*_Value = pszNewValue;
		}
		//ImGuiStyle& style = ImGui::GetStyle();
		//style.ScaleAllSizes(flValue);
		thiz->m_pScale->Set(flValue);
	}, this);
	m_pScale->AddFlag(kValueFlagDontRender);
	m_pVecValues->push_back(m_pStyle = Q_new(CListValue)("Style", "sc_clickgui_style", "Default\0Simple\0\0", 0));
	m_pStyle->AddFlag(kValueFlagDontRender);
	m_pVecValues->push_back(m_pRounding = Q_new(CBoolValue)("Rounding", "sc_clickgui_rounding", true));
	m_pVecValues->push_back(m_pPanelSize = Q_new(CFloatValue)("Panel & module button size", "sc_clickgui_panel_size", 18.f, 36.f, 18.f));
	m_pPanelSize->AddFlag(kValueFlagDontRender);

	m_pMouseRawInputConVar = g_pEngfuncs->pfnGetCvarPointer("m_rawinput");

	m_bDebugConsoleActive = false;
	m_bIsInCustomFontsGui = false;
	//28 symbols available to enter a code
	m_acInputField = indirect_cast<char*>(Q_malloc(28 + 1));
	m_acInputField[28 + 1] = '\0';
	m_nCurrentInputFieldIdx = 0;

	g_pClickGUIModule = this;
}

void CClickGUIModule::OnEnable() {
	CModule::OnEnable();
	GetCursorPos(&m_pntLastCursorPos);

	m_nCurrentInputFieldIdx = 0;
	Q_memset(m_acInputField, 0, 29);

	if (!m_pClickGUI)
		m_pClickGUI = Q_new(CClickGUI)();

	if (!m_pSimpleGUI)
		m_pSimpleGUI = Q_new(CSimpleGui)();

	if (!m_pClickGUI->m_pCustomFontsGui)
		m_pClickGUI->m_pCustomFontsGui = Q_new(CCustomFontsGui)();

	ClipCursor(nullptr);

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CClickGUIModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	for (int idx = 0; idx < m_pClickGUI->m_pVecPanels->size(); idx++) {
		CPanel* lpPanel = m_pClickGUI->m_pVecPanels->at(idx);
		lpPanel->m_bDragging = false;

		for (int j = 0; j < lpPanel->m_pVecElements->size(); j++) {
			CModuleButton* lpButton = lpPanel->m_pVecElements->at(j);
			lpButton->m_bHovered = false;
		}
	}

	m_bDebugConsoleActive = false;
	//SetCursorPos(m_pntLastCursorPos.x, m_pntLastCursorPos.y);
	CModule::OnDisable();
}

static CModule* gs_pSelection = nullptr;
static bool gs_bModuleSelected = false;

void CClickGUIModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (m_bDebugConsoleActive == false && m_bIsInCustomFontsGui == false && m_pStyle->Get() == 0 /* Default */) {
		m_pClickGUI->OnEvent(_Event);
	}

	if (_Event->GetType() == EEventType::kKeyEvent) {
		auto e = static_cast<const CKeyEvent*>(_Event);

		if (e->m_eAction == EKeyAction::kDown && m_bDebugConsoleActive == false && m_bIsInCustomFontsGui == false) {
			if (e->m_iButton >= 0 && e->m_iButton <= 255) {
				if (m_nCurrentInputFieldIdx < 29) {
					m_nCurrentInputFieldIdx++;
					//gta_sa.exe - void CCheat::AddToCheatString(char)
					for (auto i = 29 - 1 - 1; i >= 1; --i) {
						m_acInputField[i] = m_acInputField[i - 1];
					}
					m_acInputField[0] = e->m_iButton;
					m_acInputField[29] = '\0';

					if (strlen(m_acInputField) < sizeof("debug") - 1) 
						return;

					if (!_strnicmp(m_acInputField, "gubed", sizeof("debug") - 1)) {
						m_bDebugConsoleActive = true;

						m_acInputField[0] = '\0';
					}
					if (!_strnicmp(m_acInputField, "stnof", sizeof("fonts") - 1)) {
						m_bIsInCustomFontsGui = true;

						m_acInputField[0] = '\0';
					}
					if (!_strnicmp(m_acInputField, "gnabgnabyttihcyttihc", sizeof("chittychittybangbang") - 1)) {
						m_pStyle->m_nCurrentIdx = m_pStyle->m_nCurrentIdx == 0 ? 1 : 0;
						m_pStyle->SetConVarValue(m_pStyle->Get() == 0 ? 1 : 0);
					}
				} else {
					m_acInputField[0] = '\0';
					m_nCurrentInputFieldIdx = 0;
				}
			}
		}
	}

	if (_Event->GetType() == EEventType::kRenderEvent) {
		//auto e = static_cast<const CRenderEvent*>(_Event);

		ImGui::PushFont(g_pClientFont);

		static double dblMemTotal = (FUNCTIONAL_HEAP_SIZE / 1024.0) / 1024.0;
		double dblMemFree = 0.0, dblMemConsumed = 0.0;
		CAllocatedSegment* lpSegment = Q_GetAllocator()->m_pSegments;
		while (lpSegment) {
			if (!lpSegment->m_bIsFree) {
				dblMemConsumed += static_cast<double>(lpSegment->m_iSize);
			}

			lpSegment = lpSegment->m_pNext;
		}
		dblMemConsumed = (dblMemConsumed / 1024.0) / 1024.0;
		dblMemFree = dblMemTotal - dblMemConsumed;
		char* szBuffer = (char*) Q_malloc(512);
		szBuffer[512 - 1] = '\0';
		_snprintf_s(szBuffer, 512, 512, "Mem total: %f MB\nMem free: %f MB\nMem consumed: %f MB", dblMemTotal, dblMemFree, dblMemConsumed);
		ImVec2 vecStringSize = g_pClientFont->CalcTextSizeA(20.f, FLT_MAX, 0.0f, szBuffer);
		ImVec2 vecScreenSize = ImGui::GetIO().DisplaySize;
		ImGui::GetForegroundDrawList()->AddText(ImVec2(0, vecScreenSize.y - vecStringSize.y), ImColor(1.f, 1.f, 1.f), szBuffer);
		Q_free(szBuffer);

		if (m_bDebugConsoleActive == true) {
			CCheat::GetCheat()->m_pConsole->Draw(m_bDebugConsoleActive);
		} else if (m_bIsInCustomFontsGui == true && m_bDebugConsoleActive == false) {
			m_pClickGUI->m_pCustomFontsGui->Draw(m_bIsInCustomFontsGui);
		} else {
			if (m_pStyle->Get() == 0 /* Default */) {
				m_pClickGUI->Draw();
			} else if (m_pStyle->Get() == 1 /* Simple */) {
				m_pSimpleGUI->Draw();
			}
		}
		ImGui::PopFont();
	}
}