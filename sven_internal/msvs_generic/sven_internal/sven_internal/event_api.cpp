#include "StdAfx.h"

ISimpleEvent::ISimpleEvent(_In_ EEventType _EventType) : m_eEventType(_EventType) {
	m_bHasBeenFired = false;
}

EEventType ISimpleEvent::GetType() {
	return m_eEventType;
}

EEventType ISimpleEvent::GetType() const {
	return m_eEventType;
}

int ISimpleEvent::GetTypeAsInt() {
	return (int)m_eEventType;
}

int ISimpleEvent::GetTypeAsInt() const {
	return (int)m_eEventType;
}

ICancellableEvent::ICancellableEvent(EEventType _EventType) : ISimpleEvent(_EventType) {

}

void ICancellableEvent::CancelEvent() {
	m_bIsCancelled = true;
}

void ICancellableEvent::CancelEvent() const {
	m_bIsCancelled = true;
}

CEventManager::CEventManager() {
	m_pListeners = Q_new(CLinkedList<CAbstractEventListener*>)();
	m_bAcceptingFurtherEventFires = true;
}

CEventManager::~CEventManager() {
	Q_delete(m_pListeners); //will clean itself
	Shutdown();
}

bool CEventManager::IsThisVeryElementUnique(CAbstractEventListener* _Listener) const {
	CLinkedChainNode<CAbstractEventListener*>* pCurrent = m_pListeners->m_pHead;

	while (pCurrent != nullptr) {
		CAbstractEventListener* pListener = pCurrent->m_Data;

		if (pListener == _Listener) {
			return false;
		}

		pCurrent = pCurrent->m_pNext;
	}

	return true;
}

void CEventManager::RegisterListener(CAbstractEventListener* _Listener) {
	//if (!IsThisVeryElementUnique(_Listener)) return;

	m_pListeners->Append(_Listener);
}

void CEventManager::UnregisterListener(CAbstractEventListener* _Listener) {
	m_pListeners->Remove(_Listener);
}

void CEventManager::Post(_In_ const ISimpleEvent* _Event) {
	if (!_Event) return;
	if (!m_bAcceptingFurtherEventFires) return;
	CLinkedChainNode<CAbstractEventListener*>* pCurrent = m_pListeners->m_pHead;

	if (_Event->m_bHasBeenFired) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CEventManager::Post: already fired event has been passed into Post!!! Event type: %d\n", _Event->GetType());
	}

	while (pCurrent) {
		if (CAbstractEventListener* pListener = pCurrent->m_Data) {
			pListener->OnEvent(_Event);
		} else {
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CEventManager::Post: null listener in Post!!!\n");
		}

		pCurrent = pCurrent->m_pNext;
	}

	_Event->m_bHasBeenFired = true;

}

void CEventManager::ProcessStalledEventsUnconditionally() {
	;
}

void CEventManager::Shutdown() {
	m_bAcceptingFurtherEventFires = false;
}