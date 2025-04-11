/**
 * Copyright - xWhitey, 2023.
 * HwDLL.hpp - description
 */

#ifdef HWDLL_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in HwDLL.hpp
#else //HWDLL_HPP_RECURSE_GUARD

#define HWDLL_HPP_RECURSE_GUARD

#ifndef HWDLL_HPP_GUARD
#define HWDLL_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CEngineHooks {
	static void Initialize();
	static void SetupEngineCommandHook(_In_z_ const char* _CommandName, _In_ void(__cdecl* _Detour)(), _In_ void** _Original);
} CEngineHooks;

using CEngineHooks = struct CEngineHooks;

#else //!__cplusplus
#error C++ compiler required to compile HwDLL.hpp
#endif //__cplusplus

#endif //HWDLL_HPP_GUARD

#undef HWDLL_HPP_RECURSE_GUARD
#endif //HWDLL_HPP_RECURSE_GUARD