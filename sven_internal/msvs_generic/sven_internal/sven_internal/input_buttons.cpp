/**
 * Copyright - xWhitey, 2024.
 * input_buttons.cpp - "input buttons"
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "input_buttons.hpp"
#include "HwDLL.hpp"

CVector<CVirtualInputButton*>* CVirtualInputButtons::ms_pVecButtons = nullptr;

CInputButtons::CInputButtons() {
	m_usButtons = 0;

	m_pquQueuedButtons = Q_new(CQueue<EInputBtns>)();
	m_pquQueuedToBeUnholdButtons = Q_new(CQueue<EInputBtns>)();
}

void CInputButtons::Process(_In_ unsigned short _AlreadyHeldButtons) {
	m_usButtons = _AlreadyHeldButtons;
}

unsigned short CInputButtons::ProcessQueuedButtons() {
	while (!m_pquQueuedButtons->IsExhausted()) {
		m_usButtons |= static_cast<unsigned short>(m_pquQueuedButtons->Pop());
	}
	while (!m_pquQueuedToBeUnholdButtons->IsExhausted()) {
		m_usButtons &= ~static_cast<unsigned short>(m_pquQueuedToBeUnholdButtons->Pop());
	}

	return m_usButtons;
}

void CInputButtons::AddButtonToNextFrameQueue(_In_ EInputBtns _Button) {
	m_pquQueuedButtons->Push(_Button);
}

void CInputButtons::AddButtonToBeUnholdNextFrameQueue(_In_ EInputBtns _Button) {
	m_pquQueuedToBeUnholdButtons->Push(_Button);
}

void CInputButtons::AddButtonToBeUnholdNextFrameQueue(_In_ unsigned short _Button) {
	m_pquQueuedToBeUnholdButtons->Push(static_cast<EInputBtns>(_Button));
}

void CInputButtons::AddButtonToNextFrameQueue(_In_ unsigned short _Button) {
	m_pquQueuedButtons->Push(static_cast<EInputBtns>(_Button));
}

bool CInputButtons::IsButtonBeingHold(_In_ EInputBtns _Button) {
	return IsButtonBeingHold(static_cast<unsigned short>(_Button));
}

bool CInputButtons::IsButtonBeingHold(_In_ unsigned short _Button) {
	for (int idx = 0; idx < m_pquQueuedButtons->m_Deque.size(); idx++) {
		EInputBtns btn = m_pquQueuedButtons->m_Deque[idx];
		if (_Button == static_cast<unsigned short>(btn)) {
			return true;
		}
	}

	return (m_usButtons & _Button) != 0;
}

CVirtualInputButton::CVirtualInputButton(_In_z_ const char* _CommandName, _In_ EInputBtns _MappedButton) : m_pszCommandName(_CommandName) {
	m_eMappedButton = _MappedButton;
	m_bBeingHold = false;

	int iCommandNameLen = strlen(_CommandName);

	char* buffer = (char*) Q_malloc(sizeof('-') + iCommandNameLen + sizeof('\0'));
	buffer[0] = '-';
	for (int idx = 1; idx < iCommandNameLen + 1; idx++) {
		buffer[idx] = _CommandName[idx - 1];
		if (idx == iCommandNameLen + 1) {
			buffer[idx + 1] = '\0';
		}
	}
	CEngineHooks::SetupEngineCommandHook(buffer, CVirtualInputButtons::Process, reinterpret_cast<void**>(&m_pfnOriginalUpCommand));
	buffer[0] = '+';
	CEngineHooks::SetupEngineCommandHook(buffer, CVirtualInputButtons::Process, reinterpret_cast<void**>(&m_pfnOriginalDownCommand));
	Q_free(buffer);
}

void CVirtualInputButtons::Initialise() {
	ms_pVecButtons = Q_new(CVector<CVirtualInputButton*>)({});
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("attack", EInputBtns::kAttack));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("jump", EInputBtns::kJump));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("duck", EInputBtns::kDuck));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("forward", EInputBtns::kForward));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("back", EInputBtns::kBack));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("use", EInputBtns::kUse));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("left", EInputBtns::kLeft));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("right", EInputBtns::kRight));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("moveleft", EInputBtns::kMoveLeft));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("moveright", EInputBtns::kMoveRight));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("attack2", EInputBtns::kSecondaryAttack));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("reload", EInputBtns::kReload));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("alt1", EInputBtns::kTertiaryAttack));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("showscores", EInputBtns::kOpenScoreboard));

	/*ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("speed", EInputBtns::kStealth));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("moveup", EInputBtns::kMoveUp));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("movedown", EInputBtns::kMoveDown));
	ms_pVecButtons->push_back(Q_new(CVirtualInputButton)("break", EInputBtns::kBreak));*/
}

void CVirtualInputButtons::Shutdown() {
	for (CVirtualInputButton** ppBegin = ms_pVecButtons->begin(); ppBegin != NULL && ppBegin != ms_pVecButtons->end(); ++ppBegin) {
		if (!ppBegin) continue;
		CVirtualInputButton* pButton = *ppBegin;
		if (!pButton) continue;
		Q_delete(pButton);
	}
	Q_delete(ms_pVecButtons);
}

bool CVirtualInputButtons::IsButtonBeingHold(_In_ EInputBtns _Button) {
	CVector<CVirtualInputButton*>::iterator it = ms_pVecButtons->begin();
	for (it; it && it != ms_pVecButtons->end(); it++) {
		CVirtualInputButton* lpButton = *it;
		if (lpButton->m_eMappedButton == _Button)
			return lpButton->m_bBeingHold;
	}

	return false;
}

bool CVirtualInputButtons::IsButtonBeingHold(_In_ int _Button) {
	return IsButtonBeingHold(static_cast<EInputBtns>(_Button));
}

void CVirtualInputButtons::Process() {
	if (!g_pEngfuncs) return;
	if (!ms_pVecButtons) return;
	char* szFirstArg = g_pEngfuncs->Cmd_Argv(0);
	if (!szFirstArg) return;
	bool bIsDown = szFirstArg[0] == '+';
	szFirstArg++;
	char* szSpacePosition = nullptr;
	if (szSpacePosition = strstr(szFirstArg, " ")) {
		*szSpacePosition = '\0';
	}

	CVector<CVirtualInputButton*>::iterator it = ms_pVecButtons->begin();
	for (it; it && it != ms_pVecButtons->end(); it++) {
		CVirtualInputButton* lpButton = *it;
		if (!_strnicmp(lpButton->m_pszCommandName, szFirstArg, strlen(szFirstArg))) {
			if (szSpacePosition) {
				*szSpacePosition = ' ';
			}
			lpButton->m_bBeingHold = bIsDown;
			szFirstArg--;
			if (bIsDown) {
				lpButton->m_pfnOriginalDownCommand();
			} else {
				lpButton->m_pfnOriginalUpCommand();
			}
			break;
		}
	}
}