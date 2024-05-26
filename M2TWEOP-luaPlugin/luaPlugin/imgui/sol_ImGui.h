///
//@module LuaPluginImGui
//@author youneuoy
//@license GPL-3.0
#pragma once
#include "imgui.h"
#include "../lua/sol.hpp"
namespace sol_ImGui
{
	inline ImGuiIO* GetIO();
	// Windows
	inline bool Begin(const std::string& name);
	inline std::tuple<bool, bool> Begin(const std::string& name, bool open);
	inline std::tuple<bool, bool> Begin(const std::string& name, bool open, int flags);
	inline void End();

	// Child Windows
	inline bool BeginChild(const std::string& name);
	inline bool BeginChild(const std::string& name, float sizeX);
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY);
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY, int childFlags);
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY, int childFlags, int windowFlags);

	// Windows Utilities
	inline bool IsWindowAppearing();
	inline bool IsWindowCollapsed();
	inline bool IsWindowFocused();
	inline bool IsWindowFocused(int flags);
	inline bool IsWindowHovered();
	inline bool IsWindowHovered(int flags);
	inline ImDrawList* GetWindowDrawList();
#if defined(IMGUI_HAS_DOCK)
	inline float GetWindowDpiScale();
	inline ImGuiViewport* GetWindowViewport();
#endif

	inline std::tuple<float, float> GetWindowPos();
	inline std::tuple<float, float> GetWindowSize();
	inline float GetWindowWidth();
	inline float GetWindowHeight();

	// Window manipulation
	// Prefer using SetNext...
	inline void SetNextWindowPos(float posX, float posY);
	inline void SetNextWindowPos(float posX, float posY, int cond);
	inline void SetNextWindowPos(float posX, float posY, int cond, float pivotX, float pivotY);
	inline void SetNextWindowSize(float sizeX, float sizeY);
	inline void SetNextWindowSize(float sizeX, float sizeY, int cond);
	inline void SetNextWindowSizeConstraints(float minX, float minY, float maxX, float maxY);
	inline void SetNextWindowContentSize(float sizeX, float sizeY);
	inline void SetNextWindowCollapsed(bool collapsed);
	inline void SetNextWindowCollapsed(bool collapsed, int cond);
	inline void SetNextWindowFocus();
	inline void SetNextWindowBgAlpha(float alpha);
	inline void SetWindowPos(float posX, float posY);
	inline void SetWindowPos(float posX, float posY, int cond);
	inline void SetWindowSize(float sizeX, float sizeY);
	inline void SetWindowSize(float sizeX, float sizeY, int cond);
	inline void SetWindowCollapsed(bool collapsed);
	inline void SetWindowCollapsed(bool collapsed, int cond);
	inline void SetWindowFocus();
	inline void SetWindowFontScale(float scale);
	inline void SetWindowPos(const std::string& name, float posX, float posY);
	inline void SetWindowPos(const std::string& name, float posX, float posY, int cond);
	inline void SetWindowSize(const std::string& name, float sizeX, float sizeY);
	inline void SetWindowSize(const std::string& name, float sizeX, float sizeY, int cond);
	inline void SetWindowCollapsed(const std::string& name, bool collapsed);
	inline void SetWindowCollapsed(const std::string& name, bool collapsed, int cond);
	inline void SetWindowFocus(const std::string& name);

	// Content Region
	inline std::tuple<float, float> GetContentRegionMax();
	inline std::tuple<float, float> GetContentRegionAvail();
	inline std::tuple<float, float> GetWindowContentRegionMin();
	inline std::tuple<float, float> GetWindowContentRegionMax();
	inline float GetWindowContentRegionWidth();

	// Windows Scrolling
	inline float GetScrollX();
	inline float GetScrollY();
	inline float GetScrollMaxX();
	inline float GetScrollMaxY();
	inline void SetScrollX(float scrollX);
	inline void SetScrollY(float scrollY);
	inline void SetScrollHereX();
	inline void SetScrollHereX(float centerXRatio);
	inline void SetScrollHereY();
	inline void SetScrollHereY(float centerYRatio);
	inline void SetScrollFromPosX(float localX);
	inline void SetScrollFromPosX(float localX, float centerXRatio);
	inline void SetScrollFromPosY(float localY);
	inline void SetScrollFromPosY(float localY, float centerYRatio);

	// Parameters stacks (shared)
	inline void PushFont(ImFont* pFont);
	inline void PopFont();
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline void PushStyleColor(int idx, int col);
#endif
	inline void PushStyleColor(int idx, float colR, float colG, float colB, float colA); 
	inline void PopStyleColor();
	inline void PopStyleColor(int count); 
	inline void PushStyleVar(int idx, float val); ;
	inline void PushStyleVar(int idx, float valX, float valY);
	inline void PopStyleVar();
	inline void PopStyleVar(int count);
	inline void PushTabStop(bool tabStop); 
	inline void PopTabStop();
	inline void PushButtonRepeat(bool repeat);
	inline void PopButtonRepeat(); 
	inline void PushItemWidth(float itemWidth);
	// Parameters stacks (current window)
	inline void PopItemWidth();
	inline void SetNextItemWidth(float itemWidth); 
	inline float CalcItemWidth();
	inline void PushTextWrapPos(); 
	inline void PushTextWrapPos(float wrapLocalPosX);
	inline void PopTextWrapPos(); 

	// Style read access
