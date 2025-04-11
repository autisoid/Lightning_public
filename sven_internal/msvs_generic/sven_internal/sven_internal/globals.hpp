/**
 * Copyright - xWhitey, 2023.
 * globals.hpp - description
 */

#ifdef GLOBALS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in globals.hpp
#else //GLOBALS_HPP_RECURSE_GUARD

#define GLOBALS_HPP_RECURSE_GUARD

#ifndef GLOBALS_HPP_GUARD
#define GLOBALS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "common.hpp"
#include "preprocessor.hpp"

typedef BOOL(WINAPI* _GetMessage)(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax);
typedef BOOL(WINAPI* _PeekMessage)(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax, _In_ IN UINT _RemoveMsg);
typedef BOOL(WINAPI* _PostMessage)(_In_ IN HWND _hWnd, _In_ IN unsigned int _Msg, _In_ IN WPARAM _wParam, _In_ IN LPARAM _param);
typedef BOOL(WINAPI* _ClipCursor)(_Inout_ IN OUT const LPRECT* _Rect);
typedef BOOL(WINAPI* _SetCursorPos)(_In_ IN int _X, _In_ IN int _Y);
typedef BOOL(WINAPI* _GetCursorPos)(_Out_ OUT LPPOINT _Point);
_GetMessage ORIG_GetMessageA;
_GetMessage ORIG_GetMessageW;
_PeekMessage ORIG_PeekMessageA;
_PeekMessage ORIG_PeekMessageW;
_PostMessage ORIG_PostMessageA;
_PostMessage ORIG_PostMessageW;
_ClipCursor ORIG_ClipCursor;
_SetCursorPos ORIG_SetCursorPos;
_GetCursorPos ORIG_GetCursorPos;

extern HHOOK g_pHook;

extern void* g_pSelfHandle;

extern int g_iLastMouseX;
extern int g_iLastMouseY;

extern void* g_pHwDll;
extern void* g_pClientDll;
extern cl_enginefunc_t* g_pEngfuncs;
extern playermove_s* g_pPlayerMove;
extern ref_params_s* g_pRefParams;
extern usercmd_s* g_pCmd;

extern WNDPROC ORIG_wndproc;
extern ImFont* g_pClientFont;
extern bool g_bForceImGuiToNotDrawAnything;

typedef BOOL(WINAPI* _QueryPerformanceCounter)(LARGE_INTEGER*);
extern _QueryPerformanceCounter ORIG_QueryPerformanceCounter;

#ifndef LIGHTNING_TO_MINT_MC
extern void* g_pNetchanMessage;
extern float* g_pfNextCmdTime;
extern client_static_t* g_pClientStatic;
extern client_state_t* g_pClientState;
extern int* g_piIncomingSequence;
extern int* g_piOutgoingSequence;
extern int* g_piIncomingReliableSequence;
extern double* g_pdblClientTime;
extern float g_flFrametime;
extern int g_iActive;
extern int* g_piLastOutgoingCmd;
extern double* g_p_fGameSpeed;
extern cvar_t* sv_accelerate;
extern cvar_t* sv_airaccelerate;
extern cvar_t* sv_bounce;
extern cvar_t* sv_friction;
extern cvar_t* edgefriction;
extern cvar_t* sv_gravity;
extern cvar_t* sv_stepsize;
extern cvar_t* sv_maxvelocity;
extern cvar_t* sv_stopspeed;
extern cvar_t* sc_lightning_dont_send_sendents;

extern ImColor g_DefaultClientColor;

typedef int(__cdecl* _SZ_GetSpace)(void*, int);
extern _SZ_GetSpace ORIG_SZ_GetSpace;

