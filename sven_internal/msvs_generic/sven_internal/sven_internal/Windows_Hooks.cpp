#include "StdAfx.h"

#if !defined(__linux__) && defined(_WIN32)
#include <windowsx.h>

#include "Windows_Hooks.hpp"

#include "CClickGUIModule.hpp"
#include "CHUDModule.hpp"
#include "CApplicationSpeedModule.hpp"
#include "CClientDesignerModule.hpp"
#include "custom_fonts.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4244)
#endif //_MSC_VER

#ifdef LIGHTNING_EXTENDED_KEYMASK
#undef LIGHTNING_EXTENDED_KEYMASK
#endif //LIGHTNING_EXTENDED_KEYMASK
#define LIGHTNING_EXTENDED_KEYMASK (1 << 24)

static bool gs_bFirstRun = true;
static HGLRC g_hOriginalGLRenderCtx;
static HGLRC g_hGLRenderCtx;

void SetColors() {
	ImColor mainColor = ImColor(int(15), int(135), int(105), 255);
	ImColor bodyColor = ImColor(int(24), int(24), int(24), 255);
	ImColor fontColor = ImColor(int(255), int(255), int(255), 255);

	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
	ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
	ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
	ImVec4 frameBgColor = ImVec4(bodyColor.Value.x + 0.1f, bodyColor.Value.y + 0.1f, bodyColor.Value.z + 0.1f, bodyColor.Value.w + .1f);
	ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildRounding = 0.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(4, 3);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 8.f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 1.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.CurveTessellationTol = 0.1f;

	style.Colors[ImGuiCol_Text] = fontColor;

	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = bodyColor;
	style.Colors[ImGuiCol_ChildBg] = ImVec4(.0f, .0f, .0f, .0f);
	style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
	style.Colors[ImGuiCol_Border] = ImColor(200, 200, 200, 220);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = frameBgColor;
	style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
	style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
	style.Colors[ImGuiCol_TitleBg] = mainColor;
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = mainColor;
	style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
	style.Colors[ImGuiCol_ScrollbarGrab] = mainColor;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
	style.Colors[ImGuiCol_CheckMark] = mainColor;
	style.Colors[ImGuiCol_SliderGrab] = mainColorHovered;
	style.Colors[ImGuiCol_SliderGrabActive] = mainColorActive;
	style.Colors[ImGuiCol_Button] = mainColor;
	style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
	style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
	style.Colors[ImGuiCol_Header] = mainColor;
	style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
	style.Colors[ImGuiCol_HeaderActive] = mainColorActive;


	style.Colors[ImGuiCol_ResizeGrip] = mainColor;
	style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
	style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
	style.Colors[ImGuiCol_PlotLines] = mainColor;
	style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
	style.Colors[ImGuiCol_PlotHistogram] = mainColor;
	style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	style.WindowShadowSize = 0.f;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //we put this here so we don't have to modify imgui source
long __stdcall HOOKED_wndproc(const HWND a1, unsigned int a2, unsigned a3, long a4) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true && g_pClickGUIModule->m_pMouseRawInputConVar && g_pClickGUIModule->m_pMouseRawInputConVar->value != 0) {
		if (a2 == WM_INPUT) {
			RAWINPUT rawInput;
			UINT nRawInputStructSize = sizeof(RAWINPUT);
			if (GET_RAWINPUT_CODE_WPARAM(a3) == RIM_INPUT && GetRawInputData(reinterpret_cast<HRAWINPUT>(a4), RID_INPUT, &rawInput, &nRawInputStructSize, sizeof(rawInput.header) != -1)) {
				switch (rawInput.header.dwType) {
				case RIM_TYPEMOUSE: {
					int iMessage = -1;
					short wParam = 0;
					long lParam = 0;
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) { iMessage = WM_LBUTTONDOWN; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) { iMessage = WM_RBUTTONDOWN; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) { iMessage = WM_MBUTTONDOWN; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) { iMessage = WM_XBUTTONDOWN; wParam = XBUTTON1; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) { iMessage = WM_XBUTTONDOWN; wParam = XBUTTON2; }

					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) { iMessage = WM_LBUTTONUP; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) { iMessage = WM_RBUTTONUP; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) { iMessage = WM_MBUTTONUP; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) { iMessage = WM_XBUTTONUP; wParam = XBUTTON1; }
					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) { iMessage = WM_XBUTTONUP; wParam = XBUTTON2; }

					if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_WHEEL) { iMessage = WM_MOUSEWHEEL; wParam = static_cast<short>(rawInput.data.mouse.usButtonData); }

					if (rawInput.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
						RECT rect;
						if (rawInput.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) {
							rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
							rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
							rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
							rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
						} else {
							rect.left = 0;
							rect.top = 0;
							rect.right = GetSystemMetrics(SM_CXSCREEN);
							rect.bottom = GetSystemMetrics(SM_CYSCREEN);
						}

						short wAbsoluteX = (short)(MulDiv(rawInput.data.mouse.lLastX, rect.right, USHRT_MAX) + rect.left);
						short wAbsoluteY = (short)(MulDiv(rawInput.data.mouse.lLastY, rect.bottom, USHRT_MAX) + rect.top);

						lParam = MAKELONG(wAbsoluteX, wAbsoluteY);
						iMessage = WM_NCMOUSEMOVE;
					}
					else if (rawInput.data.mouse.lLastX != 0 || rawInput.data.mouse.lLastY != 0) {
						lParam = MAKELONG((short)rawInput.data.mouse.lLastX, (short)rawInput.data.mouse.lLastY);
						iMessage = WM_MOUSEMOVE;
					}

					SetMessageExtraInfo(0); //Well...

					if (iMessage != -1) {
						//don't send the message to imgui because HOOKED_wndproc will do that for us
						//ImGui_ImplWin32_WndProcHandler(a1, iMessage, MAKELONG(0, wParam), lParam);
						HOOKED_wndproc(a1, iMessage, MAKELONG(0, wParam), lParam);
					}
				}
					break;
				}
			}
		}
	}

	if (a2 == WM_NCMOUSEMOVE || a2 == WM_MOUSEMOVE) {
		POINT ptMousePos = { (LONG)GET_X_LPARAM(a4), (LONG)GET_Y_LPARAM(a4) };
		if (a2 == WM_NCMOUSEMOVE && !ScreenToClient(a1, &ptMousePos)) {
			//return CallWindowProcA(ORIG_wndproc, a1, a2, a3, a4);
			return TRUE;
		}

		auto e = Q_new(CMouseMoveEvent)(ptMousePos.x, ptMousePos.y);
		CCheat::GetCheat()->m_pEventBus->Post(e);
		Q_delete(e);
	}
	else if (a2 == WM_LBUTTONDOWN || a2 == WM_LBUTTONDBLCLK ||
		a2 == WM_RBUTTONDOWN || a2 == WM_RBUTTONDBLCLK ||
		a2 == WM_MBUTTONDOWN || a2 == WM_MBUTTONDBLCLK ||
		a2 == WM_XBUTTONDOWN || a2 == WM_XBUTTONDBLCLK) {
		int button = 0;
		if (a2 == WM_LBUTTONDOWN || a2 == WM_LBUTTONDBLCLK) { button = 0; }
		if (a2 == WM_RBUTTONDOWN || a2 == WM_RBUTTONDBLCLK) { button = 1; }
		if (a2 == WM_MBUTTONDOWN || a2 == WM_MBUTTONDBLCLK) { button = 2; }
		if (a2 == WM_XBUTTONDOWN || a2 == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(a3) == XBUTTON1) ? 3 : 4; }

		CMouseBtnDownEvent* e = Q_new(CMouseBtnDownEvent)(button);

		CCheat::GetCheat()->m_pEventBus->Post(e);

		if (e->m_bIsCancelled) {
			Q_delete(e);
			return TRUE;
		}

		Q_delete(e);
	}
	else if (a2 == WM_LBUTTONUP || a2 == WM_RBUTTONUP || a2 == WM_MBUTTONUP || a2 == WM_XBUTTONUP) {
		int button = 0;
		if (a2 == WM_LBUTTONUP) { button = 0; }
		if (a2 == WM_RBUTTONUP) { button = 1; }
		if (a2 == WM_MBUTTONUP) { button = 2; }
		if (a2 == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(a3) == XBUTTON1) ? 3 : 4; }

		CMouseBtnUpEvent* e = Q_new(CMouseBtnUpEvent)(button);

		CCheat::GetCheat()->m_pEventBus->Post(e);

		if (e->m_bIsCancelled) {
			Q_delete(e);
			return TRUE;
		}

		Q_delete(e);
	}
	else if (a2 == WM_KEYDOWN) {
		unsigned long new_a3 = a3;
		unsigned int scancode = (a4 & 0x00ff0000) >> 16; //TODO: check if any macros exist for this
		bool extended = (a4 & LIGHTNING_EXTENDED_KEYMASK) != 0;
		if (a3 == VK_SHIFT) new_a3 = MapVirtualKeyExA(scancode, MAPVK_VSC_TO_VK_EX, nullptr);
		if (a3 == VK_MENU) if (extended) new_a3 = VK_RMENU; //don't do this!
		if (a3 == VK_CONTROL) if (extended) new_a3 = VK_RCONTROL;
		unsigned long ulTransitionTest = MapVirtualKeyExA(scancode, MAPVK_VSC_TO_VK_EX, nullptr);
		if (ulTransitionTest != 0) new_a3 = ulTransitionTest;

		CKeyEvent* e = Q_new(CKeyEvent)(EKeyAction::kDown, new_a3, scancode);

		CCheat::GetCheat()->m_pEventBus->Post(e);

		if (e->m_bIsCancelled) {
			Q_delete(e);
			return TRUE;
		}

		Q_delete(e);

		if ((*g_piKeyDest) == LIGHTNING_REHL_CONST_KEYDEST_KEY_GAME) {
			bool bSure = true;
			// let's see if we're actually rendering stuff AND we are in game...
			if (s_nLastHudRedrawTime != 0 && s_nLastImGuiCtxRenderTime != 0) {
				if (g_bIsPlaying == false && g_pPlayerMove && g_pRefParams && g_pEngfuncs->GetLocalPlayer())
					bSure = false; //we're in console, etc...
				if (new_a3 == g_pClickGUIModule->m_nKeyBind)
					bSure = true;
			}
			if (bSure) {
				CModuleManager* lpModuleManager = CCheat::GetCheat()->m_pModuleManager;
				CVector<CModule*>::iterator beg = lpModuleManager->m_pVecModules->begin();
				for (beg; beg != lpModuleManager->m_pVecModules->end(); beg++) {
					CModule* lpModule = *beg;
					if (new_a3 == lpModule->m_nKeyBind) {
						lpModule->Toggle();
					}
				}
				beg = lpModuleManager->m_pVecScriptModules->begin();
				for (beg; beg != lpModuleManager->m_pVecScriptModules->end(); beg++) {
					CModule* lpModule = *beg;
					if (new_a3 == lpModule->m_nKeyBind) {
						lpModule->Toggle();
					}
				}
			}
		}
	}
	else if (a2 == WM_KEYUP) {
		unsigned long new_a3 = a3;
		unsigned int scancode = (a4 & 0x00ff0000) >> 16; //TODO: check if any macros exist for this
		bool extended = (a4 & LIGHTNING_EXTENDED_KEYMASK) != 0;
		if (a3 == VK_SHIFT) new_a3 = MapVirtualKeyExA(scancode, MAPVK_VSC_TO_VK_EX, nullptr);
		if (a3 == VK_MENU) if (extended) new_a3 = VK_RMENU;
		if (a3 == VK_CONTROL) if (extended) new_a3 = VK_RCONTROL;
		switch (a3) {
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
		case VK_RCONTROL: case VK_RMENU:
		case VK_LWIN: case VK_RWIN: case VK_APPS:
		case VK_PRIOR: case VK_NEXT:
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE:
		case VK_NUMLOCK:
			scancode |= KF_EXTENDED;
		}
		unsigned long ulTransitionTest = MapVirtualKeyExA(scancode, MAPVK_VSC_TO_VK_EX, nullptr);
		if (ulTransitionTest != 0) new_a3 = ulTransitionTest;

		CKeyEvent* e = Q_new(CKeyEvent)(EKeyAction::kUp, new_a3, scancode);

		CCheat::GetCheat()->m_pEventBus->Post(e);

		if (e->m_bIsCancelled) {
			Q_delete(e);
			return TRUE;
		}

		Q_delete(e);
	}
	else if (a2 == WM_QUIT || a2 == WM_DESTROY) {
		UnhookWindowsHookEx(g_pHook);
		PostQuitMessage(0);
		ExitProcess(0);
	}

	if (g_pClickGUIModule->m_bState == true) {
		ImGui_ImplWin32_WndProcHandler(a1, a2, a3, a4);

		return a2 != WM_QUIT;
	}

	return CallWindowProcA(ORIG_wndproc, a1, a2, a3, a4);
}

