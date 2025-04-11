/**
 * Copyright - xWhitey, 2024.
 * svenmod_emulation.hpp - an advanced emulation system which allows us to act as a SvenMod plugin.
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef SVENMOD_EMULATION_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in svenmod_emulation.hpp
#else //SVENMOD_EMULATION_HPP_RECURSE_GUARD

#define SVENMOD_EMULATION_HPP_RECURSE_GUARD

#ifndef SVENMOD_EMULATION_HPP_GUARD
#define SVENMOD_EMULATION_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "iface.hpp"

extern bool g_bHasLoaded;

typedef struct api_version_s
{
	int major_version;
	int minor_version;
} api_version_t;

typedef enum
{
	PLUGIN_CONTINUE = 0,	// keep going
	PLUGIN_OVERRIDE,		// run the game dll function but use our return value instead
	PLUGIN_STOP,			// don't run the game dll function at all
	PLUGIN_CALL_STOP		// call the game dll function and stop further hooks from executing 
} PLUGIN_RESULT;

class IClientPlugin
{
public:
	virtual ~IClientPlugin() {}

	virtual api_version_t GetAPIVersion(void) = 0;

	virtual bool Load(CreateInterfaceFn pfnSvenModFactory, void *pSvenModAPI, void *pPluginHelpers) = 0;

	virtual void PostLoad(bool bGlobalLoad) = 0;

	virtual void Unload(void) = 0;

	virtual bool Pause(void) = 0;

	virtual void Unpause(void) = 0;

	virtual void OnFirstClientdataReceived(void *pcldata, float flTime) = 0;

	virtual void OnBeginLoading(void) = 0;

	virtual void OnEndLoading(void) = 0;

	virtual void OnDisconnect(void) = 0;

	virtual void GameFrame(void* state, double frametime, bool bPostRunCmd) = 0;

	// Called to draw 2D paints after rendering the game view
	virtual PLUGIN_RESULT Draw(void) = 0;

	// Called to redraw client's HUD
	virtual PLUGIN_RESULT DrawHUD(float time, int intermission) = 0;

	// Name of the plugin
	virtual const char			*GetName(void) = 0;

	// Author of the plugin
	virtual const char			*GetAuthor(void) = 0;

	// Version of the plugin
	virtual const char			*GetVersion(void) = 0;

	// Description of the plugin
	virtual const char			*GetDescription(void) = 0;

	// URL of the plugin
	virtual const char			*GetURL(void) = 0;

	// Build date of the plugin
	virtual const char			*GetDate(void) = 0;

	// Tag of the plugin to log
	virtual const char			*GetLogTag(void) = 0;
};

typedef struct CLightning : IClientPlugin {
	virtual api_version_t GetAPIVersion(void);

	virtual bool Load(CreateInterfaceFn pfnSvenModFactory, void *pSvenModAPI, void *pPluginHelpers);

	virtual void PostLoad(bool bGlobalLoad);

	virtual void Unload(void);

	virtual bool Pause(void);

	virtual void Unpause(void);

	virtual void OnFirstClientdataReceived(void *pcldata, float flTime);

	virtual void OnBeginLoading(void);

	virtual void OnEndLoading(void);

	virtual void OnDisconnect(void);

	virtual void GameFrame(void* state, double frametime, bool bPostRunCmd);

	// Called to draw 2D paints after rendering the game view
	virtual PLUGIN_RESULT Draw(void);

	// Called to redraw client's HUD
	virtual PLUGIN_RESULT DrawHUD(float time, int intermission);

	// Name of the plugin
	virtual const char			*GetName(void);

	// Author of the plugin
	virtual const char			*GetAuthor(void);

	// Version of the plugin
	virtual const char			*GetVersion(void);

	// Description of the plugin
	virtual const char			*GetDescription(void);

	// URL of the plugin
	virtual const char			*GetURL(void);

	// Build date of the plugin
	virtual const char			*GetDate(void);

	// Tag of the plugin to log
	virtual const char			*GetLogTag(void);
} CLightning;

using CLightning = struct CLightning;

extern CLightning g_Lightning;
extern CLightning* g_pLightning;

#else //!__cplusplus
#error C++ compiler required to compile svenmod_emulation.hpp
#endif //__cplusplus

#endif //SVENMOD_EMULATION_HPP_GUARD

#undef SVENMOD_EMULATION_HPP_RECURSE_GUARD
#endif //SVENMOD_EMULATION_HPP_RECURSE_GUARD