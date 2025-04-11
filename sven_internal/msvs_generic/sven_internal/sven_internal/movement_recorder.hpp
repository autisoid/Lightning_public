/**
 * Copyright - xWhitey, 2024.
 * movement_recorder.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef MOVEMENT_RECORDER_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in movement_recorder.hpp
#else //MOVEMENT_RECORDER_HPP_RECURSE_GUARD

#define MOVEMENT_RECORDER_HPP_RECURSE_GUARD

#ifndef MOVEMENT_RECORDER_HPP_GUARD
#define MOVEMENT_RECORDER_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "event_api.hpp"

typedef struct CMovementRecord {
	CMovementRecord(_In_ const Vector& _ViewAngles, _In_ const Vector& _Moves, _In_ const Vector& _Origin);

	Vector m_vecViewAngles;
	Vector m_vecMoves;
	Vector m_vecOrigin;
} CMovementRecord;

using CMovementRecord = struct CMovementRecord;

typedef struct CMovementRecorder : CAbstractEventListener {
	CMovementRecorder();

	void StartRecording(_In_z_ const char* _FileName);
	void StopRecording();
	void Flush();
	void WriteData(_In_ char* _Data, _In_ size_t _DataSize);

	void StartPlayback(_In_z_ const char* _FileName);
	void StopPlayback();
	CMovementRecord* ReadRecordForThisVeryFrame();
	bool ProcessPlayback(_In_ CMovementRecord* _Recorded, _In_ usercmd_s* _Target);

	void ProcessFrame(_In_ usercmd_s* _UserCmd);

	void OnEvent(_In_ const ISimpleEvent* _Event) override;

	int m_nFrame;
	char* m_pchData;
	size_t m_sAcknowledgedDataSize;
	void* m_pOutputFile; //could be FILE*
	byte m_bMode;
} CMovementRecorder;

using CMovementRecorder = struct CMovementRecorder;

#else //!__cplusplus
#error C++ compiler required to compile movement_recorder.hpp
#endif //__cplusplus

#endif //MOVEMENT_RECORDER_HPP_GUARD

#undef MOVEMENT_RECORDER_HPP_RECURSE_GUARD
#endif //MOVEMENT_RECORDER_HPP_RECURSE_GUARD