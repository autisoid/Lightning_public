/**
 * Copyright - xWhitey, 2024.
 * CClientDesignerModule.cpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CClientDesignerModule.hpp"

CClientDesignerModule* g_pClientDesignerModule;

Vector CClientDesignerModule::GetRainbowColour32(double _Offset, float _Saturation, float _Lightness) {
	double dblRainbowState = ceil((g_pTimer->m_nTimeInMilliseconds + double(_Offset)) / 20.0);
	double dblRem = fmod(dblRainbowState, 360.0);

	return CUtils::HSL2RGB(Vector(float(dblRem / 360.0), _Saturation, _Lightness));
}

CClientDesignerModule::CClientDesignerModule() : CModule("ClientDesigner", "Appearance", 0, false) {
	m_pVecValues->push_back(m_pRainbow = Q_new(CBoolValue)("Rainbow", "sc_clientdesigner_rainbow", true));
	m_pVecValues->push_back(m_pRainbowSaturation = Q_new(CFloatValue)("Rainbow saturation", "sc_clientdesigner_rainbow_saturation", 0.0f, 1.0f, 0.8f));
	m_pVecValues->push_back(m_pRainbowLightness = Q_new(CFloatValue)("Rainbow lightness", "sc_clientdesigner_rainbow_lightness", 0.0f, 1.0f, 0.5f));
	m_pVecValues->push_back(m_pClientColor = Q_new(CColourValue)("Client color", "sc_clientdesigner_client_color", 
	[](void* _UserData, float* _RGBA) {
		g_DefaultClientColor = ImColor(_RGBA[0], _RGBA[1], _RGBA[2], _RGBA[3]);
	}, this, g_DefaultClientColor.Value.x, g_DefaultClientColor.Value.y, g_DefaultClientColor.Value.z));
	m_pRainbow->RegisterOnceValueChangedCallback([](void* _UserData, void* _Value, void* _PreviousValue) {
		CClientDesignerModule* thiz = (CClientDesignerModule*)(_UserData);
		bool* value = (bool*)_Value;
		if (!(*value)) {
			float* rgba = thiz->m_pClientColor->Get();
			g_DefaultClientColor = ImColor(rgba[0], rgba[1], rgba[2], rgba[3]);
		}
	}, this);
	m_pVecValues->push_back(m_pRainbowMode = Q_new(CListValue)("Rainbow mode", "sc_clientdesigner_rainbow_mode", "Simplified\0LiquidBounce\0\0", 1));

	m_pVecValues->push_back(m_pNotifications = Q_new(CBoolValue)("Notifications", "sc_clientdesigner_notifications", true));

	m_flGlowRGB_HUE = 0.0f;
	m_dblNextUpdateTime = g_pEngfuncs->GetAbsoluteTime();

	g_pClientDesignerModule = this;

	//Toggle(); //do NOT call Toggle at least in module constructor as this will INSTANTLY override modules.ini
	if (!m_bState)
		CModule::SetState(true, true);
}

void CClientDesignerModule::OnEnable() {
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CClientDesignerModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	Toggle(); //it is PROBABLY ok to call Toggle here
}

void CClientDesignerModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kRenderEvent) {
		if (m_pNotifications->Get()) {
			CCheat::GetCheat()->m_pNotifications->Process();
		}

		if (m_pRainbow->Get()) {
			if (!m_bHasTriedAccessingFastTime) {
				m_bHasTriedAccessingFastTime = true;
				if (g_pdblClientTime) {
					m_bUseFastAccessToTime = true;
				}
			}

			double dblTime;
			if (m_bUseFastAccessToTime) {
				dblTime = *g_pdblClientTime;
			} else {
				dblTime = g_pEngfuncs->GetAbsoluteTime();
			}

			if (m_dblNextUpdateTime - 1.0 > dblTime /*&& !g_pEngfuncs->GetLocalPlayer()*/) { //reset time in case of a disconnect/level cahnge
				m_dblNextUpdateTime = 0.0;
			}

			if (dblTime <= m_dblNextUpdateTime + 0.05)
				return;
			m_dblNextUpdateTime = dblTime;
			if (m_pRainbowMode->Get() == 0 /* Simplified */) {
				Vector vecRGB = CUtils::HSL2RGB(Vector(m_flGlowRGB_HUE, m_pRainbowSaturation->Get(), m_pRainbowLightness->Get()));

				m_flGlowRGB_HUE = m_flGlowRGB_HUE + 0.015f;

				while (m_flGlowRGB_HUE > 1.0f) {
					m_flGlowRGB_HUE = m_flGlowRGB_HUE - 1.0f;
				}

				g_DefaultClientColor = ImColor(vecRGB.x, vecRGB.y, vecRGB.z);
			} else if (m_pRainbowMode->Get() == 1 /* LiquidBounce */) {
				Vector vecRGB = GetRainbowColour32(1.0, m_pRainbowSaturation->Get(), m_pRainbowLightness->Get());
				g_DefaultClientColor = ImColor(vecRGB.x, vecRGB.y, vecRGB.z);
			}
		}
	}
}