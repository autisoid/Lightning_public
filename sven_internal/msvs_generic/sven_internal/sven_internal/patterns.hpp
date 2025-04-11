/**
 * Copyright - xWhitey ~ ScriptedSnark, 2022.
 * patterns.hpp - Our pattern-system
 */

#ifdef PATTERNS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in patterns.hpp
#else //PATTERNS_HPP_RECURSE_GUARD

#define PATTERNS_HPP_RECURSE_GUARD

#ifndef PATTERNS_HPP_GUARD
#define PATTERNS_HPP_GUARD
#pragma once

#include "common.hpp"

#include "MemUtils.hpp"

#include "string_utils.hpp"

#include <vector>

#ifndef __CONCATENATE_INTERNAL__
#define _CONCATENATE_INTERNAL_WAS_DEFINED_BY_US
#define __CONCATENATE_INTERNAL__(_V, _B) _V ## _B
#endif //!__CONCATENATE_INTERNAL__

#ifndef __CONCATENATE__
#define _CONCATENATE_WAS_DEFINED_BY_US
#define __CONCATENATE__(_V, _B) __CONCATENATE_INTERNAL__(_V, _B)
#endif //!__CONCATENATE__

typedef struct CIntelligentPattern {
	CIntelligentPattern(_In_z_ const char* _IDAPattern, _In_z_ const char* _Version);
	~CIntelligentPattern();

	unsigned char* m_pcPattern;
	char* m_pszMask;
	const char* m_pszVersion;
} CIntelligentPattern;

using CIntelligentPattern = struct CIntelligentPattern;

typedef struct CIntelligentPatterns {
	static CIntelligentPattern* MakePattern(_In_z_ const char* _IDAPattern, _In_z_ const char* _Version);
	static std::vector<CIntelligentPattern*> MakePatterns(std::vector<const char*> _Patterns, std::vector<const char*> _Versions);
} CIntelligentPatterns;

using CIntelligentPatterns = struct CIntelligentPatterns;

#define PATTERNS(_Name, ...) \
	static const char* __CONCATENATE__(_Name, _Patterns)[] = { __VA_ARGS__ }; \
	const static auto _Name = [](const char** _SPTLibStyledPatterns, int _Size) { \
	std::vector<const char*> vecPatterns = {}; \
	std::vector<const char*> vecVersions = {}; \
											   \
	for (unsigned int idx = 0; idx < _Size; idx += 2) { \
		const char* szTheVersion = _SPTLibStyledPatterns[idx]; \
		const char* szThePattern = _SPTLibStyledPatterns[idx + 1]; \
		vecVersions.push_back(szTheVersion); \
		vecPatterns.push_back(szThePattern); \
	} \
	\
	return CIntelligentPatterns::MakePatterns(vecPatterns, vecVersions); \
	}(__CONCATENATE__(_Name, _Patterns), Q_ARRAYSIZE(__CONCATENATE__(_Name, _Patterns)))

#define PATTERN(_Name, _Version, _Pattern) const static auto _Name = CIntelligentPatterns::MakePattern(_Pattern, _Version)

typedef void(*g_tToDoUponFoundCallback)(const char* _Version);
typedef void(*g_tToDoIfCouldntFindCallback)();

template<class _Ty> struct CIntelligentPatternFinder {
	_Success_(return == true) static bool FindPattern(_In_ void* _Module, _In_ unsigned char* _Pattern, _In_ const char* _Mask, _Out_ _Ty& _Result);
	_Success_(return == true) static bool FindAsync(_In_ void* _Module, _In_ unsigned char* _Pattern, _In_ const char* _Mask, _Out_ _Ty& _Result);
	_Success_(return == true) static bool FindAsync(_In_ void* _Module, _In_ CIntelligentPattern * _Pattern, _Out_ _Ty& _Result);
	static void FindAsync(_In_ void* _Module, _In_ CIntelligentPattern* _Pattern, _Out_ _Ty& _Result, _In_opt_ g_tToDoUponFoundCallback _ToDoUponFound, _In_opt_ g_tToDoIfCouldntFindCallback _ToDoIfCouldntFind);
	_Success_(return == true) static bool FindAsync(_In_ void* _Module, _In_ std::vector<CIntelligentPattern*> _Patterns, _Out_ _Ty& _Result);
	static void FindAsync(_In_ void* _Module, _In_ std::vector<CIntelligentPattern*> _Patterns, _Out_ _Ty& _Result, _In_opt_ g_tToDoUponFoundCallback _ToDoUponFound, _In_opt_ g_tToDoIfCouldntFindCallback _ToDoIfCouldntFind);
};

_Success_(return == true) template<class _Ty> bool FindPattern(_In_ void* _Module, _In_ unsigned char* _Pattern, _In_ const char* _Mask, _Out_ _Ty& _Result) {
	return CIntelligentPatternFinder<_Ty>::FindPattern(_Module, _Pattern, _Mask, CReferenceKeeper(_Result));
}

_Success_(return == true) template<class _Ty> bool FindAsync(_In_ void* _Module, _In_ unsigned char* _Pattern, _In_ const char* _Mask, _Out_ _Ty& _Result) {
	return CIntelligentPatternFinder<_Ty>::FindAsync(_Module, _Pattern, _Mask, CReferenceKeeper(_Result));
}

_Success_(return == true) template<class _Ty> bool FindAsync(_In_ void* _Module, _In_ CIntelligentPattern* _Pattern, _Out_ _Ty& _Result) {
	return CIntelligentPatternFinder<_Ty>::FindAsync(_Module, _Pattern, CReferenceKeeper(_Result));
}

