/**
 * Copyright - xWhitey, 2023.
 * custom_hud.hpp - description
 */

#ifdef CUSTOM_FONTS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in custom_fonts.hpp
#else //CUSTOM_FONTS_HPP_RECURSE_GUARD

#define CUSTOM_FONTS_HPP_RECURSE_GUARD

#ifndef CUSTOM_FONTS_HPP_GUARD
#define CUSTOM_FONTS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define CUSTOMFONTS_CUSTOM_FONTS_INI_PATH "./lightning.tech/fonts/fonts.ini"

typedef struct CCustomFont {
	CCustomFont(_In_z_ _Pre_z_ const char* _Name, _In_ char* _FileName, _In_ int _FontSize, _In_ float _flFontSizeInPixels, _In_opt_ bool _Compressed = false);

	void Init(_In_opt_ bool _Compressed);

	const char* m_pszName;
	char* m_pcBuffer;
	int m_iFontSize;
	int m_flFontSizeInPixels;
	ImFont* m_pFont;
	CCustomFont* m_pPrev, *m_pNext;
} CCustomFont;

using CCustomFont = struct CCustomFont;

typedef struct CCustomFonts {
	static CCustomFont* FindFontByName(_In_z_ const char* _Name);
	static void Load();
	static CConfigMgr* ms_pConfig;
} CCustomFonts;

using CCustomFonts = struct CCustomFonts;

extern CCustomFont* g_pFirstLoadedFont;

#else //__cplusplus
#error C++ compiler required to compile custom_fonts.hpp.
#endif //__cplusplus

#endif //CUSTOM_FONTS_HPP_GUARD

#undef CUSTOM_FONTS_HPP_RECURSE_GUARD
#endif //CUSTOM_FONTS_HPP_RECURSE_GUARD