// - Use the ShowStyleEditor() function to interactively see/edit the colors.
	inline ImFont* GetFont();
	inline float GetFontSize();
	inline std::tuple<float, float> GetFontTexUvWhitePixel();
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline int GetColorU32(int idx, float alphaMul);
	inline int GetColorU32(float colR, float colG, float colB, float colA);
	inline int GetColorU32(int col);
#endif
	inline std::tuple<float, float, float, float> GetStyleColorVec4(int idx);
	inline int GetColorU32(float colR, float colG, float colB, float colA);


	// Layout cursor positioning
	inline std::tuple<float, float> GetCursorScreenPos();
	inline void SetCursorScreenPos(float posX, float posY);
	inline std::tuple<float, float> GetCursorPos();
	inline float GetCursorPosX();
	inline float GetCursorPosY();
	inline void SetCursorPos(float localX, float localY);
	inline void SetCursorPosX(float localX);
	inline void SetCursorPosY(float localY);
	inline std::tuple<float, float> GetCursorStartPos();

	// Other layout functions
	inline void Separator();
	inline void SameLine();
	inline void SameLine(float offsetFromStartX);
	inline void SameLine(float offsetFromStartX, float spacing);
	inline void NewLine();
	inline void Spacing();
	inline void Dummy(float sizeX, float sizeY);
	inline void Indent();
	inline void Indent(float indentW) ;
	inline void Unindent();
	inline void Unindent(float indentW);
	inline void BeginGroup();
	inline void EndGroup();
	inline void AlignTextToFramePadding();
	inline float GetTextLineHeight();
	inline float GetTextLineHeightWithSpacing();
	inline float GetFrameHeight();
	inline float GetFrameHeightWithSpacing();

	// ID stack / scopes
	inline void PushID(const std::string& stringID);
	inline void PushID(const std::string& stringIDBegin, const std::string& stringIDEnd);
	//	inline void PushID(sol::table t) { ImGui::PushID(t.pointer()); }
	inline void PushID(int intID);
	inline void PopID();
	inline int GetID(const std::string& stringID);
	inline int GetID(const std::string& stringIDBegin, const std::string& stringIDEnd);
	inline int GetID(const void*);

	// Widgets: Text
	inline void TextUnformatted(const std::string& text);
	inline void TextUnformatted(const std::string& text, const std::string& textEnd);
	inline void Text(const std::string& text);;
	inline void TextColored(float colR, float colG, float colB, float colA, const std::string& text);
	inline void TextDisabled(const std::string& text);
	inline void TextWrapped(const std::string& text);
	inline void LabelText(const std::string& label, const std::string& text);
	inline void BulletText(const std::string& text); 
	inline void SeparatorText(const std::string& text);

	// Widgets: Main
	inline bool Button(const std::string& label);
	inline bool Button(const std::string& label, float sizeX, float sizeY);
	inline bool SmallButton(const std::string& label);
	inline bool InvisibleButton(const std::string& stringID, float sizeX, float sizeY);
	inline bool ArrowButton(const std::string& stringID, int dir);
	inline std::tuple<bool, bool> Checkbox(const std::string& label, bool v);
	inline bool CheckboxFlags(const std::string& label, int flags, int flags_value);
	inline bool RadioButton(const std::string& label, bool active);
	inline std::tuple<int, bool> RadioButton(const std::string& label, int v, int vButton);
	inline void ProgressBar(float fraction);
	inline void ProgressBar(float fraction, float sizeX, float sizeY);
	inline void ProgressBar(float fraction, float sizeX, float sizeY, const std::string& overlay);
	inline void Bullet();

	// Widgets: Images
	inline void Image(void* texture, float x, float y);
	inline bool ImageButton(const std::string& label, void* texture, float sizeX, float sizeY);

	// Widgets: Combo Box
	inline bool BeginCombo(const std::string& label, const std::string& previewValue);
	inline bool BeginCombo(const std::string& label, const std::string& previewValue, int flags);
	inline void EndCombo();
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const sol::table& items, int itemsCount);
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const sol::table& items, int itemsCount, int popupMaxHeightInItems);
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const std::string& itemsSeparatedByZeros);
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const std::string& itemsSeparatedByZeros, int popupMaxHeightInItems);
	// TODO: 3rd Combo from ImGui not Supported

	// Widgets: Drag Sliders
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v);
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed);
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min);
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min, float v_max);
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min, float v_max, const std::string& format);
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format, float power);
	inline void DragFloatRange2();
	inline std::tuple<int, bool> DragInt(const std::string& label, int v);
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed);
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed, int v_min);
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed, int v_min, int v_max);
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed, int v_min, int v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed, int v_min);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed, int v_min);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed, int v_min);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max, const std::string& format);
	inline void DragIntRange2();
	inline void DragScalar();
	inline void DragScalarN();

	// Widgets: Regular Sliders
	inline std::tuple<float, bool> SliderFloat(const std::string& label, float v, float v_min, float v_max);
	inline std::tuple<float, bool> SliderFloat(const std::string& label, float v, float v_min, float v_max, const std::string& format);
	inline std::tuple<float, bool> SliderFloat(const std::string& label, float v, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat2(const std::string& label, const sol::table& v, float v_min, float v_max);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat2(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat2(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat3(const std::string& label, const sol::table& v, float v_min, float v_max);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat3(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat3(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat4(const std::string& label, const sol::table& v, float v_min, float v_max);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat4(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat4(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad);
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad, float v_degrees_min);
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad, float v_degrees_min, float v_degrees_max);
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad, float v_degrees_min, float v_degrees_max, const std::string& format);
	inline std::tuple<int, bool> SliderInt(const std::string& label, int v, int v_min, int v_max);
	inline std::tuple<int, bool> SliderInt(const std::string& label, int v, int v_min, int v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt2(const std::string& label, const sol::table& v, int v_min, int v_max);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt2(const std::string& label, const sol::table& v, int v_min, int v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt3(const std::string& label, const sol::table& v, int v_min, int v_max);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt3(const std::string& label, const sol::table& v, int v_min, int v_max, const std::string& format);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt4(const std::string& label, const sol::table& v, int v_min, int v_max);
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt4(const std::string& label, const sol::table& v, int v_min, int v_max, const std::string& format);
	inline void SliderScalar();
	inline void SliderScalarN();
	inline std::tuple<float, bool> VSliderFloat(const std::string& label, float sizeX, float sizeY, float v, float v_min, float v_max);
	inline std::tuple<float, bool> VSliderFloat(const std::string& label, float sizeX, float sizeY, float v, float v_min, float v_max, const std::string& format);
	inline std::tuple<float, bool> VSliderFloat(const std::string& label, float sizeX, float sizeY, float v, float v_min, float v_max, const std::string& format, float power);
	inline std::tuple<int, bool> VSliderInt(const std::string& label, float sizeX, float sizeY, int v, int v_min, int v_max);
	inline std::tuple<int, bool> VSliderInt(const std::string& label, float sizeX, float sizeY, int v, int v_min, int v_max, const std::string& format);
	inline void VSliderScalar();

	// Widgets: Input with Keyboard
	inline std::tuple<std::string, bool> InputText(const std::string& label, std::string text);
	inline std::tuple<std::string, bool> InputText(const std::string& label, std::string text, unsigned int buf_size, int flags);
	inline std::tuple<std::string, bool> InputTextMultiline(const std::string& label, std::string text, unsigned int buf_size);
	inline std::tuple<std::string, bool> InputTextMultiline(const std::string& label, std::string text, unsigned int buf_size, float sizeX, float sizeY);
	inline std::tuple<std::string, bool> InputTextMultiline(const std::string& label, std::string text, unsigned int buf_size, float sizeX, float sizeY, int flags);
	inline std::tuple<std::string, bool> InputTextWithHint(const std::string& label, const std::string& hint, std::string text, unsigned int buf_size);
	inline std::tuple<std::string, bool> InputTextWithHint(const std::string& label, const std::string& hint, std::string text, unsigned int buf_size, int flags);
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v);
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step);
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step, float step_fast);;
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step, float step_fast, const std::string& format);
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step, float step_fast, const std::string& format, int flags);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat2(const std::string& label, const sol::table& v);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat2(const std::string& label, const sol::table& v, const std::string& format);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat2(const std::string& label, const sol::table& v, const std::string& format, int flags);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat3(const std::string& label, const sol::table& v);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat3(const std::string& label, const sol::table& v, const std::string& format);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat3(const std::string& label, const sol::table& v, const std::string& format, int flags);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat4(const std::string& label, const sol::table& v);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat4(const std::string& label, const sol::table& v, const std::string& format);
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat4(const std::string& label, const sol::table& v, const std::string& format, int flags);
	inline std::tuple<int, bool> InputInt(const std::string& label, int v);
	inline std::tuple<int, bool> InputInt(const std::string& label, int v, int step);
	inline std::tuple<int, bool> InputInt(const std::string& label, int v, int step, int step_fast);
	inline std::tuple<int, bool> InputInt(const std::string& label, int v, int step, int step_fast, int flags);
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt2(const std::string& label, const sol::table& v);
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt2(const std::string& label, const sol::table& v, int flags);
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt3(const std::string& label, const sol::table& v);
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt3(const std::string& label, const sol::table& v, int flags);
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt4(const std::string& label, const sol::table& v);
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt4(const std::string& label, const sol::table& v, int flags);
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v);
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step);
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step, double step_fast);
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step, double step_fast, const std::string& format);
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step, double step_fast, const std::string& format, int flags);
	inline void InputScalar();
	inline void InputScalarN();

	// Widgets: Color Editor / Picker
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit3(const std::string& label, const sol::table& col);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit3(const std::string& label, const sol::table& col, int flags);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit4(const std::string& label, const sol::table& col);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit4(const std::string& label, const sol::table& col, int flags);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker3(const std::string& label, const sol::table& col);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker3(const std::string& label, const sol::table& col, int flags);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker4(const std::string& label, const sol::table& col);
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker4(const std::string& label, const sol::table& col, int flags);
	inline bool ColorButton(const std::string& desc_id, const sol::table& col);
	inline bool ColorButton(const std::string& desc_id, const sol::table& col, int flags);
	inline bool ColorButton(const std::string& desc_id, const sol::table& col, int flags, float sizeX, float sizeY);
	inline void SetColorEditOptions(int flags);

	// Widgets: Trees
	inline bool TreeNode(const std::string& label);
	inline bool TreeNode(const std::string& label, const std::string& fmt);
	/* TODO: TreeNodeV(...) (2) ==> UNSUPPORTED */
	inline bool TreeNodeEx(const std::string& label);
	inline bool TreeNodeEx(const std::string& label, int flags);
	inline bool TreeNodeEx(const std::string& label, int flags, const std::string& fmt);
	/* TODO: TreeNodeExV(...) (2) ==> UNSUPPORTED */
	inline void TreePush(const std::string& str_id);
	/* TODO: TreePush(const void*) ==> UNSUPPORTED */
	inline void TreePop();
	inline float GetTreeNodeToLabelSpacing();
	inline bool CollapsingHeader(const std::string& label);
	inline bool CollapsingHeader(const std::string& label, int flags);
	inline std::tuple<bool, bool> CollapsingHeader(const std::string& label, bool open);
	inline std::tuple<bool, bool> CollapsingHeader(const std::string& label, bool open, int flags);
	inline void SetNextItemOpen(bool is_open);
	inline void SetNextItemOpen(bool is_open, int cond);

	// Widgets: Selectables
	// TODO: Only one of Selectable variations is possible due to same parameters for Lua
	inline bool Selectable(const std::string& label);
	inline bool Selectable(const std::string& label, bool selected);
	inline bool Selectable(const std::string& label, bool selected, int flags);
	inline bool Selectable(const std::string& label, bool selected, int flags, float sizeX, float sizeY);

	// Widgets: List Boxes
	inline bool BeginListBox(const std::string& label, float sizeX, float sizeY);
	inline void EndListBox();
	inline std::tuple<int, bool> ListBox(const std::string& label, int current_item, const sol::table& items, int items_count);
	inline std::tuple<int, bool> ListBox(const std::string& label, int current_item, const sol::table& items, int items_count, int height_in_items);

	// Widgets: Data Plotting
	/* TODO: Widgets Data Plotting ==> UNSUPPORTED (barely used and quite long functions) */

	// Widgets: Value() helpers
	inline void Value(const std::string& prefix, bool b);
	inline void Value(const std::string& prefix, int v);
	inline void Value(const std::string& prefix, unsigned int v);
	inline void Value(const std::string& prefix, float v);
	inline void Value(const std::string& prefix, float v, const std::string& float_format);

	// Widgets: Menus
	inline bool BeginMenuBar();
	inline void EndMenuBar();
	inline bool BeginMainMenuBar();
	inline void EndMainMenuBar();
	inline bool BeginMenu(const std::string& label);
	inline bool BeginMenu(const std::string& label, bool enabled);
	inline void EndMenu();
	inline bool MenuItem(const std::string& label);
	inline bool MenuItem(const std::string& label, const std::string& shortcut);
	inline std::tuple<bool, bool> MenuItem(const std::string& label, const std::string& shortcut, bool selected);
	inline std::tuple<bool, bool> MenuItem(const std::string& label, const std::string& shortcut, bool selected, bool enabled); 

	// Tooltips
	inline bool BeginTooltip();
	inline void EndTooltip();
	inline void SetTooltip(const std::string& fmt);
	inline void SetTooltipV();

	// Tooltips: helpers for showing a tooltip when hovering an item
	inline bool BeginItemTooltip();
	inline void SetItemTooltip(const std::string& fmt);

	// Popups, Modals
	inline bool BeginPopup(const std::string& str_id);
	inline bool BeginPopup(const std::string& str_id, int flags);
	inline bool BeginPopupModal(const std::string& name);
	inline bool BeginPopupModal(const std::string& name, bool open);
	inline bool BeginPopupModal(const std::string& name, bool open, int flags);
	inline void EndPopup();

	// Popups: open/close functions
	inline void OpenPopup(const std::string& str_id);;
	inline void OpenPopup(const std::string& str_id, int popup_flags);
	inline void OpenPopupOnItemClick();
	inline void OpenPopupOnItemClick(const std::string& str_id);
	inline void OpenPopupOnItemClick(const std::string& str_id, int popup_flags);
	inline void CloseCurrentPopup();

	// Popups: open+begin combined functions helpers
	inline bool BeginPopupContextItem();
	inline bool BeginPopupContextItem(const std::string& str_id);;
	inline bool BeginPopupContextItem(const std::string& str_id, int popup_flags);
	inline bool BeginPopupContextWindow();
	inline bool BeginPopupContextWindow(const std::string& str_id);;
	inline bool BeginPopupContextWindow(const std::string& str_id, int popup_flags);
	inline bool BeginPopupContextVoid();
	inline bool BeginPopupContextVoid(const std::string& str_id);
	inline bool BeginPopupContextVoid(const std::string& str_id, int popup_flags);

	// Popups: query functions
	inline bool IsPopupOpen(const std::string& str_id);
	inline bool IsPopupOpen(const std::string& str_id, int popup_flags);

	// Tables
	inline bool BeginTable(const std::string& str_id, int column);
	inline bool BeginTable(const std::string& str_id, int column, int flags);
	inline bool BeginTable(const std::string& str_id, int column, int flags, float sizeX, float sizeY); 
	inline bool BeginTable(const std::string& str_id, int column, int flags, float sizeX, float sizeY, float inner_width);
	inline void EndTable();
	inline void TableNextRow();
	inline void TableNextRow(int row_flags);
	inline void TableNextRow(int row_flags, float min_row_height);
	inline void TableNextColumn();
	inline void TableSetColumnIndex(int column_n);

	// Tables: Headers & Columns declaration
	inline void TableSetupColumn(const std::string& str_id);
	inline void TableSetupColumn(const std::string& str_id, int flags);
	inline void TableSetupColumn(const std::string& str_id, int flags, float init_width_or_weight);
	inline void TableSetupColumn(const std::string& str_id, int flags, float init_width_or_weight, int id);
	inline void TableSetupScrollFreeze(int cols, int rows);
	inline void TableHeader(const std::string& str_id);
	inline void TableHeadersRow();
	inline void TableAngledHeadersRow();

	// Tables: Sorting & Miscellaneous functions
	inline const ImGuiTableColumnSortSpecs* GetSortSpecs(ImGuiTableSortSpecs* tablesort, int index);
	inline ImGuiTableSortSpecs* TableGetSortSpecs();
	inline int TableGetColumnCount();
	inline int TableGetColumnIndex();;
	inline int TableGetRowIndex();
	inline const char* TableGetColumnName();
	inline const char* TableGetColumnName(int column);
	inline int TableGetColumnFlags();;
	inline int TableGetColumnFlags(int column);
	inline void TableSetColumnEnabled(int column, bool v);



	// Columns
	inline void Columns();
	inline void Columns(int count);
	inline void Columns(int count, const std::string& id);
	inline void Columns(int count, const std::string& id, bool border);
	inline void NextColumn();
	inline int GetColumnIndex();
	inline float GetColumnWidth();
	inline float GetColumnWidth(int column_index);
	inline void SetColumnWidth(int column_index, float width);
	inline float GetColumnOffset();
	inline float GetColumnOffset(int column_index);
	inline void SetColumnOffset(int column_index, float offset_x);
	inline int GetColumnsCount();

	// Tab Bars, Tabs
	inline bool BeginTabBar(const std::string& str_id);
	inline bool BeginTabBar(const std::string& str_id, int flags);
	inline void EndTabBar();
	inline bool BeginTabItem(const std::string& label);
	inline std::tuple<bool, bool> BeginTabItem(const std::string& label, bool open);
	inline std::tuple<bool, bool> BeginTabItem(const std::string& label, bool open, int flags);
	inline void EndTabItem();
	inline void SetTabItemClosed(const std::string& tab_or_docked_window_label);
