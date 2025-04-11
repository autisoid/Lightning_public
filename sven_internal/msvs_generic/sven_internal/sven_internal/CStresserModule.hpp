/**
 * Copyright - xWhitey, 2024.
 * CStresserModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CSTRESSERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CStresserModule.hpp
#else //CSTRESSERMODULE_HPP_RECURSE_GUARD

#define CSTRESSERMODULE_HPP_RECURSE_GUARD

#ifndef CSTRESSERMODULE_HPP_GUARD
#define CSTRESSERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CStresserModule : CModule {
	CStresserModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent* _Event) override;

	CListValue* m_pMode;
	CIntegerValue* m_pVoicedataSize;

	CTrustedRandom* m_pTrustedRandom;
} CStresserModule;

using CStresserModule = struct CStresserModule;

#else //!__cplusplus
#error C++ compiler required to compile CStresserModule.hpp
#endif //__cplusplus

#endif //CSTRESSERMODULE_HPP_GUARD

#undef CSTRESSERMODULE_HPP_RECURSE_GUARD
#endif //CSTRESSERMODULE_HPP_RECURSE_GUARD