#include <algorithm>
#include <cassert>
#include <cmath>
#include <optional>

#include "hlstrafe.hpp"
#include "util.hpp"

namespace HLStrafe
{
	PositionType GetPositionType(PlayerData& player, TraceFunc traceFunc, PointContentsFunc pointContentsFunc, unsigned version)
	{
		float point[3];

		if (version > 4) {
			// Check water. If we're under water, return here.
			const int CONTENTS_WATER = -3;
			const int CONTENTS_TRANSLUCENT = -15;

			VecCopy<float, 3>(player.Origin, point);

			// Pick a spot just above the players feet.
			point[0] += (player.Ducking ? (VEC_DUCK_HULL_MIN[0] + VEC_DUCK_HULL_MAX[0]) : (VEC_HULL_MIN[0] + VEC_HULL_MAX[0])) * 0.5f;
			point[1] += (player.Ducking ? (VEC_DUCK_HULL_MIN[1] + VEC_DUCK_HULL_MAX[1]) : (VEC_HULL_MIN[1] + VEC_HULL_MAX[1])) * 0.5f;
			point[2] += ((player.Ducking ? VEC_DUCK_HULL_MIN[2] : VEC_HULL_MIN[2]) + 1);

			int contents = pointContentsFunc(point);

			if (contents <= CONTENTS_WATER && contents > CONTENTS_TRANSLUCENT) {
				// Now check a point that is at the player hull midpoint.
				point[2] = player.Origin[2] + (player.Ducking ? (VEC_DUCK_HULL_MIN[2] + VEC_DUCK_HULL_MAX[2]) : (VEC_HULL_MIN[2] + VEC_HULL_MAX[2])) * 0.5f;
				contents = pointContentsFunc(point);
				// If that point is also under water...
				if (contents <= CONTENTS_WATER && contents > CONTENTS_TRANSLUCENT)
					return PositionType::WATER;
			}
		}

		// Check ground.
		if (player.Velocity[2] > 180)
			return PositionType::AIR;

		VecCopy<float, 3>(player.Origin, point);
		point[2] -= 2;

		auto tr = traceFunc(player.Origin, point, (player.Ducking) ? HullType::DUCKED : HullType::NORMAL);
		if (tr.PlaneNormal[2] < 0.7 || tr.Entity == -1)
			return PositionType::AIR;

		if (!tr.StartSolid && !tr.AllSolid)
			VecCopy<float, 3>(tr.EndPos, player.Origin);
		return PositionType::GROUND;
	}

	void VectorFME(PlayerData& player, const MovementVars& vars, PositionType postype, double wishspeed, const double a[2])
	{
		assert(postype != PositionType::WATER);

		bool onground = (postype == PositionType::GROUND);
		double wishspeed_capped = onground ? wishspeed : 30;
		double tmp = wishspeed_capped - DotProduct<float, double, 2>(player.Velocity, a);
		if (tmp <= 0.0)
			return;

		double accel = onground ? vars.Accelerate : vars.Airaccelerate;
		double accelspeed = accel * wishspeed * vars.EntFriction * vars.Frametime;
		if (accelspeed <= tmp)
			tmp = accelspeed;

		player.Velocity[0] += static_cast<float>(a[0] * tmp);
		player.Velocity[1] += static_cast<float>(a[1] * tmp);
	}

	void CheckVelocity(PlayerData& player, const MovementVars& vars)
	{
		for (std::size_t i = 0; i < 3; ++i) {
			if (player.Velocity[i] > vars.Maxvelocity)
				player.Velocity[i] = vars.Maxvelocity;
			if (player.Velocity[i] < -vars.Maxvelocity)
				player.Velocity[i] = -vars.Maxvelocity;
		}
	}

