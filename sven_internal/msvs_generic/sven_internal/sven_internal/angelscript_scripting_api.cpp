#include "StdAfx.h"
#include "angelscript_scripting_api.hpp"
#include "scriptarray.h"
#include "scriptbuilder.h"
#include "scriptstdstring.h"
#include "script_string.hpp"
#include "scriptany.h"
#include "script_console.hpp"
#include "script_module.hpp"
#include "as_vector.hpp"
#include "as_event_api.hpp"
#include "as_value_api.hpp"
#include "as_enginestuff.hpp"

void __cdecl CTheScripts::MessageCB(const void* _Message, void* _Param) {
	const asSMessageInfo* pMessage = (const asSMessageInfo*)(_Message);

	const char* pszType = "[ERROR]";
	if (pMessage->type == asMSGTYPE_WARNING)
		pszType = "WARN";
	else if (pMessage->type == asMSGTYPE_INFORMATION)
		pszType = "INFO";
	CCheat::GetCheat()->m_pConsole->Printf("%s (%d, %d) : %s : %s\n", pMessage->section, pMessage->row, pMessage->col, pszType, pMessage->message);
}

extern CBaseScriptModule* CBaseScriptModuleFactory(const std::string& _Name, const std::string& _Category, int _KeyBind, bool _ShownInArrayList);
extern CAbstractEventListener* CBaseScriptModuleToCAbstractEventListener(CBaseScriptModule* _Mod);
extern CASVector* CASVectorFactory(float _X, float _Y, float _Z);
extern CASVector* CASVectorFactory();
extern CASRect* CASRectFactory(const int& _Left, const int& _Right, const int& _Top, const int& _Bottom);
extern CASRect* CASRectFactory();
extern CScriptValue* CScriptValueFactory(const std::string& _Name, const std::string& _ConvarName);
extern CScriptBoolValue* CScriptBoolValueFactory(const std::string& _Name, const std::string& _ConvarName, const bool& _DefaultValue);
extern void RegisterScriptBoolOnceChangedCallback(CScriptBoolValue* _Value, asIScriptFunction* _Function, CScriptAny* _UserData);

#define ASRegisterEnumValue(_Enum, _Value) do { r = m_pScriptEngine->RegisterEnumValue(#_Enum, #_Value, (int) _Enum::_Value); if (r < 0) { CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::Initialise: RegisterEnumValue for %s::%s returned value less than zero: %d.\n", #_Enum, #_Value, r); } } while (0)
#define ASRegisterObjectType(_Name) do { r = m_pScriptEngine->RegisterObjectType(#_Name, 0, asOBJ_REF | asOBJ_NOCOUNT); } while (0)
#define ASRegisterObjectProperty(_Class, _Declaration, _Field) do { r = m_pScriptEngine->RegisterObjectProperty(#_Class, _Declaration, asOFFSET(_Class, _Field)); } while (0)
#define ASRegisterRawObjectProperty(_ClassName, _Class, _Declaration, _Field) do { r = m_pScriptEngine->RegisterObjectProperty(_ClassName, _Declaration, asOFFSET(_Class, _Field)); } while (0)
#define ASRegisterRawEnumValue(_Enum, _Value) do { r = m_pScriptEngine->RegisterEnumValue(#_Enum, #_Value, (int) _Value); if (r < 0) { CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::Initialise: RegisterEnumValue for %s::%s returned value less than zero: %d.\n", #_Enum, #_Value, r); } } while (0)

void CTheScripts::Reload() {
	m_pScriptEngine->ShutDownAndRelease();
	m_pEngineCommands->UnregisterCommands();

	//TODO: Implement	

	Initialise();
}

