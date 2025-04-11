#include "globals.hpp"

#ifdef _DEBUG
#pragma comment(lib, "freetype_debug.lib")
#pragma comment(lib, "angelscript_debug.lib")
#pragma comment(lib, "libMinHook_debug.lib")
#pragma comment(lib, "libImGui_debug.lib")
#else //!_DEBUG
#pragma comment(lib, "freetype_release.lib")
#pragma comment(lib, "angelscript_release.lib")
#pragma comment(lib, "libMinHook_release.lib")
#pragma comment(lib, "libImGui_release.lib")
#endif //_DEBUG

const Vector g_vecZero = Vector({ 0.f, 0.f, 0.f });
const Vector VEC_DUCK_HULL_MIN = Vector(-16.f, -16.f, -18.f);
const Vector VEC_DUCK_HULL_MAX = Vector(16.f, 16.f, 18.f);
const Vector VEC_DUCK_VIEW = Vector(0.f, 0.f, 12.f);
const Vector VEC_HULL_MIN = Vector(-16.f, -16.f, -36.f);
const Vector VEC_HULL_MAX = Vector(16.f, 16.f, 36.f);
const Vector VEC_HUMAN_HULL_MIN = Vector(-16.f, -16.f, 0.f);
const Vector VEC_HUMAN_HULL_MAX = Vector(16.f, 16.f, 72.f);
const Vector VEC_HUMAN_HULL_DUCK = Vector(16.f, 16.f, 36.f);

void* g_pSelfHandle;
int* g_piKeyDest;
double* g_p_fGameSpeed;
float* g_pfNextCmdTime;
int* g_piLastOutgoingCmd;
double* g_pdblClientTime;
int* g_piIncomingSequence;
int* g_piOutgoingSequence;
engine_studio_api_s* g_pEngineStudioAPI;
cl_entity_s** g_pCurrentEntity;
client_static_t* g_pClientStatic;
client_state_t* g_pClientState;
float g_flFrametime;
int g_iActive;
usermsg_t** g_alpUserMsgs;
_CurWeapon_UserMsg ORIG_CurWeapon_UserMsg;
void* g_pClientDll = nullptr;
cl_enginefunc_t* g_pEngfuncs = nullptr;
_CL_CreateMove ORIG_CL_CreateMove;
_V_CalcRefdef ORIG_V_CalcRefdef;
_HUD_PlayerMove ORIG_HUD_PlayerMove;
_HUD_PostRunCmd ORIG_HUD_PostRunCmd;
_IN_ActivateMouse ORIG_IN_ActivateMouse;
_IN_DeactivateMouse ORIG_IN_DeactivateMouse;
_CL_CameraOffset ORIG_CL_CameraOffset;
_CL_IsThirdPerson ORIG_CL_IsThirdPerson;
_HUD_Redraw ORIG_HUD_Redraw;
_HUD_AddEntity ORIG_HUD_AddEntity;
decltype(ORIG_SayText_UserMsg) ORIG_SayText_UserMsg;
_QueryPerformanceCounter ORIG_QueryPerformanceCounter;
float g_flBaseApplicationSpeed = 1.0f;
bool g_bDoingManualJB = false;
cvar_t* sv_accelerate;
cvar_t* sv_airaccelerate;
cvar_t* sv_bounce;
cvar_t* sv_friction;
cvar_t* edgefriction;
cvar_t* sv_gravity;
cvar_t* sv_stepsize;
cvar_t* sv_maxvelocity;
cvar_t* sv_stopspeed;
usercmd_s* g_pCmd;
playermove_s* g_pPlayerMove;
ref_params_s* g_pRefParams;
SCREENINFO g_ScreenInfo;
bool g_bIsPlaying = false;
unsigned long long s_nLastHudRedrawTime = 0;
unsigned long long s_nLastImGuiCtxRenderTime = 0;
void* g_pHwDll = nullptr;
_Netchan_TransmitBits ORIG_Netchan_TransmitBits = nullptr;
_SCR_UpdateScreen ORIG_SCR_UpdateScreen = nullptr;
_Cbuf_InsertText ORIG_Cbuf_InsertText = nullptr;
_Cbuf_InsertText ORIG_Cbuf_AddText = nullptr;
_SPR_Set ORIG_SPR_Set = nullptr;
_Draw_FillRGBA ORIG_Draw_FillRGBA = nullptr;
_MSG_WriteUsercmd ORIG_MSG_WriteUsercmd = nullptr;
_R_StudioDrawPlayer ORIG_R_StudioDrawPlayer = nullptr;
_SCR_EndLoadingPlaque ORIG_SCR_EndLoadingPlaque = nullptr;
cmd_function_t* g_pCmdFunctions = nullptr;
movevars_s* g_pMoveVars = nullptr;
bool g_bIsSven5_26;
cvar_t* sc_game_hud_color_same_as_client_color;
_SZ_GetSpace ORIG_SZ_GetSpace;
void* g_pNetchanMessage;
ImColor g_DefaultClientColor = ImColor(48, 255, 245);
WNDPROC ORIG_wndproc;
_wglSwapBuffers ORIG_wglSwapBuffers;
ImFont* g_pClientFont;
HHOOK g_pHook;
bool g_bForceImGuiToNotDrawAnything;
HWND g_hWndOfGame;
int g_iLastMouseX;
int g_iLastMouseY;
int* g_piIncomingReliableSequence;
CEntityDescriptions* g_pEntityDescriptions;
CEntityList* g_pEntityList;
int g_iServerSpawnCount;
_wglGetCurrentContext ORIG_wglGetCurrentContext;
_wglCreateContext ORIG_wglCreateContext;
_wglMakeCurrent ORIG_wglMakeCurrent;
cvar_t* sc_lightning_dont_send_sendents;
double g_dblStubGameSpeed = 1000.0;
_Cvar_RegisterVariable ORIG_Cvar_RegisterVariable;
CTimer* g_pTimer;
double* g_pdblLatency; //multiply this by 1000 to get latency like in scoreboard
FILE* g_pfStdOutput;
FILE* g_pfStdInput;
FILE* g_pfOriginalStdOutput;
FILE* g_pfOriginalStdInput;
CConfigMgr* g_pGlobalInternals;