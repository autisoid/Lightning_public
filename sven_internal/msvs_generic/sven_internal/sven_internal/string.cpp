#include "StdAfx.h"
#include "string.hpp"
#include "string_utils.hpp"

CBasicString::CBasicString(_MemoryAllocateFunction _Malloc, _MemoryFreeFunction _Free, const char* _InitialBuffer) : _m_pfnMalloc(nullptr), _m_pfnFree(nullptr), m_pchBuffer(nullptr), m_sTotalLength(0) {
	if (!_InitialBuffer || !_Malloc || !_Free) return;
	_m_pfnMalloc = _Malloc;
	_m_pfnFree = _Free;
	size_t sInitialBufferLength = Q_strlen(_InitialBuffer);
	m_pchBuffer = (char*)_m_pfnMalloc(sInitialBufferLength + 1);
	m_sTotalLength = sInitialBufferLength;
	if (Q_strncpy_s(m_pchBuffer, sInitialBufferLength + 1, _InitialBuffer, sInitialBufferLength)) {
		//assert(false && "must be");
	}
}

CBasicString::CBasicString(_MemoryAllocateFunction _Malloc, _MemoryFreeFunction _Free) : _m_pfnMalloc(nullptr), _m_pfnFree(nullptr), m_pchBuffer(nullptr), m_sTotalLength(0) {
	if (!_Malloc || !_Free) return;
	_m_pfnMalloc = _Malloc;
	_m_pfnFree = _Free;
	m_pchBuffer = (char*)_m_pfnMalloc(sizeof('\0'));
	m_sTotalLength = 0;
	m_pchBuffer[0] = '\0';
}

CBasicString::CBasicString(const CBasicString& _Rhs) : CBasicString(_Rhs._m_pfnMalloc, _Rhs._m_pfnFree, _Rhs.m_pchBuffer) {
}

CBasicString::~CBasicString() {
	if (_m_pfnFree && m_pchBuffer) {
		_m_pfnFree(m_pchBuffer);
		m_pchBuffer = nullptr;
	}
}

size_t CBasicString::Length() const {
	if (!m_pchBuffer) return 0;

	return Q_strlen(m_pchBuffer);
}

size_t CBasicString::FastLength() const {
	return m_sTotalLength;
}

CBasicString& CBasicString::ToLowercase() {
	for (size_t idx = 0; idx < FastLength(); idx++) {
		m_pchBuffer[idx] = Q_tolower(m_pchBuffer[idx]);
	}

	return *this;
}

CBasicString& CBasicString::ToUppercase() {
	for (size_t idx = 0; idx < FastLength(); idx++) {
		m_pchBuffer[idx] = Q_toupper(m_pchBuffer[idx]);
	}

	return *this;
}

char* CBasicString::Data() const {
	return m_pchBuffer;
}

const char* CBasicString::CStr() const {
	return Data();
}

CBasicString& CBasicString::Concatenate(const CBasicString& _Rhs) {
	size_t sNewLength = Length() + _Rhs.Length();

	char* pchNewBuffer = (char*)_m_pfnMalloc(sNewLength + 1);
	Q_strncpy_s(pchNewBuffer, sNewLength + 1, m_pchBuffer, sNewLength);
	Q_strncat_s(pchNewBuffer, sNewLength + 1, _Rhs.m_pchBuffer, sNewLength);
	char* pchTemp = m_pchBuffer;
	m_pchBuffer = pchNewBuffer;
	_m_pfnFree(pchTemp);

	m_sTotalLength = sNewLength;

	return *this;
}

CBasicString& CBasicString::Concatenate(const char* _Rhs) {
	if (!_Rhs)
		return *this;

	size_t sNewLength = Length() + Q_strlen(_Rhs);

	char* pchNewBuffer = (char*)_m_pfnMalloc(sNewLength + 1);
	Q_strncpy_s(pchNewBuffer, sNewLength + 1, m_pchBuffer, sNewLength);
	Q_strncat_s(pchNewBuffer, sNewLength + 1, _Rhs, sNewLength);
	char* pchTemp = m_pchBuffer;
	m_pchBuffer = pchNewBuffer;
	_m_pfnFree(pchTemp);

	m_sTotalLength = sNewLength;

	return *this;
}

CBasicString& CBasicString::operator+=(const CBasicString& _Rhs) {
	return Concatenate(_Rhs);
}

CBasicString& CBasicString::operator+=(const char* _Rhs) {
	return Concatenate(_Rhs);
}

bool CBasicString::IsEmpty() {
	return Length() == 0;
}

bool CBasicString::IsEmpty() const {
	return Length() == 0;
}

int CBasicString::Compare(const CBasicString& _Rhs) const {
	if (this == &_Rhs) return 0;
	if (_Rhs.FastLength() > FastLength()) return 1;
	if (_Rhs.FastLength() < FastLength()) return -1;

	return Q_strncmp_s(m_pchBuffer, m_sTotalLength + 1, _Rhs.m_pchBuffer, m_sTotalLength);
}

int CBasicString::ICompare(const CBasicString& _Rhs) const {
	if (this == &_Rhs) return 0;
	if (_Rhs.FastLength() > FastLength()) return 1;
	if (_Rhs.FastLength() < FastLength()) return -1;

	return Q_strnicmp_s(m_pchBuffer, m_sTotalLength + 1, _Rhs.m_pchBuffer, m_sTotalLength);
}

bool CBasicString::operator==(const CBasicString& _Rhs) const {
	return Compare(_Rhs) == 0;
}

int CBasicString::Compare(const char* _Rhs) const {
	return Q_strncmp_s(m_pchBuffer, m_sTotalLength + 1, _Rhs, m_sTotalLength);
}