template<class _Ty> void FindAsync(_In_ void* _Module, _In_ CIntelligentPattern* _Pattern, _Out_ _Ty& _Result, _In_opt_ g_tToDoUponFoundCallback _ToDoUponFound, _In_opt_ g_tToDoIfCouldntFindCallback _ToDoIfCouldntFind) {
	return CIntelligentPatternFinder<_Ty>::FindAsync(_Module, _Pattern, CReferenceKeeper(_Result), _ToDoUponFound, _ToDoIfCouldntFind);
}

_Success_(return == true) template<class _Ty> bool FindAsync(_In_ void* _Module, _In_ std::vector<CIntelligentPattern*> _Patterns, _Out_ _Ty& _Result) {
	return CIntelligentPatternFinder<_Ty>::FindAsync(_Module, _Patterns, CReferenceKeeper(_Result));
}

template<class _Ty> void FindAsync(_In_ void* _Module, _In_ std::vector<CIntelligentPattern*> _Patterns, _Out_ _Ty& _Result, _In_opt_ g_tToDoUponFoundCallback _ToDoUponFound, _In_opt_ g_tToDoIfCouldntFindCallback _ToDoIfCouldntFind) {
	return CIntelligentPatternFinder<_Ty>::FindAsync(_Module, _Patterns, CReferenceKeeper(_Result), _ToDoUponFound, _ToDoIfCouldntFind);
}

//detail
template<class _Ty> bool CIntelligentPatternFinder<_Ty>::FindPattern(_In_ void* _Module, _In_ unsigned char* _Pattern, _In_ const char* _Mask, _Out_ _Ty& _Result) {
	if (!_Module || !_Pattern || !_Mask) return false;
	if (_Mask[0] == '\0') return false;

	DWORD dwModuleSize = MemUtils::GetModuleSize(_Module);

	size_t sPatternLength = strlen(_Mask);

	MEMORY_BASIC_INFORMATION mbi;

	if (!VirtualQuery(_Module, &mbi, sizeof(mbi))) return false;

	if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS) {
		unsigned char* pSearchStart = (unsigned char*)MemUtils::GetModuleBase(_Module);
		unsigned char* pSearchEnd = pSearchStart + dwModuleSize - sPatternLength;

		while (pSearchStart < pSearchEnd) {
			bool bFound = true;

			for (size_t sIndex = 0; sIndex < sPatternLength; ++sIndex) {
				if (_Mask[sIndex] != '?' && _Pattern[sIndex] != pSearchStart[sIndex]) {
					bFound = false;
					break;
				}
			}

			if (bFound) {
				_Result = (_Ty)(pSearchStart);

				return true;
			}

			pSearchStart++;
		}
	}

	return false;
}

template<class _Ty> bool CIntelligentPatternFinder<_Ty>::FindAsync(_In_ void* _Module, _In_ unsigned char* _Pattern, _In_ const char* _Mask, _Out_ _Ty& _Result) {
	/*CIntelligentPatternThreadParms<_Ty>* pParms = Q_new(CIntelligentPatternThreadParms<_Ty>)({_Module, _Pattern, _Mask, CReferenceKeeper(_Result), false, false});

	HANDLE hThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(FindPatternThread<_Ty>), pParms, 0, NULL);

	if (!hThread) {
		Q_delete(pParms);

		return false;
	}

	SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

	while (!pParms->m_bFinishedSearching) {
		SwitchToThread();
		continue;
	}
	TerminateThread(hThread, 0);
	
	bool bFound = pParms->m_bFound;
	Q_delete(pParms);

	return bFound;*/

	//Since we create a thread for ourself in DllMain, it's unnecessary to do asynchronous find.
	return FindPattern(_Module, _Pattern, _Mask, CReferenceKeeper(_Result));
}

template<class _Ty> bool CIntelligentPatternFinder<_Ty>::FindAsync(_In_ void* _Module, _In_ CIntelligentPattern* _Pattern, _Out_ _Ty& _Result) {
	return FindAsync(_Module, _Pattern->m_pcPattern, _Pattern->m_pszMask, CReferenceKeeper(_Result));
}

template<class _Ty> void CIntelligentPatternFinder<_Ty>::FindAsync(_In_ void* _Module, _In_ CIntelligentPattern* _Pattern, _Out_ _Ty& _Result, _In_opt_ g_tToDoUponFoundCallback _ToDoUponFound, _In_opt_ g_tToDoIfCouldntFindCallback _ToDoIfCouldntFind) {
	if (FindAsync(_Module, _Pattern, CReferenceKeeper(_Result))) {
		if (_ToDoUponFound)
			_ToDoUponFound(_Pattern->m_pszVersion);
	}
	else {
		if (_ToDoIfCouldntFind)
			_ToDoIfCouldntFind();
	}
}

template<class _Ty> bool CIntelligentPatternFinder<_Ty>::FindAsync(_In_ void* _Module, _In_ std::vector<CIntelligentPattern*> _Patterns, _Out_ _Ty& _Result) {
	for (size_t sIndex = 0; sIndex < _Patterns.size(); sIndex++) {
		if (FindAsync(_Module, _Patterns.at(sIndex), CReferenceKeeper(_Result)))
			return true;
	}

	return false;
}

template<class _Ty> void CIntelligentPatternFinder<_Ty>::FindAsync(_In_ void* _Module, _In_ std::vector<CIntelligentPattern*> _Patterns, _Out_ _Ty& _Result, _In_opt_ g_tToDoUponFoundCallback _ToDoUponFound, _In_opt_ g_tToDoIfCouldntFindCallback _ToDoIfCouldntFind) {
	for (functional_size_t sIndex = 0; sIndex < _Patterns.size(); sIndex++) {
		CIntelligentPattern* pThePattern = _Patterns.at(sIndex);
		if (FindAsync(_Module, pThePattern, CReferenceKeeper(_Result))) {
			if (_ToDoUponFound)
				_ToDoUponFound(pThePattern->m_pszVersion);
			return;
		}
	}

	if (_ToDoIfCouldntFind)
		_ToDoIfCouldntFind();
}

