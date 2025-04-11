/**
 * Copyright - xWhitey, 2023.
 * CSlideWalkModule.hpp - Memewalk
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CSLIDEWALKMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CSlideWalkModule.hpp
#else //CSLIDEWALKMODULE_HPP_RECURSE_GUARD

#define CSLIDEWALKMODULE_HPP_RECURSE_GUARD

#ifndef CSLIDEWALKMODULE_HPP_GUARD
#define CSLIDEWALKMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CSlideWalkModule : CModule {
	CSlideWalkModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;
} CSlideWalkModule;

using CSlideWalkModule = struct CSlideWalkModule;

#else //!__cplusplus
#error C++ compiler required to compile CSlideWalkModule.hpp
#endif //__cplusplus

#endif //CSLIDEWALKMODULE_HPP_GUARD

#undef CSLIDEWALKMODULE_HPP_RECURSE_GUARD
#endif //CSLIDEWALKMODULE_HPP_RECURSE_GUARD