int CBasicString::ICompare(const char* _Rhs) const {
	return Q_strnicmp_s(m_pchBuffer, m_sTotalLength + 1, _Rhs, m_sTotalLength);
}

bool CBasicString::operator==(const char* _Rhs) const {
	return Compare(_Rhs) == 0;
}

CBasicString& CBasicString::operator=(const CBasicString& _Rhs) {
	if (this == &_Rhs) return *this;
	m_sTotalLength = _Rhs.m_sTotalLength;
	char* pchTemp = m_pchBuffer;
	m_pchBuffer = (char*)_m_pfnMalloc(m_sTotalLength + 1);
	if (Q_strncpy_s(m_pchBuffer, m_sTotalLength + 1, _Rhs.m_pchBuffer, m_sTotalLength)) {
		//assert(false && "must be");
	}
	_m_pfnFree(pchTemp);
	return *this;
}

CBasicString& CBasicString::operator=(const char* _Rhs) {
	m_sTotalLength = Q_strlen(_Rhs);
	char* pchTemp = m_pchBuffer;
	m_pchBuffer = (char*)_m_pfnMalloc(m_sTotalLength + 1);
	if (Q_strncpy_s(m_pchBuffer, m_sTotalLength + 1, _Rhs, m_sTotalLength)) {
		//assert(false && "must be");
	}
	_m_pfnFree(pchTemp);
	return *this;
}

CBasicString CBasicString::operator+(const CBasicString& _Rhs) {
	CBasicString temp(*this);
	temp.Concatenate(_Rhs);
	return temp;
}

CBasicString CBasicString::operator+(const char* _Rhs) {
	CBasicString temp(*this);
	temp.Concatenate(_Rhs);
	return temp;
}

CBasicString CBasicString::SubString(const size_t _Offset /* = 0 */, const size_t _Count /* = _EVALAUTO */) {
	if (_Offset >= FastLength())
		return CBasicString(_m_pfnMalloc, _m_pfnFree);

	char* pchSubString = &m_pchBuffer[_Offset];
	size_t sRemainingLength = FastLength() - _Offset; // Remaining length from _Offset
	size_t sSubStringLength = (_Count == _EVALAUTO) ? sRemainingLength : min(sRemainingLength, _Count);
	char* pchNewString = (char*)_m_pfnMalloc(sSubStringLength + 1);
	Q_strncpy_s(pchNewString, sSubStringLength + 1, pchSubString, sSubStringLength);

	CBasicString result(_m_pfnMalloc, _m_pfnFree, pchNewString);
	_m_pfnFree(pchNewString);

	return result;
}

CBasicString CBasicString::SubString(const size_t _Offset /* = 0 */, const size_t _Count /* = _EVALAUTO */) const {
	if (_Offset >= FastLength())
		return CBasicString(_m_pfnMalloc, _m_pfnFree);

	char* pchSubString = &m_pchBuffer[_Offset];
	size_t sRemainingLength = FastLength() - _Offset; // Remaining length from _Offset
	size_t sSubStringLength = (_Count == _EVALAUTO) ? sRemainingLength : min(sRemainingLength, _Count);
	char* pchNewString = (char*)_m_pfnMalloc(sSubStringLength + 1);
	Q_strncpy_s(pchNewString, sSubStringLength + 1, pchSubString, sSubStringLength);

	CBasicString result(_m_pfnMalloc, _m_pfnFree, pchNewString);
	_m_pfnFree(pchNewString);

	return result;
}

size_t CBasicString::Find(const char _What, const size_t _Offset /* = 0 */) {
	if (_Offset >= FastLength())
		return _EVALAUTO;

	char* pchBuffer = &m_pchBuffer[_Offset];
	for (size_t sIndex = _Offset; sIndex < FastLength(); sIndex++) {
		if (pchBuffer[sIndex] == _What) {
			return sIndex;
		}
	}

	return _EVALAUTO;
}

size_t CBasicString::Find(const char _What, const size_t _Offset /* = 0 */) const {
	if (_Offset >= FastLength())
		return _EVALAUTO;

	char* pchBuffer = &m_pchBuffer[_Offset];
	for (size_t sIndex = _Offset; sIndex < FastLength(); sIndex++) {
		if (pchBuffer[sIndex] == _What) {
			return sIndex;
		}
	}

	return _EVALAUTO;
}

const char CBasicString::operator[](const size_t _Which) {
	if (_Which >= FastLength())
		return '\0';

	return m_pchBuffer[_Which];
}

const char CBasicString::operator[](const size_t _Which) const {
	if (_Which >= FastLength())
		return '\0';

	return m_pchBuffer[_Which];
}

CString::CString(const char* _InitialBuffer) : CBasicString(Q_malloc, Q_free, _InitialBuffer) {

}

CString::CString() : CBasicString(Q_malloc, Q_free) {

}

CString::CString(const CString& _Rhs) : CBasicString(_Rhs) {

}

CString::CString(const CBasicString& _Rhs) : CBasicString(Q_malloc, Q_free, _Rhs.CStr()) {

}

CString::~CString() {
	if (_m_pfnFree && m_pchBuffer) {
		_m_pfnFree(m_pchBuffer);
		m_pchBuffer = nullptr;
	}
}

CStandardString::CStandardString(const char* _InitialBuffer) : CBasicString(malloc, free, _InitialBuffer) {

}

CStandardString::CStandardString() : CBasicString(malloc, free) {

}

CStandardString::CStandardString(const CStandardString& _Rhs) : CBasicString(_Rhs) {

}

CStandardString::~CStandardString() {
	if (_m_pfnFree && m_pchBuffer) {
		_m_pfnFree(m_pchBuffer);
		m_pchBuffer = nullptr;
	}
}