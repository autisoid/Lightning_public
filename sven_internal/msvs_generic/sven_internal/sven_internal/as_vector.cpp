#include "StdAfx.h"
#include "as_vector.hpp"

CASVector* CASVectorFactory(float _X, float _Y, float _Z) {
	return Q_new(CASVector)(_X, _Y, _Z);
}

CASVector* CASVectorFactory() {
	return Q_new(CASVector)();
}

CASVector::CASVector(float _X, float _Y, float _Z) {
	m_nRefCount = 0;

	x = _X;
	y = _Y;
	z = _Z;
}

CASVector::CASVector() {
	m_nRefCount = 0;

	x = y = z = 0.f;
}

void CASVector::AddRef() {
	m_nRefCount++;
}

void CASVector::Release() {
	if (--m_nRefCount == 0)
		Q_delete(this);
}

CASVector& CASVector::operator=(const CASVector& _Rhs) {
	x = _Rhs.x;
	y = _Rhs.y;
	z = _Rhs.z;

	return *this;
}

float* CASVector::At(unsigned int _Which) {
	if (_Which > 2) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CASVector::At: an attempt to access data at invalid index %d was made\n", _Which);

		return nullptr;
	}

	return &((&x)[_Which]);
}

float CASVector::At(unsigned int _Which) const {
	if (_Which > 2) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CASVector::At: an attempt to access data at invalid index %d was made\n", _Which);

		return NAN;
	}

	return (&x)[_Which];
}

float CASVector::LengthSqr() {
	return x * x + y * y + z * z;
}

float CASVector::Length() {
	return sqrtf(LengthSqr());
}

float CASVector::Length2DSqr() {
	return x * x + y * y;
}

float CASVector::Length2D() {
	return sqrtf(Length2DSqr());
}

CASVector CASVector::operator*(float _Value) {
	return CASVector(x * _Value, y * _Value, z * _Value);
}

CASVector CASVector::operator/(float _Value) {
	return CASVector(x / _Value, y / _Value, z / _Value);
}

bool CASVector::operator==(const CASVector& _Rhs) {
	return x == _Rhs.x && y == _Rhs.y && z == _Rhs.z;
}

CASVector CASVector::operator+(const CASVector& _Rhs) {
	return CASVector(x + _Rhs.x, y + _Rhs.y, z + _Rhs.z);
}

CASVector CASVector::operator-(const CASVector& _Rhs) {
	return CASVector(x - _Rhs.x, y - _Rhs.y, z - _Rhs.z);
}