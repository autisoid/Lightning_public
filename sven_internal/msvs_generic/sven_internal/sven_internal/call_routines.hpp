/**
 * Copyright - xWhitey, 2024.
 * call_routines.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CALL_ROUTINES_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in call_routines.hpp
#else //CALL_ROUTINES_HPP_RECURSE_GUARD

#define CALL_ROUTINES_HPP_RECURSE_GUARD

#ifndef CALL_ROUTINES_HPP_GUARD
#define CALL_ROUTINES_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "callstack.hpp"

typedef struct CCallRoutines {
	template<class _ReturnType, class _Ty>
	_ReturnType CallStub(_ReturnType(*_Fun)(), _Ty&& _Arg) {
		g_Callstack.Push(_Arg);
		return _Fun();
	}

	template<class _ReturnType, class _Ty, class... _Ts>
	_ReturnType CallStub(_ReturnType(*_Fun)(), _Ty&& _Arg, _Ts... _Rest) {
		g_Callstack.Push(_Arg);
		return CallStub(_Fun, _Rest...);
	}

	template<class _ReturnType, class... _Ts>
	_ReturnType CallStub(_ReturnType(*_Fun)(), _Ts... _Args) {
		return CallStub(_Fun, _Args...);
	}

	/*void* GenerateStub(void* _JumpTarget);

	static void __cdecl ExampleStub(void* _JumpTarget);
	static void __cdecl ExampleStubEnd();*/
} CCallRoutines;

using CCallRoutines = struct CCallRoutines;

#else //!__cplusplus
#error C++ compiler required to compile call_routines.hpp
#endif //__cplusplus

#endif //CALL_ROUTINES_HPP_GUARD

#undef CALL_ROUTINES_HPP_RECURSE_GUARD
#endif //CALL_ROUTINES_HPP_RECURSE_GUARD