typedef void(__cdecl* _CL_CreateMove)(FLOAT, usercmd_s*, INT);
extern _CL_CreateMove ORIG_CL_CreateMove;
typedef void(__cdecl* _V_CalcRefdef)(ref_params_s*);
extern _V_CalcRefdef ORIG_V_CalcRefdef;
typedef void(__cdecl* _HUD_PlayerMove)(playermove_s*, qboolean);
extern _HUD_PlayerMove ORIG_HUD_PlayerMove;
typedef void(__cdecl* _HUD_PostRunCmd)(struct local_state_s*, struct local_state_s*, usercmd_s*, int, double, unsigned int);
extern _HUD_PostRunCmd ORIG_HUD_PostRunCmd;
typedef void(__cdecl* _Netchan_TransmitBits)(netchan_s*, int, char*);
extern _Netchan_TransmitBits ORIG_Netchan_TransmitBits;
typedef int(__cdecl* _SCR_UpdateScreen)();
extern _SCR_UpdateScreen ORIG_SCR_UpdateScreen;
extern int __cdecl HOOKED_CurWeapon_UserMsg(const char* pszName, int iSize, void* pbuf);
extern int __cdecl HOOKED_SayText_UserMsg(const char* pszName, int iSize, void* pbuf);
extern usermsg_t** g_alpUserMsgs;
typedef int(__cdecl* _CurWeapon_UserMsg)(const char*, int, void*);
extern _CurWeapon_UserMsg ORIG_CurWeapon_UserMsg;
extern _CurWeapon_UserMsg ORIG_SayText_UserMsg;
typedef void(__cdecl* _Cbuf_InsertText)(const char*);
extern _Cbuf_InsertText ORIG_Cbuf_InsertText;
extern _Cbuf_InsertText ORIG_Cbuf_AddText;
typedef void(__cdecl* _CL_CameraOffset)(float*);
extern _CL_CameraOffset ORIG_CL_CameraOffset;
typedef qboolean(__cdecl* _CL_IsThirdPerson)();
extern _CL_IsThirdPerson ORIG_CL_IsThirdPerson;
typedef void(__cdecl* _IN_ActivateMouse)();
extern _IN_ActivateMouse ORIG_IN_ActivateMouse;
typedef BOOL(__cdecl* _IN_DeactivateMouse)();
extern _IN_DeactivateMouse ORIG_IN_DeactivateMouse;
typedef void(__cdecl* _SPR_Set)(HSPRITE_HL, int, int, int);
extern _SPR_Set ORIG_SPR_Set;
typedef void(__cdecl* _Draw_FillRGBA)(int, int, int, int, int, int, int, int);
extern _Draw_FillRGBA ORIG_Draw_FillRGBA;
typedef void(__cdecl* _MSG_WriteUsercmd)(sizebuf_t*, usercmd_s*, usercmd_s*);
extern _MSG_WriteUsercmd ORIG_MSG_WriteUsercmd;
typedef void(__cdecl* _HUD_Redraw)(float, int);
extern _HUD_Redraw ORIG_HUD_Redraw;
typedef int(__cdecl* _R_StudioDrawPlayer)(int, entity_state_t*);
extern _R_StudioDrawPlayer ORIG_R_StudioDrawPlayer;
typedef int(__cdecl* _HUD_AddEntity)(int, cl_entity_s*, const char*);
extern _HUD_AddEntity ORIG_HUD_AddEntity;
typedef void(__cdecl* _SCR_EndLoadingPlaque)();
extern _SCR_EndLoadingPlaque ORIG_SCR_EndLoadingPlaque;
extern struct CFrameSkipperModule* g_pFrameSkipperModule;
extern struct CAutoJumpbugModule* g_pAutoJumpbugModule;
extern struct CClickGUIModule* g_pClickGUIModule;
extern struct CHUDModule* g_pHUDModule;
extern struct CApplicationSpeedModule* g_pApplicationSpeedModule;
extern struct CClientDesignerModule* g_pClientDesignerModule;
extern struct CBacktrackModule* g_pBacktrackModule;
extern struct CForwardtrackModule* g_pForwardtrackModule;
extern struct CFakeLagModule* g_pFakeLagModule;
extern struct CMessageStealerModule* g_pMessageStealerModule;
extern struct CDisablerModule* g_pDisablerModule;
extern struct CSpeedModule* g_pSpeedModule;
extern struct CServerCrasherModule* g_pServerCrasherModule;
extern struct CKickerModule* g_pKickerModule;
extern struct CEntityDescriptions* g_pEntityDescriptions;
extern struct CEntityList* g_pEntityList;
extern cvar_t* sc_game_hud_color_same_as_client_color;
extern bool g_bDoingManualJB;
extern HWND g_hWndOfGame;
extern cmd_function_t* g_pCmdFunctions;
extern int* g_piKeyDest;
#define LIGHTNING_REHL_CONST_KEYDEST_KEY_GAME 0x0
#define LIGHTNING_REHL_CONST_KEYDEST_KEY_MESSAGE 0x1
#define LIGHTNING_REHL_CONST_KEYDEST_KEY_MENU 0x2
extern cvar_t* sc_lightning_crosshair;
extern cvar_t* sc_lightning_crosshair_color;
extern cvar_t* sc_lightning_crosshair_alpha;
extern cvar_t* sc_lightning_crosshair_outline;
extern cvar_t* sc_lightning_crosshair_dot;
extern cvar_t* sc_lightning_crosshair_dot_size;
extern cvar_t* sc_lightning_crosshair_dot_color;
extern cvar_t* sc_lightning_crosshair_size;
extern cvar_t* sc_lightning_crosshair_gap;
extern cvar_t* sc_lightning_crosshair_thickness;
extern cvar_t* sc_lightning_crosshair_top_line;
extern cvar_t* sc_lightning_crosshair_bottom_line;
extern cvar_t* sc_lightning_crosshair_left_line;
extern cvar_t* sc_lightning_crosshair_right_line;
extern bool g_bIsPlaying;
extern unsigned long long s_nLastHudRedrawTime;
extern unsigned long long s_nLastImGuiCtxRenderTime;
extern double TimeDifference(unsigned long long _Start, unsigned long long _End);
extern SCREENINFO g_ScreenInfo;
extern engine_studio_api_s* g_pEngineStudioAPI;
extern cl_entity_s** g_pCurrentEntity;
extern bool g_bIsSven5_26;
extern struct movevars_s* g_pMoveVars;
extern int g_iServerSpawnCount;
extern double g_dblStubGameSpeed;
typedef void(__cdecl* _Cvar_RegisterVariable)(cvar_t* _ConVar);
extern _Cvar_RegisterVariable ORIG_Cvar_RegisterVariable;
extern struct CTimer* g_pTimer;
extern double* g_pdblLatency;
extern FILE* g_pfStdOutput;
extern FILE* g_pfStdInput;
extern FILE* g_pfOriginalStdOutput;
extern FILE* g_pfOriginalStdInput;
extern struct CConfigMgr* g_pGlobalInternals;

