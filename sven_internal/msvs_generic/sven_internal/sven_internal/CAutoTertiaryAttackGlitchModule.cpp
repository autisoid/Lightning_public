#include "StdAfx.h"
#include "CAutoTertiaryAttackGlitchModule.hpp"

//A module mainly used by myself on Constantium and Autism servers - automatically remaps RMB to MMB after N ticks so we have gaussbug!
//Wow. This comment is so really old damn. It was probably written back in October, 2023 because the OG HLA server closed back then IIRC. (xWhitey ~ 01/03/2025 ~ MM/DD/YYYY)
CAutoTertiaryAttackGlitchModule::CAutoTertiaryAttackGlitchModule() : CModule("AutoTertiaryAttackGlitch", "Misc") {
	m_pVecValues->push_back(m_pWaitTicksBeforeDoingGlitch = Q_new(CIntegerValue)("Wait ticks before doing the glitch", "sc_autotertiaryattackglitch_wait_count", 8, 256, 16));

	m_bAllowedToDoGlitch = false;
	m_bHasSwitchedWeaponsUsingLastinv = false;
	m_bPreviousWeaponWasGauss = false;
	m_bTotallySure = false; 
	m_bWaitingForWeaponAnimation = false;
	m_iWaitCount = 0;
	m_iTicksWaitedAfterWeaponHasBeenSelected = 0;

	DefineConCmd("sc_lightning_autotertiaryattackglitch", CAutoTertiaryAttackGlitchModule);
}

void CAutoTertiaryAttackGlitchModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CAutoTertiaryAttackGlitchModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CAutoTertiaryAttackGlitchModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		auto e = static_cast<const CUpdateEvent*>(_Event);

		if (m_bAllowedToDoGlitch == false) {
			cl_entity_t* viewmodel = g_pEngfuncs->GetViewModel();
			if (!viewmodel)
				return;

			char* szModelName = viewmodel->model->name;
			if (!szModelName)
				return;

			if (strstr(szModelName, "gauss"))
				m_bAllowedToDoGlitch = m_bTotallySure = true;
		}

		if (m_bHasSwitchedWeaponsUsingLastinv == true) {
			m_bHasSwitchedWeaponsUsingLastinv = false;

			if (m_bPreviousWeaponWasGauss == true) {
				m_bAllowedToDoGlitch = m_bPreviousWeaponWasGauss = false;
				return;
			}
		}

		if (m_bAllowedToDoGlitch == false) return;

		if (e->m_pCmd->buttons & IN_ATTACK2) {
			if (m_iWaitCount < m_pWaitTicksBeforeDoingGlitch->Get()) {
				m_iWaitCount++;
				return;
			} else {
				if (m_bTotallySure == true) {
					cl_entity_t* viewmodel = g_pEngfuncs->GetViewModel();
					if (!viewmodel)
						return;

					char* szModelName = viewmodel->model->name;
					if (!szModelName)
						return;

					if (!strstr(szModelName, "gauss"))
						m_bAllowedToDoGlitch = m_bTotallySure = false;
				}

				if (m_bTotallySure == false) return;
				if (e->m_pCmd->weaponselect) {
					m_bWaitingForWeaponAnimation = true;
				}

				if (m_bWaitingForWeaponAnimation == true && m_iTicksWaitedAfterWeaponHasBeenSelected < m_pWaitTicksBeforeDoingGlitch->Get()) {
					m_iTicksWaitedAfterWeaponHasBeenSelected++;
					return;
				} else {
					m_iTicksWaitedAfterWeaponHasBeenSelected = 0;
					m_bWaitingForWeaponAnimation = false;
				}

				e->m_pCmd->buttons &= ~IN_ATTACK2;
				e->m_pCmd->buttons |= IN_ALT1;
			}
		} else {
			m_iWaitCount = 0;
		}
	}

	if (_Event->GetType() == EEventType::kWeaponChangedEvent) {
		auto e = static_cast<const CWeaponChangedEvent*>(_Event);

		m_bAllowedToDoGlitch = e->m_iWeaponId == WEAPON_GAUSS ? true : false;
	}

	if (_Event->GetType() == EEventType::kCommandIssuedEvent) {
		auto e = static_cast<const CCommandIssuedEvent*>(_Event);

		if (!_strnicmp(e->m_pszText, "lastinv", sizeof("lastinv") - 1)) {
			m_bHasSwitchedWeaponsUsingLastinv = true;
			cl_entity_t* viewmodel = g_pEngfuncs->GetViewModel();
			if (!viewmodel)
				return;

			char* szModelName = viewmodel->model->name;
			if (!szModelName)
				return;

			if (strstr(szModelName, "gauss"))
				m_bPreviousWeaponWasGauss = true;
		} else if (!_strnicmp(e->m_pszText, "weapon_", sizeof("weapon_") - 1) && strstr(e->m_pszText, /* weapon_gauss, weapon_hldm_gauss, weapon_clgauss */ "gauss")) {
			m_bAllowedToDoGlitch = true;
		}
	}
}