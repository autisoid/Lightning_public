/**
 * Copyright - YaLTeR ~ xWhitey, 20??.
 * [hlstrafe.hpp] - Our custom mini-version of HLStrafe that doesn't use HLTAS.
 */

#ifdef HLSTRAFE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in hlstrafe.hpp
#else //HLSTRAFE_HPP_RECURSE_GUARD

#define HLSTRAFE_HPP_RECURSE_GUARD

#ifndef HLSTRAFE_HPP_GUARD
#define HLSTRAFE_HPP_GUARD
#pragma once

namespace HLStrafe
{
	const unsigned MAX_SUPPORTED_VERSION = 5;

	struct PlayerData {
		float Origin[3];
		float Velocity[3];
		float Basevelocity[3];
		float Viewangles[3];

		bool Ducking;
		bool InDuckAnimation;
		float DuckTime;
		float StaminaTime;
		bool Walking;
	};

	struct MovementVars {
		float Maxvelocity;
		float Maxspeed;
		float Frametime;
		float Friction;
		float Edgefriction;
		float EntFriction; // Aka pmove->friction.
		float Accelerate;
		float Airaccelerate;
		float Gravity;
		float EntGravity; // Aka pmove->gravity.
		float Stepsize;
		float Bounce;
	};

	struct TraceResult {
		bool AllSolid;
		bool StartSolid;
		float Fraction;
		float EndPos[3];
		float PlaneNormal[3];
		int Entity;
	};

	// On ground, in the air or underwater.
	enum class PositionType {
		GROUND = 0,
		AIR,
		WATER
	};

	enum class HullType : int {
		NORMAL = 0,
		DUCKED = 1,
		POINT = 2
	};

	inline int HullTypeToInt(HullType hull) {
		switch (hull) {
		case HullType::NORMAL: {
			return 0;
		}
			break;
		case HullType::DUCKED: {
			return 1;
		}
			break;
		case HullType::POINT: {
			return 2;
		}
			break;
		}
		return -1;
	}

	typedef TraceResult(*TraceFunc)(float[3], float[3], HLStrafe::HullType);
	typedef int(*PointContentsFunc)(float _Position[3]);

	/*
		Figures out the player's position type and, if necessary, updates player.Origin and curState.
	*/
	PositionType GetPositionType(PlayerData& player, TraceFunc traceFunc, PointContentsFunc pointContentsFunc, unsigned version);

	/*
		Limits the velocity components to maxvelocity.
	*/
	void CheckVelocity(PlayerData& player, const MovementVars& vars);

	/*
		Computes the new velocity given unit acceleration vector and wishspeed
		and stores the result in player.Velocity. Postype != WATER.

		Struct requirements:
			Velocity;
			Frametime, Accelerate or Airaccelerate, EntFriction.
	*/
	void VectorFME(PlayerData& player, const MovementVars& vars, PositionType postype, double wishspeed, const double a[2]);

	/*
		Computes the new velocity given unit acceleration vector and wishspeed,
		stores the result in player.Velocity, computes the new position taking the world into consideration
		and stores the result in player.Origin. Postype != WATER.

		Struct requirements:
		Velocity, Basevelocity, Origin;
		Frametime, Accelerate or Airaccelerate, EntFriction, EntGravity, Gravity.
	*/
	PositionType Move(PlayerData& player, const MovementVars& vars, PositionType postype, double wishspeed, TraceFunc traceFunc, PointContentsFunc pointContentsFunc, unsigned version, bool calcVelocity = false, const double a[2] = nullptr, float fractions[4] = nullptr, float normalzs[4] = nullptr);

	/*
		Helpers for the movement prediction, do exactly what PM_FlyMove and PM_ClipVelocity do.
	*/
	void FlyMove(PlayerData& player, const MovementVars& vars, PositionType postype, TraceFunc traceFunc, float fractions[4] = nullptr, float normalzs[4] = nullptr);
	int ClipVelocity(float velocity[3], const float normal[3], float overbounce);

	/*
		Wrappers around engine functions
	*/

	TraceResult TraceFunction(float a[3], float b[3], HLStrafe::HullType c);
	int PointContentsFunction(float a[3]);
}

#endif //HLSTRAFE_HPP_GUARD

#undef HLSTRAFE_HPP_RECURSE_GUARD
#endif //HLSTRAFE_HPP_RECURSE_GUARD