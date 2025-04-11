/**
 * Copyright - xWhitey, 2024.
 * preprocessor.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef PREPROCESSOR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in preprocessor.hpp
#else //PREPROCESSOR_HPP_RECURSE_GUARD

#define PREPROCESSOR_HPP_RECURSE_GUARD

#ifndef PREPROCESSOR_HPP_GUARD
#define PREPROCESSOR_HPP_GUARD
#pragma once

#ifdef __cplusplus

//https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
#if defined(_MSC_VER) && defined(_MSVC_TRADITIONAL) && !defined(__clang__)

#define GET_ARG_COUNT(...)  INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))

#define INTERNAL_ARGS_AUGMENTER(...) _Unused, __VA_ARGS__
#define INTERNAL_EXPAND(_Val) _Val
#define INTERNAL_EXPAND_ARGS_PRIVATE(...) INTERNAL_EXPAND(INTERNAL_GET_ARG_COUNT_PRIVATE(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define INTERNAL_GET_ARG_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _Count, ...) _Count

#elif defined(__clang__)

#define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _Count, ...) _Count

#else //!defined(__clang__)

#define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _Count, ...) _Count

#endif //!defined(__clang__) && defined(_MSC_VER)

#define _INTERNAL_COMMA_0
#define _INTERNAL_COMMA_1 ,
#define _INTERNAL_COMMA_2 ,
#define _INTERNAL_COMMA_3 ,
#define _INTERNAL_COMMA_4 ,
#define _INTERNAL_COMMA_5 ,
#define _INTERNAL_COMMA_6 ,
#define _INTERNAL_COMMA_7 ,
#define _INTERNAL_COMMA_8 ,
#define _INTERNAL_COMMA_9 ,
#define _INTERNAL_COMMA_10 ,
#define _INTERNAL_COMMA_11 ,
#define _INTERNAL_COMMA_12 ,
#define _INTERNAL_COMMA_13 ,
#define _INTERNAL_COMMA_14 ,
#define _INTERNAL_COMMA_15 ,
#define _INTERNAL_COMMA_16 ,
#define _INTERNAL_COMMA_17 ,
#define _INTERNAL_COMMA_18 ,
#define _INTERNAL_COMMA_19 ,
#define _INTERNAL_COMMA_20 ,
#define _INTERNAL_COMMA_21 ,
#define _INTERNAL_COMMA_22 ,
#define _INTERNAL_COMMA_23 ,
#define _INTERNAL_COMMA_24 ,
#define _INTERNAL_COMMA_25 ,
#define _INTERNAL_COMMA_26 ,
#define _INTERNAL_COMMA_27 ,
#define _INTERNAL_COMMA_28 ,
#define _INTERNAL_COMMA_29 ,
#define _INTERNAL_COMMA_30 ,
#define _INTERNAL_COMMA_31 ,
#define _INTERNAL_COMMA_32 ,
#define _INTERNAL_COMMA_33 ,
#define _INTERNAL_COMMA_34 ,
#define _INTERNAL_COMMA_35 ,
#define _INTERNAL_COMMA_36 ,
#define _INTERNAL_COMMA_37 ,
#define _INTERNAL_COMMA_38 ,
#define _INTERNAL_COMMA_39 ,
#define _INTERNAL_COMMA_40 ,
#define _INTERNAL_COMMA_41 ,
#define _INTERNAL_COMMA_42 ,
#define _INTERNAL_COMMA_43 ,
#define _INTERNAL_COMMA_44 ,
#define _INTERNAL_COMMA_45 ,
#define _INTERNAL_COMMA_46 ,
#define _INTERNAL_COMMA_47 ,
#define _INTERNAL_COMMA_48 ,
#define _INTERNAL_COMMA_49 ,
#define _INTERNAL_COMMA_50 ,
#define _INTERNAL_COMMA_51 ,
#define _INTERNAL_COMMA_52 ,
#define _INTERNAL_COMMA_53 ,
#define _INTERNAL_COMMA_54 ,
#define _INTERNAL_COMMA_55 ,
#define _INTERNAL_COMMA_56 ,
#define _INTERNAL_COMMA_57 ,
#define _INTERNAL_COMMA_58 ,
#define _INTERNAL_COMMA_59 ,
#define _INTERNAL_COMMA_60 ,
#define _INTERNAL_COMMA_61 ,
#define _INTERNAL_COMMA_62 ,
#define _INTERNAL_COMMA_63 ,
#define _INTERNAL_COMMA_64 ,
#define _INTERNAL_COMMA_65 ,
#define _INTERNAL_COMMA_66 ,
#define _INTERNAL_COMMA_67 ,
#define _INTERNAL_COMMA_68 ,
#define _INTERNAL_COMMA_69 ,

#define _COMMA_DETAIL(_Count) _INTERNAL_COMMA ## _ ## _Count
#define _COMMA_EXPAND(...) _COMMA_DETAIL(__VA_ARGS__)
#define _COMMA(...) _COMMA_EXPAND(INTERNAL_EXPAND(GET_ARG_COUNT(__VA_ARGS__)))

#else //!__cplusplus
#error C++ compiler required to compile preprocessor.hpp
#endif //__cplusplus

#endif //PREPROCESSOR_HPP_GUARD

#undef PREPROCESSOR_HPP_RECURSE_GUARD
#endif //PREPROCESSOR_HPP_RECURSE_GUARD