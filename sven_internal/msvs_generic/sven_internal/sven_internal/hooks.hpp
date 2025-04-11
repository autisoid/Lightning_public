/**
 * Copyright - xWhitey, 2024.
 * hooks.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef HOOKS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in hooks.hpp
#else //HOOKS_HPP_RECURSE_GUARD

#define HOOKS_HPP_RECURSE_GUARD

#ifndef HOOKS_HPP_GUARD
#define HOOKS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif //_MSC_VER

typedef struct CIntelligentHookedFunction {
	CIntelligentHookedFunction(void* _Target, void* _Detour);

	void* m_pTarget;
	void* m_pDetour;
	bool m_bIsEnabled;
} CIntelligentHookedFunction;

#ifdef _MSC_VER
#pragma pack(pop)
#endif //_MSC_VER

typedef struct CHooks {
	static void Initialise();
	static void CreateHook(void* _Target, void* _Detour);
	static void EnableHook(void* _Target);
	static void DisableHook(void* _Target);
	static void AddPageGuardProtection(void* _Target);
	static void RemovePageGuardProtection(void* _Target);
	static CIntelligentHookedFunction* FindHookByTarget(void* _Target);
	static void ProcessCall(void* _Target);
	static void ProtectPortion(void* _Address, size_t _Size, DWORD _NewProtect);

	static LONG CALLBACK UnhandledExceptionHandler(PEXCEPTION_POINTERS _ExceptionPointers);

	static CVector<CIntelligentHookedFunction*>* ms_pHooks;
} CHooks;

using CHooks = struct CHooks;

#else //!__cplusplus
#error C++ compiler required to compile hooks.hpp
#endif //__cplusplus

#endif //HOOKS_HPP_GUARD

#undef HOOKS_HPP_RECURSE_GUARD
#endif //HOOKS_HPP_RECURSE_GUARD