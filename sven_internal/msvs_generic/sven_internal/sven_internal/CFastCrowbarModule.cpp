#include "StdAfx.h"
#include "CFastCrowbarModule.hpp"
#include "CSpeedModule.hpp"
#include "HwDLL.hpp"

CFastCrowbarModule* g_pFastCrowbarModule;

int HOOKED_CurWeapon_UserMsg(const char* pszName, int iSize, void* pbuf) {
	int iReadCount = 0;
	byte* buf = (byte*)pbuf;

	int iState = ((unsigned char)buf[iReadCount]);
	iReadCount++;

	int iId = (short)(buf[iReadCount] + (buf[iReadCount + 1] << 8));
	iReadCount += 2; //is it really needed?

	if (iState) {
		CWeaponChangedEvent* e = Q_new(CWeaponChangedEvent)(iState, iId);
		CCheat::GetCheat()->m_pEventBus->Post(e);
		Q_delete(e);
	} else if (iId <= WEAPON_NONE) {
		CWeaponChangedEvent* e = Q_new(CWeaponChangedEvent)(iState, WEAPON_NONE);
		CCheat::GetCheat()->m_pEventBus->Post(e);
		Q_delete(e);
	}

	return ORIG_CurWeapon_UserMsg(pszName, iSize, pbuf);
}

CFastCrowbarModule::CFastCrowbarModule() : CModule("FastCrowbar", "Exploit") {
	m_pVecValues->push_back(m_pChargeFor = Q_new(CIntegerValue)("Charge for", "sc_fastcrowbar_charge_for", 25, 256, 45));
	m_pVecValues->push_back(m_pBlockMovements = Q_new(CBoolValue)("Block movements", "sc_fastcrowbar_block_movements", true));
	m_iChargedFor = 0;
	m_bAllowedToCharge = false;
	m_bHasSwitchedWeaponsUsingLastinv = false;
	m_bPreviousWeaponWasCrowbar = false; 
	
	g_pFastCrowbarModule = this;
	g_pSpeedModule = CCheat::GetCheat()->m_pModuleManager->GetModule<CSpeedModule>();

	DefineConCmd("sc_lightning_fastcrowbar", CFastCrowbarModule);
}

void CFastCrowbarModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CFastCrowbarModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);

	*g_pfNextCmdTime = -1.f;
	CModule::OnDisable();
}

//I think this module has some problems with checks if it's a crowbar or no... - xWhitey
void CFastCrowbarModule::OnEvent(const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		auto e = static_cast<const CUpdateEvent*>(_Event);

		if (!g_p_fGameSpeed || !g_pPlayerMove) {
			return;
		}

		if (((((g_pPlayerMove->oldbuttons & IN_ATTACK) && !(e->m_pCmd->buttons & IN_ATTACK))) || !(e->m_pCmd->buttons & IN_ATTACK)) && m_bAllowedToCharge == true) {
			*g_pfNextCmdTime = -1.f;
			m_iChargedFor = 0;
			return;
		}

		if (g_pSpeedModule->m_bState == true) {
			return;
		}

		if (m_bAllowedToCharge == false) {
			cl_entity_t* viewmodel = g_pEngfuncs->GetViewModel();
			if (!viewmodel)
				return;

			char* szModelName = viewmodel->model->name;
			if (!szModelName)
				return;

			if (strstr(szModelName, "crowbar") || strstr(szModelName, "knife") || strstr(szModelName, "wrench") || strstr(szModelName, "shovel") || strstr(szModelName, "pitchfork"))
				m_bAllowedToCharge = true;
		}

		if (m_bHasSwitchedWeaponsUsingLastinv == true) {
			m_bHasSwitchedWeaponsUsingLastinv = false;

			if (m_bPreviousWeaponWasCrowbar == true) {
				m_bAllowedToCharge = m_bPreviousWeaponWasCrowbar = false;
			}
		}
		else if (m_bPreviousWeaponWasCrowbar == true) {
			m_bAllowedToCharge = m_bPreviousWeaponWasCrowbar = false;
		}

		//Let's check twice we're holding attack button
		if (((((g_pPlayerMove->oldbuttons & IN_ATTACK) && !(e->m_pCmd->buttons & IN_ATTACK))) || !(e->m_pCmd->buttons & IN_ATTACK)) && m_bAllowedToCharge == true) {
			*g_pfNextCmdTime = -1.f;
			m_iChargedFor = 0;
			return;
		}

		if (m_bAllowedToCharge == false) {
			*g_pfNextCmdTime = -1.f;
			m_iChargedFor = 0;
			return;
		}

		if (m_iChargedFor < m_pChargeFor->Get()) {
			*g_pfNextCmdTime = FLT_MAX;
			m_iChargedFor++;
		}	else {
			*g_pfNextCmdTime = -1.f;
			m_iChargedFor = 0;
		}

		if (m_pBlockMovements->Get()) {
			e->m_pCmd->forwardmove = 0;
			e->m_pCmd->sidemove = 0;
		}
		*g_p_fGameSpeed = static_cast<double>(m_pChargeFor->Get()) * 1000.0;
	}

	if (_Event->GetType() == EEventType::kWeaponChangedEvent) {
		auto e = static_cast<const CWeaponChangedEvent*>(_Event);

		if (e->m_iWeaponId == WEAPON_CROWBAR || e->m_iWeaponId == WEAPON_WRENCH) {
			m_bAllowedToCharge = true;
		} else {
			m_bAllowedToCharge = false;
			m_iChargedFor = 0;
			*g_pfNextCmdTime = -1.f;
		}
	}

	if (_Event->GetType() == EEventType::kCommandIssuedEvent) {
		auto e = static_cast<const CCommandIssuedEvent*>(_Event);
		if (!_strnicmp(e->m_pszText, "weapon_", sizeof("weapon_") - 1) && (strstr(e->m_pszText, /* weapon_crowbar, weapon_clcrowbar */ "crowbar") || strstr(e->m_pszText, "knife") || strstr(e->m_pszText, "wrench") || strstr(e->m_pszText, "shovel") || strstr(e->m_pszText, "pitchfork"))) {
			m_bAllowedToCharge = true;
		}
		if (!_strnicmp(e->m_pszText, "lastinv", sizeof("lastinv") - 1)) {
			m_bHasSwitchedWeaponsUsingLastinv = true;
			cl_entity_t* viewmodel = g_pEngfuncs->GetViewModel();
			if (!viewmodel)
				return;

			char* szModelName = viewmodel->model->name;
			if (!szModelName)
				return;

			if (strstr(szModelName, "crowbar") || strstr(szModelName, "knife") || strstr(szModelName, "wrench") || strstr(szModelName, "shovel") || strstr(szModelName, "pitchfork"))
				m_bPreviousWeaponWasCrowbar = true;
		}
	}
}