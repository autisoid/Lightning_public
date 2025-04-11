/**
 * Copyright - xWhitey, 2023.
 * functional_lite_static_assert.hpp - description
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_STATIC_ASSERT_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_static_assert.hpp
#else //FUNCTIONAL_LITE_STATIC_ASSERT_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_STATIC_ASSERT_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_STATIC_ASSERT_HPP_GUARD
#define FUNCTIONAL_LITE_STATIC_ASSERT_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_preprocessor.hpp"

//Since we need a good-looking and eye-catching name, we name this struct as in Boost.
template<bool _Unused> struct STATIC_ASSERTION_FAILURE;

template<> struct STATIC_ASSERTION_FAILURE<true> { enum { _Value = 1 }; };

#define Q_static_assert(_Condition) enum { Q_rand_name() = sizeof(STATIC_ASSERTION_FAILURE<((_Condition) != 0)>) }
#define Q_STATIC_ASSERT(_Condition) Q_static_assert(_Condition)

//This way requires us to have RTTI.
//template<functional_unsigned_size_t _Unused> struct CStaticAssertTester {};
//#define Q_static_assert(_Condition, ...) typedef CStaticAssertTester<sizeof(STATIC_ASSERTION_FAILURE<((_Condition) != 0)>)> Q_rand_name();

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_static_assert.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_STATIC_ASSERT_HPP_GUARD

#undef FUNCTIONAL_LITE_STATIC_ASSERT_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_STATIC_ASSERT_HPP_RECURSE_GUARD