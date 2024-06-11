///
//@module LuaPluginImGui
//@author youneuoy
//@license GPL-3.0
#include "sol_ImGui.h"

namespace sol_ImGui
{
    void InitKeyEnum(sol::state& lua)
    {
        

		//@section ImGuiKey
		/***
		Enum with a list of ImGui Key.

		@tfield int None
		@tfield int Tab
		@tfield int LeftArrow
		@tfield int RightArrow
		@tfield int UpArrow
		@tfield int DownArrow
		@tfield int PageUp
		@tfield int PageDown
		@tfield int Home
		@tfield int End
		@tfield int Insert
		@tfield int Delete
		@tfield int Backspace
		@tfield int Space
		@tfield int Enter
		@tfield int Escape
		@tfield int LeftCtrl
		@tfield int LeftShift
		@tfield int LeftAlt
		@tfield int LeftSuper
		@tfield int RightCtrl
		@tfield int RightShift
		@tfield int RightAlt
		@tfield int RightSuper
		@tfield int Menu
		@tfield int _0
		@tfield int _1
		@tfield int _2
		@tfield int _3
		@tfield int _4
		@tfield int _5
		@tfield int _6
		@tfield int _7
		@tfield int _8
		@tfield int _9
		@tfield int A
		@tfield int B
		@tfield int C
		@tfield int D
		@tfield int E
		@tfield int F
		@tfield int G
		@tfield int H
		@tfield int I
		@tfield int J
		@tfield int K
		@tfield int L
		@tfield int M
		@tfield int N
		@tfield int O
		@tfield int P
		@tfield int Q
		@tfield int R
		@tfield int S
		@tfield int T
		@tfield int U
		@tfield int V
		@tfield int W
		@tfield int X
		@tfield int Y
		@tfield int Z
		@tfield int F1
		@tfield int F2
		@tfield int F3
		@tfield int F4
		@tfield int F5
		@tfield int F6
		@tfield int F7
		@tfield int F8
		@tfield int F9
		@tfield int F10
		@tfield int F11
		@tfield int F12
		@tfield int F13
		@tfield int F14
		@tfield int F15
		@tfield int F16
		@tfield int F17
		@tfield int F18
		@tfield int F19
		@tfield int F20
		@tfield int F21
		@tfield int F22
		@tfield int F23
		@tfield int F24
		@tfield int Apostrophe '
		@tfield int Comma ,
		@tfield int Minus -
		@tfield int Period .
		@tfield int Slash /
		@tfield int Semicolon ;
		@tfield int Equal =
		@tfield int LeftBracket [
		@tfield int Backslash \ (this text inhibit multiline comment caused by backslash)
		@tfield int RightBracket ]
		@tfield int GraveAccent `
		@tfield int CapsLock
		@tfield int ScrollLock
		@tfield int NumLock
		@tfield int PrintScreen
		@tfield int Pause
		@tfield int Keypad0
		@tfield int Keypad1
		@tfield int Keypad2
		@tfield int Keypad3
		@tfield int Keypad4
		@tfield int Keypad5
		@tfield int Keypad6
		@tfield int Keypad7
		@tfield int Keypad8
		@tfield int Keypad9
		@tfield int KeypadDecimal
		@tfield int KeypadDivide
		@tfield int KeypadMultiply
		@tfield int KeypadSubtract
		@tfield int KeypadAdd
		@tfield int KeypadEnter
		@tfield int KeypadEqual
		@tfield int AppBack Available on some keyboard/mouses. Often referred as "Browser Back"
		@tfield int AppForward
		@tfield int GamepadStart Menu (Xbox)      + (Switch)   Start/Options (PS)
		@tfield int GamepadBack View (Xbox)      - (Switch)   Share (PS)
		@tfield int GamepadFaceLeft X (Xbox)         Y (Switch)   Square (PS) Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
		@tfield int GamepadFaceRight B (Xbox)         A (Switch)   Circle (PS) Cancel / Close / Exit
		@tfield int GamepadFaceUp Y (Xbox)         X (Switch)   Triangle (PS) Text Input / On-screen Keyboard
		@tfield int GamepadFaceDown // A (Xbox)         B (Switch)   Cross (PS) Activate / Open / Toggle / Tweak
		@tfield int GamepadDpadLeft
		@tfield int GamepadDpadRight
		@tfield int GamepadDpadUp
		@tfield int GamepadDpadDown
		@tfield int GamepadL1
		@tfield int GamepadR1
		@tfield int GamepadL2
		@tfield int GamepadR2
		@tfield int GamepadL3
		@tfield int GamepadR3
		@tfield int GamepadLStickLeft
		@tfield int GamepadLStickRight
		@tfield int GamepadLStickUp
		@tfield int GamepadLStickDown
		@tfield int GamepadRStickLeft
		@tfield int GamepadRStickRight
		@tfield int GamepadRStickUp
		@tfield int GamepadRStickDown
		@tfield int MouseLeft
		@tfield int MouseRight
		@tfield int MouseMiddle
		@tfield int MouseX1
		@tfield int MouseX2
		@tfield int MouseWheelX
		@tfield int MouseWheelY
		@tfield int ReservedForModCtrl
		@tfield int ReservedForModShift
		@tfield int ReservedForModAlt
		@tfield int ReservedForModSuper
		@tfield int COUNT

		@table ImGuiKey
		*/
#pragma region Key
		lua.new_enum("ImGuiKey",
			"None", ImGuiKey_None,
			"Tab", ImGuiKey_Tab,
			"LeftArrow", ImGuiKey_LeftArrow,
			"RightArrow", ImGuiKey_RightArrow,
			"UpArrow", ImGuiKey_UpArrow,
			"DownArrow", ImGuiKey_DownArrow,
			"PageUp", ImGuiKey_PageUp,
			"PageDown", ImGuiKey_PageDown,
			"Home", ImGuiKey_Home,
			"End", ImGuiKey_End,
			"Insert", ImGuiKey_Insert,
			"Delete", ImGuiKey_Delete,
			"Backspace", ImGuiKey_Backspace,
			"Space", ImGuiKey_Space,
			"Enter", ImGuiKey_Enter,
			"Escape", ImGuiKey_Escape,
			"LeftCtrl", ImGuiKey_LeftCtrl,
			"LeftShift", ImGuiKey_LeftShift,
			"LeftAlt", ImGuiKey_LeftAlt,
			"LeftSuper", ImGuiKey_LeftSuper,
			"RightCtrl", ImGuiKey_RightCtrl,
			"RightShift", ImGuiKey_RightShift,
			"RightAlt", ImGuiKey_RightAlt,
			"RightSuper", ImGuiKey_RightSuper,
			"Menu", ImGuiKey_Menu,
			"_0", ImGuiKey_0,
			"_1", ImGuiKey_1,
			"_2", ImGuiKey_2,
			"_3", ImGuiKey_3,
			"_4", ImGuiKey_4,
			"_5", ImGuiKey_5,
			"_6", ImGuiKey_6,
			"_7", ImGuiKey_7,
			"_8", ImGuiKey_8,
			"_9", ImGuiKey_9,
			"A", ImGuiKey_A,
			"B", ImGuiKey_B,
			"C", ImGuiKey_C,
			"D", ImGuiKey_D,
			"E", ImGuiKey_E,
			"F", ImGuiKey_F,
			"G", ImGuiKey_G,
			"H", ImGuiKey_H,
			"I", ImGuiKey_I,
			"J", ImGuiKey_J,
			"K", ImGuiKey_K,
			"L", ImGuiKey_L,
			"M", ImGuiKey_M,
			"N", ImGuiKey_N,
			"O", ImGuiKey_O,
			"P", ImGuiKey_P,
			"Q", ImGuiKey_Q,
			"R", ImGuiKey_R,
			"S", ImGuiKey_S,
			"T", ImGuiKey_T,
			"U", ImGuiKey_U,
			"V", ImGuiKey_V,
			"W", ImGuiKey_W,
			"X", ImGuiKey_X,
			"Y", ImGuiKey_Y,
			"Z", ImGuiKey_Z,
			"F1", ImGuiKey_F1,
			"F2", ImGuiKey_F2,
			"F3", ImGuiKey_F3,
			"F4", ImGuiKey_F4,
			"F5", ImGuiKey_F5,
			"F6", ImGuiKey_F6,
			"F7", ImGuiKey_F7,
			"F8", ImGuiKey_F8,
			"F9", ImGuiKey_F9,
			"F10", ImGuiKey_F10,
			"F11", ImGuiKey_F11,
			"F12", ImGuiKey_F12,
			"F13", ImGuiKey_F13,
			"F14", ImGuiKey_F14,
			"F15", ImGuiKey_F15,
			"F16", ImGuiKey_F16,
			"F17", ImGuiKey_F17,
			"F18", ImGuiKey_F18,
			"F19", ImGuiKey_F19,
			"F20", ImGuiKey_F20,
			"F21", ImGuiKey_F21,
			"F22", ImGuiKey_F22,
			"F23", ImGuiKey_F23,
			"F24", ImGuiKey_F24,
			"Apostrophe", ImGuiKey_Apostrophe,
			"Comma", ImGuiKey_Comma,
			"Minus", ImGuiKey_Minus,
			"Period", ImGuiKey_Period,
			"Slash", ImGuiKey_Slash,
			"Semicolon", ImGuiKey_Semicolon,
			"Equal", ImGuiKey_Equal,
			"LeftBracket", ImGuiKey_LeftBracket,
			"Backslash", ImGuiKey_Backslash,
			"RightBracket", ImGuiKey_RightBracket,
			"GraveAccent", ImGuiKey_GraveAccent,
			"CapsLock", ImGuiKey_CapsLock,
			"ScrollLock", ImGuiKey_ScrollLock,
			"NumLock", ImGuiKey_NumLock,
			"PrintScreen", ImGuiKey_PrintScreen,
			"Pause", ImGuiKey_Pause,
			"Keypad0", ImGuiKey_Keypad0,
			"Keypad1", ImGuiKey_Keypad1,
			"Keypad2", ImGuiKey_Keypad2,
			"Keypad3", ImGuiKey_Keypad3,
			"Keypad4", ImGuiKey_Keypad4,
			"Keypad5", ImGuiKey_Keypad5,
			"Keypad6", ImGuiKey_Keypad6,
			"Keypad7", ImGuiKey_Keypad7,
			"Keypad8", ImGuiKey_Keypad8,
			"Keypad9", ImGuiKey_Keypad9,
			"KeypadDecimal", ImGuiKey_KeypadDecimal,
			"KeypadDivide", ImGuiKey_KeypadDivide,
			"KeypadMultiply", ImGuiKey_KeypadMultiply,
			"KeypadSubtract", ImGuiKey_KeypadSubtract,
			"KeypadAdd", ImGuiKey_KeypadAdd,
			"KeypadEnter", ImGuiKey_KeypadEnter,
			"KeypadEqual", ImGuiKey_KeypadEqual,
			"AppBack", ImGuiKey_AppBack,
			"AppForward", ImGuiKey_AppForward,
			"GamepadStart", ImGuiKey_GamepadStart,
			"GamepadBack", ImGuiKey_GamepadBack,
			"GamepadFaceLeft", ImGuiKey_GamepadFaceLeft,
			"GamepadFaceRight", ImGuiKey_GamepadFaceRight,
			"GamepadFaceUp", ImGuiKey_GamepadFaceUp,
			"GamepadFaceDown", ImGuiKey_GamepadFaceDown,
			"GamepadDpadLeft", ImGuiKey_GamepadDpadLeft,
			"GamepadDpadRight", ImGuiKey_GamepadDpadRight,
			"GamepadDpadUp", ImGuiKey_GamepadDpadUp,
			"GamepadDpadDown", ImGuiKey_GamepadDpadDown,
			"GamepadL1", ImGuiKey_GamepadL1,
			"GamepadR1", ImGuiKey_GamepadR1,
			"GamepadL2", ImGuiKey_GamepadL2,
			"GamepadR2", ImGuiKey_GamepadR2,
			"GamepadL3", ImGuiKey_GamepadL3,
			"GamepadR3", ImGuiKey_GamepadR3,
			"GamepadLStickLeft", ImGuiKey_GamepadLStickLeft,
			"GamepadLStickRight", ImGuiKey_GamepadLStickRight,
			"GamepadLStickUp", ImGuiKey_GamepadLStickUp,
			"GamepadLStickDown", ImGuiKey_GamepadLStickDown,
			"GamepadRStickLeft", ImGuiKey_GamepadRStickLeft,
			"GamepadRStickRight", ImGuiKey_GamepadRStickRight,
			"GamepadRStickUp", ImGuiKey_GamepadRStickUp,
			"GamepadRStickDown", ImGuiKey_GamepadRStickDown,
			"MouseLeft", ImGuiKey_MouseLeft,
			"MouseRight", ImGuiKey_MouseRight,
			"MouseMiddle", ImGuiKey_MouseMiddle,
			"MouseX1", ImGuiKey_MouseX1,
			"MouseX2", ImGuiKey_MouseX2,
			"MouseWheelX", ImGuiKey_MouseWheelX,
			"MouseWheelY", ImGuiKey_MouseWheelY,
			"ReservedForModCtrl", ImGuiKey_ReservedForModCtrl,
			"ReservedForModShift", ImGuiKey_ReservedForModShift,
			"ReservedForModAlt", ImGuiKey_ReservedForModAlt,
			"ReservedForModSuper", ImGuiKey_ReservedForModSuper,
			"COUNT", ImGuiKey_COUNT
		);
#pragma endregion Key
    }
}