#if defined(IMGUI_HAS_DOCK)
	// Docking
	inline void DockSpace(unsigned int id);
	inline void DockSpace(unsigned int id, float sizeX, float sizeY);
	inline void DockSpace(unsigned int id, float sizeX, float sizeY, int flags);
	inline unsigned int DockSpaceOverViewport();
	inline void SetNextWindowDockID(unsigned int dock_id);
	inline void SetNextWindowDockID(unsigned int dock_id, int cond);
	inline void SetNextWindowClass();
	inline unsigned int GetWindowDockID();
	inline bool IsWindowDocked();
#endif
	// Logging
	inline void LogToTTY();
	inline void LogToTTY(int auto_open_depth);
	inline void LogToFile();
	inline void LogToFile(int auto_open_depth);
	inline void LogToFile(int auto_open_depth, const std::string& filename);
	inline void LogToClipboard();
	inline void LogToClipboard(int auto_open_depth);
	inline void LogFinish();
	inline void LogButtons();
	inline void LogText(const std::string& fmt);

	// Drag and Drop
	// TODO: Drag and Drop ==> UNSUPPORTED

	// Clipping
	inline void PushClipRect(float min_x, float min_y, float max_x, float max_y, bool intersect_current);
	inline void PopClipRect();

	// Focus, Activation
	inline void SetItemDefaultFocus();
	inline void SetKeyboardFocusHere();
	inline void SetKeyboardFocusHere(int offset);

	// Overlapping mode
	inline void SetNextItemAllowOverlap();

	// Item/Widgets Utilities and Query Functions
	inline bool IsItemHovered();
	inline bool IsItemHovered(int flags) ;
	inline bool IsItemActive();
	inline bool IsItemFocused();
	inline bool IsItemClicked();
	inline bool IsItemClicked(int mouse_button);
	inline bool IsItemVisible();
	inline bool IsItemEdited();
	inline bool IsItemActivated();
	inline bool IsItemDeactivated();
	inline bool IsItemDeactivatedAfterEdit();
	inline bool IsItemToggledOpen();
	inline bool IsAnyItemHovered();
	inline bool IsAnyItemActive();
	inline bool IsAnyItemFocused();
	inline int GetItemID();
	inline std::tuple<float, float> GetItemRectMin();
	inline std::tuple<float, float> GetItemRectMax();
	inline std::tuple<float, float> GetItemRectSize();

	// Miscellaneous Utilities
	inline bool IsRectVisible(float sizeX, float sizeY);
	inline bool IsRectVisible(float minX, float minY, float maxX, float maxY);
	inline double GetTime();
	inline int GetFrameCount();
	/* TODO: GetBackgroundDrawList(), GetForeGroundDrawList(), GetDrawListSharedData() ==> UNSUPPORTED */
	inline std::string GetStyleColorName(int idx);
	/* TODO: SetStateStorage(), GetStateStorage(), CalcListClipping() ==> UNSUPPORTED */

	// Text Utilities
	inline std::tuple<float, float> CalcTextSize(const std::string& text);
	inline std::tuple<float, float> CalcTextSize(const std::string& text, const std::string& text_end);
	inline std::tuple<float, float> CalcTextSize(const std::string& text, const std::string& text_end, bool hide_text_after_double_hash);
	inline std::tuple<float, float> CalcTextSize(const std::string& text, const std::string& text_end, bool hide_text_after_double_hash, float wrap_width);

	// Color Utilities
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline sol::as_table_t<std::vector<float>> ColorConvertU32ToFloat4(unsigned int in);
	inline unsigned int ColorConvertFloat4ToU32(const sol::table& rgba);
