/**
 * Copyright - xWhitey, 2023.
 * functional_lite_msvc_fixes.hpp - Should fix some bugs of MSVC. (Currently providing only one bug-fix)
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_MSVC_FIXES_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_msvc_fixes.hpp
#else //FUNCTIONAL_LITE_MSVC_FIXES_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_MSVC_FIXES_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_MSVC_FIXES_HPP_GUARD
#define FUNCTIONAL_LITE_MSVC_FIXES_HPP_GUARD
#pragma once

#ifdef __cplusplus

//Include this header ONLY if necessary.

extern "C" int _fltused = 1;

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_msvc_fixes.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_MSVC_FIXES_HPP_GUARD

#undef FUNCTIONAL_LITE_MSVC_FIXES_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_MSVC_FIXES_HPP_RECURSE_GUARD