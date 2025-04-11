/**
 * Copyright - xWhitey, 2024.
 * callstack.hpp - A dynamically-sized callstack
 *
 * Generic project (This file can be used anywhere) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CALLSTACK_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in callstack.hpp
#else //CALLSTACK_HPP_RECURSE_GUARD

#define CALLSTACK_HPP_RECURSE_GUARD

#ifndef CALLSTACK_HPP_GUARD
#define CALLSTACK_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "queue.hpp"

typedef struct CStackStorageableWrapper {
	CStackStorageableWrapper(_In_ void* _Body, _In_ int _SizeOfBody) {
		m_pBody = _Body;
		m_nSizeOfBody = _SizeOfBody;
	}

	void* m_pBody;
	int m_nSizeOfBody;
} CStackStorageableWrapper;

using CStackStorageableWrapper = struct CStackStorageableWrapper;

typedef struct CCallstack {
	CCallstack() {
		m_pTheQueue = Q_new(CQueue<CStackStorageableWrapper*>)();
	}

	template<class _Ty> void Push(_Ty const& _Rhs) {
		void* pRightHandSide = (void*)(&_Rhs);
		CStackStorageableWrapper* pStorage = Q_new(CStackStorageableWrapper)(pRightHandSide, sizeof(_Rhs));
		m_pTheQueue->Push(pStorage);
	}

	template<class _Ty> _Ty& Pop() {
		CStackStorageableWrapper* pStorage = m_pTheQueue->Pop();
		if (sizeof(_Ty) != pStorage->m_nSizeOfBody) {
			printf("CCalstack#Pop<_Ty>: sizeof(_Ty) (%d) isn't equal to pStorage->m_nSizeOfBody (%d)!\n", sizeof(_Ty), pStorage->m_nSizeOfBody);
		}
		_Ty* pBody = (_Ty*) pStorage->m_pBody;
		Q_delete(pStorage);

		return *pBody;
	}

	CQueue<CStackStorageableWrapper*>* m_pTheQueue;
} CCallstack;

using CCallstack = struct CCallstack;

extern CCallstack g_Callstack;

#else //!__cplusplus
#error C++ compiler required to compile callstack.hpp
#endif //__cplusplus

#endif //CALLSTACK_HPP_GUARD

#undef CALLSTACK_HPP_RECURSE_GUARD
#endif //CALLSTACK_HPP_RECURSE_GUARD