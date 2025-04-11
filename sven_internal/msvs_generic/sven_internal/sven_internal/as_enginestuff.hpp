/**
 * Copyright - xWhitey, 2024.
 * as_enginefuncs.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef AS_ENGINEFUNCS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in as_enginefuncs.hpp
#else //AS_ENGINEFUNCS_HPP_RECURSE_GUARD

#define AS_ENGINEFUNCS_HPP_RECURSE_GUARD

#ifndef AS_ENGINEFUNCS_HPP_GUARD
#define AS_ENGINEFUNCS_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CASEngineCommand {
	CASEngineCommand(const char* _Name, void* _Command);

	const char* m_pszName;
	void* m_pfnCommand;
} CASEngineCommand;

using CASEngineCommand = struct CASEngineCommand;

typedef struct CASEngineCommands {
	CASEngineCommands();
	void ExecuteCommand();
	void UnregisterCommands();

	CVector<CASEngineCommand*>* m_pCommands;
} CASEngineCommands;

using CASEngineCommands = struct CASEngineCommands;

typedef struct CASClientSprite {
	static char __cdecl GetNameSingleChar(struct client_sprite_s* _ThisInstance, int _Index);
	static char __cdecl GetSpriteSingleChar(struct client_sprite_s* _ThisInstance, int _Index);
} CASClientSprite;

using CASClientSprite = struct CASClientSprite;

typedef struct CASRect {
	CASRect();
	CASRect(const int& _Left, const int& _Right, const int& _Top, const int& _Bottom);

	void AddRef();
	void Release();

	int left, right, top, bottom;

	int m_nRefCount;
} CASRect;

using CASRect = struct CASRect;

typedef struct CASScreenInfo {
	static short __cdecl GetCharWidthsSingleShort(SCREENINFO* _ScreenInfo, int _Index);
} CASScreenInfo;

using CASScreenInfo = struct CASScreenInfo;

typedef struct CASEngineFuncs {
	HSPRITE_HL SPR_Load(const std::string& szPicName);
	int SPR_Frames(HSPRITE_HL hPic);
	int SPR_Height(HSPRITE_HL hPic, int frame);
	int SPR_Width(HSPRITE_HL hPic, int frame);
	void SPR_Set(HSPRITE_HL hPic, int r, int g, int b);
	void SPR_Draw(int frame, int x, int y, const CASRect& prc);
	void SPR_DrawHoles(int frame, int x, int y, const CASRect& prc);
	void SPR_DrawAdditive(int frame, int x, int y, const CASRect& prc);
	void SPR_EnableScissor(int x, int y, int width, int height);
	void SPR_DisableScissor();
	struct client_sprite_s* SPR_GetList(const std::string& psz, int& piCount);
	void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a);
	int GetScreenInfo(SCREENINFO& _ScreenInfo);
	// TODO: Implement

	void __thiscall Cmd_AddCommand(const std::string& _Name, void* _Function);
	void __thiscall Con_Printf(const std::string& _Fmt);
} CASEngineFuncs;

using CASEngineFuncs = struct CASEngineFuncs;

typedef struct CASGlobals {
	static playermove_s* __cdecl GetPlayerMove();
	static ref_params_s* __cdecl GetRefParams();
	static movevars_s* __cdecl GetMovevars();
	static usercmd_s* __cdecl GetUserCmd();
} CASGlobals;

using CASGlobals = struct CASGlobals;

typedef struct CASPlayerMove {
	static char __cdecl GetTextureNameSingleChar(playermove_s* _PlayerMove, int _Index);
	static physent_t* __cdecl GetPhysicalEntityAtIndex(playermove_s* _PlayerMove, int _Index);
	static physent_t* __cdecl GetMovementEntityAtIndex(playermove_s* _PlayerMove, int _Index);
	static physent_t* __cdecl GetVisEntityAtIndex(playermove_s* _PlayerMove, int _Index);
	static pmtrace_t* __cdecl GetTouchIndexAtIndex(playermove_s* _PlayerMove, int _Index);
	static char __cdecl GetPhysInfoSingleChar(playermove_s* _PlayerMove, int _Index);
	static CASVector* __cdecl GetPlayerMinsAtIndex(playermove_s* _PlayerMove, int _Index);
	static CASVector* __cdecl GetPlayerMaxsAtIndex(playermove_s* _PlayerMove, int _Index);
	static int __cdecl GetNumPhysEnt(playermove_s* _PlayerMove);
	static void __cdecl SetNumPhysEnt(playermove_s* _PlayerMove, int _Value);
	static int __cdecl GetNumMoveEnt(playermove_s* _PlayerMove);
	static void __cdecl SetNumMoveEnt(playermove_s* _PlayerMove, int _Value);
	static int __cdecl GetNumVisEnt(playermove_s* _PlayerMove);
	static void __cdecl SetNumVisEnt(playermove_s* _PlayerMove, int _Value);
	static movevars_s* __cdecl GetMovementVariables(playermove_s* _PlayerMove);
	static usercmd_s* __cdecl GetUserCmd(playermove_s* _PlayerMove);
	static int __cdecl GetNumTouch(playermove_s* _PlayerMove);
	static void __cdecl SetNumTouch(playermove_s* _PlayerMove, int _Value);
} CASPlayerMove;

using CASPlayerMove = struct CASPlayerMove;

typedef struct CASPhysicalEntity {
	static char __cdecl GetNameSingleChar(physent_t* _ThisInstance, int _Index);
	static byte __cdecl GetControllerAtIndex(physent_t* _ThisInstance, int _Index);
	static byte __cdecl GetBlendingAtIndex(physent_t* _ThisInstance, int _Index);
} CASPhysicalEntity;

using CASPhysicalEntity = struct CASPhysicalEntity;

typedef struct CASMovementVariables {
	static char __cdecl GetSkyNameSingleChar(movevars_t* _ThisInstance, int _Index);
} CASMovementVariables;

using CASMovementVariables = struct CASMovementVariables;

typedef struct CASRefParams {
	static int __cdecl GetViewPortAtIndex(ref_params_t* _ThisInstance, int _Index);
} CASRefParams;

using CASRefParams = struct CASRefParams;

#else //!__cplusplus
#error C++ compiler required to compile as_enginefuncs.hpp
#endif //__cplusplus

#endif //AS_ENGINEFUNCS_HPP_GUARD

#undef AS_ENGINEFUNCS_HPP_RECURSE_GUARD
#endif //AS_ENGINEFUNCS_HPP_RECURSE_GUARD