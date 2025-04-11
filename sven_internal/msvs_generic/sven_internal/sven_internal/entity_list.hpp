/**
 * Copyright - xWhitey, 2024.
 * entity_list.hpp - description
 *
 * Project (Project desc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef ENTITY_LIST_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in entity_list.hpp
#else //ENTITY_LIST_HPP_RECURSE_GUARD

#define ENTITY_LIST_HPP_RECURSE_GUARD

#ifndef ENTITY_LIST_HPP_GUARD
#define ENTITY_LIST_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "extra_player_info.hpp"

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif //_MSC_VER

typedef enum {
	kIsValid = (1 << 0),
	kAlive = (1 << 1),
} EEntityFlags;

typedef struct CEntity {
	CEntity();

	union {
		char m_nFlags;

		struct {
			bool m_bIsValid : 1;
			bool m_bAlive : 1;

			bool m_bUnused2 : 1;
			bool m_bUnused3 : 1;
			bool m_bUnused4 : 1;
			bool m_bUnused5 : 1;
			bool m_bUnused6 : 1;
			bool m_bUnused7 : 1;
		};
	};

	CEntityDescription* m_pDescription;
	cl_entity_t* m_pTheEntity;
	CExtraPlayerInfo* m_pExtraPlayerInfo;
	CExtraPlayerInfo_526* m_pExtraPlayerInfo_526;
	hud_player_info_t m_PlayerInfo;
	EEntityRelationship m_eRelationshipOverride;

	void TryAssigningDescription(_In_z_ const char* _ModelName);
} CEntity;

using CEntity = struct CEntity;

#ifdef _MSC_VER
#pragma pack(pop)
#endif //_MSC_VER

typedef struct CEntityList {
	CEntityList();
	~CEntityList();

	void Process();
	void ProcessPlayer(int _Index);

	CEntity* m_pEntityList;
} CEntityList;

using CEntityList = struct CEntityList;

#else //!__cplusplus
#error C++ compiler required to compile entity_list.hpp
#endif //__cplusplus

#endif //ENTITY_LIST_HPP_GUARD

#undef ENTITY_LIST_HPP_RECURSE_GUARD
#endif //ENTITY_LIST_HPP_RECURSE_GUARD