#include "StdAfx.h"
#include "CDebugModule.hpp"
#include "HLStrafe/hlstrafe.hpp"

//Does nothing useful - just creates a notification (leftover system, was scheduled for deletion back in the days)
CDebugModule::CDebugModule() : CModule("Debug", "Misc") {
	DefineConCmd("sc_lightning_debug", CDebugModule);
}

void CDebugModule::OnEnable() {
	//CCheat::GetCheat()->m_pNotifications->PushNotification("Debug enabled!", 2.0);

	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CDebugModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();

	//CCheat::GetCheat()->m_pNotifications->PushNotification("Debug disabled!", 2.0);
}

//This is awful. Don't do that
#define UTIL_GetHLStrafeMovementVars(_VarName) \
	_VarName = HLStrafe::MovementVars(); \
	if (g_pMoveVars) { \
		_VarName.Accelerate = g_pMoveVars->accelerate; \
		_VarName.Airaccelerate = g_pMoveVars->airaccelerate; \
		_VarName.Bounce = g_pMoveVars->bounce; \
		_VarName.Friction = g_pMoveVars->friction; \
		_VarName.Edgefriction = g_pMoveVars->edgefriction; \
		_VarName.Gravity = g_pMoveVars->gravity; \
		_VarName.EntGravity = g_pMoveVars->entgravity; \
		_VarName.Maxvelocity = g_pMoveVars->maxvelocity; \
		_VarName.Stepsize = g_pMoveVars->stepsize; \
	} else { \
		_VarName.Accelerate = sv_accelerate->value; \
		_VarName.Airaccelerate = sv_airaccelerate->value; \
		_VarName.Bounce = sv_bounce->value; \
		_VarName.Friction = sv_friction->value; \
		_VarName.Edgefriction = edgefriction->value; \
		_VarName.Gravity = sv_gravity->value; \
		_VarName.EntGravity = 1.0f; \
		_VarName.Maxvelocity = sv_maxvelocity->value; \
		_VarName.Stepsize = sv_stepsize->value; \
	} \
	_VarName.EntFriction = g_pPlayerMove->friction; \
	_VarName.Maxspeed = g_pPlayerMove->maxspeed; \

void CDebugModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kRenderEvent) {
		if (!g_pCmd) return;
		if (!g_pPlayerMove) return;

		HLStrafe::PlayerData plr = HLStrafe::PlayerData();
		plr.Origin[0] = g_pPlayerMove->origin[0];
		plr.Origin[1] = g_pPlayerMove->origin[1];
		plr.Origin[2] = g_pPlayerMove->origin[2];
		plr.Velocity[0] = g_pPlayerMove->velocity[0];
		plr.Velocity[1] = g_pPlayerMove->velocity[1];
		plr.Velocity[2] = g_pPlayerMove->velocity[2];
		plr.Basevelocity[0] = g_pPlayerMove->basevelocity[0];
		plr.Basevelocity[1] = g_pPlayerMove->basevelocity[1];
		plr.Basevelocity[2] = g_pPlayerMove->basevelocity[2];
		plr.Viewangles[0] = g_pCmd->viewangles[0];
		plr.Viewangles[1] = g_pCmd->viewangles[1];
		plr.Viewangles[2] = g_pCmd->viewangles[2];
		plr.Ducking = (g_pPlayerMove->flags & FL_DUCKING) != 0;
		plr.InDuckAnimation = (g_pPlayerMove->bInDuck != 0);
		plr.DuckTime = g_pPlayerMove->flDuckTime;
		plr.StaminaTime = g_pPlayerMove->fuser2;
		plr.Walking = (g_pPlayerMove->movetype == MOVETYPE_WALK);
		auto postype = HLStrafe::GetPositionType(plr, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
		if (postype == HLStrafe::PositionType::AIR) {
			HLStrafe::MovementVars vars;
			UTIL_GetHLStrafeMovementVars(vars);
			vars.Frametime = g_flFrametime;
			auto nextPostype = HLStrafe::Move(plr, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
			while (nextPostype != HLStrafe::PositionType::GROUND) {
				Vector2D vecScreen;
				if (CUtils::CalcScreen(plr.Origin, vecScreen, g_ScreenInfo)) {
					ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(vecScreen.x - 2.f, vecScreen.y - 2.f), ImVec2(vecScreen.x + 2.f, vecScreen.y + 2.f), ImColor(255, 255, 255, 255));
				}
				nextPostype = HLStrafe::Move(plr, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
			}
		}
	}
}