/**
 * Copyright - xWhitey, 2024.
 * timer.hpp - high-resolution timer to measure time intervals
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef TIMER_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in timer.hpp
#else //TIMER_HPP_RECURSE_GUARD

#define TIMER_HPP_RECURSE_GUARD

#ifndef TIMER_HPP_GUARD
#define TIMER_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif //_MSC_VER

typedef struct CTimer {
	CTimer();

	void Initialise();
	void Process();

	unsigned long long m_nTimeInMilliseconds;
	unsigned long long m_nPreviousTimeInMilliseconds;
	double m_dblTimeScale;
	unsigned long long m_nCyclesPerMillisecond;
	LARGE_INTEGER m_ullOldPerfCounter;

	double m_dblPossibleFrametime;

	bool m_bInitialized;
} CTimer;

using CTimer = struct CTimer;

#ifdef _MSC_VER
#pragma pack(pop)
#endif //_MSC_VER

#else //!__cplusplus
#error C++ compiler required to compile timer.hpp
#endif //__cplusplus

#endif //TIMER_HPP_GUARD

#undef TIMER_HPP_RECURSE_GUARD
#endif //TIMER_HPP_RECURSE_GUARD