void Render() {
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
	if (ImGui::Begin("Lightning", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing)) {
		s_nLastImGuiCtxRenderTime = g_pTimer->m_nTimeInMilliseconds;
		auto e = Q_new(CRenderEvent)();
		CCheat::GetCheat()->m_pEventBus->Post(e);
		Q_delete(e);
		double dblTimeDifference = 0.1; //by default we think that hud isn't being drawn
		if (s_nLastHudRedrawTime != 0) {
			dblTimeDifference = TimeDifference(s_nLastHudRedrawTime, s_nLastImGuiCtxRenderTime); //scale it up with frametime so we keep up with fps_max
		}
		//if difference between our current time and last hud draw time is larger or equal to 900 millis, it means that we have paused the game or sth else has happened
		if (g_bIsPlaying == true && dblTimeDifference >= 0.09) {
			g_bIsPlaying = false;
		}

		//yes we have to draw the crosshair here because if we do that in HUD_Redraw we'll be rendering out of the gl context
		if (g_bIsPlaying == true && g_pClickGUIModule->m_bState == false && sc_lightning_crosshair &&
			sc_lightning_crosshair->value != 0.0f && (*g_piKeyDest) == LIGHTNING_REHL_CONST_KEYDEST_KEY_GAME) {
			ImVec2 vecScreenSize = ImGui::GetIO().DisplaySize;

			float r = 0.0f, g = 0.0f, b = 0.0f;
			int cScanned = sscanf_s(sc_lightning_crosshair_color->string, "%f %f %f", &r, &g, &b);
			if (cScanned == 3) {
				r = CUtils::Clamp(0.0f, 255.0f, r) / 255.f;
				g = CUtils::Clamp(0.0f, 255.0f, g) / 255.f;
				b = CUtils::Clamp(0.0f, 255.0f, b) / 255.f;

				float flAlpha = CUtils::Clamp(0.f, 255.f, sc_lightning_crosshair_alpha->value) / 255.0f;

				ImVec2 vecCenter(vecScreenSize.x / 2.0f, vecScreenSize.y / 2.0f);

				float flOutline = sc_lightning_crosshair_outline->value;
				if (flOutline > 0.0f) {
					float flSize = sc_lightning_crosshair_size->value;
					float flGap = sc_lightning_crosshair_gap->value;
					float flHalfThickness = sc_lightning_crosshair_thickness->value / 2.0f;
					float flHalfWidth = flOutline / 2.0f;
					float flOffset = flHalfWidth + flHalfThickness;

					ImColor colBlackColor(0.f, 0.f, 0.f, flAlpha);

					if (sc_lightning_crosshair_top_line->value != 0.f) {
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flOffset, vecCenter.y - flGap - flSize), ImVec2(vecCenter.x + flOffset, vecCenter.y - flGap - flSize), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flHalfThickness, vecCenter.y - flGap - flSize + flHalfWidth), ImVec2(vecCenter.x + flHalfThickness, vecCenter.y - flGap - flHalfWidth), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flOffset, vecCenter.y - flGap), ImVec2(vecCenter.x - flOffset, vecCenter.y - flGap), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flHalfThickness, vecCenter.y - flGap - flHalfWidth), ImVec2(vecCenter.x - flHalfThickness, vecCenter.y - flGap - flSize + flHalfWidth), colBlackColor, flOutline);
					}

					if (sc_lightning_crosshair_bottom_line->value != 0.f) {
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flOffset, vecCenter.y + flGap + flSize), ImVec2(vecCenter.x + flOffset, vecCenter.y + flGap + flSize), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flHalfThickness, vecCenter.y + flGap + flSize - flHalfWidth), ImVec2(vecCenter.x + flHalfThickness, vecCenter.y + flGap + flHalfWidth), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flOffset, vecCenter.y + flGap), ImVec2(vecCenter.x - flOffset, vecCenter.y + flGap), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flHalfThickness, vecCenter.y + flGap + flHalfWidth), ImVec2(vecCenter.x - flHalfThickness, vecCenter.y + flGap + flSize - flHalfWidth), colBlackColor, flOutline);
					}

					if (sc_lightning_crosshair_left_line->value != 0.f) {
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flGap - flSize, vecCenter.y - flOffset), ImVec2(vecCenter.x - flGap - flSize, vecCenter.y + flOffset), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flGap - flSize + flHalfWidth, vecCenter.y + flHalfThickness), ImVec2(vecCenter.x - flGap - flHalfWidth, vecCenter.y + flHalfThickness), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flGap, vecCenter.y + flOffset), ImVec2(vecCenter.x - flGap, vecCenter.y - flOffset), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flGap - flHalfWidth, vecCenter.y - flHalfThickness), ImVec2(vecCenter.x - flGap - flSize + flHalfWidth, vecCenter.y - flHalfThickness), colBlackColor, flOutline);
					}

					if (sc_lightning_crosshair_right_line->value != 0.f) {
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flGap + flSize, vecCenter.y - flOffset), ImVec2(vecCenter.x + flGap + flSize, vecCenter.y + flOffset), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flGap + flSize - flHalfWidth, vecCenter.y + flHalfThickness), ImVec2(vecCenter.x + flGap + flHalfWidth, vecCenter.y + flHalfThickness), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flGap, vecCenter.y + flOffset), ImVec2(vecCenter.x + flGap, vecCenter.y - flOffset), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flGap + flHalfWidth, vecCenter.y - flHalfThickness), ImVec2(vecCenter.x + flGap + flSize - flHalfWidth, vecCenter.y - flHalfThickness), colBlackColor, flOutline);
					}

					float flDotSize = sc_lightning_crosshair_dot_size->value;
					if (flDotSize > 0.0f) {
						ImVec2 vecOffset(flDotSize / 2.0f, flDotSize / 2.0f);

						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - vecOffset.x - flHalfWidth, vecCenter.y - vecOffset.y), ImVec2(vecCenter.x + vecOffset.x + flHalfWidth, vecCenter.y - vecOffset.y), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + vecOffset.x, vecCenter.y - vecOffset.y + flHalfWidth), ImVec2(vecCenter.x + vecOffset.x, vecCenter.y + vecOffset.y - flHalfWidth), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - vecOffset.x, vecCenter.y - vecOffset.y + flHalfWidth), ImVec2(vecCenter.x - vecOffset.x, vecCenter.y + vecOffset.y - flHalfWidth), colBlackColor, flOutline);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - vecOffset.x - flHalfWidth, vecCenter.y + vecOffset.y), ImVec2(vecCenter.x + vecOffset.x + flHalfWidth, vecCenter.y + vecOffset.y), colBlackColor, flOutline);
					}
				}

				float flThickness = sc_lightning_crosshair_thickness->value;
				if (flThickness > 0.0f) {
					float flSize = sc_lightning_crosshair_size->value;
					float flGap = sc_lightning_crosshair_gap->value;

					ImColor vecColour(r, g, b, flAlpha);

					if (sc_lightning_crosshair_top_line->value != 0.f)
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x, vecCenter.y - flGap - flSize), ImVec2(vecCenter.x, vecCenter.y - flGap), vecColour, flThickness);
					if (sc_lightning_crosshair_bottom_line->value != 0.f)
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x, vecCenter.y + flGap + flSize), ImVec2(vecCenter.x, vecCenter.y + flGap), vecColour, flThickness);
					if (sc_lightning_crosshair_left_line->value != 0.f)
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x - flGap - flSize, vecCenter.y), ImVec2(vecCenter.x - flGap, vecCenter.y), vecColour, flThickness);
					if (sc_lightning_crosshair_right_line->value != 0.f)
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(vecCenter.x + flGap + flSize, vecCenter.y), ImVec2(vecCenter.x + flGap, vecCenter.y), vecColour, flThickness);
				}

				float flSize = sc_lightning_crosshair_dot_size->value;
				if (flSize > 0.0f) {
					float flDotR = 0.0f, flDotG = 0.0f, flDotB = 0.0f;
					sscanf_s(sc_lightning_crosshair_dot_color->string, "%f %f %f", &flDotR, &flDotG, &flDotB);
					flDotR = CUtils::Clamp(0.0f, 255.0f, flDotR) / 255.f;
					flDotG = CUtils::Clamp(0.0f, 255.0f, flDotG) / 255.f;
					flDotB = CUtils::Clamp(0.0f, 255.0f, flDotB) / 255.f;

					ImVec2 vecOffset(flSize / 2.0f, flSize / 2.0f);

					ImGui::GetForegroundDrawList()->AddRect(ImVec2(vecCenter.x - vecOffset.x, vecCenter.y - vecOffset.y), ImVec2(vecCenter.x + vecOffset.x, vecCenter.y + vecOffset.y), ImColor(flDotR, flDotG, flDotB, flAlpha));
				}
			}
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern "C" int __stdcall HOOKED_wglSwapBuffers(_In_ HDC a1) {
	if (gs_bFirstRun) {
		gs_bFirstRun = false;
		g_bForceImGuiToNotDrawAnything = false;
		g_hWndOfGame = WindowFromDC(a1);

		PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };

		int iPixelFormat = ChoosePixelFormat(a1, &pfd);
		SetPixelFormat(a1, iPixelFormat, &pfd);

		g_hOriginalGLRenderCtx = ORIG_wglGetCurrentContext();
		g_hGLRenderCtx = ORIG_wglCreateContext(a1);

		ORIG_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(g_hWndOfGame, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HOOKED_wndproc)));

		ImGui::CreateContext();
		SetColors();
		CCustomFonts::Load();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplWin32_Init(g_hWndOfGame);

		CCheat::GetCheat()->m_pModuleManager->RegisterModules();
		if (CCheat::GetCheat()->m_pModuleManager->m_pValuesManager->m_bExistedOnLaunch == true)
			CCheat::GetCheat()->m_pModuleManager->LoadValuesFromConfigFile();
		if (CCheat::GetCheat()->m_pModuleManager->m_pConfigManager->m_bExistedOnLaunch == true)
			CCheat::GetCheat()->m_pModuleManager->EnableModulesFromConfigFile();
		if (CCheat::GetCheat()->m_pModuleManager->m_pConfigManager->m_bExistedOnLaunch == true)
			CCheat::GetCheat()->m_pModuleManager->LoadKeybindsFromConfigFile();
		if (g_pEngfuncs)
			g_pEngfuncs->pfnClientCmd(";exec lightning.cfg;\n");
		CCheat::GetCheat()->LoadUpScripts();

		return ORIG_wglSwapBuffers(a1);
	}

	if (g_pTimer) {
		g_pTimer->Process();
		//printf("wglSwapBuffers: g_pTimer->m_dblPossibleFrametime: %f\n", g_pTimer->m_dblPossibleFrametime);
	} else {
		g_pTimer = Q_new(CTimer)();
		g_pTimer->Initialise();
	}

	if (CCheat::GetCheat()->m_pEventBus)
		CCheat::GetCheat()->m_pEventBus->ProcessStalledEventsUnconditionally(); //There might be stalled events which weren't posted due to a reason, let's fire them if they aren't destructed.

	if (!g_bForceImGuiToNotDrawAnything) {
		//Let's switch to our own opengl context so we don't shit into the OG one.
		if (ORIG_wglMakeCurrent(a1, g_hGLRenderCtx)) {
			Render();
			ORIG_wglMakeCurrent(a1, g_hOriginalGLRenderCtx);
		}
	}

	return ORIG_wglSwapBuffers(a1);
}

