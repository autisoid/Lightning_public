#include "StdAfx.h"
#include "CHUDModule.hpp"
#include "CClientDesignerModule.hpp"
#include "ImGui/imgui_internal.h"

CHUDModule* g_pHUDModule;

ImColor Darker(_In_ ImColor _Which) {
	return ImColor(max(_Which.Value.x * 0.9f, 0), max(_Which.Value.y * 0.9f, 0), max(_Which.Value.z * 0.9f, 0), _Which.Value.w);
}

void DrawStringWithShadow(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text, _In_opt_ float _FontSize = 25.f) {
	for (int idx = 1; idx <= 1; idx++) {
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y + idx), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y - idx), ImColor(0, 0, 0), _Text);
	}
	ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y), _Color, _Text);
}

void DrawStringWithGlowEffect(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text, _In_opt_ float _FontSize = 25.f) {
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

CHUDModule::CHUDModule() : CModule("HUD", "Render") {
	m_pVecValues->push_back(m_pDrawWatermark = Q_new(CBoolValue)("Draw watermark", "sc_hud_draw_watermark", true));
	m_pVecValues->push_back(m_pDrawArrayList = Q_new(CBoolValue)("Draw arraylist", "sc_hud_draw_arraylist", true));

	m_pVecValues->push_back(m_pArrayListTextHeight = Q_new(CFloatValue)("Text height (ArrayList)", "sc_hud_arraylist_text_height", 1.f, 20.0f, 11.f));
	m_pVecValues->push_back(m_pArrayListBackground = Q_new(CBoolValue)("Background (ArrayList)", "sc_hud_arraylist_background", true));
	m_pVecValues->push_back(m_pArrayListRect = Q_new(CBoolValue)("Rect (ArrayList)", "sc_hud_arraylist_rect", true));

	m_pVecValues->push_back(m_pWatermarkText = Q_new(CTextValue)("Watermark text (client name)", "sc_hud_watermark_text", "Lightning"));

	m_pVecValues->push_back(m_pGlow = Q_new(CBoolValue)("Glow", "sc_hud_glow", true));

	m_pVecValues->push_back(m_pRainbowStep = Q_new(CFloatValue)("Rainbow step (ArrayList)", "sc_hud_arraylist_rainbow_step", 0.1f, 2.0f, 0.5f));

	g_pHUDModule = this;

	DefineConCmd("sc_lightning_hud", CHUDModule);
}

void CHUDModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CHUDModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CHUDModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kRenderEvent) {
		if (CCheat::GetCheat()->m_pModuleManager->m_pVecSortedModules == nullptr) CCheat::GetCheat()->m_pModuleManager->SortModules();

		if (m_pDrawWatermark->Get()) {
			const char* pszText = m_pWatermarkText->Get();
			ImVec2 vecSize = g_pClientFont->CalcTextSizeA(25.f, FLT_MAX, 0.0f, pszText);
			float flX = 18.f, flY = flX;
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(flX - 2.f, flY - 2.f), ImVec2(vecSize.x + 30.f, vecSize.y * 2.f), ImColor(0, 0, 0, 100));
			if (!m_pGlow->Get()) {
				ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(flX - 2.f, flY - 2.f), ImVec2(8, vecSize.y * 2.f), g_DefaultClientColor);
			} else {
				ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(flX - 2.f, flY - 2.f), ImVec2(8, vecSize.y * 2.f), g_DefaultClientColor);
				ImGui::GetForegroundDrawList()->AddShadowRect(ImVec2(flX - 2.f, flY - 2.f), ImVec2(8, vecSize.y * 2.f), g_DefaultClientColor, 40.f, ImVec2(0.f, 0.f), ImDrawFlags_RoundCornersAll, 0.1f);
			}
			//ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 25.f, ImVec2(x + 6.f, y), g_DefaultClientColor, text);
			if (!m_pGlow->Get()) {
				DrawStringWithShadow(flX + 6.f, flY, g_DefaultClientColor, pszText);
			} else {
				DrawStringWithGlowEffect(flX + 6.f, flY, g_DefaultClientColor, pszText);
			}
		}

		if (m_pDrawArrayList->Get()) {
			ImVec2 vecScaledResolution = ImGui::GetIO().DisplaySize;
			int iModulesSize = CCheat::GetCheat()->m_pModuleManager->m_pVecSortedModules->size();
			for (int idx = 0; idx < iModulesSize; idx++) {
				ImColor vecColour = g_DefaultClientColor;
				if (g_pClientDesignerModule->m_pRainbowMode->Get() == 1) {
					Vector vecRGB = g_pClientDesignerModule->GetRainbowColour32((double(m_pRainbowStep->Get()) * double(idx)) * 300.0, 
						g_pClientDesignerModule->m_pRainbowSaturation->Get(), g_pClientDesignerModule->m_pRainbowLightness->Get());

					vecColour = ImColor(vecRGB.x, vecRGB.y, vecRGB.z);
				}
				CModule* mod = CCheat::GetCheat()->m_pModuleManager->m_pVecSortedModules->at(idx);

				float yPos = m_pArrayListTextHeight->Get() * idx;
				ImVec2 vecSize = g_pClientFont->CalcTextSizeA(22.f, FLT_MAX, 0.0f, mod->m_pszName);
				if (m_pArrayListRect->Get())
					vecSize.x += 14.f;

				ImVec2 vecTagSize(0, 0);
				bool bHasTag = false;
				if (mod->GetTag() && mod->GetTag()[0] != '\0') {
					bHasTag = true;
					char szBuffer[512];
					sprintf_s(szBuffer, " %s", mod->GetTag());
					vecTagSize = g_pClientFont->CalcTextSizeA(22.f, FLT_MAX, 0.0f, szBuffer);
					vecTagSize.x += 14.f;
					vecSize.x += vecTagSize.x;
				}
				if (m_pArrayListBackground->Get()) {
					ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(vecScaledResolution.x - vecSize.x - 12.f, yPos + 14.f), ImVec2(vecScaledResolution.x - 8.f, yPos + 14.f + m_pArrayListTextHeight->Get()), ImColor(0.f, 0.f, 0.f, 0.5f));
				}
				if (m_pArrayListRect->Get()) {
					Vector vecRGB = g_pClientDesignerModule->GetRainbowColour32((double(m_pRainbowStep->Get()) * double(idx + 1)) * 300.0,
						g_pClientDesignerModule->m_pRainbowSaturation->Get(), g_pClientDesignerModule->m_pRainbowLightness->Get());
					ImColor vecColour2 = ImColor(vecRGB.x, vecRGB.y, vecRGB.z);
					ImVec2 vecMin(vecScaledResolution.x - 16.f, yPos + 14.f);
					ImVec2 vecMax(vecScaledResolution.x - 8.f, yPos + 14.f + m_pArrayListTextHeight->Get());
					if (g_pClientDesignerModule->m_pRainbowMode->Get() == 1 /* LiquidBounce */ && m_pGlow->Get()) {
						int iStart = ImGui::GetForegroundDrawList()->VtxBuffer.Size;
						ImGui::GetForegroundDrawList()->AddShadowRect(vecMin, vecMax, vecColour, 40.f, ImVec2(0.f, 0.f));
						int iEnd = ImGui::GetForegroundDrawList()->VtxBuffer.Size;
						ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetForegroundDrawList(), iStart, iEnd, vecMin, vecMax, vecColour, vecColour2);
					}
					/*int iStart = ImGui::GetForegroundDrawList()->VtxBuffer.Size;
					ImGui::GetForegroundDrawList()->AddShadowRect(vecMin, vecMax, vecColour, 40.f, ImVec2(0.f, 0.f));
					int iEnd = ImGui::GetForegroundDrawList()->VtxBuffer.Size;
					ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetForegroundDrawList(), iStart, iEnd, vecMin, vecMax, vecColour, vecColour2);*/
					if (g_pClientDesignerModule->m_pRainbowMode->Get() == 1 /* LiquidBounce */) {
						ImGui::GetForegroundDrawList()->AddRectFilledMultiColor(vecMin, vecMax, vecColour, vecColour, vecColour2, vecColour2);
					} else {
						ImGui::GetForegroundDrawList()->AddRectFilled(vecMin, vecMax, vecColour);
					}
				}
				if (!m_pGlow->Get()) {
					DrawStringWithShadow(vecScaledResolution.x - vecSize.x - 8.f, yPos + 12.f, vecColour, mod->m_pszName, 22.f);
				} else {
					DrawStringWithGlowEffect(vecScaledResolution.x - vecSize.x - 8.f, yPos + 12.f, vecColour, mod->m_pszName, 22.f);
				}
				if (bHasTag) {
					if (!m_pGlow->Get()) {
						DrawStringWithShadow(vecScaledResolution.x - vecTagSize.x - 8.f, yPos + 12.f, ImColor(160, 160, 160), mod->GetTag(), 22.f);
					} else {
						DrawStringWithGlowEffect(vecScaledResolution.x - vecTagSize.x - 8.f, yPos + 12.f, ImColor(160, 160, 160), mod->GetTag(), 22.f);
					}
				}
			}
			if (m_pArrayListRect->Get() && m_pGlow->Get() && g_pClientDesignerModule->m_pRainbowMode->Get() == 0 /* Simplified */) {
				ImGui::GetForegroundDrawList()->AddShadowRect(ImVec2(vecScaledResolution.x - 16.f, 8.f),
					ImVec2(vecScaledResolution.x - 8.f, (m_pArrayListTextHeight->Get() * iModulesSize) + m_pArrayListTextHeight->Get()), g_DefaultClientColor, 40.f, ImVec2(0.f, 0.f));
			}
		}
	}
}