/**
 * Copyright - xWhitey, 2023.
 * functional_lite_config.hpp - The config file for our project.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_CONFIG_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_config.hpp
#else //FUNCTIONAL_LITE_CONFIG_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_CONFIG_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_CONFIG_HPP_GUARD
#define FUNCTIONAL_LITE_CONFIG_HPP_GUARD
#pragma once

#ifdef __cplusplus

//Checking whether our headers were included after Windows.h, or before...
//Don't include our headers BEFORE Windows.h, please :)
#ifdef _INC_WINDOWS
#define FUNCTIONAL_FAKE_SAL2 "functional_empty_hdr.hpp"
#else //!_INC_WINDOWS
#define FUNCTIONAL_FAKE_SAL2 "functional_fake_sal2.hpp"
#endif //_INC_WINDOWS

//#define FUNCTIONAL_INLINE_NAMESPACE
//Define FUNCTIONAL_INLINE_NAMESPACE to make the "functional" namespace inline.
//Default: undefined

#ifdef FUNCTIONAL_INLINE_NAMESPACE
#define FUNCTIONAL_NAMESPACE_INLINE inline
#else //!FUNCTIONAL_INLINE_NAMESPACE
#define FUNCTIONAL_NAMESPACE_INLINE
#endif //FUNCTIONAL_INLINE_NAMESPACE

//Functions starting with "Q_" should NOT belong to the "functional" namespace
#define FUNCTIONAL_NAMESPACE_BEGIN FUNCTIONAL_NAMESPACE_INLINE namespace functional {
#define FUNCTIONAL_NAMESPACE_END }

// Uncomment those lines to make it automatically detect if you're building in Debug variant. (MSVC specific, we didn't test it with GCC/Clang)
/*#ifndef _DEBUG
#define FUNCTIONAL_NO_ASSERTS
#endif //_DEBUG
*/
//Define FUNCTIONAL_NO_ASSERTS to disable all assertions (not recommended when you're debugging your app)
//Default: undefined

//#define FUNCTIONAL_DEBUG_PARANOID
//Define FUNCTIONAL_DEBUG_PARANOID to make asserts literally at each line (not recommended when you're doing a fast-debug)
//Default: undefined

//You may adjust these defines to suit your needs.
#define FUNCTIONAL_HEAP_SIZE 256 * 1024 * 1024
//Default: 16 * 1024 * 1024
#define FUNCTIONAL_BLOCK_SIZE 128
//Default: 4096

//You may adjust these defines to make CTrustedRandom work more random
#define FUNCTIONAL_RANDOM_FIRST_MAGIC_WORD 0xDEAD
//Default: 0xDEAD
#define FUNCTIONAL_RANDOM_SECOND_MAGIC_WORD 0xBABE
//Default: 0xBABE

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_config.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_CONFIG_HPP_GUARD

#undef FUNCTIONAL_LITE_CONFIG_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_CONFIG_HPP_RECURSE_GUARD