/**
 * Copyright - xWhitey, 2024.
 * linked_list.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef LINKED_LIST_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in linked_list.hpp
#else //LINKED_LIST_HPP_RECURSE_GUARD

#define LINKED_LIST_HPP_RECURSE_GUARD

#ifndef LINKED_LIST_HPP_GUARD
#define LINKED_LIST_HPP_GUARD
#pragma once

#ifdef __cplusplus

template<class _Ty> struct CLinkedChainNode {
	_Ty m_Data;
	CLinkedChainNode* m_pPrev;
	CLinkedChainNode* m_pNext;

	CLinkedChainNode(const _Ty& _Data) : m_Data(_Data), m_pPrev(nullptr), m_pNext(nullptr) {}
};

template<class _Ty> struct CLinkedList {
	CLinkedChainNode<_Ty>* m_pHead;
	CLinkedChainNode<_Ty>* m_pTail;

	CLinkedList() : m_pHead(nullptr), m_pTail(nullptr) {
	}

	~CLinkedList() {
		Clear();
	}

	void Append(const _Ty& _Data) {
		CLinkedChainNode<_Ty>* pNewNode = Q_new(CLinkedChainNode<_Ty>)(_Data);
		if (!m_pHead) {
			m_pHead = pNewNode;
			m_pTail = pNewNode;
		} else {
			m_pTail->m_pNext = pNewNode;
			pNewNode->m_pPrev = m_pTail;
			m_pTail = pNewNode;
		}
	}

	bool Remove(const _Ty& _Value) {
		CLinkedChainNode<_Ty>* pCurrent = m_pHead;

		while (pCurrent) {
			if (pCurrent->m_Data == _Value) {
				if (pCurrent == m_pHead) {
					m_pHead = m_pHead->m_pNext;
					if (m_pHead) {
						m_pHead->m_pPrev = nullptr;
					} else {
						m_pTail = nullptr;
					}
				} else if (pCurrent == m_pTail) {
					m_pTail = m_pTail->m_pPrev;
					if (m_pTail) {
						m_pTail->m_pNext = nullptr;
					} else {
						m_pHead = nullptr;
					}
				} else {
					pCurrent->m_pPrev->m_pNext = pCurrent->m_pNext;
					pCurrent->m_pNext->m_pPrev = pCurrent->m_pPrev;
				}

				Q_delete(pCurrent);
				return true;
			}

			pCurrent = pCurrent->m_pNext;
		}

		return false;
	}

	void Clear() {
		CLinkedChainNode<_Ty>* pCurrent = m_pHead;
		while (pCurrent) {
			CLinkedChainNode<_Ty>* pNext = pCurrent->m_pNext;
			Q_delete(pCurrent);
			pCurrent = pNext;
		}
		m_pHead = m_pTail = nullptr;
	}

	bool IsEmpty() const {
		return m_pHead == nullptr;
	}
};

#else //!__cplusplus
#error C++ compiler required to compile linked_list.hpp
#endif //__cplusplus

#endif //LINKED_LIST_HPP_GUARD

#undef LINKED_LIST_HPP_RECURSE_GUARD
#endif //LINKED_LIST_HPP_RECURSE_GUARD