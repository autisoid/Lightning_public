/**
 * Copyright - xWhitey, 2022.
 * MemUtils.hpp - Some utilities for DOS/NT images. (and maybe some others? XD)
 */

#ifdef MEMUTILS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in MemUtils.hpp
#else //MEMUTILS_HPP_RECURSE_GUARD

#define MEMUTILS_HPP_RECURSE_GUARD

#ifndef MEMUTILS_HPP_GUARD
#define MEMUTILS_HPP_GUARD
#pragma once

#include <Windows.h>

typedef struct MemUtils {
	static unsigned long GetModuleSize(_In_ void* _Module) {
		MEMORY_BASIC_INFORMATION mbi;

		if (VirtualQuery(_Module, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
			if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
				return 0;

			unsigned long dwAllocationBase = (unsigned long)mbi.AllocationBase;

			IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)dwAllocationBase;
			IMAGE_NT_HEADERS* pe = (IMAGE_NT_HEADERS*)(dwAllocationBase + dos->e_lfanew);

			IMAGE_FILE_HEADER* file = &pe->FileHeader;
			IMAGE_OPTIONAL_HEADER* opt = &pe->OptionalHeader;

			return opt->SizeOfImage;
		}

		return 0;
	}

	static unsigned long GetModuleBase(_In_ void* _Module) {
		MEMORY_BASIC_INFORMATION mbi;

		if (VirtualQuery(_Module, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
			if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
				return 0;

			return (unsigned long)mbi.AllocationBase;
		}

		return 0;
	}
} MemUtils;

#endif //MEMUTILS_HPP_GUARD

#undef MEMUTILS_HPP_RECURSE_GUARD
#endif //MEMUTILS_HPP_RECURSE_GUARD
