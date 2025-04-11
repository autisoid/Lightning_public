/**
 * Copyright - xWhitey, 2023.
 * functional_lite_parameter_pack.hpp - Here we implement functional to expand parameter packs.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_PARAMETER_PACK_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_parameter_pack.hpp
#else //FUNCTIONAL_LITE_PARAMETER_PACK_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_PARAMETER_PACK_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_PARAMETER_PACK_HPP_GUARD
#define FUNCTIONAL_LITE_PARAMETER_PACK_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_allocator.hpp"
#include "functional_lite_cast.hpp"
#include "functional_lite_stddef.hpp"

template<class _Function> void Q_ppack_expand(_Function&& _Func) {

}

template<class _Function, class _Arg, class... _Rest> void Q_ppack_expand(_Function&& _Func, _Arg& _Argument, _Rest&... _RestArgs) {
	_Func(_Argument);
	Q_ppack_expand(_Func, _RestArgs...);
}

typedef struct CParameterPackExpander {
	functional_size_t m_iArgsSize, m_iCurrentArg;
	void** m_a_pTotalArgs;

	template<class _ResultType> _ResultType& at(_In_ functional_size_t _Where);
} CParameterPackExpander, Q_pp_list;

template<class _ResultType> _ResultType& CParameterPackExpander::at(_In_ functional_size_t _Where) {
	Q_ASSERT(_Where < m_iArgsSize);
	_ResultType& _Which = (_ResultType&)(*static_cast<_ResultType*>(m_a_pTotalArgs[_Where]));

	return _Which;
}

#define Q_ppack_start(_Klass, _Pack) do {																			\
		_Klass->m_iArgsSize = sizeof...(_Pack);																		\
		_Klass->m_iCurrentArg = 0;																					\
		_Klass->m_a_pTotalArgs	= indirect_cast<void**>(Q_malloc(_Klass->m_iArgsSize * sizeof(void*)));			\
																													\
		Q_ppack_expand([&](auto& arg) {																						\
			_Klass->m_a_pTotalArgs[_Klass->m_iCurrentArg] = indirect_cast<void*>(&arg);							\
			++_Klass->m_iCurrentArg;																				\
		}, _Pack...);																								\
		_Klass->m_iCurrentArg = 0;																					\
	} while (0)

#define Q_ppack_arg(_Klass, _Type) *indirect_cast<_Type*>(_Klass->m_a_pTotalArgs[_Klass->m_iCurrentArg++])

#define Q_ppack_end(_Klass) do {					\
		Q_free(_Klass->m_a_pTotalArgs);		\
		Q_free(_Klass);							\
	} while (0)

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_parameter_pack.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_PARAMETER_PACK_HPP_GUARD

#undef FUNCTIONAL_LITE_PARAMETER_PACK_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_PARAMETER_PACK_HPP_RECURSE_GUARD