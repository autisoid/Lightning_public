/**
 * Copyright - xWhitey, 2023.
 * functional_lite_math.hpp - This should be implementation of math.h, but I'm lazy.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_MATH_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_math.hpp
#else //FUNCTIONAL_LITE_MATH_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_MATH_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_MATH_HPP_GUARD
#define FUNCTIONAL_LITE_MATH_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_stddef.hpp"

int Q_pow(_In_ int _Base, _In_ int _Power) {
	if (_Power == 0) return 1;
	else if ((_Power % 2) == 0)
		return Q_pow(_Base, _Power / 2) * Q_pow(_Base, _Power / 2);
	else
		return _Base * Q_pow(_Base, _Power / 2) * Q_pow(_Base, _Power / 2);
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_math.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_MATH_HPP_GUARD

#undef FUNCTIONAL_LITE_MATH_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_MATH_HPP_RECURSE_GUARD