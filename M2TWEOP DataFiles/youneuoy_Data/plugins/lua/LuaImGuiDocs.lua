---@diagnostic disable: missing-return, lowercase-global
---Enum with a list of ImGui Window Flags.
---@class ImGuiWindowFlags
ImGuiWindowFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    NoTitleBar = nil,

    ---@type integer
    NoResize = nil,

    ---@type integer
    NoMove = nil,

    ---@type integer
    NoScrollbar = nil,

    ---@type integer
    NoScrollWithMouse = nil,

    ---@type integer
    NoCollapse = nil,

    ---@type integer
    AlwaysAutoResize = nil,

    ---@type integer
    NoBackground = nil,

    ---@type integer
    NoSavedSettings = nil,

    ---@type integer
    NoMouseInputs = nil,

    ---@type integer
    MenuBar = nil,

    ---@type integer
    HorizontalScrollbar = nil,

    ---@type integer
    NoFocusOnAppearing = nil,

    ---@type integer
    NoBringToFrontOnFocus = nil,

    ---@type integer
    AlwaysVerticalScrollbar = nil,

    ---@type integer
    AlwaysHorizontalScrollbar = nil,

    ---@type integer
    AlwaysUseWindowPadding = nil,

    ---@type integer
    NoNavInputs = nil,

    ---@type integer
    NoNavFocus = nil,

    ---@type integer
    UnsavedDocument = nil,

    ---@type integer
    NoDocking = nil,

    ---@type integer
    DockNodeHost = nil,

    ---@type integer
    NoClose = nil,

    ---@type integer
    NoDockTab = nil,

    ---@type integer
    NoNav = nil,

    ---@type integer
    NoDecoration = nil,

    ---@type integer
    NoInputs = nil,

    ---@type integer
    NavFlattened = nil,

    ---@type integer
    ChildWindow = nil,

    ---@type integer
    Tooltip = nil,

    ---@type integer
    Popup = nil,

    ---@type integer
    Modal = nil,

    ---@type integer
    ChildMenu = nil,

}

---Enum with a list of ImGui Focused Flags.
---@class ImGuiFocusedFlags
ImGuiFocusedFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    ChildWindows = nil,

    ---@type integer
    RootWindow = nil,

    ---@type integer
    AnyWindow = nil,

    ---@type integer
    RootAndChildWindows = nil,

}

---Enum with a list of ImGui Hovered Flags.
---@class ImGuiHoveredFlags
ImGuiHoveredFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    ChildWindows = nil,

    ---@type integer
    RootWindow = nil,

    ---@type integer
    AnyWindow = nil,

    ---@type integer
    AllowWhenBlockedByPopup = nil,

    ---@type integer
    AllowWhenBlockedByActiveItem = nil,

    ---@type integer
    AllowWhenOverlapped = nil,

    ---@type integer
    AllowWhenDisabled = nil,

    ---@type integer
    RectOnly = nil,

    ---@type integer
    RootAndChildWindows = nil,

}

