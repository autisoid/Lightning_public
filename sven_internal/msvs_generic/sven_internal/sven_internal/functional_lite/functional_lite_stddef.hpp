/**
 * Copyright - xWhitey, 2023.
 * functional_lite_stddef.hpp - Definitions of various things from stddef.h
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_STDDEF_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_stddef.hpp
#else //FUNCTIONAL_LITE_STDDEF_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_STDDEF_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_STDDEF_HPP_GUARD
#define FUNCTIONAL_LITE_STDDEF_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_type_traits.hpp"
#include "functional_lite_static_assert.hpp"

//We're getting rid outta int on 32 bit platforms and long long on 64 bit platforms, so typedef the right underlying type to suit our needs. - xWhitey
typedef functional::conditional_t<sizeof(void*) == 4, int, long long> functional_signed_size_t;
typedef functional::conditional_t<sizeof(void*) == 4, unsigned int, unsigned long long> functional_size_t;
typedef functional_size_t functional_uintptr_t;
typedef functional_uintptr_t functional_ptrdiff_t;

typedef functional_size_t Q_size_t;
typedef functional_uintptr_t Q_uintptr_t;
typedef functional_ptrdiff_t Q_ptrdiff_t;

Q_static_assert((sizeof(functional_size_t) == 4 || sizeof(functional_size_t) == 8) && "functional_lite is not intended to be used with non-desktop platforms.");

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_stddef.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_STDDEF_HPP_GUARD

#undef FUNCTIONAL_LITE_STDDEF_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_STDDEF_HPP_RECURSE_GUARD