/**
 * Copyright - xWhitey, 2023.
 * CAirStuckModule.hpp - Makes you freeze.
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CAIRSTUCKMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CAirStuckModule.hpp
#else //CAIRSTUCKMODULE_HPP_RECURSE_GUARD

#define CAIRSTUCKMODULE_HPP_RECURSE_GUARD

#ifndef CAIRSTUCKMODULE_HPP_GUARD
#define CAIRSTUCKMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CAirStuckModule : CModule {
	CAirStuckModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(const ISimpleEvent*) override;

	CListValue* m_pMode = nullptr;
	CBoolValue* m_pFixupPosition = nullptr;
	CBoolValue* m_pPreserveDuckState = nullptr;
	CBoolValue* m_pEmulateFakeMovement = nullptr;
	CBoolValue* m_pPerspective = nullptr;
	CBoolValue* m_pFixRollAngles = nullptr;
	Vector m_vecFixedPosition;
	Vector m_vecSavedViewangles;
	bool m_bWasDucking;
} CAirStuckModule;

using CAirStuckModule = struct CAirStuckModule;

#else //!__cplusplus
#error C++ compiler required to compile CAirStuckModule.hpp
#endif //__cplusplus

#endif //CAIRSTUCKMODULE_HPP_GUARD

#undef CAIRSTUCKMODULE_HPP_RECURSE_GUARD
#endif //CAIRSTUCKMODULE_HPP_RECURSE_GUARD