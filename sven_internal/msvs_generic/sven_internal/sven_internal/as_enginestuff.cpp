#include "StdAfx.h"
#include "as_vector.hpp"
#include "as_enginestuff.hpp"
#include "as_callroutines.hpp"

CASRect* CASRectFactory(const int& _Left, const int& _Right, const int& _Top, const int& _Bottom) {
	return Q_new(CASRect)(_Left, _Right, _Top, _Bottom);
}

CASRect* CASRectFactory() {
	return Q_new(CASRect)();
}

CASRect::CASRect() {
	left = right = top = bottom = 0;
	m_nRefCount = 0;
}

CASRect::CASRect(const int& _Left, const int& _Right, const int& _Top, const int& _Bottom) : left(_Left), right(_Right), top(_Top), bottom(_Bottom) {
	m_nRefCount = 0;
}

void CASRect::AddRef() {
	m_nRefCount++;
}

void CASRect::Release() {
	if (--m_nRefCount == 0)
		Q_delete(this);
}

CASEngineCommand::CASEngineCommand(const char* _Name, void* _Command) : m_pszName(_strdup(_Name)), m_pfnCommand(_Command) {}
CASEngineCommands::CASEngineCommands() {
	m_pCommands = Q_new(CVector<CASEngineCommand*>)({});
}

void CASEngineCommands::ExecuteCommand() {
	if (!g_pEngfuncs) return;
	if (!m_pCommands) return;
	const char* pCommandName = g_pEngfuncs->Cmd_Argv(0);
	if (!pCommandName || pCommandName[0] == '\0') return;
	CASEngineCommand** ppCommand = nullptr;
	CASEngineCommand* pFound = nullptr;
	for (ppCommand = m_pCommands->begin(); ppCommand != m_pCommands->end(); ppCommand++) {
		if (!ppCommand || !(*ppCommand)) continue;
		CASEngineCommand* pCurrent = *ppCommand;
		size_t sNameLength = Q_strlen(pCurrent->m_pszName);
		if (!Q_strnicmp_s(pCurrent->m_pszName, sNameLength + 1, pCommandName, sNameLength)) {
			pFound = pCurrent;
			break;
		}
	}
	if (!pFound) return;
	g_Callstack.Push(pFound->m_pfnCommand);
	CASCallRoutines::CallStub();
}

void CASEngineCommands::UnregisterCommands() {
	CASEngineCommand** ppCommand = nullptr;
	for (ppCommand = m_pCommands->begin(); ppCommand != m_pCommands->end(); ppCommand++) {
		if (!ppCommand || !(*ppCommand)) continue;
		CASEngineCommand* pCurrent = *ppCommand;
		Q_delete(pCurrent);
	}
	m_pCommands->resize(0);
	//Q_delete(m_pCommands);
	//m_pCommands = Q_new(CVector<CASEngineCommand*>)({});
}

void ASEngineCommands_Execute() {
	CCheat::GetCheat()->m_pTheScripts->m_pEngineCommands->ExecuteCommand();
}

char __cdecl CASClientSprite::GetNameSingleChar(client_sprite_s* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->szName)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->szName[_Index];
}

char __cdecl CASClientSprite::GetSpriteSingleChar(client_sprite_s* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->szSprite)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->szSprite[_Index];
}

short __cdecl CASScreenInfo::GetCharWidthsSingleShort(SCREENINFO* _ScreenInfo, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ScreenInfo->charWidths)) {
		return 0;
	}
	if (_Index < 0) {
		return 0;
	}

	return _ScreenInfo->charWidths[_Index];
}

