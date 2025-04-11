/**
 * Copyright - xWhitey, 2024.
 * virtual_console.cpp - Damn. A fucking rewrite of the "virtual console" system... A two year old code got rewritten!
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"

CVirtualConCmd::CVirtualConCmd(_In_z_ const char* _Name, _In_ g_tpfnConsoleFunc _Handler) : m_pszName(_Name), m_pfnHandler(_Handler) {}
CVirtualConVar::CVirtualConVar(_In_z_ _Pre_z_ const char* _Name, _In_z_ char* _DefaultValue) : m_pszName(_Name), m_pszCurrentValue(_DefaultValue), m_pszDefaultValue(_DefaultValue) {
	m_pfnOnceSetCallback = nullptr;
}

void CVirtualConVar::DirectSet(_In_z_ char* _Value, _In_opt_ bool _MemFreePreviousValue /* = false */) {
	if (_MemFreePreviousValue == true && m_pszCurrentValue != m_pszDefaultValue)
		Q_free(m_pszCurrentValue);

	m_pszCurrentValue = _Value;
}

char* CVirtualConVar::Get() {
	return m_pszCurrentValue;
}

bool CVirtualConVar::AsBool() {
	bool result = m_pszCurrentValue[0] == '1';

	if (!result && !_strnicmp(m_pszCurrentValue, "true", 4))
		result = true;

	return result;
}

void CVirtualConVar::RegisterCallback(_In_ g_tpfnConVarOnceSetCallback _Callback, _In_opt_ void* _UserArg) {
	m_pfnOnceSetCallback = _Callback;
	m_pCallbackUserArg = _UserArg;
}

void CVirtualConVar::Set(_In_ struct CVirtualConsole* _Console) {
	if (_strnicmp(_Console->Cmd_Argv(0), m_pszName, strlen(m_pszName))) return;

	if (_Console->Cmd_Argc() == 1) {
		_Console->Printf("\"%s\" is \"%s\"\n", _Console->Cmd_Argv(0), m_pszCurrentValue);

		return;
	}

	if (_Console->Cmd_Argc() >= 2) {
		int nTotalSize = 0;
		int iArgsSize = _Console->m_pVecArgs->size();
		for (int idx = 1; idx < iArgsSize; idx++) {
			const char* pszArg = _Console->Cmd_Argv(idx);
			if (!pszArg) continue;
			nTotalSize += strlen(pszArg);
			if (idx < iArgsSize - 1) nTotalSize += sizeof(' ');
		}
		char* szBuffer = (char*)Q_malloc(sizeof(char) * (nTotalSize + 1));
		szBuffer[0] = '\0';
		int iCurrentPos = 0;
		int iRemainingSize = nTotalSize + 1;
		for (int idx = 1; idx < iArgsSize; idx++) {
			const char* szEntry = _Console->Cmd_Argv(idx);
			if (!szEntry) continue;

			size_t sLength = strlen(szEntry);

			strncpy_s(szBuffer + iCurrentPos, iRemainingSize, szEntry, sLength);

			iCurrentPos += strlen(szEntry);
			iRemainingSize -= sLength;

			if (idx < iArgsSize - 1 && iRemainingSize > 1) {
				szBuffer[iCurrentPos] = ' ';
				iCurrentPos++;
				iRemainingSize--;
			}
		}
		szBuffer[nTotalSize] = '\0';

		if (_stricmp(m_pszCurrentValue, m_pszDefaultValue) || m_pszCurrentValue != m_pszDefaultValue) {
			Q_free(m_pszCurrentValue);
		}

		if (m_pfnOnceSetCallback) {
			m_pfnOnceSetCallback(m_pCallbackUserArg, &szBuffer);
		}

		m_pszCurrentValue = szBuffer;
	}
}

CVirtualConsole::CVirtualConsole() {
	m_bScrollToBottom = false;
	m_pVecArgs = Q_new(CVector<const char*>)({});
	m_pVecItems = Q_new(CVector<char*>)({});
	m_pVecCommands = Q_new(CVector<CVirtualConCmd*>)({});
	m_pVecConVars = Q_new(CVector<CVirtualConVar*>)({});
	m_pVecHistory = Q_new(CVector<char*>)({});
	memset(m_acInputBuf, 0, Q_ARRAYSIZE(m_acInputBuf));
	m_nHistoryPos = -1;
}

