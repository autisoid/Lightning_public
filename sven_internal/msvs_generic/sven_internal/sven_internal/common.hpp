/**
 * Copyright - xWhitey, 2023.
 * common.hpp - The main file of our project with includes
 */

#ifdef COMMON_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in common.hpp
#else //COMMON_HPP_RECURSE_GUARD

#define COMMON_HPP_RECURSE_GUARD

#ifndef COMMON_HPP_GUARD
#define COMMON_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define IMGUI_DISABLE_OBSOLETE_KEYIO

//WinAPI
//Fix of some weird bug which appears only in old MSVC toolchain.
#if _MSC_VER < 1920
#ifdef no_init_all
#undef no_init_all
#endif //no_init_all
#define no_init_all
#endif //MSC_VER < 1920

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely used components from Windows headers.
#define VC_EXTRALEAN		// Exclude rarely used components from Windows headers.

#include <Windows.h>
#include <conio.h>

//functional
#include "functional_lite/functional_lite.hpp"

//CRT
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//STL
#include <set>
#include <string>
#include <sstream>
#include <vector>

//MinHook
#include "MinHook/MinHook.h"

//HLSDK
#include "hlsdk_mini.hpp"

//ImGUI
#include "imgui/imconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3_loader.h"
#include "imgui/imgui_impl_win32.h"

#else //!__cplusplus
#error C++ compiler required to compile common.hpp
#endif //__cplusplus

#endif //COMMON_HPP_GUARD

#undef COMMON_HPP_RECURSE_GUARD
#endif //COMMON_HPP_RECURSE_GUARD