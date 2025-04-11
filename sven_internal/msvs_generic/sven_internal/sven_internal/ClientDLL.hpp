/**
 * Copyright - xWhitey, 2023.
 * ClientDLL.hpp - description
 */

#ifdef CLIENTDLL_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in ClientDLL.hpp
#else //CLIENTDLL_HPP_RECURSE_GUARD

#define CLIENTDLL_HPP_RECURSE_GUARD

#ifndef CLIENTDLL_HPP_GUARD
#define CLIENTDLL_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CClientHooks {
	static void Initialize();
} CClientHooks;

using CClientHooks = struct CClientHooks;

#else //!__cplusplus
#error C++ compiler required to compile ClientDLL.hpp
#endif //__cplusplus

#endif //CLIENTDLL_HPP_GUARD

#undef CLIENTDLL_HPP_RECURSE_GUARD
#endif //CLIENTDLL_HPP_RECURSE_GUARD