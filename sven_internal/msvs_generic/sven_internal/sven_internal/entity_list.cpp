#include "StdAfx.h"

CEntity::CEntity() {
	m_nFlags = 0;
	m_pDescription = nullptr;
	m_pTheEntity = nullptr;
	m_pExtraPlayerInfo = nullptr;
	m_pExtraPlayerInfo_526 = nullptr;
	memset(&m_PlayerInfo, 0, sizeof(m_PlayerInfo));
	m_eRelationshipOverride = EEntityRelationship::kNone;
}

void CEntity::TryAssigningDescription(_In_z_ const char* _ModelName) {
	m_eRelationshipOverride = EEntityRelationship::kNone;
	m_pDescription = g_pEntityDescriptions->FindDescription(_ModelName);
	if (!m_pDescription) {
		m_pDescription = g_pEntityDescriptions->m_pVecDescriptions->at(0); //"Unknown" entity description goes first
	}
}

CEntityList::CEntityList() {
	m_pEntityList = (CEntity*)Q_malloc(sizeof(CEntity) * (4096 + 1));
}

CEntityList::~CEntityList() {
	Q_free(m_pEntityList);
}

void CEntityList::Process() {
	cl_entity_t* lpTheLocalPlayer = g_pEngfuncs->GetLocalPlayer();

	for (int idx = 1; idx <= 4096; ++idx) {
		m_pEntityList[idx].m_bIsValid = false;

		if (idx == lpTheLocalPlayer->index) continue;

		cl_entity_t* lpEntity = g_pEngfuncs->GetEntityByIndex(idx);
		if (!lpEntity) continue;
		model_t* lpModel = lpEntity->model;

		if (!lpModel) continue;
		//HACKHACK: this is the second check for validness of the entity (i.e. it hasn't despawned midframe)
		//HACKHACK: maybe add 's'prite check here too? so it also shows sprite entities - xWhitey
		if (lpModel->name[0] != 'm') continue; //'m'odels
		if (lpEntity->curstate.messagenum < lpTheLocalPlayer->curstate.messagenum) continue;
		if (lpEntity->player == 1) {
			ProcessPlayer(idx);
		} else {
			m_pEntityList[idx].m_bAlive = true;
			if (const char* pszLastSlashOccurrence = strrchr(lpModel->name, '/')) {
				m_pEntityList[idx].TryAssigningDescription(pszLastSlashOccurrence + 1);

				if (m_pEntityList[idx].m_pDescription->m_eClass == EEntityClass::kCharger && (m_pEntityList[idx].m_pDescription->m_eSubclass == EEntitySubclass::kHealthChargerDrink || m_pEntityList[idx].m_pDescription->m_eSubclass == EEntitySubclass::kHEVGlass)) continue;
			} else {
				continue;
			}
		}

		m_pEntityList[idx].m_pTheEntity = lpEntity;
		m_pEntityList[idx].m_bIsValid = true;
	}
}

void CEntityList::ProcessPlayer(int _Index) {
	cl_entity_t* lpTheLocalPlayer = g_pEngfuncs->GetLocalPlayer();

	if (_Index <= 33) {
		g_pEngfuncs->pfnGetPlayerInfo(_Index, &m_pEntityList[_Index].m_PlayerInfo);
		float flHealth = -2.f; //Force assume that every player is an enemy for us.
		if (g_bIsSven5_26) {
			if (g_pExtraPlayerInfo_526) {
				m_pEntityList[_Index].m_pExtraPlayerInfo_526 = &g_pExtraPlayerInfo_526[_Index];
				flHealth = m_pEntityList[_Index].m_pExtraPlayerInfo_526->m_flHealth;
			}
		} else if (g_pExtraPlayerInfo) {
			m_pEntityList[_Index].m_pExtraPlayerInfo = &g_pExtraPlayerInfo[_Index];
			flHealth = m_pEntityList[_Index].m_pExtraPlayerInfo->m_flHealth;
		}
		m_pEntityList[_Index].m_bAlive = (flHealth > 0.f || flHealth < -1.f);
		if (!g_bIsSven5_26 && g_pExtraPlayerInfo) {
			if (m_pEntityList[_Index].m_pExtraPlayerInfo->m_sTeamNumber != g_pExtraPlayerInfo[lpTheLocalPlayer->index].m_sTeamNumber) {
				m_pEntityList[_Index].m_eRelationshipOverride = EEntityRelationship::kEnemy;
			}
		}
		if (flHealth < -1.f) {
			m_pEntityList[_Index].m_eRelationshipOverride = EEntityRelationship::kEnemy;
		} else {
			m_pEntityList[_Index].m_eRelationshipOverride = EEntityRelationship::kNone;
		}
	}
	//m_pEntityList[_Index].m_pszPlayerName = player_info.name;
	m_pEntityList[_Index].m_pDescription = g_pEntityDescriptions->m_pVecDescriptions->at(1); //player description goes second (after "Unknown" entity description)
}