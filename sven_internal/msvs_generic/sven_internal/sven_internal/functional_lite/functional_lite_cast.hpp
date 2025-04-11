/**
 * Copyright - xWhitey, 2023.
 * functional_lite_cast.hpp - Our own weird implementation of reinterpret_cast. Though it can cast away "const" and "volatile" :eyes:.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_CAST_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_cast.hpp
#else //FUNCTIONAL_LITE_CAST_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_CAST_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_CAST_HPP_GUARD
#define FUNCTIONAL_LITE_CAST_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_allocator.hpp"
#include "functional_lite_type_traits.hpp"
#include "functional_lite_stdnull.hpp"
#include "functional_lite_memory.hpp"

template<class _To, class _From> _To union_cast(_From&& _What) {
	union {
		functional::remove_reference_t<_From>* m_pFrom;
		functional::remove_reference_t<_To>* m_pTo;
	} c_uCaster;

	c_uCaster.m_pFrom = &_What;

	return (*c_uCaster.m_pTo);
}

template<class _Callee> union uStorage {
	uStorage() {}
	functional::decay_t<_Callee> callee;
};

template<class _Callee, class _ReturnType, class... _Ts> auto functional_cast_internal(_Callee&& c, _ReturnType(*)(_Ts...)) {
	static bool used = false;
	static uStorage<_Callee> s;
	typedef decltype(s.callee) type;

	if (used)
		s.callee.~type();

	new (INewWrapper(), &s.callee) type(functional::forward<_Callee>(c));
	used = true;

	return [](_Ts... _Args) -> _ReturnType {
		return _ReturnType(s.callee(functional::forward<_Ts>(_Args)...));
	};
}

//Deduction is disabled here. - xWhitey
template<class _ReturnType, class _Which, class = functional::enable_if_t<true, _ReturnType>, class = typename functional::CForceNoDeduction<_ReturnType>::type > _ReturnType* functional_cast(_Which&& c) {
	return functional_cast_internal(functional::forward<_Which>(c), (_ReturnType*)nullptr);
}

template<class _To, class _From, class = functional::enable_if_t<true, _To>, class = functional::CForceNoDeduction<_To>> _To indirect_cast(_From&& _What) {
	return functional_cast<_To()>([_What]() { return _What; })();
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_cast.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_CAST_HPP_GUARD

#undef FUNCTIONAL_LITE_CAST_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_CAST_HPP_RECURSE_GUARD