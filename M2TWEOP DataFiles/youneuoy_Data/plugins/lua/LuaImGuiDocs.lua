---@diagnostic disable: missing-return, lowercase-global
---Enum with a list of ImGui Window Flags.
---@class ImGuiWindowFlags
ImGuiWindowFlags = { 

    ---@type integer
    None = nil,

    ---Disable title-bar
    ---@type integer
    NoTitleBar = nil,

    ---Disable user resizing with the lower-right grip
    ---@type integer
    NoResize = nil,

    ---Disable user moving the window
    ---@type integer
    NoMove = nil,

    ---Disable scrollbars (window can still scroll with mouse or programmatically)
    ---@type integer
    NoScrollbar = nil,

    ---Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    ---@type integer
    NoScrollWithMouse = nil,

    ---Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    ---@type integer
    NoCollapse = nil,

    ---Resize every window to its content every frame
    ---@type integer
    AlwaysAutoResize = nil,

    ---Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    ---@type integer
    NoBackground = nil,

    ---Never load/save settings in .ini file
    ---@type integer
    NoSavedSettings = nil,

    ---Disable catching mouse, hovering test with pass through.
    ---@type integer
    NoMouseInputs = nil,

    ---Has a menu-bar
    ---@type integer
    MenuBar = nil,

    ---Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    ---@type integer
    HorizontalScrollbar = nil,

    ---Disable taking focus when transitioning from hidden to visible state
    ---@type integer
    NoFocusOnAppearing = nil,

    ---Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    ---@type integer
    NoBringToFrontOnFocus = nil,

    ---Always show vertical scrollbar (even if ContentSize.y < Size.y)
    ---@type integer
    AlwaysVerticalScrollbar = nil,

    ---Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    ---@type integer
    AlwaysHorizontalScrollbar = nil,

    ---No gamepad/keyboard navigation within the window
    ---@type integer
    NoNavInputs = nil,

    ---No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
    ---@type integer
    NoNavFocus = nil,

    ---Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    ---@type integer
    UnsavedDocument = nil,

    ---@type integer
    ImGuiWindowFlags_NoNav = nil,

    ---@type integer
    ImGuiWindowFlags_NoDecoration = nil,

    ---@type integer
    ImGuiWindowFlags_NoInputs = nil,

}

---Enum with a list of ImGui Focused Flags.
---@class ImGuiChildFlags
ImGuiChildFlags = { 

    ---@type integer
    None = nil,

    ---Show an outer border and enable WindowPadding. (Important: this is always == 1 == true for legacy reason)
    ---@type integer
    Border = nil,

    ---Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    ---@type integer
    AlwaysUseWindowPadding = nil,

    ---Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags)
    ---@type integer
    ResizeX = nil,

    ---Allow resize from bottom border (layout direction). "
    ---@type integer
    ResizeY = nil,

    ---Enable auto-resizing height.
    ---@type integer
    AutoResizeX = nil,

    ---Enable auto-resizing height.
    ---@type integer
    AutoResizeY = nil,

    ---Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
    ---@type integer
    AlwaysAutoResize = nil,

    ---Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.
    ---@type integer
    FrameStyle = nil,

}

---Enum with a list of ImGui Input Text Flags.
---@class ImGuiInputTextFlags
ImGuiInputTextFlags = { 

    ---@type integer
    None = nil,

    ---Allow 0123456789.+-*
    ---@type integer
    CharsDecimal = nil,

    ---Allow 0123456789ABCDEFabcdef
    ---@type integer
    CharsHexadecimal = nil,

    ---Turn a..z into A..Z
    ---@type integer
    CharsUppercase = nil,

    ---Filter out spaces, tabs
    ---@type integer
    CharsNoBlank = nil,

    ---Select entire text when first taking mouse focus
    ---@type integer
    AutoSelectAll = nil,

    ---Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
    ---@type integer
    EnterReturnsTrue = nil,

    ---Callback on pressing TAB (for completion handling)
    ---@type integer
    CallbackCompletion = nil,

    ---Callback on pressing Up/Down arrows (for history handling)
    ---@type integer
    CallbackHistory = nil,

    ---Callback on each iteration. User code may query cursor position, modify text buffer.
    ---@type integer
    CallbackAlways = nil,

    ---Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
    ---@type integer
    CallbackCharFilter = nil,

    ---Pressing TAB input a '\t' character into the text field
    ---@type integer
    AllowTabInput = nil,

    ---In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
    ---@type integer
    CtrlEnterForNewLine = nil,

    ---Disable following the cursor horizontally
    ---@type integer
    NoHorizontalScroll = nil,

    ---Overwrite mode
    ---@type integer
    AlwaysOverwrite = nil,

    ---Read-only mode
    ---@type integer
    ReadOnly = nil,

    ---Password mode, display all characters as '*'
    ---@type integer
    Password = nil,

    ---Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
    ---@type integer
    NoUndoRedo = nil,

    ---Allow 0123456789.+-*eE(Scientific notation input)
    ---@type integer
    CharsScientific = nil,

    ---Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
    ---@type integer
    CallbackResize = nil,

    ---Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
    ---@type integer
    CallbackEdit = nil,

    ---Escape key clears content if not empty, and deactivate otherwise (contrast to default behavior of Escape to revert)
    ---@type integer
    EscapeClearsAll = nil,

}

---Enum with a list of ImGui Tree Node Flags.
---@class ImGuiTreeNodeFlags
ImGuiTreeNodeFlags = { 

    ---@type integer
    None = nil,

    ---Draw as selected
    ---@type integer
    Selected = nil,

    ---Draw frame with background (e.g. for CollapsingHeader)
    ---@type integer
    Framed = nil,

    ---Hit testing to allow subsequent widgets to overlap this one
    ---@type integer
    AllowOverlap = nil,

    ---Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    ---@type integer
    NoTreePushOnOpen = nil,

    ---Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
    ---@type integer
    NoAutoOpenOnLog = nil,

    ---Default node to be open
    ---@type integer
    DefaultOpen = nil,

    ---Need double-click to open node
    ---@type integer
    OpenOnDoubleClick = nil,

    ---Only open when clicking on the arrow part. If ImGuiTreeNodeFlags.OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
    ---@type integer
    OpenOnArrow = nil,

    ---No collapsing, no arrow (use as a convenience for leaf nodes).
    ---@type integer
    Leaf = nil,

    ---Display a bullet instead of arrow. IMPORTANT: node can still be marked open/close if you don't set the _Leaf flag!
    ---@type integer
    Bullet = nil,

    ---Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
    ---@type integer
    FramePadding = nil,

    ---Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
    ---@type integer
    SpanAvailWidth = nil,

    ---Extend hit box to the left-most and right-most edges (bypass the indented area).
    ---@type integer
    SpanFullWidth = nil,

    ---(WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
    ---@type integer
    NavLeftJumpsBackHere = nil,

    ---@type integer
    CollapsingHeader = nil,

}

---Enum with a list of ImGui Popup Flags.
---@class ImGuiPopupFlags
ImGuiPopupFlags = { 

    ---@type integer
    None = nil,

    ---For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as ImGuiMouseButton_Left)
    ---@type integer
    MouseButtonLeft = nil,

    ---For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as ImGuiMouseButton_Right)
    ---@type integer
    MouseButtonRight = nil,

    ---For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as ImGuiMouseButton_Middle)
    ---@type integer
    MouseButtonMiddle = nil,

    ---@type integer
    MouseButtonMask_ = nil,

    ---@type integer
    MouseButtonDefault_ = nil,

    ---For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack
    ---@type integer
    NoOpenOverExistingPopup = nil,

    ---For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
    ---@type integer
    NoOpenOverItems = nil,

    ---For IsPopupOpen(): ignore the ImGuiID parameter and test for any popup.
    ---@type integer
    AnyPopupId = nil,

    ---For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)
    ---@type integer
    AnyPopupLevel = nil,

    ---@type integer
    AnyPopup = nil,

}

---Enum with a list of ImGui Selectable Flags.
---@class ImGuiSelectableFlags
ImGuiSelectableFlags = { 

    ---@type integer
    None = nil,

    ---Clicking this doesn't close parent popup window
    ---@type integer
    DontClosePopups = nil,

    ---Frame will span all columns of its container table (text will still fit in current column)
    ---@type integer
    SpanAllColumns = nil,

    ---Generate press events on double clicks too
    ---@type integer
    AllowDoubleClick = nil,

    ---Cannot be selected, display grayed out text
    ---@type integer
    Disabled = nil,

    ---(WIP) Hit testing to allow subsequent widgets to overlap this one
    ---@type integer
    AllowOverlap = nil,

}

---Enum with a list of ImGui Combo Flags.
---@class ImGuiComboFlags
ImGuiComboFlags = { 

    ---@type integer
    None = nil,

    ---Align the popup toward the left by default
    ---@type integer
    PopupAlignLeft = nil,

    ---Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    ---@type integer
    HeightSmall = nil,

    ---Max ~8 items visible (default)
    ---@type integer
    HeightRegular = nil,

    ---Max ~20 items visible
    ---@type integer
    HeightLarge = nil,

    ---As many fitting items as possible
    ---@type integer
    HeightLargest = nil,

    ---Display on the preview box without the square arrow button
    ---@type integer
    NoArrowButton = nil,

    ---Display only a square arrow button
    ---@type integer
    NoPreview = nil,

    ---Width dynamically calculated from preview contents
    ---@type integer
    WidthFitPreview = nil,

    ---@type integer
    HeightMask = nil,

}

