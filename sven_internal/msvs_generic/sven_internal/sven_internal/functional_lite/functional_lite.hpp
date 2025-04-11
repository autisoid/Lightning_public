/**
 * Copyright - xWhitey, 2023.
 * functional_lite.hpp - Here we rather include all the header files we provide.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite.hpp
#else //FUNCTIONAL_LITE_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_HPP_GUARD
#define FUNCTIONAL_LITE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_allocator.hpp"
#include "functional_lite_arithmetic.hpp"
#include "functional_lite_assert.hpp"
#include "functional_lite_cast.hpp"
#include "functional_lite_math.hpp"
#include "functional_lite_memory.hpp"
#include "functional_lite_parameter_pack.hpp"
#include "functional_lite_preprocessor.hpp"
#include "functional_lite_rng.hpp"
#include "functional_lite_static_assert.hpp"
#include "functional_lite_stdbool.hpp"
#include "functional_lite_stddef.hpp"
#include "functional_lite_stdnull.hpp"
//#include "functional_lite_string.hpp"
#include "functional_lite_type_traits.hpp"
#include "functional_lite_unique_ptr.hpp"
#include "functional_lite_vector.hpp"

#else //!__cplusplus
#error C++ compiler required to compile functional_lite.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_HPP_GUARD

#undef FUNCTIONAL_LITE_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_HPP_RECURSE_GUARD