#endif
	inline std::tuple<float, float, float> ColorConvertRGBtoHSV(float r, float g, float b);
	inline std::tuple<float, float, float> ColorConvertHSVtoRGB(float h, float s, float v);

	// Inputs Utilities: Keyboard/Mouse/Gamepad
	inline bool IsKeyDown(int user_key_index);
	inline bool IsKeyPressed(int user_key_index);
	inline bool IsKeyPressed(int user_key_index, bool repeat);
	inline bool IsKeyReleased(int user_key_index);
	inline int GetKeyPressedAmount(int key_index, float repeat_delay, float rate);
	inline void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard);

	// Inputs Utilities: Mouse specific
	inline bool IsMouseDown(int button);
	inline bool IsMouseClicked(int button);
	inline bool IsMouseClicked(int button, bool repeat);
	inline bool IsMouseReleased(int button);
	inline bool IsMouseDoubleClicked(int button);
	inline int GetMouseClickedCount(int button);
	inline bool IsMouseHoveringRect(float min_x, float min_y, float max_x, float max_y);
	inline bool IsMouseHoveringRect(float min_x, float min_y, float max_x, float max_y, bool clip);
	inline bool IsMousePosValid();
	inline bool IsAnyMouseDown();
	inline std::tuple<float, float> GetMousePos();
	inline std::tuple<float, float> GetMousePosOnOpeningCurrentPopup();
	inline bool IsMouseDragging(int button);
	inline bool IsMouseDragging(int button, float lock_threshold);
	inline std::tuple<float, float> GetMouseDragDelta();
	inline std::tuple<float, float> GetMouseDragDelta(int button);
	inline std::tuple<float, float> GetMouseDragDelta(int button, float lock_threshold);
	inline void ResetMouseDragDelta();
	inline void ResetMouseDragDelta(int button);
	inline int GetMouseCursor();
	inline void SetMouseCursor(int cursor_type);
	inline void SetNextFrameWantCaptureMouse(bool want_capture_mouse_value);
	inline ImFont* AddFontFromFileTTF(ImFontAtlas* fontAtlas, const char* filename, float size_pixels);
	
	inline void PushClipRectDraw(ImDrawList* drawlist, float posXmin, float posYmin, float posXmax, float posYmax);
	inline void PushClipRectDraw(ImDrawList* drawlist, float posXmin, float posYmin, float posXmax, float posYmax, bool intersect);	
    inline void PopClipRectDraw(ImDrawList* drawlist);
    inline ImVec2 GetClipRectMin(ImDrawList* drawlist);
    inline ImVec2 GetClipRectMax(ImDrawList* drawlist);
    inline void AddLine(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color);
	inline void AddLine(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float thickness);
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color);
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding);
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding, int flags);
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding, int flags, float thickness);
	inline void AddRectFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color);
	inline void AddRectFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding);
	inline void AddRectFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding, int flags);
	inline void AddRectFilledMultiColor(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t colorUpperLeft, uint32_t colorUpperRight, uint32_t colorBottomRight, uint32_t colorBottomLeft);
	inline void AddQuad(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, float p3X, float p3Y, float p4X, float p4Y, uint32_t color);
	inline void AddQuad(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, float p3X, float p3Y, float p4X, float p4Y, uint32_t color, float thickness);
	inline void AddQuadFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, float p3X, float p3Y, float p4X, float p4Y, uint32_t color);
	inline void AddTriangle(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, float p3X, float p3Y, uint32_t color);
	inline void AddTriangle(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, float p3X, float p3Y, uint32_t color, float thickness);
	inline void AddTriangleFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, float p3X, float p3Y, uint32_t color);
	inline void AddCircle(ImDrawList* drawlist, float pX, float pY, float radius, uint32_t color);
	inline void AddCircle(ImDrawList* drawlist, float pX, float pY, float radius, uint32_t color, int numSegments);
	inline void AddCircle(ImDrawList* drawlist, float pX, float pY, float radius, uint32_t color, int numSegments, float thickness);
	inline void AddCircleFilled(ImDrawList* drawlist, float pX, float pY, float radius, uint32_t color);
	inline void AddCircleFilled(ImDrawList* drawlist, float pX, float pY, float radius, uint32_t color, int numSegments);
	inline void AddNgon(ImDrawList* drawlist, float centerX, float centerY, float radius, uint32_t color, int numSegments);
	inline void AddNgon(ImDrawList* drawlist, float centerX, float centerY, float radius, uint32_t color, int numSegments, float thickness);
	inline void AddNgonFilled(ImDrawList* drawlist, float centerX, float centerY, float radius, uint32_t color, int numSegments);
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color);
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color, float rotation);
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color, float rotation, int numSegments);
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color, float rotation, int numSegments, float thickness);
	inline void AddEllipseFilled(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color);
	inline void AddEllipseFilled(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color, float rotation);
	inline void AddEllipseFilled(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color, float rotation, int numSegments);
	inline void AddText(ImDrawList* drawlist, float posX, float posY, uint32_t color, const char* textBegin);
	inline void AddText(ImDrawList* drawlist, float posX, float posY, uint32_t color, const char* textBegin, const char* textEnd);
	inline void AddBezierCubic(ImDrawList* drawlist, float p1X, float p1Y, 
		float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y,
		uint32_t color, float thickness);
	inline void AddBezierCubic(ImDrawList* drawlist, float p1X, float p1Y, 
		float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y,
		uint32_t color, float thickness, int numSegments);
	inline void AddBezierQuadratic(ImDrawList* drawlist, float p1X, float p1Y, 
		float p2X, float p2Y, float p3X, float p3Y,
		uint32_t color, float thickness);
	inline void AddBezierQuadratic(ImDrawList* drawlist, float p1X, float p1Y, 
		float p2X, float p2Y, float p3X, float p3Y,
		uint32_t color, float thickness, int numSegments);
	inline void PathClear(ImDrawList* drawlist);
	inline void PathLineTo(ImDrawList* drawlist, float posX, float posY);
	inline void PathLineToMergeDuplicate(ImDrawList* drawlist, float posX, float posY);
	inline void PathFillConvex(ImDrawList* drawlist, uint32_t color);
	inline void PathStroke(ImDrawList* drawlist, uint32_t color);
	inline void PathStroke(ImDrawList* drawlist, uint32_t color, int flags);
	inline void PathArcTo(ImDrawList* drawlist, float centerX, float centerY, float radius, float aMin, float aMax);
	inline void PathArcTo(ImDrawList* drawlist, float centerX, float centerY, float radius, float aMin, float aMax, int numSegments);
	inline void PathArcToFast(ImDrawList* drawlist, float centerX, float centerY, float radius, int aMin, int aMax);
	inline void PathEllipticalArcTo(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, float rotation, float aMin, float aMax);
	inline void PathEllipticalArcTo(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, float rotation, float aMin, float aMax, int numSegments);
	inline void PathBezierCubicCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y);
	inline void PathBezierCubicCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y, int numSegments);
	inline void PathBezierQuadraticCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y);
	inline void PathBezierQuadraticCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y, int numSegments);
	inline void PathRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2Y);
	inline void PathRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2Y, float rounding);
	inline void PathRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2Y, float rounding, int flags);
	
	// Clipboard Utilities
	inline std::string GetClipboardText();
	inline void SetClipboardText(const std::string& text);

	inline void InitEnums(sol::state& lua);
	
	void Init(sol::state& lua);
	
}