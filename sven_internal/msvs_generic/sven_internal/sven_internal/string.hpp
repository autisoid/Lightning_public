/**
 * Copyright - xWhitey, 2024.
 * string.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef STRING_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in string.hpp
#else //STRING_HPP_RECURSE_GUARD

#define STRING_HPP_RECURSE_GUARD

#ifndef STRING_HPP_GUARD
#define STRING_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define _EVALAUTO ((size_t)-1)

typedef void* (*_MemoryAllocateFunction)(size_t);
typedef void (*_MemoryFreeFunction)(void*);

struct CString;
struct CStandardString;

typedef struct CBasicString {
	friend struct CString;
	friend struct CStandardString;

	CBasicString(_MemoryAllocateFunction _Malloc, _MemoryFreeFunction _Free, const char* _InitialBuffer);
	CBasicString(_MemoryAllocateFunction _Malloc, _MemoryFreeFunction _Free);
	CBasicString(const CBasicString& _Rhs);
	~CBasicString();

	CBasicString& ToLowercase();
	CBasicString& ToUppercase();

	size_t Length() const;
	size_t FastLength() const;

	char* Data() const;
	const char* CStr() const;

	bool IsEmpty();
	bool IsEmpty() const;

	CBasicString& Concatenate(const CBasicString& _Rhs);
	CBasicString& Concatenate(const char* _Rhs);

	CBasicString& operator+=(const CBasicString& _Rhs);
	CBasicString& operator+=(const char* _Rhs);

	int Compare(const CBasicString& _Rhs) const;
	int ICompare(const CBasicString& _Rhs) const;
	bool operator==(const CBasicString& _Rhs) const;

	int Compare(const char* _Rhs) const;
	int ICompare(const char* _Rhs) const;
	bool operator==(const char* _Rhs) const;

	CBasicString& operator=(const CBasicString& _Rhs);
	CBasicString& operator=(const char* _Rhs);

	CBasicString operator+(const CBasicString& _Rhs);
	CBasicString operator+(const char* _Rhs);

	CBasicString SubString(const size_t _Offset = 0, const size_t _Count = _EVALAUTO);
	size_t Find(const char _What, const size_t _Offset = 0);

	CBasicString SubString(const size_t _Offset = 0, const size_t _Count = _EVALAUTO) const;
	size_t Find(const char _What, const size_t _Offset = 0) const;

	const char operator[](const size_t _Which);
	const char operator[](const size_t _Which) const;

	char* m_pchBuffer;
	size_t m_sTotalLength;
private:
	_MemoryAllocateFunction _m_pfnMalloc;
	_MemoryFreeFunction _m_pfnFree;
} CBasicString;

using CBasicString = struct CBasicString;

typedef struct CString : CBasicString {
	friend struct CBasicString;

	CString(const char* _InitialBuffer);
	CString(const CString& _Rhs);
	CString(const CBasicString& _Rhs);
	CString();
	~CString();
} CString;

using CString = struct CString;

typedef struct CStandardString : CBasicString {
	friend struct CBasicString;

	CStandardString(const char* _InitialBuffer);
	CStandardString(const CStandardString& _Rhs);
	CStandardString();
	~CStandardString();
} CStandardString;

using CStandardString = struct CStandardString;

#else //!__cplusplus
#error C++ compiler required to compile string.hpp
#endif //__cplusplus

#endif //STRING_HPP_GUARD

#undef STRING_HPP_RECURSE_GUARD
#endif //STRING_HPP_RECURSE_GUARD