HSPRITE_HL CASEngineFuncs::SPR_Load(const std::string& szPicName) {
	if (!g_pEngfuncs) return 0;
	return g_pEngfuncs->pfnSPR_Load(szPicName.c_str());
}
int CASEngineFuncs::SPR_Frames(HSPRITE_HL hPic) {
	if (!g_pEngfuncs) return 0;
	return g_pEngfuncs->pfnSPR_Frames(hPic);
}
int CASEngineFuncs::SPR_Height(HSPRITE_HL hPic, int frame) {
	if (!g_pEngfuncs) return 0;
	return g_pEngfuncs->pfnSPR_Height(hPic, frame);
}
int CASEngineFuncs::SPR_Width(HSPRITE_HL hPic, int frame) {
	if (!g_pEngfuncs) return 0;
	return g_pEngfuncs->pfnSPR_Width(hPic, frame);
}
void CASEngineFuncs::SPR_Set(HSPRITE_HL hPic, int r, int g, int b) {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnSPR_Set(hPic, r, g, b);
}
void CASEngineFuncs::SPR_Draw(int frame, int x, int y, const CASRect& prc) {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnSPR_Draw(frame, x, y, (const rect_s*) &prc);
}
void CASEngineFuncs::SPR_DrawHoles(int frame, int x, int y, const CASRect& prc) {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnSPR_DrawHoles(frame, x, y, (const rect_s*)&prc);
}
void CASEngineFuncs::SPR_DrawAdditive(int frame, int x, int y, const CASRect& prc) {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnSPR_DrawAdditive(frame, x, y, (const rect_s*)&prc);
}
void CASEngineFuncs::SPR_EnableScissor(int x, int y, int width, int height) {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnSPR_EnableScissor(x, y, width, height);
}
void CASEngineFuncs::SPR_DisableScissor() {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnSPR_DisableScissor();
}
struct client_sprite_s* CASEngineFuncs::SPR_GetList(const std::string& psz, int& piCount) {
	if (!g_pEngfuncs) return 0;
	return g_pEngfuncs->pfnSPR_GetList(const_cast<char*>(psz.c_str()), &piCount);
}
void CASEngineFuncs::FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a) {
	if (!g_pEngfuncs) return;
	return g_pEngfuncs->pfnFillRGBA(x, y, width, height, r, g, b, a);
}

int CASEngineFuncs::GetScreenInfo(SCREENINFO& _ScreenInfo) {
	if (!g_pEngfuncs) return 0;
	return g_pEngfuncs->pfnGetScreenInfo(&_ScreenInfo);
}

void __thiscall CASEngineFuncs::Cmd_AddCommand(const std::string& _Name, void* _Function) {
	if (!g_pEngfuncs) return;
	CASEngineCommand* pCommand = Q_new(CASEngineCommand)(_Name.c_str(), _Function);
	CCheat::GetCheat()->m_pTheScripts->m_pEngineCommands->m_pCommands->push_back(pCommand);
	g_pEngfuncs->pfnAddCommand(const_cast<char*>(pCommand->m_pszName), ASEngineCommands_Execute);
}

void __thiscall CASEngineFuncs::Con_Printf(const std::string& _Fmt) {
	g_pEngfuncs->Con_Printf(const_cast<char*>(_Fmt.c_str()));
}

/*void __cdecl CASEngineFuncs::Con_Printf(void* _Generic) {
	if (!g_pEngfuncs) return;
	if (!_Generic) return;
	asIScriptGeneric* pGeneric = (asIScriptGeneric*) _Generic;
	switch (pGeneric->GetArgCount()) {
	case 1: {
		std::string str = *(std::string*)(pGeneric->GetArgObject(0));
		g_pEngfuncs->Con_Printf(const_cast<char*>(str.c_str()));
	}
		break;
	//TODO: Finish
	case 2: {
		std::string str = *(std::string*)(pGeneric->GetArgObject(0));
		int iTypeId = pGeneric->GetArgTypeId(1);
		asITypeInfo* pTypeInfo = CCheat::GetCheat()->m_pTheScripts->m_pScriptEngine->GetTypeInfoById(iTypeId);
		g_pEngfuncs->Con_Printf(const_cast<char*>(str.c_str()));
	}
		  break;
	}
}*/

playermove_s* __cdecl CASGlobals::GetPlayerMove() {
	return g_pPlayerMove;
}

ref_params_s* __cdecl CASGlobals::GetRefParams() {
	return g_pRefParams;
}

movevars_s* __cdecl CASGlobals::GetMovevars() {
	return g_pMoveVars;
}

usercmd_s* __cdecl CASGlobals::GetUserCmd() {
	return g_pCmd;
}

char __cdecl CASPlayerMove::GetTextureNameSingleChar(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->sztexturename)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _PlayerMove->sztexturename[_Index];
}

