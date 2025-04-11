#include "StdAfx.h"

CLinkedList<void*>* CFileMgr::ms_pUnclosedHandles = nullptr;

void* CFileMgr::OpenFile(_In_z_ const char* _FileName, _In_z_ const char* _Mode) {
	FILE* result;
	errno_t err = fopen_s(&result, _FileName, _Mode);

	if (err) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CFileMgr::OpenFile(char const*,char const*): Failed to open %s file with %d reason.\n", _FileName, err);

		return nullptr;
	} else {
		if (ms_pUnclosedHandles)
			ms_pUnclosedHandles->Append(result);
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CFileMgr::OpenFile(char const*,char const*): Sucessfully opened file %s (handle is 0x%p)\n", _FileName, result);
	}

	return result;
}

bool CFileMgr::DoesFileExist(_In_z_ const char* _FileName) {
	FILE* result;
	errno_t err = fopen_s(&result, _FileName, "r");

	bool res = !err && result;

	if (result) {
		CloseFile(result);
	}

	return res;
}

void* CFileMgr::ReopenFile(_In_ void* _OldHandle, _In_z_ const char* _FileName, _In_z_ const char* _Mode) {
	FILE* result;
	errno_t err = freopen_s(&result, _FileName, _Mode, reinterpret_cast<FILE*>(_OldHandle));

	if (err) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CFileMgr::ReopenFile(void*,char const*,char const*): Failed to reopen %s file with %d reason.\n", _FileName, err);

		return nullptr;
	} else {
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CFileMgr::ReopenFile(void*,char const*,char const*): Sucessfully reopened file %s (handle is 0x%p)\n", _FileName, result);
	}

	return result;
}

void CFileMgr::CloseFile(_In_ void* _PtrToFile) {
	if (_PtrToFile) {
		if (ms_pUnclosedHandles)
			ms_pUnclosedHandles->Remove(_PtrToFile);

		errno_t err = fclose(reinterpret_cast<FILE*>(_PtrToFile));

		if (err) {
			CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CFileMgr::Close(void*): Failed to close a file with 0x%p descriptor and %d reason.\n", _PtrToFile, err);
		}
	} else {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CFileMgr::Close(void*): Failed to close a file: _PtrToFile is nullptr!\n");
	}
}

int CFileMgr::ReadFile(_In_ void* _PtrToFile, _In_ int _BufferSize, _In_reads_(_BufferSize) char*& _Buffer) {
	int size = _BufferSize;
	if (size == 0) {
		fseek(reinterpret_cast<FILE*>(_PtrToFile), 0, SEEK_END);
		size = ftell(reinterpret_cast<FILE*>(_PtrToFile));
		rewind(reinterpret_cast<FILE*>(_PtrToFile));
	}

	_Buffer[size - 1] = '\0';

	int read = fread(_Buffer, sizeof(char), size, reinterpret_cast<FILE*>(_PtrToFile));

	if (read == 0 || _Buffer[0] == '\0' || size == 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[WARNING] CFileMgr::ReadFile(void*,int,char*&): Read 0 characters from file 0x%p.\n", _PtrToFile);

		return 0;
	}

	return size;
}

void CFileMgr::WriteFile(_In_ void* _PtrToFile, _In_z_ const char* _Data) {
	if (fwrite(_Data, sizeof(char), strlen(_Data), reinterpret_cast<FILE*>(_PtrToFile)) == 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[WARNING] CFileMgr::WriteFile(void*,char const*): Wrote 0 characters to a file.\n");
	}
}

bool CFileMgr::IsEOF(_In_ void* _PtrToFile) {
	return feof(reinterpret_cast<FILE*>(_PtrToFile)) != 0;
}

void CFileMgr::Shutdown() {
	if (!ms_pUnclosedHandles) {
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CFileMgr::Shutdown(void): Shutting down gracefully, assuming that there are no unclosed handles.\n");
		return;
	}

	size_t cbUnclosedHandles = 0;
	CLinkedChainNode<void*>* pCurrent = ms_pUnclosedHandles->m_pHead;
	while (pCurrent) {
		cbUnclosedHandles++;
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CFileMgr::Shutdown(void): Handle 0x%p wasn't closed!\n", pCurrent->m_Data);
		pCurrent = pCurrent->m_pNext;
	}

	if (cbUnclosedHandles > 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[SEVERE] CFileMgr::Shutdown(void): Performed a dirty shutdown.\n");
	} else { 
		CCheat::GetCheat()->m_pConsole->Printf("[DEBUG] CFileMgr::Shutdown(void): Shutting down gracefully.\n");
	}
}