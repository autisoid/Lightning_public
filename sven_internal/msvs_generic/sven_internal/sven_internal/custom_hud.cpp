#include "StdAfx.h"
#include "custom_hud.hpp"
#include "hud_arraylist_element.hpp"

CHUDElement::CHUDElement(const char* _Name) {
	m_pszName = _Name;
}

CCustomHUD::CCustomHUD() {
	m_pElements = Q_new(CLinkedList<CHUDElement*>)();
	m_pElements->Append(Q_new(CArrayListHUDElement)()); //create the list before loading up the config so we can call callback in the element so it loads the values

	m_pElementsMgr = Q_new(CConfigMgr)("./lightning.tech/custom_hud.ini");
}

CCustomHUD::~CCustomHUD() {
	SaveElements();
}

void CCustomHUD::LoadElements() {
	// TODO: Implement
}

void CCustomHUD::SaveElements() {
	// TODO: Implement
}

void CCustomHUD::Draw() {
	if (!m_pElements) return;

	CLinkedChainNode<CHUDElement*>* pCurrent = m_pElements->m_pHead;

	while (pCurrent) {
		CHUDElement* pElement = pCurrent->m_Data;
		if (pElement) {
			pElement->Draw();
		}

		pCurrent = pCurrent->m_pNext;
	}
}