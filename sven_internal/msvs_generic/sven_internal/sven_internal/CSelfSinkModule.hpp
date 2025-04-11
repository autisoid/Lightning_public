/**
 * Copyright - xWhitey, 2024.
 * CSelfSinkModule.hpp - Self-sink?
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CSELFSINKMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CSelfSinkModule.hpp
#else //CSELFSINKMODULE_HPP_RECURSE_GUARD

#define CSELFSINKMODULE_HPP_RECURSE_GUARD

#ifndef CSELFSINKMODULE_HPP_GUARD
#define CSELFSINKMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CSelfSinkModule : CModule {
	CSelfSinkModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CIntegerValue* m_pDepth;
} CSelfSinkModule;

using CSelfSinkModule = struct CSelfSinkModule;

#else //!__cplusplus
#error C++ compiler required to compile CSelfSinkModule.hpp
#endif //__cplusplus

#endif //CSELFSINKMODULE_HPP_GUARD

#undef CSELFSINKMODULE_HPP_RECURSE_GUARD
#endif //CSELFSINKMODULE_HPP_RECURSE_GUARD