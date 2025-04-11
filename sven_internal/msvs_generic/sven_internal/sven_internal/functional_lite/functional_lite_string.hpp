/**
 * Copyright - xWhitey, 2023.
 * functional_lite_string.hpp - Implementation of str_
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_STRING_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_string.hpp
#else //FUNCTIONAL_LITE_STRING_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_STRING_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_STRING_HPP_GUARD
#define FUNCTIONAL_LITE_STRING_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_allocator.hpp"
#include "functional_lite_assert.hpp"
#include "functional_lite_stddef.hpp"
#include "functional_lite_stdnull.hpp"
#include "functional_lite_memory.hpp"
#include "functional_lite_math.hpp"
#include "functional_lite_preprocessor.hpp"

#ifndef FUNCTIONAL_INT_MAX
#define FUNCTIONAL_INT_MAX 2147483647
#endif //!FUNCTIONAL_INT_MAX

#ifndef FUNCTIONAL_BYTE_SIZE_IN_BITS
#define FUNCTIONAL_BYTE_SIZE_IN_BITS 8
#endif //!FUNCTIONAL_BYTE_SIZE_IN_BITS

#ifndef FUNCTIONAL_CHAR_BIT
#define FUNCTIONAL_CHAR_BIT (sizeof(char) * FUNCTIONAL_BYTE_SIZE_IN_BITS)
#endif //!FUNCTIONAL_CHAR_BIT

 //(-)2147483647
#define INT_STR_SIZE (sizeof(int) * FUNCTIONAL_CHAR_BIT / 3 + 3)

//(-)3.402823466e+38F
#define FLOAT_STR_SIZE (sizeof(float) * FUNCTIONAL_CHAR_BIT / 3 + 18)

//Yes, it is slow.
int Q_strlen(_In_z_ const char* _Str) {
	const char* p = const_cast<char*>(&_Str[0]);
	int count = 0;

	for (; p[0] != '\0'; ++p, count++);

	return count;
}

char* Q_strcpy(_Always_(_Post_z_) _Out_ char* _Destination, _In_z_ const char* _Source) {
	if (!_Source) return _Destination;

	if (_Source) {
		const functional_size_t length = Q_strlen(_Source);
		for (unsigned int idx = 0; idx < length; idx++) {
			_Destination[idx] = _Source[idx];
		}
	}

	return _Destination;
}

char* Q_strcat(_Inout_ char* _Destination, _In_z_ const char* _Source) {
	if (!_Source) return _Destination;

	if (_Source) {
		const int srcLength = Q_strlen(_Source);
		const int destLength = Q_strlen(_Destination);
		for (int idx = destLength; idx < destLength + srcLength; idx++) {
			_Destination[idx] = _Source[idx - destLength];
		}
	}

	return _Destination;
}

int Q_strcmp(_In_z_ const char* _Str1, _In_z_ const char* _Str2) {
	for (; *_Str1 == *_Str2; ++_Str1, ++_Str2) {
		if (*_Str1 == '\0') return 0;
	}

	return Q_strlen(_Str1) < Q_strlen(_Str2) ? -1 : 1;
}

int Q_strncmp(_In_reads_or_z_(_MaxCount) const char* _Str1, _In_reads_or_z_(_MaxCount) const char* _Str2, _In_ functional_size_t _MaxCount) {
	while (_MaxCount && *_Str1 && (*_Str1 == *_Str2)) ++_Str1, ++_Str2, --_MaxCount;

	if (_MaxCount == 0) return 0;

	return(*(unsigned char*)_Str1 - *(unsigned char*)_Str2);
}

char* Q_strdup(_In_z_ const char* _Source) {
	const auto buffer = static_cast<char*>(Q_malloc(Q_strlen(_Source) + 1));
	Q_SLOWASSERT(buffer && "Failed to allocate buffer (size is dynamic) at Q_strdup");

	Q_memcpy(buffer, _Source, Q_strlen(_Source));

	buffer[Q_strlen(_Source) + 1] = '\0';

	return buffer;
}

char Q_tolower(_In_ char _C) {
	if (_C >= 'A' && _C <= 'Z') {
		_C += ('a' - 'A');
	}

	return _C;
}

int Q_stricmp(_In_z_ const char* _Str1, _In_z_ const char* _Str2) {
	//Q_ASSERT(_Str1 && _Str2);

	while (Q_tolower((unsigned char)*_Str1) == Q_tolower((unsigned char)*_Str2)) {
		if (*_Str1 == '\0')
			return 0;
		_Str1++; _Str2++;
	}

	return (int)Q_tolower((unsigned char)*_Str1) -
		(int)Q_tolower((unsigned char)*_Str2);
}

char* Q_itoa_internal(_Pre_notnull_ _Always_(_Post_z_) _Out_opt_ char* _Dest, _In_ functional_size_t _Size, _In_ int _Value) {
	char buf[INT_STR_SIZE];
	char* p = &buf[INT_STR_SIZE - 1];
	*p = '\0';
	int i = _Value;

	do {
		*(--p) = Q_pp_abs(i % 10) + '0';
		i /= 10;
	} while (i);

	if (_Value < 0) {
		*(--p) = '-';
	}
	const auto len = static_cast<functional_size_t>(&buf[INT_STR_SIZE] - p);
	if (len > _Size) {
		return nullptr;
	}
	return static_cast<char*>(Q_memcpy(_Dest, p, len));
}

int Q_integer_to_octal(_In_ int _Number) {
	int modulo, octal = 0, idx = 1;

	while (_Number != 0) {
		modulo = _Number % 8;
		_Number = _Number / 8;
		octal = octal + (modulo * idx);
		idx = idx * 10;
	}

	return octal;
}

char* Q_itoa(_In_ int _Number) {
	const auto buffer = static_cast<char*>(Q_malloc(INT_STR_SIZE));
	Q_SLOWASSERT(buffer && "Failed to allocate buffer (size=INT_STR_SIZE) at Q_itoa");
	Q_itoa_internal(buffer, INT_STR_SIZE, _Number);
	return buffer;
}

int Q_atoi(char* s) {
	int c = 1, a = 0, sign, end, base = 1;

	if (s[0] == '-')
		sign = -1;
	else if (s[0] <= '9' && s[0] >= '0')
		sign = 1;
	else if (s[0] == '+')
		sign = 2;
	else
		return 0;

	while (s[c] != '\n' && s[c] <= '9' && s[c] >= '0')
		c++;

	if (sign == -1)
		end = 1;
	else if (sign == 1)
		end = 0;
	else {
		end = 1;
		sign = 1;
	}

	for (int i = c - 1; i >= end; i--) {
		a += (s[i] - '0') * base;
		base *= 10;
	}

	return sign * a;
}

char* Q_ftoa(_In_ float _Value, _In_opt_ int _Precision = 2) {
	const auto buffer = static_cast<char*>(Q_malloc(FLOAT_STR_SIZE));
	int b, l, i = 0;
	if (_Value < 0.f) {
		buffer[i++] = '-';
		_Value *= -1;
	}
	int a = (int)_Value;
	_Value -= a;
	int k = 0;
	while (true) {
		l = Q_pow(10, k);
		int m = a / l;
		if (m == 0) {
			break;
		}
		k++;
	}
	k--;
	for (l = k + 1; l > 0; l--) {
		b = Q_pow(10, l - 1);
		const int c = a / b;
		buffer[i++] = c + '0';
		a %= b;
	}

	if (_Precision != 0)
		buffer[i++] = '.';

	for (l = 0; l < _Precision; l++) {
		_Value *= 10.0;
		b = (int)_Value;
		buffer[i++] = b + '0';
		_Value -= b;
	}

	buffer[i] = '\0';

	return buffer;
}

char Q_toupper(_In_ char _C) {
	if (_C >= 'a' && _C <= 'z') {
		_C -= ('a' - 'A');
	}

	return _C;
}

char* Q_itohexa_helper(_In_ char* _Dest, _In_ functional_uintptr_t _Val) {
	if (_Val >= 16) {
		_Dest = Q_itohexa_helper(_Dest, _Val / 16);
	}
	*_Dest++ = "0123456789abcdef"[_Val & 15];
	return _Dest;
}

char* Q_itohexa(_In_ functional_uintptr_t _Val) {
	const auto buffer = static_cast<char*>(Q_malloc(32));
	Q_SLOWASSERT(buffer && "Failed to allocate buffer (size=32) at Q_itohexa");
	*Q_itohexa_helper(buffer, _Val) = '\0';

	return buffer;
}

char* Q_itohexa_upper(_In_ functional_uintptr_t _Val) {
	char* buffer = Q_itohexa(_Val);
	const int len = Q_strlen(buffer);

	for (int idx = 0; idx < len; idx++) {
		buffer[idx] = Q_toupper(buffer[idx]);
	}

	return buffer;
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_string.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_STRING_HPP_GUARD

#undef FUNCTIONAL_LITE_STRING_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_STRING_HPP_RECURSE_GUARD