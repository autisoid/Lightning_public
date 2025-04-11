/**
 * Copyright - xWhitey, 2024.
 * CConsoleSpammerModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CCONSOLESPAMMERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CConsoleSpammerModule.hpp
#else //CCONSOLESPAMMERMODULE_HPP_RECURSE_GUARD

#define CCONSOLESPAMMERMODULE_HPP_RECURSE_GUARD

#ifndef CCONSOLESPAMMERMODULE_HPP_GUARD
#define CCONSOLESPAMMERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CConsoleSpammerModule : CModule {
	CConsoleSpammerModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CListValue* m_pMode = nullptr;
	CTextValue* m_pCustomCvarName = nullptr;
	CTextValue* m_pCustomCvarValue = nullptr;
	CIntegerValue* m_pRequestId = nullptr;
	CIntegerValue* m_pNewlinesAmount = nullptr;
	CFloatValue* m_pSendInterval = nullptr;
	CIntegerValue* m_pImpulseValue = nullptr;
	CBoolValue* m_pRandomImpulse = nullptr;

	double m_dblNextCmdSendTime;

	CTrustedRandom* m_pTrustedRandom = nullptr;
} CConsoleSpammerModule;

using CConsoleSpammerModule = struct CConsoleSpammerModule;

#else //!__cplusplus
#error C++ compiler required to compile CConsoleSpammerModule.hpp
#endif //__cplusplus

#endif //CCONSOLESPAMMERMODULE_HPP_GUARD

#undef CCONSOLESPAMMERMODULE_HPP_RECURSE_GUARD
#endif //CCONSOLESPAMMERMODULE_HPP_RECURSE_GUARD