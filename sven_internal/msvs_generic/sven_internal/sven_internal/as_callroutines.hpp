/**
 * Copyright - xWhitey, 2024.
 * as_callroutines.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef AS_CALLROUTINES_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in as_callroutines.hpp
#else //AS_CALLROUTINES_HPP_RECURSE_GUARD

#define AS_CALLROUTINES_HPP_RECURSE_GUARD

#ifndef AS_CALLROUTINES_HPP_GUARD
#define AS_CALLROUTINES_HPP_GUARD
#pragma once

#ifdef __cplusplus

//#include "callstack.hpp" //call_routines.hpp includes it so not needed
#include "call_routines.hpp"

typedef struct CASCallRoutines {
	static void CallStub();
} CASCallRoutines;

using CASCallRoutines = struct CASCallRoutines;

#else //!__cplusplus
#error C++ compiler required to compile as_callroutines.hpp
#endif //__cplusplus

#endif //AS_CALLROUTINES_HPP_GUARD

#undef AS_CALLROUTINES_HPP_RECURSE_GUARD
#endif //AS_CALLROUTINES_HPP_RECURSE_GUARD