physent_t* __cdecl CASPlayerMove::GetPhysicalEntityAtIndex(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->physents)) {
		return NULL;
	}
	if (_Index < 0) {
		return NULL;
	}

	return &_PlayerMove->physents[_Index];
}

physent_t* __cdecl CASPlayerMove::GetMovementEntityAtIndex(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->moveents)) {
		return NULL;
	}
	if (_Index < 0) {
		return NULL;
	}

	return &_PlayerMove->moveents[_Index];
}

physent_t* __cdecl CASPlayerMove::GetVisEntityAtIndex(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->visents)) {
		return NULL;
	}
	if (_Index < 0) {
		return NULL;
	}

	return &_PlayerMove->visents[_Index];
}

pmtrace_t* __cdecl CASPlayerMove::GetTouchIndexAtIndex(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->touchindex)) {
		return NULL;
	}
	if (_Index < 0) {
		return NULL;
	}

	return &_PlayerMove->touchindex[_Index];
}

char __cdecl CASPlayerMove::GetPhysInfoSingleChar(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->physinfo)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _PlayerMove->physinfo[_Index];
}

CASVector* __cdecl CASPlayerMove::GetPlayerMinsAtIndex(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->player_mins)) {
		return NULL;
	}
	if (_Index < 0) {
		return NULL;
	}

	return (CASVector*) &_PlayerMove->player_mins[_Index];
}

CASVector* __cdecl CASPlayerMove::GetPlayerMaxsAtIndex(playermove_s* _PlayerMove, int _Index) {
	if (_Index > Q_ARRAYSIZE(_PlayerMove->player_maxs)) {
		return NULL;
	}
	if (_Index < 0) {
		return NULL;
	}

	return (CASVector*)&_PlayerMove->player_maxs[_Index];
}

int __cdecl CASPlayerMove::GetNumPhysEnt(playermove_s* _PlayerMove) {
	return _PlayerMove->numphysent;
}

int __cdecl CASPlayerMove::GetNumMoveEnt(playermove_s* _PlayerMove) {
	return _PlayerMove->nummoveent;
}

int __cdecl CASPlayerMove::GetNumVisEnt(playermove_s* _PlayerMove) {
	return _PlayerMove->numvisent;
}

void __cdecl CASPlayerMove::SetNumPhysEnt(playermove_s* _PlayerMove, int _Value) {
	_PlayerMove->numphysent = _Value;
}

void __cdecl CASPlayerMove::SetNumMoveEnt(playermove_s* _PlayerMove, int _Value) {
	_PlayerMove->nummoveent = _Value;
}

void __cdecl CASPlayerMove::SetNumVisEnt(playermove_s* _PlayerMove, int _Value) {
	_PlayerMove->numvisent = _Value;
}

movevars_s* __cdecl CASPlayerMove::GetMovementVariables(playermove_s* _PlayerMove) {
	return _PlayerMove->movevars;
}

usercmd_s* __cdecl CASPlayerMove::GetUserCmd(playermove_s* _PlayerMove) {
	return &_PlayerMove->cmd;
}

int __cdecl CASPlayerMove::GetNumTouch(playermove_s* _PlayerMove) {
	return _PlayerMove->numtouch;
}

void __cdecl CASPlayerMove::SetNumTouch(playermove_s* _PlayerMove, int _Value) {
	_PlayerMove->numtouch = _Value;
}

char __cdecl CASPhysicalEntity::GetNameSingleChar(physent_t* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->name)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->name[_Index];
}

byte __cdecl CASPhysicalEntity::GetControllerAtIndex(physent_t* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->controller)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->controller[_Index];
}

byte __cdecl CASPhysicalEntity::GetBlendingAtIndex(physent_t* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->blending)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->blending[_Index];
}

char __cdecl CASMovementVariables::GetSkyNameSingleChar(movevars_t* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->skyName)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->skyName[_Index];
}

int __cdecl CASRefParams::GetViewPortAtIndex(ref_params_s* _ThisInstance, int _Index) {
	if (_Index > Q_ARRAYSIZE(_ThisInstance->viewport)) {
		return '\0';
	}
	if (_Index < 0) {
		return '\0';
	}

	return _ThisInstance->viewport[_Index];
}