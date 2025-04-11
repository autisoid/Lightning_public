/**
 * Copyright - xWhitey, 2024.
 * CForwardtrackModule.hpp - predictin' players' movement (goin' back to the future)
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CFORWARDTRACKMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CForwardtrackModule.hpp
#else //CFORWARDTRACKMODULE_HPP_RECURSE_GUARD

#define CFORWARDTRACKMODULE_HPP_RECURSE_GUARD

#ifndef CFORWARDTRACKMODULE_HPP_GUARD
#define CFORWARDTRACKMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CForwardtrackModule : CModule {
	CForwardtrackModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	bool ForwardtrackEntity(_In_ cl_entity_t* _Entity, _In_ float _Frametime, _Inout_ Vector& _SimulationOrigin);

	float m_flFrametime;
} CForwardtrackModule;

using CForwardtrackModule = struct CForwardtrackModule;

extern cl_entity_t g_aForwardtrackEntities[];

#else //!__cplusplus
#error C++ compiler required to compile CForwardtrackModule.hpp
#endif //__cplusplus

#endif //CFORWARDTRACKMODULE_HPP_GUARD

#undef CFORWARDTRACKMODULE_HPP_RECURSE_GUARD
#endif //CFORWARDTRACKMODULE_HPP_RECURSE_GUARD