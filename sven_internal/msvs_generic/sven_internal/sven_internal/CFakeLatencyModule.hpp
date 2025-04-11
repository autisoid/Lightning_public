/**
 * Copyright - xWhitey, 2024.
 * CFakeLatencyModule.hpp - fake scoreboard ping (ft. oxware)
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CFAKELATENCYMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CFakeLatencyModule.hpp
#else //CFAKELATENCYMODULE_HPP_RECURSE_GUARD

#define CFAKELATENCYMODULE_HPP_RECURSE_GUARD

#ifndef CFAKELATENCYMODULE_HPP_GUARD
#define CFAKELATENCYMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CNetSequence {
	CNetSequence(_In_ int _Sequence, _In_ double _Timestamp) : m_iSequence(_Sequence), m_dblTimestamp(_Timestamp) {}

	int m_iSequence;
	double m_dblTimestamp;
} CNetSequence;

using CNetSequence = struct CNetSequence;

typedef struct CFakeLatencyModule : CModule {
	CFakeLatencyModule();

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnEvent(_In_ const ISimpleEvent*) override;

	void Flush();
	CNetSequence* FindSequenceEntry();
	void GenerateFakeLatency();

	CFloatValue* m_pAmount;

	CVector<CNetSequence*>* m_pvecSequences;
	int m_iLastIncomingSequence;
	double m_dblDesiredFakeLatency;
	bool m_bHasFlushed;
} CFakeLatencyModule;

using CFakeLatencyModule = struct CFakeLatencyModule;

#else //!__cplusplus
#error C++ compiler required to compile CFakeLatencyModule.hpp
#endif //__cplusplus

#endif //CFAKELATENCYMODULE_HPP_GUARD

#undef CFAKELATENCYMODULE_HPP_RECURSE_GUARD
#endif //CFAKELATENCYMODULE_HPP_RECURSE_GUARD