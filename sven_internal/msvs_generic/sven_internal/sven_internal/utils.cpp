#include "StdAfx.h"

void AngleVectors(_In_ const Vector& _Target, _In_ Vector* _Forward, _In_ Vector* _Right, _In_ Vector* _Up) {
	float sp = sin(DEG2RAD(_Target.x));
	float sy = sin(DEG2RAD(_Target.y));
	float sr = sin(DEG2RAD(_Target.z));

	float cp = cos(DEG2RAD(_Target.x));
	float cy = cos(DEG2RAD(_Target.y));
	float cr = cos(DEG2RAD(_Target.z));

	if (_Forward) {
		_Forward->x = cp * cy;
		_Forward->y = cp * sy;
		_Forward->z = -sp;
	}

	if (_Right) {
		_Right->x = -1.f * sr * sp * cy + -1.f * cr * -sy;
		_Right->y = -1.f * sr * sp * sy + -1.f * cr * cy;
		_Right->z = -1.f * sr * cp;
	}

	if (_Up) {
		_Up->x = cr * sp * cy + -sr * -sy;
		_Up->y = cr * sp * sy + -sr * cy;
		_Up->z = cr * cp;
	}
}

void NormalizeAngles(_In_ Vector& _Angles) {
	for (int i = 0; i < 3; ++i) {
		float flRevolutions = _Angles[i] / 360.f;

		if (_Angles[i] > 180.f || _Angles[i] < -180.f) {
			flRevolutions = roundf(fabsf(flRevolutions));

			if (_Angles[i] < 0) {
				_Angles[i] = (_Angles[i] + 360.f * flRevolutions);
			} else {
				_Angles[i] = (_Angles[i] - 360.f * flRevolutions);
			}
		}
	}
}

void VectorAngles(_In_ const Vector& _Forward, _In_ Vector& _Angles) {
	float flYaw, flPitch;

	if (_Forward.x == 0.f && _Forward.y == 0.f) {
		flYaw = 0.f;
		if (_Forward.z > 0.f)
			flPitch = 90.f;
		else
			flPitch = 270.f;
	} else {
		flYaw = RAD2DEG(atan2(_Forward.y, _Forward.x));
		if (flYaw < 0.f)
			flYaw += 360.f;

		flPitch = RAD2DEG(atan2(_Forward.z, _Forward.Length2D()));
		if (flPitch < 0.f)
			flPitch += 360.f;
	}

	_Angles.x = flPitch;
	_Angles.y = flYaw;
	_Angles.z = 0.f;
}

bool CUtils::CalcScreen(_In_ const Vector& _Origin, _In_ Vector2D& _Screen, _In_ const SCREENINFO& _ScreenInfo) {
	bool bResult = (g_pEngfuncs->pTriAPI->WorldToScreen(_Origin, _Screen) == 0);

	if (_Screen.x < 1 && _Screen.y < 1 && _Screen.x > -1 && _Screen.y > -1 && bResult) {
		_Screen.x = _Screen.x * _ScreenInfo.iWidth / 2 + _ScreenInfo.iWidth / 2;
		_Screen.y = -_Screen.y * _ScreenInfo.iHeight / 2 + _ScreenInfo.iHeight / 2;
		return bResult;
	}

	return false;
}

unsigned int CUtils::BuildIntFromUnsignedChars(_In_ unsigned char* _Array) {
	unsigned int r, result;

	result = _Array[0];
	r = _Array[1];
	result = result | (r << 8);
	r = _Array[2];
	result = result | (r << 16);
	r = _Array[3];
	return result | (r << 24);
}

float CUtils::Clamp(_In_ float _Min, _In_ float _Max, _In_ float _Value) {
	if (_Value <= _Min)
		return _Min;

	if (_Value >= _Max)
		return _Max;

	return _Value;
}