namespace patterns {
	namespace engine {
		PATTERNS(SZ_GetSpace, "Sven-5.25", "56 8B 74 24 08 57 8B 7C 24 10 8B 4E 10 8B 56 0C 8D 04 39 3B C2 0F 8E ?? ?? ?? ?? F6 46 04 01 75 2E 8B 06 85 D2 75 11 85 C0 75 05 B8 ?? ?? ?? ?? 50 68 ?? ?? ?? ?? EB 0F 85 C0 75 05 B8",
                              "HL-8684", "55 8B EC 56 8B 75 08 57 8B 7D 0C 8B 4E 10",
                              "HL-9920", "55 8B EC 53 8B 5D 0C 56 8B 75 08 8B 4E 10 8B 56 0C 8D 04 19 3B C2 0F 8E ?? ?? ?? ?? F6 46 04 01 57 BF ?? ?? ?? ?? 75 28 8B 0E 8B C7 85 D2 75 0D 85 C9 0F 45 C1 50 68 ?? ?? ?? ?? EB 0B 85 C9 0F 45 C1 50 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 3B 5E 0C 7E 31 F6 46 04 01 8B C7 8B 0E 75 13 85 C9 0F 45 C1 50");
		PATTERNS(ClientConnect__MSG_WriteByte_cls_netchan_message, "Sven-5.25", "68 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? 83 C4 10 DD 15 ?? ?? ?? ?? D9 1D ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 5F 5E C3",
                              "HL-8684", "68 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? D9 1D ?? ?? ?? ?? 83 C4 10 83 C8 FF",
                              "HL-9920", "68 ?? ?? ?? ?? E8 ?? ?? ?? ?? F2 0F 10 05 ?? ?? ?? ?? 83 C4 10 F2 0F 11 05 ?? ?? ?? ?? 66 0F 5A C0 C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F 11 05 ?? ?? ?? ?? 5E 5B 8B 4D FC 33 CD E8 ?? ?? ?? ?? 8B E5 5D");
		PATTERNS(g_fNextCmdTime,  "Sven-5.25", "D9 1D ?? ?? ?? ?? 75 0A A1",
                                 "HL-8684", "D9 05 ?? ?? ?? ?? D8 05 ?? ?? ?? ?? DC 1D ?? ?? ?? ?? DF E0 25 00 41 00 00 0F 84 ?? ?? ?? ?? 8B 45 FC BE 01 00 00 00 3B C7 0F 8E",
                                 "HL-9920", "8B 0D ?? ?? ?? ?? F3 0F 10 25 ?? ?? ?? ?? F2 0F 10 15 ?? ?? ?? ?? F3 0F 10 1D ?? ?? ?? ?? 85 C9 74 3C 83 3D ?? ?? ?? ?? ?? 75 33 A1 ?? ?? ?? ?? 3B 05 ?? ?? ?? ?? 75 26 83 3D ?? ?? ?? ?? ?? 74 09 83 3D ?? ?? ?? ?? ?? 75 14");
		PATTERNS(Netchan_TransmitBits, "Sven-5.25", "B8 40 01 01 00 E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 3C 01 01 00 8B 84 24 4C 01 01 00 53 55 8B AC 24 4C 01 01 00 89 44 24 28 8D 84 24 44 01 00 00 89 44 24 1C 33 C0 56 F6 45 70 02 57 C7 44 ?? ?? ?? ?? ?? ?? C7 44 24 28 FF FF 00 00 C7 44 24 2C 00 00 00 00 66 89 44 24 20 74 28 50 83 EC 24 8D 75 04 B9 09 00 00 00 8B FC",
                                      "HL-8684", "55 8B EC B8 00 11 00 00 E8 ?? ?? ?? ?? 53 8B 5D 08 8D 8D 00 EF FF FF 33 C0 89 4D E0 8A 4B 60 56 57",
									  "Sven-5.25-Compat", "E9 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 3C 01 01 00 8B 84 24 4C 01 01 00 53 55 8B AC 24 4C 01 01 00 89 44 24 28 8D 84 24 44 01 00 00 89 44 24 1C 33 C0 56 F6 45 70 02 57 C7 44 ?? ?? ?? ?? ?? ?? C7 44 24 28 FF FF 00 00 C7 44 24 2C 00 00 00 00 66 89 44 24 20 74 28 50 83 EC 24 8D 75 04 B9 09 00 00 00 8B FC",
                                      "HL-9920", "55 8B EC B8 E4 18 00 00 E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 FC 8B 45 10 33 C9 53 56 57 8B 7D 08 89 85 24 E7 FF FF 8D 85 60 E7 FF FF 89 85 40 E7 FF FF 33 C0 89 BD 34 E7 FF FF F6 47 60 02 89 8D 30 E7 FF FF C7 85 ?? ?? ?? ?? ?? ?? ?? ?? C7 85 44 E7 FF FF 95 17 00 00 89 8D 48 E7 FF FF",
									  "Sven-5.26", "B8 40 01 01 00 E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 3C 01 01 00 8B 84 24 4C 01 01 00 53 55 8B AC 24 4C 01 01 00 89 44 24 30 8D 84 24 44 01 00 00 89 44 24 20 33 C0 56 F6 45 70 02 57");
		PATTERNS(SCR_UpdateScreen, "Sven-5.25", "83 EC 44 A1 ?? ?? ?? ?? 33 C4 89 44 24 40 83 3D ?? ?? ?? ?? ?? 0F 85 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? DD 54 24 08 D9 EE 83 F8 03 74 0A 83 F8 02 74 05 83 F8 05 75 40 D9 05 ?? ?? ?? ?? D8 D1 DF E0 DD D9 F6 C4 41 75 2F DE E9 DC 1D ?? ?? ?? ?? DF E0 F6 C4 41 75 24",
                                  "HL-8684", "55 8B EC 83 EC 10 A1 ?? ?? ?? ?? 56 33 F6 3B C6 0F",
								  "Sven-5.25-Compat", "83 EC 44 A1 ?? ?? ?? ?? 33 C4 89 44 24 40 83 3D ?? ?? ?? ?? ?? 0F 85 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? DD 54 24 08 D9 EE 83 F8 03 74 0A 83 F8 02 74 05 83 F8 05 75 40 D9 05 ?? ?? ?? ?? D8 D1 DF E0 DD D9 F6 C4 41 75 2F DE E9 DC 1D ?? ?? ?? ?? DF E0 F6 C4 41 75 24",
                                  "HL-9920", "55 8B EC 83 EC 08 83 3D ?? ?? ?? ?? ?? 0F 85 34");
        //HL-9920 has mulsd   xmm0, ds:qword_103066C8 which is four bytes length, so I've cut two bytes from the pattern not to modify the existing code.
		PATTERNS(g_fGameSpeed, "Sven-5.25", "DC 0D ?? ?? ?? ?? DD 1C 24 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 8C 24 8C 00 00 00 83 C4 0C 5E 5D 5F 5B 33 CC E8 ?? ?? ?? ?? 83 C4 74",
                              "HL-8684", "DC 0D ?? ?? ?? ?? DD 1C 24 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C 5E 5B 5F 8B E5 5D",
                              "HL-9920", "59 05 ?? ?? ?? ?? F2 0F 11 04 24 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 4D FC 83 C4 0C 33 CD 5E 5F 5B E8 ?? ?? ?? ?? 8B E5 5D",
							  "Sven-5.26", "DC 0D ?? ?? ?? ?? DD 1C 24 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 8C 24 8C 00 00 00 83 C4 0C 5F 5D 5B 5E 33 CC E8 ?? ?? ?? ?? 83 C4 74 C3");
		PATTERNS(g_alpUserMsgs, "Sven-5.25", "8B ?? ?? ?? ?? ?? 57 33 FF 85 F6 74 21 8D 46 08 50 53 E8 ?? ?? ?? ?? 83 C4 08 85 C0 75 09 8B 46 1C 8B FE 3B C5 74 49 8B 76 18 85 F6",
                               "HL-8684", "8B 35 ?? ?? ?? ?? 57 33 FF 85 F6 74 24",
                               "HL-9920", "8B 35 ?? ?? ?? ?? 57 33 FF 85 F6 74 22 8D 46 08 50 53 E8 ?? ?? ?? ?? 83 C4 08 85 C0 75 0A 8B 46 1C 8B FE 3B 45 0C 74 4C");
		PATTERN(CL_Move_fld_st0_cl_commands, "Sven-5.25", "D9 05 ?? ?? ?? ?? D9 5E 38 C7 46 40 00 00 00 00 C7 46 44 00 00 00 00 C7 46 48 00 00 00 00 E8 ?? ?? ?? ?? 33 DB 83 3D ?? ?? ?? ?? ?? 0F 94 C3 E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? FF 35 ?? ?? ?? ?? E8 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? 83 C4 10 53 56 51 D9 1C 24");
		PATTERNS(Cbuf_InsertText, "Sven-5.25", "53 8B 5C 24 ?? 56 8B 35 ?? ?? ?? ?? 53 E8 ?? ?? ?? ?? 03 C6",
                                 "HL-8684", "55 8B EC 53 8B 5D 08 56 8B 35 ?? ?? ?? ?? 57 53 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 03 C6 83 C4 04 3B C1 7C 12 68 ?? ?? ?? ?? E8",
                                 "HL-9920", "55 8B EC 53 8B 5D ?? 56 8B 35 ?? ?? ?? ?? 53 E8 ?? ?? ?? ?? 03 C6 83 C4 04 3B 05 ?? ?? ?? ?? 7C ?? 5E 5B C7 45 ?? ?? ?? ?? ?? 5D E9 ?? ?? ?? ?? 57 85 F6 74 ?? 56 E8 ?? ?? ?? ?? 56 FF 35 ?? ?? ?? ??");
		PATTERNS(Cbuf_AddText, "Sven-5.25", "56 8B 74 24 ?? 56 E8 ?? ?? ?? ?? 03 05 ?? ?? ?? ?? 83 C4 04 3B 05 ?? ?? ?? ?? 7C ?? 5E C7 44 24 04 68 34",
                              "HL-8684", "55 8B EC 56 8B 75 08 56 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 83 C4 04 03 C8 A1 ?? ?? ?? ?? 3B C8 7C 10 68",
                              "HL-9920", "55 8B EC 56 8B 75 08 56 E8 ?? ?? ?? ?? 03 ?? ?? ?? ?? ?? 83 C4 04 3B ?? ?? ?? ?? ?? 7C 0E 5E C7 45 ?? ?? ?? ?? ?? 5D E9 ?? ?? ?? ?? 56 E8 ?? ?? ?? ?? 50 56 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 10 5E 5D ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 6A FF 6A 01 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 6A 01 6A 00 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 18 ??",
							  "Sven-5.26", "E8 ?? ?? ?? ?? E9 A0 FE FF FF");
		PATTERNS(SPR_Set, "Sven-5.25", "8B 44 24 ?? 83 C0 FF 78 ?? 3B 05 ?? ?? ?? ?? 7D ?? 8D 0C ?? A1 ?? ?? ?? ?? 8D 04 ?? 85 C0 74 ?? 8B 00 85 C0 74 ?? 8B 80 ?? ?? ?? ?? A3 ?? ?? ?? ??",
                         "HL-8684", "55 8B EC 83 EC 08 8D 45 14 8D 4D 10 50 8D 55 0C 51 8D 45 08 52 50 FF 15 ?? ?? ?? ?? 8B 4D 08 51 E8 ?? ?? ?? ?? 83 C4 14 85 C0 74 6A 50 E8 ?? ?? ?? ?? 83 C4 04");
		PATTERNS(Draw_FillRGBA, "Sven-5.25", "56 68 E1 0D 00 00 FF 15 ?? ?? ?? ?? 68 E2 0B 00 00 FF 15 ?? ?? ?? ?? D9 05 ?? ?? ?? ?? 51 D9 1C 24 68 00 22 00 00 68 00 23 00 00 FF 15 ?? ?? ?? ?? 6A 01",
                               "HL-8684", "55 8B EC 83 EC 08 8D 45 24 8D 4D 20 50 8D 55 1C 51 8D 45 18 52 8D 4D 14 50 8D 55 10 51 8D 45 0C 52 8D 4D 08 50 51 FF 15 ?? ?? ?? ?? 83 C4 20 68 E1 0D 00 00 FF 15 ?? ?? ?? ?? 68 E2 0B 00 00 FF 15 ?? ?? ?? ?? 68 00 00 04 46 68 00 22 00 00 68 00 23 00 00 FF 15 ?? ?? ?? ?? 6A 01");
		PATTERNS(MSG_WriteUsercmd, "Sven-5.25", "56 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 74 24 0C C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 6A 00 8B 4E 10 03 4E 08 89 ?? ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? FF 30 6A 01 FF 74 24 1C FF 74 24 24 E8 ?? ?? ?? ?? 56 E8 ?? ?? ?? ?? 83 C4 1C 5E C3",
                                  "HL-8684", "55 8B EC 56 57 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B",
                                  "HL-9920", "55 8B EC 56 57 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B F8 83 C4 04 85 FF 74 05 83 3F 00 75 18 68 67 03 00 00 68 ?? ?? ?? ?? 68");
		PATTERNS(Cmd_AddCommandWithFlags_mov_dword_ptr_ds_cmd_functions_eax, "Sven-5.25", "A3 ?? ?? ?? ?? 5F 83 C4 10 C3 57 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 5E 5F 83 C4 10 C3 89 1E 8B 44 24 0C 89 0B 5B 5E A3",
                                                                            "HL-8684", "35 ?? ?? ?? ?? 85 F6 74 17 8B 46 04 50 53 E8 ?? ?? ?? ?? 83 C4 08 85 C0 74 25 8B 36 85 F6 75 E9",
                                                                            "HL-9920", "35 ?? ?? ?? ?? 85 F6 74 1A 0F 1F 40 00 FF 76 04 57 E8 ?? ?? ?? ?? 83 C4 08 85 C0 74 78 8B 36 85 F6 75 EA");
		PATTERNS(MSG_BeginReading_mov_dword_ptr_msg_readcount, "Sven-5.25", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 8D 51 01 3B ?? ?? ?? ?? ?? 7E 0E",
                                                              "HL-8684", "C0 A3 ?? ?? ?? ?? A3 ?? ?? ?? ?? ?? ?? ?? ?? A1 ?? ?? ?? ?? 8B 15 ?? ?? ?? ?? 8D 48 01 3B CA",
                                                              "HL-9920", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 75 09 A1 ?? ?? ?? ?? C1 E0 03 ?? 8B");
		PATTERNS(MSG_ReadString, "Sven-5.25", "8B ?? ?? ?? ?? ?? 33 C0 53 55 8B ?? ?? ?? ?? ?? 56 57 8B ?? ?? ?? ?? ?? 8D 4A 01 3B CF 7F 24 8A 1C 2A",
                                "HL-8684", "53 56 33 DB 33 F6 E8 ?? ?? ?? ?? 83 F8 FF 74 13",
                                "HL-9920", "8B 15 ?? ?? ?? ?? 33 C0 53 56 57 8B 3D ?? ?? ?? ?? 8D 4A ?? 3B CF 7F ?? 8B 1D ?? ?? ?? ?? 8A 1C ?? 8B D1 89 15 ?? ?? ?? ?? 80 FB FF 74 ?? 84 DB 74 ?? 88 98 ?? ?? ?? ?? 40 3D FF 1F 00 00 72 ??");
		PATTERNS(CL_Parse_StuffText, "Sven-5.25", "56 E8 ?? ?? ?? ?? 8B F0 8B D6 8D 42 01 8D 49 00 8A 0A 42 84 C9 75 F9 D9 EE 2B D0 D8 1D",
                                    "HL-8684", "56 E8 ?? ?? ?? ?? 8B F0 56 E8 ?? ?? ?? ?? 83 C4 04 85",
                                    "HL-9920", "56 57 E8 ?? ?? ?? ?? 8B F0 56 E8 ?? ?? ?? ?? 6A 00 E8 ?? ?? ?? ?? 8B F8 83 C4 08 85 FF 0F 84 ?? ?? ?? ?? 68 ?? ?? ?? ?? 57");
		PATTERNS(CL_Parse_Disconnect, "Sven-5.26", "51 56 E8 ?? ?? ?? ?? 8B F0 E8 ?? ?? ?? ?? 85 F6 0F 84 96 01 00 00 8B CE 8D 51 01 EB 03 8D 49 00 8A 01 41 84 C0 75 F9 2B CA 0F 84 7D 01 00 00 68 ?? ?? ?? ?? 6A 01 0F AE E8 E8 ?? ?? ?? ?? 56 6A 01 E8 ?? ?? ?? ?? 6A 0F 68 ?? ?? ?? ?? 56 E8 ?? ?? ?? ?? 83 C4 1C 85 C0 75 1C 6A 01 6A 01",
									  "HL-8684", "56 E8 ?? ?? ?? ?? 8B F0 E8 ?? ?? ?? ?? 85 F6 0F 84 74 01 00 00 57 8B FE 83 C9 FF 33 C0 F2 AE F7 D1 49 5F 0F 84 60 01 00 00 68 ?? ?? ?? ?? 6A 01 E8 ?? ?? ?? ?? 56 6A 01 E8 ?? ?? ?? ?? 6A 15 68 ?? ?? ?? ?? 56 E8 ?? ?? ?? ?? 83 C4 1C 85 C0 75 1B 6A 01 6A 01 E8 ?? ?? ?? ?? 83 C4 08 68 ?? ?? ?? ?? E8");
		PATTERNS(Cmd_Messagemode2_f_mov_dword_ptr_ds_key_dest, "Sven-5.25", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 ?? ?? ?? ?? D9 E8 D8 1D ?? ?? ?? ?? DF E0 F6 C4 41 0F 8B ?? ?? ?? ?? E9",
                                                              "HL-8684", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08",
                                                              "HL-9920", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08");
		PATTERNS(SCR_NetGraph_mov_ebx_cls_netchan_incoming_sequence, "Sven-5.25", "8B ?? ?? ?? ?? ?? 33 C9 2B ?? ?? ?? ?? ?? 33 ED 8B ?? ?? ?? ?? ?? 43 83 C4 10 89 6C 24 24 89 4C 24 18 89 4C 24 14 89 4C 24 1C 3B ?? ?? ?? ?? ?? 0F 8F ?? ?? ?? ?? DD 05 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? D9 E8 DD 05",
                                                                    "HL-8684", "A1 ?? ?? ?? ?? 83 EC 08 D9 05 ?? ?? ?? ?? 25 FF 03 00 00 33 D2 DD 1C 24 8D 0C C0 8B 45 14 66 8B 54 48 0E 8D 8D F8 FE FF FF 52 68 ?? ?? ?? ?? 68 00 01 00 00 51 E8 ?? ?? ?? ?? 8D 95 F8 FE FF FF 52 68 33 33 33 3F 68 66 66 66 3F 68 66 66 66 3F",
                                                                    "HL-9920", "A1 ?? ?? ?? ?? 83 C6 0F 48 25 FF 03 00 00 8D 04 40 8B 7C 87 04 85 FF 75 08 8B 3D ?? ?? ?? ?? EB 06 89 3D",
																	"Sven-5.26", "8B 1D ?? ?? ?? ?? 33 C9 2B 1D ?? ?? ?? ?? 33 ED 8B 15 ?? ?? ?? ?? 43 83 C4 10 89 4C 24 18 89 6C 24 24 89 4C 24 14 89 4C 24 1C 3B 1D ?? ?? ?? ?? 0F 8F C7 01 00 00 DD 05 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? D9 E8 DD 05 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? DD 05 ?? ?? ?? ?? 23 D3 8B F3 69 FA 18 85 00 00 81 E6 FF 03 00 00 8B 87 ?? ?? ?? ?? 89 04 F5 ?? ?? ?? ?? 85 C0 74 05 41 89 4C 24 18");
        //The same thing here as with g_fGameSpeed for HL-9920
		PATTERNS(V_SetRefParams_fld_st0_cl_time, "Sven-5.25", "DD 05 ?? ?? ?? ?? D9 5E 40 DD 05 ?? ?? ?? ?? D9 5E 3C 39 ?? ?? ?? ?? ?? 0F 95 C0 89 46 44 33 C0 39 ?? ?? ?? ?? ?? C7 46 4C 00 00 00 00 0F 95 C0 89 46 48 33 C0 83 3D ?? ?? ?? ?? ?? 0F 95 C0 89 46 50 A1 ?? ?? ?? ?? 89 46 54 D9 05 ?? ?? ?? ?? D9 5E 58 D9 05",
                                                "HL-8684", "DD 05 ?? ?? ?? ?? 89 4E 48 8B 0D ?? ?? ?? ?? 0F 95 C2 33 C0 83 F9 FF 8B 0D ?? ?? ?? ?? 89 56 4C 8B 15 ?? ?? ?? ?? 89 4E 54 D9 5E 40 8B 0D ?? ?? ?? ?? 89 56 58 8B 15",
                                                "HL-9920", "10 05 ?? ?? ?? ?? 66 0F 5A C0 89 85 58 FF FF FF 33 C0 39 05 ?? ?? ?? ?? 0F 95 C0 89 85 5C FF FF FF 33 C0 39 05 ?? ?? ?? ?? F3 0F 11 85 54 FF FF FF F2 0F 10 05 ?? ?? ?? ?? 0F 95 C0 66 0F 5A C0 89 85 60 FF FF FF 33 C0 83 3D ?? ?? ?? ?? ?? 0F 95 C0 89 85 64 FF FF FF A1 ?? ?? ?? ?? F3 0F 11 85 50 FF FF FF F3 0F 10 05 ?? ?? ?? ?? F3 0F 11 85 6C FF FF FF F3 0F 10 05 ?? ?? ?? ?? F3 0F 11 85 70 FF FF FF F3 0F 10 05 ?? ?? ?? ?? F3 0F 11 85 74 FF FF FF");
		PATTERNS(ClientDLL_CheckStudioInterface_push_engine_studio_api, "Sven-5.25", "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 FF D0 83 C4 0C 85 C0 75 12 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 A3 ?? ?? ?? ?? 5E C3",
                                                                       "HL-8684", "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 FF D0 83 C4 0C 85 C0 75 12",
                                                                       "HL-9920", "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 FF D0 83 C4 0C 85 C0 75 12 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 E8");
		PATTERNS(R_StudioDrawPlayer, "Sven-5.26", "55 8B EC 83 E4 F8 81 EC EC 0B", "Sven-5.25", "55 8B EC 83 E4 F8 81 EC EC 0B 00 00 A1 ?? ?? ?? ?? 33 C4 89 84 24 E8 0B 00 00 53 56 57 8B 7D 0C 8B 77 04 83 EE 01 89 ?? ?? ?? ?? ?? 0F 88 ?? ?? ?? ?? 3B ?? ?? ?? ?? ?? 0F 8D ?? ?? ?? ?? D9 05 ?? ?? ?? ?? D9 EE DA E9 DF E0 F6 C4 44 7A 0F E8 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 85 C0");
		PATTERNS(R_StudioDrawPlayer_mov_eax_currententity, "Sven-5.25", "A1 ?? ?? ?? ?? 69 CE 0C 02 00 00 C6 81 ?? ?? ?? ?? ?? 8B 80 94 0B 00 00 39 ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? E9 ?? ?? ?? ?? 50 69 C6 0C 02 00 00 05 ?? ?? ?? ?? 50 E8 ?? ?? ?? ?? 83 C4 08 85 C0 0F 84 ?? ?? ?? ?? 69");
		PATTERNS(GetClientColor_movsx_eax_word_ptr_ds_g_pExtraPlayerInfo_10, "Sven-5.25", "0F BF ?? ?? ?? ?? ?? 48 83 F8 03 77 1F FF 24 ?? ?? ?? ?? ?? B8 ?? ?? ?? ?? C3 B8 ?? ?? ?? ?? C3 B8 ?? ?? ?? ?? C3 B8 ?? ?? ?? ?? C3 B8 ?? ?? ?? ?? C3 85 C9 B8 ?? ?? ?? ?? BA ?? ?? ?? ?? 0F 45 C2");
		PATTERNS(V_EngineStudio_Init_push_g_pEngineStudio, "Sven-5.25", "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 FF D0 83 C4 0C 85 C0 75 12 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 A3 ?? ?? ?? ?? 5E",
                                                          "HL-8684", "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 FF D0 83 C4 0C 85 C0 75 12",
                                                          "HL-9920", "68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 FF D0 83 C4 0C 85 C0 75 12 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 E8");
		PATTERNS(Cvar_RegisterVariable, "Sven-5.25", "83 EC 14 53 56 57 8B 7C",
			"HL-8684", "55 8B EC 83 EC 14 53 56 8B 75 08 57 8B 06 50 E8 ?? ?? ?? ?? 83 C4 04 85 C0 74 17 8B 0E",
            "HL-9920", "55 8B EC 83 EC 14 57 8B 7D ?? FF 37 E8 ?? ?? ?? ?? 8B 0F 83 C4 04 51 85 C0 74 ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 5F 8B E5 5D C3 E8 ?? ?? ?? ?? 83 C4 04 85 C0 74 ?? FF 37 68 ?? ?? ?? ??");
		PATTERNS(CL_Move_mov_esi_cls_netchan_outgoing_sequence, "Sven-5.25", "23 1D ?? ?? ?? ?? 33 C0 6B F3 4C 6A 34 66 89 44 24 18 8D 44 24 7C 6A 00",
			"HL-8684", "8B 35 ?? ?? ?? ?? 8D 85 88 F7 FF FF 89 45 EC A1 ?? ?? ?? ?? 23 C6 6A 34 DD 05 ?? ?? ?? ?? 8D 0C C0 57",
            "HL-9920", "A1 ?? ?? ?? ?? 83 C4 08 2B 05 ?? ?? ?? ?? 81 3D ?? ?? ?? ?? ?? ?? ?? ?? 7C 0E BE 3E 00 00 00 2B F7 3B C6 0F 4E F0 EB 0C");
		PATTERNS(CL_Parse_Director, "Sven-5.25", "E8 ?? ?? ?? ?? 01 05",
			"HL-8684", "56 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8B F0",
			"HL-9920", "56 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8B F0");
		//5.25 pattern works for 5.26 too
		PATTERNS(MSG_ReadByte, "HL-8684", "A1 ?? ?? ?? ?? 8B 15 ?? ?? ?? ?? 8D 48 01 3B CA 7E 0E C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 83 C8 FF C3 56",
							   "Sven-5.25", "8B 0D ?? ?? ?? ?? 8D 51 01 3B 15 ?? ?? ?? ?? 7E 0E C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 83 C8 FF C3 A1 ?? ?? ?? ?? 0F B6");
		//5.26 pattern works for 5.25 too
		PATTERNS(MSG_WriteString, "HL-8684", "55 8B EC 56 8B 75 0C 85 F6 75",
							   "Sven-5.26", "8B 4C 24 08 85 C9 75 1B");
		//5.26 pattern works for 5.25 too
		PATTERNS(CL_Parse_ServerInfo, "Sven-5.26", "E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? D9 E8",
			"HL-8684", "E8 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C3");
		//5.26 pattern works for 5.25 too
		PATTERNS(MSG_ReadLong, "Sven-5.26", "56 8B 35 ?? ?? ?? ?? 57 8D 7E 04",
							   "HL-8684", "8B 0D ?? ?? ?? ?? A1 ?? ?? ?? ?? 57 8D 79 04");
		//5.26 pattern works for 5.25 too
		PATTERNS(CL_Parse_SignonNum, "Sven-5.26", "E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 3B C1 7F 17 51 50 68 ?? ?? ?? ?? 0F AE E8 E8 ?? ?? ?? ?? 83 C4 0C E9 ?? ?? ?? ?? A3 ?? ?? ?? ?? E9",
									 "HL-8684", "E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 3B C1 7F 14 51 50 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C E9 ?? ?? ?? ?? A3 ?? ?? ?? ?? E9");
		PATTERNS(SCR_EndLoadingPlaque, "Sven-5.26", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 ?? C7 05",
									   "HL-8684", "33 C0 A3 ?? ?? ?? ?? A3 ?? ?? ?? ?? E8");
		PATTERNS(CL_Move_call_MSG_WriteByte_outgoing_seq_stuff, "Sven-5.26", "E8 ?? ?? ?? ?? 6A 34 8D 44 24 50 6A 00 50 E8 ?? ?? ?? ?? 8D 7E FF 83 C4 14 03 FD 8D 4C 24 44 78 30 8D 64 24 00 A1 ?? ?? ?? ?? 2B C7 23 05 ?? ?? ?? ?? 6B F0 4C 8D 44 24 10 51 81 C6 ?? ?? ?? ?? 56 50 E8 ?? ?? ?? ?? 83 C4 0C 8B CE",
																"HL-8684", "E8 ?? ?? ?? ?? 6A 34 8D 4D 88 6A 00 51 E8 ?? ?? ?? ?? 8D 74 3E FF 83 C4 14 85 F6 8D 4D 88 7C 2F A1 ?? ?? ?? ?? 8B 3D ?? ?? ?? ?? 2B C6 51 23 C7 8D 4D E4 8D 14 C0 8D 04 50 8D 3C 85 ?? ?? ?? ?? 57 51 E8");
		PATTERNS(CL_Move_call_MSG_WriteByte_numbackup_pranks, "Sven-5.26", "E8 ?? ?? ?? ?? 8B 35 ?? ?? ?? ?? 83 C4 08 2B 35 ?? ?? ?? ?? 81 3D ?? ?? ?? ?? ?? ?? ?? ?? 7C 0F B8 3E 00 00 00 2B C5 3B F0 7E 0E 8B F0 EB 0A");
		//probably useless because we have SZ_GetSpace and cls.netchan.message so we can write as much stuff as we want.
		//PATTERNS(MSG_WriteByte, "HL-8684", "E8 ?? ?? ?? ?? 8D 45 E4 6A 00",
								//"Sven-5.26", "E8 ?? ?? ?? ?? 57 53 68");
		PATTERNS(CL_ParseClientdata_g_flLatency, "Sven-5.26", "DD 1D ?? ?? ?? ?? E8 ?? ?? ?? ?? 6A 01 E8 ?? ?? ?? ?? 83 C4 08 89 44 24 10 85 C0 75 0A 8B 74 24 18 8D 4C 24 78 EB 20 6A 10 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8B F0 23 CE 83 C4 04 69 C9 18 85 00 00 81 C1",
												"HL-8684", "89 0D ?? ?? ?? ?? 74 11 5F 5E C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 5B 8B E5 5D C3");
		//The signature points to a call insn which calls MSG_ReadByte (the opcode received from the server, e.g. svc_disconnect)
		PATTERNS(CL_ParseServerMessage_call_MSG_ReadByte, "HL-8684", "E8 ?? ?? ?? ?? 8B F0 83 FE FF 0F 84 98 02 00 00 83 FE 01 74 30 8B 0D ?? ?? ?? ?? 8B 15 ?? ?? ?? ?? 8B C1 83 E0 1F 8D 04 40 C1 E0 02 41 89 B0 ?? ?? ?? ?? 89 98 ?? ?? ?? ?? 89 90 ?? ?? ?? ?? 89 0D", 
			"Sven-5.26", "E8 ?? ?? ?? ?? 8B F8 83 FF FF 0F 84 A7 04 00 00 83 FF 01 0F 84 BB 01 00 00 8B 15 ?? ?? ?? ?? 8B C2 83 E0 1F 42 89 15 ?? ?? ?? ?? 8D 0C 40 A1 ?? ?? ?? ?? 89 3C 8D ?? ?? ?? ?? 89 2C 8D ?? ?? ?? ?? 89 04 8D");
		PATTERNS(SV_ExecuteClientMessage_call_MSG_ReadByte, "HL-8684", "E8 ?? ?? ?? ?? 83 F8 FF 74 47 85 C0 7C 28 83 F8 0B 7F 23 8D 0C 40 8B 04 8D ?? ?? ?? ?? 85 C0 74 D6 56 FF D0 EB CE",
			"Sven-5.26", "E8 ?? ?? ?? ?? 83 F8 FF 74 4E 83 F8 0B 77 2E 8D 04 40 8B 04 85 ?? ?? ?? ?? 85 C0 74 06 57 FF D0 83 C4 04");
	}
	namespace client {
		PATTERNS(Initialize_mov_edi_g_pEngfuncs, "Sven-5.25-5.26", "BF ?? ?? ?? ?? F3 A5 75 63 8B F0 B9 86 00 00 00 BF ?? ?? ?? ?? F3 A5 E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 47 A1 ?? ?? ?? ?? 68 ?? ?? ?? ?? FF D0 A3 ?? ?? ?? ?? A1 ?? ?? ?? ?? 68",
												 "HL-8684", "BF ?? ?? ?? ?? F3 A5 83 BC 24 10 02 00 00 07 74 0B 5F 33 C0 5E 81 C4 00 02 00 00 C3",
												 "HL-Compat", "FF 15 ?? ?? ?? ?? 83 C4 04 89 ?? ?? ?? ?? ?? B8 01 00 00 00 5F 5E 81 C4 00 10 00 00 C3");
	}
}

#ifdef _CONCATENATE_INTERNAL_WAS_DEFINED_BY_US
#undef __CONCATENATE_INTERNAL__
#endif //_CONCATENATE_INTERNAL_WAS_DEFINED_BY_US

#ifdef _CONCATENATE_WAS_DEFINED_BY_US
#undef __CONCATENATE__
#endif //_CONCATENATE_WAS_DEFINED_BY_US

#endif //PATTERNS_HPP_GUARD

#undef PATTERNS_HPP_RECURSE_GUARD
#endif //PATTERNS_HPP_RECURSE_GUARD
