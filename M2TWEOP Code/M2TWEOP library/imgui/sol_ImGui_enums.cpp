///
//@module LuaPluginImGui
//@author youneuoy
//@license GPL-3.0
#include "sol_ImGui.h"

namespace sol_ImGui
{
	void InitEnums(sol::state& lua)
	{
#pragma region Window Flags
		//@section ImGuiWindowFlags
		/***
		Enum with a list of ImGui Window Flags.

		@tfield int None
		@tfield int NoTitleBar Disable title-bar
		@tfield int NoResize Disable user resizing with the lower-right grip
		@tfield int NoMove Disable user moving the window
		@tfield int NoScrollbar Disable scrollbars (window can still scroll with mouse or programmatically)
		@tfield int NoScrollWithMouse Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
		@tfield int NoCollapse Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
		@tfield int AlwaysAutoResize Resize every window to its content every frame
		@tfield int NoBackground Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
		@tfield int NoSavedSettings Never load/save settings in .ini file
		@tfield int NoMouseInputs Disable catching mouse, hovering test with pass through.
		@tfield int MenuBar Has a menu-bar
		@tfield int HorizontalScrollbar Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
		@tfield int NoFocusOnAppearing Disable taking focus when transitioning from hidden to visible state
		@tfield int NoBringToFrontOnFocus Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
		@tfield int AlwaysVerticalScrollbar Always show vertical scrollbar (even if ContentSize.y < Size.y)
		@tfield int AlwaysHorizontalScrollbar Always show horizontal scrollbar (even if ContentSize.x < Size.x)
		@tfield int NoNavInputs No gamepad/keyboard navigation within the window
		@tfield int NoNavFocus No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
		@tfield int UnsavedDocument Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
		@tfield int NoNav
		@tfield int NoDecoration
		@tfield int NoInputs
		

		@table ImGuiWindowFlags
		*/
		lua.new_enum("ImGuiWindowFlags",
			"None", ImGuiWindowFlags_None,
			"NoTitleBar", ImGuiWindowFlags_NoTitleBar,
			"NoResize", ImGuiWindowFlags_NoResize,
			"NoMove", ImGuiWindowFlags_NoMove,
			"NoScrollbar", ImGuiWindowFlags_NoScrollbar,
			"NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse,
			"NoCollapse", ImGuiWindowFlags_NoCollapse,
			"AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize,
			"NoBackground", ImGuiWindowFlags_NoBackground,
			"NoSavedSettings", ImGuiWindowFlags_NoSavedSettings,
			"NoMouseInputs", ImGuiWindowFlags_NoMouseInputs,
			"MenuBar", ImGuiWindowFlags_MenuBar,
			"HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar,
			"NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing,
			"NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus,
			"AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar,
			"AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar,
			"NoNavInputs", ImGuiWindowFlags_NoNavInputs,
			"NoNavFocus", ImGuiWindowFlags_NoNavFocus,
			"UnsavedDocument", ImGuiWindowFlags_UnsavedDocument,
			"NoNav", ImGuiWindowFlags_NoNav,
			"NoDecoration", ImGuiWindowFlags_NoDecoration,
			"NoInputs", ImGuiWindowFlags_NoInputs
		);

		//@section ImGuiChildFlags
		/***
		Enum with a list of ImGui Focused Flags.

		@tfield int None
		@tfield int Border Show an outer border and enable WindowPadding. (Important: this is always == 1 == true for legacy reason)
		@tfield int AlwaysUseWindowPadding Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
		@tfield int ResizeX Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags)
		@tfield int ResizeY Allow resize from bottom border (layout direction). "
		@tfield int AutoResizeX Enable auto-resizing height.
		@tfield int AutoResizeY Enable auto-resizing height.
		@tfield int AlwaysAutoResize Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
		@tfield int FrameStyle Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.

		@table ImGuiChildFlags
		*/
		lua.new_enum("ImGuiChildFlags",
			"None", ImGuiFocusedFlags_None,
			"Border", ImGuiChildFlags_Border,
			"AlwaysUseWindowPadding", ImGuiChildFlags_AlwaysUseWindowPadding,
			"ResizeX", ImGuiChildFlags_ResizeX,
			"ResizeY", ImGuiChildFlags_ResizeY,
			"AutoResizeX", ImGuiChildFlags_AutoResizeX,
			"AutoResizeY", ImGuiChildFlags_AutoResizeY,
			"AlwaysAutoResize", ImGuiChildFlags_AlwaysAutoResize,
			"FrameStyle", ImGuiChildFlags_FrameStyle
		);



#pragma endregion Window Flags


		//@section ImGuiInputTextFlags
		/***
		Enum with a list of ImGui Input Text Flags.

		@tfield int None
		@tfield int CharsDecimal Allow 0123456789.+-*
		@tfield int CharsHexadecimal Allow 0123456789ABCDEFabcdef
		@tfield int CharsUppercase Turn a..z into A..Z
		@tfield int CharsNoBlank Filter out spaces, tabs
		@tfield int AutoSelectAll Select entire text when first taking mouse focus
		@tfield int EnterReturnsTrue Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
		@tfield int CallbackCompletion Callback on pressing TAB (for completion handling)
		@tfield int CallbackHistory Callback on pressing Up/Down arrows (for history handling)
		@tfield int CallbackAlways Callback on each iteration. User code may query cursor position, modify text buffer.
		@tfield int CallbackCharFilter Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
		@tfield int AllowTabInput Pressing TAB input a '\t' character into the text field
		@tfield int CtrlEnterForNewLine In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
		@tfield int NoHorizontalScroll Disable following the cursor horizontally
		@tfield int AlwaysOverwrite Overwrite mode
		@tfield int ReadOnly Read-only mode
		@tfield int Password Password mode, display all characters as '*'
		@tfield int NoUndoRedo Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
		@tfield int CharsScientific Allow 0123456789.+-*eE(Scientific notation input)
		@tfield int CallbackResize Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
		@tfield int CallbackEdit Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
		@tfield int EscapeClearsAll Escape key clears content if not empty, and deactivate otherwise (contrast to default behavior of Escape to revert)

		@table ImGuiInputTextFlags
		*/
#pragma region InputText Flags
		lua.new_enum("ImGuiInputTextFlags",
			"None", ImGuiInputTextFlags_None,
			"CharsDecimal", ImGuiInputTextFlags_CharsDecimal,
			"CharsHexadecimal", ImGuiInputTextFlags_CharsHexadecimal,
			"CharsUppercase", ImGuiInputTextFlags_CharsUppercase,
			"CharsNoBlank", ImGuiInputTextFlags_CharsNoBlank,
			"AutoSelectAll", ImGuiInputTextFlags_AutoSelectAll,
			"EnterReturnsTrue", ImGuiInputTextFlags_EnterReturnsTrue,
			"CallbackCompletion", ImGuiInputTextFlags_CallbackCompletion,
			"CallbackHistory", ImGuiInputTextFlags_CallbackHistory,
			"CallbackAlways", ImGuiInputTextFlags_CallbackAlways,
			"CallbackCharFilter", ImGuiInputTextFlags_CallbackCharFilter,
			"AllowTabInput", ImGuiInputTextFlags_AllowTabInput,
			"CtrlEnterForNewLine", ImGuiInputTextFlags_CtrlEnterForNewLine,
			"NoHorizontalScroll", ImGuiInputTextFlags_NoHorizontalScroll,
			"AlwaysOverwrite", ImGuiInputTextFlags_AlwaysOverwrite,
			"ReadOnly", ImGuiInputTextFlags_ReadOnly,
			"Password", ImGuiInputTextFlags_Password,
			"NoUndoRedo", ImGuiInputTextFlags_NoUndoRedo,
			"CharsScientific", ImGuiInputTextFlags_CharsScientific,
			"CallbackResize", ImGuiInputTextFlags_CallbackResize,
			"CallbackEdit", ImGuiInputTextFlags_CallbackEdit,
			"EscapeClearsAll", ImGuiInputTextFlags_EscapeClearsAll
		);
#pragma endregion InputText Flags


		//@section ImGuiTreeNodeFlags
		/***
		Enum with a list of ImGui Tree Node Flags.

		@tfield int None
		@tfield int Selected Draw as selected
		@tfield int Framed Draw frame with background (e.g. for CollapsingHeader)
		@tfield int AllowOverlap Hit testing to allow subsequent widgets to overlap this one
		@tfield int NoTreePushOnOpen Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
		@tfield int NoAutoOpenOnLog Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
		@tfield int DefaultOpen Default node to be open
		@tfield int OpenOnDoubleClick Need double-click to open node
		@tfield int OpenOnArrow Only open when clicking on the arrow part. If ImGuiTreeNodeFlags.OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
		@tfield int Leaf No collapsing, no arrow (use as a convenience for leaf nodes).
		@tfield int Bullet Display a bullet instead of arrow. IMPORTANT: node can still be marked open/close if you don't set the _Leaf flag!
		@tfield int FramePadding Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
		@tfield int SpanAvailWidth Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
		@tfield int SpanFullWidth Extend hit box to the left-most and right-most edges (bypass the indented area).
		@tfield int NavLeftJumpsBackHere (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
		@tfield int CollapsingHeader

		@table ImGuiTreeNodeFlags
		*/
#pragma region TreeNode Flags
		lua.new_enum("ImGuiTreeNodeFlags",
			"None", ImGuiTreeNodeFlags_None,
			"Selected", ImGuiTreeNodeFlags_Selected,
			"Framed", ImGuiTreeNodeFlags_Framed,
			"AllowOverlap", ImGuiTreeNodeFlags_AllowOverlap,
			"NoTreePushOnOpen", ImGuiTreeNodeFlags_NoTreePushOnOpen,
			"NoAutoOpenOnLog", ImGuiTreeNodeFlags_NoAutoOpenOnLog,
			"DefaultOpen", ImGuiTreeNodeFlags_DefaultOpen,
			"OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick,
			"OpenOnArrow", ImGuiTreeNodeFlags_OpenOnArrow,
			"Leaf", ImGuiTreeNodeFlags_Leaf,
			"Bullet", ImGuiTreeNodeFlags_Bullet,
			"FramePadding", ImGuiTreeNodeFlags_FramePadding,
			"SpanAvailWidth", ImGuiTreeNodeFlags_SpanAvailWidth,
			"SpanFullWidth", ImGuiTreeNodeFlags_SpanFullWidth,
			"NavLeftJumpsBackHere", ImGuiTreeNodeFlags_NavLeftJumpsBackHere,
			"CollapsingHeader", ImGuiTreeNodeFlags_CollapsingHeader
		);
#pragma endregion TreeNode Flags

		//@section ImGuiPopupFlags
		/***
		Enum with a list of ImGui Popup Flags.

		@tfield int None
		@tfield int MouseButtonLeft For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as ImGuiMouseButton_Left)
		@tfield int MouseButtonRight For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as ImGuiMouseButton_Right)
		@tfield int MouseButtonMiddle For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as ImGuiMouseButton_Middle)
		@tfield int MouseButtonMask_
		@tfield int MouseButtonDefault_
		@tfield int NoOpenOverExistingPopup For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack
		@tfield int NoOpenOverItems For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
		@tfield int AnyPopupId For IsPopupOpen(): ignore the ImGuiID parameter and test for any popup.
		@tfield int AnyPopupLevel For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)
		@tfield int AnyPopup

		@table ImGuiPopupFlags
		*/
#pragma region Popup Flags
		lua.new_enum("ImGuiPopupFlags",
			"None", ImGuiPopupFlags_None,
			"MouseButtonLeft", ImGuiPopupFlags_MouseButtonLeft,
			"MouseButtonRight", ImGuiPopupFlags_MouseButtonRight,
			"MouseButtonMiddle", ImGuiPopupFlags_MouseButtonMiddle,
			"MouseButtonMask_", ImGuiPopupFlags_MouseButtonMask_,
			"MouseButtonDefault_", ImGuiPopupFlags_MouseButtonDefault_,
			"NoOpenOverExistingPopup", ImGuiPopupFlags_NoOpenOverExistingPopup,
			"NoOpenOverItems", ImGuiPopupFlags_NoOpenOverItems,
			"AnyPopupId", ImGuiPopupFlags_AnyPopupId,
			"AnyPopupLevel", ImGuiPopupFlags_AnyPopupLevel,
			"AnyPopup", ImGuiPopupFlags_AnyPopup
		);
#pragma endregion Popup Flags

		//@section ImGuiSelectableFlags
		/***
		Enum with a list of ImGui Selectable Flags.

		@tfield int None
		@tfield int DontClosePopups Clicking this doesn't close parent popup window
		@tfield int SpanAllColumns Frame will span all columns of its container table (text will still fit in current column)
		@tfield int AllowDoubleClick Generate press events on double clicks too
		@tfield int Disabled Cannot be selected, display grayed out text
		@tfield int AllowOverlap  (WIP) Hit testing to allow subsequent widgets to overlap this one

		@table ImGuiSelectableFlags
		*/
#pragma region Selectable Flags
		lua.new_enum("ImGuiSelectableFlags",
			"None", ImGuiSelectableFlags_None,
			"DontClosePopups", ImGuiSelectableFlags_DontClosePopups,
			"SpanAllColumns", ImGuiSelectableFlags_SpanAllColumns,
			"AllowDoubleClick", ImGuiSelectableFlags_AllowDoubleClick,
			"Disabled", ImGuiSelectableFlags_Disabled,
			"AllowOverlap ", ImGuiSelectableFlags_AllowOverlap
		);
#pragma endregion Selectable Flags

		//@section ImGuiComboFlags
		/***
		Enum with a list of ImGui Combo Flags.

		@tfield int None
		@tfield int PopupAlignLeft Align the popup toward the left by default
		@tfield int HeightSmall Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
		@tfield int HeightRegular Max ~8 items visible (default)
		@tfield int HeightLarge Max ~20 items visible
		@tfield int HeightLargest As many fitting items as possible
		@tfield int NoArrowButton Display on the preview box without the square arrow button
		@tfield int NoPreview Display only a square arrow button
		@tfield int WidthFitPreview Width dynamically calculated from preview contents
		@tfield int HeightMask

		@table ImGuiComboFlags
		*/
#pragma region Combo Flags
		lua.new_enum("ImGuiComboFlags",
			"None", ImGuiComboFlags_None,
			"PopupAlignLeft", ImGuiComboFlags_PopupAlignLeft,
			"HeightSmall", ImGuiComboFlags_HeightSmall,
			"HeightRegular", ImGuiComboFlags_HeightRegular,
			"HeightLarge", ImGuiComboFlags_HeightLarge,
			"HeightLargest", ImGuiComboFlags_HeightLargest,
			"NoArrowButton", ImGuiComboFlags_NoArrowButton,
			"NoPreview", ImGuiComboFlags_NoPreview,
			"WidthFitPreview", ImGuiComboFlags_WidthFitPreview,
			"HeightMask", ImGuiComboFlags_HeightMask_
		);
#pragma endregion Combo Flags

		//@section ImGuiTabBarFlags
		/***
		Enum with a list of ImGui TabBar Flags.

		@tfield int None
		@tfield int Reorderable Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
		@tfield int AutoSelectNewTabs Automatically select new tabs when they appear
		@tfield int TabListPopupButton Disable buttons to open the tab list popup
		@tfield int NoCloseWithMiddleMouseButton Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You may handle this behavior manually on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
		@tfield int NoTabListScrollingButtons Disable scrolling buttons (apply when fitting policy is 
		@tfield int NoTooltip Disable tooltips when hovering a tab
		@tfield int FittingPolicyResizeDown Resize tabs when they don't fit
		@tfield int FittingPolicyScroll Add scroll buttons when tabs don't fit
		@tfield int FittingPolicyMask_
		@tfield int FittingPolicyDefault_

		@table ImGuiTabBarFlags
		*/
#pragma region TabBar Flags
		lua.new_enum("ImGuiTabBarFlags",
			"None", ImGuiTabBarFlags_None,
			"Reorderable", ImGuiTabBarFlags_Reorderable,
			"AutoSelectNewTabs", ImGuiTabBarFlags_AutoSelectNewTabs,
			"TabListPopupButton", ImGuiTabBarFlags_TabListPopupButton,
			"NoCloseWithMiddleMouseButton", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton,
			"NoTabListScrollingButtons", ImGuiTabBarFlags_NoTabListScrollingButtons,
			"NoTooltip", ImGuiTabBarFlags_NoTooltip,
			"FittingPolicyResizeDown", ImGuiTabBarFlags_FittingPolicyResizeDown,
			"FittingPolicyScroll", ImGuiTabBarFlags_FittingPolicyScroll,
			"FittingPolicyMask_", ImGuiTabBarFlags_FittingPolicyMask_,
			"FittingPolicyDefault_", ImGuiTabBarFlags_FittingPolicyDefault_
		);
#pragma endregion TabBar Flags


		//@section ImGuiTabItemFlags
		/***
		Enum with a list of ImGui TabItem Flags.

		@tfield int None
		@tfield int UnsavedDocument Display a dot next to the title + set ImGuiTabItemFlags_NoAssumedClosure.
		@tfield int SetSelected Trigger flag to programmatically make the tab selected when calling BeginTabItem()
		@tfield int NoCloseWithMiddleMouseButton Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You may handle this behavior manually on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
		@tfield int NoPushId Don't call PushID()/PopID() on BeginTabItem()/EndTabItem()
		@tfield int NoTooltip Disable tooltip for the given tab
		@tfield int NoReorder Disable reordering this tab or having another tab cross over this tab
		@tfield int Leading Enforce the tab position to the left of the tab bar (after the tab list popup button)
		@tfield int Trailing Enforce the tab position to the right of the tab bar (before the scrolling buttons)
		@tfield int NoAssumedClosure Tab is selected when trying to close + closure is not immediately assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.

		@table ImGuiTabItemFlags
		*/
#pragma region TabItem Flags
		lua.new_enum("ImGuiTabItemFlags",
			"None", ImGuiTabItemFlags_None,
			"UnsavedDocument", ImGuiTabItemFlags_UnsavedDocument,
			"SetSelected", ImGuiTabItemFlags_SetSelected,
			"NoCloseWithMiddleMouseButton", ImGuiTabItemFlags_NoCloseWithMiddleMouseButton,
			"NoPushId", ImGuiTabItemFlags_NoPushId,
			"NoTooltip", ImGuiTabItemFlags_NoTooltip,
			"NoReorder", ImGuiTabItemFlags_NoReorder,
			"Leading", ImGuiTabItemFlags_Leading,
			"Trailing", ImGuiTabItemFlags_Trailing,
			"NoAssumedClosure", ImGuiTabItemFlags_NoAssumedClosure
		);
#pragma endregion TabItem Flags

		//@section ImGuiFocusedFlags
		/***
		Enum with a list of ImGui Focused Flags.

		@tfield int None
		@tfield int ChildWindows Return true if any children of the window is focused
		@tfield int RootWindow Test from root window (top most parent of the current hierarchy)
		@tfield int AnyWindow Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
		@tfield int NoPopupHierarchy Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
		@tfield int RootAndChildWindows

		@table ImGuiFocusedFlags
		*/
#pragma region Focused Flags
		lua.new_enum("ImGuiFocusedFlags",
			"None", ImGuiFocusedFlags_None,
			"ChildWindows", ImGuiFocusedFlags_ChildWindows,
			"RootWindow", ImGuiFocusedFlags_RootWindow,
			"AnyWindow", ImGuiFocusedFlags_AnyWindow,
			"NoPopupHierarchy", ImGuiFocusedFlags_NoPopupHierarchy,
			"RootAndChildWindows", ImGuiFocusedFlags_RootAndChildWindows
		);
#pragma endregion Focused Flags

		//@section ImGuiHoveredFlags
		/***
		Enum with a list of ImGui Hovered Flags.

		@tfield int None
		@tfield int ChildWindows IsWindowHovered() only: Return true if any children of the window is hovered
		@tfield int RootWindow IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
		@tfield int AnyWindow IsWindowHovered() only: Return true if any window is hovered
		@tfield int NoPopupHierarchy IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
		@tfield int AllowWhenBlockedByPopup Return true even if a popup window is normally blocking access to this item/window
		@tfield int AllowWhenBlockedByActiveItem Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
		@tfield int AllowWhenOverlappedByItem IsItemHovered() only: Return true even if the item uses AllowOverlap mode and is overlapped by another hoverable item.
		@tfield int AllowWhenOverlappedByWindow IsItemHovered() only: Return true even if the position is obstructed or overlapped by another window.
		@tfield int AllowWhenDisabled IsItemHovered() only: Return true even if the item is disabled
		@tfield int NoNavOverride IsItemHovered() only: Disable using gamepad/keyboard navigation state when active, always query mouse
		@tfield int AllowWhenOverlapped
		@tfield int RectOnly
		@tfield int RootAndChildWindows
		@tfield int ForTooltip Shortcut for standard flags when using IsItemHovered() + SetTooltip() sequence.
		@tfield int Stationary Require mouse to be stationary for style.HoverStationaryDelay (~0.15 sec) _at least one time_. After this, can move on same item/window. Using the stationary test tends to reduces the need for a long delay.
		@tfield int DelayNone IsItemHovered() only: Return true immediately (default). As this is the default you generally ignore this.
		@tfield int DelayShort IsItemHovered() only: Return true after style.HoverDelayShort elapsed (~0.15 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
		@tfield int DelayNormal IsItemHovered() only: Return true after style.HoverDelayNormal elapsed (~0.40 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
		@tfield int NoSharedDelay IsItemHovered() only: Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)

		@table ImGuiHoveredFlags
		*/
#pragma region Hovered Flags
		lua.new_enum("ImGuiHoveredFlags",
			"None", ImGuiHoveredFlags_None,
			"ChildWindows", ImGuiHoveredFlags_ChildWindows,
			"RootWindow", ImGuiHoveredFlags_RootWindow,
			"AnyWindow", ImGuiHoveredFlags_AnyWindow,
			"NoPopupHierarchy", ImGuiHoveredFlags_NoPopupHierarchy,
			"AllowWhenBlockedByPopup", ImGuiHoveredFlags_AllowWhenBlockedByPopup,
			"AllowWhenBlockedByActiveItem", ImGuiHoveredFlags_AllowWhenBlockedByActiveItem,
			"AllowWhenOverlappedByItem", ImGuiHoveredFlags_AllowWhenOverlappedByItem,
			"AllowWhenOverlappedByWindow", ImGuiHoveredFlags_AllowWhenOverlappedByWindow,
			"AllowWhenDisabled", ImGuiHoveredFlags_AllowWhenDisabled,
			"NoNavOverride", ImGuiHoveredFlags_NoNavOverride,
			"RectOnly", ImGuiHoveredFlags_RectOnly,
			"AllowWhenOverlapped", ImGuiHoveredFlags_AllowWhenOverlapped,
			"RootAndChildWindows", ImGuiHoveredFlags_RootAndChildWindows,
			"ForTooltip", ImGuiHoveredFlags_ForTooltip,
			"Stationary", ImGuiHoveredFlags_Stationary,
			"DelayNone", ImGuiHoveredFlags_DelayNone,
			"DelayShort", ImGuiHoveredFlags_DelayShort,
			"DelayNormal", ImGuiHoveredFlags_DelayNormal,
			"NoSharedDelay", ImGuiHoveredFlags_NoSharedDelay
		);
#pragma endregion Hovered Flags


	}
}