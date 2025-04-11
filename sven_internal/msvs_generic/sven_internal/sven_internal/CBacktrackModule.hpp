/**
 * Copyright - xWhitey, 2024.
 * CBacktrackModule.hpp - backtrackin'
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CBACKTRACKMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CBacktrackModule.hpp
#else //CBACKTRACKMODULE_HPP_RECURSE_GUARD

#define CBACKTRACKMODULE_HPP_RECURSE_GUARD

#ifndef CBACKTRACKMODULE_HPP_GUARD
#define CBACKTRACKMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CBacktrackModule : CModule {
	CBacktrackModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	bool FindInterpolationUpdates(_In_ cl_entity_t* _Entity, _In_ float _TargetTime, _Out_ position_history_t** _Newer, _Out_ position_history_t** _Older);
	float CalcTrueLatency(_In_ float _UpdateRate, _In_ float _MaxUnlag, _In_ float _Frametime, _In_ float _ClientLatency, _In_ float _UpdateIntervalInMS);
	bool BacktrackEntity(_In_ cl_entity_t* _Entity, _In_ float _InterpolationMsec, _In_ float _Frametime, _Inout_ Vector& _SimulationOrigin, _Inout_ Vector& _SimulationAngles);
	inline float CalcInterpolationAmount(_In_ float _ExInterp, _In_ float _UpdateIntervalInMS, _In_ float _InterpolationMsec) {
		return min((_InterpolationMsec == -1) ? _ExInterp : (_InterpolationMsec / 1000.0f), _UpdateIntervalInMS);
	}
	void ConstructBacktrackEntity(_In_ cl_entity_t& _Entity, const Vector& _InterpolatedOrigin, const Vector& _InterpolatedAngles);

	struct CFakeLatencyModule* m_pFakeLatencyModule;
	struct CESPModule* m_pESPModule;

	cvar_t* m_pExInterp;
	cvar_t* m_pSvUnlagPush;
	cvar_t* m_pClUpdateRate;
	cvar_t* m_pSvMaxUnlag;
	usercmd_s* m_pCmd;
	float m_flFrametime;
} CBacktrackModule;

using CBacktrackModule = struct CBacktrackModule;

cl_entity_t g_aBacktrackEntities[4096];

#else //!__cplusplus
#error C++ compiler required to compile CBacktrackModule.hpp
#endif //__cplusplus

#endif //CBACKTRACKMODULE_HPP_GUARD

#undef CBACKTRACKMODULE_HPP_RECURSE_GUARD
#endif //CBACKTRACKMODULE_HPP_RECURSE_GUARD