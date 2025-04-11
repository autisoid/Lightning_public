#include "StdAfx.h"
#include "CStrafeModule.hpp"

Strafe::StrafeData g_StrafeData;

void CStrafeModule::UpdateStrafeData(Strafe::StrafeData& _StrafeData, bool _ReduceWishSpeed, bool _Strafe, Strafe::StrafeDir _Dir, Strafe::StrafeType _Type, float _Yaw, float _PointX, float _PointY) {
	g_pPlayerMove->velocity.CopyToArray(_StrafeData.player.Velocity);
	g_pPlayerMove->origin.CopyToArray(_StrafeData.player.Origin);

	_StrafeData.vars.OnGround = g_pPlayerMove->onground != -1;
	_StrafeData.vars.EntFriction = g_pPlayerMove->friction;
	_StrafeData.vars.Maxspeed = g_pPlayerMove->maxspeed;

	_StrafeData.vars.ReduceWishspeed = _ReduceWishSpeed ? true : (_StrafeData.vars.OnGround && (g_pPlayerMove->flags & FL_DUCKING));

	if (g_pMoveVars) {
		_StrafeData.vars.Stopspeed = g_pMoveVars->stopspeed;
		_StrafeData.vars.Friction = g_pMoveVars->friction;
		_StrafeData.vars.Accelerate = g_pMoveVars->accelerate;
		_StrafeData.vars.Airaccelerate = g_pMoveVars->airaccelerate;
	} else {
		_StrafeData.vars.Stopspeed = sv_stopspeed->value;
		_StrafeData.vars.Friction = sv_friction->value;
		_StrafeData.vars.Accelerate = sv_accelerate->value;
		_StrafeData.vars.Airaccelerate = sv_airaccelerate->value;
	}

	_StrafeData.vars.Frametime = g_pPlayerMove->frametime; // 1.0f / 200.0f (1.0f / fps_max)

	_StrafeData.frame.Strafe = _Strafe;
	_StrafeData.frame.SetDir(_Dir);
	_StrafeData.frame.SetType(_Type);

	_StrafeData.frame.SetX(_PointX);
	_StrafeData.frame.SetY(_PointY);

	_StrafeData.frame.SetYaw(static_cast<double>(_Yaw));
}

CStrafeModule::CStrafeModule() : CModule("Strafe", "Movement") {
	m_pVecValues->push_back(m_pStopWhenNoInput = Q_new(CBoolValue)("Stop when no input", "sc_strafe_stop_when_no_input", false));
	m_pVecValues->push_back(m_pWaitNTicksBeforeFlippingDirection = Q_new(CIntegerValue)("Wait N ticks before flipping direction", "sc_strafe_wait_n_ticks_before_fd", 0, 16, 0));
	m_pVecValues->push_back(m_pTargetX = Q_new(CFloatValue)("Target X", "sc_strafe_target_x", -171072.f, 171072.f, 0.f));
	m_pVecValues->push_back(m_pTargetY = Q_new(CFloatValue)("Target Y", "sc_strafe_target_y", -171072.f, 171072.f, 0.f));
	m_pVecValues->push_back(m_pTargetDirection = Q_new(CListValue)("Target direction", "sc_strafe_target_dir", "To the left\0To the right\0Best strafe\0To viewangles\0To the point\0Buttons\0\0", 3));
	m_pVecValues->push_back(m_pStrafeType = Q_new(CListValue)("Strafe type", "sc_strafe_type", "Max acceleration\0Max angle\0Max deceleration\0Constant speed\0\0", 3));
	m_pVecValues->push_back(m_pReduceWishSpeed = Q_new(CBoolValue)("Buttons mode - Reduce wish speed on left/right", "sc_strafe_reduce_wish_speed", true));
	m_pVecValues->push_back(m_pSilentViewangles = Q_new(CBoolValue)("Silent viewangles change", "sc_strafe_silent_angles", true));
	m_pVecValues->push_back(m_pPerfectSilentViewangles = Q_new(CBoolValue)("Perfect ilent viewangles change", "sc_strafe_perfect_silent_angles", true));
	m_pVecValues->push_back(m_pInstantStopOnBackwards = Q_new(CBoolValue)("Buttons mode - Instant stop on backwards", "sc_strafe_instant_stop_on_backwards", true));

	g_StrafeData.frame.UseGivenButtons = true;
	g_StrafeData.frame.buttons = Strafe::StrafeButtons();
	g_StrafeData.frame.buttons.AirLeft = Strafe::Button::LEFT;
	g_StrafeData.frame.buttons.AirRight = Strafe::Button::RIGHT;

	m_nTicksPassedSinceLastFlip = 0;

	DefineConCmd("sc_lightning_strafe", CStrafeModule);
}

