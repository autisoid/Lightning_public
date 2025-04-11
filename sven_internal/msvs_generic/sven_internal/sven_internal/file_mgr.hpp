/**
 * Copyright - xWhitey, 2023.
 * file_mgr.hpp - description
 */

#ifdef FILE_MGR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in file_mgr.hpp
#else //FILE_MGR_HPP_RECURSE_GUARD

#define FILE_MGR_HPP_RECURSE_GUARD

#ifndef FILE_MGR_HPP_GUARD
#define FILE_MGR_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CFileMgr {
	static void Initialise();
	static void* OpenFile(_In_z_ const char* _FileName, _In_z_ const char* _Mode);
	static void* ReopenFile(_In_ void* _OldHandle, _In_z_ const char* _FileName, _In_z_ const char* _Mode);
	static void CloseFile(_In_ void* _PtrToFile);
	static int ReadFile(_In_ void* _PtrToFile, _In_ int _BufferSize, _In_reads_(_BufferSize) char*& _Buffer);
	static void WriteFile(_In_ void* _PtrToFile, _In_z_ const char* _Data);
	static bool IsEOF(_In_ void* _PtrToFile);
	static bool DoesFileExist(_In_z_ const char* _FileName);
	static void Shutdown();

	static struct CLinkedList<void*>* ms_pUnclosedHandles;
} CFileMgr;

using CFileMgr = struct CFileMgr;

#else //!__cplusplus
#error C++ compiler required to compile file_mgr.hpp
#endif //__cplusplus

#endif //FILE_MGR_HPP_GUARD

#undef FILE_MGR_HPP_RECURSE_GUARD
#endif //FILE_MGR_HPP_RECURSE_GUARD