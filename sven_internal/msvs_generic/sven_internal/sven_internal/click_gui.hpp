/**
 * Copyright - xWhitey, 2023.
 * click_gui.hpp - description
 */

#ifdef CLICK_GUI_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in click_gui.hpp
#else //CLICK_GUI_HPP_RECURSE_GUARD

#define CLICK_GUI_HPP_RECURSE_GUARD

#ifndef CLICK_GUI_HPP_GUARD
#define CLICK_GUI_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "custom_fonts.hpp"

struct CModuleBtnElement;
struct CPanel;

typedef union CImNotVec2f {
	struct {
		float m_flX, m_flY;
		float m_rgflPoints[2];
	};
} CImNotVec2f;

typedef struct CModuleButton {
	CModuleButton(_In_ CModule* _Module, _In_ CPanel* _Owner);

	void OnEvent(_In_ const ISimpleEvent* _Event);

	void Draw();

	inline bool IsHovered(_In_ int _MouseX, _In_ int _MouseY) {
		return _MouseX >= m_vecPosition.m_flX && _MouseX <= m_vecPosition.m_flX + m_vecSize.m_flX + 70.f && _MouseY >= m_vecPosition.m_flY && _MouseY <= m_vecPosition.m_flY + m_vecSize.m_flY;
	}

	CModule* m_pModule;
	bool m_bValuesOpen, m_bHovered, m_bListeningForKeybind;
	CImNotVec2f m_vecPosition, m_vecSize;
	CPanel* m_pOwner;
} CModuleButton;

using CModuleButton = struct CModuleButton;

bool ImGuiExtension__ColorPicker4_U32(_In_z_ const char* _Label, _In_ int _Colors[4], _In_opt_ ImGuiColorEditFlags _Flags = 0) {
	float color[4] = { ((float)_Colors[0]) / 255.f, ((float)_Colors[1]) / 255.f, ((float)_Colors[2]) / 255.f, ((float)_Colors[3]) / 255.f };

	bool result = ImGui::ColorPicker4(_Label, color, _Flags);

	_Colors[0] = (int)(color[0] * 255.f);
	_Colors[1] = (int)(color[1] * 255.f);
	_Colors[2] = (int)(color[2] * 255.f);
	_Colors[3] = (int)(color[3] * 255.f);

	return result;
}

typedef struct CCustomFontsGui {
	void Draw(_In_ bool& _Active);

	CCustomFont* m_pSelectedFont;
	bool m_bHasSelected = false;
} CCustomFontsGui;

using CCustomFontsGui = struct CCustomFontsGui;

struct CClickGUI;

typedef struct CPanel {
	CPanel(_In_ CClickGUI* _Owner, _In_z_ _Pre_z_ const char* _Title, _In_ int _X, _In_ int _Y, _In_ int _Width, _In_ int _Height, _In_opt_ bool _Open = true);
	~CPanel();

	void OnEvent(_In_ const ISimpleEvent* _Event);

	void Draw();

	inline bool IsHovered(_In_ int _MouseX, _In_ int _MouseY) {
		return _MouseX >= m_vecPosition.m_flX && _MouseX <= m_vecPosition.m_flX + m_vecSize.m_flX + 70.f && _MouseY >= m_vecPosition.m_flY && _MouseY <= m_vecPosition.m_flY + m_vecSize.m_flY;
	}

	CClickGUI* m_pOwner;
	int /* FIXME: Don't save mouseX & mouseY, use events for that. - xWhitey */ m_iMouseX, m_iMouseY;
	CImNotVec2f m_vecPosition, m_vecSize, m_vecPosition2;
	bool m_bOpen, m_bDragging;
	const char* m_pszTitle;
	CVector<CModuleButton*>* m_pVecElements;
} CPanel;

using CPanel = struct CPanel;

typedef struct CClickGUI {
	CClickGUI();
	~CClickGUI();

	void InitialisePanels();
	void OnEvent(_In_ const ISimpleEvent* _Event);

	void Draw();

	CVector<CPanel*>* m_pVecPanels;
	CCustomFontsGui* m_pCustomFontsGui;
	bool m_bHasInitialised;
} CClickGUI;

using CClickGUI = struct CClickGUI;

#else //__cplusplus
#error C++ compiler required to compile click_gui.hpp.
#endif //__cplusplus

#endif //CLICK_GUI_HPP_GUARD

#undef CLICK_GUI_HPP_RECURSE_GUARDs
#endif //CLICK_GUI_HPP_RECURSE_GUARD