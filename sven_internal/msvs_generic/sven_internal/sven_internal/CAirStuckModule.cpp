#include "StdAfx.h"
#include "CAirStuckModule.hpp"

CAirStuckModule::CAirStuckModule() : CModule("AirStuck", "Exploit") {
	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_airstuck_mode", "Hold packets\0Timestop\0\0", 0));
	m_pVecValues->push_back(m_pFixupPosition = Q_new(CBoolValue)("Fixup position", "sc_airstuck_fixup_position", true));
	m_pVecValues->push_back(m_pPreserveDuckState = Q_new(CBoolValue)("Preserve duck state", "sc_airstuck_preserve_duck_state", true));
	m_pVecValues->push_back(m_pEmulateFakeMovement = Q_new(CBoolValue)("Timestop mode - Emulate fake movement (makes fake jumps)", "sc_airstuck_emulate_fake_movement", true));
	m_pVecValues->push_back(m_pPerspective = Q_new(CBoolValue)("Timestop mode - Lock viewangles (a.k.a. perspective)", "sc_airstuck_perspective", false));
	m_pVecValues->push_back(m_pFixRollAngles = Q_new(CBoolValue)("Fix roll angles", "sc_airstuck_fix_roll_angles", false));

	m_vecFixedPosition = g_vecZero;
	m_bWasDucking = false;

	DefineConCmd("sc_lightning_airstuck", CAirStuckModule);
}

void CAirStuckModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);

	if (g_pPlayerMove) {
		if (g_pRefParams) {
			g_pEngfuncs->GetViewAngles(m_vecSavedViewangles);
		}

		m_vecFixedPosition = g_pPlayerMove->origin;
		m_bWasDucking = (g_pPlayerMove->flags & FL_DUCKING) ? true : false;
	}
}

void CAirStuckModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CAirStuckModule::OnEvent(const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kMoveEvent) {
		//auto e = static_cast<const CMoveEvent*>(_Event);

		if (m_pFixupPosition->Get()) {
			if (g_pPlayerMove && g_pRefParams) {
				g_pPlayerMove->origin = m_vecFixedPosition;
				m_vecFixedPosition.CopyToArray(g_pRefParams->simorg);
				m_vecFixedPosition.CopyToArray(g_pRefParams->vieworg);
				g_pRefParams->vieworg[0] += g_pRefParams->viewheight[0];
				g_pRefParams->vieworg[1] += g_pRefParams->viewheight[1];
				g_pRefParams->vieworg[2] += g_pRefParams->viewheight[2];

				cl_entity_t* view = g_pEngfuncs->GetViewModel();
				m_vecFixedPosition.CopyToArray(view->origin);
				view->origin[0] += g_pRefParams->viewheight[0];
				view->origin[1] += g_pRefParams->viewheight[1];
				view->origin[2] += g_pRefParams->viewheight[2];

				if (ORIG_CL_IsThirdPerson && ORIG_CL_IsThirdPerson()) {
					Vector ofs = g_vecZero;

					ORIG_CL_CameraOffset(ofs);

					Vector vecCamForward;
					Vector vecCamAngles = ofs;
					vecCamAngles[2] = 0;

					g_pEngfuncs->pfnAngleVectors(vecCamAngles, vecCamForward, NULL, NULL);

					for (int idx = 0; idx < 3; idx++) {
						g_pRefParams->vieworg[idx] += -ofs[2] * vecCamForward[idx];
					}
				}

				cl_entity_t* plr = g_pEngfuncs->GetLocalPlayer();
				plr->origin = m_vecFixedPosition;
				plr->curstate.origin = m_vecFixedPosition;
				plr->prevstate.origin = m_vecFixedPosition;
				plr->latched.prevorigin = m_vecFixedPosition;
			}
		}

		if (m_pPreserveDuckState->Get()) {
			if (m_bWasDucking == true) {
				if (g_pPlayerMove) {
					g_pPlayerMove->bInDuck = 1;
					g_pPlayerMove->flags |= FL_DUCKING;
				}
			}
		}

		if (m_pFixRollAngles->Get()) {
			if (m_pMode->Get() == 1 /* Timestop */ && !m_pPerspective->Get()) return;

			cl_entity_t* plr = g_pEngfuncs->GetLocalPlayer();

			float flPitch = CUtils::Clamp(-79.f, 79.f, m_vecSavedViewangles.x / -3.0f);

			plr->angles.x = flPitch;
			plr->curstate.angles.x = flPitch;
			plr->prevstate.angles.x = flPitch;
			plr->latched.prevangles.x = flPitch;
		}
	}

	if (_Event->GetType() == EEventType::kNetworkUserCommandSubmitEvent) {
		auto e = static_cast<const CNetworkUserCommandSubmitEvent*>(_Event);

		if (m_pPreserveDuckState->Get()) {
			if (m_bWasDucking == true) {
				e->m_pTo->buttons |= IN_DUCK;
				if (g_pPlayerMove) {
					g_pPlayerMove->bInDuck = 1;
					g_pPlayerMove->flags |= FL_DUCKING;
				}
			}
		}

		if (m_pEmulateFakeMovement->Get()) {
			if ((e->m_pFrom->buttons & IN_JUMP) == 0) {
				e->m_pTo->buttons |= IN_JUMP;
			} else {
				e->m_pFrom->buttons &= ~IN_JUMP;
			}
		}

		if (m_pMode->Get() == 1 /* Timestop */) {
			e->m_pTo->msec = 0;
			if (m_pPerspective->Get()) {
				e->m_pTo->viewangles = m_vecSavedViewangles;
			}
		}
	}

	if (_Event->GetType() == EEventType::kPacketEvent) {
		auto e = static_cast<const CPacketEvent*>(_Event);

		if (m_pMode->Get() == 0 /* Hold packets */)
			e->CancelEvent();

		if (m_pMode->Get() == 1 /* Timestop */) {
			if (g_pPlayerMove) {
				if (g_pPlayerMove->waterlevel > 1)
					e->CancelEvent();
			}
		}
	}
}