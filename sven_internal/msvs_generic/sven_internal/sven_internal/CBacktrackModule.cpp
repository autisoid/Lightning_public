/**
 * Copyright - xWhitey, 2024.
 * CBacktrackModule.cpp - CBacktrackModule impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CBacktrackModule.hpp"
#include "CFakeLatencyModule.hpp"
#include "CESPModule.hpp"
#include "CForwardtrackModule.hpp"

CBacktrackModule* g_pBacktrackModule;

//Since this was pasted from oxware, I didn't change most of the stuff in the code at all, thus there's no hungarian notation (painfully to read) - xWhitey

//https://github.com/oxiKKK/oxware/blob/main/src/cheat/features/BackTrack.cpp

#define HISTORY_MAX		64  // Must be power of 2
#define HISTORY_MASK	( HISTORY_MAX - 1 )

// ~ from 03 leak lag compensation code
#define LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ( 64.0f * 64.0f )
#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )
// Allow 4 units of error ( about 1 / 8 bbox width )
#define LAG_COMPENSATION_ERROR_EPS_SQR ( 4.0f * 4.0f )
// Only keep 1 second of data
#define LAG_COMPENSATION_DATA_TIME	1.0f

#define STUDIO_NONE				0
#define STUDIO_RENDER			1
#define STUDIO_EVENTS			2

CBacktrackModule::CBacktrackModule() : CModule("Backtrack", "Combat") {
	g_pBacktrackModule = this;

	DefineConCmd("sc_lightning_backtrack", CBacktrackModule);
}

void CBacktrackModule::OnEnable() {
	CModule::OnEnable();

	if (!m_pExInterp)
		m_pExInterp = g_pEngfuncs->pfnGetCvarPointer("ex_interp");

	if (!m_pSvUnlagPush)
		m_pSvUnlagPush = g_pEngfuncs->pfnGetCvarPointer("sv_unlagpush");

	if (!m_pClUpdateRate)
		m_pClUpdateRate = g_pEngfuncs->pfnGetCvarPointer("cl_updaterate");

	if (!m_pSvMaxUnlag)
		m_pSvMaxUnlag = g_pEngfuncs->pfnGetCvarPointer("sv_maxunlag");

	if (!m_pFakeLatencyModule)
		m_pFakeLatencyModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CFakeLatencyModule>();

	if (!m_pFakeLatencyModule) {
		CCheat::GetCheat()->m_pConsole->Printf("[Lightning] [Backtrack] Something went horribly wrong! We've failed to retrieve FakeLatency module pointer.\n");

		this->Toggle();

		return;
	}

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CBacktrackModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void VectorMA(const Vector& vector, float scale, const Vector& vector1, Vector& vector_out) {
	vector_out = vector + scale * vector1;
}

bool CBacktrackModule::FindInterpolationUpdates(_In_ cl_entity_t* _Entity, _In_ float _TargetTime, _Out_ position_history_t** _Newer, _Out_ position_history_t** _Older) {
	int		i, i0, i1, imod;
	float	at;
	bool	extrapolate;

	imod = _Entity->current_position;
	i0 = (imod - 0) & HISTORY_MASK; // Position history for this ph record
	i1 = (imod - 1) & HISTORY_MASK; // Position history for last update ph record

	extrapolate = true;

	//
	// accumulate through saved history positions and see if we're ahead
	// of the animation time for current animation time record, if yes, then
	// we interpolate. Otherwise we extrapolate.
	//

	if (_TargetTime <= _Entity->ph[i0].animtime)
	{
		for (i = 1; i < HISTORY_MAX - 1; i++)
		{
			// Get the movement time for this particular history record
			at = _Entity->ph[(imod - i) & HISTORY_MASK].animtime;
			if (at == 0.0f)
				break;

			// see if we're not ahead of our time
			if (_TargetTime > at)
			{
				i0 = ((imod - i) + 1) & HISTORY_MASK;
				i1 = ((imod - i) + 0) & HISTORY_MASK;
				extrapolate = false;
				break;
			}
		}
	}

	if (_Newer)
		*_Newer = &_Entity->ph[i0];
	if (_Older)
		*_Older = &_Entity->ph[i1];

	return extrapolate;
}

float CBacktrackModule::CalcTrueLatency(_In_ float _UpdateRate, _In_ float _MaxUnlag, _In_ float _Frametime, _In_ float _ClientLatency, _In_ float _UpdateIntervalInMS) {
	float l = min(_ClientLatency, 1.5f);

	l -= _UpdateIntervalInMS;
	l -= _Frametime;

	float correction = min(LAG_COMPENSATION_DATA_TIME, l);

	if (_MaxUnlag) {
		if (_MaxUnlag < 0.0f)
			_MaxUnlag = 0.0f;

		correction = min(correction, _MaxUnlag);
	}

	return correction;
}

bool CBacktrackModule::BacktrackEntity(_In_ cl_entity_t* _Entity, _In_ float _InterpolationMsec, _In_ float _Frametime, _Inout_ Vector& _SimulationOrigin, _Inout_ Vector& _SimulationAngles) {
	float realtime = static_cast<float>(*g_pdblClientTime);

	float update_interval_ms = 1.0f / max(m_pClUpdateRate->value, 10.0f);

	double dblLatency = 0.0;
	if (g_pdblLatency) {
		dblLatency += *g_pdblLatency;
	}
	if (m_pFakeLatencyModule && m_pFakeLatencyModule->m_bState && !m_pFakeLatencyModule->m_bHasFlushed) {
		dblLatency += m_pFakeLatencyModule->m_dblDesiredFakeLatency;
	}

	float true_client_latency = CalcTrueLatency(m_pClUpdateRate->value, m_pSvMaxUnlag->value, _Frametime, static_cast<float>(dblLatency), update_interval_ms);
	float target_time = realtime - true_client_latency;
	float client_interp_amount = CalcInterpolationAmount(m_pExInterp->value, 0.1f, _InterpolationMsec);
	target_time -= client_interp_amount;
	target_time += m_pSvUnlagPush->value;
	target_time = min(realtime, target_time);

	position_history_t* newer, *older;
	FindInterpolationUpdates(_Entity, target_time, &newer, &older);

	if (!newer || !older) {
		return false;
	}

	if (newer->animtime > 0.f) {
		Vector pos, ang;
		Vector delta;
		float frac;

		delta = newer->origin - older->origin;

		if (newer->animtime != older->animtime) {
			frac = (target_time - older->animtime) / (newer->animtime - older->animtime);
		} else {
			frac = 1.0f;
		}

		frac = CUtils::Clamp(0.0f, 1.2f, frac);

		VectorMA(older->origin, frac, delta, pos);
		VectorMA(older->angles, frac, delta, ang);

		_SimulationOrigin = pos;
		_SimulationAngles = ang;
	} else {
		_SimulationOrigin = older->origin;
		_SimulationAngles = older->angles;
	}

	return true;
}

void CBacktrackModule::ConstructBacktrackEntity(_In_ cl_entity_t& _Entity, const Vector& _InterpolatedOrigin, const Vector& _InterpolatedAngles) {
	_Entity.origin = _InterpolatedOrigin;
	_Entity.angles = _InterpolatedAngles;

	_Entity.curstate.weaponmodel = 0;

	_Entity.curstate.renderamt = 0;
	_Entity.curstate.rendercolor = {};
	_Entity.curstate.renderfx = kRenderFxNone;
	_Entity.curstate.rendermode = kRenderNormal;
}

void DrawStringWithShadow(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text, _In_ float _FontSize) {
	for (int idx = 1; idx <= 1; idx++) {
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y + idx), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y - idx), ImColor(0, 0, 0), _Text);
	}
	ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y), _Color, _Text);
}

void CBacktrackModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);
		m_pCmd = e->m_pCmd;
		m_flFrametime = e->m_flFrametime;
	}
}