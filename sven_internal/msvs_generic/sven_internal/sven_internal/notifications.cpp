/**
 * Copyright - xWhitey, 2023.
 * notifications.cpp - A simple implementation of notifications
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "notifications.hpp"

void DrawStringWithShadow(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text, _In_opt_ float _FontSize = 25.f) {
	for (int idx = 1; idx <= 1; idx++) {
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y + idx), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y - idx), ImColor(0, 0, 0), _Text);
	}
	ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y), _Color, _Text);
}

CNotification::CNotification(_In_ CNotifications* _Owner, _In_z_ const char* _TextOne, _In_z_ const char* _TextTwo,
	_In_z_ const char* _TextThree, _In_ double _Delay, _In_ ImColor _Color) : m_pOwner(_Owner), m_pszTextOne(_TextOne), m_pszTextTwo(_TextTwo), m_pszTextThree(_TextThree) {
	m_bLerpForeground = true;
	m_bLerpBackground = false;
	m_bLerpBackwards = false;
	m_bLerpComplete = false;
	m_bLerpFullCompletion = false;
	m_flLerpForegroundWidth = 0.0f;
	m_flLerpBackgroundWidth = 0.0f;
	m_dblLerpEaseOut = static_cast<double>(g_pTimer->m_nTimeInMilliseconds) + _Delay;
	m_NotificationColor = _Color;
}

bool CNotification::IsStillShown() {
	return !m_bLerpFullCompletion;
}

void CNotification::Draw() {
	ImVec2 vecFirstTextSize = g_pClientFont->CalcTextSizeA(10.0f, FLT_MAX, 0.0f, m_pszTextOne);
	ImVec2 vecSecondTextSize = g_pClientFont->CalcTextSizeA(10.0f, FLT_MAX, 0.0f, m_pszTextTwo);
	ImVec2 vecThirdTextSize = g_pClientFont->CalcTextSizeA(10.0f, FLT_MAX, 0.0f, m_pszTextThree);

	float flTextTotalWidth = (vecFirstTextSize.x + vecSecondTextSize.x + vecThirdTextSize.x) + 16.0f;
	float flTextPositionX = -flTextTotalWidth;

	if (!m_bLerpFullCompletion) {
		if (m_bLerpForeground && !m_bLerpBackwards) {
			m_flLerpForegroundWidth = m_pOwner->Interpolate(m_flLerpForegroundWidth, flTextTotalWidth, ImGui::GetIO().Framerate * 0.0002f);
		}

		if (m_bLerpForeground && m_flLerpForegroundWidth >= flTextTotalWidth - 1.0f) {
			m_bLerpForeground = false;
			m_bLerpBackground = true;
			m_flLerpForegroundWidth = flTextTotalWidth;
		}

		if (m_bLerpBackground && !m_bLerpBackwards) {
			m_flLerpBackgroundWidth = m_pOwner->Interpolate(m_flLerpBackgroundWidth, flTextTotalWidth, ImGui::GetIO().Framerate * 0.0002f);
		}

		if (m_bLerpBackground && m_flLerpBackgroundWidth >= flTextTotalWidth - 1.0f) {
			m_bLerpBackground = false;
			m_bLerpBackground = flTextTotalWidth;
			m_bLerpComplete = true;
		}

		if (m_bLerpComplete) {
			if (((double)g_pTimer->m_nTimeInMilliseconds) > m_dblLerpEaseOut) {
				m_bLerpBackwards = true;
			}

			if (m_bLerpBackwards) {
				m_flLerpBackgroundWidth = m_pOwner->Interpolate(m_flLerpBackgroundWidth, 0.0f, ImGui::GetIO().Framerate * 0.0002f);

				if (m_flLerpBackgroundWidth <= 1.0f) {
					m_bLerpForeground = true;
					m_flLerpBackgroundWidth = 0.0f;
				}

				if (m_bLerpForeground)
					m_flLerpForegroundWidth = m_pOwner->Interpolate(m_flLerpForegroundWidth, 0.0f, ImGui::GetIO().Framerate * 0.0002f);

				if (m_flLerpForegroundWidth <= 1.0f) {
					m_bLerpForeground = false;
					m_flLerpForegroundWidth = 0.0f;
					m_bLerpFullCompletion = true;
				}
			}
		}

		ImVec2 vecPosition = ImVec2(0.0f, m_flNextNotificationPadding);
		ImVec2 vecSizeForFirstRect = ImVec2(m_flLerpForegroundWidth, 20.0f);
		ImVec2 vecSizeForSecondRect = ImVec2(m_flLerpBackgroundWidth, 20.0f);
		ImGui::GetForegroundDrawList()->AddRectFilled(vecPosition, ImVec2(vecPosition.x + vecSizeForFirstRect.x, vecPosition.y + vecSizeForFirstRect.y), m_NotificationColor);
		ImGui::GetForegroundDrawList()->AddRectFilled(vecPosition, ImVec2(vecPosition.x + vecSizeForSecondRect.x, vecPosition.y + vecSizeForSecondRect.y), ImColor(15, 15, 15, 255));

		float flLerpText = flTextPositionX + m_flLerpBackgroundWidth;
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 10.f, ImVec2(flLerpText + 8.0f, m_flNextNotificationPadding + 6.0f), ImColor(200, 200, 200, 255), m_pszTextOne);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 10.f, ImVec2(flLerpText + 8.0f + (vecFirstTextSize.x), m_flNextNotificationPadding + 6.0f), m_NotificationColor, m_pszTextTwo);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, 10.f, ImVec2(flLerpText + 8.0f + (vecFirstTextSize.x + vecSecondTextSize.x), m_flNextNotificationPadding + 6.0f), ImColor(200, 200, 200, 255), m_pszTextThree);
	}
}

CNotifications::CNotifications() {
	m_pvecNotifications = Q_new(CVector<CNotification*>)({});
}

void CNotifications::PushNotification(_In_z_ const char* _TextOne, _In_z_ const char* _TextTwo, _In_z_ const char* _TextThree, _In_ double _TimeToLive /* = 4000.0 */, ImColor _Color /* = ImColor(255, 147, 183, 255) */) {
	CNotification* pNotification = Q_new(CNotification)(this, _TextOne, _TextTwo, _TextThree, _TimeToLive, _Color);
	
	pNotification->m_flNextNotificationPadding = m_pvecNotifications->size() * 20.f;

	m_pvecNotifications->push_back(pNotification);
}

void CNotifications::Process() {
	if (!m_pvecNotifications) {
		m_pvecNotifications = Q_new(CVector<CNotification*>)({});
		return;
	}

	CNotification** ppBegin = m_pvecNotifications->begin();
	for (ppBegin; ppBegin != m_pvecNotifications->end() && ppBegin; ppBegin++) {
		if (!ppBegin) continue;

		CNotification* pNotification = *ppBegin;
		if (!pNotification) {
			m_pvecNotifications->erase(ppBegin);
			continue;
		}
		pNotification->Draw();

		if (pNotification->m_bLerpFullCompletion) {
			for (int nIndex = 0; nIndex < m_pvecNotifications->size(); nIndex++) {
				m_pvecNotifications->at(nIndex)->m_flNextNotificationPadding -= 20.f;
			}
			Q_delete(pNotification);
		}
	}
}

float CNotifications::Interpolate(_In_ float _A, _In_ float _B, _In_ float _T) {
	return _A + (_B - _A) * _T;
}