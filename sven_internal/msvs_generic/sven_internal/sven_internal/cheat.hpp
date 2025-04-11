/**
 * Copyright - xWhitey, 2023.
 * cheat.hpp - description
 */

#ifdef CHEAT_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in cheat.hpp
#else //CHEAT_HPP_RECURSE_GUARD

#define CHEAT_HPP_RECURSE_GUARD

#ifndef CHEAT_HPP_GUARD
#define CHEAT_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CCheat {
	CCheat();

	static CCheat* GetCheat();

	void SetupHooks();
	void MainLoop();
	void LoadUpScripts();

	CModuleManager* m_pModuleManager;
	CEventManager* m_pEventBus;
	CVirtualConsole* m_pConsole;
	CNotifications* m_pNotifications;
	CEntityPlayerSP* m_pLocalPlayer;

	CTheScripts* m_pTheScripts;
	CConfigMgr* m_pTheScriptsMgr;

	CMovementRecorder* m_pMovementRecorder;

	const static char* const ms_pszVersion;
private:
	static inline CCheat* _ms_pTheCheat;
} CCheat;

using CCheat = struct CCheat;

#else //!__cplusplus
#error C++ compiler required to compile cheat.hpp
#endif //__cplusplus

#endif //CHEAT_HPP_GUARD

#undef CHEAT_HPP_RECURSE_GUARD
#endif //CHEAT_HPP_RECURSE_GUARD