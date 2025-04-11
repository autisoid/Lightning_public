/**
 * Copyright - xWhitey, 2023.
 * CBreakLCModule.hpp - description
 */

#ifdef CBREAKLCMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CBreakLCModule.hpp
#else //CBREAKLCMODULE_HPP_RECURSE_GUARD

#define CBREAKLCMODULE_HPP_RECURSE_GUARD

#ifndef CBREAKLCMODULE_HPP_GUARD
#define CBREAKLCMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CBreakLCModule : CModule {
	CBreakLCModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CIntegerValue* m_pTest = nullptr;
	CBoolValue* m_pFlipTicks = nullptr;
} CBreakLCModule;

using CBreakLCModule = struct CBreakLCModule;

#else //__cplusplus
#error C++ compiler required to compile CBreakLCModule.hpp.
#endif //__cplusplus

#endif //CBREAKLCMODULE_HPP_GUARD

#undef CBREAKLCMODULE_HPP_RECURSE_GUARD
#endif //CBREAKLCMODULE_HPP_RECURSE_GUARD