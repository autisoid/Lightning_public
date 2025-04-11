/**
 * Copyright - xWhitey, 2023.
 * CDoubletapModule.hpp - description
 */

#ifdef CDOUBLETAPMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CDoubletapModule.hpp
#else //CDOUBLETAPMODULE_HPP_RECURSE_GUARD

#define CDOUBLETAPMODULE_HPP_RECURSE_GUARD

#ifndef CDOUBLETAPMODULE_HPP_GUARD
#define CDOUBLETAPMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifndef LIGHTNING_TO_MINT_MC
typedef struct CDoubletapModule : CModule {
	CDoubletapModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	CListValue* m_pMode = nullptr;
	CIntegerValue* m_pRecharge = nullptr;
	CFloatValue* m_pTicksToShift = nullptr;

	int m_nTicksPassed = 0;
	bool m_bShouldShift = false;

	bool m_bCharging = false;
	bool m_bHasDoneShifting = false;

	Vector m_vecFixedPosition;
} CDoubletapModule;

using CDoubletapModule = struct CDoubletapModule;

#endif //LIGHTNING_TO_MINT_MC

#else //__cplusplus
#error C++ compiler required to compile CDoubletapModule.hpp.
#endif //__cplusplus

#endif //CDOUBLETAPMODULE_HPP_GUARD

#undef CDOUBLETAPMODULE_HPP_RECURSE_GUARD
#endif //CDOUBLETAPMODULE_HPP_RECURSE_GUARD