/**
 * Copyright - xWhitey, 2024.
 * metahook_emulation.hpp - here we're going to emulate metahook plugin behaviour
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef METAHOOK_EMULATION_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in metahook_emulation.hpp
#else //METAHOOK_EMULATION_HPP_RECURSE_GUARD

#define METAHOOK_EMULATION_HPP_RECURSE_GUARD

#ifndef METAHOOK_EMULATION_HPP_GUARD
#define METAHOOK_EMULATION_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "iface.hpp"

typedef struct CMHLightning : public IBaseInterface {
	virtual void Init(_In_ void* _MetaHookAPI, _In_ void* _Interface, _In_ void* _Save);
	virtual void Shutdown();
	virtual void LoadEngine(_In_ void* _EngineFuncs);
	virtual void LoadClient(_In_ void* _ExportFuncs);
	virtual void ExitGame(_In_ int Result);
} CMHLightning;

using CMHLightning = struct CMHLightning;

#define METAHOOK_PLUGIN_API_VERSION_V3 "METAHOOK_PLUGIN_API_VERSION003"

#else //!__cplusplus
#error C++ compiler required to compile metahook_emulation.hpp
#endif //__cplusplus

#endif //METAHOOK_EMULATION_HPP_GUARD

#undef METAHOOK_EMULATION_HPP_RECURSE_GUARD
#endif //METAHOOK_EMULATION_HPP_RECURSE_GUARD