CVector<char*>* CVirtualConsole::Tokenize(_In_z_ const char* _Text, _In_ char _Delimiter) {
	if (!_Text) {
		return nullptr;
	}

	const char* lpszStart = _Text;

	CVector<char*>* lpVecOutput = Q_new(CVector<char*>)({});

	while (*_Text != '\0') {
		if (*_Text == _Delimiter) {
			int iLength = _Text - lpszStart;
			char* lpszEntry = (char*)Q_malloc(sizeof(char) * (iLength + 1));
			strncpy_s(lpszEntry, iLength + 1, lpszStart, iLength);
			lpszEntry[iLength] = '\0';
			lpVecOutput->push_back(lpszEntry);
			lpszStart = _Text + 1;
		}

		_Text++;
	}

	int iLength = _Text - lpszStart;
	if (iLength > 0) {
		char* lpszEntry = (char*)Q_malloc(sizeof(char) * (iLength + 1));
		strncpy_s(lpszEntry, iLength + 1, lpszStart, iLength);
		lpszEntry[iLength] = '\0';
		lpVecOutput->push_back(lpszEntry);
	}

	return lpVecOutput;
}

void CVirtualConsole::Clear() {
	for (int idx = 0; idx < m_pVecItems->size(); idx++) {
		Q_free(m_pVecItems->at(idx));
	}

	m_pVecItems->clear();
}

void CVirtualConsole::Draw(_In_ bool& _Active) {
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Lightning Debug Console", reinterpret_cast<bool*>(&_Active), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)) {
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings)) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
			if (m_pVecItems->size() > 4096) {
				Clear(); //so we don't overflow our allocator in case of a shit ton of debug text
			}

			for (int i = 0; i < m_pVecItems->size(); i++) {
				const char* item = m_pVecItems->at(i);

				ImVec4 color;
				bool has_color = false;
				if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
				else if (strstr(item, "[info]")) { color = ImVec4(0.0f, 1.0f, 0.4f, 1.0f); has_color = true; }
				else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
				if (has_color)
					ImGui::PushStyleColor(ImGuiCol_Text, color);
				if (i != 0) {
					const char* szPreviousItem = m_pVecItems->at(i - 1);
					if (szPreviousItem[strlen(szPreviousItem) - 1] != '\n')
						ImGui::SameLine();
				}

				ImGui::TextUnformatted(item);
				if (has_color)
					ImGui::PopStyleColor();
			}

			if (m_bScrollToBottom)
				ImGui::SetScrollHereY(1.0f);
			m_bScrollToBottom = false;

			ImGui::PopStyleVar();
		}
		ImGui::EndChild();
		ImGui::Separator();

		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("##Input", m_acInputBuf, Q_ARRAYSIZE(m_acInputBuf), input_text_flags, &TextEditCallbackStub, this)) {
			InsertText(m_acInputBuf);
			memset(m_acInputBuf, 0, Q_ARRAYSIZE(m_acInputBuf));
			reclaim_focus = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Submit##InputBufSubmitter")) {
			if (m_acInputBuf[0] != '\0' && m_acInputBuf[0] != '\n') {
				InsertText(m_acInputBuf);
				memset(m_acInputBuf, 0, Q_ARRAYSIZE(m_acInputBuf));
				reclaim_focus = true;
			}
		}

		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1);

		ImGui::End();
	}
}

void CVirtualConsole::InsertText(_In_z_ const char* _Text, _In_opt_ bool _ShouldDisplayTheText /* = true */) {
	if (!m_pVecArgs->empty()) {
		for (int idx = 0; idx < m_pVecArgs->size(); idx++) {
			Q_free(static_cast<void*>(const_cast<char*>(m_pVecArgs->at(idx))));
		}
		
		m_pVecArgs->clear();
	}

	if (_ShouldDisplayTheText) {
		Printf("] %s\n", _Text);

		m_nHistoryPos = -1;
		for (int idx = m_pVecHistory->size() - 1; idx >= 0; idx--) {
			if (_stricmp(m_pVecHistory->at(idx), _Text) == 0) {
				Q_free(m_pVecHistory->at(idx));
				m_pVecHistory->erase(m_pVecHistory->begin() + idx);
				break;
			}
		}
		int nTextLength = strlen(_Text);
		char* lpszCopy = (char*)Q_malloc(sizeof(char) * (nTextLength + 1));
		lpszCopy[nTextLength - 1] = '\0';
		strncpy_s(lpszCopy, nTextLength + 1, _Text, nTextLength);
		m_pVecHistory->push_back(lpszCopy);
	}

	CVector<char*>* lpVecTokens = Tokenize(_Text, ' ');
	if (lpVecTokens->empty()) {
		AddText(_Text, _ShouldDisplayTheText);
		for (int idx = 0; idx < lpVecTokens->size(); idx++) {
			Q_free(static_cast<void*>(const_cast<char*>(lpVecTokens->at(idx))));
		}
		Q_delete(lpVecTokens);

		return;
	}

	char* lpszTheCommand = lpVecTokens->at(0);

	for (int idx = 0; idx < lpVecTokens->size(); idx++) {
		m_pVecArgs->push_back(lpVecTokens->at(idx));
	}

	AddText(lpszTheCommand, _ShouldDisplayTheText);

	for (int idx = 0; idx < lpVecTokens->size(); idx++) {
		Q_free(static_cast<void*>(const_cast<char*>(lpVecTokens->at(idx))));
	}
	Q_delete(lpVecTokens);
}