extern "C" BOOL WINAPI HOOKED_GetMessageA(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true && _Msg->message != WM_QUIT) {
		HWND hwnd = nullptr;

		if (!_hWnd && _Msg->hwnd)
			hwnd = _Msg->hwnd;
		else if (!_Msg->hwnd && _hWnd)
			hwnd = _hWnd;
		else if (_Msg->hwnd == _hWnd)
			hwnd = _hWnd;
		else
			hwnd = _Msg->hwnd;

		if (!hwnd || hwnd != g_hWndOfGame) return ORIG_GetMessageA(_Msg, _hWnd, _MsgFilterMin, _MsgFilterMax);

		HOOKED_wndproc(hwnd, _Msg->message, _Msg->wParam, _Msg->lParam);
		memset(_Msg, 0, sizeof(MSG));

		return -1;
	}

	return ORIG_GetMessageA(_Msg, _hWnd, _MsgFilterMin, _MsgFilterMax); //_Msg->message != WM_QUIT;
}
extern "C" BOOL WINAPI HOOKED_GetMessageW(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true && _Msg->message != WM_QUIT) {
		HWND hwnd = nullptr;

		if (!_hWnd && _Msg->hwnd)
			hwnd = _Msg->hwnd;
		else if (!_Msg->hwnd && _hWnd)
			hwnd = _hWnd;
		else if (_Msg->hwnd == _hWnd)
			hwnd = _hWnd;
		else
			hwnd = _Msg->hwnd;

		if (!hwnd || hwnd != g_hWndOfGame) return ORIG_GetMessageW(_Msg, _hWnd, _MsgFilterMin, _MsgFilterMax);

		HOOKED_wndproc(hwnd, _Msg->message, _Msg->wParam, _Msg->lParam);
		memset(_Msg, 0, sizeof(MSG));

		return -1;
	}

	return ORIG_GetMessageW(_Msg, _hWnd, _MsgFilterMin, _MsgFilterMax);//_Msg->message != WM_QUIT;
}

