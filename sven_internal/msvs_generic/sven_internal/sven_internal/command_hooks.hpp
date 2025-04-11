/**
 * Copyright - xWhitey, 2024.
 * command_hooks.hpp - hookin' engine commands here haha
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef COMMAND_HOOKS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in command_hooks.hpp
#else //COMMAND_HOOKS_HPP_RECURSE_GUARD

#define COMMAND_HOOKS_HPP_RECURSE_GUARD

#ifndef COMMAND_HOOKS_HPP_GUARD
#define COMMAND_HOOKS_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CCommandHooks {
	static void Initialise();
} CCommandHooks;

using CCommandHooks = struct CCommandHooks;

#else //!__cplusplus
#error C++ compiler required to compile command_hooks.hpp
#endif //__cplusplus

#endif //COMMAND_HOOKS_HPP_GUARD

#undef COMMAND_HOOKS_HPP_RECURSE_GUARD
#endif //COMMAND_HOOKS_HPP_RECURSE_GUARD