---Enum with a list of ImGui Conditions.
---@class ImGuiCond
ImGuiCond = { 

    ---@type integer
    None = nil,

    ---@type integer
    Always = nil,

    ---@type integer
    Once = nil,

    ---@type integer
    FirstUseEver = nil,

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

    ---@type integer
    WindowBg = nil,

    ---@type integer
    ChildBg = nil,

    ---@type integer
    PopupBg = nil,

    ---@type integer
    Border = nil,

    ---@type integer
    BorderShadow = nil,

    ---@type integer
    FrameBg = nil,

    ---@type integer
    FrameBgHovered = nil,

    ---@type integer
    FrameBgActive = nil,

    ---@type integer
    TitleBg = nil,

    ---@type integer
    TitleBgActive = nil,

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

    ---@type integer
    ResizeGrip = nil,

    ---@type integer
    ResizeGripHovered = nil,

    ---@type integer
    ResizeGripActive = nil,

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
    DockingPreview = nil,

    ---@type integer
    DockingEmptyBg = nil,

    ---@type integer
    PlotLines = nil,

    ---@type integer
    PlotLinesHovered = nil,

    ---@type integer
    PlotHistogram = nil,

    ---@type integer
    PlotHistogramHovered = nil,

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

    ---@type integer
    Alpha = nil,

    ---@type integer
    WindowPadding = nil,

    ---@type integer
    WindowRounding = nil,

    ---@type integer
    WindowBorderSize = nil,

    ---@type integer
    WindowMinSize = nil,

    ---@type integer
    WindowTitleAlign = nil,

    ---@type integer
    ChildRounding = nil,

    ---@type integer
    ChildBorderSize = nil,

    ---@type integer
    PopupRounding = nil,

    ---@type integer
    PopupBorderSize = nil,

    ---@type integer
    FramePadding = nil,

    ---@type integer
    FrameRounding = nil,

    ---@type integer
    FrameBorderSize = nil,

    ---@type integer
    ItemSpacing = nil,

    ---@type integer
    ItemInnerSpacing = nil,

    ---@type integer
    IndentSpacing = nil,

    ---@type integer
    ScrollbarSize = nil,

    ---@type integer
    ScrollbarRounding = nil,

    ---@type integer
    GrabMinSize = nil,

    ---@type integer
    GrabRounding = nil,

    ---@type integer
    TabRounding = nil,

    ---@type integer
    SelectableTextAlign = nil,

    ---@type integer
    ButtonTextAlign = nil,

    ---@type integer
    COUNT = nil,

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

---Enum with a list of ImGui Combo Flags.
---@class ImGuiComboFlags
ImGuiComboFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    PopupAlignLeft = nil,

    ---@type integer
    HeightSmall = nil,

    ---@type integer
    HeightRegular = nil,

    ---@type integer
    HeightLarge = nil,

    ---@type integer
    HeightLargest = nil,

    ---@type integer
    NoArrowButton = nil,

    ---@type integer
    NoPreview = nil,

    ---@type integer
    HeightMask = nil,

}

---Enum with a list of ImGui Input Text Flags.
---@class ImGuiInputTextFlags
ImGuiInputTextFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    CharsDecimal = nil,

    ---@type integer
    CharsHexadecimal = nil,

    ---@type integer
    CharsUppercase = nil,

    ---@type integer
    CharsNoBlank = nil,

    ---@type integer
    AutoSelectAll = nil,

    ---@type integer
    EnterReturnsTrue = nil,

    ---@type integer
    CallbackCompletion = nil,

    ---@type integer
    CallbackHistory = nil,

    ---@type integer
    CallbackAlways = nil,

    ---@type integer
    CallbackCharFilter = nil,

    ---@type integer
    AllowTabInput = nil,

    ---@type integer
    CtrlEnterForNewLine = nil,

    ---@type integer
    NoHorizontalScroll = nil,

    ---@type integer
    AlwaysInsertMode = nil,

    ---@type integer
    ReadOnly = nil,

    ---@type integer
    Password = nil,

    ---@type integer
    NoUndoRedo = nil,

    ---@type integer
    CharsScientific = nil,

    ---@type integer
    CallbackResize = nil,

    ---@type integer
    Multiline = nil,

    ---@type integer
    NoMarkEdited = nil,

}

---Enum with a list of ImGui Color Edit Flags.
---@class ImGuiColorEditFlags
ImGuiColorEditFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    NoAlpha = nil,

    ---@type integer
    NoPicker = nil,

    ---@type integer
    NoOptions = nil,

    ---@type integer
    NoSmallPreview = nil,

    ---@type integer
    NoInputs = nil,

    ---@type integer
    NoTooltip = nil,

    ---@type integer
    NoLabel = nil,

    ---@type integer
    NoSidePreview = nil,

    ---@type integer
    NoDragDrop = nil,

    ---@type integer
    NoBorder = nil,

    ---@type integer
    AlphaBar = nil,

    ---@type integer
    AlphaPreview = nil,

    ---@type integer
    AlphaPreviewHalf = nil,

    ---@type integer
    HDR = nil,

    ---@type integer
    DisplayRGB = nil,

    ---@type integer
    DisplayHSV = nil,

    ---@type integer
    DisplayHex = nil,

    ---@type integer
    Uint8 = nil,

    ---@type integer
    Float = nil,

    ---@type integer
    PickerHueBar = nil,

    ---@type integer
    PickerHueWheel = nil,

    ---@type integer
    InputRGB = nil,

    ---@type integer
    InputHSV = nil,

    ---@type integer
    RGB = nil,

}

