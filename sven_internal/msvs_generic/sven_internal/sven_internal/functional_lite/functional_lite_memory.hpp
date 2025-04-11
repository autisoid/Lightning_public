/**
 * Copyright - xWhitey, 2023.
 * functional_lite_memory.hpp - description
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_MEMORY_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_memory.hpp
#else //FUNCTIONAL_LITE_MEMORY_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_MEMORY_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_MEMORY_HPP_GUARD
#define FUNCTIONAL_LITE_MEMORY_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_allocator.hpp"
#include "functional_lite_assert.hpp"
#include "functional_lite_stddef.hpp"
#include "functional_lite_stdnull.hpp"

Q_static_assert(sizeof(CAllocator) != 0 && "must be");

void* Q_memcpy(_Out_writes_bytes_all_(_Size) void* _Dst, _In_reads_bytes_(_Size) const void* _Src, _In_ unsigned int _Size) {
	auto dest = static_cast<char*>(_Dst);
	auto source = static_cast<const char*>(_Src);

	Q_SLOWASSERT(dest && source && "Q_memcpy: Where should I store copied values?");

	if (dest && source) {
		while (_Size) {
			*dest++ = *source++;
			--_Size;
		}
	}

	return dest;
}

void* Q_memset(_Out_writes_bytes_all_(_Size) void* _Dst, _In_reads_bytes_(_Size) functional_size_t _Value, _In_ unsigned int _Size) {
	Q_SLOWASSERT(_Dst && "Q_memset: Where should I store your values?");

	if (_Dst) {
		for (unsigned int idx = 0; idx < _Size && _Dst; idx++) {
			((unsigned char*)_Dst)[idx] = _Value;
		}
	}

	return _Dst;
}

void* Q_memmove(void* _Dst, _In_reads_bytes_(_Size) const void* _Src, _In_ unsigned int _Size) {
	char* dest = static_cast<char*>(_Dst);
	const char* src = static_cast<const char*>(_Src);

	char* tmp = static_cast<char*>(Q_malloc(_Size));
	if (!tmp) {
		return nullptr;
	} else {
		for (unsigned int idx = 0; idx < _Size; ++idx) {
			tmp[idx] = src[idx];
		}
		for (unsigned int idx = 0; idx < _Size; ++idx) {
			dest[idx] = tmp[idx];
		}
		Q_free(tmp);
	}

	return dest;
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_memory.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_MEMORY_HPP_GUARD

#undef FUNCTIONAL_LITE_MEMORY_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_MEMORY_HPP_RECURSE_GUARD