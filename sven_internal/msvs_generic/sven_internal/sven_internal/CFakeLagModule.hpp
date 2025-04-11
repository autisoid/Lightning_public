/**
 * Copyright - xWhitey, 2023.
 * CFakeLagModule.hpp - description
 */

#ifdef CFAKELAGMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CFakeLagModule.hpp
#else //CFAKELAGMODULE_HPP_RECURSE_GUARD

#define CFAKELAGMODULE_HPP_RECURSE_GUARD

#ifndef CFAKELAGMODULE_HPP_GUARD
#define CFAKELAGMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CFakeLagModule : CModule {
	CFakeLagModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CIntegerValue* m_pChokeCmdsCount = nullptr;
} CFakeLagModule;

using CFakeLagModule = struct CFakeLagModule;

#else //__cplusplus
#error C++ compiler required to compile CFakeLagModule.hpp.
#endif //__cplusplus

#endif //CFAKELAGMODULE_HPP_GUARD

#undef CFAKELAGMODULE_HPP_RECURSE_GUARD
#endif //CFAKELAGMODULE_HPP_RECURSE_GUARD