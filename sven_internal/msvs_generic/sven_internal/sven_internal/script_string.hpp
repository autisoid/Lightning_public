/**
 * Copyright - xWhitey, 2024.
 * script_string.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef SCRIPT_STRING_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in script_string.hpp
#else //SCRIPT_STRING_HPP_RECURSE_GUARD

#define SCRIPT_STRING_HPP_RECURSE_GUARD

#ifndef SCRIPT_STRING_HPP_GUARD
#define SCRIPT_STRING_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifndef ANGELSCRIPT_H 
 // Avoid having to inform include path if header is already include before
#include "angelscript/angelscript.h"
#endif

void RegisterStringType(asIScriptEngine* _Engine);

#else //!__cplusplus
#error C++ compiler required to compile script_string.hpp
#endif //__cplusplus

#endif //SCRIPT_STRING_HPP_GUARD

#undef SCRIPT_STRING_HPP_RECURSE_GUARD
#endif //SCRIPT_STRING_HPP_RECURSE_GUARD