///
//@module LuaPluginImGui
//@author youneuoy
//@license GPL-3.0
#include "sol_ImGui.h"

namespace sol_ImGui
{
    
    void InitEnums2(sol::state& lua)
    {
        		//@section ImGuiDir
		/***
		Enum with a list of ImGui Dir.

		@tfield int None
		@tfield int Left
		@tfield int Right
		@tfield int Up
		@tfield int Down
		@tfield int COUNT

		@table ImGuiDir
		*/
#pragma region Dir
		lua.new_enum("ImGuiDir",
			"None", ImGuiDir_None,
			"Left", ImGuiDir_Left,
			"Right", ImGuiDir_Right,
			"Up", ImGuiDir_Up,
			"Down", ImGuiDir_Down,
			"COUNT", ImGuiDir_COUNT
		);
#pragma endregion Dir

		//@section ImGuiSortDirection
		/***
		Enum with a list of sort directions.

		@tfield int None
		@tfield int Ascending
		@tfield int Descending

		@table ImGuiSortDirection
		*/
#pragma region SortDirection
		lua.new_enum("ImGuiSortDirection",
			"None", ImGuiDir_None,
			"Ascending", ImGuiSortDirection_Ascending,
			"Descending", ImGuiSortDirection_Descending
		);


#pragma endregion SortDirection

		//@section ImGuiCond
		/***
		Enum with a list of ImGui Conditions.

		@tfield int None No condition (always set the variable), same as _Always
		@tfield int Always No condition (always set the variable), same as _None
		@tfield int Once Set the variable once per runtime session (only the first call will succeed)
		@tfield int FirstUseEver Set the variable if the object/window has no persistently saved data (no entry in .ini file)
		@tfield int Appearing Set the variable if the object/window is appearing after being hidden/inactive (or the first time)

		@table ImGuiCond
		*/
#pragma region Cond
		lua.new_enum("ImGuiCond",
			"None", ImGuiCond_None,
			"Always", ImGuiCond_Always,
			"Once", ImGuiCond_Once,
			"FirstUseEver", ImGuiCond_FirstUseEver,
			"Appearing", ImGuiCond_Appearing
		);
#pragma endregion Cond

		//@section ImGuiCol
		/***
		Enum with a list of ImGui Col.

		@tfield int Text
		@tfield int TextDisabled
		@tfield int WindowBg Background of normal windows
		@tfield int ChildBg Background of child windows
		@tfield int PopupBg Background of popups, menus, tooltips windows
		@tfield int Border
		@tfield int BorderShadow
		@tfield int FrameBg Background of checkbox, radio button, plot, slider, text input
		@tfield int FrameBgHovered
		@tfield int FrameBgActive
		@tfield int TitleBg Title bar
		@tfield int TitleBgActive Title bar when focused
		@tfield int TitleBgCollapsed Title bar when collapsed
		@tfield int MenuBarBg
		@tfield int ScrollbarBg
		@tfield int ScrollbarGrab
		@tfield int ScrollbarGrabHovered
		@tfield int ScrollbarGrabActive
		@tfield int CheckMark Checkbox tick and RadioButton circle
		@tfield int SliderGrab
		@tfield int SliderGrabActive
		@tfield int Button
		@tfield int ButtonHovered
		@tfield int ButtonActive
		@tfield int Header Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		@tfield int HeaderHovered
		@tfield int HeaderActive
		@tfield int Separator
		@tfield int SeparatorHovered
		@tfield int SeparatorActive
		@tfield int ResizeGrip Resize grip in lower-right and lower-left corners of windows.
		@tfield int ResizeGripHovered
		@tfield int ResizeGripActive
		@tfield int Tab TabItem in a TabBar
		@tfield int TabHovered
		@tfield int TabActive
		@tfield int TabUnfocused
		@tfield int TabUnfocusedActive
		@tfield int PlotLines
		@tfield int PlotLinesHovered
		@tfield int PlotHistogram
		@tfield int PlotHistogramHovered
		@tfield int TableHeaderBg Table header background
		@tfield int TableBorderStrong Table outer and header borders (prefer using Alpha=1.0 here)
		@tfield int TableBorderLight Table inner borders (prefer using Alpha=1.0 here)
		@tfield int TableRowBg Table row background (even rows)
		@tfield int TableRowBgAlt Table row background (odd rows)
		@tfield int TextSelectedBg
		@tfield int DragDropTarget
		@tfield int NavHighlight
		@tfield int NavWindowingHighlight
		@tfield int NavWindowingDimBg
		@tfield int ModalWindowDimBg
		@tfield int ModalWindowDarkening
		@tfield int COUNT

		@table ImGuiCol
		*/
#pragma region Col
		lua.new_enum("ImGuiCol",
			"Text", ImGuiCol_Text,
			"TextDisabled", ImGuiCol_TextDisabled,
			"WindowBg", ImGuiCol_WindowBg,
			"ChildBg", ImGuiCol_ChildBg,
			"PopupBg", ImGuiCol_PopupBg,
			"Border", ImGuiCol_Border,
			"BorderShadow", ImGuiCol_BorderShadow,
			"FrameBg", ImGuiCol_FrameBg,
			"FrameBgHovered", ImGuiCol_FrameBgHovered,
			"FrameBgActive", ImGuiCol_FrameBgActive,
			"TitleBg", ImGuiCol_TitleBg,
			"TitleBgActive", ImGuiCol_TitleBgActive,
			"TitleBgCollapsed", ImGuiCol_TitleBgCollapsed,
			"MenuBarBg", ImGuiCol_MenuBarBg,
			"ScrollbarBg", ImGuiCol_ScrollbarBg,
			"ScrollbarGrab", ImGuiCol_ScrollbarGrab,
			"ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered,
			"ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive,
			"CheckMark", ImGuiCol_CheckMark,
			"SliderGrab", ImGuiCol_SliderGrab,
			"SliderGrabActive", ImGuiCol_SliderGrabActive,
			"Button", ImGuiCol_Button,
			"ButtonHovered", ImGuiCol_ButtonHovered,
			"ButtonActive", ImGuiCol_ButtonActive,
			"Header", ImGuiCol_Header,
			"HeaderHovered", ImGuiCol_HeaderHovered,
			"HeaderActive", ImGuiCol_HeaderActive,
			"Separator", ImGuiCol_Separator,
			"SeparatorHovered", ImGuiCol_SeparatorHovered,
			"SeparatorActive", ImGuiCol_SeparatorActive,
			"ResizeGrip", ImGuiCol_ResizeGrip,
			"ResizeGripHovered", ImGuiCol_ResizeGripHovered,
			"ResizeGripActive", ImGuiCol_ResizeGripActive,
			"Tab", ImGuiCol_Tab,
			"TabHovered", ImGuiCol_TabHovered,
			"TabActive", ImGuiCol_TabActive,
			"TabUnfocused", ImGuiCol_TabUnfocused,
			"TabUnfocusedActive", ImGuiCol_TabUnfocusedActive,
			"PlotLines", ImGuiCol_PlotLines,
			"PlotLinesHovered", ImGuiCol_PlotLinesHovered,
			"PlotHistogram", ImGuiCol_PlotHistogram,
			"PlotHistogramHovered", ImGuiCol_PlotHistogramHovered,
			"TableHeaderBg",ImGuiCol_TableHeaderBg,
			"TableBorderStrong",ImGuiCol_TableBorderStrong,
			"TableBorderLight",ImGuiCol_TableBorderLight,
			"TableRowBg",ImGuiCol_TableRowBg,
			"TableRowBgAlt",ImGuiCol_TableRowBgAlt,
			"TextSelectedBg", ImGuiCol_TextSelectedBg,
			"DragDropTarget", ImGuiCol_DragDropTarget,
			"NavHighlight", ImGuiCol_NavHighlight,
			"NavWindowingHighlight", ImGuiCol_NavWindowingHighlight,
			"NavWindowingDimBg", ImGuiCol_NavWindowingDimBg,
			"ModalWindowDimBg", ImGuiCol_ModalWindowDimBg,
			"ModalWindowDarkening", ImGuiCol_ModalWindowDimBg,
			"COUNT", ImGuiCol_COUNT
		);
#pragma endregion Col

		//@section ImGuiStyleVar
		/***
		Enum with a list of ImGui Style Var.

		@tfield int Alpha float
		@tfield int DisabledAlpha float
		@tfield int WindowPadding ImVec2
		@tfield int WindowRounding float
		@tfield int WindowBorderSize float
		@tfield int WindowMinSize ImVec2
		@tfield int WindowTitleAlign ImVec2
		@tfield int ChildRounding float
		@tfield int ChildBorderSize float
		@tfield int PopupRounding float
		@tfield int PopupBorderSize float
		@tfield int FramePadding ImVec2
		@tfield int FrameRounding float
		@tfield int FrameBorderSize float
		@tfield int ItemSpacing ImVec2
		@tfield int ItemInnerSpacing ImVec2
		@tfield int IndentSpacing float
		@tfield int CellPadding ImVec2
		@tfield int ScrollbarSize float
		@tfield int ScrollbarRounding float
		@tfield int GrabMinSize float
		@tfield int GrabRounding float
		@tfield int TabRounding float
		@tfield int TabBarBorderSize float
		@tfield int ButtonTextAlign ImVec2
		@tfield int SelectableTextAlign ImVec2
		@tfield int SeparatorTextBorderSize float
		@tfield int SeparatorTextAlign ImVec2
		@tfield int SeparatorTextPadding ImVec2
		@tfield int COUNT

		@table ImGuiStyleVar
		*/
#pragma region Style
		lua.new_enum("ImGuiStyleVar",
			"Alpha", ImGuiStyleVar_Alpha,
			"DisabledAlpha", ImGuiStyleVar_DisabledAlpha,
			"WindowPadding", ImGuiStyleVar_WindowPadding,
			"WindowRounding", ImGuiStyleVar_WindowRounding,
			"WindowBorderSize", ImGuiStyleVar_WindowBorderSize,
			"WindowMinSize", ImGuiStyleVar_WindowMinSize,
			"WindowTitleAlign", ImGuiStyleVar_WindowTitleAlign,
			"ChildRounding", ImGuiStyleVar_ChildRounding,
			"ChildBorderSize", ImGuiStyleVar_ChildBorderSize,
			"PopupRounding", ImGuiStyleVar_PopupRounding,
			"PopupBorderSize", ImGuiStyleVar_PopupBorderSize,
			"FramePadding", ImGuiStyleVar_FramePadding,
			"FrameRounding", ImGuiStyleVar_FrameRounding,
			"FrameBorderSize", ImGuiStyleVar_FrameBorderSize,
			"ItemSpacing", ImGuiStyleVar_ItemSpacing,
			"ItemInnerSpacing", ImGuiStyleVar_ItemInnerSpacing,
			"IndentSpacing", ImGuiStyleVar_IndentSpacing,
			"CellPadding", ImGuiStyleVar_CellPadding,
			"ScrollbarSize", ImGuiStyleVar_ScrollbarSize,
			"ScrollbarRounding", ImGuiStyleVar_ScrollbarRounding,
			"GrabMinSize", ImGuiStyleVar_GrabMinSize,
			"GrabRounding", ImGuiStyleVar_GrabRounding,
			"TabRounding", ImGuiStyleVar_TabRounding,
			"TabBarBorderSize", ImGuiStyleVar_TabBarBorderSize,
			"ButtonTextAlign", ImGuiStyleVar_ButtonTextAlign,
			"SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign,
			"SeparatorTextBorderSize", ImGuiStyleVar_SeparatorTextBorderSize,
			"SeparatorTextAlign", ImGuiStyleVar_SeparatorTextAlign,
			"SeparatorTextPadding", ImGuiStyleVar_SeparatorTextPadding,
			"COUNT", ImGuiStyleVar_COUNT
		);

		//@section ImGuiButtonFlags
		/***
		Enum with a list of ImGui Button Flags.

		@tfield int None
		@tfield int Left React on left mouse button (default)
		@tfield int Right React on right mouse button
		@tfield int Middle React on center mouse button

		@table ImGuiButtonFlags
		*/
#pragma region ImGuiButtonFlags
		lua.new_enum("ImGuiButtonFlags",
			"None", ImGuiButtonFlags_None,
			"Left", ImGuiButtonFlags_MouseButtonLeft,
			"Right", ImGuiButtonFlags_MouseButtonRight,
			"Middle", ImGuiButtonFlags_MouseButtonMiddle
		);
#pragma endregion ImGuiButtonFlags


#pragma endregion Style

		//@section ImGuiColorEditFlags
		/***
		Enum with a list of ImGui Color Edit Flags.

		@tfield int None
		@tfield int NoAlpha ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
		@tfield int NoPicker ColorEdit: disable picker when clicking on color square.
		@tfield int NoOptions ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
		@tfield int NoSmallPreview ColorEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)
		@tfield int NoInputs ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square)
		@tfield int NoTooltip ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
		@tfield int NoLabel ColorEdit, ColorPicker: disable display of text label (the label is still forwarded to the tooltip and picker).
		@tfield int NoSidePreview ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.
		@tfield int NoDragDrop ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
		@tfield int NoBorder ColorButton: disable border (which is enforced by default)
		@tfield int AlphaBar ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
		@tfield int AlphaPreview ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
		@tfield int AlphaPreviewHalf ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
		@tfield int HDR (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use ImGuiColorEditFlags_Float flag as well).
		@tfield int DisplayRGB ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
		@tfield int DisplayHSV ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
		@tfield int DisplayHex ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
		@tfield int Uint8 ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
		@tfield int Float ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
		@tfield int PickerHueBar ColorPicker: bar for Hue, rectangle for Sat/Value.
		@tfield int PickerHueWheel ColorPicker: wheel for Hue, triangle for Sat/Value.
		@tfield int InputRGB ColorEdit, ColorPicker: input and output data in RGB format.
		@tfield int InputHSV ColorEdit, ColorPicker: input and output data in HSV format.
		@tfield int DefaultOptions

		@table ImGuiColorEditFlags
		*/
#pragma region ColorEdit Flags
		lua.new_enum("ImGuiColorEditFlags",
			"None", ImGuiColorEditFlags_None,
			"NoAlpha", ImGuiColorEditFlags_NoAlpha,
			"NoPicker", ImGuiColorEditFlags_NoPicker,
			"NoOptions", ImGuiColorEditFlags_NoOptions,
			"NoSmallPreview", ImGuiColorEditFlags_NoSmallPreview,
			"NoInputs", ImGuiColorEditFlags_NoInputs,
			"NoTooltip", ImGuiColorEditFlags_NoTooltip,
			"NoLabel", ImGuiColorEditFlags_NoLabel,
			"NoSidePreview", ImGuiColorEditFlags_NoSidePreview,
			"NoDragDrop", ImGuiColorEditFlags_NoDragDrop,
			"NoBorder", ImGuiColorEditFlags_NoBorder,

			"AlphaBar", ImGuiColorEditFlags_AlphaBar,
			"AlphaPreview", ImGuiColorEditFlags_AlphaPreview,
			"AlphaPreviewHalf", ImGuiColorEditFlags_AlphaPreviewHalf,
			"HDR", ImGuiColorEditFlags_HDR,
			"DisplayRGB", ImGuiColorEditFlags_DisplayRGB,
			"DisplayHSV", ImGuiColorEditFlags_DisplayHSV,
			"DisplayHex", ImGuiColorEditFlags_DisplayHex,
			"Uint8", ImGuiColorEditFlags_Uint8,
			"Float", ImGuiColorEditFlags_Float,
			"PickerHueBar", ImGuiColorEditFlags_PickerHueBar,
			"PickerHueWheel", ImGuiColorEditFlags_PickerHueWheel,
			"InputRGB", ImGuiColorEditFlags_InputRGB,
			"InputHSV", ImGuiColorEditFlags_InputHSV,
			"DefaultOptions", ImGuiColorEditFlags_DefaultOptions_
		);
#pragma endregion ColorEdit Flags

		//@section ImGuiSliderFlags
		/***
		Enum with a list of ImGui ImGuiSliderFlags.

		@tfield int None
		@tfield int AlwaysClamp Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.
		@tfield int Logarithmic Make the widget logarithmic (linear otherwise). Consider using ImGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
		@tfield int NoRoundToFormat Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits)
		@tfield int NoInput Disable CTRL+Click or Enter key allowing to input text directly into the widget
		@tfield int InvalidMask We treat using those bits as being potentially a 'float power' argument from the previous API that has got miscast to this enum, and will trigger an assert if needed.

		@table ImGuiSliderFlags
		*/
#pragma region ImGuiSliderFlags
		lua.new_enum("ImGuiSliderFlags",
			"None", ImGuiSliderFlags_None,
			"AlwaysClamp", ImGuiSliderFlags_AlwaysClamp,
			"Logarithmic", ImGuiSliderFlags_Logarithmic,
			"NoRoundToFormat", ImGuiSliderFlags_NoRoundToFormat,
			"NoInput ", ImGuiSliderFlags_NoInput,
			"InvalidMask", ImGuiSliderFlags_InvalidMask_
		);
#pragma endregion ImGuiSliderFlags

		//@section ImGuiMouseButton
		/***
		Enum with a list of ImGui MouseButton.

		@tfield int Left
		@tfield int Right
		@tfield int Middle
		@tfield int COUNT

		@table ImGuiMouseButton
		*/
#pragma region MouseButton
		lua.new_enum("ImGuiMouseButton",
			"Left", ImGuiMouseButton_Left,
			"Right", ImGuiMouseButton_Right,
			"Middle", ImGuiMouseButton_Middle,
			"COUNT", ImGuiMouseButton_COUNT
		);
#pragma endregion MouseButton

		//@section ImGuiMod
		/***
		Enum with a list of ImGui Key Modifiers.

		@tfield int None
		@tfield int Ctrl Ctrl
		@tfield int Shift Shift
		@tfield int Alt Option/Menu
		@tfield int Super Cmd/Super/Windows
		@tfield int Shortcut Alias for Ctrl (non-macOS) _or_ Super (macOS).
		@tfield int Mask 5-bits

		@table ImGuiMod
		*/
#pragma region ImGuiMod
		lua.new_enum("ImGuiMod",
			"None", ImGuiMouseCursor_None,
			"Ctrl", ImGuiMod_Ctrl,
			"Shift", ImGuiMod_Shift,
			"Alt", ImGuiMod_Alt,
			"Super", ImGuiMod_Super,
			"Shortcut", ImGuiMod_Shortcut,
			"Mask", ImGuiMod_Mask_
		);
#pragma endregion ImGuiMod

		//@section ImGuiMouseCursor
		/***
		Enum with a list of ImGui Mouse Cursor.

		@tfield int None
		@tfield int Arrow
		@tfield int TextInput When hovering over InputText, etc.
		@tfield int ResizeAll (Unused by Dear ImGui functions)
		@tfield int ResizeNS When hovering over a horizontal border
		@tfield int ResizeEW When hovering over a vertical border or a column
		@tfield int ResizeNESW When hovering over the bottom-left corner of a window
		@tfield int ResizeNWSE When hovering over the bottom-right corner of a window
		@tfield int Hand (Unused by Dear ImGui functions. Use for e.g. hyperlinks)
		@tfield int NotAllowed When hovering something with disallowed interaction. Usually a crossed circle.
		@tfield int COUNT

		@table ImGuiMouseCursor
		*/
#pragma region MouseCursor
		lua.new_enum("ImGuiMouseCursor",
			"None", ImGuiMouseCursor_None,
			"Arrow", ImGuiMouseCursor_Arrow,
			"TextInput", ImGuiMouseCursor_TextInput, 
			"ResizeAll", ImGuiMouseCursor_ResizeAll,
			"ResizeNS", ImGuiMouseCursor_ResizeNS,
			"ResizeEW", ImGuiMouseCursor_ResizeEW,
			"ResizeNESW", ImGuiMouseCursor_ResizeNESW,
			"ResizeNWSE", ImGuiMouseCursor_ResizeNWSE,
			"Hand", ImGuiMouseCursor_Hand,
			"NotAllowed", ImGuiMouseCursor_NotAllowed,
			"COUNT", ImGuiMouseCursor_COUNT
		);
#pragma endregion MouseCursor

		//@section ImGuiTableFlags
		/***
		Enum with a list of ImGuiTableFlags.

		@tfield int None
		@tfield int Resizable Enable resizing columns.
		@tfield int Reorderable Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
		@tfield int Hideable Enable hiding/disabling columns in context menu. 
		@tfield int Sortable Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.
		@tfield int NoSavedSettings Disable persisting columns order, width and sort settings in the .ini file.
		@tfield int ContextMenuInBody Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().
		@tfield int RowBg Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)
		@tfield int BordersInnerH Draw horizontal borders between rows.
		@tfield int BordersOuterH Draw horizontal borders at the top and bottom.
		@tfield int BordersInnerV Draw vertical borders between columns.
		@tfield int BordersOuterV Draw vertical borders on the left and right sides.
		@tfield int BordersH Draw horizontal borders.
		@tfield int BordersV Draw vertical borders.
		@tfield int BordersInner Draw inner borders.
		@tfield int BordersOuter Draw outer borders.
		@tfield int Borders Draw all borders.
		@tfield int NoBordersInBody [ALPHA] Disable vertical borders in columns Body (borders will always appear in Headers). -> May move to style
		@tfield int NoBordersInBodyUntilResize [ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers)
		@tfield int SizingFixedFitColumns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.
		@tfield int SizingFixedSame Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Implicitly enable ImGuiTableFlags_NoKeepColumnsVisible.
		@tfield int SizingStretchProp Columns default to _WidthStretch with default weights proportional to each columns contents widths.
		@tfield int SizingStretchSame Columns default to _WidthStretch with default weights all equal, unless overridden by TableSetupColumn().
		@tfield int NoHostExtendX Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
		@tfield int NoHostExtendY Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.
		@tfield int NoKeepColumnsVisible Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
		@tfield int PreciseWidths Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.
		@tfield int NoClip Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with TableSetupScrollFreeze().
		@tfield int PadOuterX Default if BordersOuterV is on. Enable outermost padding. Generally desirable if you have headers.
		@tfield int NoPadOuterX Default if BordersOuterV is off. Disable outermost padding.
		@tfield int NoPadInnerX Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
		@tfield int ScrollX Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this creates a child window, ScrollY is currently generally recommended when using ScrollX.
		@tfield int ScrollY Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.
		@tfield int SortMulti Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).
		@tfield int SortTristate Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).
		@tfield int HighlightHoveredColumn Highlight column headers when hovered (may evolve into a fuller highlight)

		@table ImGuiTableFlags
		*/
#pragma region ImGuiTableFlags
		lua.new_enum("ImGuiTableFlags",
			"None", ImGuiTableFlags_None,
			"Resizable",ImGuiTableFlags_Resizable,
			"Reorderable",ImGuiTableFlags_Reorderable,
			"Hideable",ImGuiTableFlags_Hideable,
			"Sortable",ImGuiTableFlags_Sortable,
			"NoSavedSettings",ImGuiTableFlags_NoSavedSettings,
			"ContextMenuInBody",ImGuiTableFlags_ContextMenuInBody,
			"RowBg",ImGuiTableFlags_RowBg,
			"BordersInnerH",ImGuiTableFlags_BordersInnerH,
			"BordersOuterH",ImGuiTableFlags_BordersOuterH,
			"BordersInnerV",ImGuiTableFlags_BordersInnerV,
			"BordersOuterV",ImGuiTableFlags_BordersOuterV,
			"BordersH",ImGuiTableFlags_BordersH,
			"BordersOuterH",ImGuiTableFlags_BordersOuterH,
			"BordersV",ImGuiTableFlags_BordersV,
			"BordersOuterV",ImGuiTableFlags_BordersOuterV,
			"BordersInner",ImGuiTableFlags_BordersInner,
			"BordersInnerH",ImGuiTableFlags_BordersInnerH,
			"BordersOuter",ImGuiTableFlags_BordersOuter,
			"BordersOuterH",ImGuiTableFlags_BordersOuterH,
			"Borders",ImGuiTableFlags_Borders,
			"NoBordersInBody",ImGuiTableFlags_NoBordersInBody,
			"NoBordersInBodyUntilResize",ImGuiTableFlags_NoBordersInBodyUntilResize,
			"SizingFixedFit",ImGuiTableFlags_SizingFixedFit,
			"SizingFixedSame",ImGuiTableFlags_SizingFixedSame,
			"SizingStretchProp",ImGuiTableFlags_SizingStretchProp,
			"SizingStretchSame",ImGuiTableFlags_SizingStretchSame,
			"NoHostExtendX",ImGuiTableFlags_NoHostExtendX,
			"NoHostExtendY",ImGuiTableFlags_NoHostExtendY,
			"NoKeepColumnsVisible",ImGuiTableFlags_NoKeepColumnsVisible,
			"PreciseWidths",ImGuiTableFlags_PreciseWidths,
			"NoClip",ImGuiTableFlags_NoClip,
			"PadOuterX",ImGuiTableFlags_PadOuterX,
			"NoPadOuterX",ImGuiTableFlags_NoPadOuterX,
			"NoPadInnerX",ImGuiTableFlags_NoPadInnerX,
			"ScrollX",ImGuiTableFlags_ScrollX,
			"ScrollY",ImGuiTableFlags_ScrollY,
			"SortMulti",ImGuiTableFlags_SortMulti,
			"SortTristate",ImGuiTableFlags_SortTristate,
			"HighlightHoveredColumn",ImGuiTableFlags_HighlightHoveredColumn
		);
#pragma endregion ImGuiTableFlags
    }
}