#undef printf
#define printf
#undef printf

//here we use _COMMA macro to swallow the variadic args comma in case there are no varargs
#define printf(_Format, ...) do { if ( g_pfStdOutput ) { fprintf( g_pfStdOutput, _Format _COMMA(__VA_ARGS__) __VA_ARGS__ ); } } while (0)

extern const Vector g_vecZero;
extern const Vector VEC_DUCK_HULL_MIN;
extern const Vector VEC_DUCK_HULL_MAX;
extern const Vector VEC_DUCK_VIEW;
extern const Vector VEC_HULL_MIN;
extern const Vector VEC_HULL_MAX;
extern const Vector VEC_HUMAN_HULL_MIN;
extern const Vector VEC_HUMAN_HULL_MAX;
extern const Vector VEC_HUMAN_HULL_DUCK;
#endif //LIGHTNING_TO_MINT_MC

typedef int(__stdcall* _wglSwapBuffers)(HDC);
extern _wglSwapBuffers ORIG_wglSwapBuffers;
typedef HGLRC(__stdcall* _wglGetCurrentContext)(void);
extern _wglGetCurrentContext ORIG_wglGetCurrentContext;
typedef HGLRC(__stdcall* _wglCreateContext)(HDC);
extern _wglCreateContext ORIG_wglCreateContext;
typedef BOOL(__stdcall* _wglMakeCurrent)(HDC, HGLRC);
extern _wglMakeCurrent ORIG_wglMakeCurrent;

#else //!__cplusplus
#error C++ compiler required to compile globals.hpp
#endif //__cplusplus

#endif //GLOBALS_HPP_GUARD

#undef GLOBALS_HPP_RECURSE_GUARD
#endif //GLOBALS_HPP_RECURSE_GUARD