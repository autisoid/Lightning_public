/**
 * Copyright - xWhitey, 2024.
 * string_utils.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef STRING_UTILS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in string_utils.hpp
#else //STRING_UTILS_HPP_RECURSE_GUARD

#define STRING_UTILS_HPP_RECURSE_GUARD

#ifndef STRING_UTILS_HPP_GUARD
#define STRING_UTILS_HPP_GUARD
#pragma once

#ifdef __cplusplus

size_t Q_strnlen(const char* _String, size_t _NumberOfElements);
size_t Q_strlen(const char* _String);
int Q_strncpy_s(char* _Destination, size_t _NumberOfElements, const char* _Source, size_t _Count);
int Q_strncat_s(char* _Destination, size_t _NumberOfElements, const char* _Source, size_t _Count);
char Q_tolower(char _Character);
char Q_toupper(char _Character);
int Q_strncmp_s(const char* _String1, size_t _NumberOfElements, const char* _String2, size_t _Count);
int Q_strnicmp_s(const char* _String1, size_t _NumberOfElements, const char* _String2, size_t _Count);
//int Q_snprintf_s(char* _Buffer, size_t _SizeOfBuffer, size_t _Count, const char* _Format, ...);
//int Q_vsnprintf_s(char* _Buffer, size_t _SizeOfBuffer, size_t _Count, const char* _Format, va_list _VarArgs);

#else //!__cplusplus
#error C++ compiler required to compile string_utils.hpp
#endif //__cplusplus

#endif //STRING_UTILS_HPP_GUARD

#undef STRING_UTILS_HPP_RECURSE_GUARD
#endif //STRING_UTILS_HPP_RECURSE_GUARD