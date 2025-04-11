/**
 * Copyright - xWhitey, 2023.
 * CDebugModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CDEBUGMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CDebugModule.hpp
#else //CDEBUGMODULE_HPP_RECURSE_GUARD

#define CDEBUGMODULE_HPP_RECURSE_GUARD

#ifndef CDEBUGMODULE_HPP_GUARD
#define CDEBUGMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CDebugModule : CModule {
	CDebugModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;
} CDebugModule;

using CDebugModule = struct CDebugModule;

#else //!__cplusplus
#error C++ compiler required to compile CDebugModule.hpp
#endif //__cplusplus

#endif //CDEBUGMODULE_HPP_GUARD

#undef CDEBUGMODULE_HPP_RECURSE_GUARD
#endif //CDEBUGMODULE_HPP_RECURSE_GUARD