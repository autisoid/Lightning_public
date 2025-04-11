/**
 * Copyright - xWhitey, 2023.
 * functional_lite_vector.hpp - A simple implementation of std::vector.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_VECTOR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_vector.hpp
#else //FUNCTIONAL_LITE_VECTOR_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_VECTOR_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_VECTOR_HPP_GUARD
#define FUNCTIONAL_LITE_VECTOR_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_assert.hpp"
#include "functional_lite_allocator.hpp"
#include "functional_lite_cast.hpp"
#include "functional_lite_stddef.hpp"
#include "functional_lite_stdnull.hpp"
#include "functional_lite_memory.hpp"

template<class _Ty> struct CVector { // NOLINT(bugprone-reserved-identifier)
	int m_nSize;
	int m_nCapacity;
	_Ty* m_a_pData;

	typedef _Ty value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;

	inline CVector() {
		m_nSize = 0;
		m_nCapacity = 0;
		m_a_pData = nullptr;
	}

	inline CVector(const CVector<_Ty>& src) {
		m_nSize = 0;
		m_nCapacity = 0;
		m_a_pData = nullptr;
		operator=(src);
	}

	inline CVector<_Ty>& operator=(const CVector<_Ty>& src) {
		clear();
		resize(src.m_nSize);

		if (src.m_a_pData)
			Q_memcpy(m_a_pData, src.m_a_pData, functional_size_t(m_nSize * sizeof(_Ty)));

		return *this;
	}

	inline ~CVector() {
		if (m_a_pData)
			Q_free(m_a_pData);
	}
	
	inline _Ty* c_arr() {
		return m_a_pData;
	}

	inline void clear() {
		if (m_a_pData) {
			m_nSize = m_nCapacity = 0;
			Q_free(m_a_pData);
			m_a_pData = nullptr;
		}
	}

	inline void clear_delete() {
		for (int idx = 0; idx < m_nSize; idx++) {
			Q_delete(m_a_pData[idx]);
		}

		clear();
	}

	inline void clear_destruct() {
		for (int idx = 0; idx < m_nSize; idx++) {
			m_a_pData[idx].~_Ty();
		}

		clear();
	}

	inline bool empty() const {
		return m_nSize == 0;
	}

	inline int size() const {
		return m_nSize;
	}

	inline int size_in_bytes() const {
		return m_nSize * sizeof(_Ty);
	}

	inline int max_size() const {
		return 0x7FFFFFFF / sizeof(_Ty);
	}

	inline int capacity() const {
		return m_nCapacity;
	}

	inline _Ty& operator[](_In_ int _Which) {
		Q_ASSERT(_Which >= 0 && _Which < m_nSize);

		return m_a_pData[_Which];
	}

	inline const _Ty& operator[](_In_ int _Which) const {
		Q_ASSERT(_Which >= 0 && _Which < m_nSize);

		return m_a_pData[_Which];
	}

	inline _Ty& at(_In_ int _Which) const {
		Q_ASSERT(_Which >= 0 && _Which < m_nSize);

		return m_a_pData[_Which];
	}

	inline _Ty* begin() {
		return m_a_pData;
	}

	inline const _Ty* begin() const {
		return m_a_pData;
	}

	inline _Ty* end() {
		return m_a_pData + m_nSize;
	}

	inline const _Ty* end() const {
		return m_a_pData + m_nSize;
	}

	inline _Ty& front() {
		Q_ASSERT(m_nSize > 0);

		return m_a_pData[0];
	}

	inline const _Ty& front() const {
		Q_ASSERT(m_nSize > 0);

		return m_a_pData[0];
	}

	inline _Ty& back() {
		Q_ASSERT(m_nSize > 0);

		return m_a_pData[m_nSize - 1];
	}

	inline const _Ty& back() const {
		Q_ASSERT(m_nSize > 0);

		return m_a_pData[m_nSize - 1];
	}

	inline void swap(_In_ CVector<_Ty>& _Rhs) {
		int rhs_size = _Rhs.Size;

		_Rhs.Size = m_nSize;
		m_nSize = rhs_size;

		int rhs_cap = _Rhs.Capacity;
		_Rhs.Capacity = m_nCapacity;
		m_nCapacity = rhs_cap;

		_Ty* rhs_data = _Rhs.m_a_pData;
		_Rhs.m_a_pData = m_a_pData;

		m_a_pData = rhs_data;
	}


	inline int _grow_capacity(_In_ int _Size) const {
		int new_capacity = m_nCapacity ? (m_nCapacity + m_nCapacity / 2) : 8;

		return new_capacity > _Size ? new_capacity : _Size;
	}

	inline void resize(_In_ int _Size) {
		if (_Size > m_nCapacity)
			this->reserve(_grow_capacity(_Size));

		m_nSize = _Size;
	}

	inline void resize(_In_ int _NewSize, _In_ const _Ty& _Placeholder) {
		if (_NewSize > m_nCapacity) {
			reserve(_grow_capacity(_NewSize));
		}

		if (_NewSize > m_nSize) {
			for (int idx = m_nSize; idx < _NewSize; idx++) {
				Q_memcpy(&m_a_pData[idx], &_Placeholder, sizeof(_Ty));
			}
		}

		m_nSize = _NewSize;
	}

	inline void shrink(_In_ int _NewSize) {
		Q_ASSERT(_NewSize <= m_nSize);

		m_nSize = _NewSize;
	}

	inline void reserve(_In_ int _Capacity) {
		if (_Capacity <= m_nCapacity)
			return;

		_Ty* new_data = indirect_cast<_Ty*>(Q_malloc((functional_size_t)(_Capacity * sizeof(_Ty))));

		if (m_a_pData) {
			Q_memcpy(new_data, m_a_pData, functional_size_t(m_nSize * sizeof(_Ty)));

			Q_free(m_a_pData);
		}

		m_a_pData = new_data;
		m_nCapacity = _Capacity;
	}

	inline void reserve_discard(_In_ int _Capacity) {
		if (_Capacity <= m_nCapacity)
			return;

		if (m_a_pData) {
			Q_free(m_a_pData);
		}

		m_a_pData = indirect_cast<_Ty*>(Q_malloc(indirect_cast<functional_size_t>(_Capacity * sizeof(_Ty))));
		m_nCapacity = _Capacity;
	}

	inline void push_back(_In_ const _Ty& _What) {
		if (m_nSize == m_nCapacity) {
			reserve(_grow_capacity(m_nSize + 1));
		}

		Q_memcpy(&m_a_pData[m_nSize], &_What, sizeof(_Ty));

		m_nSize++;
	}

	inline void pop_back() {
		Q_ASSERT(m_nSize > 0);

		m_nSize--;
	}

	inline void push_front(_In_ const _Ty& _What) {
		if (m_nSize == 0) {
			push_back(_What);
		} else {
			insert(m_a_pData, _What);
		}
	}

	inline _Ty* erase(_In_ const _Ty* _Which) {
		Q_ASSERT(_Which >= m_a_pData && _Which < m_a_pData + m_nSize);

		const Q_ptrdiff_t off = _Which - m_a_pData;

		Q_memmove(m_a_pData + off, m_a_pData + off + 1, ((functional_size_t)m_nSize - (functional_size_t)off - 1) * sizeof(_Ty));

		m_nSize--;

		return m_a_pData + off;
	}

	inline _Ty* erase(_In_ const _Ty* _It, _In_ const _Ty* _ItLast) {
		Q_ASSERT(_It >= m_a_pData && _It < m_a_pData + m_nSize && _ItLast >= _It && _ItLast <= m_a_pData + m_nSize);

		const Q_ptrdiff_t count = _ItLast - _It;
		const Q_ptrdiff_t off = _It - m_a_pData;

		Q_memmove(m_a_pData + off, m_a_pData + off + count, (indirect_cast<functional_size_t>(m_nSize) - indirect_cast<functional_size_t>(off) - indirect_cast<functional_size_t>(count)) * sizeof(_Ty));

		m_nSize -= (int)count;

		return m_a_pData + off;
	}

	inline _Ty* erase_unsorted(_In_ const _Ty* _It) {
		Q_ASSERT(_It >= m_a_pData && _It < m_a_pData + m_nSize);

		const Q_ptrdiff_t off = _It - m_a_pData;

		if (_It < m_a_pData + m_nSize - 1) {
			Q_memcpy(m_a_pData + off, m_a_pData + m_nSize - 1, sizeof(_Ty));
		}

		m_nSize--;

		return m_a_pData + off;
	}

	inline _Ty* insert(_In_ const _Ty* _It, const _Ty& _What) {
		Q_ASSERT(_It >= m_a_pData && _It <= m_a_pData + m_nSize);

		const Q_ptrdiff_t off = _It - m_a_pData;

		if (m_nSize == m_nCapacity) {
			reserve(_grow_capacity(m_nSize + 1));
		}

		if (off < indirect_cast<Q_ptrdiff_t>(m_nSize)) {
			Q_memmove(m_a_pData + off + 1, m_a_pData + off, ((indirect_cast<functional_size_t>(m_nSize) - indirect_cast<functional_size_t>(off)) * sizeof(_Ty)));
		}

		Q_memcpy(&m_a_pData[off], &_What, sizeof(_Ty));

		m_nSize++;

		return m_a_pData + off;
	}

	inline bool contains(_In_ const _Ty& _Which) const {
		const _Ty* data = m_a_pData;
		const _Ty* data_end = m_a_pData + m_nSize;

		while (data < data_end)
			if (*data++ == _Which)
				return true;

		return false;
	}

	inline _Ty* find(_In_ const _Ty& _What) {
		_Ty* data = m_a_pData;
		const _Ty* data_end = m_a_pData + m_nSize;

		while (data < data_end)
			if (*data == _What)
				break;
			else
				++data;

		return data;
	}

	inline const _Ty* find(_In_ const _Ty& _What) const {
		const _Ty* data = m_a_pData;
		const _Ty* data_end = m_a_pData + m_nSize;

		while (data < data_end)
			if (*data == _What)
				break;
			else
				++data;

		return data;
	}

	inline bool find_erase(_In_ const _Ty& _What) {
		const _Ty* it = find(_What);

		if (it < m_a_pData + m_nSize) {
			erase(it);
			return true;
		}

		return false;
	}

	inline bool find_erase_unsorted(_In_ const _Ty& _What) {
		const _Ty* it = find(_What);

		if (it < m_a_pData + m_nSize) {
			erase_unsorted(it);
			return true;
		}

		return false;
	}

	inline int index_from_ptr(_In_ const _Ty* _What) const {
		Q_ASSERT(_What >= m_a_pData && _What < m_a_pData + m_nSize);

		const Q_ptrdiff_t off = _What - m_a_pData;

		return indirect_cast<int>(off);
	}
};

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_vector.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_VECTOR_HPP_GUARD

#undef FUNCTIONAL_LITE_VECTOR_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_VECTOR_HPP_RECURSE_GUARD