#ifdef TARGETVER_H_RECURSE_GUARD
#error Recursive header files inclusion detected in targetver.h
#else //TARGETVER_H_RECURSE_GUARD

#define TARGETVER_H_RECURSE_GUARD

#ifndef TARGETVER_H_GUARD
#define TARGETVER_H_GUARD
#pragma once

#ifdef __cplusplus

// When you turn on SDKDDKVer.h, the newest of the available Windows platforms will be set.
// If you are planning to build an application for a previous version of the Windows platform, 
// turn on WinSDKVer.h and set the desired platform in the _WIN32_WINNT macro before enabling SDKDDKVer.h.

#include <winsdkver.h>

#ifdef _WIN32_WINNT
#pragma message("_WIN32_WINNT was defined by something before we could do that. Undefining it...")
#undef _WIN32_WINNT
#endif //_WIN32_WINNT

//_WIN32_WINNT_WIN7 == 0x0601, which is NT 6.1

#define _WIN32_WINNT	0x0601
#define _WIN32_WINDOWS	_WIN32_WINNT

#ifndef WINVER
#define WINVER _WIN32_WINNT
#endif //WINVER

#include <sdkddkver.h>


#else //!__cplusplus
#error C++ compiler required to compile targetver.h
#endif //__cplusplus

#endif //TARGETVER_H_GUARD

#undef TARGETVER_H_RECURSE_GUARD
#endif //TARGETVER_H_RECURSE_GUARD