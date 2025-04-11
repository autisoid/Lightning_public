/**
 * Copyright - xWhitey, 2024.
 * input_buttons.hpp - "input buttons"
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef INPUT_BUTTONS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in input_buttons.hpp
#else //INPUT_BUTTONS_HPP_RECURSE_GUARD

#define INPUT_BUTTONS_HPP_RECURSE_GUARD

#ifndef INPUT_BUTTONS_HPP_GUARD
#define INPUT_BUTTONS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "queue.hpp"

typedef enum struct EInputBtns : unsigned short {
	kAttack = (1 << 0), //attack
	kJump = (1 << 1), //jump
	kDuck = (1 << 2), //crouch
	kForward = (1 << 3), //go forward
	kBack = (1 << 4), //go back
	kUse = (1 << 5), //use sth
	kCancel = (1 << 6), //whenever a vgui menu is open, user is able to cancel selection by pressing "esc"
	kLeft = (1 << 7), //rotate the camera to left
	kRight = (1 << 8), //rotate the camera to right
	kMoveLeft = (1 << 9), //go left
	kMoveRight = (1 << 10), //go right
	kSecondaryAttack = (1 << 11), //secondary attack
	kRun = (1 << 12), //unused? perhaps this may be +speed
	kReload = (1 << 13), //reload weapon
	kTertiaryAttack = (1 << 14), //alternative attack (tertiary)
	kOpenScoreboard = (1 << 15), //scoreboard btn is being held

	kAnyAttack = kAttack | kSecondaryAttack,
	kMovementButtons = kForward | kBack | kMoveLeft | kMoveRight,

	kTotalInputButtons = 16,

	//Custom movement buttons (actually they exist in the engine, but have no mapped keys)
	/*kStealth = (1 << 16),
	kMoveUp = (1 << 17),
	kMoveDown = (1 << 18),
	kBreak = (1 << 19)*/
} EInputBtns;

typedef struct CInputButtons {
	CInputButtons();
	
	bool IsButtonBeingHold(_In_ EInputBtns _Button);
	bool IsButtonBeingHold(_In_ unsigned short _Button);
	void AddButtonToNextFrameQueue(_In_ EInputBtns _Button);
	void AddButtonToNextFrameQueue(_In_ unsigned short _Button);
	void AddButtonToBeUnholdNextFrameQueue(_In_ EInputBtns _Button);
	void AddButtonToBeUnholdNextFrameQueue(_In_ unsigned short _Button);
	unsigned short ProcessQueuedButtons();
	void Process(_In_ unsigned short _AlreadyHeldButtons);

	unsigned short m_usButtons;
	CQueue<EInputBtns>* m_pquQueuedButtons;
	CQueue<EInputBtns>* m_pquQueuedToBeUnholdButtons;
} CInputButtons;

using CInputButtons = struct CInputButtons;

typedef struct CVirtualInputButton {
	CVirtualInputButton(_In_z_ const char* _CommandName, _In_ EInputBtns _MappedButton);

	const char* m_pszCommandName;
	EInputBtns m_eMappedButton;
	bool m_bBeingHold;
	void(__cdecl* m_pfnOriginalDownCommand)();
	void(__cdecl* m_pfnOriginalUpCommand)();
} CVirtualInputButton;

using CVirtualInputButton = struct CVirtualInputButton;

typedef struct CVirtualInputButtons {
	static void Initialise();
	static void Shutdown();
	bool IsButtonBeingHold(_In_ EInputBtns _Button);
	bool IsButtonBeingHold(_In_ int _Button);
	static void Process();

	static CVector<CVirtualInputButton*>* ms_pVecButtons;
} CVirtualInputButtons;

using CVirtualInputButtons = struct CVirtualInputButtons;

#else //!__cplusplus
#error C++ compiler required to compile input_buttons.hpp
#endif //__cplusplus

#endif //INPUT_BUTTONS_HPP_GUARD

#undef INPUT_BUTTONS_HPP_RECURSE_GUARD
#endif //INPUT_BUTTONS_HPP_RECURSE_GUARD