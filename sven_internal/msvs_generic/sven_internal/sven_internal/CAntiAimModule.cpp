#include "StdAfx.h"
#include "CAntiAimModule.hpp"

CAntiAimModule::CAntiAimModule() : CModule("AntiAim", "Movement") {
	m_pVecValues->push_back(m_pJitter = Q_new(CBoolValue)("Jitter", "sc_antiaim_jitter", true));
	m_pVecValues->push_back(m_pJitterMin = Q_new(CIntegerValue)("Jitter - Minimum", "sc_antiaim_jitter_min", 1, 180, 15));
	m_pVecValues->push_back(m_pJitterMax = Q_new(CIntegerValue)("Jitter - Maximum", "sc_antiaim_jitter_max", 1, 180, 45));
	m_pVecValues->push_back(m_pRollAngle = Q_new(CFloatValue)("Roll angle", "sc_antiaim_roll_angle", -179.f, 179.f, 179.f));

	m_vecAntiAimAngles = g_vecZero;

	m_bStop = false;

	DefineConCmd("sc_lightning_antiaim", CAntiAimModule);
}

void CAntiAimModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CAntiAimModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

//TODO: rewrite me (21/04/2024, DD/MM/YYYY)
//still actual for 01/08/2024 (DD/MM/YYYY) - xWhitey
void CAntiAimModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		auto e = static_cast<const CUpdateEvent*>(_Event);

		if (!g_pPlayerMove
			|| g_pPlayerMove->movetype == MOVETYPE_FLY /* on ladder */
			|| g_pPlayerMove->iuser1 != 0 /* spectating */
			|| e->m_pCmd->buttons & (IN_USE | IN_ATTACK | IN_ATTACK2)
			|| e->m_pCmd->impulse == 201 /* spray */
			|| g_pPlayerMove->waterlevel > 1) {
			m_bStop = true;
			return;
		}

		m_bStop = false;

		m_vecAntiAimAngles[0] = m_pRollAngle->Get();
		if (m_pJitter->Get()) {
			m_vecAntiAimAngles[1] = e->m_pCmd->viewangles[1] - 180.f + rand() % (m_pJitterMax->Get()) + -(m_pJitterMin->Get());
		} else {
			m_vecAntiAimAngles[1] = e->m_pCmd->viewangles[1] - 180.f;
		}
		m_vecAntiAimAngles[2] = m_pRollAngle->Get();

		CUtils::SetRotation(e->m_pCmd, m_vecAntiAimAngles);
	}

	if (_Event->GetType() == EEventType::kMoveEvent) {
		if (m_bStop == true) return;

		//auto e = static_cast<const CMoveEvent*>(_Event);

		cl_entity_t* plr = g_pEngfuncs->GetLocalPlayer();

		float flPitch = CUtils::Clamp(-79.f, 79.f, m_vecAntiAimAngles.x / -3.0f);

		plr->angles.x = flPitch;
		plr->curstate.angles.x = flPitch;
		plr->prevstate.angles.x = flPitch;
		plr->latched.prevangles.x = flPitch;
	}
}