	PositionType Move(PlayerData& player, const MovementVars& vars, PositionType postype, double wishspeed, TraceFunc traceFunc, PointContentsFunc pointContentsFunc, unsigned version, bool calcVelocity, const double a[2], float fractions[4], float normalzs[4])
	{
		assert(postype != PositionType::WATER);

		bool onground = (postype == PositionType::GROUND);
		CheckVelocity(player, vars);

		// AddCorrectGravity
		float entGravity = vars.EntGravity;
		if (entGravity == 0.0f)
			entGravity = 1.0f;
		player.Velocity[2] -= static_cast<float>(entGravity * vars.Gravity * 0.5 * vars.Frametime);
		player.Velocity[2] += player.Basevelocity[2] * vars.Frametime;
		player.Basevelocity[2] = 0;
		CheckVelocity(player, vars);

		// Move
		wishspeed = std::min(wishspeed, 500.0);
		if (onground)
			player.Velocity[2] = 0;

		// Accelerate
		if (calcVelocity && a)
			VectorFME(player, vars, postype, wishspeed, a);

		// Move
		VecAdd<float, float, 3>(player.Velocity, player.Basevelocity, player.Velocity);
		if (onground) {
			// WalkMove
			auto spd = Length<float, 3>(player.Velocity);
			if (spd < 1) {
				VecScale<float, 3>(player.Velocity, 0, player.Velocity); // Clear velocity.
			} else {
				float dest[3];
				VecCopy<float, 3>(player.Origin, dest);
				dest[0] += player.Velocity[0] * vars.Frametime;
				dest[1] += player.Velocity[1] * vars.Frametime;

				auto tr = traceFunc(player.Origin, dest, (player.Ducking) ? HullType::DUCKED : HullType::NORMAL);
				if (tr.Fraction == 1.0f) {
					VecCopy<float, 3>(tr.EndPos, player.Origin);
					if (fractions)
						fractions[0] = 1;
				} else {
					// Figure out the end position when trying to walk up a step.
					VecCopy<float, 3>(player.Origin, dest);

					auto playerUp = PlayerData(player);
					dest[2] += vars.Stepsize;
					tr = traceFunc(playerUp.Origin, dest, (player.Ducking) ? HullType::DUCKED : HullType::NORMAL);
					if (!tr.StartSolid && !tr.AllSolid)
						VecCopy<float, 3>(tr.EndPos, playerUp.Origin);

					float fractions_up[4] = {0, 0, 0, 0}, normalzs_up[4] = {0, 0, 0, 0};
					FlyMove(playerUp, vars, postype, traceFunc, fractions_up, normalzs_up);
					VecCopy<float, 3>(playerUp.Origin, dest);
					dest[2] -= vars.Stepsize;

					tr = traceFunc(playerUp.Origin, dest, (player.Ducking) ? HullType::DUCKED : HullType::NORMAL);
					if (!tr.StartSolid && !tr.AllSolid)
						VecCopy<float, 3>(tr.EndPos, playerUp.Origin);

					// Figure out the end position when _not_ trying to walk up a step.
					auto playerDown = PlayerData(player);
					FlyMove(playerDown, vars, postype, traceFunc, fractions, normalzs);

					// Take whichever move was the furthest.
					auto downdist = (playerDown.Origin[0] - player.Origin[0]) * (playerDown.Origin[0] - player.Origin[0])
						+ (playerDown.Origin[1] - player.Origin[1]) * (playerDown.Origin[1] - player.Origin[1]);
					auto updist = (playerUp.Origin[0] - player.Origin[0]) * (playerUp.Origin[0] - player.Origin[0])
						+ (playerUp.Origin[1] - player.Origin[1]) * (playerUp.Origin[1] - player.Origin[1]);

					if ((tr.PlaneNormal[2] < 0.7) || (downdist > updist)) {
						VecCopy<float, 3>(playerDown.Origin, player.Origin);
						VecCopy<float, 3>(playerDown.Velocity, player.Velocity);
					} else {
						VecCopy<float, 3>(playerUp.Origin, player.Origin);
						VecCopy<float, 2>(playerUp.Velocity, player.Velocity);
						player.Velocity[2] = playerDown.Velocity[2];
						if (fractions)
							VecCopy<float, 4>(fractions_up, fractions);
						if (normalzs)
							VecCopy<float, 4>(normalzs_up, normalzs);
					}
				}
			}
		} else {
			// AirMove
			FlyMove(player, vars, postype, traceFunc, fractions, normalzs);
		}

		postype = GetPositionType(player, traceFunc, pointContentsFunc, version);
		VecSubtract<float, float, 3>(player.Velocity, player.Basevelocity, player.Velocity);
		CheckVelocity(player, vars);
		if (postype != PositionType::GROUND && postype != PositionType::WATER) {
			// FixupGravityVelocity
			player.Velocity[2] -= static_cast<float>(entGravity * vars.Gravity * 0.5 * vars.Frametime);
			CheckVelocity(player, vars);
		}

		if (postype == PositionType::GROUND)
			player.Velocity[2] = 0;

		return postype;
	}

