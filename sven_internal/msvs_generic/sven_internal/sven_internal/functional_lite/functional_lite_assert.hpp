/**
 * Copyright - xWhitey, 2023.
 * functional_lite_assert.hpp - A simple implementation of "assert"
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_ASSERT_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_assert.hpp
#else //FUNCTIONAL_LITE_ASSERT_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_ASSERT_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_ASSERT_HPP_GUARD
#define FUNCTIONAL_LITE_ASSERT_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_preprocessor.hpp"

//Bypassing compiler checks when dividing by zero: 1337 / 0 will (or may) produce a warning/error for some compilers (MSVC, G++, Clang do such).
static float gs_flZeroTest;
//Once an assertion has failed, please consider using Q_GetAssertionFailureReason 
//or just directly read reason from gs_pszAssertionFailureReason using a debugger (when using Q_GetAssertionFailureReason, call it in a 'critical section' or so-called crash handler).
static const char* gs_pszAssertionFailureReason;

const char* Q_GetAssertionFailureReason() {
	return gs_pszAssertionFailureReason;
}

void Q_SetAssertionFailureReason(_In_z_ const char* _Reason) {
	gs_pszAssertionFailureReason = _Reason;
}

#ifndef __LINE__
#warning __LINE__ macro unsupported
#define __LINE__ -1
#endif //__LINE__
#ifndef __FILE__
#warning __FILE__ macro unsupported
#define __FILE__ "Compiler unsupported"
#endif //__FILE__
#ifndef FUNCTIONAL_NO_ASSERTS
//Checking passed expression for truth, if false - setting failure reason, crashing application using access violation and divide by zero exceptions.
#define Q_ASSERT(_Expr) do { (!!(_Expr)) || (gs_flZeroTest = 0.f, gs_pszAssertionFailureReason = "Assertion failed at " Q_STRINGIZE(__LINE__) " line in file " __FILE__ " with code " Q_STRINGIZE(_Expr) ".", (*((unsigned int*)0) = 0xCAFE)); /*, (*((unsigned int*)0) = 1.f / gs_flZeroTest));*/ } while (0)
#define Q_assert(_Expr) Q_ASSERT(_Expr)
#ifdef FUNCTIONAL_DEBUG_PARANOID
#define Q_SLOWASSERT(_Expr) Q_ASSERT(_Expr)
#else //!FUNCTIONAL_DEBUG_PARANOID
#define Q_SLOWASSERT(_Expr)
#endif //FUNCTIONAL_DEBUG_PARANOID
#else //FUNCTIONAL_NO_ASSERTS
#define Q_ASSERT(_Expr)
#define Q_SLOWASSERT(_Expr)
#endif //!FUNCTIONAL_NO_ASSERTS

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_assert.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_ASSERT_HPP_GUARD

#undef FUNCTIONAL_LITE_ASSERT_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_ASSERT_HPP_RECURSE_GUARD