extern "C" BOOL WINAPI HOOKED_PeekMessageA(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax, _In_ IN UINT _RemoveMsg) {
	if (!ORIG_PeekMessageA(_Msg, _hWnd, _MsgFilterMin, _MsgFilterMax, _RemoveMsg) || !_Msg)
		return FALSE;

	if (_Msg && (_RemoveMsg & PM_REMOVE) != 0 && g_pClickGUIModule && g_pClickGUIModule->m_bState == true) {
		TranslateMessage(_Msg);

		HWND hwnd = nullptr;

		if (!_hWnd && _Msg->hwnd)
			hwnd = _Msg->hwnd;
		else if (!_Msg->hwnd && _hWnd)
			hwnd = _hWnd;
		else if (_Msg->hwnd == _hWnd)
			hwnd = _hWnd;
		else
			hwnd = _Msg->hwnd;

		if (!hwnd) return FALSE;

		HOOKED_wndproc(hwnd, _Msg->message, _Msg->wParam, _Msg->lParam);

		_Msg->message = WM_NULL;
	}

	return TRUE;
}
extern "C" BOOL WINAPI HOOKED_PeekMessageW(_In_ IN LPMSG _Msg, _In_ IN HWND _hWnd, _In_ IN UINT _MsgFilterMin, _In_ IN UINT _MsgFilterMax, _In_ IN UINT _RemoveMsg) {
	if (!ORIG_PeekMessageW(_Msg, _hWnd, _MsgFilterMin, _MsgFilterMax, _RemoveMsg) || !_Msg)
		return FALSE;

	if (_Msg && (_RemoveMsg & PM_REMOVE) != 0 && g_pClickGUIModule && g_pClickGUIModule->m_bState == true) {
		TranslateMessage(_Msg);

		HWND hwnd = nullptr;

		if (!_hWnd && _Msg->hwnd)
			hwnd = _Msg->hwnd;
		else if (!_Msg->hwnd && _hWnd)
			hwnd = _hWnd;
		else if (_Msg->hwnd == _hWnd)
			hwnd = _hWnd;
		else
			hwnd = _Msg->hwnd;

		if (!hwnd) return FALSE;

		HOOKED_wndproc(hwnd, _Msg->message, _Msg->wParam, _Msg->lParam);

		_Msg->message = WM_NULL;
	}

	return TRUE;
}