void CStrafeModule::OnEnable() {
	CModule::OnEnable();
	CCheat::GetCheat()->m_pEventBus->RegisterListener(this);
}

void CStrafeModule::OnDisable() {
	CCheat::GetCheat()->m_pEventBus->UnregisterListener(this);
	CModule::OnDisable();
}

void CStrafeModule::OnEvent(_In_ const ISimpleEvent* _Event) {
	if (_Event->GetType() == EEventType::kUpdateEvent) {
		if (!g_pPlayerMove)
			return;

		const CUpdateEvent* e = static_cast<const CUpdateEvent*>(_Event);

		if ((g_pPlayerMove->flags & FL_ONGROUND) != 0)
			return;

		if (m_pTargetDirection->Get() == 5 /* Buttons */ && (e->m_pCmd->buttons & (IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT | IN_BACK)) == 0)
			return;

		if (m_pTargetDirection->Get() != 5 /* Buttons */ && (e->m_pCmd->buttons & (IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT | IN_BACK)) != 0 && !m_pStopWhenNoInput->Get())
			return;

		Strafe::StrafeDir eStrafeDir = static_cast<Strafe::StrafeDir>(m_pTargetDirection->Get());
		Strafe::StrafeType eStrafeType = static_cast<Strafe::StrafeType>(m_pStrafeType->Get());
		bool bStrafe = true;
		bool bReduceWishSpeed = false;
		Vector vecViewAngles;
		g_pEngfuncs->GetViewAngles(vecViewAngles);
		if (m_pTargetDirection->Get() == 5 /* Buttons */) {
			bStrafe = false;
			if ((e->m_pCmd->buttons & IN_MOVELEFT) != 0) {
				//eStrafeDir = Strafe::StrafeDir::LEFT;
				eStrafeDir = Strafe::StrafeDir::YAW;
				bStrafe = true;
				vecViewAngles.y += 90.f;
				bReduceWishSpeed = m_pReduceWishSpeed->Get();
			}
			if ((e->m_pCmd->buttons & IN_MOVERIGHT) != 0) {
				//eStrafeDir = Strafe::StrafeDir::RIGHT;
				eStrafeDir = Strafe::StrafeDir::YAW;
				bStrafe = true;
				vecViewAngles.y -= 90.f;
				bReduceWishSpeed = m_pReduceWishSpeed->Get();
			}
			if (((e->m_pCmd->buttons & IN_MOVELEFT) != 0 && (e->m_pCmd->buttons & IN_MOVERIGHT) != 0) || (e->m_pCmd->buttons & IN_FORWARD) != 0) {
				eStrafeDir = Strafe::StrafeDir::YAW;
				bStrafe = true;
			}
			if ((e->m_pCmd->buttons & IN_BACK) != 0) {
				eStrafeDir = Strafe::StrafeDir::YAW;
				if (m_pInstantStopOnBackwards->Get()) {
					eStrafeType = Strafe::StrafeType::MAXDECCEL;
				} else {
					vecViewAngles.y -= 180.f;
				}
				bStrafe = true;
			}
		}

		UpdateStrafeData(g_StrafeData,
			bReduceWishSpeed,
			m_bState && bStrafe,
			eStrafeDir,
			eStrafeType,
			vecViewAngles[1],
			m_pTargetX->Get(),
			m_pTargetY->Get());

		if (g_StrafeData.frame.Strafe) {
			Strafe::ProcessedFrame out;
			out.Yaw = vecViewAngles[1];

			Strafe::Friction(g_StrafeData);

			Strafe::StrafeVectorial(g_StrafeData, out, false);

			if (out.Processed) {
				if (m_nTicksPassedSinceLastFlip >= m_pWaitNTicksBeforeFlippingDirection->Get()) {
					e->m_pCmd->forwardmove = out.Forwardspeed;
					e->m_pCmd->sidemove = out.Sidespeed;

					vecViewAngles[1] = out.Yaw;
					m_nTicksPassedSinceLastFlip = 0;
				} else {
					m_nTicksPassedSinceLastFlip++;
				}

				if (m_pSilentViewangles->Get()) {
					if (m_pPerfectSilentViewangles->Get()) {
						Vector vecOldViewAngles;
						g_pEngfuncs->GetViewAngles(vecOldViewAngles);
						e->m_pCmd->viewangles = vecViewAngles;
						CUtils::SetRotation(e->m_pCmd, vecOldViewAngles);
					}
					else {
						e->m_pCmd->viewangles = vecViewAngles;
					}
				}
				else {
					g_pEngfuncs->SetViewAngles(vecViewAngles);
				}
			}
		}
	}
}