float CUtils::Hue2RGB(float p, float q, float t) {
	if (t < 0.0f) {
		t = t + 1.0f;
	}

	if (t > 1.0f) {
		t = t - 1.0f;
	}

	if (t < 1.0f / 6.0f) {
		return p + (q - p) * 6.0f * t;
	}

	if (t < 1.0f / 2.0f) {
		return q;
	}

	if (t < 2.0f / 3.0f) {
		return p + (q - p) * ((2.0f / 3.0f) - t) * 6.0f;
	}

	return p;
}

Vector CUtils::HSL2RGB(Vector _HSL) {
	Vector vecRGB;

	if (_HSL.y == 0.0f) {
		vecRGB.x = _HSL.z;
		vecRGB.y = _HSL.z;
		vecRGB.z = _HSL.z;

		return vecRGB;
	}

	float q = _HSL.z < 0.5f ? _HSL.z * (1.0f + _HSL.y) : _HSL.z + _HSL.y - _HSL.z * _HSL.y;
	float p = 2.0f * _HSL.z - q;

	vecRGB.x = Hue2RGB(p, q, _HSL.x + (1.0f / 3.0f));
	vecRGB.y = Hue2RGB(p, q, _HSL.x);
	vecRGB.z = Hue2RGB(p, q, _HSL.x - (1.0f / 3.0f));

	return vecRGB;
}

void CUtils::SetRotation(_In_ usercmd_s* _Cmd, _In_ Vector _Angles) {
	const float flBackupFM = _Cmd->forwardmove;
	const float flBackupSM = _Cmd->sidemove;
	const float flBackupUM = _Cmd->upmove;

	Vector vecViewForward, vecViewRight, vecViewUp;
	Vector vecAimForward, vecAimRight, vecAimUp;

	bool bOnlyUseYaw = g_pPlayerMove && g_pPlayerMove->iuser1 == 0 /* spec */;
	if (bOnlyUseYaw)
		AngleVectors(Vector(0.0f, _Cmd->viewangles.y, 0.0f), &vecViewForward, &vecViewRight, &vecViewUp);
	else
		AngleVectors(_Cmd->viewangles, &vecViewForward, &vecViewRight, &vecViewUp);

	_Cmd->viewangles = _Angles;
	NormalizeAngles(_Cmd->viewangles);

	if (bOnlyUseYaw)
		AngleVectors(Vector(0.0f, _Cmd->viewangles.y, 0.0f), &vecAimForward, &vecAimRight, &vecAimUp);
	else
		AngleVectors(_Cmd->viewangles, &vecAimForward, &vecAimRight, &vecAimUp);

	Vector vecForwardMove = vecViewForward * flBackupFM;
	Vector vecSideMove = vecViewRight * flBackupSM;
	Vector vecUpMove = vecViewUp * flBackupUM;

	_Cmd->forwardmove = DotProduct(vecForwardMove + vecSideMove + vecUpMove, vecAimForward);
	_Cmd->sidemove = DotProduct(vecForwardMove + vecSideMove + vecUpMove, vecAimRight);
	_Cmd->upmove = DotProduct(vecForwardMove + vecSideMove + vecUpMove, vecAimUp);

	Vector vecMove(_Cmd->forwardmove, _Cmd->sidemove, _Cmd->upmove);
	float flSpeed = sqrtf(_Cmd->forwardmove * _Cmd->forwardmove + _Cmd->sidemove * _Cmd->sidemove);

	if (flSpeed > 0.0f) {
		Vector vecAngularMove;
		VectorAngles(vecMove, vecAngularMove);
		float flYaw = DEG2RAD(vecAngularMove.y);

		_Cmd->forwardmove = cosf(flYaw) * flSpeed;

		if (_Cmd->viewangles.x >= 90.f || _Cmd->viewangles.x <= -90.f)
			_Cmd->forwardmove *= -1.f;

		_Cmd->sidemove = sinf(flYaw) * flSpeed;
	}
}