extern "C" BOOL WINAPI HOOKED_PostMessageA(_In_ IN HWND _hWnd, _In_ IN unsigned int _Msg, _In_ IN WPARAM _wParam, _In_ IN LPARAM _param) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true) {
		HOOKED_wndproc(_hWnd, _Msg, _wParam, _param);

		return TRUE;
	}

	return ORIG_PostMessageA(_hWnd, _Msg, _wParam, _param);
}
extern "C" BOOL WINAPI HOOKED_PostMessageW(_In_ IN HWND _hWnd, _In_ IN unsigned int _Msg, _In_ IN WPARAM _wParam, _In_ IN LPARAM _param) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true) {
		HOOKED_wndproc(_hWnd, _Msg, _wParam, _param);

		return TRUE;
	}

	return ORIG_PostMessageW(_hWnd, _Msg, _wParam, _param);
}

extern "C" BOOL WINAPI HOOKED_ClipCursor(_Inout_ IN OUT const LPRECT * _Rect) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true)
		_Rect = nullptr;

	return ORIG_ClipCursor(_Rect);
}

extern "C" BOOL WINAPI HOOKED_SetCursorPos(_In_ IN int _X, _In_ IN int _Y) {
	g_iLastMouseX = _X;
	g_iLastMouseY = _Y;

	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true)
		return TRUE;

	return ORIG_SetCursorPos(_X, _Y);
}

extern "C" BOOL WINAPI HOOKED_GetCursorPos(_Out_ OUT LPPOINT _Point) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true) {
		if (_Point) {
			POINT pos = { g_iLastMouseX, g_iLastMouseY };
			*_Point = pos;
		}

		return TRUE;
	}

	return ORIG_GetCursorPos(_Point);
}

LRESULT CALLBACK AAL_LowLevelMouseHook(_In_ int _Code, _In_ WPARAM _wParam, _In_ LPARAM _param) {
	if (g_pClickGUIModule && g_pClickGUIModule->m_bState == true && (_wParam == WM_NCMOUSEMOVE || _wParam == WM_MOUSEMOVE)) {
		MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)_param;
		POINT sPointMousePos = p->pt;
		HWND hWnd = WindowFromPoint(sPointMousePos);
		if (hWnd != g_hWndOfGame || !ScreenToClient(hWnd, &p->pt)) return CallNextHookEx(g_pHook, _Code, _wParam, _param);

		HOOKED_wndproc(hWnd, _wParam, 0, MAKELONG((short)sPointMousePos.x, (short)sPointMousePos.y));
	}

	return CallNextHookEx(g_pHook, _Code, _wParam, _param);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER

#endif //not defined(__linux__) and defined(_WIN32)