---Enum with a list of ImGui TabBar Flags.
---@class ImGuiTabBarFlags
ImGuiTabBarFlags = { 

    ---@type integer
    None = nil,

    ---Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
    ---@type integer
    Reorderable = nil,

    ---Automatically select new tabs when they appear
    ---@type integer
    AutoSelectNewTabs = nil,

    ---Disable buttons to open the tab list popup
    ---@type integer
    TabListPopupButton = nil,

    ---Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You may handle this behavior manually on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    ---@type integer
    NoCloseWithMiddleMouseButton = nil,

    ---Disable scrolling buttons (apply when fitting policy is
    ---@type integer
    NoTabListScrollingButtons = nil,

    ---Disable tooltips when hovering a tab
    ---@type integer
    NoTooltip = nil,

    ---Resize tabs when they don't fit
    ---@type integer
    FittingPolicyResizeDown = nil,

    ---Add scroll buttons when tabs don't fit
    ---@type integer
    FittingPolicyScroll = nil,

    ---@type integer
    FittingPolicyMask_ = nil,

    ---@type integer
    FittingPolicyDefault_ = nil,

}

---Enum with a list of ImGui TabItem Flags.
---@class ImGuiTabItemFlags
ImGuiTabItemFlags = { 

    ---@type integer
    None = nil,

    ---Display a dot next to the title + set ImGuiTabItemFlags_NoAssumedClosure.
    ---@type integer
    UnsavedDocument = nil,

    ---Trigger flag to programmatically make the tab selected when calling BeginTabItem()
    ---@type integer
    SetSelected = nil,

    ---Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You may handle this behavior manually on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    ---@type integer
    NoCloseWithMiddleMouseButton = nil,

    ---Don't call PushID()/PopID() on BeginTabItem()/EndTabItem()
    ---@type integer
    NoPushId = nil,

    ---Disable tooltip for the given tab
    ---@type integer
    NoTooltip = nil,

    ---Disable reordering this tab or having another tab cross over this tab
    ---@type integer
    NoReorder = nil,

    ---Enforce the tab position to the left of the tab bar (after the tab list popup button)
    ---@type integer
    Leading = nil,

    ---Enforce the tab position to the right of the tab bar (before the scrolling buttons)
    ---@type integer
    Trailing = nil,

    ---Tab is selected when trying to close + closure is not immediately assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    ---@type integer
    NoAssumedClosure = nil,

}

---Enum with a list of ImGui Focused Flags.
---@class ImGuiFocusedFlags
ImGuiFocusedFlags = { 

    ---@type integer
    None = nil,

    ---Return true if any children of the window is focused
    ---@type integer
    ChildWindows = nil,

    ---Test from root window (top most parent of the current hierarchy)
    ---@type integer
    RootWindow = nil,

    ---Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
    ---@type integer
    AnyWindow = nil,

    ---Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    ---@type integer
    NoPopupHierarchy = nil,

    ---@type integer
    RootAndChildWindows = nil,

}

---Enum with a list of ImGui Hovered Flags.
---@class ImGuiHoveredFlags
ImGuiHoveredFlags = { 

    ---@type integer
    None = nil,

    ---IsWindowHovered() only: Return true if any children of the window is hovered
    ---@type integer
    ChildWindows = nil,

    ---IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
    ---@type integer
    RootWindow = nil,

    ---IsWindowHovered() only: Return true if any window is hovered
    ---@type integer
    AnyWindow = nil,

    ---IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    ---@type integer
    NoPopupHierarchy = nil,

    ---Return true even if a popup window is normally blocking access to this item/window
    ---@type integer
    AllowWhenBlockedByPopup = nil,

    ---Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
    ---@type integer
    AllowWhenBlockedByActiveItem = nil,

    ---IsItemHovered() only: Return true even if the item uses AllowOverlap mode and is overlapped by another hoverable item.
    ---@type integer
    AllowWhenOverlappedByItem = nil,

    ---IsItemHovered() only: Return true even if the position is obstructed or overlapped by another window.
    ---@type integer
    AllowWhenOverlappedByWindow = nil,

    ---IsItemHovered() only: Return true even if the item is disabled
    ---@type integer
    AllowWhenDisabled = nil,

    ---IsItemHovered() only: Disable using gamepad/keyboard navigation state when active, always query mouse
    ---@type integer
    NoNavOverride = nil,

    ---@type integer
    AllowWhenOverlapped = nil,

    ---@type integer
    RectOnly = nil,

    ---@type integer
    RootAndChildWindows = nil,

    ---Shortcut for standard flags when using IsItemHovered() + SetTooltip() sequence.
    ---@type integer
    ForTooltip = nil,

    ---Require mouse to be stationary for style.HoverStationaryDelay (~0.15 sec) _at least one time_. After this, can move on same item/window. Using the stationary test tends to reduces the need for a long delay.
    ---@type integer
    Stationary = nil,

    ---IsItemHovered() only: Return true immediately (default). As this is the default you generally ignore this.
    ---@type integer
    DelayNone = nil,

    ---IsItemHovered() only: Return true after style.HoverDelayShort elapsed (~0.15 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    ---@type integer
    DelayShort = nil,

    ---IsItemHovered() only: Return true after style.HoverDelayNormal elapsed (~0.40 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    ---@type integer
    DelayNormal = nil,

    ---IsItemHovered() only: Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)
    ---@type integer
    NoSharedDelay = nil,

}

---Enum with a list of ImGui Dir.
---@class ImGuiDir
ImGuiDir = { 

    ---@type integer
    None = nil,

    ---@type integer
    Left = nil,

    ---@type integer
    Right = nil,

    ---@type integer
    Up = nil,

    ---@type integer
    Down = nil,

    ---@type integer
    COUNT = nil,

}

---Enum with a list of sort directions.
---@class ImGuiSortDirection
ImGuiSortDirection = { 

    ---@type integer
    None = nil,

    ---@type integer
    Ascending = nil,

    ---@type integer
    Descending = nil,

}

---Enum with a list of ImGui Key.
---@class ImGuiKey
ImGuiKey = { 

    ---@type integer
    None = nil,

    ---@type integer
    Tab = nil,

    ---@type integer
    LeftArrow = nil,

    ---@type integer
    RightArrow = nil,

    ---@type integer
    UpArrow = nil,

    ---@type integer
    DownArrow = nil,

    ---@type integer
    PageUp = nil,

    ---@type integer
    PageDown = nil,

    ---@type integer
    Home = nil,

    ---@type integer
    End = nil,

    ---@type integer
    Insert = nil,

    ---@type integer
    Delete = nil,

    ---@type integer
    Backspace = nil,

    ---@type integer
    Space = nil,

    ---@type integer
    Enter = nil,

    ---@type integer
    Escape = nil,

    ---@type integer
    LeftCtrl = nil,

    ---@type integer
    LeftShift = nil,

    ---@type integer
    LeftAlt = nil,

    ---@type integer
    LeftSuper = nil,

    ---@type integer
    RightCtrl = nil,

    ---@type integer
    RightShift = nil,

    ---@type integer
    RightAlt = nil,

    ---@type integer
    RightSuper = nil,

    ---@type integer
    Menu = nil,

    ---@type integer
    _0 = nil,

    ---@type integer
    _1 = nil,

    ---@type integer
    _2 = nil,

    ---@type integer
    _3 = nil,

    ---@type integer
    _4 = nil,

    ---@type integer
    _5 = nil,

    ---@type integer
    _6 = nil,

    ---@type integer
    _7 = nil,

    ---@type integer
    _8 = nil,

    ---@type integer
    _9 = nil,

    ---@type integer
    A = nil,

    ---@type integer
    B = nil,

    ---@type integer
    C = nil,

    ---@type integer
    D = nil,

    ---@type integer
    E = nil,

    ---@type integer
    F = nil,

    ---@type integer
    G = nil,

    ---@type integer
    H = nil,

    ---@type integer
    I = nil,

    ---@type integer
    J = nil,

    ---@type integer
    K = nil,

    ---@type integer
    L = nil,

    ---@type integer
    M = nil,

    ---@type integer
    N = nil,

    ---@type integer
    O = nil,

    ---@type integer
    P = nil,

    ---@type integer
    Q = nil,

    ---@type integer
    R = nil,

    ---@type integer
    S = nil,

    ---@type integer
    T = nil,

    ---@type integer
    U = nil,

    ---@type integer
    V = nil,

    ---@type integer
    W = nil,

    ---@type integer
    X = nil,

    ---@type integer
    Y = nil,

    ---@type integer
    Z = nil,

    ---@type integer
    F1 = nil,

    ---@type integer
    F2 = nil,

    ---@type integer
    F3 = nil,

    ---@type integer
    F4 = nil,

    ---@type integer
    F5 = nil,

    ---@type integer
    F6 = nil,

    ---@type integer
    F7 = nil,

    ---@type integer
    F8 = nil,

    ---@type integer
    F9 = nil,

    ---@type integer
    F10 = nil,

    ---@type integer
    F11 = nil,

    ---@type integer
    F12 = nil,

    ---@type integer
    F13 = nil,

    ---@type integer
    F14 = nil,

    ---@type integer
    F15 = nil,

    ---@type integer
    F16 = nil,

    ---@type integer
    F17 = nil,

    ---@type integer
    F18 = nil,

    ---@type integer
    F19 = nil,

    ---@type integer
    F20 = nil,

    ---@type integer
    F21 = nil,

    ---@type integer
    F22 = nil,

    ---@type integer
    F23 = nil,

    ---@type integer
    F24 = nil,

    ---'
    ---@type integer
    Apostrophe = nil,

    ---,
    ---@type integer
    Comma = nil,

    ----
    ---@type integer
    Minus = nil,

    ---.
    ---@type integer
    Period = nil,

    ---/
    ---@type integer
    Slash = nil,

    ---;
    ---@type integer
    Semicolon = nil,

    ---=
    ---@type integer
    Equal = nil,

    ---[
    ---@type integer
    LeftBracket = nil,

    ---\ (this text inhibit multiline comment caused by backslash)
    ---@type integer
    Backslash = nil,

    ---]
    ---@type integer
    RightBracket = nil,

    ---`
    ---@type integer
    GraveAccent = nil,

    ---@type integer
    CapsLock = nil,

    ---@type integer
    ScrollLock = nil,

    ---@type integer
    NumLock = nil,

    ---@type integer
    PrintScreen = nil,

    ---@type integer
    Pause = nil,

    ---@type integer
    Keypad0 = nil,

    ---@type integer
    Keypad1 = nil,

    ---@type integer
    Keypad2 = nil,

    ---@type integer
    Keypad3 = nil,

    ---@type integer
    Keypad4 = nil,

    ---@type integer
    Keypad5 = nil,

    ---@type integer
    Keypad6 = nil,

    ---@type integer
    Keypad7 = nil,

    ---@type integer
    Keypad8 = nil,

    ---@type integer
    Keypad9 = nil,

    ---@type integer
    KeypadDecimal = nil,

    ---@type integer
    KeypadDivide = nil,

    ---@type integer
    KeypadMultiply = nil,

    ---@type integer
    KeypadSubtract = nil,

    ---@type integer
    KeypadAdd = nil,

    ---@type integer
    KeypadEnter = nil,

    ---@type integer
    KeypadEqual = nil,

    ---Available on some keyboard/mouses. Often referred as "Browser Back"
    ---@type integer
    AppBack = nil,

    ---@type integer
    AppForward = nil,

    ---Menu (Xbox)      + (Switch)   Start/Options (PS)
    ---@type integer
    GamepadStart = nil,

    ---View (Xbox)      - (Switch)   Share (PS)
    ---@type integer
    GamepadBack = nil,

    ---X (Xbox)         Y (Switch)   Square (PS) Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
    ---@type integer
    GamepadFaceLeft = nil,

    ---B (Xbox)         A (Switch)   Circle (PS) Cancel / Close / Exit
    ---@type integer
    GamepadFaceRight = nil,

    ---Y (Xbox)         X (Switch)   Triangle (PS) Text Input / On-screen Keyboard
    ---@type integer
    GamepadFaceUp = nil,

    ---// A (Xbox)         B (Switch)   Cross (PS) Activate / Open / Toggle / Tweak
    ---@type integer
    GamepadFaceDown = nil,

    ---@type integer
    GamepadDpadLeft = nil,

    ---@type integer
    GamepadDpadRight = nil,

    ---@type integer
    GamepadDpadUp = nil,

    ---@type integer
    GamepadDpadDown = nil,

    ---@type integer
    GamepadL1 = nil,

    ---@type integer
    GamepadR1 = nil,

    ---@type integer
    GamepadL2 = nil,

    ---@type integer
    GamepadR2 = nil,

    ---@type integer
    GamepadL3 = nil,

    ---@type integer
    GamepadR3 = nil,

    ---@type integer
    GamepadLStickLeft = nil,

    ---@type integer
    GamepadLStickRight = nil,

    ---@type integer
    GamepadLStickUp = nil,

    ---@type integer
    GamepadLStickDown = nil,

    ---@type integer
    GamepadRStickLeft = nil,

    ---@type integer
    GamepadRStickRight = nil,

    ---@type integer
    GamepadRStickUp = nil,

    ---@type integer
    GamepadRStickDown = nil,

    ---@type integer
    MouseLeft = nil,

    ---@type integer
    MouseRight = nil,

    ---@type integer
    MouseMiddle = nil,

    ---@type integer
    MouseX1 = nil,

    ---@type integer
    MouseX2 = nil,

    ---@type integer
    MouseWheelX = nil,

    ---@type integer
    MouseWheelY = nil,

    ---@type integer
    ReservedForModCtrl = nil,

    ---@type integer
    ReservedForModShift = nil,

    ---@type integer
    ReservedForModAlt = nil,

    ---@type integer
    ReservedForModSuper = nil,

    ---@type integer
    COUNT = nil,

}

---Enum with a list of ImGui Conditions.
---@class ImGuiCond
ImGuiCond = { 

    ---No condition (always set the variable), same as _Always
    ---@type integer
    None = nil,

    ---No condition (always set the variable), same as _None
    ---@type integer
    Always = nil,

    ---Set the variable once per runtime session (only the first call will succeed)
    ---@type integer
    Once = nil,

    ---Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    ---@type integer
    FirstUseEver = nil,

    ---Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
    ---@type integer
    Appearing = nil,

}

---Enum with a list of ImGui Col.
---@class ImGuiCol
ImGuiCol = { 

    ---@type integer
    Text = nil,

    ---@type integer
    TextDisabled = nil,

    ---Background of normal windows
    ---@type integer
    WindowBg = nil,

    ---Background of child windows
    ---@type integer
    ChildBg = nil,

    ---Background of popups, menus, tooltips windows
    ---@type integer
    PopupBg = nil,

    ---@type integer
    Border = nil,

    ---@type integer
    BorderShadow = nil,

    ---Background of checkbox, radio button, plot, slider, text input
    ---@type integer
    FrameBg = nil,

    ---@type integer
    FrameBgHovered = nil,

    ---@type integer
    FrameBgActive = nil,

    ---Title bar
    ---@type integer
    TitleBg = nil,

    ---Title bar when focused
    ---@type integer
    TitleBgActive = nil,

    ---Title bar when collapsed
    ---@type integer
    TitleBgCollapsed = nil,

    ---@type integer
    MenuBarBg = nil,

    ---@type integer
    ScrollbarBg = nil,

    ---@type integer
    ScrollbarGrab = nil,

    ---@type integer
    ScrollbarGrabHovered = nil,

    ---@type integer
    ScrollbarGrabActive = nil,

    ---Checkbox tick and RadioButton circle
    ---@type integer
    CheckMark = nil,

    ---@type integer
    SliderGrab = nil,

    ---@type integer
    SliderGrabActive = nil,

    ---@type integer
    Button = nil,

    ---@type integer
    ButtonHovered = nil,

    ---@type integer
    ButtonActive = nil,

    ---Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    ---@type integer
    Header = nil,

    ---@type integer
    HeaderHovered = nil,

    ---@type integer
    HeaderActive = nil,

    ---@type integer
    Separator = nil,

    ---@type integer
    SeparatorHovered = nil,

    ---@type integer
    SeparatorActive = nil,

    ---Resize grip in lower-right and lower-left corners of windows.
    ---@type integer
    ResizeGrip = nil,

    ---@type integer
    ResizeGripHovered = nil,

    ---@type integer
    ResizeGripActive = nil,

    ---TabItem in a TabBar
    ---@type integer
    Tab = nil,

    ---@type integer
    TabHovered = nil,

    ---@type integer
    TabActive = nil,

    ---@type integer
    TabUnfocused = nil,

    ---@type integer
    TabUnfocusedActive = nil,

    ---@type integer
    PlotLines = nil,

    ---@type integer
    PlotLinesHovered = nil,

    ---@type integer
    PlotHistogram = nil,

    ---@type integer
    PlotHistogramHovered = nil,

    ---Table header background
    ---@type integer
    TableHeaderBg = nil,

    ---Table outer and header borders (prefer using Alpha=1.0 here)
    ---@type integer
    TableBorderStrong = nil,

    ---Table inner borders (prefer using Alpha=1.0 here)
    ---@type integer
    TableBorderLight = nil,

    ---Table row background (even rows)
    ---@type integer
    TableRowBg = nil,

    ---Table row background (odd rows)
    ---@type integer
    TableRowBgAlt = nil,

    ---@type integer
    TextSelectedBg = nil,

    ---@type integer
    DragDropTarget = nil,

    ---@type integer
    NavHighlight = nil,

    ---@type integer
    NavWindowingHighlight = nil,

    ---@type integer
    NavWindowingDimBg = nil,

    ---@type integer
    ModalWindowDimBg = nil,

    ---@type integer
    ModalWindowDarkening = nil,

    ---@type integer
    COUNT = nil,

}

---Enum with a list of ImGui Style Var.
---@class ImGuiStyleVar
ImGuiStyleVar = { 

    ---float
    ---@type integer
    Alpha = nil,

    ---float
    ---@type integer
    DisabledAlpha = nil,

    ---ImVec2
    ---@type integer
    WindowPadding = nil,

    ---float
    ---@type integer
    WindowRounding = nil,

    ---float
    ---@type integer
    WindowBorderSize = nil,

    ---ImVec2
    ---@type integer
    WindowMinSize = nil,

    ---ImVec2
    ---@type integer
    WindowTitleAlign = nil,

    ---float
    ---@type integer
    ChildRounding = nil,

    ---float
    ---@type integer
    ChildBorderSize = nil,

    ---float
    ---@type integer
    PopupRounding = nil,

    ---float
    ---@type integer
    PopupBorderSize = nil,

    ---ImVec2
    ---@type integer
    FramePadding = nil,

    ---float
    ---@type integer
    FrameRounding = nil,

    ---float
    ---@type integer
    FrameBorderSize = nil,

    ---ImVec2
    ---@type integer
    ItemSpacing = nil,

    ---ImVec2
    ---@type integer
    ItemInnerSpacing = nil,

    ---float
    ---@type integer
    IndentSpacing = nil,

    ---ImVec2
    ---@type integer
    CellPadding = nil,

    ---float
    ---@type integer
    ScrollbarSize = nil,

    ---float
    ---@type integer
    ScrollbarRounding = nil,

    ---float
    ---@type integer
    GrabMinSize = nil,

    ---float
    ---@type integer
    GrabRounding = nil,

    ---float
    ---@type integer
    TabRounding = nil,

    ---float
    ---@type integer
    TabBarBorderSize = nil,

    ---ImVec2
    ---@type integer
    ButtonTextAlign = nil,

    ---ImVec2
    ---@type integer
    SelectableTextAlign = nil,

    ---float
    ---@type integer
    SeparatorTextBorderSize = nil,

    ---ImVec2
    ---@type integer
    SeparatorTextAlign = nil,

    ---ImVec2
    ---@type integer
    SeparatorTextPadding = nil,

    ---@type integer
    COUNT = nil,

}

---Enum with a list of ImGui Button Flags.
---@class ImGuiButtonFlags
ImGuiButtonFlags = { 

    ---@type integer
    None = nil,

    ---React on left mouse button (default)
    ---@type integer
    Left = nil,

    ---React on right mouse button
    ---@type integer
    Right = nil,

    ---React on center mouse button
    ---@type integer
    Middle = nil,

}

---Enum with a list of ImGui Color Edit Flags.
---@class ImGuiColorEditFlags
ImGuiColorEditFlags = { 

    ---@type integer
    None = nil,

    ---ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
    ---@type integer
    NoAlpha = nil,

    ---ColorEdit: disable picker when clicking on color square.
    ---@type integer
    NoPicker = nil,

    ---ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
    ---@type integer
    NoOptions = nil,

    ---ColorEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)
    ---@type integer
    NoSmallPreview = nil,

    ---ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square)
    ---@type integer
    NoInputs = nil,

    ---ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
    ---@type integer
    NoTooltip = nil,

    ---ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
    ---@type integer
    NoLabel = nil,

    ---ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.
    ---@type integer
    NoSidePreview = nil,

    ---ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
    ---@type integer
    NoDragDrop = nil,

    ---ColorButton: disable border (which is enforced by default)
    ---@type integer
    NoBorder = nil,

    ---ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
    ---@type integer
    AlphaBar = nil,

    ---ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
    ---@type integer
    AlphaPreview = nil,

    ---ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
    ---@type integer
    AlphaPreviewHalf = nil,

    ---(WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use ImGuiColorEditFlags_Float flag as well).
    ---@type integer
    HDR = nil,

    ---ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    ---@type integer
    DisplayRGB = nil,

    ---ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    ---@type integer
    DisplayHSV = nil,

    ---ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    ---@type integer
    DisplayHex = nil,

    ---ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
    ---@type integer
    Uint8 = nil,

    ---ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
    ---@type integer
    Float = nil,

    ---ColorPicker: bar for Hue, rectangle for Sat/Value.
    ---@type integer
    PickerHueBar = nil,

    ---ColorPicker: wheel for Hue, triangle for Sat/Value.
    ---@type integer
    PickerHueWheel = nil,

    ---ColorEdit, ColorPicker: input and output data in RGB format.
    ---@type integer
    InputRGB = nil,

    ---ColorEdit, ColorPicker: input and output data in HSV format.
    ---@type integer
    InputHSV = nil,

    ---@type integer
    DefaultOptions = nil,

}

---Enum with a list of ImGui ImGuiSliderFlags.
---@class ImGuiSliderFlags
ImGuiSliderFlags = { 

    ---@type integer
    None = nil,

    ---Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.
    ---@type integer
    AlwaysClamp = nil,

    ---Make the widget logarithmic (linear otherwise). Consider using ImGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
    ---@type integer
    Logarithmic = nil,

    ---Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits)
    ---@type integer
    NoRoundToFormat = nil,

    ---Disable CTRL+Click or Enter key allowing to input text directly into the widget
    ---@type integer
    NoInput = nil,

    ---We treat using those bits as being potentially a 'float power' argument from the previous API that has got miscast to this enum, and will trigger an assert if needed.
    ---@type integer
    InvalidMask = nil,

}

---Enum with a list of ImGui MouseButton.
---@class ImGuiMouseButton
ImGuiMouseButton = { 

    ---@type integer
    Left = nil,

    ---@type integer
    Right = nil,

    ---@type integer
    Middle = nil,

    ---@type integer
    COUNT = nil,

}

---Enum with a list of ImGui Key Modifiers.
---@class ImGuiMod
ImGuiMod = { 

    ---@type integer
    None = nil,

    ---Ctrl
    ---@type integer
    Ctrl = nil,

    ---Shift
    ---@type integer
    Shift = nil,

    ---Option/Menu
    ---@type integer
    Alt = nil,

    ---Cmd/Super/Windows
    ---@type integer
    Super = nil,

    ---Alias for Ctrl (non-macOS) _or_ Super (macOS).
    ---@type integer
    Shortcut = nil,

    ---5-bits
    ---@type integer
    Mask = nil,

}

---Enum with a list of ImGui Mouse Cursor.
---@class ImGuiMouseCursor
ImGuiMouseCursor = { 

    ---@type integer
    None = nil,

    ---@type integer
    Arrow = nil,

    ---When hovering over InputText, etc.
    ---@type integer
    TextInput = nil,

    ---(Unused by Dear ImGui functions)
    ---@type integer
    ResizeAll = nil,

    ---When hovering over a horizontal border
    ---@type integer
    ResizeNS = nil,

    ---When hovering over a vertical border or a column
    ---@type integer
    ResizeEW = nil,

    ---When hovering over the bottom-left corner of a window
    ---@type integer
    ResizeNESW = nil,

    ---When hovering over the bottom-right corner of a window
    ---@type integer
    ResizeNWSE = nil,

    ---(Unused by Dear ImGui functions. Use for e.g. hyperlinks)
    ---@type integer
    Hand = nil,

    ---When hovering something with disallowed interaction. Usually a crossed circle.
    ---@type integer
    NotAllowed = nil,

    ---@type integer
    COUNT = nil,

}

---Enum with a list of ImGuiTableFlags.
---@class ImGuiTableFlags
ImGuiTableFlags = { 

    ---@type integer
    None = nil,

    ---Enable resizing columns.
    ---@type integer
    Resizable = nil,

    ---Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
    ---@type integer
    Reorderable = nil,

    ---Enable hiding/disabling columns in context menu.
    ---@type integer
    Hideable = nil,

    ---Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.
    ---@type integer
    Sortable = nil,

    ---Disable persisting columns order, width and sort settings in the .ini file.
    ---@type integer
    NoSavedSettings = nil,

    ---Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().
    ---@type integer
    ContextMenuInBody = nil,

    ---Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)
    ---@type integer
    RowBg = nil,

    ---Draw horizontal borders between rows.
    ---@type integer
    BordersInnerH = nil,

    ---Draw horizontal borders at the top and bottom.
    ---@type integer
    BordersOuterH = nil,

    ---Draw vertical borders between columns.
    ---@type integer
    BordersInnerV = nil,

    ---Draw vertical borders on the left and right sides.
    ---@type integer
    BordersOuterV = nil,

    ---Draw horizontal borders.
    ---@type integer
    BordersH = nil,

    ---Draw vertical borders.
    ---@type integer
    BordersV = nil,

    ---Draw inner borders.
    ---@type integer
    BordersInner = nil,

    ---Draw outer borders.
    ---@type integer
    BordersOuter = nil,

    ---Draw all borders.
    ---@type integer
    Borders = nil,

    ---[ALPHA] Disable vertical borders in columns Body (borders will always appear in Headers). -> May move to style
    ---@type integer
    NoBordersInBody = nil,

    ---[ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers)
    ---@type integer
    NoBordersInBodyUntilResize = nil,

    ---default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.
    ---@type integer
    SizingFixedFitColumns = nil,

    ---Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Implicitly enable ImGuiTableFlags_NoKeepColumnsVisible.
    ---@type integer
    SizingFixedSame = nil,

    ---Columns default to _WidthStretch with default weights proportional to each columns contents widths.
    ---@type integer
    SizingStretchProp = nil,

    ---Columns default to _WidthStretch with default weights all equal, unless overridden by TableSetupColumn().
    ---@type integer
    SizingStretchSame = nil,

    ---Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
    ---@type integer
    NoHostExtendX = nil,

    ---Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.
    ---@type integer
    NoHostExtendY = nil,

    ---Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
    ---@type integer
    NoKeepColumnsVisible = nil,

    ---Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.
    ---@type integer
    PreciseWidths = nil,

    ---Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with TableSetupScrollFreeze().
    ---@type integer
    NoClip = nil,

    ---Default if BordersOuterV is on. Enable outermost padding. Generally desirable if you have headers.
    ---@type integer
    PadOuterX = nil,

    ---Default if BordersOuterV is off. Disable outermost padding.
    ---@type integer
    NoPadOuterX = nil,

    ---Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
    ---@type integer
    NoPadInnerX = nil,

    ---Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this creates a child window, ScrollY is currently generally recommended when using ScrollX.
    ---@type integer
    ScrollX = nil,

    ---Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.
    ---@type integer
    ScrollY = nil,

    ---Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).
    ---@type integer
    SortMulti = nil,

    ---Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).
    ---@type integer
    SortTristate = nil,

    ---Highlight column headers when hovered (may evolve into a fuller highlight)
    ---@type integer
    HighlightHoveredColumn = nil,

}

---Basic ImGui table
---@class ImGui
ImGui = { 

}

--- Push window to the stack and start appending to it. Always call a matching End() for each Begin() call, regardless of its return value!
---@param name string window name
---@param open boolean? optional
---@param flags integer? optional
---@return boolean collapsed 
function ImGui.Begin(name, open, flags) end 

--- Pop window from the stack.
function ImGui.End() end 

--- Begin child. Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child. Always call a matching EndChild() for each BeginChild() call, regardless of its return value.
---@param name string window name
---@param sizeX number? optional
---@param sizeY number? optional
---@param open boolean? optional
---@param flags integer? optional
---@return boolean collapsed 
function ImGui.BeginChild(name, sizeX, sizeY, open, flags) end 

--- End Child.
function ImGui.EndChild() end 

--- Is Window Appearing?.
---@return boolean appearing 
function ImGui.IsWindowAppearing() end 

--- Is Window Collapsed?.
---@return boolean collapsed 
function ImGui.IsWindowCollapsed() end 

--- Is current window focused? Or its root/child, depending on flags.
---@param focusedFlags integer? optional
---@return boolean collapsed 
function ImGui.IsWindowFocused(focusedFlags) end 

--- Is current window hovered (and typically: not blocked by a popup/modal)?. IMPORTANT: If you are trying to check whether your mouse should be dispatched to Dear ImGui or to your underlying app, you should not use this function! Use the 'io.WantCaptureMouse' boolean for that!
---@param hoveredFlags integer? optional
---@return boolean collapsed 
function ImGui.IsWindowHovered(hoveredFlags) end 

--- Get window dpi scale.
---@return number scale 
function ImGui.GetWindowDpiScale() end 

--- Get current window position in screen space  (note: it is unlikely you need to use this. Consider using current layout pos instead, GetCursorScreenPos()).
---@return number posX 
---@return number posY 
function ImGui.GetWindowPos() end 

--- Get current window size. (note: it is unlikely you need to use this. Consider using GetCursorScreenPos() and e.g. GetContentRegionAvail() instead)
---@return number sizeX 
---@return number sizeY 
function ImGui.GetWindowSize() end 

--- Get current window width.
---@return number width 
function ImGui.GetWindowWidth() end 

--- Get current window height.
---@return number height 
function ImGui.GetWindowHeight() end 

--- Set next window position. Call before Begin(). Use pivot 0.5, 0.5 to center on given point, etc.
---@param posX number 
---@param posY number 
---@param condition integer? use ImGuiCond enum optional
---@param pivotX number? optional
---@param pivotY number? optional
function ImGui.SetNextWindowPos(posX, posY, condition, pivotX, pivotY) end 

--- Set next window size. Set axis to 0.0 to force an auto-fit on this axis. Call before Begin().
---@param sizeX number 
---@param sizeY number 
---@param condition integer? use ImGuiCond enum optional
function ImGui.SetNextWindowSize(sizeX, sizeY, condition) end 

--- Set next window size limits. Use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down.
---@param minX number 
---@param minY number 
---@param maxX number 
---@param maxY number 
function ImGui.SetNextWindowSizeConstraints(minX, minY, maxX, maxY) end 

--- Set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. Set an axis to 0.0f to leave it automatic. Call before Begin()
---@param sizeX number 
---@param sizeY number 
function ImGui.SetNextWindowContentSize(sizeX, sizeY) end 

--- Set next window collapsed state. Call before Begin()
---@param collapsed boolean 
---@param condition integer? use ImGuiCond enum optional
function ImGui.SetNextWindowCollapsed(collapsed, condition) end 

--- Set next window to be focused / top-most. Call before Begin()
function ImGui.SetNextWindowFocus() end 

--- Set next window background color alpha. Helper to easily override the Alpha component of ImGuiCol_WindowBg/ChildBg/PopupBg. You may also use ImGuiWindowFlags.NoBackground.
---@param alpha number 
function ImGui.SetNextWindowBgAlpha(alpha) end 

--- (not recommended) Set current window position - call within Begin()/End(). Prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
---@param name string? optional
---@param posX number 
---@param posY number 
---@param condition integer? use ImGuiCond enum optional
function ImGui.SetWindowPos(name, posX, posY, condition) end 

--- (not recommended) Set current window size - call within Begin()/End(). Prefer using SetNextWindowSize(), as this may incur tearing and side-effects.
---@param name string? optional
---@param sizeX number 
---@param sizeY number 
---@param condition integer? use ImGuiCond enum optional
function ImGui.SetWindowSize(name, sizeX, sizeY, condition) end 

--- (not recommended) Set current window size - call within Begin()/End(). Prefer using SetNextWindowSize(), as this may incur tearing and side-effects.
---@param name string? optional
---@param collapsed boolean 
---@param condition integer? use ImGuiCond enum optional
function ImGui.SetWindowCollapsed(name, collapsed, condition) end 

--- (not recommended) Set current window to be focused / top-most. Prefer using SetNextWindowFocus().
---@param name string? optional
function ImGui.SetWindowFocus(name) end 

--- Set font scale.
---@param scale number 
function ImGui.SetWindowFontScale(scale) end 

--- Current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates.
---@return number x 
---@return number y 
function ImGui.GetContentRegionMax() end 

--- GetContentRegionMax() - GetCursorPos().
---@return number x 
---@return number y 
function ImGui.GetContentRegionAvail() end 

--- Content boundaries min for the full window (roughly (0,0)-Scroll), in window coordinates.
---@return number x 
---@return number y 
function ImGui.GetWindowContentRegionMin() end 

--- Content boundaries max for the full window (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
---@return number x 
---@return number y 
function ImGui.GetWindowContentRegionMax() end 

--- Get window content region width
---@return number width 
function ImGui.GetWindowContentRegionWidth() end 

--- Get scrolling amount [0 .. GetScrollMaxX()]
---@return number x 
function ImGui.GetScrollX() end 

--- Get scrolling amount [0 .. GetScrollMaxY()]
---@return number y 
function ImGui.GetScrollY() end 

--- Get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - DecorationsSize.x
---@return number x 
function ImGui.GetScrollMaxX() end 

--- Get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - DecorationsSize.y
---@return number y 
function ImGui.GetScrollMaxY() end 

--- Set scrolling amount [0 .. GetScrollMaxX()]
---@param x number 
function ImGui.SetScrollX(x) end 

--- Set scrolling amount [0 .. GetScrollMaxY()]
---@param y number 
function ImGui.SetScrollY(y) end 

--- Adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
---@param centerXRatio number? optional
function ImGui.SetScrollHereX(centerXRatio) end 

--- Adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
---@param centerYRatio number? optional
function ImGui.SetScrollHereY(centerYRatio) end 

--- Adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
---@param localX number? optional
---@param centerXRatio number? optional
function ImGui.SetScrollFromPosX(localX, centerXRatio) end 

--- Adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
---@param localY number? optional
---@param centerYRatio number? optional
function ImGui.SetScrollFromPosY(localY, centerYRatio) end 

--- Modify a style variable.
---@param styleVar integer 
---@param value number? (or valX) optional
---@param valY number? optional
function ImGui.PushStyleVar(styleVar, value, valY) end 

--- Pop a style variable.
---@param count integer? optional
function ImGui.PopStyleVar(count) end 

--- Use nil as a shortcut to push default font.
---@param font ImFont 
function ImGui.PushFont(font) end 

--- Pop font.
function ImGui.PopFont() end 

--- Push style color.
---@param idx integer use ImGuiCol enum
---@param colorR number 
---@param colorG number 
---@param colorB number 
---@param colorA number 
function ImGui.PushStyleColor(idx, colorR, colorG, colorB, colorA) end 

--- Pop style color.
---@param count integer? optional
function ImGui.PopStyleColor(count) end 

--- Retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in
---@param idx integer use ImGuiCol enum
---@return number colorR 
---@return number colorG 
---@return number colorB 
---@return number colorA 
function ImGui.GetStyleColorVec4(idx) end 

--- Get current font.
---@return ImFont font 
function ImGui.GetFont() end 

--- Get current font size (= height in pixels) of current font with current scale applied
---@return number size 
function ImGui.GetFontSize() end 

--- Get UV coordinate for a white pixel.
---@return number x 
---@return number y 
function ImGui.GetFontTexUvWhitePixel() end 

--- Push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side).
---@param width number 
function ImGui.PushItemWidth(width) end 

--- Pop Item Width.
function ImGui.PopItemWidth() end 

--- Set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side)
---@param width number 
function ImGui.SetNextItemWidth(width) end 

--- Width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
---@return number width 
function ImGui.CalcItemWidth() end 

--- Push word-wrapping position for Text commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrapLocalPosX' position in window local space
---@return number wrapLocalPosX optional
function ImGui.PushTextWrapPos() end 

--- Pop Text Wrap Pos
function ImGui.PopTextWrapPos() end 

--- Tab stop enable. Allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets.
---@param tabStop boolean 
function ImGui.PushTabStop(tabStop) end 

--- Pop Tab Stop.
function ImGui.PopTabStop() end 

--- In 'repeat' mode, Button functions return repeated true in a typematic manner. Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
---@param doRepeat boolean 
function ImGui.PushButtonRepeat(doRepeat) end 

--- Pop buttom repeat.
function ImGui.PopButtonRepeat() end 

--- Separator, generally horizontal. Inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
function ImGui.Separator() end 

--- Call between widgets or groups to layout them horizontally. X position given in window coordinates.
---@param offsetFromStartX number? optional
function ImGui.SameLine(offsetFromStartX) end 

--- Undo a SameLine() or force a new line when in an horizontal-layout context.
function ImGui.NewLine() end 

--- Add vertical spacing.
function ImGui.Spacing() end 

--- Add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into..
---@param sizeX number 
---@param sizeY number 
function ImGui.Dummy(sizeX, sizeY) end 

--- Move content position toward the right, by indentWidth, or style.IndentSpacing if indentWidth <= 0
---@param indentWidth number? optional
function ImGui.Indent(indentWidth) end 

--- Move content position back to the left, by indentWidth, or style.IndentSpacing if indentWidth <= 0
---@param indentWidth number? optional
function ImGui.Unindent(indentWidth) end 

--- Lock horizontal starting position
function ImGui.BeginGroup() end 

--- Unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
function ImGui.EndGroup() end 

--- Cursor position in window coordinates (relative to window position)
---@return number posX 
---@return number posY 
function ImGui.GetCursorPos() end 

--- Some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.
---@return number posX 
function ImGui.GetCursorPosX() end 

--- Other functions such as GetCursorScreenPos.
---@return number posY 
function ImGui.GetCursorPosY() end 

--- Are using the main, absolute coordinate system.
---@param localX number 
---@param localY number 
function ImGui.SetCursorPos(localX, localY) end 

--- GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.).
---@param localX number 
function ImGui.SetCursorPosX(localX) end 

--- GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.).
---@param localY number 
function ImGui.SetCursorPosY(localY) end 

--- Initial cursor position in window coordinates.
---@return number posX 
---@return number posY 
function ImGui.GetCursorStartPos() end 

--- Cursor position in absolute coordinates. generally top-left == GetMainViewport()->Pos == (0,0) in single viewport mode, and bottom-right == GetMainViewport()->Pos+Size == io.DisplaySize in single-viewport mode..
---@return number posX 
---@return number posY 
function ImGui.GetCursorScreenPos() end 

--- Cursor position in window coordinates.
---@param posX number 
---@param posY number 
function ImGui.SetCursorScreenPos(posX, posY) end 

--- Vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item).
function ImGui.AlignTextToFramePadding() end 

--- FontSize.
---@return number height 
function ImGui.GetTextLineHeight() end 

--- FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text).
---@return number height 
function ImGui.GetTextLineHeightWithSpacing() end 

--- FontSize + style.FramePadding.y * 2
---@return number height 
function ImGui.GetFrameHeight() end 

--- FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)
---@return number height 
function ImGui.GetFrameHeightWithSpacing() end 

--- Push string into the ID stack (will hash string).
---@param id string 
---@param idEnd string? optional
function ImGui.PushID(id, idEnd) end 

--- Pop from the ID stack.
function ImGui.PopID() end 

--- Calculate unique ID (hash of whole ID stack + given parameter).
---@param id string 
---@param idEnd string? optional
---@return integer ImGUIID 
function ImGui.GetID(id, idEnd) end 

--- Raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text..
---@param text string 
---@param textEnd string? optional
function ImGui.TextUnformatted(text, textEnd) end 

--- Formatted text.
---@param text string 
function ImGui.Text(text) end 

--- Shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();.
---@param text string 
---@return number colorR 
---@return number colorG 
---@return number colorB 
---@return number colorA 
function ImGui.TextColored(text) end 

--- Shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();.
---@param text string 
function ImGui.TextDisabled(text) end 

--- Shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
---@param text string 
function ImGui.TextWrapped(text) end 

--- Display text+label aligned the same way as value+label widgets.
---@param label string 
---@param text string 
function ImGui.LabelText(label, text) end 

--- Shortcut for Bullet()+Text().
---@param text string 
function ImGui.BulletText(text) end 

--- Button.
---@param label string 
---@param sizeX number? optional
---@param sizeY number? optional
function ImGui.Button(label, sizeX, sizeY) end 

--- Button with FramePadding=(0,0) to easily embed within text.
---@param label string 
function ImGui.SmallButton(label) end 

--- Flexible button behavior without the visuals.
---@param label string 
---@param sizeX number 
---@param sizeY number 
function ImGui.InvisibleButton(label, sizeX, sizeY) end 

--- Square button with an arrow shape.
---@param label string 
---@param direction integer use ImGuiDir enum
function ImGui.ArrowButton(label, direction) end 

--- Checkbox.
---@param label string 
---@param isChecked boolean 
---@return boolean isChecked 
---@return boolean somethingElse 
function ImGui.Checkbox(label, isChecked) end 

--- Radio button.
---@param label string 
---@param value boolean 
---@param valueButton integer? optional
---@return boolean isChecked 
---@return boolean somethingElse 
function ImGui.RadioButton(label, value, valueButton) end 

--- Progress Bar.
---@param value number 
---@param sizeX number? optional
---@param sizeY number? optional
---@param overlay string? optional
function ImGui.ProgressBar(value, sizeX, sizeY, overlay) end 

--- Draw a small circle + keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses.
function ImGui.Bullet() end 

--- Currently: formatted text with an horizontal line
---@param text string 
function ImGui.SeparatorText(text) end 

--- Image button.
---@param texture any 
---@param sizeX number 
---@param sizeY number 
function ImGui.ImageButton(texture, sizeX, sizeY) end 

--- Image.
---@param texture any 
---@param sizeX number 
---@param sizeY number 
function ImGui.Image(texture, sizeX, sizeY) end 

--- The BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
---@param label string 
---@param previewValue string 
---@param comboFlags integer? optional
---@return boolean returnValue 
function ImGui.BeginCombo(label, previewValue, comboFlags) end 

--- Only call EndCombo() if BeginCombo() returns true!
function ImGui.EndCombo() end 

--- The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose. This is analogous to how ListBox are created.
---@param label string 
---@param currentItem integer 
---@param items table 
---@param itemCount integer? optional
---@param popupMaxHeightInItems integer? optional
---@return integer returnInteger 
---@return boolean returnBool 
function ImGui.Combo(label, currentItem, items, itemCount, popupMaxHeightInItems) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValue number 
---@param speed number? optional
---@param minValue number? optional
---@param maxValue number? optional
---@param format string? optional
---@param power number? optional
---@return number value 
---@return boolean returnBool 
function ImGui.DragFloat(label, inputValue, speed, minValue, maxValue, format, power) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValues table<integer, number> Maximum: 2
---@param speed number? optional
---@param minValue number? optional
---@param maxValue number? optional
---@param format string? optional
---@param power number? optional
---@return table<integer, number> values Maximum: 2
---@return boolean returnBool 
function ImGui.DragFloat2(label, inputValues, speed, minValue, maxValue, format, power) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValues table<integer, number> Maximum: 3
---@param speed number? optional
---@param minValue number? optional
---@param maxValue number? optional
---@param format string? optional
---@param power number? optional
---@return table<integer, number> values Maximum: 3
---@return boolean returnBool 
function ImGui.DragFloat3(label, inputValues, speed, minValue, maxValue, format, power) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValues table<integer, number> Maximum: 4
---@param speed number? optional
---@param minValue number? optional
---@param maxValue number? optional
---@param format string? optional
---@param power number? optional
---@return table<integer, number> values Maximum: 4
---@return boolean returnBool 
function ImGui.DragFloat4(label, inputValues, speed, minValue, maxValue, format, power) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValue integer 
---@param speed number? optional
---@param minValue integer? optional
---@param maxValue integer? optional
---@param format string? optional
---@return integer value 
---@return boolean returnBool 
function ImGui.DragInt(label, inputValue, speed, minValue, maxValue, format) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValues table<integer, integer> Maximum: 2
---@param speed number? optional
---@param minValue integer? optional
---@param maxValue integer? optional
---@param format string? optional
---@return table<integer, integer> values Maximum: 2
---@return boolean returnBool 
function ImGui.DragInt2(label, inputValues, speed, minValue, maxValue, format) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValues table<integer, integer> Maximum: 3
---@param speed number? optional
---@param minValue integer? optional
---@param maxValue integer? optional
---@param format string? optional
---@return table<integer, integer> values Maximum: 3
---@return boolean returnBool 
function ImGui.DragInt3(label, inputValues, speed, minValue, maxValue, format) end 

--- CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
---@param label string 
---@param inputValues table<integer, integer> Maximum: 4
---@param speed number? optional
---@param minValue integer? optional
---@param maxValue integer? optional
---@param format string? optional
---@return table<integer, integer> values Maximum: 4
---@return boolean returnBool 
function ImGui.DragInt4(label, inputValues, speed, minValue, maxValue, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValue number 
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@param power number? optional
---@return number value 
---@return boolean returnBool 
function ImGui.SliderFloat(label, inputValue, minValue, maxValue, format, power) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValues table<integer, number> Maximum: 2
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@param power number? optional
---@return table<integer, number> values Maximum: 2
---@return boolean returnBool 
function ImGui.SliderFloat2(label, inputValues, minValue, maxValue, format, power) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValues table<integer, number> Maximum: 3
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@param power number? optional
---@return table<integer, number> values Maximum: 3
---@return boolean returnBool 
function ImGui.SliderFloat3(label, inputValues, minValue, maxValue, format, power) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValues table<integer, number> Maximum: 4
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@param power number? optional
---@return table<integer, number> values Maximum: 4
---@return boolean returnBool 
function ImGui.SliderFloat4(label, inputValues, minValue, maxValue, format, power) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param radius number 
---@param minDegree number? optional
---@param maxDegree number? optional
---@param format string? optional
---@return number degree 
---@return boolean returnBool 
function ImGui.SliderAngle(label, radius, minDegree, maxDegree, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValue integer 
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@return integer value 
---@return boolean returnBool 
function ImGui.SliderInt(label, inputValue, minValue, maxValue, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValues table<integer, integer> Maximum: 2
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@return table<integer, integer> values Maximum: 2
---@return boolean returnBool 
function ImGui.SliderInt2(label, inputValues, minValue, maxValue, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValues table<integer, integer> Maximum: 3
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@return table<integer, integer> values Maximum: 3
---@return boolean returnBool 
function ImGui.SliderInt3(label, inputValues, minValue, maxValue, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param inputValues table<integer, integer> Maximum: 4
---@param minValue number 
---@param maxValue number 
---@param format string? optional
---@return table<integer, integer> values Maximum: 4
---@return boolean returnBool 
function ImGui.SliderInt4(label, inputValues, minValue, maxValue, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param sizeX number 
---@param sizeY number 
---@param inputValue number 
---@param minValue number? optional
---@param maxValue number? optional
---@param format string? optional
---@return number value 
---@return boolean returnBool 
function ImGui.VSliderFloat(label, sizeX, sizeY, inputValue, minValue, maxValue, format) end 

--- CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
---@param label string 
---@param sizeX number 
---@param sizeY number 
---@param inputValue integer 
---@param minValue integer? optional
---@param maxValue integer? optional
---@param format string? optional
---@return integer value 
---@return boolean returnBool 
function ImGui.VSliderInt(label, sizeX, sizeY, inputValue, minValue, maxValue, format) end 

--- Input with Keyboard.
---@param label string 
---@param text string 
---@param bufferSize integer? optional
---@param inputTextFlags integer? optional
---@return string value 
---@return boolean returnBool 
function ImGui.InputText(label, text, bufferSize, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param text string 
---@param bufferSize integer 
---@param sizeX number? optional
---@param sizeY number? optional
---@param inputTextFlags integer? optional
---@return string value 
---@return boolean returnBool 
function ImGui.InputTextMultiline(label, text, bufferSize, sizeX, sizeY, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param text string 
---@param hint string 
---@param bufferSize integer 
---@param inputTextFlags integer? optional
---@return string value 
---@return boolean returnBool 
function ImGui.InputTextWithHint(label, text, hint, bufferSize, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input number 
---@param step number? optional
---@param stepFast number? optional
---@param format string? optional
---@param inputTextFlags integer? optional
---@return number value 
---@return boolean returnBool 
function ImGui.InputFloat(label, input, step, stepFast, format, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input table<integer, number> Maximum: 2
---@param format string? optional
---@param inputTextFlags integer? optional
---@return table<integer, number> value Maximum: 2
---@return boolean returnBool 
function ImGui.InputFloat2(label, input, format, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input table<integer, number> Maximum: 3
---@param format string? optional
---@param inputTextFlags integer? optional
---@return table<integer, number> value Maximum: 3
---@return boolean returnBool 
function ImGui.InputFloat3(label, input, format, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input table<integer, number> Maximum: 4
---@param format string? optional
---@param inputTextFlags integer? optional
---@return table<integer, number> value Maximum: 4
---@return boolean returnBool 
function ImGui.InputFloat4(label, input, format, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input integer 
---@param step integer? optional
---@param stepFast integer? optional
---@param inputTextFlags integer? optional
---@return integer value 
---@return boolean returnBool 
function ImGui.InputInt(label, input, step, stepFast, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input table<integer, integer> Maximum: 2
---@param inputTextFlags integer? optional
---@return table<integer, integer> value Maximum: 2
---@return boolean returnBool 
function ImGui.InputInt2(label, input, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input table<integer, integer> Maximum: 3
---@param inputTextFlags integer? optional
---@return table<integer, integer> value Maximum: 3
---@return boolean returnBool 
function ImGui.InputInt3(label, input, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input table<integer, integer> Maximum: 4
---@param inputTextFlags integer? optional
---@return table<integer, integer> value Maximum: 4
---@return boolean returnBool 
function ImGui.InputInt4(label, input, inputTextFlags) end 

--- Input with Keyboard.
---@param label string 
---@param input number 
---@param step number? optional
---@param stepFast number? optional
---@param format string? optional
---@param inputTextFlags integer? optional
---@return number value 
---@return boolean returnBool 
function ImGui.InputDouble(label, input, step, stepFast, format, inputTextFlags) end 

--- Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
---@param label string 
---@param input table<integer, number> Maximum: 3
---@param colorEditFlags integer? optional
---@return table<integer, number> color Maximum: 3
---@return boolean returnBool 
function ImGui.ColorEdit3(label, input, colorEditFlags) end 

--- Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
---@param label string 
---@param input table<integer, number> Maximum: 4
---@param colorEditFlags integer? optional
---@return table<integer, number> color Maximum: 4
---@return boolean returnBool 
function ImGui.ColorEdit4(label, input, colorEditFlags) end 

--- Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
---@param label string 
---@param input table<integer, number> Maximum: 3
---@param colorEditFlags integer? optional
---@return table<integer, number> color Maximum: 3
---@return boolean returnBool 
function ImGui.ColorPicker3(label, input, colorEditFlags) end 

--- Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
---@param label string 
---@param input table<integer, number> Maximum: 4
---@param colorEditFlags integer? optional
---@return table<integer, number> color Maximum: 4
---@return boolean returnBool 
function ImGui.ColorPicker4(label, input, colorEditFlags) end 

--- Display a color square/button, hover for details, return true when pressed.
---@param label string 
---@param input table<integer, number> Maximum: 4
---@param colorEditFlags integer? optional
---@param sizeX number? optional
---@param sizeY number? optional
---@return boolean result 
function ImGui.ColorButton(label, input, colorEditFlags, sizeX, sizeY) end 

--- Initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.
---@param colorEditFlags integer 
function ImGui.SetColorEditOptions(colorEditFlags) end 

--- TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents..
---@param label string 
---@param text string? helper variation to easily decorelate the id from the displayed string. optional
---@return boolean returnBool 
function ImGui.TreeNode(label, text) end 

--- TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
---@param label string 
---@param treeNodeFlags integer? optional
---@param text string? helper variation to easily decorelate the id from the displayed string. optional
---@return boolean returnBool 
function ImGui.TreeNodeEx(label, treeNodeFlags, text) end 

--- Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
---@param label string 
function ImGui.TreePush(label) end 

--- Tree pop.
function ImGui.TreePop() end 

--- Horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode.
---@return number spacing 
function ImGui.GetTreeNodeToLabelSpacing() end 

--- If returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop(). If second argument bool then display an additional small close button on upper right of the header which will set the bool to false when clicked.
---@param label string 
---@param openOrFlags integer 
---@param flags integer 
---@return boolean open 
---@return boolean returnBool2 
function ImGui.CollapsingHeader(label, openOrFlags, flags) end 

--- Set next TreeNode/CollapsingHeader open state.
---@param open boolean 
---@param condition integer? optional
function ImGui.SetNextItemOpen(open, condition) end 

--- A selectable highlights when hovered, and can display another color when selected. Neighbors selectable extend their highlight bounds in order to leave no gap between them. This is so a series of selected Selectable appear contiguous. "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
---@param label string 
---@param selected boolean? optional
---@param selectableFlags integer? optional
---@param sizeX number? optional
---@param sizeY number? optional
---@return boolean value 
function ImGui.Selectable(label, selected, selectableFlags, sizeX, sizeY) end 

--- Open a framed scrolling region. This is essentially a thin wrapper to using BeginChild/EndChild with the ImGuiChildFlags_FrameStyle flag for stylistic changes + displaying a label. You can submit contents and manage your selection state however you want it, by creating e.g. Selectable() or any other items.
---@param label string 
---@param sizeX number 
---@param sizeY number 
---@return boolean result 
function ImGui.BeginListBox(label, sizeX, sizeY) end 

--- Only call EndListBox() if BeginListBox() returned true!
function ImGui.EndListBox() end 

--- This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes. Open a framed scrolling region.
---@param label string 
---@param currentItem integer 
---@param items table 
---@param itemCount integer 
---@param heightInItems integer? optional
function ImGui.ListBox(label, currentItem, items, itemCount, heightInItems) end 

--- This is merely a shortcut to calling Text() with a format string. Output single value in "name: value" format.
---@param prefix string 
---@param value number 
---@param floatFormat string? optional
function ImGui.Value(prefix, value, floatFormat) end 

--- Append to menu-bar of current window (requires ImGuiWindowFlags.MenuBar flag set on parent window). Use BeginMenuBar() on a window ImGuiWindowFlags.MenuBar to append to its menu bar.
---@return boolean value 
function ImGui.BeginMenuBar() end 

--- Only call EndMenuBar() if BeginMenuBar() returns true!
function ImGui.EndMenuBar() end 

--- Create and append to a full screen menu-bar. Use BeginMainMenuBar() to create a menu bar at the top of the screen and append to it.
---@return boolean value 
function ImGui.BeginMainMenuBar() end 

--- Only call EndMainMenuBar() if BeginMainMenuBar() returns true!
function ImGui.EndMainMenuBar() end 

--- Create a sub-menu entry. only call EndMenu() if this returns true! Use BeginMenu() to create a menu. You can call BeginMenu() multiple time with the same identifier to append more items to it.
---@param id string 
---@param enabled boolean? optional
---@return boolean value 
function ImGui.BeginMenu(id, enabled) end 

--- Only call EndMenu() if BeginMenu() returns true!
function ImGui.EndMenu() end 

--- Return true when activated.
---@param label string 
---@param shortcut string? optional
---@param selected boolean? optional
---@param enabled boolean? optional
---@return boolean value 
---@return boolean value2 return true when activated + param selected is true
function ImGui.MenuItem(label, shortcut, selected, enabled) end 

--- Tooltip are windows following the mouse. They do not take focus away. Begin/append a tooltip window. To create full-featured tooltip (with any kind of items).
---@return boolean result 
function ImGui.BeginTooltip() end 

--- Only call EndTooltip() if BeginTooltip()/BeginItemTooltip() returns true!
function ImGui.EndTooltip() end 

--- Tooltip are windows following the mouse. They do not take focus away. Set a text-only tooltip, typically use with IsItemHovered(). Override any previous call to SetTooltip().
---@param text string 
function ImGui.SetTooltip(text) end 

--- Begin/append a tooltip window if preceding item was hovered. BeginItemTooltip() is a shortcut for the 'if (IsItemHovered(ImGuiHoveredFlags_ForTooltip) && BeginTooltip())' idiom. Where 'ImGuiHoveredFlags_ForTooltip' itself is a shortcut to use 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' depending on active input type. For mouse it defaults to 'ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayShort'.
---@return boolean result 
function ImGui.BeginItemTooltip() end 

--- Set a text-only tooltip if preceeding item was hovered. override any previous call to SetTooltip(). SetItemTooltip() is a shortcut for the 'if (IsItemHovered(ImGuiHoveredFlags_ForTooltip)) { SetTooltip(...); }' idiom.
---@param text string 
function ImGui.SetItemTooltip(text) end 

--- They block normal mouse hovering detection (and therefore most mouse interactions) behind them. They can be closed by clicking anywhere outside them, or by pressing ESCAPE. Their visibility state (~bool) is held internally instead of being held by the programmer as we are used to with regular Begin*() calls. The 3 properties above are related: we need to retain popup visibility state in the library because popups may be closed as any time. Popup identifiers are relative to the current ID stack, so OpenPopup and BeginPopup generally needs to be at the same level of the stack. BeginPopup(): query popup state, if open start appending into the window. Call EndPopup() afterwards. ImGuiWindowFlags are forwarded to the window.
---@param label string 
---@param windowFlags integer? optional
---@return boolean value Return true if the popup is open, and you can start outputting to it.
function ImGui.BeginPopup(label, windowFlags) end 

--- They block normal mouse hovering detection (and therefore most mouse interactions) behind them. Their visibility state (~bool) is held internally instead of being held by the programmer as we are used to with regular Begin*() calls. The 3 properties above are related: we need to retain popup visibility state in the library because popups may be closed as any time. Popup identifiers are relative to the current ID stack, so OpenPopup and BeginPopup generally needs to be at the same level of the stack. BeginPopupModal(): block every interactions behind the window, cannot be closed by user, add a dimming background, has a title bar.
---@param label string 
---@param open boolean? optional
---@param windowFlags integer? optional
---@return boolean value Return true if the popup is open, and you can start outputting to it.
function ImGui.BeginPopupModal(label, open, windowFlags) end 

--- Only call EndPopup() if BeginPopupXXX() returns true!
function ImGui.EndPopup() end 

--- Set popup state to open. ImGuiPopupFlags are available for opening options. If not modal they can be closed by clicking anywhere outside them, or by pressing ESCAPE. Use ImGuiPopupFlags.NoOpenOverExistingPopup to avoid opening a popup if there's already one at the same level. This is equivalent to e.g. testing for !IsAnyPopupOpen() prior to OpenPopup(). Use IsWindowAppearing() after BeginPopup() to tell if a window just opened.
---@param label string 
---@param popupFlags integer? optional
function ImGui.OpenPopup(label, popupFlags) end 

--- Helper to open popup when clicked on last item. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)
---@param label string? optional
---@param popupFlags integer? optional
function ImGui.OpenPopupOnItemClick(label, popupFlags) end 

--- Manually close the popup we have begin-ed into. Use inside the BeginPopup()/EndPopup() scope to close manually. CloseCurrentPopup() is called by default by Selectable()/MenuItem() when activated.
function ImGui.CloseCurrentPopup() end 

--- Open+begin popup when clicked on last item. Use label nil to associate the popup to previous item. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking. They are convenient to easily create context menus, hence the name. Notice that BeginPopupContextXXX takes ImGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). We exceptionally default their flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the ImGuiPopupFlags_MouseButtonRight.
---@param label string? optional
---@param popupFlags integer? optional
---@return boolean value 
function ImGui.BeginPopupContextItem(label, popupFlags) end 

--- Open+begin popup when clicked on current window. OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking. They are convenient to easily create context menus, hence the name. Notice that BeginPopupContextXXX takes ImGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). We exceptionally default their flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the ImGuiPopupFlags_MouseButtonRight.
---@param label string? optional
---@param popupFlags integer? optional
---@return boolean value 
function ImGui.BeginPopupContextWindow(label, popupFlags) end 

--- Open+begin popup when clicked in void (where there are no windows). Open+begin popup when clicked on current window. OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking. They are convenient to easily create context menus, hence the name. Notice that BeginPopupContextXXX takes ImGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). We exceptionally default their flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the ImGuiPopupFlags_MouseButtonRight.
---@param label string? optional
---@param popupFlags integer? optional
---@return boolean value 
function ImGui.BeginPopupContextVoid(label, popupFlags) end 

--- Return true if the popup is open at the current BeginPopup() level of the popup stack. IsPopupOpen() with ImGuiPopupFlags_AnyPopupId: return true if any popup is open at the current BeginPopup() level of the popup stack. IsPopupOpen() with ImGuiPopupFlags_AnyPopupId + ImGuiPopupFlags_AnyPopupLevel: return true if any popup is open.
---@param label string 
---@param popupFlags integer? optional
---@return boolean isOpen return true if the popup is open.
function ImGui.IsPopupOpen(label, popupFlags) end 

--- Full-featured replacement for old Columns API. See ImGuiTableFlags_ and ImGuiTableColumnFlags_ enums for a description of available flags. In most situations you can use TableNextRow() + TableSetColumnIndex(N) to start appending into a column. If you are using tables as a sort of grid, where every column is holding the same type of contents, you may prefer using TableNextColumn() instead of TableNextRow() + TableSetColumnIndex(). TableNextColumn() will automatically wrap-around into the next row if needed. IMPORTANT: Comparatively to the old Columns() API, we need to call TableNextColumn() for the first column!
---@param str_id string 
---@param column integer 
---@param flags integer? optional
---@param sizeX number? optional
---@param sizeY number? optional
---@param innerWidth number? optional
function ImGui.BeginTable(str_id, column, flags, sizeX, sizeY, innerWidth) end 

--- End the ImGui table. Only call EndTable() if BeginTable() returns true!
function ImGui.EndTable() end 

--- Append into the first cell of a new row.
---@param flags integer? optional
---@param minRowHeight number? optional
function ImGui.TableNextRow(flags, minRowHeight) end 

--- Append into the next column (or first column of next row if currently in last column). Return true when column is visible.
---@return boolean result 
function ImGui.TableNextColumn() end 

--- Append into the specified column. Return true when column is visible.
---@param index integer 
---@return boolean result 
function ImGui.TableSetColumnIndex(index) end 

--- Use TableSetupColumn() to specify label, resizing policy, default width/weight, id, various other flags etc. The context menu can also be made available in columns body using ImGuiTableFlags_ContextMenuInBody.
---@param label string 
---@param flags integer? optional
---@param initWidthOrWeight number? optional
---@param userID integer? optional
function ImGui.TableSetupColumn(label, flags, initWidthOrWeight, userID) end 

--- Lock columns/rows so they stay visible when scrolled.
---@param cols integer 
---@param rows integer 
function ImGui.TableSetupScrollFreeze(cols, rows) end 

--- Submit a row with headers cells based on data provided to TableSetupColumn() + submit context menu. Use TableHeadersRow() to create a header row and automatically submit a TableHeader() for each column.
---@param label string 
function ImGui.TableHeader(label) end 

--- Get latest sort specs for the table (NULL if not sorting).  Lifetime: don't hold on this pointer over multiple frames or past any subsequent call to BeginTable()
---@return ImGuiTableSortSpecs specs 
function ImGui.TableGetSortSpecs() end 

--- Return number of columns (value passed to BeginTable)
---@return integer count 
function ImGui.TableGetColumnCount() end 

--- Return current column index.
---@return integer index 
function ImGui.TableGetColumnIndex() end 

--- Return current row index.
---@return integer index 
function ImGui.TableGetRowIndex() end 

--- Return "" if column didn't have a name declared by TableSetupColumn(). Pass -1 to use current column.
---@param index integer? optional
---@return string name 
function ImGui.TableGetColumnName(index) end 

--- Return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass -1 to use current column.
---@param index integer? optional
---@return integer flags 
function ImGui.TableGetColumnFlags(index) end 

--- Change user accessible enabled/disabled state of a column. Set to false to hide the column. User can use the context menu to change this themselves (right-click in headers, or right-click in columns body with ImGuiTableFlags_ContextMenuInBody)
---@param index integer 
---@param value boolean 
function ImGui.TableSetColumnEnabled(index, value) end 

--- Columns.
---@param count integer 
---@param label string? optional
---@param border boolean? optional
function ImGui.Columns(count, label, border) end 

--- Next column, defaults to current row or next row if the current row is finished.
function ImGui.NextColumn() end 

--- Get current column index.
---@return integer index 
function ImGui.GetColumnIndex() end 

--- Get column width (in pixels). Pass -1 to use current column.
---@param index integer? optional
---@return number width 
function ImGui.GetColumnWidth(index) end 

--- Set column width (in pixels). Pass -1 to use current column.
---@param index integer 
---@param width number 
function ImGui.SetColumnWidth(index, width) end 

--- Get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f.
---@param index integer 
---@param offset number 
function ImGui.GetColumnOffset(index, offset) end 

--- Set position of column line (in pixels, from the left side of the contents region). Pass -1 to use current column.
---@param index integer 
---@param offset number 
function ImGui.SetColumnOffset(index, offset) end 

--- Get Columns Count.
---@return integer count 
function ImGui.GetColumnsCount() end 

--- Create and append into a TabBar.
---@param label string 
---@param flags integer? optional
---@return boolean value 
function ImGui.BeginTabBar(label, flags) end 

--- Only call EndTabBar() if BeginTabBar() returns true!
function ImGui.EndTabBar() end 

--- Create a Tab. Returns true if the Tab is selected.
---@param label string 
---@param open boolean? optional
---@param flags integer? optional
---@return boolean value 
---@return boolean value2 
function ImGui.BeginTabItem(label, open, flags) end 

--- Only call EndTabItem() if BeginTabItem() returns true!
function ImGui.EndTabItem() end 

--- Notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.
---@param label string 
function ImGui.SetTabItemClosed(label) end 

--- Start logging to tty (stdout). All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
---@param autoOpenDepth integer? optional
function ImGui.LogToTTY(autoOpenDepth) end 

--- Start logging to file. All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
---@param autoOpenDepth integer 
---@param filename string? optional
function ImGui.LogToFile(autoOpenDepth, filename) end 

--- Start logging to OS clipboard. All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
---@param autoOpenDepth integer? optional
function ImGui.LogToClipboard(autoOpenDepth) end 

--- Stop logging (close file, etc.).
function ImGui.LogFinish() end 

--- Helper to display buttons for logging to tty/file/clipboard.
function ImGui.LogButtons() end 

--- Pass text data straight to log (without being displayed).
---@param text string 
function ImGui.LogText(text) end 

--- Mouse hovering is affected by PushClipRect() calls.
---@param minX number 
---@param minY number 
---@param maxX number 
---@param maxY number 
---@param intersect boolean 
function ImGui.PushClipRect(minX, minY, maxX, maxY, intersect) end 

--- Pop Clip Rect.
function ImGui.PopClipRect() end 

--- Make last item the default focused item of a window. Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHereY()" when applicable to signify "this is the default item".
function ImGui.SetItemDefaultFocus() end 

--- Focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.
---@param offset integer? optional
function ImGui.SetKeyboardFocusHere(offset) end 

--- Allow next item to be overlapped by a subsequent item. Useful with invisible buttons, selectable, treenode covering an area where subsequent items may need to be added. Note that both Selectable() and TreeNode() have dedicated flags doing this.
function ImGui.SetNextItemAllowOverlap() end 

--- Is the last item hovered? (and usable, aka not blocked by a popup, etc.). See ImGuiHoveredFlags for more options.
---@param flags integer? optional
---@return boolean result 
function ImGui.IsItemHovered(flags) end 

--- Is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
---@return boolean result 
function ImGui.IsItemActive() end 

--- Is the last item focused for keyboard/gamepad navigation?
---@return boolean result 
function ImGui.IsItemFocused() end 

--- Is the last item hovered and mouse clicked on? (**)  == IsMouseClicked(mouse_button) && IsItemHovered()Important. (**) this it NOT equivalent to the behavior of e.g. Button().
---@param flags integer? optional
---@return boolean result 
function ImGui.IsItemClicked(flags) end 

--- Is the last item visible? (items may be out of sight because of clipping/scrolling)
---@return boolean result 
function ImGui.IsItemVisible() end 

--- Did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets
---@return boolean result 
function ImGui.IsItemEdited() end 

--- Was the last item just made active (item was previously inactive).
---@return boolean result 
function ImGui.IsItemActivated() end 

--- Was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
---@return boolean result 
function ImGui.IsItemDeactivated() end 

--- Was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
---@return boolean result 
function ImGui.IsItemDeactivatedAfterEdit() end 

--- Was the last item open state toggled? set by TreeNode().
---@return boolean result 
function ImGui.IsItemToggledOpen() end 

--- Is any item hovered?
---@return boolean result 
function ImGui.IsAnyItemHovered() end 

--- Is any item active?
---@return boolean result 
function ImGui.IsAnyItemActive() end 

--- Is any item focused?
---@return boolean result 
function ImGui.IsAnyItemFocused() end 

--- Get ID of last item (~~ often same ImGui::GetID(label) beforehand)
---@return integer id 
function ImGui.GetItemID() end 

--- Get upper-left bounding rectangle of the last item (screen space).
---@return number x 
---@return number y 
function ImGui.GetItemRectMin() end 

--- get lower-right bounding rectangle of the last item (screen space).
---@return number x 
---@return number y 
function ImGui.GetItemRectMax() end 

--- Get size of last item.
---@return number x 
---@return number y 
function ImGui.GetItemRectSize() end 

--- Test if rectangle (of given size, starting from cursor position) is visible / not clipped.
---@param x number 
---@param y number 
---@param maxX number? optional
---@param maxY number? optional
---@return boolean result 
function ImGui.IsRectVisible(x, y, maxX, maxY) end 

--- Get global imgui time. incremented by io.DeltaTime every frame..
---@return number time 
function ImGui.GetTime() end 

--- Get global imgui frame count. Incremented by 1 every frame.
---@return integer count 
function ImGui.GetFrameCount() end 

--- Get a string corresponding to the enum value (for display, saving, etc.).
---@param imGuiCol integer use enum ImGuiCol
---@return string name 
function ImGui.GetStyleColorName(imGuiCol) end 

--- Calculate text size.
---@param text string 
---@param textEnd string? optional
---@param hideAfterDoubleHash boolean? optional
---@param wrapWidth number? optional
---@return number sizeX 
---@return number sizeY 
function ImGui.CalcTextSize(text, textEnd, hideAfterDoubleHash, wrapWidth) end 

--- Convert Color.
---@param R number 
---@param G number 
---@param B number 
---@return number H 
---@return number S 
---@return number V 
function ImGui.ColorConvertRGBtoHSV(R, G, B) end 

--- Convert Color.
---@param H number 
---@param S number 
---@param V number 
---@return number R 
---@return number G 
---@return number B 
function ImGui.ColorConvertHSVtoRGB(H, S, V) end 

--- Is key down.
---@param key integer 
---@return boolean result 
function ImGui.IsKeyDown(key) end 

--- Is key pressed.
---@param key integer 
---@param doRepeat boolean? optional
---@return boolean result 
function ImGui.IsKeyPressed(key, doRepeat) end 

--- Is key released.
---@param key integer 
---@return boolean result 
function ImGui.IsKeyReleased(key) end 

--- Uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
---@param key integer 
---@param repeatDelay number 
---@param rate number 
---@return integer amount 
function ImGui.GetKeyPressedAmount(key, repeatDelay, rate) end 

--- Override io.WantCaptureKeyboard flag next frame (said flag is left for your application to handle, typically when true it instructs your app to ignore inputs). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard"; after the next NewFrame() call.
---@param wantCaptureKeyboard boolean 
function ImGui.SetNextFrameWantCaptureKeyboard(wantCaptureKeyboard) end 

--- Is mouse button held?
---@param button integer use MouseButton enum
---@return boolean result 
function ImGui.IsMouseDown(button) end 

--- Did mouse button click? (went from !Down to Down)
---@param button integer use MouseButton enum
---@param doRepeat boolean? optional
---@return boolean result 
function ImGui.IsMouseClicked(button, doRepeat) end 

--- Did mouse button released? (went from Down to !Down)
---@param button integer use MouseButton enum
---@return boolean result 
function ImGui.IsMouseReleased(button) end 

--- Did mouse button double-clicked? (note that a double-click will also report IsMouseClicked() == true).
---@param button integer use MouseButton enum
---@return boolean result 
function ImGui.IsMouseDoubleClicked(button) end 

--- Return the number of successive mouse-clicks at the time where a click happen (otherwise 0).
---@param button integer use MouseButton enum
---@return integer count 
function ImGui.GetMouseClickedCount(button) end 

--- Is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
---@param minX number 
---@param minY number 
---@param maxX number 
---@param maxY number 
---@param clip boolean? optional
---@return boolean result 
function ImGui.IsMouseHoveringRect(minX, minY, maxX, maxY, clip) end 

--- Is any mouse button held?
---@return boolean result 
function ImGui.IsAnyMouseDown() end 

--- Get mouse pos.
---@return number x 
---@return number y 
function ImGui.GetMousePos() end 

--- Retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves).
---@return number x 
---@return number y 
function ImGui.GetMousePosOnOpeningCurrentPopup() end 

--- Is mouse dragging?
---@param button integer use MouseButton enum
---@param lockThreshold number? optional
function ImGui.IsMouseDragging(button, lockThreshold) end 

--- Return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
---@param button integer? use MouseButton enum optional
---@param lockThreshold number? optional
---@return number x 
---@return number y 
function ImGui.GetMouseDragDelta(button, lockThreshold) end 

--- Reset mouse drag delta.
---@param button integer? use MouseButton enum optional
function ImGui.ResetMouseDragDelta(button) end 

--- Get desired cursor type, reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
---@return any mouseCursor 
function ImGui.GetMouseCursor() end 

--- Set desired cursor type
---@param mouseCursor any 
function ImGui.SetMouseCursor(mouseCursor) end 

--- Override io.WantCaptureMouse flag next frame (said flag is left for your application to handle, typical when true it instucts your app to ignore inputs). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse;" after the next NewFrame() call.
---@param wantCaptureMouse boolean 
function ImGui.SetNextFrameWantCaptureMouse(wantCaptureMouse) end 

--- Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
---@return string text 
function ImGui.GetClipboardText() end 

--- Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
---@param text string 
function ImGui.SetClipboardText(text) end 

--- Access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame!
---@return ImGuiStyle style 
function ImGui.GetStyle() end 

--- Access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags).
---@return ImGuiIO io 
function ImGui.GetIO() end 

---Basic ImVec4 table
---@class ImVec4
ImVec4 = { 

    ---@type number
    x = nil,

    ---@type number
    y = nil,

    ---@type number
    z = nil,

    ---@type number
    w = nil,

}

---Basic ImVec2 table
---@class ImVec2
ImVec2 = { 

    ---@type number
    x = nil,

    ---@type number
    y = nil,

}

---Basic ImGuiStyle table
---@class ImGuiStyle
ImGuiStyle = { 

    ---Global alpha applies to everything in Dear ImGui.
    ---@type number
    Alpha = nil,

    ---Additional alpha multiplier applied by BeginDisabled(). Multiply over current value of Alpha.
    ---@type number
    DisabledAlpha = nil,

    ---Padding within a window.
    ---@type ImVec2
    WindowPadding = nil,

    ---Radius of window corners rounding. Set to 0.0f to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.
    ---@type number
    WindowRounding = nil,

    ---Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ---@type number
    WindowBorderSize = nil,

    ---Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().
    ---@type ImVec2
    WindowMinSize = nil,

    ---Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
    ---@type ImVec2
    WindowTitleAlign = nil,

    ---Aside of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir.Left.
    ---@type integer
    WindowMenuButtonPosition = nil,

    ---Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
    ---@type number
    ChildRounding = nil,

    ---Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ---@type number
    ChildBorderSize = nil,

    ---Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
    ---@type number
    PopupRounding = nil,

    ---Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ---@type number
    PopupBorderSize = nil,

    ---Padding within a framed rectangle (used by most widgets).
    ---@type ImVec2
    FramePadding = nil,

    ---Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    ---@type number
    FrameRounding = nil,

    ---Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ---@type number
    FrameBorderSize = nil,

    ---Horizontal and vertical spacing between widgets/lines.
    ---@type ImVec2
    ItemSpacing = nil,

    ---Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
    ---@type ImVec2
    ItemInnerSpacing = nil,

    ---Padding within a table cell. CellPadding.y may be altered between different rows.
    ---@type ImVec2
    CellPadding = nil,

    ---Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    ---@type ImVec2
    TouchExtraPadding = nil,

    ---Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    ---@type number
    IndentSpacing = nil,

    ---Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
    ---@type number
    ColumnsMinSpacing = nil,

    ---Width of the vertical scrollbar, Height of the horizontal scrollbar.
    ---@type number
    ScrollbarSize = nil,

    ---Radius of grab corners for scrollbar.
    ---@type number
    ScrollbarRounding = nil,

    ---Minimum width/height of a grab box for slider/scrollbar.
    ---@type number
    GrabMinSize = nil,

    ---Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    ---@type number
    GrabRounding = nil,

    ---The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.
    ---@type number
    LogSliderDeadzone = nil,

    ---Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
    ---@type number
    TabRounding = nil,

    ---Thickness of border around tabs.
    ---@type number
    TabBorderSize = nil,

    ---Minimum width for close button to appear on an unselected tab when hovered. Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected.
    ---@type number
    TabMinWidthForCloseButton = nil,

    ---Thickness of tab-bar separator, which takes on the tab active color to denote focus.
    ---@type number
    TabBarBorderSize = nil,

    ---Angle of angled headers (supported values range from -50.0f degrees to +50.0f degrees).
    ---@type number
    TableAngledHeadersAngle = nil,

    ---Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir.Right.
    ---@type integer
    ColorButtonPosition = nil,

    ---Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
    ---@type ImVec2
    ButtonTextAlign = nil,

    ---Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.
    ---@type ImVec2
    SelectableTextAlign = nil,

    ---Thickkness of border in SeparatorText()
    ---@type number
    SeparatorTextBorderSize = nil,

    ---Alignment of text within the separator. Defaults to (0.0f, 0.5f) (left aligned, center).
    ---@type ImVec2
    SeparatorTextAlign = nil,

    ---Horizontal offset of text from each edge of the separator + spacing on other axis. Generally small values. .y is recommended to be == FramePadding.y.
    ---@type ImVec2
    SeparatorTextPadding = nil,

    ---Window position are clamped to be visible within the display area or monitors by at least this amount. Only applies to regular windows.
    ---@type ImVec2
    DisplayWindowPadding = nil,

    ---If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
    ---@type ImVec2
    DisplaySafeAreaPadding = nil,

    ---Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
    ---@type number
    MouseCursorScale = nil,

    ---Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
    ---@type boolean
    AntiAliasedLines = nil,

    ---Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to ImDrawList).
    ---@type boolean
    AntiAliasedLinesUseTex = nil,

    ---Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
    ---@type boolean
    AntiAliasedFill = nil,

    ---Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    ---@type number
    CurveTessellationTol = nil,

    ---Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
    ---@type number
    CircleTessellationMaxError = nil,

    ---Maximum: 52
    ---@type table<integer, ImVec4>
    Colors = nil,

    ---Delay for IsItemHovered(ImGuiHoveredFlags_Stationary). Time required to consider mouse stationary.
    ---@type number
    HoverStationaryDelay = nil,

    ---Delay for IsItemHovered(ImGuiHoveredFlags_DelayShort). Usually used along with HoverStationaryDelay.
    ---@type number
    HoverDelayShort = nil,

    ---Delay for IsItemHovered(ImGuiHoveredFlags_DelayNormal).
    ---@type number
    HoverDelayNormal = nil,

    ---Default flags when using IsItemHovered(ImGuiHoveredFlags_ForTooltip) or BeginItemTooltip()/SetItemTooltip() while using mouse.
    ---@type integer
    HoverFlagsForTooltipMouse = nil,

    ---Default flags when using IsItemHovered(ImGuiHoveredFlags_ForTooltip) or BeginItemTooltip()/SetItemTooltip() while using keyboard/gamepad.
    ---@type integer
    HoverFlagsForTooltipNav = nil,

}

---Basic ImFont table
---@class ImFont
ImFont = { 

}

---Basic ImFontAtlas table
---@class ImFontAtlas
ImFontAtlas = { 

}

--- Access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags).
---@param filename string 
---@param sizePixels number 
---@return ImGuiIO io 
function ImFontAtlas.AddFontFromFileTTF(filename, sizePixels) end 

---Basic ImGuiIO table
---@class ImGuiIO
ImGuiIO = { 

    ---@type ImFontAtlas
    Fonts = nil,

    ---Main display size, in pixels (generally == GetMainViewport()->Size). May change every frame.
    ---@type ImVec2
    DisplaySize = nil,

    ---Time elapsed since last frame, in seconds. May change every frame.
    ---@type number
    DeltaTime = nil,

    ---Minimum time between saving positions/sizes to .ini file, in seconds.
    ---@type number
    IniSavingRate = nil,

    ---Path to .ini file (important: default "imgui.ini" is relative to current working dir!). Set NULL to disable automatic .ini loading/saving or if you want to manually call LoadIniSettingsXXX() / SaveIniSettingsXXX() functions.
    ---@type string
    IniFilename = nil,

    ---Path to .log file (default parameter to ImGui::LogToFile when no file is specified).
    ---@type string
    LogFilename = nil,

    ---Global scale all fonts
    ---@type number
    FontGlobalScale = nil,

    ---Allow user scaling text of individual window with CTRL+Wheel.
    ---@type boolean
    FontAllowUserScaling = nil,

    ---Font to use on NewFrame(). Use NULL to uses Fonts->Fonts[0].
    ---@type ImFont
    FontDefault = nil,

    ---For retina display or other situations where window coordinates are different from framebuffer coordinates. This generally ends up in ImDrawData::FramebufferScale.
    ---@type ImVec2
    DisplayFramebufferScale = nil,

    ---Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor). Cannot be easily renamed to 'io.ConfigXXX' because this is frequently used by backend implementations.
    ---@type boolean
    MouseDrawCursor = nil,

    ---Time for a double-click, in seconds.
    ---@type number
    MouseDoubleClickTime = nil,

    ---Distance threshold to stay in to validate a double-click, in pixels.
    ---@type number
    MouseDoubleClickMaxDist = nil,

    ---Distance threshold before considering we are dragging.
    ---@type number
    MouseDragThreshold = nil,

    ---When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
    ---@type number
    KeyRepeatDelay = nil,

    ---When holding a key/button, rate at which it repeats, in seconds.
    ---@type number
    KeyRepeatRate = nil,

    ---Mouse position, in pixels. Set to ImVec2(-FLT_MAX, -FLT_MAX) if mouse is unavailable (on another screen, etc.)
    ---@type ImVec2
    MousePos = nil,

    ---Mouse wheel Vertical: 1 unit scrolls about 5 lines text. >0 scrolls Up, <0 scrolls Down. Hold SHIFT to turn vertical scroll into horizontal scroll.
    ---@type number
    MouseWheel = nil,

    ---Keyboard modifier down: Control
    ---@type boolean
    KeyCtrl = nil,

    ---Keyboard modifier down: Shift
    ---@type boolean
    KeyShift = nil,

    ---Keyboard modifier down: Alt
    ---@type boolean
    KeyAlt = nil,

    ---Keyboard modifier down: Cmd/Super/Windows
    ---@type boolean
    KeySuper = nil,

    ---Alternative to WantCaptureMouse: (WantCaptureMouse == true && WantCaptureMouseUnlessPopupClose == false) when a click over void is expected to close a popup.
    ---@type boolean
    WantCaptureMouseUnlessPopupClose = nil,

}

---Basic ImGuiTableSortSpecs table
---@class ImGuiTableSortSpecs
ImGuiTableSortSpecs = { 

    ---@type integer
    SpecsCount = nil,

    ---@type boolean
    SpecsDirty = nil,

}

