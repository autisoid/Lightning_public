#include "StdAfx.h"
#include "hooks.hpp"

CVector<CIntelligentHookedFunction*>* CHooks::ms_pHooks = nullptr;

CIntelligentHookedFunction::CIntelligentHookedFunction(void* _Target, void* _Detour) : m_pTarget(_Target), m_pDetour(_Detour) {
	m_bIsEnabled = false;
}

void CHooks::Initialise() {
	ms_pHooks = Q_new(CVector<CIntelligentHookedFunction*>)({});
	AddVectoredExceptionHandler(1, CHooks::UnhandledExceptionHandler);
}

void CHooks::CreateHook(void* _Target, void* _Detour) {
	CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CHooks::CreateHook: creating a hook at 0x%p\n", _Target);
	ms_pHooks->push_back(Q_new(CIntelligentHookedFunction)(_Target, _Detour));
}

void CHooks::EnableHook(void* _Target) {
	CIntelligentHookedFunction* pHook = FindHookByTarget(_Target);
	if (!pHook) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CHooks::EnableHook: failure finding hook with 0x%p as target address!\n", _Target);

		return;
	}
	pHook->m_bIsEnabled = true;
	AddPageGuardProtection(_Target);
}

void CHooks::DisableHook(void* _Target) {
	CIntelligentHookedFunction* pHook = FindHookByTarget(_Target);
	if (!pHook) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CHooks::DisableHook: failure finding hook with 0x%p as target address!\n", _Target);

		return;
	}
	pHook->m_bIsEnabled = false;
	//RemovePageGuardProtection(_Target);
}

void CHooks::AddPageGuardProtection(void* _Target) {
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(_Target, &mbi, sizeof(mbi));
	DWORD dwOld;
	VirtualProtect(_Target, 1, mbi.AllocationProtect | PAGE_GUARD, &dwOld);
}

void CHooks::RemovePageGuardProtection(void* _Target) {
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(_Target, &mbi, sizeof(mbi));
	DWORD dwOld;
	VirtualProtect(_Target, 1, mbi.AllocationProtect & (~PAGE_GUARD), &dwOld);
}

CIntelligentHookedFunction* CHooks::FindHookByTarget(void* _Target) {
	for (int idx = 0; idx < ms_pHooks->size(); idx++) {
		CIntelligentHookedFunction* pHook = ms_pHooks->at(idx);
		if (!pHook) continue;
		if (pHook->m_pTarget == _Target) {
			return pHook;
		}
	}

	return nullptr;
}

void CHooks::ProcessCall(void* _Target) {
	return AddPageGuardProtection(_Target);
}

void CHooks::ProtectPortion(void* _Address, size_t _Size, DWORD _NewProtect) {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	DWORD dwPageSize = si.dwPageSize;

	void* pPageStart = (void*)((DWORD_PTR)_Address & ~(dwPageSize - 1));

	void* pTempPage = VirtualAlloc(NULL, dwPageSize, MEM_COMMIT, _NewProtect);
	if (!pTempPage) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CHooks::ProtectPortion: failure allocating pTempPage\n");
		return;
	}

	memcpy(pTempPage, pPageStart, dwPageSize);

	size_t sOffset = (DWORD_PTR)_Address - (DWORD_PTR)pPageStart;
	memcpy((char*)_Address, (char*)pTempPage + sOffset, _Size);

	//VirtualFree(pTempPage, 0, MEM_RELEASE);
}

LONG CALLBACK CHooks::UnhandledExceptionHandler(PEXCEPTION_POINTERS _ExceptionPointers) {
	if (_ExceptionPointers->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION) {
		_ExceptionPointers->ContextRecord->EFlags |= (1 << 8);

		return EXCEPTION_CONTINUE_EXECUTION;
	}
	if (_ExceptionPointers->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP) {
		CIntelligentHookedFunction* pHook = FindHookByTarget(_ExceptionPointers->ExceptionRecord->ExceptionAddress);
		if (pHook) {
			if (pHook->m_bIsEnabled) {
				_ExceptionPointers->ContextRecord->Eip = (decltype(_ExceptionPointers->ContextRecord->Eip))(pHook->m_pDetour);
			}
		}
		AddPageGuardProtection(_ExceptionPointers->ExceptionRecord->ExceptionAddress);

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}