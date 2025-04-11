/**
 * Copyright - xWhitey, 2023.
 * CAutoJumpbugModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CAUTOJUMPBUGMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAutoJumpbugModule.hpp
#else //CAUTOJUMPBUGMODULE_HPP_RECURSE_GUARD

#define CAUTOJUMPBUGMODULE_HPP_RECURSE_GUARD

#ifndef CAUTOJUMPBUGMODULE_HPP_GUARD
#define CAUTOJUMPBUGMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAutoJumpbugModule : CModule {
	CAutoJumpbugModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CListValue* m_pMode;
	CFloatValue* m_pGroundNormalAngle;
	CFloatValue* m_pFrameZDist;
	CFloatValue* m_pDuckAtHeight;
	CFloatValue* m_pJumpbugAtHeight;

	int m_iJumpbugState = 0;
} CAutoJumpbugModule;

using CAutoJumpbugModule = struct CAutoJumpbugModule;

#else //!__cplusplus
#error C++ compiler required to compile CAutoJumpbugModule.hpp
#endif //__cplusplus

#endif //CAUTOJUMPBUGMODULE_HPP_GUARD

#undef CAUTOJUMPBUGMODULE_HPP_RECURSE_GUARD
#endif //CAUTOJUMPBUGMODULE_HPP_RECURSE_GUARD