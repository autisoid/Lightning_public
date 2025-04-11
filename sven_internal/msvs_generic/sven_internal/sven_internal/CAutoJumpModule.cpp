#include "StdAfx.h"
#include "CAutoJumpModule.hpp"

CAutoJumpModule::CAutoJumpModule() : CModule("AutoJump", "Movement") {
	DefineConCmd("sc_lightning_autojump", CAutoJumpModule);
}

void CAutoJumpModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CAutoJumpModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CAutoJumpModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_pPlayerMove) return;

		auto e = static_cast<const CUpdateEvent*>(_Event);

        //I don't really like that condition...
		if (/*(g_pPlayerMove->waterlevel < 1) && nope */!(g_pPlayerMove->movetype == MOVETYPE_FLY) && 
			(g_pPlayerMove->iuser1 == 0 /* not spectating */) && !(g_pPlayerMove->flags & FL_ONGROUND) && (e->m_pCmd->buttons & IN_JUMP) != 0) {
			e->m_pCmd->buttons &= ~IN_JUMP;
		}
	}
}