---Enum with a list of ImGui Tree Node Flags.
---@class ImGuiTreeNodeFlags
ImGuiTreeNodeFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    Selected = nil,

    ---@type integer
    Framed = nil,

    ---@type integer
    AllowItemOverlap = nil,

    ---@type integer
    NoTreePushOnOpen = nil,

    ---@type integer
    NoAutoOpenOnLog = nil,

    ---@type integer
    DefaultOpen = nil,

    ---@type integer
    OpenOnDoubleClick = nil,

    ---@type integer
    OpenOnArrow = nil,

    ---@type integer
    Leaf = nil,

    ---@type integer
    Bullet = nil,

    ---@type integer
    FramePadding = nil,

    ---@type integer
    SpanAvailWidth = nil,

    ---@type integer
    SpanFullWidth = nil,

    ---@type integer
    NavLeftJumpsBackHere = nil,

    ---@type integer
    CollapsingHeader = nil,

}

---Enum with a list of ImGui Selectable Flags.
---@class ImGuiSelectableFlags
ImGuiSelectableFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    DontClosePopups = nil,

    ---@type integer
    SpanAllColumns = nil,

    ---@type integer
    AllowDoubleClick = nil,

    ---@type integer
    Disabled = nil,

    ---@type integer
    AllowItemOverlap = nil,

}

---Enum with a list of ImGui Popup Flags.
---@class ImGuiPopupFlags
ImGuiPopupFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    MouseButtonLeft = nil,

    ---@type integer
    MouseButtonRight = nil,

    ---@type integer
    MouseButtonMiddle = nil,

    ---@type integer
    MouseButtonMask_ = nil,

    ---@type integer
    MouseButtonDefault_ = nil,

    ---@type integer
    NoOpenOverExistingPopup = nil,

    ---@type integer
    NoOpenOverItems = nil,

    ---@type integer
    AnyPopupId = nil,

    ---@type integer
    AnyPopupLevel = nil,

    ---@type integer
    AnyPopup = nil,

}

---Enum with a list of ImGui TabBar Flags.
---@class ImGuiTabBarFlags
ImGuiTabBarFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    Reorderable = nil,

    ---@type integer
    AutoSelectNewTabs = nil,

    ---@type integer
    TabListPopupButton = nil,

    ---@type integer
    NoCloseWithMiddleMouseButton = nil,

    ---@type integer
    NoTabListScrollingButtons = nil,

    ---@type integer
    NoTooltip = nil,

    ---@type integer
    FittingPolicyResizeDown = nil,

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

    ---@type integer
    UnsavedDocument = nil,

    ---@type integer
    SetSelected = nil,

    ---@type integer
    NoCloseWithMiddleMouseButton = nil,

    ---@type integer
    NoTooltip = nil,

}

---Enum with a list of ImGui DockNode Flags.
---@class ImGuiDockNodeFlags
ImGuiDockNodeFlags = { 

    ---@type integer
    None = nil,

    ---@type integer
    KeepAliveOnly = nil,

    ---@type integer
    NoDockingInCentralNode = nil,

    ---@type integer
    PassthruCentralNode = nil,

    ---@type integer
    NoSplit = nil,

    ---@type integer
    NoResize = nil,

    ---@type integer
    AutoHideTabBar = nil,

}

---Enum with a list of ImGui MouseButton.
---@class ImGuiMouseButton
ImGuiMouseButton = { 

    ---@type integer
    ImGuiMouseButton_Left = nil,

    ---@type integer
    ImGuiMouseButton_Right = nil,

    ---@type integer
    ImGuiMouseButton_Middle = nil,

    ---@type integer
    ImGuiMouseButton_COUNT = nil,

}

---Enum with a list of ImGui Key.
---@class ImGuiKey
ImGuiKey = { 

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
    KeyPadEnter = nil,

    ---@type integer
    One = nil,

    ---@type integer
    Two = nil,

    ---@type integer
    Three = nil,

    ---@type integer
    Four = nil,

    ---@type integer
    Five = nil,

    ---@type integer
    Six = nil,

    ---@type integer
    Seven = nil,

    ---@type integer
    Eight = nil,

    ---@type integer
    Nine = nil,

    ---@type integer
    Zero = nil,

    ---@type integer
    Tilde = nil,

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
    LeftCtrl = nil,

    ---@type integer
    LeftShift = nil,

    ---@type integer
    LeftAlt = nil,

    ---@type integer
    CapsLock = nil,

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
    COUNT = nil,

}

