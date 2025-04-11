#include "StdAfx.h"
#include "custom_fonts.hpp"
#include "AlbertSansFont.h"
#include "compress.hpp"
#include "ImGui/imgui_freetype.h"

CCustomFont* g_pFirstLoadedFont = nullptr;
CConfigMgr* CCustomFonts::ms_pConfig = nullptr;

CCustomFont::CCustomFont(_In_z_ _Pre_z_ const char* _Name, _In_ char* _Buffer, _In_ int _FontSize, _In_ float _flFontSizeInPixels, _In_opt_ bool _Compressed /* = false */) : m_pszName(std::move(_Name)), m_pcBuffer(_Buffer) {
	m_iFontSize = _FontSize;
	m_flFontSizeInPixels = _flFontSizeInPixels;
	Init(_Compressed);
}

void CCustomFont::Init(_In_opt_ bool _Compressed) {
	ImFontConfig config{};
	config.FontBuilderFlags |= (1 << 2);

	CCheat::GetCheat()->m_pConsole->Printf("CCustomFont::Init(bool): Loading %s (%s) font...\n", m_pszName, _Compressed == true ? "compressed" : "uncompressed, compressing it");

	if (_Compressed == true) {
		m_pFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(m_pcBuffer, m_flFontSizeInPixels, &config);
	} else {
		m_pFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(m_pcBuffer, m_iFontSize, m_flFontSizeInPixels, &config);
		Q_free(m_pcBuffer);
	}

	ImGui::GetIO().Fonts->Build();

	m_pPrev = nullptr;
	m_pNext = nullptr;
	 
	if (!g_pFirstLoadedFont) {
		g_pFirstLoadedFont = this;
	} else {
		CCustomFont* node = g_pFirstLoadedFont;

		while (node->m_pNext) {
			node = node->m_pNext;
		}

		m_pPrev = node;
		node->m_pNext = this;
	}
}

CCustomFont* CCustomFonts::FindFontByName(_In_z_ const char* _Name) {
	CCustomFont* font = g_pFirstLoadedFont;

	while (font) {
		if (!strcmp(font->m_pszName, _Name)) return font;

		font = font->m_pNext;
	}

	return nullptr;
}

void CCustomFonts::Load() {
	bool bExistedOnLaunch = false;

	if (!CFileMgr::DoesFileExist(CUSTOMFONTS_CUSTOM_FONTS_INI_PATH))
		CFileMgr::CloseFile(CFileMgr::OpenFile(CUSTOMFONTS_CUSTOM_FONTS_INI_PATH, "a+"));
	else bExistedOnLaunch = true;

	if (!ms_pConfig)
		ms_pConfig = Q_new(CConfigMgr)(CUSTOMFONTS_CUSTOM_FONTS_INI_PATH);

	if (ms_pConfig->m_bLoadFailed == true) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CCustomFonts::Load(): Failed to load \"" CUSTOMFONTS_CUSTOM_FONTS_INI_PATH "\": %d.\n", errno);

		return;
	}

	ms_pConfig->m_bExistedOnLaunch = bExistedOnLaunch;

	const char* szCurrentFontName = bExistedOnLaunch ? ms_pConfig->GetValue("Data", "ClientFont") : nullptr;

	CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CCustomFonts::Load(): Loading default font.\n");
	CCustomFont* font = Q_new(CCustomFont)("AlbertSans", indirect_cast<char*>(AlbertSans_compressed_data_base85), 20, 20.f, true);

	CSimpleIniA::TNamesDepend values;
	ms_pConfig->m_pIni->GetAllSections(values);
	for (auto section : values) {
		if (!strcmp(section.pItem, "Data")) continue;

		int fontSize = atoi(ms_pConfig->m_pIni->GetValue(section.pItem /* font name */, "Size", "10"));
		float fontSizeInPx = atof(ms_pConfig->m_pIni->GetValue(section.pItem /* font name */, "SizeInPixels", "10.0"));

		const functional_size_t length = (sizeof("./lightning.tech/fonts/") - 1) + strlen(section.pItem);
		char* szFileName = indirect_cast<char*>(Q_malloc(length + 1));
		memset(szFileName, 0, length + 1);

		sprintf_s(szFileName, length + 1, "./lightning.tech/fonts/%s", section.pItem);

		void* hFontFile = CFileMgr::OpenFile(szFileName, "rb");
		if (!hFontFile) {
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CCustomFonts::Load(): failed to load custom font \"%s\"\n", szFileName);
			//CFileMgr::CloseFile(hFontFile);

			continue;
		}

		fseek(reinterpret_cast<FILE*>(hFontFile), 0, SEEK_END);
		int size = ftell(reinterpret_cast<FILE*>(hFontFile));
		char* cFontBuffer = indirect_cast<char*>(Q_malloc(size + 4));
		cFontBuffer[size] = '\0';
		fseek(reinterpret_cast<FILE*>(hFontFile), 0, SEEK_SET);
		//rewind(reinterpret_cast<FILE*>(hFontFile));
		int read = fread(cFontBuffer, sizeof(char), size, reinterpret_cast<FILE*>(hFontFile));
		if (read != size) {
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CCustomFonts::Load(): failed to load custom font (read size is different) \"%s\"\n", szFileName);
			CFileMgr::CloseFile(hFontFile);

			continue;
		}
		memset((void*)(((char*)cFontBuffer) + size), 0, 4);
		CFileMgr::CloseFile(hFontFile);

		Q_free(szFileName);

		int iMaxLength = size + 512 + (size >> 2) + sizeof(int);
		char* acCompressed = (char*)(Q_malloc(sizeof(char) * iMaxLength));
		(void) stb_compress((stb_uchar*)acCompressed, (stb_uchar*)cFontBuffer, size);

		Q_free(cFontBuffer);

		CCustomFont* font = Q_new(CCustomFont)(section.pItem, acCompressed, fontSize, fontSizeInPx);
	}

	if (szCurrentFontName) {
		if (!strcmp(szCurrentFontName, "Default")) {
			g_pClientFont = g_pFirstLoadedFont->m_pFont;

			return;
		}

		if (CCustomFont* font = FindFontByName(szCurrentFontName)) {
			if (font->m_pFont) {
				g_pClientFont = font->m_pFont;
			} else {
				g_pClientFont = g_pFirstLoadedFont->m_pFont;
			}
		} else {
			g_pClientFont = g_pFirstLoadedFont->m_pFont;
		}
	} else {
		g_pClientFont = g_pFirstLoadedFont->m_pFont;
	}
}