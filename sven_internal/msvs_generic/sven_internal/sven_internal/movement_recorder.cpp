#include "StdAfx.h"
#include "movement_recorder.hpp"

#define SQR(_Value) (_Value * _Value)
#define MAX_DIFFERENCE_EPSILON 1.5f

CMovementRecord::CMovementRecord(_In_ const Vector& _ViewAngles, _In_ const Vector& _Moves, _In_ const Vector& _Origin) {
	m_vecViewAngles = _ViewAngles;
	m_vecMoves = _Moves;
	m_vecOrigin = _Origin;
}

CMovementRecorder::CMovementRecorder() {
	m_nFrame = 0;
	m_pchData = (char*) Q_malloc(sizeof(char) * 0);
	m_sAcknowledgedDataSize = 0;
	m_pOutputFile = nullptr;
	m_bMode = 0;
}

void CMovementRecorder::StartRecording(_In_z_ const char* _FileName) {
	char rgszFileNameBuffer[2048];
	size_t sFileName = strlen(_FileName);
	if (sFileName > (Q_ARRAYSIZE(rgszFileNameBuffer) - (sizeof("./lightning.tech/movement_records/") - 1))) {
		g_pEngfuncs->Con_Printf("[SEVERE] CMovementRecorder::StartRecording: I'm refusing to write my stuff into this file: %s\n", _FileName);
		return;
	}
	sprintf_s(rgszFileNameBuffer, "./lightning.tech/movement_records/%s", _FileName);
	m_pOutputFile = CFileMgr::OpenFile(rgszFileNameBuffer, "w+"); //will create the file for us if it doesn't exist and also overwrite existing one (that's why not a+)
	WriteData((char*)g_pMoveVars, sizeof(decltype(*g_pMoveVars)));
	m_bMode = 1; //recording
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CMovementRecorder::StopRecording() {
	if (m_bMode != 1) return;

	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	Flush();
	CFileMgr::CloseFile(m_pOutputFile);
	Q_free(m_pchData);
	m_sAcknowledgedDataSize = 0;
	m_nFrame = 0;
	m_bMode = 0; //nothing
	m_pOutputFile = nullptr;
}

void CMovementRecorder::Flush() {
	fwrite(m_pchData, sizeof(char), m_sAcknowledgedDataSize, reinterpret_cast<FILE*>(m_pOutputFile));
}

void CMovementRecorder::WriteData(_In_ char* _Data, _In_ size_t _DataSize) {
	m_pchData = (char*)Q_realloc(m_pchData, m_sAcknowledgedDataSize + _DataSize);
	char* pchDestination = m_pchData + m_sAcknowledgedDataSize;
	memcpy(pchDestination, _Data, _DataSize);
	m_sAcknowledgedDataSize += _DataSize;
}

void CMovementRecorder::ProcessFrame(_In_ usercmd_s* _UserCmd) {
	WriteData((char*) &m_nFrame, sizeof(decltype(m_nFrame)));
	CMovementRecord sRecord(_UserCmd->viewangles, Vector(_UserCmd->forwardmove, _UserCmd->sidemove, _UserCmd->upmove), g_pPlayerMove->origin);
	WriteData((char*) &sRecord, sizeof(decltype(sRecord)));
	m_nFrame++;
}

template<class _Ty> _Ty* ReadThisVeryStuffFromThisVeryData(char**&& _Data) {
	_Ty* pResult = (_Ty*)(*_Data);
	(*_Data) += sizeof(_Ty);
	return pResult;
}

CMovementRecord* CMovementRecorder::ReadRecordForThisVeryFrame() {
	//TODO: maybe add const int& _Frame parm?
	CMovementRecord* pResult = ReadThisVeryStuffFromThisVeryData<CMovementRecord>(&m_pchData);
	int* pnCurrentFrameInTheRecording = ReadThisVeryStuffFromThisVeryData<int>(&m_pchData);
	if (m_nFrame > *pnCurrentFrameInTheRecording) {
		m_pchData -= sizeof(CMovementRecord) + sizeof(int);
		g_pEngfuncs->Con_Printf("[SEVERE] CMovementRecorder::ReadUsercmdForThisVeryFrame: I've ran out of the data\n");
		return nullptr;
	}

	return pResult;
}

void CMovementRecorder::StartPlayback(_In_z_ const char* _FileName) {
	char rgszFileNameBuffer[2048];
	size_t sFileName = strlen(_FileName);
	if (sFileName > (Q_ARRAYSIZE(rgszFileNameBuffer) - (sizeof("./lightning.tech/movement_records/") - 1))) {
		g_pEngfuncs->Con_Printf("[SEVERE] CMovementRecorder::StartPlayback: I'm refusing to read stuff from this file: %s\n", _FileName);
		return;
	}
	sprintf_s(rgszFileNameBuffer, "./lightning.tech/movement_records/%s", _FileName);
	if (!CFileMgr::DoesFileExist(rgszFileNameBuffer)) {
		g_pEngfuncs->Con_Printf("[SEVERE] CMovementRecorder::StartPlayback: I'm aware there's no such file: %s\n", _FileName);
		return;
	}
	m_pOutputFile = CFileMgr::OpenFile(rgszFileNameBuffer, "rb");
	fseek(reinterpret_cast<FILE*>(m_pOutputFile), 0, SEEK_END);
	int iSize = ftell(reinterpret_cast<FILE*>(m_pOutputFile));
	m_pchData = (char*) (Q_malloc(iSize));
	fseek(reinterpret_cast<FILE*>(m_pOutputFile), 0, SEEK_SET);
	fread(m_pchData, sizeof(char), iSize, reinterpret_cast<FILE*>(m_pOutputFile));
	movevars_s sMoveVars;
	memcpy(&sMoveVars, m_pchData, sizeof(decltype(sMoveVars)));
	m_pchData += sizeof(decltype(sMoveVars));
	if (memcmp(&sMoveVars, g_pMoveVars, sizeof(decltype(sMoveVars)))) {
		g_pEngfuncs->Con_Printf("[SEVERE] CMovementRecorder::StartPlayback: I'm aware movevars differ in this recording from the ones we have right now.\n");
		m_bMode = 2; //playback and it's here because StopPlayback does "return" if the current mode isn't 2 (it's zero when we're inside this condition)
		StopPlayback();
		return;
	}

	m_bMode = 2; //playback
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CMovementRecorder::StopPlayback() {
	if (m_bMode != 2) return;

	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CFileMgr::CloseFile(m_pOutputFile);
	Q_free(m_pchData);
	m_nFrame = 0;
	m_sAcknowledgedDataSize = 0;
	m_bMode = 0; //nothing
	m_pOutputFile = nullptr;
}

bool CMovementRecorder::ProcessPlayback(_In_ CMovementRecord* _Recorded, _In_ usercmd_s* _Target) {
	Vector vecCurrentOrigin = g_pPlayerMove->origin;
	Vector vecDifference = _Recorded->m_vecOrigin - vecCurrentOrigin;
	float flDifference = fabsf(vecDifference.LengthSqr());
	if (flDifference > SQR(MAX_DIFFERENCE_EPSILON))
		return false;

	_Target->viewangles = _Recorded->m_vecViewAngles;

	_Target->forwardmove = _Recorded->m_vecMoves.x;
	_Target->sidemove = _Recorded->m_vecMoves.y;
	_Target->upmove = _Recorded->m_vecMoves.z;

	return true;
}

void CMovementRecorder::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (!g_pPlayerMove) {
		StopPlayback();
		StopRecording();
		return;
	}

	if (_Event->GetType() == EEventType::kUpdateEvent) {
		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);

		switch (m_bMode) {
		case 1: { //recording
			ProcessFrame(e->m_pCmd);
		}
			break;
		case 2: { //playback
			CMovementRecord* pRecorded = ReadRecordForThisVeryFrame();
			if (!pRecorded) {
				StopPlayback();
				return;
			}
			if (ProcessPlayback(pRecorded, e->m_pCmd)) {
				m_nFrame++;
			} else {
				//let's rollback for one record because we've failed to playback this very record
				m_pchData -= sizeof(m_nFrame);
				m_pchData -= sizeof(decltype(*pRecorded));
			}
		}
			break;
		}
	}
}