void CVirtualConsole::AddText(_In_z_ const char* _CommandLine, _In_opt_ bool _ShouldDisplayTheText /* = true */) {
	bool bHasFound = false;

	for (int idx = 0; idx < m_pVecCommands->size(); idx++) {
		if (_stricmp(_CommandLine, m_pVecCommands->at(idx)->m_pszName) == 0) {
			bHasFound = true;
			m_pVecCommands->at(idx)->m_pfnHandler(this);
			break;
		}
	}

	for (int idx = 0; idx < m_pVecConVars->size(); idx++) {
		if (_stricmp(_CommandLine, m_pVecConVars->at(idx)->m_pszName) == 0) {
			bHasFound = true;
			m_pVecConVars->at(idx)->Set(this);
			break;
		}
	}

	if (!bHasFound && _ShouldDisplayTheText)
		Printf("Unknown command: %s\n", _CommandLine);

	m_bScrollToBottom = true;
}

void CVirtualConsole::Printf(_In_z_ _Pre_z_ const char* _Format, _In_opt_ ...) {
	int iBufferSize = sizeof(char) * 1024;
	char* szBuffer = (char*) Q_malloc(iBufferSize);
	va_list args;
	va_start(args, _Format);
	vsnprintf(szBuffer, iBufferSize, _Format, args);
	szBuffer[iBufferSize - 1] = 0;
	va_end(args);
	m_pVecItems->push_back(szBuffer);
}

void CVirtualConsole::RegisterCommand(_In_ CVirtualConCmd* _ConCmd) {
	m_pVecCommands->push_back(_ConCmd);
}

void CVirtualConsole::RegisterConVar(_In_ CVirtualConVar* _ConVar) {
	m_pVecConVars->push_back(_ConVar);
}

const char* CVirtualConsole::Cmd_Argv(_In_ int _Index) {
	if (Cmd_Argc() == 0) return "Arguments are empty!";
	if (_Index > Cmd_Argc()) return "";

	return m_pVecArgs->at(_Index);
}

int CVirtualConsole::Cmd_Argc() {
	return m_pVecArgs->size();
}

int CVirtualConsole::TextEditCallbackStub(_In_ ImGuiInputTextCallbackData* _Data) {
	CVirtualConsole* lpConsole = (CVirtualConsole*)_Data->UserData;
	return lpConsole->TextEditCallback(_Data);
}

int CVirtualConsole::TextEditCallback(_In_ ImGuiInputTextCallbackData* _Data) {
	switch (_Data->EventFlag) {
	case ImGuiInputTextFlags_CallbackCompletion: {
		const char* word_end = _Data->Buf + _Data->CursorPos;
		const char* word_start = word_end;
		while (word_start > _Data->Buf) {
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		CVector<const char*> candidates;
		for (int i = 0; i < m_pVecCommands->size(); i++)
			if (_strnicmp(m_pVecCommands->at(i)->m_pszName, word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(m_pVecCommands->at(i)->m_pszName);
		for (int i = 0; i < m_pVecConVars->size(); i++)
			if (_strnicmp(m_pVecConVars->at(i)->m_pszName, word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(m_pVecConVars->at(i)->m_pszName);

		if (candidates.size() == 0) {
			Printf("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
		} else if (candidates.size() == 1) {
			_Data->DeleteChars(word_start - _Data->Buf, word_end - word_start);
			_Data->InsertChars(_Data->CursorPos, candidates[0]);
			_Data->InsertChars(_Data->CursorPos, " ");
		} else {
			int match_len = (int)(word_end - word_start);
			for (;;) {
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}

			if (match_len > 0) {
				_Data->DeleteChars((int)(word_start - _Data->Buf), (int)(word_end - word_start));
				_Data->InsertChars(_Data->CursorPos, candidates[0], candidates[0] + match_len);

				m_bScrollToBottom = true;
			}

			Printf("Possible matches:\n");
			for (int i = 0; i < candidates.size(); i++)
				Printf("- %s\n", candidates[i]);
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackHistory: {
		const int prev_history_pos = m_nHistoryPos;
		if (_Data->EventKey == ImGuiKey_UpArrow) {
			if (m_nHistoryPos == -1)
				m_nHistoryPos = m_pVecHistory->size() - 1;
			else if (m_nHistoryPos > 0)
				m_nHistoryPos--;
		} else if (_Data->EventKey == ImGuiKey_DownArrow) {
			if (m_nHistoryPos != -1)
				if (++m_nHistoryPos >= m_pVecHistory->size())
					m_nHistoryPos = -1;
		}

		if (prev_history_pos != m_nHistoryPos) {
			const char* history_str = (m_nHistoryPos >= 0) ? m_pVecHistory->at(m_nHistoryPos) : "";
			_Data->DeleteChars(0, _Data->BufTextLen);
			_Data->InsertChars(0, history_str);
		}
	}
	}
	return 0;
}