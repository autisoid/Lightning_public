/**
 * Copyright - xWhitey, 2022.
 * utils.hpp - Various utilities
 */

#ifdef UTILS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in utils.hpp
#else //UTILS_HPP_RECURSE_GUARD

#define UTILS_HPP_RECURSE_GUARD

#ifndef UTILS_HPP_GUARD
#define UTILS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifndef DEG2RAD
#define DEG2RAD(_Value) ((float)(_Value) * (float)(M_PI / 180.f))
#endif //!DEG2RAD

#ifndef RAD2DEG
#define RAD2DEG(_Value) (((float)(_Value) * (float)(180.f / M_PI)))
#endif //!RAD2DEG

void AngleVectors(_In_ const Vector& _Target, _In_ Vector* _Forward, _In_ Vector* _Right, _In_ Vector* _Up);
void NormalizeAngles(_In_ Vector& _Angles);
void VectorAngles(_In_ const Vector& _Forward, _In_ Vector& _Angles);

template<class _Ty> struct CReferenceKeeper {
	CReferenceKeeper(_Ty& _Rhs) : m_Ref(_Rhs) {
		;
	}
	CReferenceKeeper(_Ty&& _Rhs) = delete; //we don't store rvalue references

	operator _Ty&() {
		return m_Ref;
	}

	_Ty& m_Ref;
};

typedef struct CUtils {
	static unsigned int BuildIntFromUnsignedChars(_In_ unsigned char* _Array);

	static bool CalcScreen(_In_ const Vector& _Origin, _In_ Vector2D& _Screen, _In_ const SCREENINFO& _ScreenInfo);
		
	static float Clamp(_In_ float _Min, _In_ float _Max, _In_ float _Value);

	static float Hue2RGB(float p, float q, float t);

	static Vector HSL2RGB(Vector _HSL);

	/*
		SetRotation used to be this, but it worked bad with roll angles.

		if (_Cmd->viewangles.x >= 90.f || _Cmd->viewangles.x <= -90.f)
		_Cmd->forwardmove = -_Cmd->forwardmove;

		Vector vecAngles(_Angles), vecPrevAngles(_Cmd->viewangles);

		vecAngles.Normalize();

		vecPrevAngles.Normalize();

		float flSpeed = Vector2D(_Cmd->forwardmove, _Cmd->sidemove).Length();

		if (flSpeed != 0.f) {
			float flYaw = vecAngles.y - vecPrevAngles.y;
			float flYawRadians = atan2(_Cmd->sidemove, _Cmd->forwardmove) + DEG2RAD(flYaw);

			_Cmd->forwardmove = cos(flYawRadians) * flSpeed;
			_Cmd->sidemove = sin(flYawRadians) * flSpeed;

			bool bBackwardsOld = (vecPrevAngles.x > 90.f || vecPrevAngles.x < -90.f);
			bool bBackwardsNew = (vecAngles.x > 90.f || vecAngles.x < -90.f);

			if (bBackwardsOld != bBackwardsNew)
				_Cmd->forwardmove = -_Cmd->forwardmove;
		}

		_Cmd->viewangles = vecAngles;
	*/

	static void SetRotation(_In_ usercmd_s* _Cmd, _In_ Vector _Angles);
} CUtils;

using CUtils = struct CUtils;

#else //__cplusplus
#error C++ compiler required to compile utils.hpp.
#endif //__cplusplus

#endif //UTILS_HPP_GUARD

#undef UTILS_HPP_RECURSE_GUARD
#endif //UTILS_HPP_RECURSE_GUARD