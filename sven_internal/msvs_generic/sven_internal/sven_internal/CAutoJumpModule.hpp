/**
 * Copyright - xWhitey, 2023.
 * CAutoJumpModule.hpp - an autojump module huh
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CAUTOJUMPMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAutoJumpModule.hpp
#else //CAUTOJUMPMODULE_HPP_RECURSE_GUARD

#define CAUTOJUMPMODULE_HPP_RECURSE_GUARD

#ifndef CAUTOJUMPMODULE_HPP_GUARD
#define CAUTOJUMPMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAutoJumpModule : CModule {
	CAutoJumpModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*);
} CAutoJumpModule;

using CAutoJumpModule = struct CAutoJumpModule;

#else //!__cplusplus
#error C++ compiler required to compile CAutoJumpModule.hpp
#endif //__cplusplus

#endif //CAUTOJUMPMODULE_HPP_GUARD

#undef CAUTOJUMPMODULE_HPP_RECURSE_GUARD
#endif //CAUTOJUMPMODULE_HPP_RECURSE_GUARD