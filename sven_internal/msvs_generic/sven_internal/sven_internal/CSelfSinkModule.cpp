/**
 * Copyright - xWhitey, 2024.
 * CSelfSinkModule.cpp - CSelfSinkModule impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CSelfSinkModule.hpp"

#include "HLStrafe/hlstrafe.hpp"

//TODO: add module which uses jumpbug's way to predict our next position (ofc with speedhack too!)
CSelfSinkModule::CSelfSinkModule() : CModule("SelfSink", "Exploit") {
	m_pVecValues->push_back(m_pDepth = Q_new(CIntegerValue)("Depth", "sc_selfsink_depth", 1, 128, 2));

	DefineConCmd("sc_lightning_selfsink", CSelfSinkModule);
}

void CSelfSinkModule::OnEnable() {
	CModule::OnEnable();

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CSelfSinkModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	CModule::OnDisable();
}

void CSelfSinkModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (m_bState == false) {
		this->OnDisable();
		return;
	}

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_pPlayerMove) return;
		if (g_pPlayerMove->dead || g_pPlayerMove->deadflag == DEAD_DEAD) {
			m_bState = false;
			this->OnDisable();
			return;
		}
		auto e = static_cast<const CUpdateEvent*>(_Event);
		if (g_pPlayerMove->flags & FL_ONGROUND) {
			e->m_pCmd->buttons |= IN_JUMP;
		}
		HLStrafe::PlayerData plr = HLStrafe::PlayerData();
		plr.Origin[0] = g_pPlayerMove->origin[0];
		plr.Origin[1] = g_pPlayerMove->origin[1];
		plr.Origin[2] = g_pPlayerMove->origin[2];
		plr.Velocity[0] = g_pPlayerMove->velocity[0];
		plr.Velocity[1] = g_pPlayerMove->velocity[1];
		plr.Velocity[2] = g_pPlayerMove->velocity[2];
		plr.Basevelocity[0] = 0.f;
		plr.Basevelocity[1] = 0.f;
		plr.Basevelocity[2] = 0.f;
		plr.Viewangles[0] = e->m_pCmd->viewangles[1];
		plr.Viewangles[1] = e->m_pCmd->viewangles[1];
		plr.Viewangles[2] = e->m_pCmd->viewangles[1];
		plr.Ducking = (g_pPlayerMove->flags & FL_DUCKING) != 0;
		plr.InDuckAnimation = (g_pPlayerMove->bInDuck != 0);
		plr.DuckTime = g_pPlayerMove->flDuckTime;
		plr.StaminaTime = g_pPlayerMove->fuser2;
		plr.Walking = (g_pPlayerMove->movetype == MOVETYPE_WALK);
		auto postype = HLStrafe::GetPositionType(plr, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
		if (postype == HLStrafe::PositionType::AIR) {
			e->m_pCmd->buttons |= IN_DUCK;
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
			}
			else {
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
			vars.Frametime = e->m_flFrametime;
			auto playerCopy = HLStrafe::PlayerData(plr);
			playerCopy.Ducking = true;
			playerCopy.InDuckAnimation = false;
			playerCopy.DuckTime = 0;
			for (int idx = 0; idx < m_pDepth->Get(); idx++) {
				auto nextPostype = HLStrafe::Move(playerCopy, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
				if (nextPostype == HLStrafe::PositionType::GROUND) {
					g_pEngfuncs->pfnClientCmd(";stuck_kill;\n");
				}
			}
		}
	}
}