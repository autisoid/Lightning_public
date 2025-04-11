#include "StdAfx.h"
#include "CFrameSkipperModule.hpp"

//Look into HwDLL.cpp for implementation (HOOKED_SCR_UpdateScreen)
//TODO: oxi told me that there's another way to boost fps by stopping the world from being rendered each Nth frame - investigate. - xWhitey

CFrameSkipperModule* g_pFrameSkipperModule;

CFrameSkipperModule::CFrameSkipperModule() : CModule("FrameSkipper", "Render") {
	g_pFrameSkipperModule = this;

	DefineConCmd("sc_lightning_frameskipper", CFrameSkipperModule);
}

void CFrameSkipperModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CFrameSkipperModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CFrameSkipperModule::OnEvent(_In_ const ISimpleEvent* _Event) {}