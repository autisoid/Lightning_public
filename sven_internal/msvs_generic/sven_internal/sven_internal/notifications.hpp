/**
 * Copyright - xWhitey, 2023.
 * notifications.hpp - A simple implementation of notifications
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef NOTIFICATIONS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in notifications.hpp
#else //NOTIFICATIONS_HPP_RECURSE_GUARD

#define NOTIFICATIONS_HPP_RECURSE_GUARD

#ifndef NOTIFICATIONS_HPP_GUARD
#define NOTIFICATIONS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "timer.hpp"

struct CNotifications;

//notifications pasted from guwi hack
typedef struct CNotification {
	CNotification(_In_ CNotifications* _Owner, _In_z_ const char* _TextOne, _In_z_ const char* _TextTwo, _In_z_ const char* _TextThree, _In_ double _Delay, _In_ ImColor _Color);

	void Draw();
	bool IsStillShown();

	bool m_bLerpForeground;
	bool m_bLerpBackground;
	bool m_bLerpBackwards;
	bool m_bLerpComplete;
	bool m_bLerpFullCompletion;

	float m_flLerpForegroundWidth;
	float m_flLerpBackgroundWidth;
	double m_dblLerpEaseOut;

	const char* m_pszTextOne;
	const char* m_pszTextTwo;
	const char* m_pszTextThree;

	ImColor m_NotificationColor;

	float m_flNextNotificationPadding;

	CNotifications* m_pOwner;
} CNotification;

using CNotification = struct CNotification;

typedef struct CNotifications {
	CNotifications();

	void PushNotification(_In_z_ const char* _TextOne, _In_z_ const char* _TextTwo, _In_z_ const char* _TextThree, _In_ double _TimeToLive = 4000.0, ImColor _Color = ImColor(255, 147, 183, 255));
	void Process();
	float Interpolate(_In_ float _A, _In_ float _B, _In_ float _T);

	CVector<CNotification*>* m_pvecNotifications;
} CNotifications;

using CNotifications = struct CNotifications;

#else //!__cplusplus
#error C++ compiler required to compile notifications.hpp
#endif //__cplusplus

#endif //NOTIFICATIONS_HPP_GUARD

#undef NOTIFICATIONS_HPP_RECURSE_GUARD
#endif //NOTIFICATIONS_HPP_RECURSE_GUARD