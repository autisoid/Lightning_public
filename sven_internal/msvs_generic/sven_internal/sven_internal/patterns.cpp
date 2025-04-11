/**
 * Copyright - xWhitey, 2023.
 * patterns.cpp - Implementation of the pattern system.
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"

//SPTLib
unsigned char UTIL_CharToHex(char _TheChar) {
	return (_TheChar >= '0' && _TheChar <= '9') ? (unsigned char)(_TheChar - '0')
		: (_TheChar >= 'a' && _TheChar <= 'f') ? (unsigned char)(_TheChar - 'a' + 10)
		: (_TheChar >= 'A' && _TheChar <= 'F') ? (unsigned char)(_TheChar - 'A' + 10) : '\0';
}

CIntelligentPattern::CIntelligentPattern(_In_z_ const char* _IDAPattern, _In_z_ const char* _Version) {
	m_pszVersion = _Version;

	if (_IDAPattern == NULL || strlen(_IDAPattern) == 0) {
		return;
	}

	size_t cbPatternSize = 1;
	
	const char* pszTemp = strchr(_IDAPattern, ' ');
	while (pszTemp != NULL && *pszTemp != '\0') {
		cbPatternSize++;
		pszTemp = strchr(pszTemp + 1, ' ');
	}

	m_pszMask = (char*)(malloc(cbPatternSize + 1));
	memset(m_pszMask, 0, cbPatternSize + 1);
	m_pcPattern = (unsigned char*)(malloc(cbPatternSize));

	const char* pszFirstSpace = strchr(_IDAPattern, ' ');

	size_t cbCurrentIndex = 0;

	if (pszFirstSpace != NULL) {
		size_t cbFirstPartLength = pszFirstSpace - _IDAPattern; // Length of the first part

		char* pszFirstPart = (char*)malloc(cbFirstPartLength + 1);
		if (pszFirstPart == NULL) {
			return;
		}

		strncpy_s(pszFirstPart, cbFirstPartLength + 1, _IDAPattern, cbFirstPartLength);
		pszFirstPart[cbFirstPartLength] = '\0';

		if (!Q_strnicmp_s(pszFirstPart, cbFirstPartLength + 1, "??", sizeof("??") - 1)) {
			m_pszMask[cbCurrentIndex] = '?';
			m_pcPattern[cbCurrentIndex] = 0xCC;
			cbCurrentIndex++;
		} else {
			m_pszMask[cbCurrentIndex] = 'x';
			m_pcPattern[cbCurrentIndex] = UTIL_CharToHex(pszFirstPart[0]) * 16 + UTIL_CharToHex(pszFirstPart[1]);
			cbCurrentIndex++;
		}

		free(pszFirstPart);

		const char* pszPart = pszFirstSpace + 1;

		while (pszPart != NULL && *pszPart != '\0') {
			const char* pszNextSpace = strchr(pszPart, ' ');
			size_t cbPartLength = (pszNextSpace != NULL) ? (pszNextSpace - pszPart) : strlen(pszPart);

			char* pszPartial = (char*)malloc(cbPartLength + 1);
			if (pszPartial == NULL) {
				return;
			}

			strncpy_s(pszPartial, cbPartLength + 1, pszPart, cbPartLength);
			pszPartial[cbPartLength] = '\0';

			if (!Q_strnicmp_s(pszPartial, cbFirstPartLength + 1, "??", sizeof("??") - 1)) {
				m_pszMask[cbCurrentIndex] = '?';
				m_pcPattern[cbCurrentIndex] = 0xCC;
				cbCurrentIndex++;
			} else {
				m_pszMask[cbCurrentIndex] = 'x';
				m_pcPattern[cbCurrentIndex] = UTIL_CharToHex(pszPartial[0]) * 16 + UTIL_CharToHex(pszPartial[1]);
				cbCurrentIndex++;
			}

			free(pszPartial);

			if (pszNextSpace != NULL) {
				pszPart = pszNextSpace + 1;
			} else {
				pszPart = NULL;
			}
		}
	} else {
		if (!Q_strnicmp_s(_IDAPattern, strlen(_IDAPattern) + 1, "??", sizeof("??") - 1)) {
			m_pszMask[cbCurrentIndex] = '?';
			m_pcPattern[cbCurrentIndex] = 0xCC;
			cbCurrentIndex++;
		}
		else {
			m_pszMask[cbCurrentIndex] = 'x';
			m_pcPattern[cbCurrentIndex] = UTIL_CharToHex(_IDAPattern[0]) * 16 + UTIL_CharToHex(_IDAPattern[1]);
			cbCurrentIndex++;
		}
	}
}

CIntelligentPattern::~CIntelligentPattern() {
	free(m_pszMask);
	free(m_pcPattern);
}

CIntelligentPattern* CIntelligentPatterns::MakePattern(_In_z_ const char* _IDAPattern, _In_z_ const char* _Version) {
	return new CIntelligentPattern(_IDAPattern, _Version);
}

std::vector<CIntelligentPattern*> CIntelligentPatterns::MakePatterns(std::vector<const char*> _Patterns, std::vector<const char*> _Versions) {
	assert(_Patterns.size() == _Versions.size() && "must be");

	std::vector<CIntelligentPattern*> result = {};

	for (unsigned int idx = 0; idx < _Patterns.size(); idx++) {
		result.push_back(MakePattern(_Patterns.at(idx), _Versions.at(idx)));
	}

	return result;
}