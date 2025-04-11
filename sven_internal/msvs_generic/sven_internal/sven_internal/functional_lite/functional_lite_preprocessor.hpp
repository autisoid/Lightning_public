/**
 * Copyright - xWhitey, 2023.
 * functional_lite_preprocessor.hpp - Implementation of various function-like macros.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_PREPROCESSOR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_preprocessor.hpp
#else //FUNCTIONAL_LITE_PREPROCESSOR_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_PREPROCESSOR_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_PREPROCESSOR_HPP_GUARD
#define FUNCTIONAL_LITE_PREPROCESSOR_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define Q_offsetof(_Klass, _Field) ((functional_size_t)((int)&(((_Klass*)16)->_Field) - 16))
#define Q_offset_of(_Klass, _Field) Q_offsetof(_Klass, _Field)

#define Q_max(_Which, _To) ((_Which < _To) ? _To : _Which)
#define Q_pp_abs(_Which) (_Which > 0 ? _Which : -_Which)

#define Q_join_internal_2(_Expr, _Expr2) _Expr##_Expr2
#define Q_join_internal(_Expr, _Expr2) Q_join_internal_2(_Expr, _Expr2)
#define Q_join(_Expr, _Expr2) Q_join_internal(_Expr, _Expr2)

#define Q_RandomNumber_Stage1(_Counter) (_Counter * ((int) (_Counter / 2)) + __COUNTER__)
#define Q_RandomNumber_Stage2(_Counter) (Q_RandomNumber_Stage1(_Counter) + Q_RandomNumber_Stage1((__COUNTER__ % (_Counter + 1)) + 1))
#define Q_RandomNumber_Stage3(_Counter) (_Counter % 2 == 0 ? Q_RandomNumber_Stage2(_Counter + ((int) (__COUNTER__ / 2))) : Q_RandomNumber_Stage1(_Counter + ((int) (__COUNTER__ * 2))))
#define Q_RandomNumber_Stage4(_Counter) (__LINE__ % 2 == 0 ? Q_RandomNumber_Stage3(_Counter + __LINE__) : Q_RandomNumber_Stage2(_Counter + __LINE__ / 3))
#define Q_RandomNumber_Stage5(_Counter) (Q_RandomNumber_Stage4(_Counter) * RandomNumber_Stage2(__LINE__))
#define Q_RandomNumber_Stage6(_Counter) (Q_RandomNumber_Stage5(_Counter / 2) * RandomNumber_Stage4(_Counter / 3))
#define Q_RandomNumber_Stage7(_Counter) (Q_RandomNumber_Stage6((_Counter)) + Q_RandomNumber_Stage3(__LINE__ / 2))
#define Q_RandomNumber(_Min, _Max) (Q_pp_abs((Q_RandomNumber_Stage7(__COUNTER__ + _Min + _Max) % (_Max + 1 - _Min) + _Min)))

#define Q_RandomNameInternal(_Counter) Q_join(__DummyName__, _Counter)

#define Q_rand_name() Q_RandomNameInternal(Q_join(__LINE__, Q_join(__COUNTER__, Q_join(__COUNTER__, __COUNTER__))))

#define Q_stringize_impl(_Expr) #_Expr
#define Q_stringize_expand(_Expr) Q_stringize_impl(_Expr)
#define Q_stringize(_Expr) Q_stringize_expand(_Expr)
#define Q_STRINGIZE(_Expr) Q_stringize(_Expr)

#define Q_ARRAYSIZE(_Array) (sizeof(_Array) / sizeof(_Array[0]))

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_preprocessor.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_PREPROCESSOR_HPP_GUARD

#undef FUNCTIONAL_LITE_PREPROCESSOR_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_PREPROCESSOR_HPP_RECURSE_GUARD