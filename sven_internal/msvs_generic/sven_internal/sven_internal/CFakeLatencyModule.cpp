/**
 * Copyright - xWhitey, 2024.
 * CFakeLatencyModule.cpp - CFakeLatencyModule impl
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CFakeLatencyModule.hpp"

//pasted from oxware

CFakeLatencyModule::CFakeLatencyModule() : CModule("FakeLatency", "Exploit") {
	m_pVecValues->push_back(m_pAmount = Q_new(CFloatValue)("Amount", "sc_fakelatency_amount", 50.f, 2500.f, 200.f));
	m_pAmount->RegisterOnceValueChangedCallback([](void* _UserData, void* _Value, void* _PreviousValue) {
		CFakeLatencyModule* thiz = (CFakeLatencyModule*)_UserData;
		float* pflValue = (float*)_Value;
		float flValue = *pflValue;
		if (flValue < 50.f) {
			*pflValue = 50.f;
		}
		if (flValue > 2500.f) {
			*pflValue = 2500.f;
		}
	}, this);

	m_pvecSequences = Q_new(CVector<CNetSequence*>)({});

	DefineConCmd("sc_lightning_fakelatency", CFakeLatencyModule);
}

void CFakeLatencyModule::Flush() {
	m_pvecSequences->clear_delete();
	m_iLastIncomingSequence = 0;
}

CNetSequence* CFakeLatencyModule::FindSequenceEntry() {
	double dblRealtime = static_cast<double>(g_pTimer->m_nTimeInMilliseconds) / 1000.0;
	double dblDesiredLatency = static_cast<double>(m_pAmount->Get()) / 1000.0;

	for (int nIndex = 0; nIndex < m_pvecSequences->size(); nIndex++) {
		CNetSequence* pSequence = m_pvecSequences->at(nIndex);

		double dblDelta = dblRealtime - pSequence->m_dblTimestamp;

		if (dblDelta >= dblDesiredLatency) {
			m_dblDesiredFakeLatency = dblDelta;

			return pSequence;
		}
	}

	return nullptr;
}

void CFakeLatencyModule::GenerateFakeLatency() {
	CNetSequence* pEntry = FindSequenceEntry();
	if (!pEntry) {
		return;
	}

	*g_piIncomingSequence = pEntry->m_iSequence;
}

void CFakeLatencyModule::OnEnable() {
	CModule::OnEnable();

	Flush();

	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CFakeLatencyModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	
	Flush();

	CModule::OnDisable();
}

void CFakeLatencyModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kRenderEvent) {
		if (g_bIsPlaying == false && m_bHasFlushed == false && !m_pvecSequences->empty()) {
			Flush();
			m_bHasFlushed = true;
		}
		if (g_bIsPlaying == true && m_bHasFlushed == true) {
			m_bHasFlushed = false;
		}
	}

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);
		if (!e->m_iActive) return;
		if (m_bHasFlushed) return;
		if (!g_piIncomingSequence) return;

		//clamp here though it's impossible to set value lower than 50 or greater than 2500, still better safe than sorry - no memory leaks.
		decltype(m_pvecSequences->size()) cbSize = m_pvecSequences->size();
		if (cbSize > static_cast<decltype(cbSize)>(CUtils::Clamp(50.f, 2500.f, m_pAmount->Get())) /* 1-2.5 seconds */) {
			CNetSequence* pBack = m_pvecSequences->back();
			Q_delete(pBack);

			m_pvecSequences->pop_back();
		}

		int nIncomingSequence = *g_piIncomingSequence;

		if (nIncomingSequence > m_iLastIncomingSequence) {
			double dblRealtime = static_cast<double>(g_pTimer->m_nTimeInMilliseconds) / 1000.0; //convert millis to seconds

			m_pvecSequences->push_front(Q_new(CNetSequence)(nIncomingSequence, dblRealtime));

			m_iLastIncomingSequence = nIncomingSequence;
		}

		GenerateFakeLatency();
	}
}