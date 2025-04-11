#include "../hlsdk_mini.hpp"
#include "../globals.hpp"
#include "hlstrafe.hpp"

namespace HLStrafe {
	TraceResult TraceFunction(float a[3], float b[3], HLStrafe::HullType c) {
		HLStrafe::TraceResult result{};
		pmtrace_t tr;
		g_pEngfuncs->pEventAPI->EV_SetTraceHull(HLStrafe::HullTypeToInt(c));
		g_pEngfuncs->pEventAPI->EV_PlayerTrace(const_cast<float*>(a), const_cast<float*>(b), PM_NORMAL, g_pEngfuncs->GetLocalPlayer()->index, &tr);
		result.AllSolid = tr.allsolid;
		result.StartSolid = tr.startsolid;
		result.Fraction = tr.fraction;
		tr.endpos.CopyToArray(result.EndPos);
		tr.plane.normal.CopyToArray(result.PlaneNormal);

		return result;
	}
	int PointContentsFunction(float a[3]) {
		return g_pEngfuncs->PM_PointContents(const_cast<float*>(a), nullptr);
	}
}