/**
 * Copyright - xWhitey, 2024.
 * CKickerModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CKICKERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CKickerModule.hpp
#else //CKICKERMODULE_HPP_RECURSE_GUARD

#define CKICKERMODULE_HPP_RECURSE_GUARD

#ifndef CKICKERMODULE_HPP_GUARD
#define CKICKERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CKickerModule : CModule {
	CKickerModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent* _Event) override;

	CListValue* m_pMode = nullptr;
	CFloatValue* m_pDelayBetweenUpdates = nullptr;
	CIntegerValue* m_pCommandAmount = nullptr;
	CBoolValue* m_pUseRandomCase = nullptr;
	bool m_bAllow = false;
	double m_dblLastCommandSendTime;

	char* GenerateCommandInRandomCase();

	const char* m_rgpszAlphabet1 = nullptr;
	const char* m_rgpszAlphabet2 = nullptr;

	CTrustedRandom* m_pTrustedRandom;
} CKickerModule;

using CKickerModule = struct CKickerModule;

#else //!__cplusplus
#error C++ compiler required to compile CKickerModule.hpp
#endif //__cplusplus

#endif //CKICKERMODULE_HPP_GUARD

#undef CKICKERMODULE_HPP_RECURSE_GUARD
#endif //CKICKERMODULE_HPP_RECURSE_GUARD