void CTheScripts::RegisterStructures() {
	int r = 0;
	r = m_pScriptEngine->RegisterTypedef("HSPRITE_HL", "int");
	r = m_pScriptEngine->RegisterTypedef("char", "int8");
	r = m_pScriptEngine->RegisterTypedef("byte", "uint8");
	r = m_pScriptEngine->RegisterTypedef("short", "int16");
	r = m_pScriptEngine->RegisterTypedef("ushort", "uint16");
	r = m_pScriptEngine->RegisterTypedef("size_t", "uint32");

	r = m_pScriptEngine->RegisterObjectType("CVector", sizeof(Vector), asOBJ_REF);
	//vvv  this one won't register for some reason
	//r = m_pScriptEngine->RegisterObjectBehaviour("CVector", asBEHAVE_CONSTRUCT, "void f(float _X, float _Y, float _Z)", asFUNCTIONPR(CASVectorFactory, (float, float, float), CASVector*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CVector", asBEHAVE_FACTORY, "CVector@ f(float _X, float _Y, float _Z)", asFUNCTIONPR(CASVectorFactory, (float, float, float), CASVector*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CVector", asBEHAVE_FACTORY, "CVector@ f()", asFUNCTIONPR(CASVectorFactory, (), CASVector*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CVector", asBEHAVE_ADDREF, "void f()", asMETHOD(CASVector, AddRef), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CVector", asBEHAVE_RELEASE, "void f()", asMETHOD(CASVector, Release), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector& opAssign(const CVector& in _Rhs)", asMETHOD(CASVector, operator=), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float& opIndex(uint _Which)", asMETHODPR(CASVector, At, (unsigned int), float*), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float opIndex(uint _Which) const", asMETHODPR(CASVector, At, (unsigned int) const, float), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "bool opEquals(const CVector& in _Rhs)", asMETHOD(CASVector, operator==), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "bool opEquals(const CVector& in _Rhs) const", asMETHOD(CASVector, operator==), asCALL_THISCALL);
	//FIXME: vvvvv these won't register too, RegisterObjectMethod returns asINVALID_DECLARATION
	/*r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector opAdd(const CVector& in other) const", asMETHOD(CASVector, operator+), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector opSub(const CVector& in other) const", asMETHOD(CASVector, operator-), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector opDiv(float fl) const", asMETHOD(CASVector, operator/), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector opDiv_r(float fl) const", asMETHOD(CASVector, operator/), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector opMul(float fl) const", asMETHOD(CASVector, operator*), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "CVector opMul_r(float fl) const", asMETHOD(CASVector, operator*), asCALL_THISCALL);*/
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float Length()", asMETHOD(CASVector, Length), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float LengthSqr()", asMETHOD(CASVector, LengthSqr), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float Length2D()", asMETHOD(CASVector, Length2D), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float Length2DSqr()", asMETHOD(CASVector, Length2DSqr), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float Magnitude()", asMETHOD(CASVector, Length), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CVector", "float MagnitudeSqr()", asMETHOD(CASVector, LengthSqr), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectProperty("CVector", "float x", asOFFSET(CASVector, x));
	r = m_pScriptEngine->RegisterObjectProperty("CVector", "float y", asOFFSET(CASVector, y));
	r = m_pScriptEngine->RegisterObjectProperty("CVector", "float z", asOFFSET(CASVector, z));

	r = m_pScriptEngine->RegisterEnum("eDeadFlag");
	ASRegisterRawEnumValue(eDeadFlag, DEAD_NO);
	ASRegisterRawEnumValue(eDeadFlag, DEAD_DYING);
	ASRegisterRawEnumValue(eDeadFlag, DEAD_DEAD);
	ASRegisterRawEnumValue(eDeadFlag, DEAD_RESPAWNABLE);
	ASRegisterRawEnumValue(eDeadFlag, DEAD_DISCARDBODY);
	r = m_pScriptEngine->RegisterEnum("eMoveTypes");
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_NONE);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_WALK);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_STEP);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_FLY);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_TOSS);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_PUSH);
    ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_NOCLIP);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_FLYMISSILE);
    ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_BOUNCE);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_BOUNCEMISSILE);
    ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_FOLLOW);
	ASRegisterRawEnumValue(eMoveTypes, MOVETYPE_PUSHSTEP);
	r = m_pScriptEngine->RegisterEnum("eEdictFlags");
	ASRegisterRawEnumValue(eEdictFlags, FL_FLY);
	ASRegisterRawEnumValue(eEdictFlags, FL_SWIM);
	ASRegisterRawEnumValue(eEdictFlags, FL_CONVEYOR);
	ASRegisterRawEnumValue(eEdictFlags, FL_CLIENT);
	ASRegisterRawEnumValue(eEdictFlags, FL_INWATER);
	ASRegisterRawEnumValue(eEdictFlags, FL_MONSTER);
	ASRegisterRawEnumValue(eEdictFlags, FL_GODMODE);
	ASRegisterRawEnumValue(eEdictFlags, FL_NOTARGET);
	ASRegisterRawEnumValue(eEdictFlags, FL_SKIPLOCALHOST);
	ASRegisterRawEnumValue(eEdictFlags, FL_ONGROUND);
	ASRegisterRawEnumValue(eEdictFlags, FL_PARTIALGROUND);
	ASRegisterRawEnumValue(eEdictFlags, FL_WATERJUMP);
	ASRegisterRawEnumValue(eEdictFlags, FL_FROZEN);
	ASRegisterRawEnumValue(eEdictFlags, FL_FAKECLIENT);
	ASRegisterRawEnumValue(eEdictFlags, FL_DUCKING);
	ASRegisterRawEnumValue(eEdictFlags, FL_FLOAT);
	ASRegisterRawEnumValue(eEdictFlags, FL_GRAPHED);
	ASRegisterRawEnumValue(eEdictFlags, FL_IMMUNE_WATER);
	ASRegisterRawEnumValue(eEdictFlags, FL_IMMUNE_SLIME);
	ASRegisterRawEnumValue(eEdictFlags, FL_IMMUNE_LAVA);
	ASRegisterRawEnumValue(eEdictFlags, FL_PROXY);
	ASRegisterRawEnumValue(eEdictFlags, FL_ALWAYSTHINK);
	ASRegisterRawEnumValue(eEdictFlags, FL_BASEVELOCITY);
	ASRegisterRawEnumValue(eEdictFlags, FL_MONSTERCLIP);
	ASRegisterRawEnumValue(eEdictFlags, FL_ONTRAIN);
	ASRegisterRawEnumValue(eEdictFlags, FL_WORLDBRUSH);
	ASRegisterRawEnumValue(eEdictFlags, FL_SPECTATOR);
	ASRegisterRawEnumValue(eEdictFlags, FL_CUSTOMENTITY);
	ASRegisterRawEnumValue(eEdictFlags, FL_KILLME);
	ASRegisterRawEnumValue(eEdictFlags, FL_DORMANT);
	r = m_pScriptEngine->RegisterEnum("eInputButtons");
	ASRegisterRawEnumValue(eInputButtons, IN_ATTACK);
	ASRegisterRawEnumValue(eInputButtons, IN_JUMP);
	ASRegisterRawEnumValue(eInputButtons, IN_DUCK);
	ASRegisterRawEnumValue(eInputButtons, IN_FORWARD);
	ASRegisterRawEnumValue(eInputButtons, IN_BACK);
	ASRegisterRawEnumValue(eInputButtons, IN_USE);
	ASRegisterRawEnumValue(eInputButtons, IN_CANCEL);
	ASRegisterRawEnumValue(eInputButtons, IN_LEFT);
	ASRegisterRawEnumValue(eInputButtons, IN_RIGHT);
	ASRegisterRawEnumValue(eInputButtons, IN_MOVELEFT);
	ASRegisterRawEnumValue(eInputButtons, IN_MOVERIGHT);
	ASRegisterRawEnumValue(eInputButtons, IN_ATTACK2);
	ASRegisterRawEnumValue(eInputButtons, IN_RUN);
	ASRegisterRawEnumValue(eInputButtons, IN_RELOAD);
	ASRegisterRawEnumValue(eInputButtons, IN_ALT1);
	ASRegisterRawEnumValue(eInputButtons, IN_SCORE);
	r = m_pScriptEngine->RegisterEnum("eTakeDamage");
	ASRegisterRawEnumValue(eTakeDamage, DAMAGE_NO);
	ASRegisterRawEnumValue(eTakeDamage, DAMAGE_YES);
	ASRegisterRawEnumValue(eTakeDamage, DAMAGE_AIM);
	r = m_pScriptEngine->RegisterEnum("eStdLimits");
	ASRegisterRawEnumValue(eStdLimits, MAX_ALIAS_NAME);
	ASRegisterRawEnumValue(eStdLimits, MAX_CLIENTS);
	ASRegisterRawEnumValue(eStdLimits, MAX_PHYSENTS);
	ASRegisterRawEnumValue(eStdLimits, MAX_PHYSINFO_STRING);
	ASRegisterRawEnumValue(eStdLimits, MAX_MOVEENTS);
	ASRegisterRawEnumValue(eStdLimits, MAX_CLIP_PLANES);
	ASRegisterRawEnumValue(eStdLimits, MAX_ENT_LEAFS);
	ASRegisterRawEnumValue(eStdLimits, MAX_MODEL_NAME);
	ASRegisterRawEnumValue(eStdLimits, MAX_MAP_HULLS);
	size_t SCREENINFO_SIZE = sizeof(SCREENINFO);
	ASRegisterRawEnumValue(eStdLimits, SCREENINFO_SIZE);

	r = m_pScriptEngine->RegisterObjectType("rect_s", sizeof(rect_s), asOBJ_REF);
	r = m_pScriptEngine->RegisterObjectBehaviour("rect_s", asBEHAVE_FACTORY, "rect_s@ f(const int& in _Left, const int& in _Right, const int& in _Top, const int& in _Bottom)", asFUNCTIONPR(CASRectFactory, (const int&, const int&, const int&, const int&), CASRect*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("rect_s", asBEHAVE_FACTORY, "rect_s@ f()", asFUNCTIONPR(CASRectFactory, (), CASRect*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("rect_s", asBEHAVE_ADDREF, "void f()", asMETHOD(CASRect, AddRef), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectBehaviour("rect_s", asBEHAVE_RELEASE, "void f()", asMETHOD(CASRect, Release), asCALL_THISCALL);
	ASRegisterObjectProperty(rect_s, "int left", left);
	ASRegisterObjectProperty(rect_s, "int right", right);
	ASRegisterObjectProperty(rect_s, "int top", top);
	ASRegisterObjectProperty(rect_s, "int bottom", bottom);

	ASRegisterObjectType(model_s);
	// TODO: Implement

	ASRegisterObjectType(physent_t);
	r = m_pScriptEngine->RegisterObjectMethod("physent_t", "char get_name(int _Index) property", asFUNCTIONPR(CASPhysicalEntity::GetNameSingleChar, (physent_t*, int), char), asCALL_CDECL_OBJFIRST);
	ASRegisterObjectProperty(physent_t, "int player", player);
	ASRegisterObjectProperty(physent_t, "CVector origin", origin);
	ASRegisterObjectProperty(physent_t, "model_s@ model", model);
	ASRegisterObjectProperty(physent_t, "model_s@ studiomodel", studiomodel);
	ASRegisterObjectProperty(physent_t, "CVector mins", mins);
	ASRegisterObjectProperty(physent_t, "CVector maxs", maxs);
	ASRegisterObjectProperty(physent_t, "int info", info);
	ASRegisterObjectProperty(physent_t, "CVector angles", angles);
	ASRegisterObjectProperty(physent_t, "int solid", solid);
	ASRegisterObjectProperty(physent_t, "int skin", skin);
	ASRegisterObjectProperty(physent_t, "int rendermode", rendermode);
	ASRegisterObjectProperty(physent_t, "float frame", frame);
	ASRegisterObjectProperty(physent_t, "int sequence", sequence);
	r = m_pScriptEngine->RegisterObjectMethod("physent_t", "byte get_controller(int _Index) property", asFUNCTIONPR(CASPhysicalEntity::GetControllerAtIndex, (physent_t*, int), byte), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("physent_t", "byte get_blending(int _Index) property", asFUNCTIONPR(CASPhysicalEntity::GetBlendingAtIndex, (physent_t*, int), byte), asCALL_CDECL_OBJFIRST);
	ASRegisterObjectProperty(physent_t, "int movetype", movetype);
	ASRegisterObjectProperty(physent_t, "int takedamage", takedamage);
	ASRegisterObjectProperty(physent_t, "int blooddecal", blooddecal);
	ASRegisterObjectProperty(physent_t, "int team", team);
	ASRegisterObjectProperty(physent_t, "int classnumber", classnumber);
	ASRegisterObjectProperty(physent_t, "int iuser1", iuser1);
	ASRegisterObjectProperty(physent_t, "int iuser2", iuser2);
	ASRegisterObjectProperty(physent_t, "int iuser3", iuser3);
	ASRegisterObjectProperty(physent_t, "int iuser4", iuser4);
	ASRegisterObjectProperty(physent_t, "float fuser1", fuser1);
	ASRegisterObjectProperty(physent_t, "float fuser2", fuser2);
	ASRegisterObjectProperty(physent_t, "float fuser3", fuser3);
	ASRegisterObjectProperty(physent_t, "float fuser4", fuser4);
	ASRegisterObjectProperty(physent_t, "CVector vuser1", vuser1);
	ASRegisterObjectProperty(physent_t, "CVector vuser2", vuser2);
	ASRegisterObjectProperty(physent_t, "CVector vuser3", vuser3);
	ASRegisterObjectProperty(physent_t, "CVector vuser4", vuser4);

	ASRegisterObjectType(usercmd_s);
	ASRegisterObjectProperty(usercmd_s, "short lerp_msec", lerp_msec);
	ASRegisterObjectProperty(usercmd_s, "byte msec", msec);
	ASRegisterObjectProperty(usercmd_s, "CVector viewangles", viewangles);
	ASRegisterObjectProperty(usercmd_s, "float forwardmove", forwardmove);
	ASRegisterObjectProperty(usercmd_s, "float sidemove", sidemove);
	ASRegisterObjectProperty(usercmd_s, "float upmove", upmove);
	ASRegisterObjectProperty(usercmd_s, "byte lightlevel", lightlevel);
	ASRegisterObjectProperty(usercmd_s, "ushort buttons", buttons);
	ASRegisterObjectProperty(usercmd_s, "byte impulse", impulse);
	ASRegisterObjectProperty(usercmd_s, "byte weaponselect", weaponselect);
	ASRegisterObjectProperty(usercmd_s, "int impact_index", impact_index);
	ASRegisterObjectProperty(usercmd_s, "CVector impact_position", impact_position);

	ASRegisterObjectType(pmplane_t);
	ASRegisterObjectProperty(pmplane_t, "CVector normal", normal);
	ASRegisterObjectProperty(pmplane_t, "float dist", dist);

	ASRegisterObjectType(pmtrace_s);
	ASRegisterObjectProperty(pmtrace_s, "int allsolid", allsolid);
	ASRegisterObjectProperty(pmtrace_s, "int startsolid", startsolid);
	ASRegisterObjectProperty(pmtrace_s, "int inopen", inopen);
	ASRegisterObjectProperty(pmtrace_s, "int inwater", inwater);
	ASRegisterObjectProperty(pmtrace_s, "float fraction", fraction);
	ASRegisterObjectProperty(pmtrace_s, "CVector endpos", endpos);
	ASRegisterObjectProperty(pmtrace_s, "pmplane_t plane", plane);
	ASRegisterObjectProperty(pmtrace_s, "int ent", ent);
	ASRegisterObjectProperty(pmtrace_s, "CVector deltavelocity", deltavelocity);
	ASRegisterObjectProperty(pmtrace_s, "int hitgroup", hitgroup);

	ASRegisterObjectType(movevars_s);
	ASRegisterObjectProperty(movevars_s, "float	gravity", gravity);
	ASRegisterObjectProperty(movevars_s, "float	stopspeed", stopspeed);
	ASRegisterObjectProperty(movevars_s, "float	maxspeed", maxspeed);
	ASRegisterObjectProperty(movevars_s, "float	spectatormaxspeed", spectatormaxspeed);
	ASRegisterObjectProperty(movevars_s, "float	accelerate", accelerate);
	ASRegisterObjectProperty(movevars_s, "float	airaccelerate", airaccelerate);
	ASRegisterObjectProperty(movevars_s, "float	wateraccelerate", wateraccelerate);
	ASRegisterObjectProperty(movevars_s, "float	friction", friction);
	ASRegisterObjectProperty(movevars_s, "float edgefriction", edgefriction);
	ASRegisterObjectProperty(movevars_s, "float	waterfriction", waterfriction);
	ASRegisterObjectProperty(movevars_s, "float	entgravity", entgravity);
	ASRegisterObjectProperty(movevars_s, "float bounce", bounce);
	ASRegisterObjectProperty(movevars_s, "float stepsize", stepsize);
	ASRegisterObjectProperty(movevars_s, "float maxvelocity", maxvelocity);
	ASRegisterObjectProperty(movevars_s, "float	zmax", zmax);
	ASRegisterObjectProperty(movevars_s, "float	waveHeight", waveHeight);
	ASRegisterObjectProperty(movevars_s, "int footsteps", footsteps);
	r = m_pScriptEngine->RegisterObjectMethod("movevars_s", "char get_skyName(int _Index) property", asFUNCTIONPR(CASMovementVariables::GetSkyNameSingleChar, (movevars_t*, int), char), asCALL_CDECL_OBJFIRST);
	ASRegisterObjectProperty(movevars_s, "float	rollangle", rollangle);
	ASRegisterObjectProperty(movevars_s, "float	rollspeed", rollspeed);
	ASRegisterObjectProperty(movevars_s, "float	skycolor_r", skycolor_r);
	ASRegisterObjectProperty(movevars_s, "float	skycolor_g", skycolor_g);
	ASRegisterObjectProperty(movevars_s, "float	skycolor_b", skycolor_b);
	ASRegisterObjectProperty(movevars_s, "float	skyvec_x", skyvec_x);
	ASRegisterObjectProperty(movevars_s, "float	skyvec_y", skyvec_y);
	ASRegisterObjectProperty(movevars_s, "float	skyvec_z", skyvec_z);

	//ASRegisterObjectType(CEngineFuncs);
	//ASRegisterObjectType(playermove_s_ext);
	ASRegisterObjectType(playermove_s);
	ASRegisterObjectProperty(playermove_s, "int player_index", player_index);
	ASRegisterObjectProperty(playermove_s, "int server", server);
	ASRegisterObjectProperty(playermove_s, "int multiplayer", multiplayer);
	ASRegisterObjectProperty(playermove_s, "float time", time);
	ASRegisterObjectProperty(playermove_s, "float frametime", frametime);
	ASRegisterObjectProperty(playermove_s, "CVector forward", forward);
	ASRegisterObjectProperty(playermove_s, "CVector right", right);
	ASRegisterObjectProperty(playermove_s, "CVector up", up);
	ASRegisterObjectProperty(playermove_s, "CVector origin", origin);
	ASRegisterObjectProperty(playermove_s, "CVector angles", angles);
	ASRegisterObjectProperty(playermove_s, "CVector oldangles", oldangles);
	ASRegisterObjectProperty(playermove_s, "CVector velocity", velocity);
	ASRegisterObjectProperty(playermove_s, "CVector movedir", movedir);
	ASRegisterObjectProperty(playermove_s, "CVector basevelocity", basevelocity);
	ASRegisterObjectProperty(playermove_s, "CVector view_ofs", view_ofs);
	ASRegisterObjectProperty(playermove_s, "float flDuckTime", flDuckTime);
	ASRegisterObjectProperty(playermove_s, "int bInDuck", bInDuck);
	ASRegisterObjectProperty(playermove_s, "int flTimeStepSound", flTimeStepSound);
	ASRegisterObjectProperty(playermove_s, "int iStepLeft", iStepLeft);
	ASRegisterObjectProperty(playermove_s, "float flFallVelocity", flFallVelocity);
	ASRegisterObjectProperty(playermove_s, "CVector punchangle", punchangle);
	ASRegisterObjectProperty(playermove_s, "float flSwimTime", flSwimTime);
	ASRegisterObjectProperty(playermove_s, "float flNextPrimaryAttack", flNextPrimaryAttack);
	ASRegisterObjectProperty(playermove_s, "int effects", effects);
	ASRegisterObjectProperty(playermove_s, "int flags", flags);
	ASRegisterObjectProperty(playermove_s, "int usehull", usehull);
	ASRegisterObjectProperty(playermove_s, "float gravity", gravity);
	ASRegisterObjectProperty(playermove_s, "float friction", friction);
	ASRegisterObjectProperty(playermove_s, "int oldbuttons", oldbuttons);
	ASRegisterObjectProperty(playermove_s, "float waterjumptime", waterjumptime);
	ASRegisterObjectProperty(playermove_s, "int dead", dead);
	ASRegisterObjectProperty(playermove_s, "int deadflag", deadflag);
	ASRegisterObjectProperty(playermove_s, "int spectator", spectator);
	ASRegisterObjectProperty(playermove_s, "int movetype", movetype);
	ASRegisterObjectProperty(playermove_s, "int onground", onground);
	ASRegisterObjectProperty(playermove_s, "int waterlevel", waterlevel);
	ASRegisterObjectProperty(playermove_s, "int watertype", watertype);
	ASRegisterObjectProperty(playermove_s, "int oldwaterlevel", oldwaterlevel);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "char get_sztexturename(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetTextureNameSingleChar, (playermove_s*, int), char), asCALL_CDECL_OBJFIRST);
	//^^^^^^ make sure this works!!!!
	ASRegisterObjectProperty(playermove_s, "char chtexturetype", chtexturetype);
	ASRegisterObjectProperty(playermove_s, "float maxspeed", maxspeed);
	ASRegisterObjectProperty(playermove_s, "float clientmaxspeed", clientmaxspeed);

	ASRegisterObjectProperty(playermove_s, "int iuser1", iuser1);
	ASRegisterObjectProperty(playermove_s, "int iuser2", iuser2);
	ASRegisterObjectProperty(playermove_s, "int iuser3", iuser3);
	ASRegisterObjectProperty(playermove_s, "int iuser4", iuser4);
	ASRegisterObjectProperty(playermove_s, "float fuser1", fuser1);
	ASRegisterObjectProperty(playermove_s, "float fuser2", fuser2);
	ASRegisterObjectProperty(playermove_s, "float fuser3", fuser3);
	ASRegisterObjectProperty(playermove_s, "float fuser4", fuser4);
	ASRegisterObjectProperty(playermove_s, "CVector vuser1", vuser1);
	ASRegisterObjectProperty(playermove_s, "CVector vuser2", vuser2);
	ASRegisterObjectProperty(playermove_s, "CVector vuser3", vuser3);
	ASRegisterObjectProperty(playermove_s, "CVector vuser4", vuser4);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "physent_t@ get_physents(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetPhysicalEntityAtIndex, (playermove_s*, int), physent_t*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "physent_t@ get_moveents(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetMovementEntityAtIndex, (playermove_s*, int), physent_t*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "physent_t@ get_visents(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetVisEntityAtIndex, (playermove_s*, int), physent_t*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "pmtrace_s@ get_touchindex(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetTouchIndexAtIndex, (playermove_s*, int), pmtrace_t*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "char get_physinfo(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetPhysInfoSingleChar, (playermove_s*, int), char), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "CVector@ get_player_mins(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetPlayerMinsAtIndex, (playermove_s*, int), CASVector*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "CVector@ get_player_maxs(int _Index) property", asFUNCTIONPR(CASPlayerMove::GetPlayerMaxsAtIndex, (playermove_s*, int), CASVector*), asCALL_CDECL_OBJFIRST);
	//^^^^^^ make sure this works!!!!

	//ANGELSCRIPT SUCKS! vvvv
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "int get_numphysent() property", asFUNCTIONPR(CASPlayerMove::GetNumPhysEnt, (playermove_s*), int), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "void set_numphysent(const int& in _Value) property", asFUNCTIONPR(CASPlayerMove::SetNumPhysEnt, (playermove_s*, int), void), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "int get_nummoveent() property", asFUNCTIONPR(CASPlayerMove::GetNumMoveEnt, (playermove_s*), int), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "void set_nummoveent(const int& in _Value) property", asFUNCTIONPR(CASPlayerMove::SetNumMoveEnt, (playermove_s*, int), void), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "int get_numvisent() property", asFUNCTIONPR(CASPlayerMove::GetNumVisEnt, (playermove_s*), int), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "void set_numvisent(const int& in _Value) property", asFUNCTIONPR(CASPlayerMove::SetNumVisEnt, (playermove_s*, int), void), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "movevars_s@ get_movevars() property", asFUNCTIONPR(CASPlayerMove::GetMovementVariables, (playermove_s*), movevars_s*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "usercmd_s@ get_cmd() property", asFUNCTIONPR(CASPlayerMove::GetUserCmd, (playermove_s*), usercmd_s*), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "int get_numtouch() property", asFUNCTIONPR(CASPlayerMove::GetNumTouch, (playermove_s*), int), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("playermove_s", "void set_numtouch(const int& in _Value) property", asFUNCTIONPR(CASPlayerMove::SetNumTouch, (playermove_s*, int), void), asCALL_CDECL_OBJFIRST);
	//ANGELSCRIPT SUCKS! ^^^^

	ASRegisterObjectType(ref_params_s);
	ASRegisterObjectProperty(ref_params_s, "CVector vieworg", vieworg);
	ASRegisterObjectProperty(ref_params_s, "CVector viewangles", viewangles);
	ASRegisterObjectProperty(ref_params_s, "CVector forward", forward);
	ASRegisterObjectProperty(ref_params_s, "CVector right", right);
	ASRegisterObjectProperty(ref_params_s, "CVector up", up);
	ASRegisterObjectProperty(ref_params_s, "float frametime", frametime);
	ASRegisterObjectProperty(ref_params_s, "float time", time);
	ASRegisterObjectProperty(ref_params_s, "int intermission", intermission);
	ASRegisterObjectProperty(ref_params_s, "int paused", paused);
	ASRegisterObjectProperty(ref_params_s, "int spectator", spectator);
	ASRegisterObjectProperty(ref_params_s, "int onground", onground);
	ASRegisterObjectProperty(ref_params_s, "int waterlevel", waterlevel);
	ASRegisterObjectProperty(ref_params_s, "CVector simvel", simvel);
	ASRegisterObjectProperty(ref_params_s, "CVector simorg", simorg);
	ASRegisterObjectProperty(ref_params_s, "CVector viewheight", viewheight);
	ASRegisterObjectProperty(ref_params_s, "float idealpitch", idealpitch);
	ASRegisterObjectProperty(ref_params_s, "CVector cl_viewangles", cl_viewangles);
	ASRegisterObjectProperty(ref_params_s, "int health", health);
	ASRegisterObjectProperty(ref_params_s, "CVector crosshairangle", crosshairangle);
	ASRegisterObjectProperty(ref_params_s, "float viewsize", viewsize);
	ASRegisterObjectProperty(ref_params_s, "CVector punchangle", punchangle);
	ASRegisterObjectProperty(ref_params_s, "int maxclients", maxclients);
	ASRegisterObjectProperty(ref_params_s, "int viewentity", viewentity);
	ASRegisterObjectProperty(ref_params_s, "int playernum", playernum);
	ASRegisterObjectProperty(ref_params_s, "int max_entities", max_entities);
	ASRegisterObjectProperty(ref_params_s, "int demoplayback", demoplayback);
	ASRegisterObjectProperty(ref_params_s, "int hardware", hardware);
	ASRegisterObjectProperty(ref_params_s, "int smoothing", smoothing);
	ASRegisterObjectProperty(ref_params_s, "usercmd_s@ cmd", cmd);
	ASRegisterObjectProperty(ref_params_s, "movevars_s@ movevars", movevars);
	r = m_pScriptEngine->RegisterObjectMethod("ref_params_s", "int get_viewport(int _Index) property", asFUNCTIONPR(CASRefParams::GetViewPortAtIndex, (ref_params_s*, int), int), asCALL_CDECL_OBJFIRST);
	ASRegisterObjectProperty(ref_params_s, "int nextView", nextView);
	ASRegisterObjectProperty(ref_params_s, "int onlyClientDraw", onlyClientDraw);

	ASRegisterObjectType(netchan_s);

	//These are null pointers when we have just loaded and so the engine won't accept that.
	//r = m_pScriptEngine->RegisterGlobalProperty("playermove_s g_PlayerMove", g_pPlayerMove);
	//r = m_pScriptEngine->RegisterGlobalProperty("movevars_s g_MoveVars", g_pMoveVars);
	//r = m_pScriptEngine->RegisterGlobalProperty("usercmd_s g_UserCmd", g_pCmd);
	//r = m_pScriptEngine->RegisterGlobalProperty("ref_params_s g_RefParams", g_pRefParams);
	r = m_pScriptEngine->RegisterGlobalProperty("double g_dblGameSpeed", g_p_fGameSpeed);
	r = m_pScriptEngine->RegisterGlobalProperty("double g_dblClientTime", g_pdblClientTime);
	r = m_pScriptEngine->RegisterGlobalProperty("int g_iLastOutgoingCmd", g_piLastOutgoingCmd);
	r = m_pScriptEngine->RegisterGlobalProperty("int g_iIncomingSequence", g_piIncomingSequence);
	r = m_pScriptEngine->RegisterGlobalProperty("int g_iOutgoingSequence", g_piOutgoingSequence);

	r = m_pScriptEngine->RegisterGlobalFunction("playermove_s@ get_g_PlayerMove() property", asFUNCTIONPR(CASGlobals::GetPlayerMove, (), playermove_s*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterGlobalFunction("ref_params_s@ get_g_RefParams() property", asFUNCTIONPR(CASGlobals::GetRefParams, (), ref_params_s*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterGlobalFunction("movevars_s@ get_g_MoveVars() property", asFUNCTIONPR(CASGlobals::GetMovevars, (), movevars_s*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterGlobalFunction("usercmd_s@ get_g_UserCmd() property", asFUNCTIONPR(CASGlobals::GetUserCmd, (), usercmd_s*), asCALL_CDECL);
}

#define COMMA ,
#define ASEventCast(_From, _To) asFUNCTION((EventCast<_From COMMA _To>))

void CTheScripts::Initialise() {
	m_pScriptEngine = asCreateScriptEngine();
	m_pEngineCommands = Q_new(CASEngineCommands)();
	m_pEngineFuncs = Q_new(CASEngineFuncs)();
	int r = m_pScriptEngine->SetMessageCallback(asFUNCTION(CTheScripts::MessageCB), 0, asCALL_CDECL);
	RegisterScriptArray(m_pScriptEngine, true);
	RegisterStdString(m_pScriptEngine);
	RegisterStdStringUtils(m_pScriptEngine);
	//RegisterStringType(m_pScriptEngine);
	RegisterScriptAny(m_pScriptEngine);
	RegisterStructures();
	if (r < 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::Initialise: SetMessageCallback returned value less than zero: %d.\n", r);
	}

	r = m_pScriptEngine->RegisterObjectType("CConsole", 0, asOBJ_REF | asOBJ_NOCOUNT);
	r = m_pScriptEngine->RegisterObjectMethod("CConsole", "void Print(const string& in _Message)", asMETHOD(CScriptConsole, Print), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterGlobalProperty("CConsole g_Console", &m_ScriptConsole);

	r = m_pScriptEngine->RegisterEnum("EEventType");
	ASRegisterEnumValue(EEventType, kUpdateEvent);
	ASRegisterEnumValue(EEventType, kTickEvent);
	ASRegisterEnumValue(EEventType, kRenderEvent);
	ASRegisterEnumValue(EEventType, kMouseMoveEvent);
	ASRegisterEnumValue(EEventType, kMouseBtnDownEvent);
	ASRegisterEnumValue(EEventType, kMouseBtnUpEvent);
	ASRegisterEnumValue(EEventType, kKeyEvent);
	ASRegisterEnumValue(EEventType, kMoveEvent);
	ASRegisterEnumValue(EEventType, kPreUpdateEvent);
	ASRegisterEnumValue(EEventType, kPreMoveEvent);
	ASRegisterEnumValue(EEventType, kPacketEvent);
	ASRegisterEnumValue(EEventType, kPrePostRunCmdEvent);
	ASRegisterEnumValue(EEventType, kPostPostRunCmdEvent);
	ASRegisterEnumValue(EEventType, kWeaponChangedEvent);
	ASRegisterEnumValue(EEventType, kCommandIssuedEvent);
	ASRegisterEnumValue(EEventType, kPostCommandIssuedEvent);
	ASRegisterEnumValue(EEventType, kNetworkUserCommandSubmitEvent);

	r = m_pScriptEngine->RegisterEnum("EKeyAction");
	ASRegisterEnumValue(EKeyAction, kDown);
	ASRegisterEnumValue(EKeyAction, kUp);

	r = m_pScriptEngine->RegisterEnum("EValueFlag");
	ASRegisterEnumValue(EValueFlag, kValueFlagNone);
	ASRegisterEnumValue(EValueFlag, kValueFlagDontRender);
	ASRegisterEnumValue(EValueFlag, kValueFlagDontSave);

	r = m_pScriptEngine->RegisterObjectType("CValue", sizeof(CValue), asOBJ_REF);
	r = m_pScriptEngine->RegisterObjectBehaviour("CValue", asBEHAVE_FACTORY, "CValue@ f(const string& in _Name, const string& in _ConvarName)", asFUNCTIONPR(CScriptValueFactory, (const std::string&, const std::string&), CScriptValue*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CValue", asBEHAVE_ADDREF, "void f()", asMETHOD(CScriptValue, AddRef), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CValue", asBEHAVE_RELEASE, "void f()", asMETHOD(CScriptValue, Release), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "bool HasFlag(const EValueFlag& in _Flag)", asMETHOD(CValue, HasFlag), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "void AddFlag(const EValueFlag& in _Flag)", asMETHOD(CValue, AddFlag), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "void TakeFlag(const EValueFlag& in _Flag)", asMETHOD(CValue, TakeFlag), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "int GetFlags()", asMETHOD(CScriptValue, GetFlags), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "CValue@ opCast() const", ASEventCast(CScriptValue*, CScriptValue*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "CValue@ opImplCast() const", ASEventCast(CScriptValue*, CScriptValue*), asCALL_CDECL_OBJLAST);
	//ASRegisterObjectProperty(CValue, "int8 m_eFlags", m_eFlags);

	r = m_pScriptEngine->RegisterObjectType("CBoolValue", sizeof(CBoolValue), asOBJ_REF);
	r = m_pScriptEngine->RegisterObjectBehaviour("CBoolValue", asBEHAVE_FACTORY, "CBoolValue@ f(const string& in _Name, const string& in _ConvarName, const bool& in _DefaultValue)", asFUNCTIONPR(CScriptBoolValueFactory, (const std::string&, const std::string&, const bool&), CScriptBoolValue*), asCALL_CDECL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CBoolValue", asBEHAVE_ADDREF, "void f()", asMETHOD(CScriptBoolValue, AddRef), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectBehaviour("CBoolValue", asBEHAVE_RELEASE, "void f()", asMETHOD(CScriptBoolValue, Release), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CValue", "CBoolValue@ opCast()", ASEventCast(CScriptValue*, CScriptBoolValue*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CBoolValue", "CValue@ opImplCast()", ASEventCast(CScriptBoolValue*, CScriptValue*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CBoolValue", "bool Get()", asMETHOD(CBoolValue, Get), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterFuncdef("void OnceBoolValueChangedCallback(any@ _UserData, bool& out _Value, const bool& in _PreviousValue)");
	r = m_pScriptEngine->RegisterObjectMethod("CBoolValue", "void RegisterOnceValueChangedCallback(OnceBoolValueChangedCallback@ _Callback, any@ _UserData = null)", asFUNCTIONPR(RegisterScriptBoolOnceChangedCallback, (CScriptBoolValue*, asIScriptFunction*, CScriptAny*), void), asCALL_CDECL_OBJFIRST);

	ASRegisterObjectType(ISimpleEvent);
	ASRegisterObjectType(ICancellableEvent);

	ASRegisterObjectType(CUpdateEvent);
	ASRegisterObjectProperty(CUpdateEvent, "usercmd_s@ m_pCmd", m_pCmd);
	ASRegisterObjectProperty(CUpdateEvent, "float m_flFrametime", m_flFrametime);
	ASRegisterObjectProperty(CUpdateEvent, "int m_iActive", m_iActive);
	ASRegisterObjectType(CRenderEvent);
	ASRegisterObjectType(CMouseMoveEvent);
	ASRegisterObjectProperty(CMouseMoveEvent, "int m_iX", m_iX);
	ASRegisterObjectProperty(CMouseMoveEvent, "int m_iY", m_iY);
	ASRegisterObjectType(CMouseBtnDownEvent);
	ASRegisterObjectProperty(CMouseBtnDownEvent, "int m_iButton", m_iButton);
	ASRegisterObjectType(CMouseBtnUpEvent);
	ASRegisterObjectProperty(CMouseBtnUpEvent, "int m_iButton", m_iButton);
	ASRegisterObjectType(CKeyEvent);
	ASRegisterObjectProperty(CKeyEvent, "EKeyAction m_eAction", m_eAction);
	ASRegisterObjectProperty(CKeyEvent, "int m_iButton", m_iButton);
	ASRegisterObjectProperty(CKeyEvent, "int m_iScancode", m_iScancode);
	ASRegisterObjectType(CMoveEvent);
	ASRegisterObjectProperty(CMoveEvent, "usercmd_s@ m_pCmd", m_pCmd);
	ASRegisterObjectProperty(CMoveEvent, "playermove_s@ m_pPlayerMove", m_pPlayerMove);
	ASRegisterObjectType(CPreUpdateEvent);
	ASRegisterObjectProperty(CPreUpdateEvent, "usercmd_s@ m_pCmd", m_pCmd);
	ASRegisterObjectProperty(CPreUpdateEvent, "float m_flFrametime", m_flFrametime);
	ASRegisterObjectType(CPreMoveEvent);
	ASRegisterObjectType(CPacketEvent);
	ASRegisterObjectProperty(CPacketEvent, "netchan_s@ m_pChan", m_pChan);
	ASRegisterObjectProperty(CPacketEvent, "int m_iDataSize", m_iDataSize);
	//TODO: CPacketEvent's m_pcData

	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "EEventType GetType()", asMETHODPR(ISimpleEvent, GetTypeAsInt, (), int), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "const EEventType GetType() const", asMETHODPR(ISimpleEvent, GetTypeAsInt, () const, int), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "ISimpleEvent@ opCast() const", ASEventCast(ISimpleEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "ISimpleEvent@ opImplCast() const", ASEventCast(ISimpleEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "ICancellableEvent@ opCast()", ASEventCast(ISimpleEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "ISimpleEvent@ opImplCast()", ASEventCast(ICancellableEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "CUpdateEvent@ opCast()", ASEventCast(ISimpleEvent*, CUpdateEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CUpdateEvent", "ISimpleEvent@ opImplCast()", ASEventCast(CUpdateEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "CRenderEvent@ opCast()", ASEventCast(ISimpleEvent*, CRenderEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CRenderEvent", "ISimpleEvent@ opImplCast()", ASEventCast(CRenderEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "CMouseMoveEvent@ opCast()", ASEventCast(ISimpleEvent*, CMouseMoveEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CMouseMoveEvent", "ISimpleEvent@ opImplCast()", ASEventCast(CMouseMoveEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "CMouseBtnDownEvent@ opCast()", ASEventCast(ICancellableEvent*, CMouseBtnDownEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CMouseBtnDownEvent", "ICancellableEvent@ opImplCast()", ASEventCast(CMouseBtnDownEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "CMouseBtnUpEvent@ opCast()", ASEventCast(ICancellableEvent*, CMouseBtnUpEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CMouseBtnUpEvent", "ICancellableEvent@ opImplCast()", ASEventCast(CMouseBtnUpEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "CKeyEvent@ opCast()", ASEventCast(ICancellableEvent*, CKeyEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CKeyEvent", "ICancellableEvent@ opImplCast()", ASEventCast(CKeyEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ISimpleEvent", "CMoveEvent@ opCast()", ASEventCast(ISimpleEvent*, CMoveEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CMoveEvent", "ISimpleEvent@ opImplCast()", ASEventCast(CMoveEvent*, ISimpleEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "CPreUpdateEvent@ opCast()", ASEventCast(ICancellableEvent*, CPreUpdateEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CPreUpdateEvent", "ICancellableEvent@ opImplCast()", ASEventCast(CPreUpdateEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "CPreMoveEvent@ opCast()", ASEventCast(ICancellableEvent*, CPreMoveEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CPreMoveEvent", "ICancellableEvent@ opImplCast()", ASEventCast(CPreMoveEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterObjectMethod("ICancellableEvent", "CPacketEvent@ opCast()", ASEventCast(ICancellableEvent*, CPacketEvent*), asCALL_CDECL_OBJLAST);
	r = m_pScriptEngine->RegisterObjectMethod("CPacketEvent", "ICancellableEvent@ opImplCast()", ASEventCast(CPacketEvent*, ICancellableEvent*), asCALL_CDECL_OBJLAST);

	r = m_pScriptEngine->RegisterFuncdef("void OnEnableCB()");
	r = m_pScriptEngine->RegisterFuncdef("void OnDisableCB()");
	r = m_pScriptEngine->RegisterFuncdef("void OnEventCB(ISimpleEvent@ _Event)");
	r = m_pScriptEngine->RegisterObjectType("CBaseScriptModule", 0, asOBJ_REF | asOBJ_NOCOUNT);
	r = m_pScriptEngine->RegisterObjectBehaviour("CBaseScriptModule", asBEHAVE_FACTORY, "CBaseScriptModule@ f(const string& in _Name, const string& in _Category, int _KeyBind, bool _ShownInArrayList)", asFUNCTION(CBaseScriptModuleFactory), asCALL_CDECL);
	/*r = m_pScriptEngine->RegisterObjectBehaviour("CBaseScriptModule", asBEHAVE_ADDREF, "void f()", asMETHOD(CBaseScriptModule, AddRef), asCALL_THISCALL);
	if (r < 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::Initialise: RegisterObjectBehaviour for CBaseScriptModule returned value less than zero: %d.\n", r);
	}
	r = m_pScriptEngine->RegisterObjectBehaviour("CBaseScriptModule", asBEHAVE_RELEASE, "void f()", asMETHOD(CBaseScriptModule, Release), asCALL_THISCALL);
	if (r < 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::Initialise: RegisterObjectBehaviour for CBaseScriptModule returned value less than zero: %d.\n", r);
	}*/
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void OnEnable()", asMETHOD(CBaseScriptModule, OnEnable), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void OnDisable()", asMETHOD(CBaseScriptModule, OnDisable), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void OnEvent(ISimpleEvent@ _Event)", asMETHODPR(CBaseScriptModule, OnEvent, (const ISimpleEvent*), void), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void SetOnEnable(OnEnableCB@ _Callback)", asMETHOD(CBaseScriptModule, SetOnEnable), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void SetOnDisable(OnDisableCB@ _Callback)", asMETHOD(CBaseScriptModule, SetOnDisable), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void SetOnEvent(OnEventCB@ _Callback)", asMETHOD(CBaseScriptModule, SetOnEvent), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "void AddValue(CValue@ _Value)", asMETHOD(CBaseScriptModule, AddValue), asCALL_THISCALL);

	r = m_pScriptEngine->RegisterObjectType("CAbstractEventListener", 0, asOBJ_REF | asOBJ_NOCOUNT);
	r = m_pScriptEngine->RegisterObjectMethod("CAbstractEventListener", "void OnEvent(ISimpleEvent@ _Event)", asMETHOD(CAbstractEventListener, OnEvent), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CBaseScriptModule", "CAbstractEventListener@ opImplCast()", asFUNCTIONPR(CBaseScriptModuleToCAbstractEventListener, (CBaseScriptModule*), CAbstractEventListener*), asCALL_CDECL_OBJFIRST);

	r = m_pScriptEngine->RegisterObjectType("CEventManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
	r = m_pScriptEngine->RegisterObjectMethod("CEventManager", "void RegisterListener(CAbstractEventListener@ _Listener)", asMETHOD(CEventManager, RegisterListener), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEventManager", "void UnregisterListener(CAbstractEventListener@ _Listener)", asMETHOD(CEventManager, UnregisterListener), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterGlobalProperty("CEventManager g_EventManager", CCheat::GetCheat()->m_pEventBus);

	r = m_pScriptEngine->RegisterObjectType("CModuleManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
	r = m_pScriptEngine->RegisterObjectMethod("CModuleManager", "void RegisterModule(CBaseScriptModule@ _Module)", asMETHOD(CModuleManager, RegisterScriptModule), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterGlobalProperty("CModuleManager g_ModuleManager", CCheat::GetCheat()->m_pModuleManager);

	ASRegisterObjectType(client_sprite_s);
	r = m_pScriptEngine->RegisterObjectMethod("client_sprite_s", "char get_szName(int _Index) property", asFUNCTION(CASClientSprite::GetNameSingleChar), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterObjectMethod("client_sprite_s", "char get_szSprite(int _Index) property", asFUNCTION(CASClientSprite::GetSpriteSingleChar), asCALL_CDECL_OBJFIRST);
	ASRegisterObjectProperty(client_sprite_s, "int hspr", hspr);
	ASRegisterObjectProperty(client_sprite_s, "int iRes", iRes);
	ASRegisterObjectProperty(client_sprite_s, "rect_s rc", rc);
	ASRegisterObjectType(SCREENINFO);
	ASRegisterObjectProperty(SCREENINFO, "int iCharHeight", iCharHeight);
	ASRegisterObjectProperty(SCREENINFO, "int iFlags", iFlags);
	ASRegisterObjectProperty(SCREENINFO, "int iHeight", iHeight);
	ASRegisterObjectProperty(SCREENINFO, "int iWidth", iWidth);
	ASRegisterObjectProperty(SCREENINFO, "int iSize", iSize);
	r = m_pScriptEngine->RegisterObjectMethod("SCREENINFO", "short get_charWidths(int _Index) property", asFUNCTION(CASScreenInfo::GetCharWidthsSingleShort), asCALL_CDECL_OBJFIRST);
	r = m_pScriptEngine->RegisterGlobalProperty("SCREENINFO g_ScreenInfo", &g_ScreenInfo);

	r = m_pScriptEngine->RegisterFuncdef("void EngineCommandCB()");
	r = m_pScriptEngine->RegisterObjectType("CEngineFuncs", 0, asOBJ_REF | asOBJ_NOCOUNT);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "HSPRITE_HL SPR_Load(const string& in _Name)", asMETHOD(CASEngineFuncs, SPR_Load), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "int SPR_Frames(HSPRITE_HL hPic)", asMETHOD(CASEngineFuncs, SPR_Frames), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "int SPR_Height(HSPRITE_HL hPic, int frame)", asMETHOD(CASEngineFuncs, SPR_Height), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "int SPR_Width(HSPRITE_HL hPic, int frame)", asMETHOD(CASEngineFuncs, SPR_Width), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void SPR_Set(HSPRITE_HL hPic, int r, int g, int b)", asMETHOD(CASEngineFuncs, SPR_Set), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void SPR_Draw(int frame, int x, int y, const rect_s& in prc)", asMETHOD(CASEngineFuncs, SPR_Draw), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void SPR_DrawHoles(int frame, int x, int y, const rect_s& in prc)", asMETHOD(CASEngineFuncs, SPR_DrawHoles), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void SPR_DrawAdditive(int frame, int x, int y, const rect_s& in prc)", asMETHOD(CASEngineFuncs, SPR_DrawAdditive), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void SPR_EnableScissor(int x, int y, int width, int height)", asMETHOD(CASEngineFuncs, SPR_EnableScissor), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void SPR_DisableScissor()", asMETHOD(CASEngineFuncs, SPR_DisableScissor), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "client_sprite_s@ SPR_GetList(const string& in psz, int& out piCount)", asMETHOD(CASEngineFuncs, SPR_GetList), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a)", asMETHOD(CASEngineFuncs, FillRGBA), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "int GetScreenInfo(SCREENINFO& out pscrinfo)", asMETHOD(CASEngineFuncs, GetScreenInfo), asCALL_THISCALL);

	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void Cmd_AddCommand(const string& in _Name, EngineCommandCB@ _Callback)", asMETHOD(CASEngineFuncs, Cmd_AddCommand), asCALL_THISCALL);
	r = m_pScriptEngine->RegisterObjectMethod("CEngineFuncs", "void Con_Printf(const string& in _Fmt)", asMETHOD(CASEngineFuncs, Con_Printf), asCALL_THISCALL);
	//TODO: ^^^^^^^^^^^^^^^ maybe add format params?
	r = m_pScriptEngine->RegisterGlobalProperty("CEngineFuncs g_EngineFuncs", m_pEngineFuncs);
}

void CTheScripts::LoadThisScriptFile(const char* _Path, const char* _ModName) {
	CScriptBuilder builder;
	int r = builder.StartNewModule(m_pScriptEngine, _ModName);
	if (r < 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::LoadThisScriptFile: Failed to start a new module with name %s: %d.\n", _ModName, r);
		return;
	}
	r = builder.AddSectionFromFile(_Path);
	if (r < 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::LoadThisScriptFile: Failed to load up this script: %s.\n", _Path);
		return;
	}
	r = builder.BuildModule();
	if (r < 0) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::LoadThisScriptFile: Failed to load up this script: %s.\n", _Path);
		return;
	}

	asIScriptModule* mod = m_pScriptEngine->GetModule(_ModName);
	asIScriptFunction* func = mod->GetFunctionByDecl("void PluginInit()");
	if (!func) {
		CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::LoadThisScriptFile: Script \"%s\" doesn't have an entry point (void PluginInit()).\n", _Path);
		return;
	}

	asIScriptContext* ctx = m_pScriptEngine->CreateContext();
	ctx->Prepare(func);
	r = ctx->Execute();
	if (r != asEXECUTION_FINISHED) {
		if (r == asEXECUTION_EXCEPTION) {
			CCheat::GetCheat()->m_pConsole->Printf("[FATAL] CTheScripts::LoadThisScriptFile: Failure executing PluginInit from script \"%s\", an exception has occurred: %s.\n", _Path, ctx->GetExceptionString());
		}
	}
	CCheat::GetCheat()->m_pConsole->Printf("[INFO] CTheScripts::LoadThisScriptFile: Successfully executed the entry point from \"%s\" script.\n", _Path);
	ctx->Release();
}