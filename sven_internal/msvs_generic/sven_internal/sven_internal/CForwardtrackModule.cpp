/**
 * Copyright - xWhitey, 2024.
 * CForwardtrackModule.cpp - CForwardtrackModule impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CForwardtrackModule.hpp"
#include "HLStrafe/hlstrafe.hpp"

//Well, this module comes from 2022 year as per I did it for Xash3D engine.
//Anyways, here it is

CForwardtrackModule* g_pForwardtrackModule;
cl_entity_t g_aForwardtrackEntities[4096];

CForwardtrackModule::CForwardtrackModule() : CModule("Forwardtrack", "Combat") {
	g_pForwardtrackModule = this;

	DefineConCmd("sc_lightning_forwardtrack", CForwardtrackModule);
}

void CForwardtrackModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CForwardtrackModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

bool CForwardtrackModule::ForwardtrackEntity(_In_ cl_entity_t* _Entity, _In_ float _Frametime, _Inout_ Vector& _SimulationOrigin) {
	if (!g_pPlayerMove) return false;

	HLStrafe::PlayerData player = HLStrafe::PlayerData();
	player.Origin[0] = _Entity->curstate.origin[0];
	player.Origin[1] = _Entity->curstate.origin[1];
	player.Origin[2] = _Entity->curstate.origin[2];
	player.Viewangles[0] = _Entity->curstate.angles[0];
	player.Viewangles[1] = _Entity->curstate.angles[1];
	player.Viewangles[2] = _Entity->curstate.angles[2];
	player.Velocity[0] = _Entity->curstate.velocity[0];
	player.Velocity[1] = _Entity->curstate.velocity[1];
	player.Velocity[2] = _Entity->curstate.velocity[2];
	player.Basevelocity[0] = _Entity->curstate.basevelocity[0];
	player.Basevelocity[1] = _Entity->curstate.basevelocity[1];
	player.Basevelocity[2] = _Entity->curstate.basevelocity[2];
	player.Ducking = _Entity->curstate.usehull != 0;
	player.InDuckAnimation = false;
	player.DuckTime = 0.0f;
	player.StaminaTime = (_Entity->curstate.fuser2 != 0);
	player.Walking = (_Entity->curstate.movetype == MOVETYPE_WALK);

	auto postype = HLStrafe::GetPositionType(player, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
	HLStrafe::MovementVars vars = HLStrafe::MovementVars();
	if (g_pMoveVars) {
		vars.Accelerate = g_pMoveVars->accelerate;
		vars.Airaccelerate = g_pMoveVars->airaccelerate;
		vars.Bounce = g_pMoveVars->bounce;
		vars.Friction = g_pMoveVars->friction;
		vars.Edgefriction = g_pMoveVars->edgefriction;
		vars.Gravity = g_pMoveVars->gravity;
		vars.EntGravity = g_pMoveVars->entgravity;
		vars.Maxvelocity = g_pMoveVars->maxvelocity;
		vars.Stepsize = g_pMoveVars->stepsize;
	} else {
		vars.Accelerate = sv_accelerate->value;
		vars.Airaccelerate = sv_airaccelerate->value;
		vars.Bounce = sv_bounce->value;
		vars.Friction = sv_friction->value;
		vars.Edgefriction = edgefriction->value;
		vars.Gravity = sv_gravity->value;
		vars.EntGravity = 1.0f;
		vars.Maxvelocity = sv_maxvelocity->value;
		vars.Stepsize = sv_stepsize->value;
	}
	vars.EntFriction = g_pPlayerMove->friction;
	vars.Maxspeed = g_pPlayerMove->maxspeed;
	vars.Frametime = _Frametime;
	postype = HLStrafe::Move(player, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);

	_SimulationOrigin = player.Origin;

	return true;
}

void CForwardtrackModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);
		m_flFrametime = e->m_flFrametime;
	}
}