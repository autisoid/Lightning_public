#include "StdAfx.h"
#include "metahook_emulation.hpp"

typedef int(*INITIALIZE_FUNC)	(struct cl_enginefuncs_s*, int);
typedef void(*HUD_INIT_FUNC)		(void);
typedef int(*HUD_VIDINIT_FUNC)	(void);
typedef int(*HUD_REDRAW_FUNC)	(float, int);
typedef int(*HUD_UPDATECLIENTDATA_FUNC) (struct client_data_s*, float);
typedef void(*HUD_RESET_FUNC)    (void);
typedef void(*HUD_CLIENTMOVE_FUNC)(struct playermove_s *ppmove, qboolean server);
typedef void(*HUD_CLIENTMOVEINIT_FUNC)(struct playermove_s *ppmove);
typedef char(*HUD_TEXTURETYPE_FUNC)(char *name);
typedef void(*HUD_IN_ACTIVATEMOUSE_FUNC) (void);
typedef void(*HUD_IN_DEACTIVATEMOUSE_FUNC)		(void);
typedef void(*HUD_IN_MOUSEEVENT_FUNC)		(int mstate);
typedef void(*HUD_IN_CLEARSTATES_FUNC)		(void);
typedef void(*HUD_IN_ACCUMULATE_FUNC) (void);
typedef void(*HUD_CL_CREATEMOVE_FUNC)		(float frametime, struct usercmd_s *cmd, int active);
typedef int(*HUD_CL_ISTHIRDPERSON_FUNC) (void);
typedef void(*HUD_CL_GETCAMERAOFFSETS_FUNC)(float *ofs);
typedef struct kbutton_s * (*HUD_KB_FIND_FUNC) (const char *name);
typedef void(*HUD_CAMTHINK_FUNC)(void);
typedef void(*HUD_CALCREF_FUNC) (struct ref_params_s *pparams);
typedef int(*HUD_ADDENTITY_FUNC) (int type, struct cl_entity_s *ent, const char *modelname);
typedef void(*HUD_CREATEENTITIES_FUNC) (void);
typedef void(*HUD_DRAWNORMALTRIS_FUNC) (void);
typedef void(*HUD_DRAWTRANSTRIS_FUNC) (void);
typedef void(*HUD_STUDIOEVENT_FUNC) (const struct mstudioevent_s *event, const struct cl_entity_s *entity);
typedef void(*HUD_POSTRUNCMD_FUNC) (struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
typedef void(*HUD_SHUTDOWN_FUNC) (void);
typedef void(*HUD_TXFERLOCALOVERRIDES_FUNC)(struct entity_state_s *state, const struct clientdata_s *client);
typedef void(*HUD_PROCESSPLAYERSTATE_FUNC)(struct entity_state_s *dst, const struct entity_state_s *src);
typedef void(*HUD_TXFERPREDICTIONDATA_FUNC) (struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd);
typedef void(*HUD_DEMOREAD_FUNC) (int size, unsigned char *buffer);
typedef int(*HUD_CONNECTIONLESS_FUNC)(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
typedef	int(*HUD_GETHULLBOUNDS_FUNC) (int hullnumber, float *mins, float *maxs);
typedef void(*HUD_FRAME_FUNC)		(double);
typedef int(*HUD_KEY_EVENT_FUNC) (int eventcode, int keynum, const char *pszCurrentBinding);
typedef void(*HUD_TEMPENTUPDATE_FUNC) (double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s *pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s *pTemp, float damp));
typedef struct cl_entity_s *(*HUD_GETUSERENTITY_FUNC) (int index);
typedef void(*HUD_VOICESTATUS_FUNC)(int entindex, qboolean bTalking);
typedef void(*HUD_DIRECTORMESSAGE_FUNC)(int iSize, void *pbuf);
typedef int(*HUD_STUDIO_INTERFACE_FUNC)(int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio);
typedef void(*HUD_CHATINPUTPOSITION_FUNC)(int *x, int *y);
typedef int(*HUD_GETPLAYERTEAM)(int iplayer);
typedef void *(*CLIENTFACTORY)();

typedef struct cl_exportfuncs_s
{
	INITIALIZE_FUNC Initialize;
	HUD_INIT_FUNC HUD_Init;
	HUD_VIDINIT_FUNC HUD_VidInit;
	HUD_REDRAW_FUNC HUD_Redraw;
	HUD_UPDATECLIENTDATA_FUNC HUD_UpdateClientData;
	HUD_RESET_FUNC HUD_Reset;
	HUD_CLIENTMOVE_FUNC HUD_PlayerMove;
	HUD_CLIENTMOVEINIT_FUNC HUD_PlayerMoveInit;
	HUD_TEXTURETYPE_FUNC HUD_PlayerMoveTexture;
	HUD_IN_ACTIVATEMOUSE_FUNC IN_ActivateMouse;
	HUD_IN_DEACTIVATEMOUSE_FUNC IN_DeactivateMouse;
	HUD_IN_MOUSEEVENT_FUNC IN_MouseEvent;
	HUD_IN_CLEARSTATES_FUNC IN_ClearStates;
	HUD_IN_ACCUMULATE_FUNC IN_Accumulate;
	HUD_CL_CREATEMOVE_FUNC CL_CreateMove;
	HUD_CL_ISTHIRDPERSON_FUNC CL_IsThirdPerson;
	HUD_CL_GETCAMERAOFFSETS_FUNC CL_CameraOffset;
	HUD_KB_FIND_FUNC KB_Find;
	HUD_CAMTHINK_FUNC CAM_Think;
	HUD_CALCREF_FUNC V_CalcRefdef;
	HUD_ADDENTITY_FUNC HUD_AddEntity;
	HUD_CREATEENTITIES_FUNC HUD_CreateEntities;
	HUD_DRAWNORMALTRIS_FUNC HUD_DrawNormalTriangles;
	HUD_DRAWTRANSTRIS_FUNC HUD_DrawTransparentTriangles;
	HUD_STUDIOEVENT_FUNC HUD_StudioEvent;
	HUD_POSTRUNCMD_FUNC HUD_PostRunCmd;
	HUD_SHUTDOWN_FUNC HUD_Shutdown;
	HUD_TXFERLOCALOVERRIDES_FUNC HUD_TxferLocalOverrides;
	HUD_PROCESSPLAYERSTATE_FUNC HUD_ProcessPlayerState;
	HUD_TXFERPREDICTIONDATA_FUNC HUD_TxferPredictionData;
	HUD_DEMOREAD_FUNC Demo_ReadBuffer;
	HUD_CONNECTIONLESS_FUNC HUD_ConnectionlessPacket;
	HUD_GETHULLBOUNDS_FUNC HUD_GetHullBounds;
	HUD_FRAME_FUNC HUD_Frame;
	HUD_KEY_EVENT_FUNC HUD_Key_Event;
	HUD_TEMPENTUPDATE_FUNC HUD_TempEntUpdate;
	HUD_GETUSERENTITY_FUNC HUD_GetUserEntity;
	HUD_VOICESTATUS_FUNC HUD_VoiceStatus;
	HUD_DIRECTORMESSAGE_FUNC HUD_DirectorMessage;
	HUD_STUDIO_INTERFACE_FUNC HUD_GetStudioModelInterface;
	HUD_CHATINPUTPOSITION_FUNC HUD_ChatInputPosition;
	HUD_GETPLAYERTEAM HUD_GetPlayerTeam;
	CLIENTFACTORY ClientFactory;
}
cl_exportfuncs_t;

typedef struct mh_enginesave_s
{
	cl_exportfuncs_t *pExportFuncs;
	cl_enginefunc_t *pEngineFuncs;
}mh_enginesave_t;

void CMHLightning::Init(_In_ void* _MetaHookAPI, _In_ void* _Interface, _In_ void* _Save) {
	if (_Save) {
		mh_enginesave_s* pSave = indirect_cast<decltype(pSave)>(_Save);
		if (pSave) {
			if (pSave->pExportFuncs) {
				if (pSave->pExportFuncs->HUD_GetStudioModelInterface) {
					r_studio_interface_s* pIface;
					pSave->pExportFuncs->HUD_GetStudioModelInterface(STUDIO_INTERFACE_VERSION, &pIface, g_pEngineStudioAPI);
				}
			}
		}
	}
}

void CMHLightning::Shutdown() {
}

void CMHLightning::LoadEngine(_In_ void* _Engfuncs) {
	g_pEngfuncs = indirect_cast<decltype(g_pEngfuncs)>(_Engfuncs);
}

void CMHLightning::LoadClient(_In_ void* _ExportFunc) {
	if (g_pClientDll = GetModuleHandleA("client")) {
		static CCheat* cheat = Q_new(CCheat)();
		cheat->SetupHooks();
	} else {
		printf("Couldn't find client.dll module! (Meta)\n");
	}
}

void CMHLightning::ExitGame(_In_ int _Result) {
}

EXPOSE_SINGLE_INTERFACE(CMHLightning, CMHLightning, METAHOOK_PLUGIN_API_VERSION_V3);