	void FlyMove(PlayerData& player, const MovementVars& vars, PositionType postype, TraceFunc traceFunc, float fractions[4], float normalzs[4])
	{
		const auto MAX_BUMPS = 4;
		const auto MAX_CLIP_PLANES = 5;

		float originalVelocity[3], savedVelocity[3];
		VecCopy<float, 3>(player.Velocity, originalVelocity);
		VecCopy<float, 3>(player.Velocity, savedVelocity);

		auto timeLeft = vars.Frametime;
		auto allFraction = 0.0f;
		auto numPlanes = 0;
		auto blockedState = 0;
		float planes[MAX_CLIP_PLANES][3];

		for (auto bumpCount = 0; bumpCount < MAX_BUMPS; ++bumpCount) {
			if (IsZero<float, 3>(player.Velocity))
				break;

			float end[3];
			for (size_t i = 0; i < 3; ++i)
				end[i] = player.Origin[i] + timeLeft * player.Velocity[i];

			auto tr = traceFunc(player.Origin, end, (player.Ducking) ? HullType::DUCKED : HullType::NORMAL);
			if (fractions)
				fractions[bumpCount] = tr.Fraction;
			if (normalzs)
				normalzs[bumpCount] = tr.PlaneNormal[2];

			allFraction += tr.Fraction;
			if (tr.AllSolid) {
				VecScale<float, 3>(player.Velocity, 0, player.Velocity);
				blockedState = 4;
				break;
			}
			if (tr.Fraction > 0) {
				VecCopy<float, 3>(tr.EndPos, player.Origin);
				VecCopy<float, 3>(player.Velocity, savedVelocity);
				numPlanes = 0;
			}
			if (tr.Fraction == 1)
				break;

			if (tr.PlaneNormal[2] > 0.7)
				blockedState |= 1;
			else if (tr.PlaneNormal[2] == 0)
				blockedState |= 2;

			timeLeft -= timeLeft * tr.Fraction;

			if (numPlanes >= MAX_CLIP_PLANES) {
				VecScale<float, 3>(player.Velocity, 0, player.Velocity);
				break;
			}

			VecCopy<float, 3>(tr.PlaneNormal, planes[numPlanes]);
			numPlanes++;

			if (postype != PositionType::GROUND || vars.EntFriction != 1) {
				for (auto i = 0; i < numPlanes; ++i)
					if (planes[i][2] > 0.7)
						ClipVelocity(savedVelocity, planes[i], 1);
					else
						ClipVelocity(savedVelocity, planes[i], static_cast<float>(1.0 + vars.Bounce * (1 - vars.EntFriction)));

				VecCopy<float, 3>(savedVelocity, player.Velocity);
			} else {
				int i = 0;
				for (i = 0; i < numPlanes; ++i) {
					VecCopy<float, 3>(savedVelocity, player.Velocity);
					ClipVelocity(player.Velocity, planes[i], 1);

					int j;
					for (j = 0; j < numPlanes; ++j)
						if (j != i)
							if (DotProduct<float, float, 3>(player.Velocity, planes[j]) < 0)
								break;

					if (j == numPlanes)
						break;
				}

				if (i == numPlanes) {
					if (numPlanes != 2) {
						VecScale<float, 3>(player.Velocity, 0, player.Velocity);
						break;
					}

					float dir[3];
					CrossProduct<float, float>(planes[0], planes[1], dir);
					auto d = static_cast<float>(DotProduct<float, float, 3>(dir, player.Velocity));
					VecScale<float, 3>(dir, d, player.Velocity);
				}

				if (DotProduct<float, float, 3>(player.Velocity, originalVelocity) <= 0) {
					VecScale<float, 3>(player.Velocity, 0, player.Velocity);
					break;
				}
			}
		}

		if (allFraction == 0)
			VecScale<float, 3>(player.Velocity, 0, player.Velocity);
	}

	int ClipVelocity(float velocty[3], const float normal[3], float overbounce)
	{
		const auto STOP_EPSILON = 0.1;

		auto backoff = static_cast<float>(DotProduct<float, float, 3>(velocty, normal) * overbounce);

		for (size_t i = 0; i < 3; ++i) {
			auto change = normal[i] * backoff;
			velocty[i] -= change;

			if (velocty[i] > -STOP_EPSILON && velocty[i] < STOP_EPSILON)
				velocty[i] = 0;
		}

		if (normal[2] > 0)
			return 1;
		else if (normal[2] == 0)
			return 2;
		else
			return 0;
	}
}
