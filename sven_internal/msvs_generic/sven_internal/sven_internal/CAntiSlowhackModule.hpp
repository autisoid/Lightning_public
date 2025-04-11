/**
 * Copyright - xWhitey, 2024.
 * CAntiSlowhackModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CANTISLOWHACKMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAntiSlowhackModule.hpp
#else //CANTISLOWHACKMODULE_HPP_RECURSE_GUARD

#define CANTISLOWHACKMODULE_HPP_RECURSE_GUARD

#ifndef CANTISLOWHACKMODULE_HPP_GUARD
#define CANTISLOWHACKMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAntiSlowhackModule : CModule {
	CAntiSlowhackModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CExtraValues* m_pConfigureWnd;
	CListValue* m_pMode;
	CTextValue* m_pWhitelist;
	CBoolValue* m_pNotifyPlayers;
	CTextValue* m_pNotificationText;
} CAntiSlowhackModule;

using CAntiSlowhackModule = struct CAntiSlowhackModule;

#else //!__cplusplus
#error C++ compiler required to compile CAntiSlowhackModule.hpp
#endif //__cplusplus

#endif //CANTISLOWHACKMODULE_HPP_GUARD

#undef CANTISLOWHACKMODULE_HPP_RECURSE_GUARD
#endif //CANTISLOWHACKMODULE_HPP_RECURSE_GUARD