---Enum with a list of ImGui Mouse Cursor.
---@class ImGuiMouseCursor
ImGuiMouseCursor = { 

    ---@type integer
    None = nil,

    ---@type integer
    Arrow = nil,

    ---@type integer
    TextInput = nil,

    ---@type integer
    ResizeAll = nil,

    ---@type integer
    ResizeNS = nil,

    ---@type integer
    ResizeEW = nil,

    ---@type integer
    ResizeNESW = nil,

    ---@type integer
    ResizeNWSE = nil,

    ---@type integer
    Hand = nil,

    ---@type integer
    NotAllowed = nil,

    ---@type integer
    COUNT = nil,

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

--- Is current window hovered (and typically: not blocked by a popup/modal)?.
---@param hoveredFlags integer? optional
---@return boolean collapsed 
function ImGui.IsWindowHovered(hoveredFlags) end 

--- Get window dpi scale.
---@return number scale 
function ImGui.GetWindowDpiScale() end 

--- Get current window position in screen space.
---@return number posX 
---@return number posY 
function ImGui.GetWindowPos() end 

--- Get current window size.
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

--- Pop style color.
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
---@param allowFocus boolean 
function ImGui.PushAllowKeyboardFocus(allowFocus) end 

--- Pop AllowKeyboardFocus
function ImGui.PopAllowKeyboardFocus() end 

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

--- This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes. Open a framed scrolling region.
---@param label string 
---@param currentItem integer 
---@param items table 
---@param itemCount integer 
---@param heightInItems integer? optional
function ImGui.ListBox(label, currentItem, items, itemCount, heightInItems) end 

--- This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes.
---@param sizeXorCount number 
---@param sizeYorHeight number? optional if first param is count
function ImGui.ListBoxHeader(sizeXorCount, sizeYorHeight) end 

--- This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes.
function ImGui.ListBoxFooter() end 

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
function ImGui.BeginTooltip() end 

--- Tooltip are windows following the mouse. They do not take focus away.
function ImGui.EndTooltip() end 

--- Tooltip are windows following the mouse. They do not take focus away. Set a text-only tooltip, typically use with IsItemHovered(). Override any previous call to SetTooltip().
---@param text string 
function ImGui.SetTooltip(text) end 

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
---@return boolean value Return true if the popup is open, and you can start outputting to it.
function ImGui.OpenPopup(label, popupFlags) end 

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

--- Allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.
function ImGui.SetItemAllowOverlap() end 

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

--- Helper to create a child window / scrolling region that looks like a normal widget frame
---@param id integer 
---@param sizeX number 
---@param sizeY number 
---@param flags integer? optional
---@return boolean result 
function ImGui.BeginChildFrame(id, sizeX, sizeY, flags) end 

--- Always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)
function ImGui.EndChildFrame() end 

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

--- Get key index.
---@param key integer use enum
---@return integer index 
function ImGui.GetKeyIndex(key) end 

--- Is key down.
---@param key integer use getKeyIndex
---@return boolean result 
function ImGui.IsKeyDown(key) end 

--- Is key pressed.
---@param key integer use getKeyIndex
---@param doRepeat boolean? optional
---@return boolean result 
function ImGui.IsKeyPressed(key, doRepeat) end 

--- Is key released.
---@param key integer use getKeyIndex
---@return boolean result 
function ImGui.IsKeyReleased(key) end 

--- Attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after the next NewFrame() call.
---@param wantCaptureKeyboardValue boolean? optional
function ImGui.CaptureKeyboardFromApp(wantCaptureKeyboardValue) end 

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

--- Attention: misleading name! manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle). This is equivalent to setting "io.WantCaptureMouse = wantCaptureMouseValue;" after the next NewFrame() call.
---@param wantCaptureMouseValue boolean? optional
function ImGui.CaptureMouseFromApp(wantCaptureMouseValue) end 

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

    ---Maximum: 54
    ---@type table<integer, ImVec4>
    colors = nil,

    ---@type number
    PopupRounding = nil,

    ---@type ImVec2
    WindowPadding = nil,

    ---@type ImVec2
    FramePadding = nil,

    ---@type ImVec2
    ItemSpacing = nil,

    ---@type number
    ScrollbarSize = nil,

    ---@type number
    WindowBorderSize = nil,

    ---@type number
    ChildBorderSize = nil,

    ---@type number
    PopupBorderSize = nil,

    ---@type number
    FrameBorderSize = nil,

    ---@type number
    WindowRounding = nil,

    ---@type number
    ChildRounding = nil,

    ---@type number
    FrameRounding = nil,

    ---@type number
    ScrollbarRounding = nil,

    ---@type number
    GrabRounding = nil,

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

    ---Maximum: 512
    ---@type table<integer, number>
    KeysDownDuration = nil,

    ---@type ImFontAtlas
    Fonts = nil,

}

