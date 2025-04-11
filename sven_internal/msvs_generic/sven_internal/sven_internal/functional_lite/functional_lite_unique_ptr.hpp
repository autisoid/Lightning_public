/**
 * Copyright - xWhitey, 2023.
 * functional_lite_unique_ptr.hpp - Implementation of std::unique_ptr.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_UNIQUE_PTR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_unique_ptr.hpp
#else //FUNCTIONAL_LITE_UNIQUE_PTR_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_UNIQUE_PTR_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_UNIQUE_PTR_HPP_GUARD
#define FUNCTIONAL_LITE_UNIQUE_PTR_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_assert.hpp"
#include "functional_lite_allocator.hpp"
#include "functional_lite_stddef.hpp"
#include "functional_lite_stdnull.hpp"
#include "functional_lite_type_traits.hpp"

//Non-copyable
template<class _Ty> struct CUniquePointer {
	explicit CUniquePointer(_In_opt_ _Ty* _Pointer = nullptr) {
		this->_m_pStorage = _Pointer;
}

	~CUniquePointer() {
		Q_delete(this->_m_pStorage);
		this->_m_pStorage = nullptr;
	}

	_Ty& operator*() const {
		Q_ASSERT(this->_m_pStorage);
		return *this->_m_pStorage;
	}

	_Ty* operator->() const {
		Q_ASSERT(this->_m_pStorage);
		return this->_m_pStorage;
	}

	_Ty* get() const {
		return this->_m_pStorage;
	}

	void reset() {
		Q_delete(this->_m_pStorage);
		this->_m_pStorage = nullptr;
	}

	_Ty* release() {
		_Ty* result = this->_m_pStorage;
		this->_m_pStorage = nullptr;

		return result;
	}
private:
	_Ty* _m_pStorage;

	CUniquePointer(CUniquePointer const&);
	CUniquePointer& operator=(CUniquePointer const&);

	void operator==(CUniquePointer const&) const;
	void operator!=(CUniquePointer const&) const;
};

FUNCTIONAL_NAMESPACE_BEGIN
	template<class _Ty, class... _Ts> CUniquePointer<_Ty> make_unique(_In_opt_ _Ts&&... _Args) {
		return CUniquePointer<_Ty>(Q_new(_Ty)(functional::forward<_Ts>(_Args)...));
	}
FUNCTIONAL_NAMESPACE_END

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_unique_ptr.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_UNIQUE_PTR_HPP_GUARD

#undef FUNCTIONAL_LITE_UNIQUE_PTR_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_UNIQUE_PTR_HPP_RECURSE_GUARD