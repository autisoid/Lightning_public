/**
 * Copyright - xWhitey, 2024.
 * svenmod_emulation.cpp - actual implementation of the emulation system
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "svenmod_emulation.hpp"

bool g_bHasLoaded = false;

CLightning g_Lightning;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLightning, IClientPlugin, "ClientPlugin002", g_Lightning);

CLightning* g_pLightning = &g_Lightning;

api_version_t CLightning::GetAPIVersion() {
	return { (4), (0) };
}

bool CLightning::Load(CreateInterfaceFn _Factory, void* _SvenModAPI, void* _Unused) {
	if (g_bHasLoaded == false) {
		g_bHasLoaded = true;

		//todo: maybe add some svenmod specific code? (nah won't ever do this actually lol) ~ xWhitey
	}

	return true;
}

void CLightning::PostLoad(_In_ bool _GlobalLoad) {}

void CLightning::Unload() {
}

bool CLightning::Pause() {
	return false;
}

void CLightning::Unpause() {}

void CLightning::OnFirstClientdataReceived(void *pcldata, float flTime) {

}

void CLightning::OnBeginLoading() {

}

void CLightning::OnEndLoading() {

}

void CLightning::OnDisconnect() {

}

void CLightning::GameFrame(void*, double, bool) {}

PLUGIN_RESULT CLightning::Draw() {
	return PLUGIN_RESULT::PLUGIN_CONTINUE;
}

PLUGIN_RESULT CLightning::DrawHUD(float time, int intermission) {
	return PLUGIN_RESULT::PLUGIN_CONTINUE;
}

const char* CLightning::GetName() {
	return "Lightning";
}

const char* CLightning::GetAuthor() {
	return "xWhitey / ScriptedSnark / kolokola777 / oxiKKK";
}

const char* CLightning::GetVersion() {
	return CCheat::ms_pszVersion;
}

const char* CLightning::GetDescription() {
	return "an advanced utility mod for Sven Co-op providing gameplay enhances";
}

const char* CLightning::GetURL() {
	return "https://github.com/autisoid/Lightning";
}

const char* CLightning::GetDate() {
	return __TIMESTAMP__;
}

const char* CLightning::GetLogTag() {
	return "Lightning";
}

enum
{
	IFACE_OK = 0,
	IFACE_FAILED
};

EXTERN_C __declspec(dllexport) void* CreateInterface(_In_z_ _Pre_z_ const char* _pszName, _In_opt_ int* _pReturnCode) {
	InterfaceReg *pCur;

	for (pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext)
	{
		if (!strcmp(pCur->m_pName, _pszName))
		{
			if (_pReturnCode)
				*_pReturnCode = IFACE_OK;

			return pCur->m_CreateFn();
		}
	}

	if (_pReturnCode)
		*_pReturnCode = IFACE_FAILED;

	return NULL;
}