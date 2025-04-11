/**
 * Copyright - xWhitey, 2023.
 * CFrameSkipperModule.hpp - skippin' frames yeeee
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CFRAMESKIPPERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CFrameSkipperModule.hpp
#else //CFRAMESKIPPERMODULE_HPP_RECURSE_GUARD

#define CFRAMESKIPPERMODULE_HPP_RECURSE_GUARD

#ifndef CFRAMESKIPPERMODULE_HPP_GUARD
#define CFRAMESKIPPERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CFrameSkipperModule : CModule {
	CFrameSkipperModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;
} CFrameSkipperModule;

using CFrameSkipperModule = struct CFrameSkipperModule;

#else //!__cplusplus
#error C++ compiler required to compile CFrameSkipperModule.hpp
#endif //__cplusplus

#endif //CFRAMESKIPPERMODULE_HPP_GUARD

#undef CFRAMESKIPPERMODULE_HPP_RECURSE_GUARD
#endif //CFRAMESKIPPERMODULE_HPP_RECURSE_GUARD