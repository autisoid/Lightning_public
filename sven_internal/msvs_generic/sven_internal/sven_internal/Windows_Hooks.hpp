/**
 * Copyright - xWhitey, 2023.
 * Windows_Hooks.hpp - description
 */

#ifdef WINDOWS_HOOKS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in Windows_Hooks.hpp
#else //WINDOWS_HOOKS_HPP_RECURSE_GUARD

#define WINDOWS_HOOKS_HPP_RECURSE_GUARD

#ifndef WINDOWS_HOOKS_HPP_GUARD
#define WINDOWS_HOOKS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#if !defined(__linux__) && defined(_WIN32)

extern void SetColors();
extern long __stdcall HOOKED_wndproc(const HWND a1, unsigned int a2, unsigned a3, long a4);
extern void Render();
extern "C" extern int __stdcall HOOKED_wglSwapBuffers(_In_ HDC a1);

extern "C" extern BOOL WINAPI HOOKED_GetMessageA(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax);
extern "C" extern BOOL WINAPI HOOKED_GetMessageW(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax);

extern "C" extern BOOL WINAPI HOOKED_PeekMessageA(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax, _In_ IN UINT _RemoveMsg);
extern "C" extern BOOL WINAPI HOOKED_PeekMessageW(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax, _In_ IN UINT _RemoveMsg);

extern "C" extern BOOL WINAPI HOOKED_PostMessageA(_In_ IN HWND _hWnd, _In_ IN unsigned int _Msg, _In_ IN WPARAM _wParam, _In_ IN LPARAM _param);
extern "C" extern BOOL WINAPI HOOKED_PostMessageW(_In_ IN HWND _hWnd, _In_ IN unsigned int _Msg, _In_ IN WPARAM _wParam, _In_ IN LPARAM _param);

extern "C" extern BOOL WINAPI HOOKED_ClipCursor(_Inout_ IN OUT const LPRECT * _Rect);

extern "C" extern BOOL WINAPI HOOKED_SetCursorPos(_In_ IN int _X, _In_ IN int _Y);
extern "C" extern BOOL WINAPI HOOKED_GetCursorPos(_Out_ OUT LPPOINT _Point);

extern LRESULT CALLBACK AAL_LowLevelMouseHook(_In_ int _Code, _In_ WPARAM _wParam, _In_ LPARAM _param);

#endif //not defined(__linux__) and defined(_WIN32)

#else //__cplusplus
#error C++ compiler required to compile Windows_Hooks.hpp.
#endif //__cplusplus

#endif //WINDOWS_HOOKS_HPP_GUARD

#undef WINDOWS_HOOKS_HPP_RECURSE_GUARD
#endif //WINDOWS_HOOKS_HPP_RECURSE_GUARD