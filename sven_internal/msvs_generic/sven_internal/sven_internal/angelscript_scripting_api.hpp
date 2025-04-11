/**
 * Copyright - xWhitey, 2024.
 * angelscript_scripting_api.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef ANGELSCRIPT_SCRIPTING_API_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in angelscript_scripting_api.hpp
#else //ANGELSCRIPT_SCRIPTING_API_HPP_RECURSE_GUARD

#define ANGELSCRIPT_SCRIPTING_API_HPP_RECURSE_GUARD

#ifndef ANGELSCRIPT_SCRIPTING_API_HPP_GUARD
#define ANGELSCRIPT_SCRIPTING_API_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "script_console.hpp"

typedef struct CTheScripts {
	void Initialise();
	void RegisterStructures();
	static void __cdecl MessageCB(const void* _Message, void* _Param);
	void LoadThisScriptFile(const char* _Path, const char* _ModName);
	void Reload();

	class asIScriptEngine* m_pScriptEngine;
	CScriptConsole m_ScriptConsole;
	struct CASEngineCommands* m_pEngineCommands;
	struct CASEngineFuncs* m_pEngineFuncs;
} CTheScripts;

using CTheScripts = struct CTheScripts;

#else //!__cplusplus
#error C++ compiler required to compile angelscript_scripting_api.hpp
#endif //__cplusplus

#endif //ANGELSCRIPT_SCRIPTING_API_HPP_GUARD

#undef ANGELSCRIPT_SCRIPTING_API_HPP_RECURSE_GUARD
#endif //ANGELSCRIPT_SCRIPTING_API_HPP_RECURSE_GUARD