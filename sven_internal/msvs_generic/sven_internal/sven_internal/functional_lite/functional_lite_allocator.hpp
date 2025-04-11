/**
 * Copyright - xWhitey, 2023.
 * functional_lite_allocator.hpp - description
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_ALLOCATOR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_allocator.hpp
#else //FUNCTIONAL_LITE_ALLOCATOR_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_ALLOCATOR_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_ALLOCATOR_HPP_GUARD
#define FUNCTIONAL_LITE_ALLOCATOR_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_assert.hpp"
#include "functional_lite_config.hpp"
#include "functional_lite_stdbool.hpp"
#include "functional_lite_stddef.hpp"
#include "functional_lite_stdnull.hpp"
#include "functional_lite_preprocessor.hpp"

typedef struct CAllocatedSegment {
	bool m_bIsFree;
	functional_size_t m_iSize;
	CAllocatedSegment* m_pNext, *m_pPrevious;
} CAllocatedSegment;

typedef struct CAllocator {
	static CAllocator* Init() {
		static CAllocator allocator = CAllocator{};
		allocator.ClearPool();
		allocator.m_pSegments = (CAllocatedSegment*)allocator._m_acMemoryPool;
		allocator.m_pSegments->m_bIsFree = true;
		allocator.m_pSegments->m_iSize = FUNCTIONAL_HEAP_SIZE / FUNCTIONAL_BLOCK_SIZE;
		allocator.m_pSegments->m_pNext = nullptr;
		allocator.m_pSegments->m_pPrevious = nullptr;
		allocator._m_pOldFreeSegment = nullptr;

		return &allocator;
	}

	void ClearPool() {
		for (unsigned int idx = 0; idx < Q_ARRAYSIZE(this->_m_acMemoryPool) && this->_m_acMemoryPool; idx++) {
			((unsigned char*)this->_m_acMemoryPool)[idx] = 0;
		}
	}

	CAllocatedSegment* SearchFreeSegment(_In_ CAllocatedSegment* _Segment, _In_ functional_size_t _MinSize) {
		while (_Segment) {
			if (_Segment->m_bIsFree && _Segment->m_iSize >= _MinSize) return _Segment;
			_Segment = _Segment->m_pNext;
		}
		return _Segment;
	}

	functional_size_t GetNumBlock(_In_ functional_size_t size) {
		if (size % FUNCTIONAL_BLOCK_SIZE) size += FUNCTIONAL_BLOCK_SIZE;
		return size / FUNCTIONAL_BLOCK_SIZE;
	}

	CAllocatedSegment* CutSegment(_In_ CAllocatedSegment* _Segment, _In_ functional_size_t _Size) {
		functional_uintptr_t addr = reinterpret_cast<functional_uintptr_t>(_Segment);
		addr += (_Segment->m_iSize - _Size) * FUNCTIONAL_BLOCK_SIZE;
		CAllocatedSegment* result = (CAllocatedSegment*)addr;
		_Segment->m_iSize -= _Size;
		result->m_iSize = _Size;
		result->m_pPrevious = _Segment;
		result->m_pNext = _Segment->m_pNext;
		if (_Segment->m_pNext) _Segment->m_pNext->m_pPrevious = result;
		_Segment->m_pNext = result;
		result->m_bIsFree = _Segment->m_bIsFree;
		return result;
	}

	CAllocatedSegment* MergeSegment(_In_ CAllocatedSegment* _Segment, _In_ CAllocatedSegment* _OldSegment) {
		if (this->_m_pOldFreeSegment == _OldSegment) this->_m_pOldFreeSegment = _Segment;
		_Segment->m_iSize += _OldSegment->m_iSize;
		_Segment->m_pNext = _OldSegment->m_pNext;
		if (_OldSegment->m_pNext) _OldSegment->m_pNext->m_pPrevious = _Segment;

		return _Segment;
	}

	void* SegmentToPtr(_In_ CAllocatedSegment* _Segment) {
		Q_SLOWASSERT(_Segment && "Expected _Segment to be non-nullptr");

		return reinterpret_cast<char*>(_Segment) + sizeof(CAllocatedSegment);
	}

	CAllocatedSegment* PtrToSegment(_In_ void* _Pointer) {
		Q_SLOWASSERT(_Pointer && "Expected _Pointer to be non-nullptr");

		return (CAllocatedSegment*)(reinterpret_cast<char*>(_Pointer) - sizeof(CAllocatedSegment));
	}

	void* Allocate(_In_ functional_size_t _Size) {
		if (_Size > FUNCTIONAL_HEAP_SIZE) {
			Q_ASSERT(_Size < FUNCTIONAL_HEAP_SIZE && "Not enough heap size.");
			return nullptr;
		}

		functional_size_t s = GetNumBlock(_Size + sizeof(CAllocatedSegment));
		CAllocatedSegment* it = SearchFreeSegment(this->_m_pOldFreeSegment, s);
		if (!it) it = SearchFreeSegment(this->m_pSegments, s);
		if (!it) {
			Q_ASSERT(it && "Couldn't find any free segments to allocate memory.");
			return nullptr;
		}

		it->m_bIsFree = false;

		if (it->m_iSize > s + GetNumBlock(sizeof(CAllocatedSegment))) {
			CAllocatedSegment* n = CutSegment(it, it->m_iSize - s);
			n->m_bIsFree = true;
			this->_m_pOldFreeSegment = n;
		}

		return SegmentToPtr(it);
	}

	bool AddressNotInSpace(_In_ void* _Pointer, _In_ void* _Low, _In_ void* _High) {
		functional_uintptr_t pointer = (functional_uintptr_t)_Pointer;
		functional_uintptr_t low = (functional_uintptr_t)_Low;
		functional_uintptr_t high = (functional_uintptr_t)_High;

		if (low > high) {
			functional_uintptr_t reverse = high;
			high = low;
			low = reverse;
		}

		return (pointer < low) || (pointer > high);
	}

	void Free(_In_ void* _Pointer) {
		if (!_Pointer) return;
		if (AddressNotInSpace(_Pointer, _m_acMemoryPool, _m_acMemoryPool + FUNCTIONAL_HEAP_SIZE - 1)) return;
		CAllocatedSegment* segment = PtrToSegment(_Pointer);
		segment->m_bIsFree = true;
		for (unsigned int idx = 0; idx < segment->m_iSize && _Pointer; idx++) {
			((unsigned char*)_Pointer)[idx] = 0;
		}

		if (segment->m_pNext && segment->m_pNext->m_bIsFree) MergeSegment(segment, segment->m_pNext);
		if (segment->m_pPrevious && segment->m_pPrevious->m_bIsFree) MergeSegment(segment->m_pPrevious, segment);
	}

	void* Reallocate(_In_ void* _Pointer, _In_ functional_size_t _Size) {
		if (_Size > FUNCTIONAL_HEAP_SIZE) {
			gs_pszAssertionFailureReason = "Not enough heap size.";
			return nullptr;
		}

		if (!_Size) {
			Free(_Pointer);

			return nullptr;
		}

		CAllocatedSegment* segment = PtrToSegment(_Pointer);
		functional_size_t block = GetNumBlock(_Size + sizeof(CAllocatedSegment));
		if (segment->m_iSize >= block) {
			return _Pointer;
		}
		else {
			if (segment->m_pNext && segment->m_pNext->m_bIsFree && segment->m_iSize + segment->m_pNext->m_iSize >= block) {
				MergeSegment(segment, segment->m_pNext);
				if (segment->m_iSize > block + GetNumBlock(sizeof(CAllocatedSegment))) {
					CAllocatedSegment* n = CutSegment(segment, segment->m_iSize - block);
					n->m_bIsFree = true;
				}

				return _Pointer;
			}
			else {
				auto storage = Allocate(_Size);
				functional_size_t size_cpy = _Size;
				auto dest = static_cast<char*>(storage);
				auto source = static_cast<const char*>(_Pointer);
				while (size_cpy) {
					*dest++ = *source++;
					--size_cpy;
				}
				Free(_Pointer);

				return storage;
			}
		}
	}

	CAllocatedSegment* m_pSegments;
private:
	CAllocator() {
		for (unsigned int idx = 0; idx < Q_ARRAYSIZE(this->_m_acMemoryPool) && this->_m_acMemoryPool; idx++) {
			((unsigned char*)this->_m_acMemoryPool)[idx] = 0;
		}
		this->m_pSegments = nullptr;
		this->_m_pOldFreeSegment = nullptr;
	}

	char _m_acMemoryPool[FUNCTIONAL_HEAP_SIZE];
	CAllocatedSegment* _m_pOldFreeSegment;
} CAllocator;

static CAllocator* gs_pAllocator = CAllocator::Init();

CAllocator* Q_GetAllocator() {
	return gs_pAllocator;
}

void* Q_malloc(_In_ functional_size_t _Size) {
	if (!gs_pAllocator || reinterpret_cast<functional_uintptr_t>(gs_pAllocator) == 1) gs_pAllocator = CAllocator::Init();

	return gs_pAllocator->Allocate(_Size);
}

void Q_free(_In_ void* _Pointer) {
	//How did you manage to use free without initializing the allocator, huh?
	//Is your code bugsafe/bugless?
	Q_SLOWASSERT(gs_pAllocator && reinterpret_cast<functional_uintptr_t>(gs_pAllocator) != 1);
	if (!gs_pAllocator || reinterpret_cast<functional_uintptr_t>(gs_pAllocator) == 1) gs_pAllocator = CAllocator::Init();

	gs_pAllocator->Free(_Pointer);
}

void* Q_realloc(_In_ void* _Pointer, _In_ functional_size_t _Size) {
	//How did you manage to use realloc without initializing the allocator, huh?
	//Is your code bugsafe/bugless?
	Q_SLOWASSERT(gs_pAllocator && reinterpret_cast<functional_uintptr_t>(gs_pAllocator) != 1);
	if (!gs_pAllocator || reinterpret_cast<functional_uintptr_t>(gs_pAllocator) == 1) gs_pAllocator = CAllocator::Init();

	return gs_pAllocator->Reallocate(_Pointer, _Size);
}

void Q_clear_allocator() {
	gs_pAllocator->ClearPool();
	gs_pAllocator = CAllocator::Init();
}

//We don't have any new operators since they're defined in CRT library. Instead we define our own which cannot be predefined by anything other because of the interface "INewWrapper"
typedef struct INewWrapper {} INewWrapper;
inline void* operator new(_In_opt_ functional_size_t _Count, _In_ INewWrapper _Wrapper, _In_ void* _Pointer) {
	(void)_Count;
	(void)_Wrapper;

	return _Pointer;
}
inline void operator delete(void*, INewWrapper, void*) {}

#define Q_new(_Type) new (INewWrapper(), Q_malloc(sizeof(_Type))) _Type

template<class _Ty> void Q_delete(_In_ _Ty* _Pointer) {
	if (_Pointer) {
		_Pointer->~_Ty();

		Q_free(_Pointer);
	}
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_allocator.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_ALLOCATOR_HPP_GUARD

#undef FUNCTIONAL_LITE_ALLOCATOR_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_ALLOCATOR_HPP_RECURSE_GUARD