#include "StdAfx.h"
#include "CAutoJumpbugModule.hpp"

CAutoJumpbugModule* g_pAutoJumpbugModule;

void CMD_SC_Manual_Jumpbug_Down() {
	g_bDoingManualJB = true;
}

void CMD_SC_Manual_Jumpbug_Up() {
	g_bDoingManualJB = false;
}

CAutoJumpbugModule::CAutoJumpbugModule() : CModule("AutoJumpbug", "Movement") {
	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_autojumpbug_mode", "Old (requires speedhacking)\0New (speedhack-less)\0\0", 0));
	m_pVecValues->push_back(m_pGroundNormalAngle = Q_new(CFloatValue)("Ground normal angle", "sc_autojumpbug_gndnormalangle", 0.f, 1.f, 0.7f));
	m_pVecValues->push_back(m_pFrameZDist = Q_new(CFloatValue)("Old mode - Z Distance", "sc_autojumpbug_z_distance", 10.f, 60.f, 20.f));
	m_pVecValues->push_back(m_pDuckAtHeight = Q_new(CFloatValue)("New mode - Duck at height", "sc_autojumpbug_duck_at_height", 3.f, 20.f, 4.f));
	m_pVecValues->push_back(m_pJumpbugAtHeight = Q_new(CFloatValue)("New mode - Jumpbug at height", "sc_autojumpbug_jumpbug_at_height", 2.f, 16.f, 2.f));

	m_iJumpbugState = 0;
	g_bDoingManualJB = false;

	g_pEngfuncs->pfnAddCommand("+sc_manual_jumpbug", CMD_SC_Manual_Jumpbug_Down);
	g_pEngfuncs->pfnAddCommand("-sc_manual_jumpbug", CMD_SC_Manual_Jumpbug_Up);

	g_pAutoJumpbugModule = this;

	DefineConCmd("sc_lightning_autojumpbug", CAutoJumpbugModule);
}

void CAutoJumpbugModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CAutoJumpbugModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CAutoJumpbugModule::OnEvent(const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_p_fGameSpeed || !g_pPlayerMove) return;
		if (g_pPlayerMove->iuser1 != 0 /* spectating */) {
			m_iJumpbugState = 0;
			return;
		}
		if (g_pPlayerMove->flFallVelocity < 500.0f && g_bDoingManualJB == false) {
			m_iJumpbugState = 0;
			return;
		}
		if (!g_pMoveVars) {
			m_iJumpbugState = 0;
			return;
		}

		auto e = static_cast<const CUpdateEvent*>(_Event);

		Vector vecPredictVelocity = g_pPlayerMove->velocity * e->m_flFrametime;
		vecPredictVelocity.z = 0.f;

		Vector vecPredictOrigin = g_pPlayerMove->origin + vecPredictVelocity;
		Vector vecBottomOrigin = vecPredictOrigin;

		vecBottomOrigin.z -= 8192.0f;

		pmtrace_t* pTrace = g_pEngfuncs->PM_TraceLine(vecPredictOrigin, vecBottomOrigin, PM_NORMAL, (g_pPlayerMove->flags & FL_DUCKING) ? 1 : 0, -1);

		float flGroundNormalAngle = acosf(pTrace->plane.normal.z);

		if (flGroundNormalAngle <= acosf(m_pGroundNormalAngle->Get())) {
			if (m_pMode->Get() == 0 /* Old mode */) {
				float flHeight = fabsf(pTrace->endpos.z - vecPredictOrigin.z);
				float flG = (g_pMoveVars->entgravity * g_pMoveVars->gravity * 0.5f * e->m_flFrametime);
				float flFrameZDist = fabsf((g_pPlayerMove->flFallVelocity + flG) * e->m_flFrametime);
				//float flFrameZDist = fabsf((g_pPlayerMove->flFallVelocity + (g_pMoveVars->gravity * e->m_flFrametime)) * e->m_flFrametime);

				e->m_pCmd->buttons |= IN_DUCK;
				e->m_pCmd->buttons &= ~IN_JUMP;

				switch (m_iJumpbugState) {
				case 1:
					e->m_pCmd->buttons &= ~IN_DUCK;
					e->m_pCmd->buttons |= IN_JUMP;

					m_iJumpbugState = 2;
					break;

				case 2:
					m_iJumpbugState = 0;
					break;

				default:
					if (flFrameZDist > 0.f && fabsf(flHeight - flFrameZDist * 1.5f) <= m_pFrameZDist->Get()) {
						float flNeedSpeed = fabsf(flHeight - 19.f);
						float flScale = fabsf(flNeedSpeed / flFrameZDist);

						*g_p_fGameSpeed = static_cast<double>(flScale) * 1000.0;

						m_iJumpbugState = 1;
					}
					break;
				}
			} else if (m_pMode->Get() == 1 /* New mode */) { //There's a problem with frametime calculations...
				float flG = (g_pMoveVars->entgravity * g_pMoveVars->gravity * 0.5f * e->m_flFrametime);
				float flFrameZSpeed = fabsf((g_pPlayerMove->flFallVelocity + flG) * e->m_flFrametime);

				Vector vecNextPredictOrigin = g_pPlayerMove->origin + vecPredictVelocity;
				vecNextPredictOrigin.z += flG;
				vecBottomOrigin = vecNextPredictOrigin;

				vecBottomOrigin.z -= 8192.0f;

				pTrace = g_pEngfuncs->PM_TraceLine(vecNextPredictOrigin, vecBottomOrigin, PM_NORMAL, (g_pPlayerMove->flags & FL_DUCKING) ? 1 : 0, -1);

				float flPredictedHeight = fabsf(pTrace->endpos.z - vecNextPredictOrigin.z);

				if (flPredictedHeight <= m_pDuckAtHeight->Get()) {
					e->m_pCmd->buttons |= IN_DUCK;
					e->m_pCmd->buttons &= ~IN_JUMP;
				}

				switch (m_iJumpbugState) {
				case 1:
					e->m_pCmd->buttons &= ~IN_DUCK;
					e->m_pCmd->buttons |= IN_JUMP;

					m_iJumpbugState = 2;
					break;

				case 2:
					m_iJumpbugState = 0;
					break;

				default:
					if (flPredictedHeight > 0.f && fabsf(flPredictedHeight - flFrameZSpeed * 1.5f) <= (m_pJumpbugAtHeight->Get() + (flFrameZSpeed / 100.f + flFrameZSpeed / 1000.f + flFrameZSpeed / 10000.f))) {
						m_iJumpbugState = 1;
					}
					break;
				}
			}
		} else {
			m_iJumpbugState = 0;
		}
	}
}