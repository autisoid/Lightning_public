/**
 * Copyright - xWhitey, 2024.
 * script_console.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef SCRIPT_CONSOLE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in script_console.hpp
#else //SCRIPT_CONSOLE_HPP_RECURSE_GUARD

#define SCRIPT_CONSOLE_HPP_RECURSE_GUARD

#ifndef SCRIPT_CONSOLE_HPP_GUARD
#define SCRIPT_CONSOLE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include <string>

typedef struct CScriptConsole {
	void Print(const std::string& _Message);
} CScriptConsole;

using CScriptConsole = struct CScriptConsole;

#else //!__cplusplus
#error C++ compiler required to compile script_console.hpp
#endif //__cplusplus

#endif //SCRIPT_CONSOLE_HPP_GUARD

#undef SCRIPT_CONSOLE_HPP_RECURSE_GUARD
#endif //SCRIPT_CONSOLE_HPP_RECURSE_GUARD