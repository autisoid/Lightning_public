/**
 * Copyright - xWhitey, 2023.
 * CESPModule.cpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"
#include "CESPModule.hpp"
#include "CClickGUIModule.hpp"	
#include "AlbertSansFont.h"

void DrawStringWithShadow(_In_ float _X, _In_ float _Y, _In_ ImColor _Color, _In_z_ _Pre_z_ const char* _Text, _In_ float _FontSize) {
	for (int idx = 1; idx <= 1; idx++) {
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X + idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X - idx, _Y), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y + idx), ImColor(0, 0, 0), _Text);
		ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y - idx), ImColor(0, 0, 0), _Text);
	}
	ImGui::GetForegroundDrawList()->AddText(g_pClientFont, _FontSize, ImVec2(_X, _Y), _Color, _Text);
}

CESPModule::CESPModule() : CModule("ESP", "Render") {
	m_ColorEnemy = ImColor(255, 0, 0, 255); //red
	m_ColorFriend = ImColor(0, 255, 0, 255); //green
	m_ColorUnknown = ImColor(255, 255, 0, 255); //yellow
	m_ColorItem = ImColor(0, 255, 255, 255); //cyan

	m_pVecValues->push_back(m_pDrawPlayers = Q_new(CBoolValue)("Draw players", "sc_esp_draw_players", true));
	m_pVecValues->push_back(m_pDrawItems = Q_new(CBoolValue)("Draw items", "sc_esp_draw_items", true));
	m_pVecValues->push_back(m_pDrawOtherEntities = Q_new(CBoolValue)("Draw other entities", "sc_esp_draw_other_entities", true));
	m_pVecValues->push_back(m_pDrawUnknown = Q_new(CBoolValue)("Draw unknown", "sc_esp_draw_unknown", true));
	m_pVecValues->push_back(m_pDrawFurniture = Q_new(CBoolValue)("Draw furniture", "sc_esp_draw_furniture", true));
	m_pVecValues->push_back(m_pConcatUnknownEntityModel = Q_new(CBoolValue)("Concatenate unknown entity's model", "sc_esp_concat_unknown_entity_model", false));
	m_pVecValues->push_back(m_pFontSize = Q_new(CFloatValue)("Font size", "sc_esp_font_size", 10.f, 30.f, 18.f));
	g_bForceImGuiToNotDrawAnything = true;
	m_pDrawingFont = Q_new(CCustomFont)("AlbertSans", indirect_cast<char*>(AlbertSans_compressed_data_base85), static_cast<int>(m_pFontSize->Get()), m_pFontSize->Get(), true);
	g_bForceImGuiToNotDrawAnything = false;

	/*m_pFontSize->RegisterOnceValueChangedCallback([](void* _UserData, void* _Value, void* _PreviousValue) {
		CESPModule* thiz = (CESPModule*)_UserData;

		g_bForceImGuiToNotDrawAnything = true;

		ImGui_ImplOpenGL3_Shutdown();

		Q_delete(thiz->m_pDrawingFont);

        //FIXME: BROKEN: The esp module is intended to reload the font once its size has been changed but this fucks up imgui rendering (only in opengl3 mode, why?) - xWhitey
		//std::async([](CESPModule* _Instance) {
		//	_Instance->m_pDrawingFont = Q_new(CCustomFont)("AlbertSans", indirect_cast<char*>(AlbertSans_compressed_data_base85), static_cast<int>(_Instance->m_pFontSize->Get()), _Instance->m_pFontSize->Get(), true);
		//}, thiz);

		ImGui_ImplOpenGL3_Init();

		g_bForceImGuiToNotDrawAnything = false;
	}, this);*/

	m_pVecValues->push_back(m_pEnemyColour = Q_new(CColourValue)("Enemy colour", "sc_esp_enemy_colour", [](void* _UserData, float* _RGBA) {
		CESPModule* thiz = (CESPModule*)_UserData;
		thiz->m_ColorEnemy = ImColor(_RGBA[0], _RGBA[1], _RGBA[2], _RGBA[3]);
	}, this, m_ColorEnemy.Value.x, m_ColorEnemy.Value.y, m_ColorEnemy.Value.z));
	m_pVecValues->push_back(m_pFriendColour = Q_new(CColourValue)("Friend colour", "sc_esp_friend_colour", [](void* _UserData, float* _RGBA) {
		CESPModule* thiz = (CESPModule*)_UserData;
		thiz->m_ColorFriend = ImColor(_RGBA[0], _RGBA[1], _RGBA[2], _RGBA[3]);
	}, this, m_ColorFriend.Value.x, m_ColorFriend.Value.y, m_ColorFriend.Value.z));
	m_pVecValues->push_back(m_pUnknownColour = Q_new(CColourValue)("Unknown colour", "sc_esp_unknown_colour", [](void* _UserData, float* _RGBA) {
		CESPModule* thiz = (CESPModule*)_UserData;
		thiz->m_ColorUnknown = ImColor(_RGBA[0], _RGBA[1], _RGBA[2], _RGBA[3]);
	}, this, m_ColorUnknown.Value.x, m_ColorUnknown.Value.y, m_ColorUnknown.Value.z));
	m_pEnemyColour->AddFlag(kValueFlagDontRender);
	m_pFriendColour->AddFlag(kValueFlagDontRender);
	m_pUnknownColour->AddFlag(kValueFlagDontRender);

	m_pVecValues->push_back(m_pExtraValues = Q_new(CExtraValues)("Extra values", "sc_esp_extra_values", [](void* _UserData) {
		CESPModule* thiz = (CESPModule*)_UserData;
		float* copy = (float*)Q_malloc(sizeof(float) * 4);
		copy[0] = thiz->m_ColorEnemy.Value.x;
		copy[1] = thiz->m_ColorEnemy.Value.y;
		copy[2] = thiz->m_ColorEnemy.Value.z;
		copy[3] = thiz->m_ColorEnemy.Value.w;
		if (ImGui::ColorPicker4("Enemy colour", copy)) {
			thiz->m_pEnemyColour->m_pfnOnceChangedCB(thiz, copy);
			thiz->m_pEnemyColour->m_aflRGBA[0] = copy[0];
			thiz->m_pEnemyColour->m_aflRGBA[1] = copy[1];
			thiz->m_pEnemyColour->m_aflRGBA[2] = copy[2];
			thiz->m_pEnemyColour->m_aflRGBA[3] = copy[3];
			CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
		}
		Q_free(copy);
		ImGui::SameLine();
		copy = (float*)Q_malloc(sizeof(float) * 4);
		copy[0] = thiz->m_ColorFriend.Value.x;
		copy[1] = thiz->m_ColorFriend.Value.y;
		copy[2] = thiz->m_ColorFriend.Value.z;
		copy[3] = thiz->m_ColorFriend.Value.w;
		if (ImGui::ColorPicker4("Friend colour", copy)) {
			thiz->m_pFriendColour->m_pfnOnceChangedCB(thiz, copy);
			thiz->m_pFriendColour->m_aflRGBA[0] = copy[0];
			thiz->m_pFriendColour->m_aflRGBA[1] = copy[1];
			thiz->m_pFriendColour->m_aflRGBA[2] = copy[2];
			thiz->m_pFriendColour->m_aflRGBA[3] = copy[3];
			CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
		}
		Q_free(copy);
		ImGui::SameLine();
		copy = (float*)Q_malloc(sizeof(float) * 4);
		copy[0] = thiz->m_ColorUnknown.Value.x;
		copy[1] = thiz->m_ColorUnknown.Value.y;
		copy[2] = thiz->m_ColorUnknown.Value.z;
		copy[3] = thiz->m_ColorUnknown.Value.w;
		if (ImGui::ColorPicker4("Unknown colour", copy)) {
			thiz->m_pUnknownColour->m_pfnOnceChangedCB(thiz, copy);
			thiz->m_pUnknownColour->m_aflRGBA[0] = copy[0];
			thiz->m_pUnknownColour->m_aflRGBA[1] = copy[1];
			thiz->m_pUnknownColour->m_aflRGBA[2] = copy[2];
			thiz->m_pUnknownColour->m_aflRGBA[3] = copy[3];
			CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
		}
		Q_free(copy);
		ImGui::SameLine();
		copy = (float*)Q_malloc(sizeof(float) * 4);
		copy[0] = thiz->m_ColorItem.Value.x;
		copy[1] = thiz->m_ColorItem.Value.y;
		copy[2] = thiz->m_ColorItem.Value.z;
		copy[3] = thiz->m_ColorItem.Value.w;
		if (ImGui::ColorPicker4("Item colour", copy)) {
			thiz->m_pItemColour->m_pfnOnceChangedCB(thiz, copy);
			thiz->m_pItemColour->m_aflRGBA[0] = copy[0];
			thiz->m_pItemColour->m_aflRGBA[1] = copy[1];
			thiz->m_pItemColour->m_aflRGBA[2] = copy[2];
			thiz->m_pItemColour->m_aflRGBA[3] = copy[3];
			CCheat::GetCheat()->m_pModuleManager->SaveValuesToConfigFile();
		}
		Q_free(copy);
	}, this));

	DefineConCmd("sc_lightning_esp", CESPModule);
}

void CESPModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CESPModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CESPModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kRenderEvent) {
		if (g_bIsPlaying == false) return;
		if (g_pClickGUIModule->m_bState == true) return;
		if (!g_pPlayerMove) return;
		if (!g_pRefParams) return;
		if (!g_pEngfuncs->GetLocalPlayer()) return;

		for (int idx = 1; idx <= 4096; ++idx) {
			CEntity& entity = g_pEntityList->m_pEntityList[idx];
			if (!entity.m_bIsValid) continue;
			if (!entity.m_pTheEntity) continue;

			cl_entity_t* lpEntity = entity.m_pTheEntity;

			if (!entity.m_pDescription) {
				model_t* lpModel = lpEntity->model;
				if (!lpModel) continue;

				if (const char* pszLastSlashOccurrence = strrchr(lpModel->name, '/')) {
					entity.TryAssigningDescription(pszLastSlashOccurrence + 1);
				}

				if (!entity.m_pDescription) continue; //must be
			}

			if (!m_pDrawPlayers->Get() && entity.m_pDescription->m_eSubtype == EEntitySubtype::kPlayer) continue;
			if (!m_pDrawItems->Get() && entity.m_pDescription->m_eSubtype == EEntitySubtype::kItem) continue;
			if (!m_pDrawOtherEntities->Get() && entity.m_pDescription->m_eSubtype == EEntitySubtype::kOtherEntity) continue;
			if (!m_pDrawUnknown->Get() && entity.m_pDescription->m_eSubtype == EEntitySubtype::kUnknown) continue;
			if (!m_pDrawFurniture->Get() && entity.m_pDescription->m_eClass == EEntityClass::kFurniture) continue;

            //Ichthyosaur is using MOVETYPE_TOSS - xWhitey
			//if (lpEntity->curstate.movetype == MOVETYPE_NONE || lpEntity->curstate.movetype == MOVETYPE_TOSS) continue;

			Vector vecRealMins = lpEntity->curstate.mins;
			Vector vecRealMaxs = lpEntity->curstate.maxs;
			switch (entity.m_pDescription->m_eClass) {
			case EEntityClass::kFurniture:
			case EEntityClass::kCheckpoint: {
				vecRealMins = vecRealMaxs = g_vecZero;
			}
				break;
			default:
				break;
			}

			Vector vecBottom = lpEntity->origin;
			Vector vecTop = lpEntity->origin;

			if (entity.m_pDescription->m_eSubclass == EEntitySubclass::kEyeScanner) {
				vecTop.z += 60.f;
				vecBottom.z += 60.f;
			}

			float flLeft, flTop, flRight, flBottom, flBoxHeight, flBoxWidth, flBottomMiddleX, flBottomMiddleY;
			Vector2D vecScreenBottom, vecScreenTop;

			if (entity.m_pDescription->m_eClass != EEntityClass::kPlayer) {
				if (entity.m_pDescription->m_eClass != EEntityClass::kItem) {
					vecTop.z += vecRealMaxs.z;
					vecBottom.z -= vecRealMins.z;

					float flHeight = (vecRealMaxs.z - vecRealMins.z);

					flBoxWidth = (flHeight != 0.f) ? (max(vecRealMaxs.x - vecRealMins.x, vecRealMaxs.y - vecRealMins.y) / flHeight) : 0.f;
				}
			} else {
				if (lpEntity->curstate.usehull) {
					vecTop.z += VEC_DUCK_HULL_MAX.z;
					vecBottom.z += VEC_DUCK_HULL_MIN.z;

					flBoxWidth = (VEC_DUCK_HULL_MAX.x - VEC_DUCK_HULL_MIN.x) / (VEC_DUCK_HULL_MAX.z - VEC_DUCK_HULL_MIN.z);
				} else {
					vecTop.z += vecRealMaxs.z;
					vecBottom.z -= vecRealMaxs.z;

					flBoxWidth = (VEC_HULL_MAX.x - VEC_HULL_MIN.x) / (VEC_HULL_MAX.z - VEC_HULL_MIN.z);
				}
			}

			if (!CUtils::CalcScreen(vecBottom, vecScreenBottom, g_ScreenInfo) || !CUtils::CalcScreen(vecTop, vecScreenTop, g_ScreenInfo))
				continue;

			flBottomMiddleX = vecScreenBottom.x;
			flBottomMiddleY = vecScreenBottom.y;
			flBoxHeight = vecScreenBottom.y - vecScreenTop.y;
			flBoxWidth = flBoxHeight * flBoxWidth;
			flLeft = vecScreenBottom.x - (flBoxWidth * 0.5f); // rotate around the pivot/legs (credits: Sw1ft747)
			flTop = vecScreenBottom.y - flBoxHeight;
			flRight = vecScreenBottom.x + (flBoxWidth * 0.5f);
			flBottom = vecScreenBottom.y;

			ImColor colRenderColor = m_ColorFriend;
			if (entity.m_pDescription->m_eRelationship == EEntityRelationship::kEnemy || entity.m_eRelationshipOverride == EEntityRelationship::kEnemy || entity.m_eRelationshipOverride == EEntityRelationship::kMinorEnemy) {
				colRenderColor = m_ColorEnemy;
			}
			if (entity.m_pDescription->m_eClass == EEntityClass::kUnknown) {
				colRenderColor = m_ColorUnknown;
			}
			if (entity.m_pDescription->m_eRelationship == EEntityRelationship::kNeutral) {
				colRenderColor = m_ColorUnknown;
			}
			if (entity.m_pDescription->m_eClass == EEntityClass::kGrenade) {
                //BROKEN: Grenades on the floor are marked as enemies because they use SOLID_BBOX (so they can be picked up), fix that somehow... - xWhitey
				if (lpEntity->curstate.solid == SOLID_BBOX) //The grenade is flying midair! Needs to be marked as enemy, so the player can notice it. - xWhitey
					colRenderColor = m_ColorEnemy;
                //The grenade has been revived or has stopped moving! Let's mark it with the yellow colour. - xWhitey
				switch (lpEntity->curstate.movetype) {
				case MOVETYPE_STEP:
				case MOVETYPE_FLY:
				case MOVETYPE_TOSS:
					colRenderColor = m_ColorUnknown;
					break;
				}
			}
			if (entity.m_pDescription->m_eClass == EEntityClass::kItem && entity.m_pDescription->m_eRelationship != EEntityRelationship::kEnemy && entity.m_pDescription->m_eRelationship != EEntityRelationship::kMinorEnemy) {
				colRenderColor = m_ColorItem;
			}
			if (!entity.m_bAlive) {
				colRenderColor = ImColor(75, 0, 130); //indigo
			}

			char* szBuffer = nullptr;
			const char* lpszName = entity.m_pDescription->m_pszHumanReadableName;
			if (entity.m_pDescription->m_eClass == EEntityClass::kPlayer) {
				lpszName = entity.m_PlayerInfo.name;
			}
			if (entity.m_pDescription->m_eClass == EEntityClass::kUnknown) {
				if (m_pConcatUnknownEntityModel->Get()) { //A debug option to render the model name in case if we got something unknown and we want that to be named. - xWhitey
					if (entity.m_pTheEntity->model) {
						int iLength = strlen(lpszName) + strlen(entity.m_pTheEntity->model->name) + sizeof('(') + sizeof(')') + sizeof(' ') + 1;
						szBuffer = (char*)Q_malloc(iLength + 1);
						snprintf(szBuffer, iLength, "%s (%s)", lpszName, entity.m_pTheEntity->model->name);
					} else {
						int iLength = strlen(lpszName);
						szBuffer = (char*)Q_malloc(iLength + 1);
						snprintf(szBuffer, iLength, "%s", lpszName);
					}
				}
			}

			if (!lpszName) continue;

			if (entity.m_pDescription->m_eClass != EEntityClass::kItem) {
				ImGui::GetForegroundDrawList()->AddRect(ImVec2(flLeft - 2.f, flTop - 2.f), ImVec2(flRight + 2.f, flBottom + 2.f), ImColor(0, 0, 0, 255), 0.f, 0);
				ImGui::GetForegroundDrawList()->AddRect(ImVec2(flLeft + 2.f, flTop + 2.f), ImVec2(flRight - 2.f, flBottom - 2.f), ImColor(0, 0, 0, 255), 0.f, 0);
				ImGui::GetForegroundDrawList()->AddRect(ImVec2(flLeft, flTop), ImVec2(flRight, flBottom), colRenderColor);
			}
			ImVec2 vecTextSize = g_pClientFont->CalcTextSizeA(m_pFontSize->Get(), FLT_MAX, 0.f, lpszName);
			float flNaturalBoxHeight = flBoxHeight;
			if (entity.m_pDescription->m_eClass == EEntityClass::kCockroach) {
				flNaturalBoxHeight *= 2.f;
			}
			if (flNaturalBoxHeight < 1.f) {
				flNaturalBoxHeight = 1.f;
			}
			float flTextScaleFactor = CUtils::Clamp(16.f, m_pFontSize->Get(), m_pFontSize->Get() - max(vecTextSize.y / flNaturalBoxHeight, 0.1f));
			if (szBuffer) {
				DrawStringWithShadow(flBottomMiddleX - (vecTextSize.x / 2.f), flBottomMiddleY + 8.f, colRenderColor, szBuffer, flTextScaleFactor);
				Q_free(szBuffer);
			} else {
				DrawStringWithShadow(flBottomMiddleX - (vecTextSize.x / 2.f), flBottomMiddleY + 8.f, colRenderColor, lpszName, flTextScaleFactor);
			}
		}
	}
}