#include "StdAfx.h"
#include "CDoubletapModule.hpp"

CDoubletapModule::CDoubletapModule() : CModule("Doubletap", "Combat") {
	m_pVecValues->push_back(m_pMode = Q_new(CListValue)("Mode", "sc_doubletap_mode", "Offensive\0Defensive\0\0", 1));
	m_pVecValues->push_back(m_pRecharge = Q_new(CIntegerValue)("Recharge", "sc_doubletap_recharge", 4, 33, 17));
	m_pVecValues->push_back(m_pTicksToShift = Q_new(CFloatValue)("Speed", "sc_doubletap_speed", 2.f, 100.f, 50.f));
	m_nTicksPassed = 0;
	m_bShouldShift = false;
	m_vecFixedPosition = g_vecZero;

	g_pEngfuncs->pfnAddCommand("sc_lightning_do_doubletap", functional_cast<void()>([this]() {
		m_bShouldShift = true;
		m_vecFixedPosition = g_pPlayerMove->origin;
	}));

	DefineConCmd("sc_lightning_doubletap", CDoubletapModule);
}

void CDoubletapModule::OnEnable() {
	CModule::OnEnable();

	m_bCharging = false;
	m_bShouldShift = false;
	m_bHasDoneShifting = false;
	m_nTicksPassed = 0;

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CDoubletapModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	if (m_bCharging) {
		m_bCharging = false;
		if (g_pfNextCmdTime)
			*g_pfNextCmdTime = -1.f;
	}

	m_nTicksPassed = 0;
	m_bShouldShift = false;
	m_bHasDoneShifting = false;

	CModule::OnDisable();
}

//TODO: implement LTFX2 dt (desync!!)
void CDoubletapModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (!g_p_fGameSpeed) return;

	if (_Event->GetType() == EEventType::kUpdateEvent) {
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

		if (m_bHasDoneShifting) {
			if (m_nTicksPassed < m_pRecharge->Get()) {
				*g_pfNextCmdTime = FLT_MAX;
				m_nTicksPassed++;
				m_bCharging = true;
			} else {
				m_bHasDoneShifting = false;
				m_bCharging = false;
				m_nTicksPassed = 0;
				*g_pfNextCmdTime = -1.f;
			}

			return;
		}

		switch (m_pMode->Get()) {
		case 0: { //Offensive
			if (m_bShouldShift && !m_bHasDoneShifting) {
				if (m_nTicksPassed < m_pRecharge->Get()) {
					*g_p_fGameSpeed = static_cast<double>(m_pTicksToShift->Get() * 1000.f);
					m_nTicksPassed++;
				} else {
					m_nTicksPassed = 0;
					m_bShouldShift = false;
					m_bHasDoneShifting = true;
				}
			}
		}
			break;
		case 1: { //Defensive
			if (m_bShouldShift && !m_bHasDoneShifting) {
				if (m_nTicksPassed < m_pRecharge->Get()) {
					*g_pfNextCmdTime = FLT_MAX;
					*g_p_fGameSpeed = static_cast<double>(m_pTicksToShift->Get() * 1000.f);
					m_nTicksPassed++;
					m_bCharging = true;
				} else {
					*g_pfNextCmdTime = -1.f;
					m_nTicksPassed = 0;
					m_bShouldShift = false;
					m_bCharging = false;
					m_bHasDoneShifting = true;
				}
			}
		}
			break;
		}
	}
}