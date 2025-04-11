/**
 * Copyright - xWhitey, 2022.
 * virtual_console.hpp - An implementation of virtual console like in GoldSrc/Source engines.
 */

#ifdef VIRTUAL_CONSOLE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in virtual_console.hpp
#else //VIRTUAL_CONSOLE_HPP_RECURSE_GUARD

#define VIRTUAL_CONSOLE_HPP_RECURSE_GUARD

#ifndef VIRTUAL_CONSOLE_HPP_GUARD
#define VIRTUAL_CONSOLE_HPP_GUARD
#pragma once

typedef void(*g_tpfnConsoleFunc)(struct CVirtualConsole*);

typedef struct CVirtualConCmd {
	CVirtualConCmd(_In_z_ const char* _Name, _In_ g_tpfnConsoleFunc _Handler);

	const char* m_pszName;
	g_tpfnConsoleFunc m_pfnHandler;
} CVirtualConCmd;

using CVirtualConCmd = struct CVirtualConCmd;

typedef void(*g_tpfnConVarOnceSetCallback)(void* /*_UserArgs*/, char**);

typedef struct CVirtualConVar {
	CVirtualConVar(_In_z_ _Pre_z_ const char* _Name, _In_z_ char* _DefaultValue);

	void Set(_In_ struct CVirtualConsole* _Console);

	void DirectSet(_In_z_ char* _Value, _In_opt_ bool _MemFreePreviousValue = false);

	char* Get();

	bool AsBool();

	void RegisterCallback(_In_ g_tpfnConVarOnceSetCallback _Callback, _In_opt_ void* _UserArg);

	const char* m_pszName;
	char* m_pszCurrentValue;
	char* m_pszDefaultValue;
	g_tpfnConVarOnceSetCallback m_pfnOnceSetCallback;
	void* m_pCallbackUserArg;
} CVirtualConVar;

using CVirtualConVar = struct CVirtualConVar;

typedef struct CVirtualConsole {
	CVirtualConsole();

	CVector<char*>* Tokenize(_In_z_ const char* _Text, _In_ char _Delimiter);

	//Register a console command
	void RegisterCommand(_In_ CVirtualConCmd* _ConCmd);

	//Register a console variable
	void RegisterConVar(_In_ CVirtualConVar* _ConVar);

	//Print formatted text
	void Printf(_In_z_ _Pre_z_ const char* _Format, _In_opt_ ...);

	//Draw the console itself
	void Draw(_In_ bool& _Active);

	//Get an argument passed to the command at _Index
	const char* Cmd_Argv(_In_ int _Index);

	//Returns the size of args passed to the command (including the command itself)
	int Cmd_Argc();

	//Tokenize "_Text" and turn it into command args, then call AddText.
	void InsertText(_In_z_ const char* _Text, _In_opt_ bool _ShouldDisplayTheText = true);

	//Find "_CommandLine" in registered commands/convars, then call its m_pfnHandler. Be careful and possibly don't use directly - it doesn't parse args on its own.
	void AddText(_In_z_ const char* _CommandLine, _In_opt_ bool _ShouldDisplayTheText = true);

	//Static stub for input, thisptr is passed as "UserData" into _Data.
	static int TextEditCallbackStub(_In_ ImGuiInputTextCallbackData* _Data);

	//Input CB
	int TextEditCallback(_In_ ImGuiInputTextCallbackData* _Data);

	//Flush current console (i.e. wipes the screen)
	void Clear();

	char m_acInputBuf[256];
	CVector<char*>* m_pVecItems;
	CVector<CVirtualConCmd*>* m_pVecCommands;
	CVector<CVirtualConVar*>* m_pVecConVars;
	CVector<char*>* m_pVecHistory;
	int m_nHistoryPos;
	bool m_bScrollToBottom;
	CVector<const char*>* m_pVecArgs;
} CVirtualConsole;

using CVirtualConsole = struct CVirtualConsole;

#endif //VIRTUAL_CONSOLE_HPP_GUARD

#undef VIRTUAL_CONSOLE_HPP_RECURSE_GUARD
#endif //VIRTUAL_CONSOLE_HPP_RECURSE_GUARD