/**
 * Copyright - xWhitey, 2023.
 * functional_lite_stdbool.hpp - Implementation of bool (presented as bool, boolean)
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_STDBOOL_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_stdbool.hpp
#else //FUNCTIONAL_LITE_STDBOOL_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_STDBOOL_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_STDBOOL_HPP_GUARD
#define FUNCTIONAL_LITE_STDBOOL_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef enum : unsigned char {
	Q_FALSE = 0x0,
	Q_TRUE = 0x1
} Q_bool, Q_boolean;

#define Q_false Q_FALSE
#define Q_true Q_TRUE

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_stdbool.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_STDBOOL_HPP_GUARD

#undef FUNCTIONAL_LITE_STDBOOL_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_STDBOOL_HPP_RECURSE_GUARD