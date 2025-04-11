#include "StdAfx.h"
#include "ClientDLL.hpp"

#include "CAutoJumpbugModule.hpp"

#include "HLStrafe/hlstrafe.hpp"
#include "CESPModule.hpp"
#include "CBacktrackModule.hpp"
#include "CForwardtrackModule.hpp"
#include "CFakeLagModule.hpp"

#pragma warning(disable: 26812)

#define Find(func_name) \
	do { \
		if ((ORIG_##func_name = reinterpret_cast<_##func_name>(GetProcAddress(reinterpret_cast<HMODULE>(g_pClientDll), #func_name)))) \
			g_pEngfuncs->Con_Printf("[client dll] Found " #func_name " at %p.\n", ORIG_##func_name); \
		else \
			g_pEngfuncs->Con_Printf("[client dll] Could not find " #func_name ".\n"); \
	} while (0)

#define CreateHook(func_name) \
		do { \
			status = MH_CreateHook(ORIG_##func_name, HOOKED_##func_name, reinterpret_cast<void**>(&ORIG_##func_name)); \
			if (status != MH_OK) { \
				g_pEngfuncs->Con_Printf("[client dll] Couldn't create hook for " #func_name ".\n"); \
			} \
		} while (0)

cvar_t* sc_lightning_speedhack;
cvar_t* bxt_tas_jumpbug_always_hold_duck;
cvar_t* bxt_tas_jumpbug_duck_at_height;
cvar_t* bxt_tas_jumpbug_auto_speedhack_once_near_ground;
cvar_t* bxt_tas_jumpbug_auto_speedhack_once_near_ground_gnd_dist;
cvar_t* bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart;
cvar_t* bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_distance;
cvar_t* bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_min_falldistance;
cvar_t* bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_last_z_velo_diff;
cvar_t* bxt_tas_ducktap_prediction;

cvar_t* sc_lightning_crosshair;
cvar_t* sc_lightning_crosshair_color;
cvar_t* sc_lightning_crosshair_alpha;
cvar_t* sc_lightning_crosshair_outline;
cvar_t* sc_lightning_crosshair_dot;
cvar_t* sc_lightning_crosshair_dot_size;
cvar_t* sc_lightning_crosshair_dot_color;
cvar_t* sc_lightning_crosshair_size;
cvar_t* sc_lightning_crosshair_gap;
cvar_t* sc_lightning_crosshair_thickness;
cvar_t* sc_lightning_crosshair_top_line;
cvar_t* sc_lightning_crosshair_bottom_line;
cvar_t* sc_lightning_crosshair_left_line;
cvar_t* sc_lightning_crosshair_right_line;

bool g_bDucktapDown = false;

bool g_bUseSpamDown = false;
bool g_bFlipUseSpamKeys = false;

bool g_bDuckJumpDown = false;

bool g_bFlashlightSpamDown = false;

bool g_bJumpbugDown = false;

bool g_bTasAutoJumpDown = false;

bool g_bNoAnimJumpDown = false;

Vector g_vecLastOnGroundPosition = Vector(0.f, 0.f, 0.f);
Vector g_vecLastPositiveZVeloPosition = Vector(0.f, 0.f, 0.f);

#ifndef DBL_MAX
#define DBL_MAX 2.2250738585072014e-308
#endif //!DBL_MAX

double TimeDifference(unsigned long long _Start, unsigned long long _End) {
	if (_End > _Start) {
		return (_End - _Start) / 1000.0;
	} else {
		return -((_Start - _End) / 1000.0);
	}

	return DBL_MAX;
}

void CMD_BXT_TAS_Ducktap_Down() {
	g_bDucktapDown = true;
}

void CMD_BXT_TAS_Ducktap_Up() {
	g_bDucktapDown = false;
}

void CMD_SC_UseSpam_Down() {
	g_bUseSpamDown = true;
}

void CMD_SC_UseSpam_Up() {
	g_bUseSpamDown = false;
}

void CMD_SC_DuckJump_Down() {
	g_bDuckJumpDown = true;
}

void CMD_SC_DuckJump_Up() {
	g_bDuckJumpDown = false;
}

void CMD_SC_FlashlightSpam_Down() {
	g_bFlashlightSpamDown = true;
}

void CMD_SC_FlashlightSpam_Up() {
	g_bFlashlightSpamDown = false;
}

void CMD_BXT_TAS_Jumpbug_Down() {
	g_bJumpbugDown = true;
}

void CMD_BXT_TAS_Jumpbug_Up() {
	g_bJumpbugDown = false;
}

void CMD_BXT_TAS_AutoJump_Down() {
	g_bTasAutoJumpDown = true;
}

void CMD_BXT_TAS_AutoJump_Up() {
	g_bTasAutoJumpDown = false;
}

void CMD_SC_NoAnimJump_Down() {
	g_bNoAnimJumpDown = true;
}

void CMD_SC_NoAnimJump_Up() {
	g_bNoAnimJumpDown = false;
}

extern "C" int __cdecl HOOKED_HUD_AddEntity(int _Type, cl_entity_s * _Entity, const char* _ModelName) {
	if (g_pBacktrackModule && g_pBacktrackModule->m_bState == true && g_pBacktrackModule->m_pCmd && g_pBacktrackModule->m_flFrametime != 0.f && _Entity && _Entity->player) {
		Vector vecSimulationOrigin, vecSimulationAngles;
		if (g_pBacktrackModule->BacktrackEntity(_Entity, (float)g_pBacktrackModule->m_pCmd->lerp_msec, g_pBacktrackModule->m_flFrametime, vecSimulationOrigin, vecSimulationAngles)) {
			g_aBacktrackEntities[_Entity->index] = *_Entity;

			g_pBacktrackModule->ConstructBacktrackEntity(g_aBacktrackEntities[_Entity->index], vecSimulationOrigin, vecSimulationAngles);
			g_pEngfuncs->CL_CreateVisibleEntity(2, &g_aBacktrackEntities[_Entity->index]);
		}
	}

	if (g_pForwardtrackModule && g_pForwardtrackModule->m_bState == true && g_pForwardtrackModule->m_flFrametime != 0.f && _Entity && _Entity->player) {
		Vector vecSimulationOrigin;
		if (g_pForwardtrackModule->ForwardtrackEntity(_Entity, g_pForwardtrackModule->m_flFrametime, vecSimulationOrigin)) {
			g_aForwardtrackEntities[_Entity->index] = *_Entity;

			g_pBacktrackModule->ConstructBacktrackEntity(g_aForwardtrackEntities[_Entity->index], vecSimulationOrigin, _Entity->curstate.angles);
			g_pEngfuncs->CL_CreateVisibleEntity(2, &g_aForwardtrackEntities[_Entity->index]);
		}
	}
	return ORIG_HUD_AddEntity(_Type, _Entity, _ModelName);
}

void __cdecl HOOKED_HUD_Redraw(float _Time, int _Intermission) {
	g_ScreenInfo.iSize = sizeof(SCREENINFO);
	g_pEngfuncs->pfnGetScreenInfo(&g_ScreenInfo);

	g_bIsPlaying = true;
	s_nLastHudRedrawTime = g_pTimer->m_nTimeInMilliseconds;

	return ORIG_HUD_Redraw(_Time, _Intermission);
}

void __cdecl HOOKED_HUD_PlayerMove(playermove_s* ppmove, qboolean server) {
	g_pPlayerMove = ppmove;
    if (!g_pMoveVars) {
        g_pMoveVars = g_pPlayerMove->movevars; //5.26 has playermove_s struct bigger than 5.25
    }

	auto e = Q_new(CPreMoveEvent);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	if (e->m_bIsCancelled) {
		Q_delete(e);
		return;
	}
	Q_delete(e);

	ORIG_HUD_PlayerMove(ppmove, server);
}

void __cdecl HOOKED_V_CalcRefdef(ref_params_s* pparams) {
	g_pRefParams = pparams;

	ORIG_V_CalcRefdef(pparams);

	auto e = Q_new(CMoveEvent)(g_pCmd, g_pPlayerMove);

	CCheat::GetCheat()->m_pEventBus->Post(e);

	Q_delete(e);
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

void __cdecl HOOKED_CL_CreateMove(FLOAT frametime, usercmd_s* cmd, INT active) {
	g_pCmd = cmd;

	g_flFrametime = frametime;
	g_iActive = active;

	if (sc_lightning_speedhack->value < 0.f) {
		sc_lightning_speedhack->value = 0.f;
	}

	if (g_pPlayerMove) {
		if ((g_pPlayerMove->flags & FL_ONGROUND) != 0) {
			g_vecLastOnGroundPosition = g_pPlayerMove->origin;
		}

		if (g_pPlayerMove->flFallVelocity < 0.0f && g_pPlayerMove->velocity.z > 0.f) {
			g_vecLastPositiveZVeloPosition = g_pPlayerMove->origin;
		}
	}

	auto e = Q_new(CPreUpdateEvent)(cmd, frametime);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	if (e->m_bIsCancelled) {
		Q_delete(e);
		return;
	}
	Q_delete(e);

	ORIG_CL_CreateMove(frametime, cmd, active);

	*g_p_fGameSpeed = (static_cast<double>(sc_lightning_speedhack->value) * 1000.0);

	if (!g_pEntityList)
		g_pEntityList = Q_new(CEntityList)();

	if (g_pEntityList)
		g_pEntityList->Process();

	CUpdateEvent* post_e = Q_new(CUpdateEvent)(cmd, frametime, active);

	CCheat::GetCheat()->m_pEventBus->Post(post_e);

	if (!post_e->m_bHasBeenFired) {
		CCheat::GetCheat()->m_pConsole->Printf("[INFO] HOOKED_CL_CreateMove: can't keep up!!! CUpdateEvent wasn't processed even after being fired!\n");
	}

	Q_delete(post_e);

	if (g_bDucktapDown == true && !g_bDoingManualJB && g_pAutoJumpbugModule->m_iJumpbugState == 0 && g_pPlayerMove) {
		if (bxt_tas_ducktap_prediction->value != 0.f) {
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
			plr.Viewangles[0] = cmd->viewangles[0];
			plr.Viewangles[1] = cmd->viewangles[1];
			plr.Viewangles[2] = cmd->viewangles[2];
			plr.Ducking = (g_pPlayerMove->flags & FL_DUCKING) != 0;
			plr.InDuckAnimation = (g_pPlayerMove->bInDuck != 0);
			plr.DuckTime = g_pPlayerMove->flDuckTime;
			plr.StaminaTime = g_pPlayerMove->fuser2;
			plr.Walking = (g_pPlayerMove->movetype == MOVETYPE_WALK);
			auto postype = HLStrafe::GetPositionType(plr, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
			if (postype == HLStrafe::PositionType::AIR) {
				HLStrafe::MovementVars vars;
				UTIL_GetHLStrafeMovementVars(vars);
				vars.Frametime = frametime;
				auto nextPostype = HLStrafe::Move(plr, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
				if (nextPostype == HLStrafe::PositionType::GROUND) {
					cmd->buttons |= IN_DUCK;
				}
			}
		}

		cmd->buttons |= IN_DUCK;

		static bool s_duck_was_down_last_frame = false;
		static bool should_release_duck;

		should_release_duck = (!(g_pPlayerMove->flags & FL_ONGROUND) && g_pPlayerMove->waterlevel < 1);

		if (s_duck_was_down_last_frame && (g_pPlayerMove->flags & FL_ONGROUND) && g_pPlayerMove->waterlevel < 1)
			should_release_duck = true;

		if (should_release_duck) {
			cmd->buttons &= ~IN_DUCK;
		}

		s_duck_was_down_last_frame = (cmd->buttons & IN_DUCK) != 0;
	}

	if (g_bUseSpamDown == true) {
		//if (cmd->buttons & IN_USE) {
		if (g_bFlipUseSpamKeys == true) {
			cmd->buttons &= ~IN_USE;
		} else {
			cmd->buttons |= IN_USE;
		}
		g_bFlipUseSpamKeys = g_bFlipUseSpamKeys ? false : true;
	}

	if (g_bJumpbugDown == true && g_pPlayerMove) {
		if (bxt_tas_jumpbug_always_hold_duck->value > 0.0f)
			cmd->buttons |= IN_DUCK;

		if (bxt_tas_jumpbug_auto_speedhack_once_near_ground->value < 0.0f)
			bxt_tas_jumpbug_auto_speedhack_once_near_ground->value = 0.0f;

		Vector vecPredictVelocity = g_pPlayerMove->velocity * frametime;
		vecPredictVelocity.z = 0.f;

		Vector vecPredictOrigin = g_pPlayerMove->origin + vecPredictVelocity;
		Vector vecBottomOrigin = vecPredictOrigin;

		vecBottomOrigin.z -= 8192.0f;

		pmtrace_t *pTrace = g_pEngfuncs->PM_TraceLine(vecPredictOrigin, vecBottomOrigin, PM_NORMAL, 1, -1);

		float flHeight = fabsf(pTrace->endpos.z - vecPredictOrigin.z);

		if (flHeight <= bxt_tas_jumpbug_auto_speedhack_once_near_ground_gnd_dist->value) {
			if (g_pPlayerMove->velocity.z <= 1.f) {
				bool bShallSpeedhack = true;
				if (bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart->value != 0.f) {
					if (g_vecLastOnGroundPosition.IsZero() && g_vecLastPositiveZVeloPosition.IsZero() /* for some reason we couldn't detect any movements of the player */) bShallSpeedhack = false;
					//Vector vecDifference = g_pPlayerMove->origin - g_vecLastOnGroundPosition;
					//if (fabsf(vecDifference.Length()) < bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_distance->value) bShallSpeedhack = false;
					if (fabsf(g_vecLastOnGroundPosition.z - g_pPlayerMove->origin.z) <= 
						fabsf(bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_min_falldistance->value) /* we fell over a short distance */) bShallSpeedhack = false;
					if (bShallSpeedhack == false) {
						if (g_pPlayerMove->flFallVelocity >= 300.f /* we got fastcrowbared */) bShallSpeedhack = true;
						if (fabsf(g_vecLastPositiveZVeloPosition.z - g_pPlayerMove->origin.z) >= 
							fabsf(bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_last_z_velo_diff->value) /* seems like we flew using a nade and then fell back, allow speehack in case of such */) bShallSpeedhack = true;
					}
				}
				if (bShallSpeedhack == true) {
					*g_p_fGameSpeed = (static_cast<double>(bxt_tas_jumpbug_auto_speedhack_once_near_ground->value) * 1000.0);
				}
			}
		}
		if (bxt_tas_jumpbug_duck_at_height->value > 0.f) {
			if (flHeight <= bxt_tas_jumpbug_duck_at_height->value) {
				cmd->buttons |= IN_DUCK;
				cmd->buttons &= ~IN_JUMP;
			}
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
		plr.Viewangles[0] = cmd->viewangles[0];
		plr.Viewangles[1] = cmd->viewangles[1];
		plr.Viewangles[2] = cmd->viewangles[2];
		plr.Ducking = (g_pPlayerMove->flags & FL_DUCKING) != 0;
		plr.InDuckAnimation = (g_pPlayerMove->bInDuck != 0);
		plr.DuckTime = g_pPlayerMove->flDuckTime;
		plr.StaminaTime = g_pPlayerMove->fuser2;
		plr.Walking = (g_pPlayerMove->movetype == MOVETYPE_WALK);
		auto playerCopy = HLStrafe::PlayerData(plr);
		auto postype = HLStrafe::GetPositionType(playerCopy, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);

		if (postype == HLStrafe::PositionType::AIR) {
			if (plr.Ducking) {
				playerCopy.Ducking = false;
				playerCopy.InDuckAnimation = false;
				playerCopy.DuckTime = 0;

				auto nextPostype = HLStrafe::GetPositionType(playerCopy, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
				if (nextPostype == HLStrafe::PositionType::GROUND) {
					cmd->buttons &= ~IN_DUCK;
					cmd->buttons |= IN_JUMP;
					g_vecLastOnGroundPosition = g_pPlayerMove->origin;
				} else {
					playerCopy.Ducking = true;
					plr.DuckTime = g_pPlayerMove->flDuckTime; 
					nextPostype = HLStrafe::GetPositionType(playerCopy, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
					if (nextPostype == HLStrafe::PositionType::GROUND) {
						cmd->buttons &= ~IN_DUCK;
						cmd->buttons |= IN_JUMP;
						g_vecLastOnGroundPosition = g_pPlayerMove->origin;
					}
				}
			} else if (bxt_tas_jumpbug_always_hold_duck->value == 0.0f) {
				HLStrafe::MovementVars vars;
				UTIL_GetHLStrafeMovementVars(vars);
				vars.Frametime = frametime;
				auto nextPostype = HLStrafe::Move(playerCopy, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
				if (nextPostype == HLStrafe::PositionType::GROUND) {
					cmd->buttons |= IN_DUCK;
					cmd->buttons &= ~IN_JUMP;
				}
			}
		}
	}

	if (g_bTasAutoJumpDown == true && g_pPlayerMove) {
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
		plr.Viewangles[0] = cmd->viewangles[0];
		plr.Viewangles[1] = cmd->viewangles[1];
		plr.Viewangles[2] = cmd->viewangles[2];
		plr.Ducking = (g_pPlayerMove->flags & FL_DUCKING) != 0;
		plr.InDuckAnimation = (g_pPlayerMove->bInDuck != 0);
		plr.DuckTime = g_pPlayerMove->flDuckTime;
		plr.StaminaTime = g_pPlayerMove->fuser2;
		plr.Walking = (g_pPlayerMove->movetype == MOVETYPE_WALK);
		auto postype = HLStrafe::GetPositionType(plr, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);

		HLStrafe::MovementVars vars;
		UTIL_GetHLStrafeMovementVars(vars);
		vars.Frametime = frametime;
		auto nextPostype = HLStrafe::Move(plr, vars, postype, vars.Maxspeed, HLStrafe::TraceFunction, HLStrafe::PointContentsFunction, HLStrafe::MAX_SUPPORTED_VERSION);
		if (nextPostype == HLStrafe::PositionType::GROUND || postype == HLStrafe::PositionType::GROUND || g_pPlayerMove->iuser1 != 0 || g_pPlayerMove->movetype == MOVETYPE_FLY) {
			cmd->buttons |= IN_JUMP;
		}

		if ((g_pPlayerMove->waterlevel < 1) && !(g_pPlayerMove->movetype == MOVETYPE_FLY) &&
			(g_pPlayerMove->iuser1 == 0 /* not spectating */) && !(g_pPlayerMove->flags & FL_ONGROUND) && (cmd->buttons & IN_JUMP) != 0) {
			cmd->buttons &= ~IN_JUMP;
		}
	}

	if (g_bDuckJumpDown == true && !g_bDoingManualJB && g_pAutoJumpbugModule->m_iJumpbugState == 0 && g_bDucktapDown == false && g_pPlayerMove) {
		bool bWasDuckManuallyPressed = (cmd->buttons & IN_DUCK) != 0 || CCheat::GetCheat()->m_pLocalPlayer->m_pVirtualInputButtons->IsButtonBeingHold(EInputBtns::kDuck) || (g_pPlayerMove->flags & FL_DUCKING) != 0;
		if (!bWasDuckManuallyPressed)
			cmd->buttons |= IN_DUCK;

		cmd->buttons |= IN_JUMP;

		static bool s_duck_was_down_last_frame = false;
		static bool should_release_duck;

		should_release_duck = (!(g_pPlayerMove->flags & FL_ONGROUND) && g_pPlayerMove->waterlevel < 1);

		if (s_duck_was_down_last_frame && (g_pPlayerMove->flags & FL_ONGROUND) && g_pPlayerMove->waterlevel < 1)
			should_release_duck = true;

		if (should_release_duck && !bWasDuckManuallyPressed) {
			cmd->buttons &= ~IN_DUCK;
		}

		s_duck_was_down_last_frame = (cmd->buttons & IN_DUCK) != 0;

		if ((g_pPlayerMove->waterlevel < 1) && !(g_pPlayerMove->movetype == MOVETYPE_FLY) &&
			(g_pPlayerMove->iuser1 == 0 /* not spectating */) && !(g_pPlayerMove->flags & FL_ONGROUND) && (cmd->buttons & IN_JUMP) != 0) {
			cmd->buttons &= ~IN_JUMP;
		}
	}

	if (g_bNoAnimJumpDown == true && !g_bDoingManualJB && g_pAutoJumpbugModule->m_iJumpbugState == 0 && g_bDucktapDown == false && g_pPlayerMove) {
		bool bWasDuckManuallyPressed = (cmd->buttons & IN_DUCK) != 0 || CCheat::GetCheat()->m_pLocalPlayer->m_pVirtualInputButtons->IsButtonBeingHold(EInputBtns::kDuck) || (g_pPlayerMove->flags & FL_DUCKING) != 0;
		if (!bWasDuckManuallyPressed)
			cmd->buttons |= IN_DUCK;

		cmd->buttons |= IN_JUMP;

		static bool s_duck_was_down_last_frame = false;
		static bool should_release_duck;
		static int cs_iDuckButtonMidAirStage = 0;
		static bool cs_bDuckWasPressedMidAir = false;

		should_release_duck = (!(g_pPlayerMove->flags & FL_ONGROUND) && g_pPlayerMove->waterlevel < 1);

		if (s_duck_was_down_last_frame && !cs_bDuckWasPressedMidAir && (g_pPlayerMove->flags & FL_ONGROUND) && g_pPlayerMove->waterlevel < 1)
			should_release_duck = true;

		if (should_release_duck && !bWasDuckManuallyPressed) {
			cmd->buttons &= ~IN_DUCK;
		}

		s_duck_was_down_last_frame = (cmd->buttons & IN_DUCK) != 0;

		if ((g_pPlayerMove->flags & FL_ONGROUND) != 0 && cs_bDuckWasPressedMidAir) {
			cs_bDuckWasPressedMidAir = false;
		}

		if ((g_pPlayerMove->waterlevel < 1) && !(g_pPlayerMove->movetype == MOVETYPE_FLY) &&
			(g_pPlayerMove->iuser1 == 0 /* not spectating */) && !(g_pPlayerMove->flags & FL_ONGROUND) && (cmd->buttons & IN_JUMP) != 0) {
			cmd->buttons &= ~IN_JUMP;

			if (!bWasDuckManuallyPressed) {
				switch (cs_iDuckButtonMidAirStage) {
				case 12: {
					if (!cs_bDuckWasPressedMidAir) {
						cmd->buttons &= ~IN_JUMP;
						cmd->buttons |= IN_DUCK;
						cs_bDuckWasPressedMidAir = true;
					}
					cs_iDuckButtonMidAirStage++;
				}
					break;
				case 13: {
					if (cs_bDuckWasPressedMidAir) {
						cmd->buttons |= IN_JUMP;
						cmd->buttons &= ~IN_DUCK;
					}
					cs_iDuckButtonMidAirStage = 0;
				}
					break;
				default: {
					cs_iDuckButtonMidAirStage++;
				}
				}
			}
		}
	}

	if (g_bFlashlightSpamDown) {
		cmd->impulse = 100;
	}
}

void CMD_BXT_Append() {
	if (g_pEngfuncs->Cmd_Argc() < 2) {
		g_pEngfuncs->Con_Printf("Usage: %s <command> - Append a command into the beginning of command queue\n Similar to how \"special\" appends \"_special\"\n", g_pEngfuncs->Cmd_Argv(0));

		return;
	}

	g_pEngfuncs->pfnClientCmd(g_pEngfuncs->Cmd_Argv(1));
}

void __cdecl HOOKED_HUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed) {
	if (!from || !to || !cmd) return ORIG_HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);

	auto e = Q_new(CPrePostRunCmdEvent)(from, to, cmd, runfuncs, time, random_seed);
	CCheat::GetCheat()->m_pEventBus->Post(e);
	if (e->m_bIsCancelled) {
		Q_delete(e);
		return;
	}

	ORIG_HUD_PostRunCmd(e->m_pFrom, e->m_pTo, e->m_pCmd, e->m_iRunfuncs, e->m_dblTime, e->m_uiRandomSeed);
	//ORIG_HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);

	auto post_e = Q_new(CPostPostRunCmdEvent)(e->m_pFrom, e->m_pTo, e->m_pCmd, e->m_iRunfuncs, e->m_dblTime, e->m_uiRandomSeed);

	CCheat::GetCheat()->m_pEventBus->Post(post_e);

	Q_delete(e);
	Q_delete(post_e);
}


void Cmd_Multiwait() {
	if (g_pEngfuncs->Cmd_Argc() > 1) {
		int iNumber = atoi(g_pEngfuncs->Cmd_Argv(1));
		char rgszCommandBuffer[256];
		if (iNumber > 1) {
			_snprintf_s(rgszCommandBuffer, Q_ARRAYSIZE(rgszCommandBuffer), ";wait;\nw %d;\n", iNumber - 1);
		} else if (iNumber == 1) {
			_snprintf_s(rgszCommandBuffer, Q_ARRAYSIZE(rgszCommandBuffer), ";wait;\n");
		} else {
			return;
		}

		ORIG_Cbuf_InsertText(rgszCommandBuffer);
	}
	else {
		ORIG_Cbuf_InsertText("wait\n");
	}
}

void CClientHooks::Initialize() {
	if (g_pClientDll) {
		FindAsync(g_pClientDll, patterns::engine::GetClientColor_movsx_eax_word_ptr_ds_g_pExtraPlayerInfo_10, g_pExtraPlayerInfo, [](const char* _Version) {
			//Sw1ft747's
			unsigned char* lpucDisassembly = reinterpret_cast<unsigned char*>(reinterpret_cast<DWORD>(g_pExtraPlayerInfo) + 3);
			DWORD dwOffset = (DWORD)(CUtils::BuildIntFromUnsignedChars(lpucDisassembly)) - 0xA /* 10 */;
			g_pExtraPlayerInfo = reinterpret_cast<CExtraPlayerInfo*>(dwOffset);
			if (g_bIsSven5_26) {
				g_pExtraPlayerInfo_526 = reinterpret_cast<CExtraPlayerInfo_526*>(dwOffset);
			}
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Found g_pExtraPlayerInfo at 0x%p (g_pExtraPlayerInfo_526 is at 0x%p)\n", g_pExtraPlayerInfo, g_pExtraPlayerInfo_526);
		}, []() {
			CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] Couldn't find g_pExtraPlayerInfo.\n");
		});

		if (g_pEngfuncs) {
			if (!ORIG_Cbuf_AddText) {
				ORIG_Cbuf_AddText = indirect_cast<decltype(ORIG_Cbuf_AddText)>(g_pEngfuncs->pfnClientCmd);
			}

			sv_accelerate = g_pEngfuncs->pfnGetCvarPointer("sv_accelerate");
			sv_airaccelerate = g_pEngfuncs->pfnGetCvarPointer("sv_airaccelerate");
			sv_bounce = g_pEngfuncs->pfnGetCvarPointer("sv_bounce");
			sv_friction = g_pEngfuncs->pfnGetCvarPointer("sv_friction");
			edgefriction = g_pEngfuncs->pfnGetCvarPointer("edgefriction");
			sv_gravity = g_pEngfuncs->pfnGetCvarPointer("sv_gravity");
			sv_stepsize = g_pEngfuncs->pfnGetCvarPointer("sv_stepsize");
			sv_maxvelocity = g_pEngfuncs->pfnGetCvarPointer("sv_maxvelocity");
			sv_stopspeed = g_pEngfuncs->pfnGetCvarPointer("sv_stopspeed");

			g_pEngfuncs->pfnAddCommand("+bxt_tas_ducktap", CMD_BXT_TAS_Ducktap_Down);
			g_pEngfuncs->pfnAddCommand("-bxt_tas_ducktap", CMD_BXT_TAS_Ducktap_Up);
			g_pEngfuncs->pfnAddCommand("+bxt_tas_jumpbug", CMD_BXT_TAS_Jumpbug_Down);
			g_pEngfuncs->pfnAddCommand("-bxt_tas_jumpbug", CMD_BXT_TAS_Jumpbug_Up);
			g_pEngfuncs->pfnAddCommand("+sc_ducktap", CMD_BXT_TAS_Ducktap_Down);
			g_pEngfuncs->pfnAddCommand("-sc_ducktap", CMD_BXT_TAS_Ducktap_Up);
			g_pEngfuncs->pfnAddCommand("+sc_usespam", CMD_SC_UseSpam_Down);
			g_pEngfuncs->pfnAddCommand("-sc_usespam", CMD_SC_UseSpam_Up);
			g_pEngfuncs->pfnAddCommand("+sc_duckjump", CMD_SC_DuckJump_Down);
			g_pEngfuncs->pfnAddCommand("-sc_duckjump", CMD_SC_DuckJump_Up);
			g_pEngfuncs->pfnAddCommand("+sc_noanimjump", CMD_SC_NoAnimJump_Down);
			g_pEngfuncs->pfnAddCommand("-sc_noanimjump", CMD_SC_NoAnimJump_Up);
			g_pEngfuncs->pfnAddCommand("+sc_flashlightspam", CMD_SC_FlashlightSpam_Down);
			g_pEngfuncs->pfnAddCommand("-sc_flashlightspam", CMD_SC_FlashlightSpam_Up);
			g_pEngfuncs->pfnAddCommand("+bxt_tas_autojump", CMD_BXT_TAS_AutoJump_Down);
			g_pEngfuncs->pfnAddCommand("-bxt_tas_autojump", CMD_BXT_TAS_AutoJump_Up);
			g_pEngfuncs->pfnAddCommand("bxt_append", CMD_BXT_Append);
			g_pEngfuncs->pfnAddCommand("sc_append", CMD_BXT_Append);
			g_pEngfuncs->pfnAddCommand("w", Cmd_Multiwait);
			sc_lightning_speedhack = g_pEngfuncs->pfnRegisterVariable("sc_lightning_speedhack", "1.0", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_always_hold_duck = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_always_hold_duck", "1.0", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_duck_at_height = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_duck_at_height", "0", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_auto_speedhack_once_near_ground = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_auto_speedhack_once_near_ground", "0.04", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_auto_speedhack_once_near_ground_gnd_dist = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_auto_speedhack_once_near_ground_gnd_dist", "50.0", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart", "1", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_distance = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_distance", "200.0", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_min_falldistance = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_min_falldistance", "100.0", FCVAR_CLIENTDLL);
			bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_last_z_velo_diff = g_pEngfuncs->pfnRegisterVariable("bxt_tas_jumpbug_auto_speedhack_once_near_ground_smart_last_z_velo_diff", "150.0", FCVAR_CLIENTDLL);
			bxt_tas_ducktap_prediction = g_pEngfuncs->pfnRegisterVariable("bxt_tas_ducktap_prediction", "1", FCVAR_CLIENTDLL);

			sc_lightning_crosshair = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair", "1", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_color = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_color", "255 255 255", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_alpha = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_alpha", "255", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_outline = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_outline", "3", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_dot = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_dot", "0", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_dot_size = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_dot_size", "0", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_dot_color = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_dot_color", "0 0 0", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_size = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_size", "13", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_gap = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_gap", "5", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_thickness = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_thickness", "2", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_top_line = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_top_line", "1", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_bottom_line = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_bottom_line", "1", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_left_line = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_left_line", "1", FCVAR_CLIENTDLL);
			sc_lightning_crosshair_right_line = g_pEngfuncs->pfnRegisterVariable("sc_lightning_crosshair_right_line", "1", FCVAR_CLIENTDLL);

			sc_lightning_dont_send_sendents = g_pEngfuncs->pfnRegisterVariable("sc_lightning_dont_send_sendents", "0", 0);

			if (ORIG_SPR_Set && ORIG_Draw_FillRGBA) {
				sc_game_hud_color_same_as_client_color = g_pEngfuncs->pfnRegisterVariable("sc_game_hud_color_same_as_client_color", "0", FCVAR_CLIENTDLL);
			}

			Find(CL_CreateMove);
			Find(V_CalcRefdef);
			Find(HUD_PlayerMove);
			Find(HUD_PostRunCmd);
			Find(IN_ActivateMouse);
			Find(IN_DeactivateMouse);
			Find(CL_CameraOffset);
			Find(CL_IsThirdPerson);
			Find(HUD_Redraw);
			Find(HUD_AddEntity);

			MH_STATUS status;
			CreateHook(CL_CreateMove);
			CreateHook(V_CalcRefdef);
			CreateHook(HUD_PlayerMove);
			CreateHook(HUD_PostRunCmd);
			CreateHook(HUD_Redraw);
			CreateHook(HUD_AddEntity);

			status = MH_EnableHook(MH_ALL_HOOKS);
			if (status == MH_OK) {
				g_pEngfuncs->Con_Printf("[client dll] Hooked!\n");
			} else {
				g_pEngfuncs->Con_Printf("[client dll] Couldn't enable hooks: %s\n", MH_StatusToString(status));
			}
		}
	} else {
		printf("Couldn't find client.dll module!\n");
	}
}