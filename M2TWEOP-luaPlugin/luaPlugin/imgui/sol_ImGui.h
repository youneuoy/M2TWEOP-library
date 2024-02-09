///
//@module LuaPluginImGui
//@author youneuoy
//@license GPL-3.0
#pragma once
#include "imgui.h"
#include <string>
#include "../lua/sol.hpp"
#include <imgui_internal.h>
#include "imgui_stdlib.h"
namespace sol_ImGui
{
	inline ImGuiIO* GetIO() { return &ImGui::GetIO(); }


	// Windows
	inline bool Begin(const std::string& name) { return ImGui::Begin(name.c_str()); }
	inline std::tuple<bool, bool> Begin(const std::string& name, bool open)
	{
		if (!open) return std::make_tuple(false, false);

		bool shouldDraw = ImGui::Begin(name.c_str(), &open);

		if (!open)
		{
			//	ImGui::End();
			return std::make_tuple(false, false);
		}

		return std::make_tuple(open, shouldDraw);
	}
	inline std::tuple<bool, bool> Begin(const std::string& name, bool open, int flags)
	{
		if (!open) return std::make_tuple(false, false);
		bool shouldDraw = ImGui::Begin(name.c_str(), &open, static_cast<ImGuiWindowFlags_>(flags));

		if (!open)
		{
			//	ImGui::End();
			return std::make_tuple(false, false);
		}

		return std::make_tuple(open, shouldDraw);
	}
	inline void End() { ImGui::End(); }

	// Child Windows
	inline bool BeginChild(const std::string& name) { return ImGui::BeginChild(name.c_str()); }
	inline bool BeginChild(const std::string& name, float sizeX) { return ImGui::BeginChild(name.c_str(), { sizeX, 0 }); }
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY) { return ImGui::BeginChild(name.c_str(), { sizeX, sizeY }); }
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY, bool border) { return ImGui::BeginChild(name.c_str(), { sizeX, sizeY }, border); }
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY, bool border, int flags) { return ImGui::BeginChild(name.c_str(), { sizeX, sizeY }, border, static_cast<ImGuiWindowFlags>(flags)); }
	inline void EndChild() { ImGui::EndChild(); }

	// Windows Utilities
	inline bool IsWindowAppearing() { return ImGui::IsWindowAppearing(); }
	inline bool IsWindowCollapsed() { return ImGui::IsWindowCollapsed(); }
	inline bool IsWindowFocused() { return ImGui::IsWindowFocused(); }
	inline bool IsWindowFocused(int flags) { return ImGui::IsWindowFocused(static_cast<ImGuiFocusedFlags>(flags)); }
	inline bool IsWindowHovered() { return ImGui::IsWindowHovered(); }
	inline bool IsWindowHovered(int flags) { return ImGui::IsWindowHovered(static_cast<ImGuiHoveredFlags>(flags)); }
	inline ImDrawList* GetWindowDrawList() { return nullptr; /* TODO: GetWindowDrawList() ==> UNSUPPORTED */ }
#if defined(IMGUI_HAS_DOCK)
	inline float GetWindowDpiScale() { return ImGui::GetWindowDpiScale(); }
	inline ImGuiViewport* GetWindowViewport() { return nullptr; /* TODO: GetWindowViewport() ==> UNSUPPORTED */ }
#endif

	inline std::tuple<float, float> GetWindowPos() { const auto vec2{ ImGui::GetWindowPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetWindowSize() { const auto vec2{ ImGui::GetWindowSize() };  return std::make_tuple(vec2.x, vec2.y); }
	inline float GetWindowWidth() { return ImGui::GetWindowWidth(); }
	inline float GetWindowHeight() { return ImGui::GetWindowHeight(); }

	// Prefer using SetNext...
	inline void SetNextWindowPos(float posX, float posY) { ImGui::SetNextWindowPos({ posX, posY }); }
	inline void SetNextWindowPos(float posX, float posY, int cond) { ImGui::SetNextWindowPos({ posX, posY }, static_cast<ImGuiCond>(cond)); }
	inline void SetNextWindowPos(float posX, float posY, int cond, float pivotX, float pivotY) { ImGui::SetNextWindowPos({ posX, posY }, static_cast<ImGuiCond>(cond), { pivotX, pivotY }); }
	inline void SetNextWindowSize(float sizeX, float sizeY) { ImGui::SetNextWindowSize({ sizeX, sizeY }); }
	inline void SetNextWindowSize(float sizeX, float sizeY, int cond) { ImGui::SetNextWindowSize({ sizeX, sizeY }, static_cast<ImGuiCond>(cond)); }
	inline void SetNextWindowSizeConstraints(float minX, float minY, float maxX, float maxY) { ImGui::SetNextWindowSizeConstraints({ minX, minY }, { maxX, maxY }); }
	inline void SetNextWindowContentSize(float sizeX, float sizeY) { ImGui::SetNextWindowContentSize({ sizeX, sizeY }); }
	inline void SetNextWindowCollapsed(bool collapsed) { ImGui::SetNextWindowCollapsed(collapsed); }
	inline void SetNextWindowCollapsed(bool collapsed, int cond) { ImGui::SetNextWindowCollapsed(collapsed, static_cast<ImGuiCond>(cond)); }
	inline void SetNextWindowFocus() { ImGui::SetNextWindowFocus(); }
	inline void SetNextWindowBgAlpha(float alpha) { ImGui::SetNextWindowBgAlpha(alpha); }
	inline void SetWindowPos(float posX, float posY) { ImGui::SetWindowPos({ posX, posY }); }
	inline void SetWindowPos(float posX, float posY, int cond) { ImGui::SetWindowPos({ posX, posY }, static_cast<ImGuiCond>(cond)); }
	inline void SetWindowSize(float sizeX, float sizeY) { ImGui::SetWindowSize({ sizeX, sizeY }); }
	inline void SetWindowSize(float sizeX, float sizeY, int cond) { ImGui::SetWindowSize({ sizeX, sizeY }, static_cast<ImGuiCond>(cond)); }
	inline void SetWindowCollapsed(bool collapsed) { ImGui::SetWindowCollapsed(collapsed); }
	inline void SetWindowCollapsed(bool collapsed, int cond) { ImGui::SetWindowCollapsed(collapsed, static_cast<ImGuiCond>(cond)); }
	inline void SetWindowFocus() { ImGui::SetWindowFocus(); }
	inline void SetWindowFontScale(float scale) { ImGui::SetWindowFontScale(scale); }
	inline void SetWindowPos(const std::string& name, float posX, float posY) { ImGui::SetWindowPos(name.c_str(), { posX, posY }); }
	inline void SetWindowPos(const std::string& name, float posX, float posY, int cond) { ImGui::SetWindowPos(name.c_str(), { posX, posY }, static_cast<ImGuiCond>(cond)); }
	inline void SetWindowSize(const std::string& name, float sizeX, float sizeY) { ImGui::SetWindowSize(name.c_str(), { sizeX, sizeY }); }
	inline void SetWindowSize(const std::string& name, float sizeX, float sizeY, int cond) { ImGui::SetWindowSize(name.c_str(), { sizeX, sizeY }, static_cast<ImGuiCond>(cond)); }
	inline void SetWindowCollapsed(const std::string& name, bool collapsed) { ImGui::SetWindowCollapsed(name.c_str(), collapsed); }
	inline void SetWindowCollapsed(const std::string& name, bool collapsed, int cond) { ImGui::SetWindowCollapsed(name.c_str(), collapsed, static_cast<ImGuiCond>(cond)); }
	inline void SetWindowFocus(const std::string& name) { ImGui::SetWindowFocus(name.c_str()); }

	// Content Region
	inline std::tuple<float, float> GetContentRegionMax() { const auto vec2{ ImGui::GetContentRegionMax() };  return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetContentRegionAvail() { const auto vec2{ ImGui::GetContentRegionAvail() };  return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetWindowContentRegionMin() { const auto vec2{ ImGui::GetWindowContentRegionMin() };  return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetWindowContentRegionMax() { const auto vec2{ ImGui::GetWindowContentRegionMax() };  return std::make_tuple(vec2.x, vec2.y); }
	inline float GetWindowContentRegionWidth() { return ImGui::GetWindowContentRegionWidth(); }

	// Windows Scrolling
	inline float GetScrollX() { return ImGui::GetScrollX(); }
	inline float GetScrollY() { return ImGui::GetScrollY(); }
	inline float GetScrollMaxX() { return ImGui::GetScrollMaxX(); }
	inline float GetScrollMaxY() { return ImGui::GetScrollMaxY(); }
	inline void SetScrollX(float scrollX) { ImGui::SetScrollX(scrollX); }
	inline void SetScrollY(float scrollY) { ImGui::SetScrollY(scrollY); }
	inline void SetScrollHereX() { ImGui::SetScrollHereX(); }
	inline void SetScrollHereX(float centerXRatio) { ImGui::SetScrollHereX(centerXRatio); }
	inline void SetScrollHereY() { ImGui::SetScrollHereY(); }
	inline void SetScrollHereY(float centerYRatio) { ImGui::SetScrollHereY(centerYRatio); }
	inline void SetScrollFromPosX(float localX) { ImGui::SetScrollFromPosX(localX); }
	inline void SetScrollFromPosX(float localX, float centerXRatio) { ImGui::SetScrollFromPosX(localX, centerXRatio); }
	inline void SetScrollFromPosY(float localY) { ImGui::SetScrollFromPosY(localY); }
	inline void SetScrollFromPosY(float localY, float centerYRatio) { ImGui::SetScrollFromPosY(localY, centerYRatio); }

	// Parameters stacks (shared)
	inline void PushFont(ImFont* pFont) { ImGui::PushFont(pFont); }
	inline void PopFont() { ImGui::PopFont(); }
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline void PushStyleColor(int idx, int col) { ImGui::PushStyleColor(static_cast<ImGuiCol>(idx), ImU32(col)); }
#endif
	inline void PushStyleColor(int idx, float colR, float colG, float colB, float colA) { ImGui::PushStyleColor(static_cast<ImGuiCol>(idx), { colR, colG, colB, colA }); }
	inline void PopStyleColor() { ImGui::PopStyleColor(); }
	inline void PopStyleColor(int count) { ImGui::PopStyleColor(count); }
	inline void PushStyleVar(int idx, float val) { ImGui::PushStyleVar(static_cast<ImGuiStyleVar>(idx), val); }
	inline void PushStyleVar(int idx, float valX, float valY) { ImGui::PushStyleVar(static_cast<ImGuiStyleVar>(idx), { valX, valY }); }
	inline void PopStyleVar() { ImGui::PopStyleVar(); }
	inline void PopStyleVar(int count) { ImGui::PopStyleVar(count); }
	inline std::tuple<float, float, float, float> GetStyleColorVec4(int idx) { const auto col{ ImGui::GetStyleColorVec4(static_cast<ImGuiCol>(idx)) };	return std::make_tuple(col.x, col.y, col.z, col.w); }
	inline ImFont* GetFont() { return ImGui::GetFont(); }
	inline float GetFontSize() { return ImGui::GetFontSize(); }
	inline std::tuple<float, float> GetFontTexUvWhitePixel() { const auto vec2{ ImGui::GetFontTexUvWhitePixel() };	return std::make_tuple(vec2.x, vec2.y); }
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline int GetColorU32(int idx, float alphaMul) { return ImGui::GetColorU32(static_cast<ImGuiCol>(idx), alphaMul); }
	inline int GetColorU32(float colR, float colG, float colB, float colA) { return ImGui::GetColorU32({ colR, colG, colB, colA }); }
	inline int GetColorU32(int col) { return ImGui::GetColorU32(ImU32(col)); }
#endif

	// Parameters stacks (current window)
	inline void PushItemWidth(float itemWidth) { ImGui::PushItemWidth(itemWidth); }
	inline void PopItemWidth() { ImGui::PopItemWidth(); }
	inline void SetNextItemWidth(float itemWidth) { ImGui::SetNextItemWidth(itemWidth); }
	inline float CalcItemWidth() { return ImGui::CalcItemWidth(); }
	inline void PushTextWrapPos() { ImGui::PushTextWrapPos(); }
	inline void PushTextWrapPos(float wrapLocalPosX) { ImGui::PushTextWrapPos(wrapLocalPosX); }
	inline void PopTextWrapPos() { ImGui::PopTextWrapPos(); }
	inline void PushAllowKeyboardFocus(bool allowKeyboardFocus) { ImGui::PushAllowKeyboardFocus(allowKeyboardFocus); }
	inline void PopAllowKeyboardFocus() { ImGui::PopAllowKeyboardFocus(); }
	inline void PushButtonRepeat(bool repeat) { ImGui::PushButtonRepeat(repeat); }
	inline void PopButtonRepeat() { ImGui::PopButtonRepeat(); }

	// Cursor / Layout
	inline void Separator() { ImGui::Separator(); }
	inline void SameLine() { ImGui::SameLine(); }
	inline void SameLine(float offsetFromStartX) { ImGui::SameLine(offsetFromStartX); }
	inline void SameLine(float offsetFromStartX, float spacing) { ImGui::SameLine(offsetFromStartX, spacing); }
	inline void NewLine() { ImGui::NewLine(); }
	inline void Spacing() { ImGui::Spacing(); }
	inline void Dummy(float sizeX, float sizeY) { ImGui::Dummy({ sizeX, sizeY }); }
	inline void Indent() { ImGui::Indent(); }
	inline void Indent(float indentW) { ImGui::Indent(indentW); }
	inline void Unindent() { ImGui::Unindent(); }
	inline void Unindent(float indentW) { ImGui::Unindent(indentW); }
	inline void BeginGroup() { ImGui::BeginGroup(); }
	inline void EndGroup() { ImGui::EndGroup(); }
	inline std::tuple<float, float> GetCursorPos() { const auto vec2{ ImGui::GetCursorPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline float GetCursorPosX() { return ImGui::GetCursorPosX(); }
	inline float GetCursorPosY() { return ImGui::GetCursorPosY(); }
	inline void SetCursorPos(float localX, float localY) { ImGui::SetCursorPos({ localX, localY }); }
	inline void SetCursorPosX(float localX) { ImGui::SetCursorPosX(localX); }
	inline void SetCursorPosY(float localY) { ImGui::SetCursorPosY(localY); }
	inline std::tuple<float, float> GetCursorStartPos() { const auto vec2{ ImGui::GetCursorStartPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetCursorScreenPos() { const auto vec2{ ImGui::GetCursorScreenPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline void SetCursorScreenPos(float posX, float posY) { ImGui::SetCursorScreenPos({ posX, posY }); }
	inline void AlignTextToFramePadding() { ImGui::AlignTextToFramePadding(); }
	inline float GetTextLineHeight() { return ImGui::GetTextLineHeight(); }
	inline float GetTextLineHeightWithSpacing() { return ImGui::GetTextLineHeightWithSpacing(); }
	inline float GetFrameHeight() { return ImGui::GetFrameHeight(); }
	inline float GetFrameHeightWithSpacing() { return ImGui::GetFrameHeightWithSpacing(); }

	// ID stack / scopes
	inline void PushID(const std::string& stringID) { ImGui::PushID(stringID.c_str()); }
	inline void PushID(const std::string& stringIDBegin, const std::string& stringIDEnd) { ImGui::PushID(stringIDBegin.c_str(), stringIDEnd.c_str()); }
	//	inline void PushID(sol::table t) { ImGui::PushID(t.pointer()); }
	inline void PushID(int intID) { ImGui::PushID(intID); }
	inline void PopID() { ImGui::PopID(); }
	inline int GetID(const std::string& stringID) { return ImGui::GetID(stringID.c_str()); }
	inline int GetID(const std::string& stringIDBegin, const std::string& stringIDEnd) { return ImGui::GetID(stringIDBegin.c_str(), stringIDEnd.c_str()); }
	inline int GetID(const void*) { return 0;  /* TODO: GetID(void*) ==> UNSUPPORTED */ }

	// Widgets: Text
	inline void TextUnformatted(const std::string& text) { ImGui::TextUnformatted(text.c_str()); }
	inline void TextUnformatted(const std::string& text, const std::string& textEnd) { ImGui::TextUnformatted(text.c_str(), textEnd.c_str()); }
	inline void Text(const std::string& text) { ImGui::Text(text.c_str()); }
	inline void TextColored(float colR, float colG, float colB, float colA, const std::string& text) { ImGui::TextColored({ colR, colG, colB, colA }, text.c_str()); }
	inline void TextDisabled(const std::string& text) { ImGui::TextDisabled(text.c_str()); }
	inline void TextWrapped(const std::string& text) { ImGui::TextWrapped(text.c_str()); }
	inline void LabelText(const std::string& label, const std::string& text) { ImGui::LabelText(label.c_str(), text.c_str()); }
	inline void BulletText(const std::string& text) { ImGui::BulletText(text.c_str()); }

	// Widgets: Main
	inline bool Button(const std::string& label)
	{
		return ImGui::Button(label.c_str());
	}
	inline bool Button(const std::string& label, float sizeX, float sizeY) { return ImGui::Button(label.c_str(), { sizeX, sizeY }); }
	inline bool SmallButton(const std::string& label) { return ImGui::SmallButton(label.c_str()); }
	inline bool InvisibleButton(const std::string& stringID, float sizeX, float sizeY) { return ImGui::InvisibleButton(stringID.c_str(), { sizeX, sizeY }); }
	inline bool ArrowButton(const std::string& stringID, int dir) { return ImGui::ArrowButton(stringID.c_str(), static_cast<ImGuiDir>(dir)); }
	inline void Image(void* texture, float x, float y)
	{
		ImGui::Image(texture, ImVec2(x, y));
	}
	inline bool ImageButton(void* texture, float sizeX, float sizeY)
	{
		ImVec2 buttonSize;
		buttonSize.x = sizeX;
		buttonSize.y = sizeY;
		return ImGui::ImageButton(texture, buttonSize);
	}
	inline std::tuple<bool, bool> Checkbox(const std::string& label, bool v)
	{
		bool value{ v };
		bool pressed = ImGui::Checkbox(label.c_str(), &value);

		return std::make_tuple(value, pressed);
	}
	inline bool CheckboxFlags() { return false; /* TODO: CheckboxFlags(...) ==> UNSUPPORTED */ }
	inline bool RadioButton(const std::string& label, bool active) { return ImGui::RadioButton(label.c_str(), active); }
	inline std::tuple<int, bool> RadioButton(const std::string& label, int v, int vButton) { bool ret{ ImGui::RadioButton(label.c_str(), &v, vButton) }; return std::make_tuple(v, ret); }
	inline void ProgressBar(float fraction) { ImGui::ProgressBar(fraction); }
	inline void ProgressBar(float fraction, float sizeX, float sizeY) { ImGui::ProgressBar(fraction, { sizeX, sizeY }); }
	inline void ProgressBar(float fraction, float sizeX, float sizeY, const std::string& overlay) { ImGui::ProgressBar(fraction, { sizeX, sizeY }, overlay.c_str()); }
	inline void Bullet() { ImGui::Bullet(); }

	// Widgets: Combo Box
	inline bool BeginCombo(const std::string& label, const std::string& previewValue) { return ImGui::BeginCombo(label.c_str(), previewValue.c_str()); }
	inline bool BeginCombo(const std::string& label, const std::string& previewValue, int flags) { return ImGui::BeginCombo(label.c_str(), previewValue.c_str(), static_cast<ImGuiComboFlags>(flags)); }
	inline void EndCombo() { ImGui::EndCombo(); }
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const sol::table& items, int itemsCount)
	{
		std::vector<std::string> strings;
		for (int i{ 1 }; i <= itemsCount; i++)
		{
			const auto& stringItem = items.get<sol::optional<std::string>>(i);
			strings.push_back(stringItem.value_or("Missing"));
		}

		std::vector<const char*> cstrings;
		for (auto& string : strings)
			cstrings.push_back(string.c_str());

		bool clicked = ImGui::Combo(label.c_str(), &currentItem, cstrings.data(), itemsCount);
		return std::make_tuple(currentItem, clicked);
	}
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const sol::table& items, int itemsCount, int popupMaxHeightInItems)
	{
		std::vector<std::string> strings;
		for (int i{ 1 }; i <= itemsCount; i++)
		{
			const auto& stringItem = items.get<sol::optional<std::string>>(i);
			strings.push_back(stringItem.value_or("Missing"));
		}

		std::vector<const char*> cstrings;
		for (auto& string : strings)
			cstrings.push_back(string.c_str());

		bool clicked = ImGui::Combo(label.c_str(), &currentItem, cstrings.data(), itemsCount, popupMaxHeightInItems);
		return std::make_tuple(currentItem, clicked);
	}
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const std::string& itemsSeparatedByZeros)
	{
		bool clicked = ImGui::Combo(label.c_str(), &currentItem, itemsSeparatedByZeros.c_str());
		return std::make_tuple(currentItem, clicked);
	}
	inline std::tuple<int, bool> Combo(const std::string& label, int currentItem, const std::string& itemsSeparatedByZeros, int popupMaxHeightInItems)
	{
		bool clicked = ImGui::Combo(label.c_str(), &currentItem, itemsSeparatedByZeros.c_str(), popupMaxHeightInItems);
		return std::make_tuple(currentItem, clicked);
	}
	// TODO: 3rd Combo from ImGui not Supported

	// Widgets: Drags
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v) { bool used = ImGui::DragFloat(label.c_str(), &v); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed) { bool used = ImGui::DragFloat(label.c_str(), &v, v_speed); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min) { bool used = ImGui::DragFloat(label.c_str(), &v, v_speed, v_min); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min, float v_max) { bool used = ImGui::DragFloat(label.c_str(), &v, v_speed, v_min, v_max); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min, float v_max, const std::string& format) { bool used = ImGui::DragFloat(label.c_str(), &v, v_speed, v_min, v_max, format.c_str()); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> DragFloat(const std::string& label, float v, float v_speed, float v_min, float v_max, const std::string& format, float power) { bool used = ImGui::DragFloat(label.c_str(), &v, v_speed, v_min, v_max, format.c_str(), power); return std::make_tuple(v, used); }
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::DragFloat2(label.c_str(), value);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::DragFloat2(label.c_str(), value, v_speed);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::DragFloat2(label.c_str(), value, v_speed, v_min);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::DragFloat2(label.c_str(), value, v_speed, v_min, v_max);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::DragFloat2(label.c_str(), value, v_speed, v_min, v_max, format.c_str());

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat2(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format, float power)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::DragFloat2(label.c_str(), value, v_speed, v_min, v_max, format.c_str(), power);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::DragFloat3(label.c_str(), value);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::DragFloat3(label.c_str(), value, v_speed);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::DragFloat3(label.c_str(), value, v_speed, v_min);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::DragFloat3(label.c_str(), value, v_speed, v_min, v_max);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::DragFloat3(label.c_str(), value, v_speed, v_min, v_max, format.c_str());

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat3(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format, float power)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::DragFloat3(label.c_str(), value, v_speed, v_min, v_max, format.c_str(), power);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::DragFloat4(label.c_str(), value);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::DragFloat4(label.c_str(), value, v_speed);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::DragFloat4(label.c_str(), value, v_speed, v_min);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::DragFloat4(label.c_str(), value, v_speed, v_min, v_max);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::DragFloat4(label.c_str(), value, v_speed, v_min, v_max, format.c_str());

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> DragFloat4(const std::string& label, const sol::table& v, float v_speed, float v_min, float v_max, const std::string& format, float power)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::DragFloat4(label.c_str(), value, v_speed, v_min, v_max, format.c_str(), power);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline void DragFloatRange2() { /* TODO: DragFloatRange2(...) ==> UNSUPPORTED */ }
	inline std::tuple<int, bool> DragInt(const std::string& label, int v) { bool used = ImGui::DragInt(label.c_str(), &v); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed) { bool used = ImGui::DragInt(label.c_str(), &v, v_speed); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed, int v_min) { bool used = ImGui::DragInt(label.c_str(), &v, v_speed, v_min); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed, int v_min, int v_max) { bool used = ImGui::DragInt(label.c_str(), &v, v_speed, v_min, v_max); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> DragInt(const std::string& label, int v, float v_speed, int v_min, int v_max, const std::string& format) { bool used = ImGui::DragInt(label.c_str(), &v, v_speed, v_min, v_max, format.c_str()); return std::make_tuple(v, used); }
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::DragInt2(label.c_str(), value);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::DragInt2(label.c_str(), value, v_speed);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed, int v_min)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::DragInt2(label.c_str(), value, v_speed, v_min);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::DragInt2(label.c_str(), value, v_speed, v_min, v_max);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt2(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::DragInt2(label.c_str(), value, v_speed, v_min, v_max, format.c_str());

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::DragInt3(label.c_str(), value);

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::DragInt3(label.c_str(), value, v_speed);

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed, int v_min)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::DragInt3(label.c_str(), value, v_speed, v_min);

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::DragInt3(label.c_str(), value, v_speed, v_min, v_max);

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt3(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::DragInt3(label.c_str(), value, v_speed, v_min, v_max, format.c_str());

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::DragInt4(label.c_str(), value);

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::DragInt4(label.c_str(), value, v_speed);

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed, int v_min)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::DragInt4(label.c_str(), value, v_speed, v_min);

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::DragInt4(label.c_str(), value, v_speed, v_min, v_max);

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> DragInt4(const std::string& label, const sol::table& v, float v_speed, int v_min, int v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::DragInt4(label.c_str(), value, v_speed, v_min, v_max, format.c_str());

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline void DragIntRange2() { /* TODO: DragIntRange2(...) ==> UNSUPPORTED */ }
	inline void DragScalar() { /* TODO: DragScalar(...) ==> UNSUPPORTED */ }
	inline void DragScalarN() { /* TODO: DragScalarN(...) ==> UNSUPPORTED */ }

	// Widgets: Sliders
	inline std::tuple<float, bool> SliderFloat(const std::string& label, float v, float v_min, float v_max) { bool used = ImGui::SliderFloat(label.c_str(), &v, v_min, v_max); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> SliderFloat(const std::string& label, float v, float v_min, float v_max, const std::string& format) { bool used = ImGui::SliderFloat(label.c_str(), &v, v_min, v_max, format.c_str()); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> SliderFloat(const std::string& label, float v, float v_min, float v_max, const std::string& format, float power) { bool used = ImGui::SliderFloat(label.c_str(), &v, v_min, v_max, format.c_str(), power); return std::make_tuple(v, used); }
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat2(const std::string& label, const sol::table& v, float v_min, float v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::SliderFloat2(label.c_str(), value, v_min, v_max);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat2(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::SliderFloat2(label.c_str(), value, v_min, v_max, format.c_str());

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat2(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format, float power)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::SliderFloat2(label.c_str(), value, v_min, v_max, format.c_str(), power);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat3(const std::string& label, const sol::table& v, float v_min, float v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::SliderFloat3(label.c_str(), value, v_min, v_max);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat3(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::SliderFloat3(label.c_str(), value, v_min, v_max, format.c_str());

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat3(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format, float power)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::SliderFloat3(label.c_str(), value, v_min, v_max, format.c_str(), power);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat4(const std::string& label, const sol::table& v, float v_min, float v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::SliderFloat4(label.c_str(), value, v_min, v_max);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat4(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::SliderFloat4(label.c_str(), value, v_min, v_max, format.c_str());

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> SliderFloat4(const std::string& label, const sol::table& v, float v_min, float v_max, const std::string& format, float power)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::SliderFloat4(label.c_str(), value, v_min, v_max, format.c_str(), power);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad) { bool used = ImGui::SliderAngle(label.c_str(), &v_rad); return std::make_tuple(v_rad, used); }
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad, float v_degrees_min) { bool used = ImGui::SliderAngle(label.c_str(), &v_rad, v_degrees_min); return std::make_tuple(v_rad, used); }
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad, float v_degrees_min, float v_degrees_max) { bool used = ImGui::SliderAngle(label.c_str(), &v_rad, v_degrees_min, v_degrees_max); return std::make_tuple(v_rad, used); }
	inline std::tuple<float, bool> SliderAngle(const std::string& label, float v_rad, float v_degrees_min, float v_degrees_max, const std::string& format) { bool used = ImGui::SliderAngle(label.c_str(), &v_rad, v_degrees_min, v_degrees_max, format.c_str()); return std::make_tuple(v_rad, used); }
	inline std::tuple<int, bool> SliderInt(const std::string& label, int v, int v_min, int v_max) { bool used = ImGui::SliderInt(label.c_str(), &v, v_min, v_max); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> SliderInt(const std::string& label, int v, int v_min, int v_max, const std::string& format) { bool used = ImGui::SliderInt(label.c_str(), &v, v_min, v_max, format.c_str()); return std::make_tuple(v, used); }
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt2(const std::string& label, const sol::table& v, int v_min, int v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::SliderInt2(label.c_str(), value, v_min, v_max);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt2(const std::string& label, const sol::table& v, int v_min, int v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::SliderInt2(label.c_str(), value, v_min, v_max, format.c_str());

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt3(const std::string& label, const sol::table& v, int v_min, int v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::SliderInt3(label.c_str(), value, v_min, v_max);

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt3(const std::string& label, const sol::table& v, int v_min, int v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::SliderInt3(label.c_str(), value, v_min, v_max, format.c_str());

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt4(const std::string& label, const sol::table& v, int v_min, int v_max)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::SliderInt4(label.c_str(), value, v_min, v_max);

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<int>>, bool> SliderInt4(const std::string& label, const sol::table& v, int v_min, int v_max, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::SliderInt4(label.c_str(), value, v_min, v_max, format.c_str());

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline void SliderScalar() { /* TODO: SliderScalar(...) ==> UNSUPPORTED */ }
	inline void SliderScalarN() { /* TODO: SliderScalarN(...) ==> UNSUPPORTED */ }
	inline std::tuple<float, bool> VSliderFloat(const std::string& label, float sizeX, float sizeY, float v, float v_min, float v_max) { bool used = ImGui::VSliderFloat(label.c_str(), { sizeX, sizeY }, &v, v_min, v_max); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> VSliderFloat(const std::string& label, float sizeX, float sizeY, float v, float v_min, float v_max, const std::string& format) { bool used = ImGui::VSliderFloat(label.c_str(), { sizeX, sizeY }, &v, v_min, v_max, format.c_str()); return std::make_tuple(v, used); }
	inline std::tuple<float, bool> VSliderFloat(const std::string& label, float sizeX, float sizeY, float v, float v_min, float v_max, const std::string& format, float power) { bool used = ImGui::VSliderFloat(label.c_str(), { sizeX, sizeY }, &v, v_min, v_max, format.c_str(), power); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> VSliderInt(const std::string& label, float sizeX, float sizeY, int v, int v_min, int v_max) { bool used = ImGui::VSliderInt(label.c_str(), { sizeX, sizeY }, &v, v_min, v_max); return std::make_tuple(v, used); }
	inline std::tuple<int, bool> VSliderInt(const std::string& label, float sizeX, float sizeY, int v, int v_min, int v_max, const std::string& format) { bool used = ImGui::VSliderInt(label.c_str(), { sizeX, sizeY }, &v, v_min, v_max, format.c_str()); return std::make_tuple(v, used); }
	inline void VSliderScalar() { /* TODO: VSliderScalar(...) ==> UNSUPPORTED */ }

	// Widgets: Input with Keyboard
	inline std::tuple<std::string, bool> InputText(const std::string& label, std::string text)
	{
		bool selected = ImGui::InputText(label.c_str(), &text); return std::make_tuple(text, selected);
	}
	inline std::tuple<std::string, bool> InputText(const std::string& label, std::string text, unsigned int buf_size, int flags) { bool selected = ImGui::InputText(label.c_str(), &text[0], buf_size, static_cast<ImGuiInputTextFlags>(flags)); return std::make_tuple(text, selected); }
	inline std::tuple<std::string, bool> InputTextMultiline(const std::string& label, std::string text, unsigned int buf_size) { bool selected = ImGui::InputTextMultiline(label.c_str(), &text[0], buf_size); return std::make_tuple(text, selected); }
	inline std::tuple<std::string, bool> InputTextMultiline(const std::string& label, std::string text, unsigned int buf_size, float sizeX, float sizeY) { bool selected = ImGui::InputTextMultiline(label.c_str(), &text[0], buf_size, { sizeX, sizeY }); return std::make_tuple(text, selected); }
	inline std::tuple<std::string, bool> InputTextMultiline(const std::string& label, std::string text, unsigned int buf_size, float sizeX, float sizeY, int flags) { bool selected = ImGui::InputTextMultiline(label.c_str(), &text[0], buf_size, { sizeX, sizeY }, static_cast<ImGuiInputTextFlags>(flags)); return std::make_tuple(text, selected); }
	inline std::tuple<std::string, bool> InputTextWithHint(const std::string& label, const std::string& hint, std::string text, unsigned int buf_size) { bool selected = ImGui::InputTextWithHint(label.c_str(), hint.c_str(), &text[0], buf_size); return std::make_tuple(text, selected); }
	inline std::tuple<std::string, bool> InputTextWithHint(const std::string& label, const std::string& hint, std::string text, unsigned int buf_size, int flags) { bool selected = ImGui::InputTextWithHint(label.c_str(), hint.c_str(), &text[0], buf_size, static_cast<ImGuiInputTextFlags>(flags)); return std::make_tuple(text, selected); }
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v) { bool selected = ImGui::InputFloat(label.c_str(), &v); return std::make_tuple(v, selected); }
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step) { bool selected = ImGui::InputFloat(label.c_str(), &v, step); return std::make_tuple(v, selected); }
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step, float step_fast) { bool selected = ImGui::InputFloat(label.c_str(), &v, step, step_fast); return std::make_tuple(v, selected); }
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step, float step_fast, const std::string& format) { bool selected = ImGui::InputFloat(label.c_str(), &v, step, step_fast, format.c_str()); return std::make_tuple(v, selected); }
	inline std::tuple<float, bool> InputFloat(const std::string& label, float v, float step, float step_fast, const std::string& format, int flags) { bool selected = ImGui::InputFloat(label.c_str(), &v, step, step_fast, format.c_str(), static_cast<ImGuiInputTextFlags>(flags)); return std::make_tuple(v, selected); }
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat2(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::InputFloat2(label.c_str(), value);

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat2(const std::string& label, const sol::table& v, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::InputFloat2(label.c_str(), value, format.c_str());

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat2(const std::string& label, const sol::table& v, const std::string& format, int flags)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[2] = { float(v1), float(v2) };
		bool used = ImGui::InputFloat2(label.c_str(), value, format.c_str(), static_cast<ImGuiInputTextFlags>(flags));

		sol::as_table_t float2 = sol::as_table(std::vector<float>{
			value[0], value[1]
		});

		return std::make_tuple(float2, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat3(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::InputFloat3(label.c_str(), value);

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat3(const std::string& label, const sol::table& v, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::InputFloat3(label.c_str(), value, format.c_str());

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat3(const std::string& label, const sol::table& v, const std::string& format, int flags)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[3] = { float(v1), float(v2), float(v3) };
		bool used = ImGui::InputFloat3(label.c_str(), value, format.c_str(), static_cast<ImGuiInputTextFlags>(flags));

		sol::as_table_t float3 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(float3, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat4(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::InputFloat4(label.c_str(), value);

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat4(const std::string& label, const sol::table& v, const std::string& format)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::InputFloat4(label.c_str(), value, format.c_str());

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<float>>, bool> InputFloat4(const std::string& label, const sol::table& v, const std::string& format, int flags)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float value[4] = { float(v1), float(v2), float(v3), float(v4) };
		bool used = ImGui::InputFloat4(label.c_str(), value, format.c_str(), static_cast<ImGuiInputTextFlags>(flags));

		sol::as_table_t float4 = sol::as_table(std::vector<float>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(float4, used);
	}
	inline std::tuple<int, bool> InputInt(const std::string& label, int v) { bool selected = ImGui::InputInt(label.c_str(), &v); return std::make_tuple(v, selected); }
	inline std::tuple<int, bool> InputInt(const std::string& label, int v, int step) { bool selected = ImGui::InputInt(label.c_str(), &v, step); return std::make_tuple(v, selected); }
	inline std::tuple<int, bool> InputInt(const std::string& label, int v, int step, int step_fast) { bool selected = ImGui::InputInt(label.c_str(), &v, step, step_fast); return std::make_tuple(v, selected); }
	inline std::tuple<int, bool> InputInt(const std::string& label, int v, int step, int step_fast, int flags) { bool selected = ImGui::InputInt(label.c_str(), &v, step, step_fast, static_cast<ImGuiInputTextFlags>(flags)); return std::make_tuple(v, selected); }
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt2(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::InputInt2(label.c_str(), value);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt2(const std::string& label, const sol::table& v, int flags)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[2] = { int(v1), int(v2) };
		bool used = ImGui::InputInt2(label.c_str(), value, static_cast<ImGuiInputTextFlags>(flags));

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			value[0], value[1]
		});

		return std::make_tuple(int2, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt3(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::InputInt3(label.c_str(), value);

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt3(const std::string& label, const sol::table& v, int flags)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[3] = { int(v1), int(v2), int(v3) };
		bool used = ImGui::InputInt3(label.c_str(), value, static_cast<ImGuiInputTextFlags>(flags));

		sol::as_table_t int3 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2]
		});

		return std::make_tuple(int3, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt4(const std::string& label, const sol::table& v)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::InputInt4(label.c_str(), value);

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple <sol::as_table_t<std::vector<int>>, bool> InputInt4(const std::string& label, const sol::table& v, int flags)
	{
		const lua_Number	v1{ v[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v2{ v[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v3{ v[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			v4{ v[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		int value[4] = { int(v1), int(v2), int(v3), int(v4) };
		bool used = ImGui::InputInt4(label.c_str(), value, static_cast<ImGuiInputTextFlags>(flags));

		sol::as_table_t int4 = sol::as_table(std::vector<int>{
			value[0], value[1], value[2], value[3]
		});

		return std::make_tuple(int4, used);
	}
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v) { bool selected = ImGui::InputDouble(label.c_str(), &v); return std::make_tuple(v, selected); }
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step) { bool selected = ImGui::InputDouble(label.c_str(), &v, step); return std::make_tuple(v, selected); }
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step, double step_fast) { bool selected = ImGui::InputDouble(label.c_str(), &v, step, step_fast); return std::make_tuple(v, selected); }
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step, double step_fast, const std::string& format) { bool selected = ImGui::InputDouble(label.c_str(), &v, step, step_fast, format.c_str()); return std::make_tuple(v, selected); }
	inline std::tuple<double, bool> InputDouble(const std::string& label, double v, double step, double step_fast, const std::string& format, int flags) { bool selected = ImGui::InputDouble(label.c_str(), &v, step, step_fast, format.c_str(), static_cast<ImGuiInputTextFlags>(flags)); return std::make_tuple(v, selected); }
	inline void InputScalar() { /* TODO: InputScalar(...) ==> UNSUPPORTED */ }
	inline void InputScalarN() { /* TODO: InputScalarN(...) ==> UNSUPPORTED */ }

	// Widgets: Color Editor / Picker
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit3(const std::string& label, const sol::table& col)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[3] = { float(r), float(g), float(b) };
		bool used = ImGui::ColorEdit3(label.c_str(), color);

		sol::as_table_t rgb = sol::as_table(std::vector<float>{
			color[0], color[1], color[2]
		});

		return std::make_tuple(rgb, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit3(const std::string& label, const sol::table& col, int flags)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[3] = { float(r), float(g), float(b) };
		bool used = ImGui::ColorEdit3(label.c_str(), color, static_cast<ImGuiColorEditFlags>(flags));

		sol::as_table_t rgb = sol::as_table(std::vector<float>{
			color[0], color[1], color[2]
		});

		return std::make_tuple(rgb, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit4(const std::string& label, const sol::table& col)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[4] = { float(r), float(g), float(b), float(a) };
		bool used = ImGui::ColorEdit4(label.c_str(), color);

		sol::as_table_t rgba = sol::as_table(std::vector<float>{
			color[0], color[1], color[2], color[3]
		});

		return std::make_tuple(rgba, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorEdit4(const std::string& label, const sol::table& col, int flags)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[4] = { float(r), float(g), float(b), float(a) };
		bool used = ImGui::ColorEdit4(label.c_str(), color, static_cast<ImGuiColorEditFlags>(flags));

		sol::as_table_t rgba = sol::as_table(std::vector<float>{
			color[0], color[1], color[2], color[3]
		});

		return std::make_tuple(rgba, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker3(const std::string& label, const sol::table& col)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[3] = { float(r), float(g), float(b) };
		bool used = ImGui::ColorPicker3(label.c_str(), color);

		sol::as_table_t rgb = sol::as_table(std::vector<float>{
			color[0], color[1], color[2]
		});

		return std::make_tuple(rgb, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker3(const std::string& label, const sol::table& col, int flags)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[3] = { float(r), float(g), float(b) };
		bool used = ImGui::ColorPicker3(label.c_str(), color, static_cast<ImGuiColorEditFlags>(flags));

		sol::as_table_t rgb = sol::as_table(std::vector<float>{
			color[0], color[1], color[2]
		});

		return std::make_tuple(rgb, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker4(const std::string& label, const sol::table& col)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[4] = { float(r), float(g), float(b), float(a) };
		bool used = ImGui::ColorPicker4(label.c_str(), color);

		sol::as_table_t rgba = sol::as_table(std::vector<float>{
			color[0], color[1], color[2], color[3]
		});

		return std::make_tuple(rgba, used);
	}
	inline std::tuple<sol::as_table_t<std::vector<float>>, bool> ColorPicker4(const std::string& label, const sol::table& col, int flags)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		float color[4] = { float(r), float(g), float(b), float(a) };
		bool used = ImGui::ColorPicker4(label.c_str(), color, static_cast<ImGuiColorEditFlags>(flags));

		sol::as_table_t rgba = sol::as_table(std::vector<float>{
			color[0], color[1], color[2], color[3]
		});

		return std::make_tuple(rgba, used);
	}
	inline bool ColorButton(const std::string& desc_id, const sol::table& col)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		const ImVec4 color{ float(r), float(g), float(b), float(a) };
		return ImGui::ColorButton(desc_id.c_str(), color);
	}
	inline bool ColorButton(const std::string& desc_id, const sol::table& col, int flags)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		const ImVec4 color{ float(r), float(g), float(b), float(a) };
		return ImGui::ColorButton(desc_id.c_str(), color, static_cast<ImGuiColorEditFlags>(flags));
	}
	inline bool ColorButton(const std::string& desc_id, const sol::table& col, int flags, float sizeX, float sizeY)
	{
		const lua_Number	r{ col[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ col[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ col[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ col[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };
		const ImVec4 color{ float(r), float(g), float(b), float(a) };
		return ImGui::ColorButton(desc_id.c_str(), color, static_cast<ImGuiColorEditFlags>(flags), { sizeX, sizeY });
	}
	inline void SetColorEditOptions(int flags) { ImGui::SetColorEditOptions(static_cast<ImGuiColorEditFlags>(flags)); }

	// Widgets: Trees
	inline bool TreeNode(const std::string& label) { return ImGui::TreeNode(label.c_str()); }
	inline bool TreeNode(const std::string& label, const std::string& fmt) { return ImGui::TreeNode(label.c_str(), fmt.c_str()); }
	/* TODO: TreeNodeV(...) (2) ==> UNSUPPORTED */
	inline bool TreeNodeEx(const std::string& label) { return ImGui::TreeNodeEx(label.c_str()); }
	inline bool TreeNodeEx(const std::string& label, int flags) { return ImGui::TreeNodeEx(label.c_str(), static_cast<ImGuiTreeNodeFlags>(flags)); }
	inline bool TreeNodeEx(const std::string& label, int flags, const std::string& fmt) { return ImGui::TreeNodeEx(label.c_str(), static_cast<ImGuiTreeNodeFlags>(flags), fmt.c_str()); }
	/* TODO: TreeNodeExV(...) (2) ==> UNSUPPORTED */
	inline void TreePush(const std::string& str_id) { ImGui::TreePush(str_id.c_str()); }
	/* TODO: TreePush(const void*) ==> UNSUPPORTED */
	inline void TreePop() { ImGui::TreePop(); }
	inline float GetTreeNodeToLabelSpacing() { return ImGui::GetTreeNodeToLabelSpacing(); }
	inline bool CollapsingHeader(const std::string& label) { return ImGui::CollapsingHeader(label.c_str()); }
	inline bool CollapsingHeader(const std::string& label, int flags) { return ImGui::CollapsingHeader(label.c_str(), static_cast<ImGuiTreeNodeFlags>(flags)); }
	inline std::tuple<bool, bool> CollapsingHeader(const std::string& label, bool open) { bool notCollapsed = ImGui::CollapsingHeader(label.c_str(), &open); return std::make_tuple(open, notCollapsed); }
	inline std::tuple<bool, bool> CollapsingHeader(const std::string& label, bool open, int flags) { bool notCollapsed = ImGui::CollapsingHeader(label.c_str(), &open, static_cast<ImGuiTreeNodeFlags>(flags)); return std::make_tuple(open, notCollapsed); }
	inline void SetNextItemOpen(bool is_open) { ImGui::SetNextItemOpen(is_open); }
	inline void SetNextItemOpen(bool is_open, int cond) { ImGui::SetNextItemOpen(is_open, static_cast<ImGuiCond>(cond)); }

	// Widgets: Selectables
	// TODO: Only one of Selectable variations is possible due to same parameters for Lua
	inline bool Selectable(const std::string& label) { return ImGui::Selectable(label.c_str()); }
	inline bool Selectable(const std::string& label, bool selected) { ImGui::Selectable(label.c_str(), &selected); return selected; }
	inline bool Selectable(const std::string& label, bool selected, int flags) { ImGui::Selectable(label.c_str(), &selected, static_cast<ImGuiSelectableFlags>(flags)); return selected; }
	inline bool Selectable(const std::string& label, bool selected, int flags, float sizeX, float sizeY) { ImGui::Selectable(label.c_str(), &selected, static_cast<ImGuiSelectableFlags>(flags), { sizeX, sizeY }); return selected; }

	// Widgets: List Boxes
	inline std::tuple<int, bool> ListBox(const std::string& label, int current_item, const sol::table& items, int items_count)
	{
		std::vector<std::string> strings;
		for (int i{ 1 }; i <= items_count; i++)
		{
			const auto& stringItem = items.get<sol::optional<std::string>>(i);
			strings.push_back(stringItem.value_or("Missing"));
		}

		std::vector<const char*> cstrings;
		for (auto& string : strings)
			cstrings.push_back(string.c_str());

		bool clicked = ImGui::ListBox(label.c_str(), &current_item, cstrings.data(), items_count);
		return std::make_tuple(current_item, clicked);
	}
	inline std::tuple<int, bool> ListBox(const std::string& label, int current_item, const sol::table& items, int items_count, int height_in_items)
	{
		std::vector<std::string> strings;
		for (int i{ 1 }; i <= items_count; i++)
		{
			const auto& stringItem = items.get<sol::optional<std::string>>(i);
			strings.push_back(stringItem.value_or("Missing"));
		}

		std::vector<const char*> cstrings;
		for (auto& string : strings)
			cstrings.push_back(string.c_str());

		bool clicked = ImGui::ListBox(label.c_str(), &current_item, cstrings.data(), items_count, height_in_items);
		return std::make_tuple(current_item, clicked);
	}
	inline bool ListBoxHeader(const std::string& label, float sizeX, float sizeY) { return ImGui::ListBoxHeader(label.c_str(), { sizeX, sizeY }); }
	inline bool ListBoxHeader(const std::string& label, int items_count) { return ImGui::ListBoxHeader(label.c_str(), items_count); }
	inline bool ListBoxHeader(const std::string& label, int items_count, int height_in_items) { return ImGui::ListBoxHeader(label.c_str(), items_count, height_in_items); }
	inline void ListBoxFooter() { ImGui::ListBoxFooter(); }

	// Widgets: Data Plotting
	/* TODO: Widgets Data Plotting ==> UNSUPPORTED (barely used and quite long functions) */

	// Widgets: Value() helpers
	inline void Value(const std::string& prefix, bool b) { ImGui::Value(prefix.c_str(), b); }
	inline void Value(const std::string& prefix, int v) { ImGui::Value(prefix.c_str(), v); }
	inline void Value(const std::string& prefix, unsigned int v) { ImGui::Value(prefix.c_str(), v); }
	inline void Value(const std::string& prefix, float v) { ImGui::Value(prefix.c_str(), v); }
	inline void Value(const std::string& prefix, float v, const std::string& float_format) { ImGui::Value(prefix.c_str(), v, float_format.c_str()); }

	// Widgets: Menus
	inline bool BeginMenuBar() { return ImGui::BeginMenuBar(); }
	inline void EndMenuBar() { ImGui::EndMenuBar(); }
	inline bool BeginMainMenuBar() { return ImGui::BeginMainMenuBar(); }
	inline void EndMainMenuBar() { ImGui::EndMainMenuBar(); }
	inline bool BeginMenu(const std::string& label) { return ImGui::BeginMenu(label.c_str()); }
	inline bool BeginMenu(const std::string& label, bool enabled) { return ImGui::BeginMenu(label.c_str(), enabled); }
	inline void EndMenu() { ImGui::EndMenu(); }
	inline bool MenuItem(const std::string& label) { return ImGui::MenuItem(label.c_str()); }
	inline bool MenuItem(const std::string& label, const std::string& shortcut) { return ImGui::MenuItem(label.c_str(), shortcut.c_str()); }
	inline std::tuple<bool, bool> MenuItem(const std::string& label, const std::string& shortcut, bool selected) { bool activated = ImGui::MenuItem(label.c_str(), shortcut.c_str(), &selected); return std::make_tuple(selected, activated); }
	inline std::tuple<bool, bool> MenuItem(const std::string& label, const std::string& shortcut, bool selected, bool enabled) { bool activated = ImGui::MenuItem(label.c_str(), shortcut.c_str(), &selected, enabled); return std::make_tuple(selected, activated); }

	// Tooltips
	inline void BeginTooltip() { ImGui::BeginTooltip(); }
	inline void EndTooltip() { ImGui::EndTooltip(); }
	inline void SetTooltip(const std::string& fmt) { ImGui::SetTooltip(fmt.c_str()); }
	inline void SetTooltipV() { /* TODO: SetTooltipV(...) ==> UNSUPPORTED */ }

	// Popups, Modals
	inline bool BeginPopup(const std::string& str_id) { return ImGui::BeginPopup(str_id.c_str()); }
	inline bool BeginPopup(const std::string& str_id, int flags) { return ImGui::BeginPopup(str_id.c_str(), static_cast<ImGuiWindowFlags>(flags)); }
	inline bool BeginPopupModal(const std::string& name) { return ImGui::BeginPopupModal(name.c_str()); }
	inline bool BeginPopupModal(const std::string& name, bool open) { return ImGui::BeginPopupModal(name.c_str(), &open); }
	inline bool BeginPopupModal(const std::string& name, bool open, int flags) { return ImGui::BeginPopupModal(name.c_str(), &open, static_cast<ImGuiWindowFlags>(flags)); }
	inline void EndPopup() { ImGui::EndPopup(); }
	inline void OpenPopup(const std::string& str_id) { ImGui::OpenPopup(str_id.c_str()); }
	inline void OpenPopup(const std::string& str_id, int popup_flags) { ImGui::OpenPopup(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline void OpenPopupContextItem() { return ImGui::OpenPopupContextItem(); }
	inline void OpenPopupContextItem(const std::string& str_id) { return ImGui::OpenPopupContextItem(str_id.c_str()); }
	inline void OpenPopupContextItem(const std::string& str_id, int popup_flags) { return ImGui::OpenPopupContextItem(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline void CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }
	inline bool BeginPopupContextItem() { return ImGui::BeginPopupContextItem(); }
	inline bool BeginPopupContextItem(const std::string& str_id) { return ImGui::BeginPopupContextItem(str_id.c_str()); }
	inline bool BeginPopupContextItem(const std::string& str_id, int popup_flags) { return ImGui::BeginPopupContextItem(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline bool BeginPopupContextWindow() { return ImGui::BeginPopupContextWindow(); }
	inline bool BeginPopupContextWindow(const std::string& str_id) { return ImGui::BeginPopupContextWindow(str_id.c_str()); }
	inline bool BeginPopupContextWindow(const std::string& str_id, int popup_flags) { return ImGui::BeginPopupContextWindow(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline bool BeginPopupContextVoid() { return ImGui::BeginPopupContextVoid(); }
	inline bool BeginPopupContextVoid(const std::string& str_id) { return ImGui::BeginPopupContextVoid(str_id.c_str()); }
	inline bool BeginPopupContextVoid(const std::string& str_id, int popup_flags) { return ImGui::BeginPopupContextVoid(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline bool IsPopupOpen(const std::string& str_id) { return ImGui::IsPopupOpen(str_id.c_str()); }
	inline bool IsPopupOpen(const std::string& str_id, int popup_flags) { return ImGui::IsPopupOpen(str_id.c_str(), popup_flags); }

	// Columns
	inline void Columns() { ImGui::Columns(); }
	inline void Columns(int count) { ImGui::Columns(count); }
	inline void Columns(int count, const std::string& id) { ImGui::Columns(count, id.c_str()); }
	inline void Columns(int count, const std::string& id, bool border) { ImGui::Columns(count, id.c_str(), border); }
	inline void NextColumn() { ImGui::NextColumn(); }
	inline int GetColumnIndex() { return ImGui::GetColumnIndex(); }
	inline float GetColumnWidth() { return ImGui::GetColumnWidth(); }
	inline float GetColumnWidth(int column_index) { return ImGui::GetColumnWidth(column_index); }
	inline void SetColumnWidth(int column_index, float width) { ImGui::SetColumnWidth(column_index, width); }
	inline float GetColumnOffset() { return ImGui::GetColumnOffset(); }
	inline float GetColumnOffset(int column_index) { return ImGui::GetColumnOffset(column_index); }
	inline void SetColumnOffset(int column_index, float offset_x) { ImGui::SetColumnOffset(column_index, offset_x); }
	inline int GetColumnsCount() { return ImGui::GetColumnsCount(); }

	// Tab Bars, Tabs
	inline bool BeginTabBar(const std::string& str_id) { return ImGui::BeginTabBar(str_id.c_str()); }
	inline bool BeginTabBar(const std::string& str_id, int flags) { return ImGui::BeginTabBar(str_id.c_str(), static_cast<ImGuiTabBarFlags>(flags)); }
	inline void EndTabBar() { ImGui::EndTabBar(); }
	inline bool BeginTabItem(const std::string& label) { return ImGui::BeginTabItem(label.c_str()); }
	inline std::tuple<bool, bool> BeginTabItem(const std::string& label, bool open) { bool selected = ImGui::BeginTabItem(label.c_str(), &open); return std::make_tuple(open, selected); }
	inline std::tuple<bool, bool> BeginTabItem(const std::string& label, bool open, int flags) { bool selected = ImGui::BeginTabItem(label.c_str(), &open, static_cast<ImGuiTabItemFlags>(flags)); return std::make_tuple(open, selected); }
	inline void EndTabItem() { ImGui::EndTabItem(); }
	inline void SetTabItemClosed(const std::string& tab_or_docked_window_label) { ImGui::SetTabItemClosed(tab_or_docked_window_label.c_str()); }
#if defined(IMGUI_HAS_DOCK)
	// Docking
	inline void DockSpace(unsigned int id) { ImGui::DockSpace(id); }
	inline void DockSpace(unsigned int id, float sizeX, float sizeY) { ImGui::DockSpace(id, { sizeX, sizeY }); }
	inline void DockSpace(unsigned int id, float sizeX, float sizeY, int flags) { ImGui::DockSpace(id, { sizeX, sizeY }, static_cast<ImGuiDockNodeFlags>(flags)); }
	inline unsigned int DockSpaceOverViewport() { return 0; /* TODO: DockSpaceOverViwport(...) ==> UNSUPPORTED */ }
	inline void SetNextWindowDockID(unsigned int dock_id) { ImGui::SetNextWindowDockID(dock_id); }
	inline void SetNextWindowDockID(unsigned int dock_id, int cond) { ImGui::SetNextWindowDockID(dock_id, static_cast<ImGuiCond>(cond)); }
	inline void SetNextWindowClass() {  /* TODO: SetNextWindowClass(...) ==> UNSUPPORTED */ }
	inline unsigned int GetWindowDockID() { return ImGui::GetWindowDockID(); }
	inline bool IsWindowDocked() { return ImGui::IsWindowDocked(); }
#endif
	// Logging
	inline void LogToTTY() { ImGui::LogToTTY(); }
	inline void LogToTTY(int auto_open_depth) { ImGui::LogToTTY(auto_open_depth); }
	inline void LogToFile() { ImGui::LogToFile(); }
	inline void LogToFile(int auto_open_depth) { ImGui::LogToFile(auto_open_depth); }
	inline void LogToFile(int auto_open_depth, const std::string& filename) { ImGui::LogToFile(auto_open_depth, filename.c_str()); }
	inline void LogToClipboard() { ImGui::LogToClipboard(); }
	inline void LogToClipboard(int auto_open_depth) { ImGui::LogToClipboard(auto_open_depth); }
	inline void LogFinish() { ImGui::LogFinish(); }
	inline void LogButtons() { ImGui::LogButtons(); }
	inline void LogText(const std::string& fmt) { ImGui::LogText(fmt.c_str()); }

	// Drag and Drop
	// TODO: Drag and Drop ==> UNSUPPORTED

	// Clipping
	inline void PushClipRect(float min_x, float min_y, float max_x, float max_y, bool intersect_current) { ImGui::PushClipRect({ min_x, min_y }, { max_x, max_y }, intersect_current); }
	inline void PopClipRect() { ImGui::PopClipRect(); }

	// Focus, Activation
	inline void SetItemDefaultFocus() { ImGui::SetItemDefaultFocus(); }
	inline void SetKeyboardFocusHere() { ImGui::SetKeyboardFocusHere(); }
	inline void SetKeyboardFocusHere(int offset) { ImGui::SetKeyboardFocusHere(offset); }

	// Item/Widgets Utilities
	inline bool IsItemHovered() { return ImGui::IsItemHovered(); }
	inline bool IsItemHovered(int flags) { return ImGui::IsItemHovered(static_cast<ImGuiHoveredFlags>(flags)); }
	inline bool IsItemActive() { return ImGui::IsItemActive(); }
	inline bool IsItemFocused() { return ImGui::IsItemFocused(); }
	inline bool IsItemClicked() { return ImGui::IsItemClicked(); }
	inline bool IsItemClicked(int mouse_button) { return ImGui::IsItemClicked(static_cast<ImGuiMouseButton>(mouse_button)); }
	inline bool IsItemVisible() { return ImGui::IsItemVisible(); }
	inline bool IsItemEdited() { return ImGui::IsItemEdited(); }
	inline bool IsItemActivated() { return ImGui::IsItemActivated(); }
	inline bool IsItemDeactivated() { return ImGui::IsItemDeactivated(); }
	inline bool IsItemDeactivatedAfterEdit() { return ImGui::IsItemDeactivatedAfterEdit(); }
	inline bool IsItemToggledOpen() { return ImGui::IsItemToggledOpen(); }
	inline bool IsAnyItemHovered() { return ImGui::IsAnyItemHovered(); }
	inline bool IsAnyItemActive() { return ImGui::IsAnyItemActive(); }
	inline bool IsAnyItemFocused() { return ImGui::IsAnyItemFocused(); }
	inline std::tuple<float, float> GetItemRectMin() { const auto vec2{ ImGui::GetItemRectMin() }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetItemRectMax() { const auto vec2{ ImGui::GetItemRectMax() }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetItemRectSize() { const auto vec2{ ImGui::GetItemRectSize() }; return std::make_tuple(vec2.x, vec2.y); }
	inline void SetItemAllowOverlap() { ImGui::SetItemAllowOverlap(); }

	// Miscellaneous Utilities
	inline bool IsRectVisible(float sizeX, float sizeY) { return ImGui::IsRectVisible({ sizeX, sizeY }); }
	inline bool IsRectVisible(float minX, float minY, float maxX, float maxY) { return ImGui::IsRectVisible({ minX, minY }, { maxX, maxY }); }
	inline double GetTime() { return ImGui::GetTime(); }
	inline int GetFrameCount() { return ImGui::GetFrameCount(); }
	/* TODO: GetBackgroundDrawList(), GetForeGroundDrawList(), GetDrawListSharedData() ==> UNSUPPORTED */
	inline std::string GetStyleColorName(int idx) { return std::string(ImGui::GetStyleColorName(static_cast<ImGuiCol>(idx))); }
	/* TODO: SetStateStorage(), GetStateStorage(), CalcListClipping() ==> UNSUPPORTED */
	inline bool BeginChildFrame(unsigned int id, float sizeX, float sizeY) { return ImGui::BeginChildFrame(id, { sizeX, sizeY }); }
	inline bool BeginChildFrame(unsigned int id, float sizeX, float sizeY, int flags) { return ImGui::BeginChildFrame(id, { sizeX, sizeY }, static_cast<ImGuiWindowFlags>(flags)); }
	inline void EndChildFrame() { return ImGui::EndChildFrame(); }

	// Text Utilities
	inline std::tuple<float, float> CalcTextSize(const std::string& text) { const auto vec2{ ImGui::CalcTextSize(text.c_str()) }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> CalcTextSize(const std::string& text, const std::string& text_end) { const auto vec2{ ImGui::CalcTextSize(text.c_str(), text_end.c_str()) }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> CalcTextSize(const std::string& text, const std::string& text_end, bool hide_text_after_double_hash) { const auto vec2{ ImGui::CalcTextSize(text.c_str(), text_end.c_str(), hide_text_after_double_hash) }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> CalcTextSize(const std::string& text, const std::string& text_end, bool hide_text_after_double_hash, float wrap_width) { const auto vec2{ ImGui::CalcTextSize(text.c_str(), text_end.c_str(), hide_text_after_double_hash, wrap_width) }; return std::make_tuple(vec2.x, vec2.y); }

	// Color Utilities
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline sol::as_table_t<std::vector<float>> ColorConvertU32ToFloat4(unsigned int in)
	{
		const auto vec4 = ImGui::ColorConvertU32ToFloat4(in);
		sol::as_table_t rgba = sol::as_table(std::vector<float>{
			vec4.x, vec4.y, vec4.z, vec4.w
		});

		return rgba;
	}
	inline unsigned int ColorConvertFloat4ToU32(const sol::table& rgba)
	{
		const lua_Number	r{ rgba[1].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			g{ rgba[2].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			b{ rgba[3].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) },
			a{ rgba[4].get<std::optional<lua_Number>>().value_or(static_cast<lua_Number>(0)) };

		return ImGui::ColorConvertFloat4ToU32({ float(r), float(g), float(b), float(a) });
	}
#endif
	inline std::tuple<float, float, float> ColorConvertRGBtoHSV(float r, float g, float b)
	{
		float h{}, s{}, v{};
		ImGui::ColorConvertRGBtoHSV(r, g, b, h, s, v);
		return std::make_tuple(h, s, v);
	}
	inline std::tuple<float, float, float> ColorConvertHSVtoRGB(float h, float s, float v)
	{
		float r{}, g{}, b{};
		ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
		return std::make_tuple(r, g, b);
	}

	// Inputs Utilities: Keyboard
	inline int GetKeyIndex(int imgui_key) { return ImGui::GetKeyIndex(static_cast<ImGuiKey>(imgui_key)); }
	inline bool IsKeyDown(int user_key_index) { return ImGui::IsKeyDown(user_key_index); }
	inline bool IsKeyPressed(int user_key_index) { return ImGui::IsKeyPressed(user_key_index); }
	inline bool IsKeyPressed(int user_key_index, bool repeat) { return ImGui::IsKeyPressed(user_key_index, repeat); }
	inline bool IsKeyReleased(int user_key_index) { return ImGui::IsKeyReleased(user_key_index); }
	inline int GetKeyPressedAmount(int key_index, float repeat_delay, float rate) { return ImGui::GetKeyPressedAmount(key_index, repeat_delay, rate); }
	inline void CaptureKeyboardFromApp() { ImGui::CaptureKeyboardFromApp(); }
	inline void CaptureKeyboardFromApp(bool want_capture_keyboard_value) { ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value); }

	// Inputs Utilities: Mouse
	inline bool IsMouseDown(int button) { return ImGui::IsMouseDown(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseClicked(int button) { return ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseClicked(int button, bool repeat) { return ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(button), repeat); }
	inline bool IsMouseReleased(int button) { return ImGui::IsMouseReleased(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseDoubleClicked(int button) { return ImGui::IsMouseDoubleClicked(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseHoveringRect(float min_x, float min_y, float max_x, float max_y) { return ImGui::IsMouseHoveringRect({ min_x, min_y }, { max_x, max_y }); }
	inline bool IsMouseHoveringRect(float min_x, float min_y, float max_x, float max_y, bool clip) { return ImGui::IsMouseHoveringRect({ min_x, min_y }, { max_x, max_y }, clip); }
	inline bool IsMousePosValid() { return false; /* TODO: IsMousePosValid() ==> UNSUPPORTED */ }
	inline bool IsAnyMouseDown() { return ImGui::IsAnyMouseDown(); }
	inline std::tuple<float, float> GetMousePos() { const auto vec2{ ImGui::GetMousePos() }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetMousePosOnOpeningCurrentPopup() { const auto vec2{ ImGui::GetMousePosOnOpeningCurrentPopup() }; return std::make_tuple(vec2.x, vec2.y); }
	inline bool IsMouseDragging(int button) { return ImGui::IsMouseDragging(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseDragging(int button, float lock_threshold) { return ImGui::IsMouseDragging(static_cast<ImGuiMouseButton>(button), lock_threshold); }
	inline std::tuple<float, float> GetMouseDragDelta() { const auto vec2{ ImGui::GetMouseDragDelta() }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetMouseDragDelta(int button) { const auto vec2{ ImGui::GetMouseDragDelta(static_cast<ImGuiMouseButton>(button)) }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetMouseDragDelta(int button, float lock_threshold) { const auto vec2{ ImGui::GetMouseDragDelta(static_cast<ImGuiMouseButton>(button), lock_threshold) }; return std::make_tuple(vec2.x, vec2.y); }
	inline void ResetMouseDragDelta() { ImGui::ResetMouseDragDelta(); }
	inline void ResetMouseDragDelta(int button) { ImGui::ResetMouseDragDelta(static_cast<ImGuiMouseButton>(button)); }
	inline int GetMouseCursor() { return ImGui::GetMouseCursor(); }
	inline void SetMouseCursor(int cursor_type) { ImGui::SetMouseCursor(static_cast<ImGuiMouseCursor>(cursor_type)); }
	inline void CaptureMouseFromApp() { ImGui::CaptureMouseFromApp(); }
	inline void CaptureMouseFromApp(bool want_capture_mouse_value) { ImGui::CaptureMouseFromApp(want_capture_mouse_value); }

	// Clipboard Utilities
	inline std::string GetClipboardText() { return std::string(ImGui::GetClipboardText()); }
	inline void SetClipboardText(const std::string& text) { ImGui::SetClipboardText(text.c_str()); }

	inline void InitEnums(sol::state& lua)
	{
#pragma region Window Flags
		//@section ImGuiWindowFlags
		/***
		Enum with a list of ImGui Window Flags.

		@tfield int None
		@tfield int NoTitleBar
		@tfield int NoResize
		@tfield int NoMove
		@tfield int NoScrollbar
		@tfield int NoScrollWithMouse
		@tfield int NoCollapse
		@tfield int AlwaysAutoResize
		@tfield int NoBackground
		@tfield int NoSavedSettings
		@tfield int NoMouseInputs
		@tfield int MenuBar
		@tfield int HorizontalScrollbar
		@tfield int NoFocusOnAppearing
		@tfield int NoBringToFrontOnFocus
		@tfield int AlwaysVerticalScrollbar
		@tfield int AlwaysHorizontalScrollbar
		@tfield int AlwaysUseWindowPadding
		@tfield int NoNavInputs
		@tfield int NoNavFocus
		@tfield int UnsavedDocument
		@tfield int NoDocking
		@tfield int DockNodeHost
		@tfield int NoClose
		@tfield int NoDockTab
		@tfield int NoNav
		@tfield int NoDecoration
		@tfield int NoInputs
		@tfield int NavFlattened
		@tfield int ChildWindow
		@tfield int Tooltip
		@tfield int Popup
		@tfield int Modal
		@tfield int ChildMenu

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
			"AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding,
			"NoNavInputs", ImGuiWindowFlags_NoNavInputs,
			"NoNavFocus", ImGuiWindowFlags_NoNavFocus,
			"UnsavedDocument", ImGuiWindowFlags_UnsavedDocument,
#if defined(IMGUI_HAS_DOCK)
			"NoDocking", ImGuiWindowFlags_NoDocking,
#endif
#ifdef CUSTOM_IMGUI
			"DockNodeHost", ImGuiWindowFlags_DockNodeHost,
			"NoClose", ImGuiWindowFlags_NoClose,
			"NoDockTab", ImGuiWindowFlags_NoDockTab,
#else
#if defined(IMGUI_HAS_DOCK)
			"DockNodeHost", ImGuiWindowFlags_DockNodeHost,
#endif
#endif
			"NoNav", ImGuiWindowFlags_NoNav,
			"NoDecoration", ImGuiWindowFlags_NoDecoration,
			"NoInputs", ImGuiWindowFlags_NoInputs,

			"NavFlattened", ImGuiWindowFlags_NavFlattened,
			"ChildWindow", ImGuiWindowFlags_ChildWindow,
			"Tooltip", ImGuiWindowFlags_Tooltip,
			"Popup", ImGuiWindowFlags_Popup,
			"Modal", ImGuiWindowFlags_Modal,
			"ChildMenu", ImGuiWindowFlags_ChildMenu


		);
#pragma endregion Window Flags

		//@section ImGuiFocusedFlags
		/***
		Enum with a list of ImGui Focused Flags.

		@tfield int None
		@tfield int ChildWindows
		@tfield int RootWindow
		@tfield int AnyWindow
		@tfield int RootAndChildWindows

		@table ImGuiFocusedFlags
		*/
#pragma region Focused Flags
		lua.new_enum("ImGuiFocusedFlags",
			"None", ImGuiFocusedFlags_None,
			"ChildWindows", ImGuiFocusedFlags_ChildWindows,
			"RootWindow", ImGuiFocusedFlags_RootWindow,
			"AnyWindow", ImGuiFocusedFlags_AnyWindow,
			"RootAndChildWindows", ImGuiFocusedFlags_RootAndChildWindows
		);
#pragma endregion Focused Flags

		//@section ImGuiHoveredFlags
		/***
		Enum with a list of ImGui Hovered Flags.

		@tfield int None
		@tfield int ChildWindows
		@tfield int RootWindow
		@tfield int AnyWindow
		@tfield int AllowWhenBlockedByPopup
		@tfield int AllowWhenBlockedByActiveItem
		@tfield int AllowWhenOverlapped
		@tfield int AllowWhenDisabled
		@tfield int RectOnly
		@tfield int RootAndChildWindows

		@table ImGuiHoveredFlags
		*/
#pragma region Hovered Flags
		lua.new_enum("ImGuiHoveredFlags",
			"None", ImGuiHoveredFlags_None,
			"ChildWindows", ImGuiHoveredFlags_ChildWindows,
			"RootWindow", ImGuiHoveredFlags_RootWindow,
			"AnyWindow", ImGuiHoveredFlags_AnyWindow,
			"AllowWhenBlockedByPopup", ImGuiHoveredFlags_AllowWhenBlockedByPopup,
			"AllowWhenBlockedByActiveItem", ImGuiHoveredFlags_AllowWhenBlockedByActiveItem,
			"AllowWhenOverlapped", ImGuiHoveredFlags_AllowWhenOverlapped,
			"AllowWhenDisabled", ImGuiHoveredFlags_AllowWhenDisabled,
			"RectOnly", ImGuiHoveredFlags_RectOnly,
			"RootAndChildWindows", ImGuiHoveredFlags_RootAndChildWindows
		);
#pragma endregion Hovered Flags

		//@section ImGuiCond
		/***
		Enum with a list of ImGui Conditions.

		@tfield int None
		@tfield int Always
		@tfield int Once
		@tfield int FirstUseEver
		@tfield int Appearing

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
		@tfield int WindowBg
		@tfield int ChildBg
		@tfield int PopupBg
		@tfield int Border
		@tfield int BorderShadow
		@tfield int FrameBg
		@tfield int FrameBgHovered
		@tfield int FrameBgActive
		@tfield int TitleBg
		@tfield int TitleBgActive
		@tfield int TitleBgCollapsed
		@tfield int MenuBarBg
		@tfield int ScrollbarBg
		@tfield int ScrollbarGrab
		@tfield int ScrollbarGrabHovered
		@tfield int ScrollbarGrabActive
		@tfield int CheckMark
		@tfield int SliderGrab
		@tfield int SliderGrabActive
		@tfield int Button
		@tfield int ButtonHovered
		@tfield int ButtonActive
		@tfield int Header
		@tfield int HeaderHovered
		@tfield int HeaderActive
		@tfield int Separator
		@tfield int SeparatorHovered
		@tfield int SeparatorActive
		@tfield int ResizeGrip
		@tfield int ResizeGripHovered
		@tfield int ResizeGripActive
		@tfield int Tab
		@tfield int TabHovered
		@tfield int TabActive
		@tfield int TabUnfocused
		@tfield int TabUnfocusedActive
		@tfield int DockingPreview
		@tfield int DockingEmptyBg
		@tfield int PlotLines
		@tfield int PlotLinesHovered
		@tfield int PlotHistogram
		@tfield int PlotHistogramHovered
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
#if defined(IMGUI_HAS_DOCK)
			"DockingPreview", ImGuiCol_DockingPreview,
			"DockingEmptyBg", ImGuiCol_DockingEmptyBg,
#endif
			"PlotLines", ImGuiCol_PlotLines,
			"PlotLinesHovered", ImGuiCol_PlotLinesHovered,
			"PlotHistogram", ImGuiCol_PlotHistogram,
			"PlotHistogramHovered", ImGuiCol_PlotHistogramHovered,
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

		@tfield int Alpha
		@tfield int WindowPadding
		@tfield int WindowRounding
		@tfield int WindowBorderSize
		@tfield int WindowMinSize
		@tfield int WindowTitleAlign
		@tfield int ChildRounding
		@tfield int ChildBorderSize
		@tfield int PopupRounding
		@tfield int PopupBorderSize
		@tfield int FramePadding
		@tfield int FrameRounding
		@tfield int FrameBorderSize
		@tfield int ItemSpacing
		@tfield int ItemInnerSpacing
		@tfield int IndentSpacing
		@tfield int ScrollbarSize
		@tfield int ScrollbarRounding
		@tfield int GrabMinSize
		@tfield int GrabRounding
		@tfield int TabRounding
		@tfield int SelectableTextAlign
		@tfield int ButtonTextAlign
		@tfield int COUNT

		@table ImGuiStyleVar
		*/
#pragma region Style
		lua.new_enum("ImGuiStyleVar",
			"Alpha", ImGuiStyleVar_Alpha,
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
			"ScrollbarSize", ImGuiStyleVar_ScrollbarSize,
			"ScrollbarRounding", ImGuiStyleVar_ScrollbarRounding,
			"GrabMinSize", ImGuiStyleVar_GrabMinSize,
			"GrabRounding", ImGuiStyleVar_GrabRounding,
			"TabRounding", ImGuiStyleVar_TabRounding,
			"SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign,
			"ButtonTextAlign", ImGuiStyleVar_ButtonTextAlign,
			"COUNT", ImGuiStyleVar_COUNT
		);
#pragma endregion Style

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

		//@section ImGuiComboFlags
		/***
		Enum with a list of ImGui Combo Flags.

		@tfield int None
		@tfield int PopupAlignLeft
		@tfield int HeightSmall
		@tfield int HeightRegular
		@tfield int HeightLarge
		@tfield int HeightLargest
		@tfield int NoArrowButton
		@tfield int NoPreview
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
			"HeightMask", ImGuiComboFlags_HeightMask_
		);
#pragma endregion Combo Flags

		//@section ImGuiInputTextFlags
		/***
		Enum with a list of ImGui Input Text Flags.

		@tfield int None
		@tfield int CharsDecimal
		@tfield int CharsHexadecimal
		@tfield int CharsUppercase
		@tfield int CharsNoBlank
		@tfield int AutoSelectAll
		@tfield int EnterReturnsTrue
		@tfield int CallbackCompletion
		@tfield int CallbackHistory
		@tfield int CallbackAlways
		@tfield int CallbackCharFilter
		@tfield int AllowTabInput
		@tfield int CtrlEnterForNewLine
		@tfield int NoHorizontalScroll
		@tfield int AlwaysInsertMode
		@tfield int ReadOnly
		@tfield int Password
		@tfield int NoUndoRedo
		@tfield int CharsScientific
		@tfield int CallbackResize
		@tfield int Multiline
		@tfield int NoMarkEdited

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
			"AlwaysInsertMode", ImGuiInputTextFlags_AlwaysInsertMode,
			"ReadOnly", ImGuiInputTextFlags_ReadOnly,
			"Password", ImGuiInputTextFlags_Password,
			"NoUndoRedo", ImGuiInputTextFlags_NoUndoRedo,
			"CharsScientific", ImGuiInputTextFlags_CharsScientific,
			"CallbackResize", ImGuiInputTextFlags_CallbackResize,
			"Multiline", 1 << 26,
			"NoMarkEdited", 1 << 27
		);
#pragma endregion InputText Flags

		//@section ImGuiColorEditFlags
		/***
		Enum with a list of ImGui Color Edit Flags.

		@tfield int None
		@tfield int NoAlpha
		@tfield int NoPicker
		@tfield int NoOptions
		@tfield int NoSmallPreview
		@tfield int NoInputs
		@tfield int NoTooltip
		@tfield int NoLabel
		@tfield int NoSidePreview
		@tfield int NoDragDrop
		@tfield int NoBorder
		@tfield int AlphaBar
		@tfield int AlphaPreview
		@tfield int AlphaPreviewHalf
		@tfield int HDR
		@tfield int DisplayRGB
		@tfield int DisplayHSV
		@tfield int DisplayHex
		@tfield int Uint8
		@tfield int Float
		@tfield int PickerHueBar
		@tfield int PickerHueWheel
		@tfield int InputRGB
		@tfield int InputHSV
		@tfield int RGB

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

			//	"_OptionsDefault"		, ImGuiColorEditFlags__OptionsDefault,

			//	"_DisplayMask"			, ImGuiColorEditFlags__DisplayMask,
				//"_DataTypeMask"			, ImGuiColorEditFlags__DataTypeMask,
				//"_PickerMask"			, ImGuiColorEditFlags__PickerMask,
			//	"_InputMask"			, ImGuiColorEditFlags__InputMask,

			"RGB", ImGuiColorEditFlags_RGB
		);
#pragma endregion ColorEdit Flags

		//@section ImGuiTreeNodeFlags
		/***
		Enum with a list of ImGui Tree Node Flags.

		@tfield int None
		@tfield int Selected
		@tfield int Framed
		@tfield int AllowItemOverlap
		@tfield int NoTreePushOnOpen
		@tfield int NoAutoOpenOnLog
		@tfield int DefaultOpen
		@tfield int OpenOnDoubleClick
		@tfield int OpenOnArrow
		@tfield int Leaf
		@tfield int Bullet
		@tfield int FramePadding
		@tfield int SpanAvailWidth
		@tfield int SpanFullWidth
		@tfield int NavLeftJumpsBackHere
		@tfield int CollapsingHeader

		@table ImGuiTreeNodeFlags
		*/
#pragma region TreeNode Flags
		lua.new_enum("ImGuiTreeNodeFlags",
			"None", ImGuiTreeNodeFlags_None,
			"Selected", ImGuiTreeNodeFlags_Selected,
			"Framed", ImGuiTreeNodeFlags_Framed,
			"AllowItemOverlap", ImGuiTreeNodeFlags_AllowItemOverlap,
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

		//@section ImGuiSelectableFlags
		/***
		Enum with a list of ImGui Selectable Flags.

		@tfield int None
		@tfield int DontClosePopups
		@tfield int SpanAllColumns
		@tfield int AllowDoubleClick
		@tfield int Disabled
		@tfield int AllowItemOverlap

		@table ImGuiSelectableFlags
		*/
#pragma region Selectable Flags
		lua.new_enum("ImGuiSelectableFlags",
			"None", ImGuiSelectableFlags_None,
			"DontClosePopups", ImGuiSelectableFlags_DontClosePopups,
			"SpanAllColumns", ImGuiSelectableFlags_SpanAllColumns,
			"AllowDoubleClick", ImGuiSelectableFlags_AllowDoubleClick,
			"Disabled", ImGuiSelectableFlags_Disabled,
			"AllowItemOverlap", ImGuiSelectableFlags_AllowItemOverlap
		);
#pragma endregion Selectable Flags

		//@section ImGuiPopupFlags
		/***
		Enum with a list of ImGui Popup Flags.

		@tfield int None
		@tfield int MouseButtonLeft
		@tfield int MouseButtonRight
		@tfield int MouseButtonMiddle
		@tfield int MouseButtonMask_
		@tfield int MouseButtonDefault_
		@tfield int NoOpenOverExistingPopup
		@tfield int NoOpenOverItems
		@tfield int AnyPopupId
		@tfield int AnyPopupLevel
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

		//@section ImGuiTabBarFlags
		/***
		Enum with a list of ImGui TabBar Flags.

		@tfield int None
		@tfield int Reorderable
		@tfield int AutoSelectNewTabs
		@tfield int TabListPopupButton
		@tfield int NoCloseWithMiddleMouseButton
		@tfield int NoTabListScrollingButtons
		@tfield int NoTooltip
		@tfield int FittingPolicyResizeDown
		@tfield int FittingPolicyScroll
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
		@tfield int UnsavedDocument
		@tfield int SetSelected
		@tfield int NoCloseWithMiddleMouseButton
		@tfield int NoTooltip

		@table ImGuiTabItemFlags
		*/
#pragma region TabItem Flags
		lua.new_enum("ImGuiTabItemFlags",
			"None", ImGuiTabItemFlags_None,
			"UnsavedDocument", ImGuiTabItemFlags_UnsavedDocument,
			"SetSelected", ImGuiTabItemFlags_SetSelected,
			"NoCloseWithMiddleMouseButton", ImGuiTabItemFlags_NoCloseWithMiddleMouseButton,
			"NoPushId", ImGuiTabItemFlags_NoPushId,
			"NoTooltip", ImGuiTabItemFlags_NoTooltip
		);
#pragma endregion TabItem Flags




		//@section ImGuiDockNodeFlags
		/***
		Enum with a list of ImGui DockNode Flags.

		@tfield int None
		@tfield int KeepAliveOnly
		@tfield int NoDockingInCentralNode
		@tfield int PassthruCentralNode
		@tfield int NoSplit
		@tfield int NoResize
		@tfield int AutoHideTabBar

		@table ImGuiDockNodeFlags
		*/
#pragma region DockNode Flags
#if defined(IMGUI_HAS_DOCK)
		lua.new_enum("ImGuiDockNodeFlags",
			"None", ImGuiDockNodeFlags_None,
			"KeepAliveOnly", ImGuiDockNodeFlags_KeepAliveOnly,
			"NoDockingInCentralNode", ImGuiDockNodeFlags_NoDockingInCentralNode,
			"PassthruCentralNode", ImGuiDockNodeFlags_PassthruCentralNode,
			"NoSplit", ImGuiDockNodeFlags_NoSplit,
			"NoResize", ImGuiDockNodeFlags_NoResize,
			"AutoHideTabBar", ImGuiDockNodeFlags_AutoHideTabBar
		);
#endif
#pragma endregion DockNode Flags


		//@section ImGuiMouseButton
		/***
		Enum with a list of ImGui MouseButton.

		@tfield int ImGuiMouseButton_Left
		@tfield int ImGuiMouseButton_Right
		@tfield int ImGuiMouseButton_Middle
		@tfield int ImGuiMouseButton_COUNT

		@table ImGuiMouseButton
		*/
#pragma region MouseButton
		lua.new_enum("ImGuiMouseButton",
			"ImGuiMouseButton_Left", ImGuiMouseButton_Left,
			"ImGuiMouseButton_Right", ImGuiMouseButton_Right,
			"ImGuiMouseButton_Middle", ImGuiMouseButton_Middle,
			"ImGuiMouseButton_COUNT", ImGuiMouseButton_COUNT
		);
#pragma endregion MouseButton

		//@section ImGuiKey
		/***
		Enum with a list of ImGui Key.

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
		@tfield int KeyPadEnter
		@tfield int One
		@tfield int Two
		@tfield int Three
		@tfield int Four
		@tfield int Five
		@tfield int Six
		@tfield int Seven
		@tfield int Eight
		@tfield int Nine
		@tfield int Zero
		@tfield int Tilde
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
		@tfield int LeftCtrl
		@tfield int LeftShift
		@tfield int LeftAlt
		@tfield int CapsLock
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
		@tfield int COUNT

		@table ImGuiKey
		*/
#pragma region Key
		lua.new_enum("ImGuiKey",
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
			"KeyPadEnter", ImGuiKey_KeyPadEnter,
			"One", ImGuiKey_One,
			"Two", ImGuiKey_Two,
			"Three", ImGuiKey_Three,
			"Four", ImGuiKey_Four,
			"Five", ImGuiKey_Five,
			"Six", ImGuiKey_Six,
			"Seven", ImGuiKey_Seven,
			"Eight", ImGuiKey_Eight,
			"Nine", ImGuiKey_Nine,
			"Zero", ImGuiKey_Zero,
			"Tilde", ImGuiKey_Tilde,
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
			"LeftCtrl", ImGuiKey_LeftCtrl,
			"LeftShift", ImGuiKey_LeftShift,
			"LeftAlt", ImGuiKey_LeftAlt,
			"CapsLock", ImGuiKey_CapsLock,
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
			"COUNT", ImGuiKey_COUNT
		);
#pragma endregion Key

		//@section ImGuiMouseCursor
		/***
		Enum with a list of ImGui Mouse Cursor.

		@tfield int None
		@tfield int Arrow
		@tfield int TextInput
		@tfield int ResizeAll
		@tfield int ResizeNS
		@tfield int ResizeEW
		@tfield int ResizeNESW
		@tfield int ResizeNWSE
		@tfield int Hand
		@tfield int NotAllowed
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
	}

	inline void Init(sol::state& lua)
	{
		///M2TWEOP ImGui
		//@section ImGuiTable

		/***
		Basic ImGui table

		@tfield Begin Begin
		@tfield End End
		@tfield BeginChild BeginChild
		@tfield EndChild EndChild
		@tfield IsWindowAppearing IsWindowAppearing
		@tfield IsWindowCollapsed IsWindowCollapsed
		@tfield IsWindowFocused IsWindowFocused
		@tfield IsWindowHovered IsWindowHovered
		@tfield GetWindowDpiScale GetWindowDpiScale
		@tfield GetWindowPos GetWindowPos
		@tfield GetWindowSize GetWindowSize
		@tfield GetWindowWidth GetWindowWidth
		@tfield GetWindowHeight GetWindowHeight
		@tfield SetNextWindowPos SetNextWindowPos
		@tfield SetNextWindowSize SetNextWindowSize
		@tfield SetNextWindowSizeConstraints SetNextWindowSizeConstraints
		@tfield SetNextWindowContentSize SetNextWindowContentSize
		@tfield SetNextWindowCollapsed SetNextWindowCollapsed
		@tfield SetNextWindowFocus SetNextWindowFocus
		@tfield SetNextWindowBgAlpha SetNextWindowBgAlpha
		@tfield SetWindowPos SetWindowPos
		@tfield SetWindowSize SetWindowSize
		@tfield SetWindowCollapsed SetWindowCollapsed
		@tfield SetWindowFocus SetWindowFocus
		@tfield SetWindowFontScale SetWindowFontScale
		@tfield GetContentRegionMax GetContentRegionMax
		@tfield GetContentRegionAvail GetContentRegionAvail
		@tfield GetWindowContentRegionMin GetWindowContentRegionMin
		@tfield GetWindowContentRegionMax GetWindowContentRegionMax
		@tfield GetWindowContentRegionWidth GetWindowContentRegionWidth
		@tfield GetScrollX GetScrollX
		@tfield GetScrollY GetScrollY
		@tfield GetScrollMaxX GetScrollMaxX
		@tfield GetScrollMaxY GetScrollMaxY
		@tfield SetScrollX SetScrollX
		@tfield SetScrollY SetScrollY
		@tfield SetScrollHereX SetScrollHereX
		@tfield SetScrollHereY SetScrollHereY
		@tfield SetScrollFromPosX SetScrollFromPosX
		@tfield SetScrollFromPosY SetScrollFromPosY
		@tfield PushStyleVar PushStyleVar
		@tfield PopStyleVar PopStyleVar
		@tfield PushFont PushFont
		@tfield PopFont PopFont
		@tfield PushStyleColor PushStyleColor
		@tfield PopStyleColor PopStyleColor
		@tfield GetStyleColorVec4 GetStyleColorVec4
		@tfield GetFont GetFont
		@tfield GetFontSize GetFontSize
		@tfield GetFontTexUvWhitePixel GetFontTexUvWhitePixel
		@tfield PushItemWidth PushItemWidth
		@tfield PopItemWidth PopItemWidth
		@tfield SetNextItemWidth SetNextItemWidth
		@tfield CalcItemWidth CalcItemWidth
		@tfield PushTextWrapPos PushTextWrapPos
		@tfield PopTextWrapPos PopTextWrapPos
		@tfield PushAllowKeyboardFocus PushAllowKeyboardFocus
		@tfield PopAllowKeyboardFocus PopAllowKeyboardFocus
		@tfield PushButtonRepeat PushButtonRepeat
		@tfield PopButtonRepeat PopButtonRepeat
		@tfield Separator Separator
		@tfield SameLine SameLine
		@tfield NewLine NewLine
		@tfield Spacing Spacing
		@tfield Dummy Dummy
		@tfield Indent Indent
		@tfield Unindent Unindent
		@tfield BeginGroup BeginGroup
		@tfield EndGroup EndGroup
		@tfield GetCursorPos GetCursorPos
		@tfield GetCursorPosX GetCursorPosX
		@tfield GetCursorPosY GetCursorPosY
		@tfield SetCursorPos SetCursorPos
		@tfield SetCursorPosX SetCursorPosX
		@tfield SetCursorPosY SetCursorPosY
		@tfield GetCursorStartPos GetCursorStartPos
		@tfield GetCursorScreenPos GetCursorScreenPos
		@tfield SetCursorScreenPos SetCursorScreenPos
		@tfield AlignTextToFramePadding AlignTextToFramePadding
		@tfield GetTextLineHeight GetTextLineHeight
		@tfield GetTextLineHeightWithSpacing GetTextLineHeightWithSpacing
		@tfield GetFrameHeight GetFrameHeight
		@tfield GetFrameHeightWithSpacing GetFrameHeightWithSpacing
		@tfield PushID PushID
		@tfield PopID PopID
		@tfield GetID GetID
		@tfield TextUnformatted TextUnformatted
		@tfield Text Text
		@tfield TextColored TextColored
		@tfield TextDisabled TextDisabled
		@tfield TextWrapped TextWrapped
		@tfield LabelText LabelText
		@tfield BulletText BulletText
		@tfield Button Button
		@tfield SmallButton SmallButton
		@tfield InvisibleButton InvisibleButton
		@tfield ArrowButton ArrowButton
		@tfield Checkbox Checkbox
		@tfield RadioButton RadioButton
		@tfield ProgressBar ProgressBar
		@tfield Bullet Bullet
		@tfield ImageButton ImageButton
		@tfield Image Image
		@tfield BeginCombo BeginCombo
		@tfield EndCombo EndCombo
		@tfield Combo Combo
		@tfield DragFloat DragFloat
		@tfield DragFloat2 DragFloat2
		@tfield DragFloat3 DragFloat3
		@tfield DragFloat4 DragFloat4
		@tfield DragInt	DragInt
		@tfield DragInt2 DragInt2
		@tfield DragInt3 DragInt3
		@tfield DragInt4 DragInt4
		@tfield SliderFloat SliderFloat
		@tfield SliderFloat2 SliderFloat2
		@tfield SliderFloat3 SliderFloat3
		@tfield SliderFloat4 SliderFloat4
		@tfield SliderAngle SliderAngle
		@tfield SliderInt SliderInt
		@tfield SliderInt2 SliderInt2
		@tfield SliderInt3 SliderInt3
		@tfield SliderInt4 SliderInt4
		@tfield VSliderFloat VSliderFloat
		@tfield VSliderInt VSliderInt
		@tfield InputText InputText
		@tfield InputTextMultiline InputTextMultiline
		@tfield InputTextWithHint InputTextWithHint
		@tfield InputFloat InputFloat
		@tfield InputFloat2 InputFloat2
		@tfield InputFloat3 InputFloat3
		@tfield InputFloat4 InputFloat4
		@tfield InputInt InputInt
		@tfield InputInt2 InputInt2
		@tfield InputInt3 InputInt3
		@tfield InputInt4 InputInt4
		@tfield InputDouble InputDouble
		@tfield ColorEdit3 ColorEdit3
		@tfield ColorEdit4 ColorEdit4
		@tfield ColorPicker3 ColorPicker3
		@tfield ColorPicker4 ColorPicker4
		@tfield TreeNode TreeNode
		@tfield TreeNodeEx TreeNodeEx
		@tfield TreePush TreePush
		@tfield TreePop TreePop
		@tfield GetTreeNodeToLabelSpacing GetTreeNodeToLabelSpacing
		@tfield CollapsingHeader CollapsingHeader
		@tfield SetNextItemOpen SetNextItemOpen
		@tfield Selectable Selectable
		@tfield ListBox ListBox
		@tfield ListBoxHeader ListBoxHeader
		@tfield ListBoxFooter ListBoxFooter
		@tfield Value Value
		@tfield BeginMenuBar BeginMenuBar
		@tfield EndMenuBar EndMenuBar
		@tfield BeginMainMenuBar BeginMainMenuBar
		@tfield EndMainMenuBar EndMainMenuBar
		@tfield BeginMenu BeginMenu
		@tfield EndMenu EndMenu
		@tfield MenuItem MenuItem
		@tfield BeginTooltip BeginTooltip
		@tfield EndTooltip EndTooltip
		@tfield SetTooltip SetTooltip
		@tfield BeginPopup BeginPopup
		@tfield BeginPopupModal BeginPopupModal
		@tfield EndPopup EndPopup
		@tfield OpenPopup OpenPopup
		@tfield CloseCurrentPopup CloseCurrentPopup
		@tfield BeginPopupContextItem BeginPopupContextItem
		@tfield BeginPopupContextWindow BeginPopupContextWindow
		@tfield BeginPopupContextVoid BeginPopupContextVoid
		@tfield IsPopupOpen IsPopupOpen
		@tfield Columns Columns
		@tfield NextColumn NextColumn
		@tfield GetColumnIndex GetColumnIndex
		@tfield GetColumnWidth GetColumnWidth
		@tfield SetColumnWidth SetColumnWidth
		@tfield GetColumnOffset GetColumnOffset
		@tfield SetColumnOffset SetColumnOffset
		@tfield GetColumnsCount GetColumnsCount
		@tfield BeginTabBar BeginTabBar
		@tfield EndTabBar EndTabBar
		@tfield BeginTabItem BeginTabItem
		@tfield EndTabItem EndTabItem
		@tfield SetTabItemClosed SetTabItemClosed
		@tfield LogToTTY LogToTTY
		@tfield LogToFile LogToFile
		@tfield LogToClipboard LogToClipboard
		@tfield LogFinish LogFinish
		@tfield LogButtons LogButtons
		@tfield LogText LogText
		@tfield PushClipRect PushClipRect
		@tfield PopClipRect PopClipRect
		@tfield SetItemDefaultFocus SetItemDefaultFocus
		@tfield SetKeyboardFocusHere SetKeyboardFocusHere
		@tfield IsItemHovered IsItemHovered
		@tfield IsItemActive IsItemActive
		@tfield IsItemFocused IsItemFocused
		@tfield IsItemClicked IsItemClicked
		@tfield IsItemVisible IsItemVisible
		@tfield IsItemEdited IsItemEdited
		@tfield IsItemActivated IsItemActivated
		@tfield IsItemDeactivated IsItemDeactivated
		@tfield IsItemDeactivatedAfterEdit IsItemDeactivatedAfterEdit
		@tfield IsItemToggledOpen IsItemToggledOpen
		@tfield IsAnyItemHovered IsAnyItemHovered
		@tfield IsAnyItemActive IsAnyItemActive
		@tfield IsAnyItemFocused IsAnyItemFocused
		@tfield GetItemRectMin GetItemRectMin
		@tfield GetItemRectMax GetItemRectMax
		@tfield GetItemRectSize GetItemRectSize
		@tfield SetItemAllowOverlap SetItemAllowOverlap
		@tfield IsRectVisible IsRectVisible
		@tfield GetTime GetTime
		@tfield GetFrameCount GetFrameCount
		@tfield GetStyleColorName GetStyleColorName
		@tfield BeginChildFrame BeginChildFrame
		@tfield EndChildFrame EndChildFrame
		@tfield CalcTextSize CalcTextSize
		@tfield ColorConvertRGBtoHSV ColorConvertRGBtoHSV
		@tfield ColorConvertHSVtoRGB ColorConvertHSVtoRGB
		@tfield GetKeyIndex GetKeyIndex
		@tfield IsKeyDown IsKeyDown
		@tfield IsKeyPressed IsKeyPressed
		@tfield IsKeyReleased IsKeyReleased
		@tfield CaptureKeyboardFromApp CaptureKeyboardFromApp
		@tfield IsMouseDown IsMouseDown
		@tfield IsMouseClicked IsMouseClicked
		@tfield IsMouseReleased IsMouseReleased
		@tfield IsMouseDoubleClicked IsMouseDoubleClicked
		@tfield IsMouseHoveringRect IsMouseHoveringRect
		@tfield IsAnyMouseDown IsAnyMouseDown
		@tfield GetMousePos GetMousePos
		@tfield GetMousePosOnOpeningCurrentPopup GetMousePosOnOpeningCurrentPopup
		@tfield IsMouseDragging IsMouseDragging
		@tfield GetMouseDragDelta GetMouseDragDelta
		@tfield ResetMouseDragDelta ResetMouseDragDelta
		@tfield GetMouseCursor GetMouseCursor
		@tfield SetMouseCursor SetMouseCursor
		@tfield CaptureMouseFromApp CaptureMouseFromApp
		@tfield GetClipboardText GetClipboardText
		@tfield SetClipboardText SetClipboardText
		@tfield GetStyle GetStyle
		@tfield GetIO GetIO
		@table ImGui
		*/


		InitEnums(lua);

		sol::table ImGui = lua.create_named_table("ImGui");

#pragma region Windows
		/***
		Push window to the stack and start appending to it. Always call a matching End() for each Begin() call, regardless of its return value!
		@function ImGui.Begin
		@tparam string name window name
		@tparam bool open optional
		@tparam int flags optional
		@treturn bool collapsed
		@usage
		ImGui.Begin("SampleWindow");
		ImGui.End();
		*/

		ImGui.set_function("Begin", sol::overload(
			sol::resolve<bool(const std::string&)>(Begin),
			sol::resolve<std::tuple<bool, bool>(const std::string&, bool)>(Begin),
			sol::resolve<std::tuple<bool, bool>(const std::string&, bool, int)>(Begin)
		));

		/***
		Pop window from the stack.
		@function ImGui.End
		@usage
		ImGui.Begin("SampleWindow");
		ImGui.End();
		*/
		ImGui.set_function("End", End);
#pragma endregion Windows

		/***
		Begin child. Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child. Always call a matching EndChild() for each BeginChild() call, regardless of its return value.
		@function ImGui.BeginChild
		@tparam string name window name
		@tparam float sizeX optional
		@tparam float sizeY optional
		@tparam bool open optional
		@tparam int flags optional
		@treturn bool collapsed
		@usage
		ImGui.BeginChild("SampleWindow");
		ImGui.End();
		*/
#pragma region Child Windows
		ImGui.set_function("BeginChild", sol::overload(
			sol::resolve<bool(const std::string&)>(BeginChild),
			sol::resolve<bool(const std::string&, float)>(BeginChild),
			sol::resolve<bool(const std::string&, float, float)>(BeginChild),
			sol::resolve<bool(const std::string&, float, float, bool)>(BeginChild),
			sol::resolve<bool(const std::string&, float, float, bool, int)>(BeginChild)
		));
		/***
		End Child.
		@function ImGui.EndChild
		@usage
		ImGui.BeginChild("SampleWindow");
		ImGui.EndChild();
		*/
		ImGui.set_function("EndChild", EndChild);
#pragma endregion Child Windows

		/***
		Is Window Appearing?.
		@function ImGui.IsWindowAppearing
		@treturn bool appearing
		@usage
			ImGui.IsWindowAppearing();
		*/
#pragma region Window Utilities
		ImGui.set_function("IsWindowAppearing", IsWindowAppearing);
		/***
		Is Window Collapsed?.
		@function ImGui.IsWindowCollapsed
		@treturn bool collapsed
		@usage
			ImGui.IsWindowCollapsed();
		*/
		ImGui.set_function("IsWindowCollapsed", IsWindowCollapsed);
		/***
		Is current window focused? Or its root/child, depending on flags.
		@function ImGui.IsWindowFocused
		@tparam int focusedFlags optional
		@treturn bool collapsed
		@usage
			ImGui.IsWindowFocused();
		*/
		ImGui.set_function("IsWindowFocused", sol::overload(
			sol::resolve<bool()>(IsWindowFocused),
			sol::resolve<bool(int)>(IsWindowFocused)
		));
		/***
		Is current window hovered (and typically: not blocked by a popup/modal)?.
		@function ImGui.IsWindowHovered
		@tparam int hoveredFlags optional
		@treturn bool collapsed
		@usage
			ImGui.IsWindowHovered();
		*/
		ImGui.set_function("IsWindowHovered", sol::overload(
			sol::resolve<bool()>(IsWindowHovered),
			sol::resolve<bool(int)>(IsWindowHovered)
		));
		/***
		Get window dpi scale.
		@function ImGui.GetWindowDpiScale
		@treturn float scale
		@usage
			ImGui.GetWindowDpiScale();
		*/
#if defined(IMGUI_HAS_DOCK)
		ImGui.set_function("GetWindowDpiScale", GetWindowDpiScale);
#endif
		/***
		Get current window position in screen space.
		@function ImGui.GetWindowPos
		@treturn float posX
		@treturn float posY
		@usage
			ImGui.GetWindowPos();
		*/
		ImGui.set_function("GetWindowPos", GetWindowPos);
		/***
		Get current window size.
		@function ImGui.GetWindowSize
		@treturn float sizeX
		@treturn float sizeY
		@usage
			ImGui.GetWindowSize();
		*/
		ImGui.set_function("GetWindowSize", GetWindowSize);
		/***
		Get current window width.
		@function ImGui.GetWindowWidth
		@treturn float width
		@usage
			ImGui.GetWindowWidth();
		*/
		ImGui.set_function("GetWindowWidth", GetWindowWidth);
		/***
		Get current window height.
		@function ImGui.GetWindowHeight
		@treturn float height
		@usage
			ImGui.GetWindowHeight();
		*/
		ImGui.set_function("GetWindowHeight", GetWindowHeight);

		/***
		Set next window position. Call before Begin(). Use pivot 0.5, 0.5 to center on given point, etc.
		@function ImGui.SetNextWindowPos
		@tparam float posX
		@tparam float posY
		@tparam int condition use ImGuiCond enum optional
		@tparam float pivotX optional
		@tparam float pivotY optional
		@usage
			ImGui.SetNextWindowPos(0,0);
		*/
		ImGui.set_function("SetNextWindowPos", sol::overload(
			sol::resolve<void(float, float)>(SetNextWindowPos),
			sol::resolve<void(float, float, int)>(SetNextWindowPos),
			sol::resolve<void(float, float, int, float, float)>(SetNextWindowPos)
		));
		/***
		Set next window size. Set axis to 0.0 to force an auto-fit on this axis. Call before Begin().
		@function ImGui.SetNextWindowSize
		@tparam float sizeX
		@tparam float sizeY
		@tparam int condition use ImGuiCond enum optional
		@usage
			ImGui.SetNextWindowSize(50.0,50.0);
		*/
		ImGui.set_function("SetNextWindowSize", sol::overload(
			sol::resolve<void(float, float)>(SetNextWindowSize),
			sol::resolve<void(float, float, int)>(SetNextWindowSize)
		));
		/***
		Set next window size limits. Use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down.
		@function ImGui.SetNextWindowSizeConstraints
		@tparam float minX
		@tparam float minY
		@tparam float maxX
		@tparam float maxY
		@usage
			ImGui.SetNextWindowSizeConstraints(50.0, 50.0, 100.0, 100.0);
		*/
		ImGui.set_function("SetNextWindowSizeConstraints", SetNextWindowSizeConstraints);
		/***
		Set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. Set an axis to 0.0f to leave it automatic. Call before Begin()
		@function ImGui.SetNextWindowContentSize
		@tparam float sizeX
		@tparam float sizeY
		@usage
			ImGui.SetNextWindowContentSize(50.0, 50.0);
		*/
		ImGui.set_function("SetNextWindowContentSize", SetNextWindowContentSize);
		/***
		Set next window collapsed state. Call before Begin()
		@function ImGui.SetNextWindowCollapsed
		@tparam bool collapsed
		@tparam int condition use ImGuiCond enum optional
		@usage
			ImGui.SetNextWindowCollapsed(true);
		*/
		ImGui.set_function("SetNextWindowCollapsed", sol::overload(
			sol::resolve<void(bool)>(SetNextWindowCollapsed),
			sol::resolve<void(bool, int)>(SetNextWindowCollapsed)
		));
		/***
		Set next window to be focused / top-most. Call before Begin()
		@function ImGui.SetNextWindowFocus
		@usage
			ImGui.SetNextWindowFocus();
		*/
		ImGui.set_function("SetNextWindowFocus", SetNextWindowFocus);
		/***
		Set next window background color alpha. Helper to easily override the Alpha component of ImGuiCol_WindowBg/ChildBg/PopupBg. You may also use ImGuiWindowFlags.NoBackground.
		@function ImGui.SetNextWindowBgAlpha
		@tparam float alpha
		@usage
			ImGui.SetNextWindowBgAlpha(0.5);
		*/
		ImGui.set_function("SetNextWindowBgAlpha", SetNextWindowBgAlpha);
		/***
		(not recommended) Set current window position - call within Begin()/End(). Prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
		@function ImGui.SetWindowPos
		@tparam string name optional
		@tparam float posX
		@tparam float posY
		@tparam int condition use ImGuiCond enum optional
		@usage
			ImGui.SetWindowPos(0,0);
		*/
		ImGui.set_function("SetWindowPos", sol::overload(
			sol::resolve<void(float, float)>(SetWindowPos),
			sol::resolve<void(float, float, int)>(SetWindowPos),
			sol::resolve<void(const std::string&, float, float)>(SetWindowPos),
			sol::resolve<void(const std::string&, float, float, int)>(SetWindowPos)
		));
		/***
		(not recommended) Set current window size - call within Begin()/End(). Prefer using SetNextWindowSize(), as this may incur tearing and side-effects.
		@function ImGui.SetWindowSize
		@tparam string name optional
		@tparam float sizeX
		@tparam float sizeY
		@tparam int condition use ImGuiCond enum optional
		@usage
			ImGui.SetWindowSize(0,0);
		*/
		ImGui.set_function("SetWindowSize", sol::overload(
			sol::resolve<void(float, float)>(SetWindowSize),
			sol::resolve<void(float, float, int)>(SetWindowSize),
			sol::resolve<void(const std::string&, float, float)>(SetWindowSize),
			sol::resolve<void(const std::string&, float, float, int)>(SetWindowSize)
		));
		/***
		(not recommended) Set current window size - call within Begin()/End(). Prefer using SetNextWindowSize(), as this may incur tearing and side-effects.
		@function ImGui.SetWindowCollapsed
		@tparam string name optional
		@tparam bool collapsed
		@tparam int condition use ImGuiCond enum optional
		@usage
			ImGui.SetWindowCollapsed(true);
		*/
		ImGui.set_function("SetWindowCollapsed", sol::overload(
			sol::resolve<void(bool)>(SetWindowCollapsed),
			sol::resolve<void(bool, int)>(SetWindowCollapsed),
			sol::resolve<void(const std::string&, bool)>(SetWindowCollapsed),
			sol::resolve<void(const std::string&, bool, int)>(SetWindowCollapsed)
		));
		/***
		(not recommended) Set current window to be focused / top-most. Prefer using SetNextWindowFocus().
		@function ImGui.SetWindowFocus
		@tparam string name optional
		@usage
			ImGui.SetWindowFocus();
		*/
		ImGui.set_function("SetWindowFocus", sol::overload(
			sol::resolve<void()>(SetWindowFocus),
			sol::resolve<void(const std::string&)>(SetWindowFocus)
		));
		/***
		Set font scale.
		@function ImGui.SetWindowFontScale
		@tparam float scale
		@usage
			ImGui.SetWindowFontScale(1.2);
		*/
		ImGui.set_function("SetWindowFontScale", SetWindowFontScale);
#pragma endregion Window Utilities

#pragma region Content Region
		/***
		Current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates.
		@function ImGui.GetContentRegionMax
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetContentRegionMax();
		*/
		ImGui.set_function("GetContentRegionMax", GetContentRegionMax);
		/***
		GetContentRegionMax() - GetCursorPos().
		@function ImGui.GetContentRegionAvail
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetContentRegionAvail();
		*/
		ImGui.set_function("GetContentRegionAvail", GetContentRegionAvail);
		/***
		Content boundaries min for the full window (roughly (0,0)-Scroll), in window coordinates.
		@function ImGui.GetWindowContentRegionMin
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetWindowContentRegionMin();
		*/
		ImGui.set_function("GetWindowContentRegionMin", GetWindowContentRegionMin);
		/***
		Content boundaries max for the full window (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
		@function ImGui.GetWindowContentRegionMax
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetWindowContentRegionMax();
		*/
		ImGui.set_function("GetWindowContentRegionMax", GetWindowContentRegionMax);
		/***
		Get window content region width
		@function ImGui.GetWindowContentRegionWidth
		@treturn float width
		@usage
			ImGui.GetWindowContentRegionWidth();
		*/
		ImGui.set_function("GetWindowContentRegionWidth", GetWindowContentRegionWidth);
#pragma endregion Content Region

#pragma region Windows Scrolling
		/***
		Get scrolling amount [0 .. GetScrollMaxX()]
		@function ImGui.GetScrollX
		@treturn float x
		@usage
			ImGui.GetScrollX();
		*/
		ImGui.set_function("GetScrollX", GetScrollX);
		/***
		Get scrolling amount [0 .. GetScrollMaxY()]
		@function ImGui.GetScrollY
		@treturn float y
		@usage
			ImGui.GetScrollY();
		*/
		ImGui.set_function("GetScrollY", GetScrollY);
		/***
		Get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - DecorationsSize.x
		@function ImGui.GetScrollMaxX
		@treturn float x
		@usage
			ImGui.GetScrollMaxX();
		*/
		ImGui.set_function("GetScrollMaxX", GetScrollMaxX);
		/***
		Get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - DecorationsSize.y
		@function ImGui.GetScrollMaxY
		@treturn float y
		@usage
			ImGui.GetScrollMaxY();
		*/
		ImGui.set_function("GetScrollMaxY", GetScrollMaxY);
		/***
		Set scrolling amount [0 .. GetScrollMaxX()]
		@function ImGui.SetScrollX
		@tparam float x
		@usage
			ImGui.SetScrollX(1);
		*/
		ImGui.set_function("SetScrollX", SetScrollX);
		/***
		Set scrolling amount [0 .. GetScrollMaxY()]
		@function ImGui.SetScrollY
		@tparam float y
		@usage
			ImGui.SetScrollY(1);
		*/
		ImGui.set_function("SetScrollY", SetScrollY);
		/***
		Adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
		@function ImGui.SetScrollHereX
		@tparam float centerXRatio optional
		@usage
			ImGui.SetScrollHereX();
		*/
		ImGui.set_function("SetScrollHereX", sol::overload(
			sol::resolve<void()>(SetScrollHereX),
			sol::resolve<void(float)>(SetScrollHereX)
		));
		/***
		Adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
		@function ImGui.SetScrollHereY
		@tparam float centerYRatio optional
		@usage
			ImGui.SetScrollHereY();
		*/
		ImGui.set_function("SetScrollHereY", sol::overload(
			sol::resolve<void()>(SetScrollHereY),
			sol::resolve<void(float)>(SetScrollHereY)
		));
		/***
		Adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
		@function ImGui.SetScrollFromPosX
		@tparam float localX optional
		@tparam float centerXRatio optional
		@usage
			ImGui.SetScrollFromPosX(1);
		*/
		ImGui.set_function("SetScrollFromPosX", sol::overload(
			sol::resolve<void(float)>(SetScrollFromPosX),
			sol::resolve<void(float, float)>(SetScrollFromPosX)
		));
		/***
		Adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
		@function ImGui.SetScrollFromPosY
		@tparam float localY optional
		@tparam float centerYRatio optional
		@usage
			ImGui.SetScrollFromPosY(1);
		*/
		ImGui.set_function("SetScrollFromPosY", sol::overload(
			sol::resolve<void(float)>(SetScrollFromPosY),
			sol::resolve<void(float, float)>(SetScrollFromPosY)
		));
#pragma endregion Windows Scrolling

		/***
		Modify a style variable.
		@function ImGui.PushStyleVar
		@tparam int styleVar
		@tparam float value (or valX) optional
		@tparam float valY optional
		@usage
			ImGui.PushStyleVar(ImGuiStyleVar.WindowPadding, 2.0);
		*/
		ImGui.set_function("PushStyleVar", sol::overload(
			sol::resolve<void(int, float)>(PushStyleVar),
			sol::resolve<void(int, float, float)>(PushStyleVar)
		));

		/***
		Pop a style variable.
		@function ImGui.PopStyleVar
		@tparam int count optional
		@usage
			ImGui.PopStyleVar();
		*/
		ImGui.set_function("PopStyleVar", sol::overload(
			sol::resolve<void()>(PopStyleVar),
			sol::resolve<void(int)>(PopStyleVar)
		));

#pragma region Parameters stacks (shared)
		/***
		Use nil as a shortcut to push default font.
		@function ImGui.PushFont
		@tparam ImFont font
		@usage
			ImGui.PushFont(font);
		*/
		ImGui.set_function("PushFont", PushFont);
		/***
		Pop font.
		@function ImGui.PopFont
		@usage
			ImGui.PopFont();
		*/
		ImGui.set_function("PopFont", PopFont);
#ifdef SOL_IMGUI_USE_COLOR_U32
		ImGui.set_function("PushStyleColor", sol::overload(
			sol::resolve<void(int, int)>(PushStyleColor),
			sol::resolve<void(int, float, float, float, float)>(PushStyleColor)
		));
#else
		/***
		Push style color.
		@function ImGui.PushStyleColor
		@tparam int idx use ImGuiCol enum
		@tparam float colorR
		@tparam float colorG
		@tparam float colorB
		@tparam float colorA
		@usage
			ImGui.PushStyleColor(ImGuiCol.TitleBg, 132, 0, 0, 255);
		*/
		ImGui.set_function("PushStyleColor", PushStyleColor);
#endif
		/***
		Pop style color.
		@function ImGui.PopStyleColor
		@tparam int count optional
		@usage
			ImGui.PopStyleColor();
		*/
		ImGui.set_function("PopStyleColor", sol::overload(
			sol::resolve<void()>(PopStyleColor),
			sol::resolve<void(int)>(PopStyleColor)
		));
		/***
		Pop style color.
		@function ImGui.GetStyleColorVec4
		@tparam int idx use ImGuiCol enum
		@treturn float colorR
		@treturn float colorG
		@treturn float colorB
		@treturn float colorA
		@usage
			ImGui.GetStyleColorVec4();
		*/
		ImGui.set_function("GetStyleColorVec4", GetStyleColorVec4);
		/***
		Get current font.
		@function ImGui.GetFont
		@treturn ImFont font
		@usage
			ImGui.GetFont();
		*/
		ImGui.set_function("GetFont", GetFont);
		/***
		Get current font size (= height in pixels) of current font with current scale applied
		@function ImGui.GetFontSize
		@treturn float size
		@usage
			ImGui.GetFontSize();
		*/
		ImGui.set_function("GetFontSize", GetFontSize);
		/***
		Get UV coordinate for a white pixel.
		@function ImGui.GetFontTexUvWhitePixel
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetFontTexUvWhitePixel();
		*/
		ImGui.set_function("GetFontTexUvWhitePixel", GetFontTexUvWhitePixel);
#ifdef SOL_IMGUI_USE_COLOR_U32
		ImGui.set_function("GetColorU32", sol::overload(
			sol::resolve<int(int, float)>(GetColorU32),
			sol::resolve<int(float, float, float, float)>(GetColorU32),
			sol::resolve<int(int)>(GetColorU32)
		));
#endif
#pragma endregion Parameters stacks (shared)

#pragma region Parameters stacks (current window)
		/***
		Push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side).
		@function ImGui.PushItemWidth
		@tparam float width
		@usage
			ImGui.PushItemWidth(1);
		*/
		ImGui.set_function("PushItemWidth", PushItemWidth);
		/***
		Pop Item Width.
		@function ImGui.PopItemWidth
		@usage
			ImGui.PopItemWidth();
		*/
		ImGui.set_function("PopItemWidth", PopItemWidth);
		/***
		Set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side)
		@function ImGui.SetNextItemWidth
		@tparam float width
		@usage
			ImGui.SetNextItemWidth(1);
		*/
		ImGui.set_function("SetNextItemWidth", SetNextItemWidth);
		/***
		Width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
		@function ImGui.CalcItemWidth
		@treturn float width
		@usage
			ImGui.CalcItemWidth();
		*/
		ImGui.set_function("CalcItemWidth", CalcItemWidth);
		/***
		Push word-wrapping position for Text commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrapLocalPosX' position in window local space
		@function ImGui.PushTextWrapPos
		@treturn float wrapLocalPosX optional
		@usage
			ImGui.PushTextWrapPos();
		*/
		ImGui.set_function("PushTextWrapPos", sol::overload(
			sol::resolve<void()>(PushTextWrapPos),
			sol::resolve<void(float)>(PushTextWrapPos)
		));
		/***
		Pop Text Wrap Pos
		@function ImGui.PopTextWrapPos
		@usage
			ImGui.PopTextWrapPos();
		*/
		ImGui.set_function("PopTextWrapPos", PopTextWrapPos);
		/***
		Tab stop enable. Allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets.
		@function ImGui.PushAllowKeyboardFocus
		@tparam bool allowFocus
		@usage
			ImGui.PushAllowKeyboardFocus(false);
		*/
		ImGui.set_function("PushAllowKeyboardFocus", PushAllowKeyboardFocus);
		/***
		Pop AllowKeyboardFocus
		@function ImGui.PopAllowKeyboardFocus
		@usage
			ImGui.PopAllowKeyboardFocus();
		*/
		ImGui.set_function("PopAllowKeyboardFocus", PopAllowKeyboardFocus);
		/***
		In 'repeat' mode, Button functions return repeated true in a typematic manner. Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
		@function ImGui.PushButtonRepeat
		@tparam bool doRepeat
		@usage
			ImGui.PushButtonRepeat(true);
		*/
		ImGui.set_function("PushButtonRepeat", PushButtonRepeat);
		/***
		Pop buttom repeat.
		@function ImGui.PopButtonRepeat
		@usage
			ImGui.PopButtonRepeat();
		*/
		ImGui.set_function("PopButtonRepeat", PopButtonRepeat);
#pragma endregion Parameters stacks (current window)

#pragma region Cursor / Layout
		/***
		Separator, generally horizontal. Inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
		@function ImGui.Separator
		@usage
			ImGui.Separator();
		*/
		ImGui.set_function("Separator", Separator);
		/***
		Call between widgets or groups to layout them horizontally. X position given in window coordinates.
		@function ImGui.SameLine
		@tparam float offsetFromStartX optional
		@usage
			ImGui.SameLine();
		*/
		ImGui.set_function("SameLine", sol::overload(
			sol::resolve<void()>(SameLine),
			sol::resolve<void(float)>(SameLine)
		));
		/***
		Undo a SameLine() or force a new line when in an horizontal-layout context.
		@function ImGui.NewLine
		@usage
			ImGui.NewLine();
		*/
		ImGui.set_function("NewLine", NewLine);
		/***
		Add vertical spacing.
		@function ImGui.Spacing
		@usage
			ImGui.Spacing();
		*/
		ImGui.set_function("Spacing", Spacing);
		/***
		Add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into..
		@function ImGui.Dummy
		@tparam float sizeX
		@tparam float sizeY
		@usage
			ImGui.Dummy(1,1);
		*/
		ImGui.set_function("Dummy", Dummy);
		/***
		Move content position toward the right, by indentWidth, or style.IndentSpacing if indentWidth <= 0
		@function ImGui.Indent
		@tparam float indentWidth optional
		@usage
			ImGui.Indent();
		*/
		ImGui.set_function("Indent", sol::overload(
			sol::resolve<void()>(Indent),
			sol::resolve<void(float)>(Indent)
		));
		/***
		Move content position back to the left, by indentWidth, or style.IndentSpacing if indentWidth <= 0
		@function ImGui.Unindent
		@tparam float indentWidth optional
		@usage
			ImGui.Unindent();
		*/
		ImGui.set_function("Unindent", sol::overload(
			sol::resolve<void()>(Unindent),
			sol::resolve<void(float)>(Unindent)
		));
		/***
		Lock horizontal starting position
		@function ImGui.BeginGroup
		@usage
			ImGui.BeginGroup();
		*/
		ImGui.set_function("BeginGroup", BeginGroup);
		/***
		Unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
		@function ImGui.EndGroup
		@usage
			ImGui.EndGroup();
		*/
		ImGui.set_function("EndGroup", EndGroup);
		/***
		Cursor position in window coordinates (relative to window position)
		@function ImGui.GetCursorPos
		@treturn float posX
		@treturn float posY
		@usage
			ImGui.GetCursorPos();
		*/
		ImGui.set_function("GetCursorPos", GetCursorPos);
		/***
		Some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.
		@function ImGui.GetCursorPosX
		@treturn float posX
		@usage
			ImGui.GetCursorPosX();
		*/
		ImGui.set_function("GetCursorPosX", GetCursorPosX);
		/***
		Other functions such as GetCursorScreenPos.
		@function ImGui.GetCursorPosY
		@treturn float posY
		@usage
			ImGui.GetCursorPosY();
		*/
		ImGui.set_function("GetCursorPosY", GetCursorPosY);
		/***
		Are using the main, absolute coordinate system.
		@function ImGui.SetCursorPos
		@tparam float localX
		@tparam float localY
		@usage
			ImGui.SetCursorPos(1,1);
		*/
		ImGui.set_function("SetCursorPos", SetCursorPos);
		/***
		GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.).
		@function ImGui.SetCursorPosX
		@tparam float localX
		@usage
			ImGui.SetCursorPosX(1);
		*/
		ImGui.set_function("SetCursorPosX", SetCursorPosX);
		/***
		GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.).
		@function ImGui.SetCursorPosY
		@tparam float localY
		@usage
			ImGui.SetCursorPosY(1);
		*/
		ImGui.set_function("SetCursorPosY", SetCursorPosY);
		/***
		Initial cursor position in window coordinates.
		@function ImGui.GetCursorStartPos
		@treturn float posX
		@treturn float posY
		@usage
			ImGui.GetCursorStartPos();
		*/
		ImGui.set_function("GetCursorStartPos", GetCursorStartPos);
		/***
		Cursor position in absolute coordinates. generally top-left == GetMainViewport()->Pos == (0,0) in single viewport mode, and bottom-right == GetMainViewport()->Pos+Size == io.DisplaySize in single-viewport mode..
		@function ImGui.GetCursorScreenPos
		@treturn float posX
		@treturn float posY
		@usage
			ImGui.GetCursorScreenPos();
		*/
		ImGui.set_function("GetCursorScreenPos", GetCursorScreenPos);
		/***
		Cursor position in window coordinates.
		@function ImGui.SetCursorScreenPos
		@tparam float posX
		@tparam float posY
		@usage
			ImGui.SetCursorScreenPos(1,1);
		*/
		ImGui.set_function("SetCursorScreenPos", SetCursorScreenPos);
		/***
		Vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item).
		@function ImGui.AlignTextToFramePadding
		@usage
			ImGui.AlignTextToFramePadding();
		*/
		ImGui.set_function("AlignTextToFramePadding", AlignTextToFramePadding);
		/***
		FontSize.
		@function ImGui.GetTextLineHeight
		@treturn float height
		@usage
			ImGui.GetTextLineHeight();
		*/
		ImGui.set_function("GetTextLineHeight", GetTextLineHeight);
		/***
		FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text).
		@function ImGui.GetTextLineHeightWithSpacing
		@treturn float height
		@usage
			ImGui.GetTextLineHeightWithSpacing();
		*/
		ImGui.set_function("GetTextLineHeightWithSpacing", GetTextLineHeightWithSpacing);
		/***
		FontSize + style.FramePadding.y * 2
		@function ImGui.GetFrameHeight
		@treturn float height
		@usage
			ImGui.GetFrameHeight();
		*/
		ImGui.set_function("GetFrameHeight", GetFrameHeight);
		/***
		FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)
		@function ImGui.GetFrameHeightWithSpacing
		@treturn float height
		@usage
			ImGui.GetFrameHeightWithSpacing();
		*/
		ImGui.set_function("GetFrameHeightWithSpacing", GetFrameHeightWithSpacing);
#pragma endregion Cursor / Layout

#pragma region ID stack / scopes
		/***
		Push string into the ID stack (will hash string).
		@function ImGui.PushID
		@tparam string|int id
		@tparam string idEnd optional
		@usage
			ImGui.PushID("thiswindow");
		*/
		ImGui.set_function("PushID", sol::overload(
			sol::resolve<void(const std::string&)>(PushID),
			sol::resolve<void(const std::string&, const std::string&)>(PushID),
			sol::resolve<void(int)>(PushID)
		));
		/***
		Pop from the ID stack.
		@function ImGui.PopID
		@usage
			ImGui.PopID();
		*/
		ImGui.set_function("PopID", PopID);
		/***
		Calculate unique ID (hash of whole ID stack + given parameter).
		@function ImGui.GetID
		@tparam string id
		@tparam string idEnd optional
		@treturn int ImGUIID
		@usage
			ImGui.GetID("thiswindow");
		*/
		ImGui.set_function("GetID", sol::overload(
			sol::resolve<int(const std::string&)>(GetID),
			sol::resolve<int(const std::string&, const std::string&)>(GetID)
		));
#pragma endregion ID stack / scopes

#pragma region Widgets: Text
		/***
		Raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text..
		@function ImGui.TextUnformatted
		@tparam string text
		@tparam string textEnd optional
		@usage
			ImGui.TextUnformatted("text");
		*/
		ImGui.set_function("TextUnformatted", sol::overload(
			sol::resolve<void(const std::string&)>(TextUnformatted),
			sol::resolve<void(const std::string&, const std::string&)>(TextUnformatted)
		));
		/***
		Formatted text.
		@function ImGui.Text
		@tparam string text
		@usage
			ImGui.Text("text");
		*/
		ImGui.set_function("Text", Text);
		/***
		Shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();.
		@function ImGui.TextColored
		@treturn float colorR
		@treturn float colorG
		@treturn float colorB
		@treturn float colorA
		@tparam string text
		@usage
			ImGui.TextColored(0,0,0,255,"text");
		*/
		ImGui.set_function("TextColored", TextColored);
		/***
		Shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();.
		@function ImGui.TextDisabled
		@tparam string text
		@usage
			ImGui.TextDisabled("text");
		*/
		ImGui.set_function("TextDisabled", TextDisabled);
		/***
		Shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
		@function ImGui.TextWrapped
		@tparam string text
		@usage
			ImGui.TextWrapped("text");
		*/
		ImGui.set_function("TextWrapped", TextWrapped);
		/***
		Display text+label aligned the same way as value+label widgets.
		@function ImGui.LabelText
		@tparam string label
		@tparam string text
		@usage
			ImGui.LabelText("label","text");
		*/
		ImGui.set_function("LabelText", LabelText);
		/***
		Shortcut for Bullet()+Text().
		@function ImGui.BulletText
		@tparam string text
		@usage
			ImGui.BulletText("label","text");
		*/
		ImGui.set_function("BulletText", BulletText);
#pragma endregion Widgets: Text

#pragma region Widgets: Main
		/***
		Button.
		@function ImGui.Button
		@tparam string label
		@tparam float sizeX optional
		@tparam float sizeY optional
		@usage
			ImGui.Button("label","text");
		*/
		ImGui.set_function("Button", sol::overload(
			sol::resolve<bool(const std::string&)>(Button),
			sol::resolve<bool(const std::string&, float, float)>(Button)
		));
		/***
		Button with FramePadding=(0,0) to easily embed within text.
		@function ImGui.SmallButton
		@tparam string label
		@usage
			ImGui.SmallButton("label","text");
		*/
		ImGui.set_function("SmallButton", SmallButton);
		/***
		Flexible button behavior without the visuals.
		@function ImGui.InvisibleButton
		@tparam string label
		@tparam float sizeX
		@tparam float sizeY
		@usage
			ImGui.InvisibleButton("label",1,1);
		*/
		ImGui.set_function("InvisibleButton", InvisibleButton);
		/***
		Square button with an arrow shape.
		@function ImGui.ArrowButton
		@tparam string label
		@tparam int direction use ImGuiDir enum
		@usage
			ImGui.ArrowButton("label",ImGuiDir.Left);
		*/
		ImGui.set_function("ArrowButton", ArrowButton);
		/***
		Checkbox.
		@function ImGui.Checkbox
		@tparam string label
		@tparam bool isChecked
		@treturn bool isChecked
		@treturn bool somethingElse
		@usage
			ImGui.Checkbox("label", true);
		*/
		ImGui.set_function("Checkbox", Checkbox);
		/***
		Radio button.
		@function ImGui.RadioButton
		@tparam string label
		@tparam bool|int value
		@tparam int valueButton optional
		@treturn bool|int isChecked
		@treturn bool somethingElse
		@usage
			ImGui.RadioButton("label", true);
		*/
		ImGui.set_function("RadioButton", sol::overload(
			sol::resolve<bool(const std::string&, bool)>(RadioButton),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int)>(RadioButton)
		));
		/***
		Progress Bar.
		@function ImGui.ProgressBar
		@tparam float value
		@tparam float sizeX optional
		@tparam float sizeY optional
		@tparam string overlay optional
		@usage
			ImGui.ProgressBar(0.7);
		*/
		ImGui.set_function("ProgressBar", sol::overload(
			sol::resolve<void(float)>(ProgressBar),
			sol::resolve<void(float, float, float)>(ProgressBar),
			sol::resolve<void(float, float, float, const std::string&)>(ProgressBar)
		));
		/***
		Draw a small circle + keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses.
		@function ImGui.Bullet
		@usage
			ImGui.Bullet();
		*/
		ImGui.set_function("Bullet", Bullet);

		/***
		Image button.
		@function ImGui.ImageButton
		@tparam any texture
		@tparam float sizeX
		@tparam float sizeY
		@usage
			ImGui.ImageButton();
		*/
		ImGui.set_function("ImageButton", ImageButton);
		/***
		Image.
		@function ImGui.Image
		@tparam any texture
		@tparam float sizeX
		@tparam float sizeY
		@usage
			ImGui.Image();
		*/
		ImGui.set_function("Image", Image);
#pragma endregion Widgets: Main

#pragma region Widgets: Combo Box
		/***
		The BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
		@function ImGui.BeginCombo
		@tparam string label
		@tparam string previewValue
		@tparam int comboFlags optional
		@treturn bool returnValue
		@usage
			ImGui.BeginCombo();
		*/
		ImGui.set_function("BeginCombo", sol::overload(
			sol::resolve<bool(const std::string&, const std::string&)>(BeginCombo),
			sol::resolve<bool(const std::string&, const std::string&, int)>(BeginCombo)
		));
		/***
		Only call EndCombo() if BeginCombo() returns true!
		@function ImGui.EndCombo
		@usage
			ImGui.EndCombo();
		*/
		ImGui.set_function("EndCombo", EndCombo);
		/***
		The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose. This is analogous to how ListBox are created.
		@function ImGui.Combo
		@tparam string label
		@tparam int currentItem
		@tparam table|string items
		@tparam int itemCount optional
		@tparam int popupMaxHeightInItems optional
		@treturn int returnInteger
		@treturn bool returnBool
		@usage
			ImGui.Combo();
		*/
		ImGui.set_function("Combo", sol::overload(
			sol::resolve<std::tuple<int, bool>(const std::string&, int, const sol::table&, int)>(Combo),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, const sol::table&, int, int)>(Combo),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, const std::string&)>(Combo),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, const std::string&, int)>(Combo)
		));
#pragma endregion Widgets: Combo Box

#pragma region Widgets: Drags
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragFloat
		@tparam string label
		@tparam float inputValue
		@tparam float speed optional
		@tparam float minValue optional
		@tparam float maxValue optional
		@tparam string format optional
		@tparam float power optional
		@treturn float value
		@treturn bool returnBool
		@usage
			ImGui.DragFloat();
		*/
		ImGui.set_function("DragFloat", sol::overload(
			sol::resolve<std::tuple<float, bool>(const std::string&, float)>(DragFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float)>(DragFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float)>(DragFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, float)>(DragFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, float, const std::string&)>(DragFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, float, const std::string&, float)>(DragFloat)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragFloat2
		@tparam string label
		@tparam float[2] inputValues Maximum: 2
		@tparam float speed optional
		@tparam float minValue optional
		@tparam float maxValue optional
		@tparam string format optional
		@tparam float power optional
		@treturn float[2] values Maximum: 2
		@treturn bool returnBool
		@usage
			ImGui.DragFloat2();
		*/
		ImGui.set_function("DragFloat2", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(DragFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float)>(DragFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float)>(DragFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float)>(DragFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float, const std::string&)>(DragFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float, const std::string&, float)>(DragFloat2)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragFloat3
		@tparam string label
		@tparam float[3] inputValues Maximum: 3
		@tparam float speed optional
		@tparam float minValue optional
		@tparam float maxValue optional
		@tparam string format optional
		@tparam float power optional
		@treturn float[3] values Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.DragFloat3();
		*/
		ImGui.set_function("DragFloat3", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(DragFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float)>(DragFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float)>(DragFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float)>(DragFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float, const std::string&)>(DragFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float, const std::string&, float)>(DragFloat3)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragFloat4
		@tparam string label
		@tparam float[3] inputValues Maximum: 4
		@tparam float speed optional
		@tparam float minValue optional
		@tparam float maxValue optional
		@tparam string format optional
		@tparam float power optional
		@treturn float[3] values Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.DragFloat4();
		*/
		ImGui.set_function("DragFloat4", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(DragFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float)>(DragFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float)>(DragFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float)>(DragFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float, const std::string&)>(DragFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, float, const std::string&, float)>(DragFloat4)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragInt
		@tparam string label
		@tparam int inputValue
		@tparam float speed optional
		@tparam int minValue optional
		@tparam int maxValue optional
		@tparam string format optional
		@treturn int value
		@treturn bool returnBool
		@usage
			ImGui.DragInt();
		*/
		ImGui.set_function("DragInt", sol::overload(
			sol::resolve<std::tuple<int, bool>(const std::string&, int)>(DragInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, float)>(DragInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, float, int)>(DragInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, float, int, int)>(DragInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, float, int, int, const std::string&)>(DragInt)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragInt2
		@tparam string label
		@tparam int[2] inputValues Maximum: 2
		@tparam float speed optional
		@tparam int minValue optional
		@tparam int maxValue optional
		@tparam string format optional
		@treturn int[2] values Maximum: 2
		@treturn bool returnBool
		@usage
			ImGui.DragInt2();
		*/
		ImGui.set_function("DragInt2", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&)>(DragInt2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float)>(DragInt2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int)>(DragInt2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int, int)>(DragInt2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int, int, const std::string&)>(DragInt2)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragInt3
		@tparam string label
		@tparam int[3] inputValues Maximum: 3
		@tparam float speed optional
		@tparam int minValue optional
		@tparam int maxValue optional
		@tparam string format optional
		@treturn int[3] values Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.DragInt3();
		*/
		ImGui.set_function("DragInt3", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&)>(DragInt3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float)>(DragInt3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int)>(DragInt3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int, int)>(DragInt3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int, int, const std::string&)>(DragInt3)
		));
		/***
		CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. If minValue >= maxValue we have no bound.
		@function ImGui.DragInt4
		@tparam string label
		@tparam int[4] inputValues Maximum: 4
		@tparam float speed optional
		@tparam int minValue optional
		@tparam int maxValue optional
		@tparam string format optional
		@treturn int[4] values Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.DragInt4();
		*/
		ImGui.set_function("DragInt4", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&)>(DragInt4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float)>(DragInt4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int)>(DragInt4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int, int)>(DragInt4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, float, int, int, const std::string&)>(DragInt4)
		));
#pragma endregion Widgets: Drags

#pragma region Widgets: Sliders
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderFloat
		@tparam string label
		@tparam float inputValue
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@tparam float power optional
		@treturn float value
		@treturn bool returnBool
		@usage
			ImGui.SliderFloat();
		*/
		ImGui.set_function("SliderFloat", sol::overload(
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float)>(SliderFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, const std::string&)>(SliderFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, const std::string&, float)>(SliderFloat)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderFloat2
		@tparam string label
		@tparam float[2] inputValues Maximum: 2
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@tparam float power optional
		@treturn float[2] values Maximum: 2
		@treturn bool returnBool
		@usage
			ImGui.SliderFloat2();
		*/
		ImGui.set_function("SliderFloat2", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float)>(SliderFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, const std::string&)>(SliderFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, const std::string&, float)>(SliderFloat2)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderFloat3
		@tparam string label
		@tparam float[3] inputValues Maximum: 3
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@tparam float power optional
		@treturn float[3] values Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.SliderFloat3();
		*/
		ImGui.set_function("SliderFloat3", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float)>(SliderFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, const std::string&)>(SliderFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, const std::string&, float)>(SliderFloat3)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderFloat4
		@tparam string label
		@tparam float[4] inputValues Maximum: 4
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@tparam float power optional
		@treturn float[4] values Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.SliderFloat4();
		*/
		ImGui.set_function("SliderFloat4", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float)>(SliderFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, const std::string&)>(SliderFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, float, float, const std::string&, float)>(SliderFloat4)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderAngle
		@tparam string label
		@tparam float radius
		@tparam float minDegree optional
		@tparam float maxDegree optional
		@tparam string format optional
		@treturn float degree
		@treturn bool returnBool
		@usage
			ImGui.SliderAngle();
		*/
		ImGui.set_function("SliderAngle", sol::overload(
			sol::resolve<std::tuple<float, bool>(const std::string&, float)>(SliderAngle),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float)>(SliderAngle),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float)>(SliderAngle),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, const std::string&)>(SliderAngle)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderInt
		@tparam string label
		@tparam int inputValue
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@treturn int value
		@treturn bool returnBool
		@usage
			ImGui.SliderInt();
		*/
		ImGui.set_function("SliderInt", sol::overload(
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int, int)>(SliderInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int, int, const std::string&)>(SliderInt)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderInt2
		@tparam string label
		@tparam int[2] inputValues Maximum: 2
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@treturn int[2] values Maximum: 2
		@treturn bool returnBool
		@usage
			ImGui.SliderInt2();
		*/
		ImGui.set_function("SliderInt2", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int, int)>(SliderInt2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int, int, const std::string&)>(SliderInt2)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderInt3
		@tparam string label
		@tparam int[3] inputValues Maximum: 3
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@treturn int[3] values Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.SliderInt3();
		*/
		ImGui.set_function("SliderInt3", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int, int)>(SliderInt3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int, int, const std::string&)>(SliderInt3)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.SliderInt4
		@tparam string label
		@tparam int[4] inputValues Maximum: 4
		@tparam float minValue
		@tparam float maxValue
		@tparam string format optional
		@treturn int[4] values Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.SliderInt4();
		*/
		ImGui.set_function("SliderInt4", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int, int)>(SliderInt4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int, int, const std::string&)>(SliderInt4)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.VSliderFloat
		@tparam string label
		@tparam float sizeX
		@tparam float sizeY
		@tparam float inputValue
		@tparam float minValue optional
		@tparam float maxValue optional
		@tparam string format optional
		@treturn float value
		@treturn bool returnBool
		@usage
			ImGui.VSliderFloat();
		*/
		ImGui.set_function("VSliderFloat", sol::overload(
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, float, float)>(VSliderFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, float, float, const std::string&)>(VSliderFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, float, float, const std::string&, float)>(VSliderFloat)
		));
		/***
		CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_AlwaysClamp to always clamp. Adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
		@function ImGui.VSliderInt
		@tparam string label
		@tparam float sizeX
		@tparam float sizeY
		@tparam int inputValue
		@tparam int minValue optional
		@tparam int maxValue optional
		@tparam string format optional
		@treturn int value
		@treturn bool returnBool
		@usage
			ImGui.VSliderInt();
		*/
		ImGui.set_function("VSliderInt", sol::overload(
			sol::resolve<std::tuple<int, bool>(const std::string&, float, float, int, int, int)>(VSliderInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, float, float, int, int, int, const std::string&)>(VSliderInt)
		));
#pragma endregion Widgets: Sliders

#pragma region Widgets: Inputs using Keyboard
		/***
		Input with Keyboard.
		@function ImGui.InputText
		@tparam string label
		@tparam string text
		@tparam int bufferSize optional
		@tparam int inputTextFlags optional
		@treturn string value
		@treturn bool returnBool
		@usage
			ImGui.InputText();
		*/
		ImGui.set_function("InputText", sol::overload(
			sol::resolve<std::tuple<std::string, bool>(const std::string&, std::string)>(InputText),
			sol::resolve<std::tuple<std::string, bool>(const std::string&, std::string, unsigned int, int)>(InputText)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputTextMultiline
		@tparam string label
		@tparam string text
		@tparam int bufferSize
		@tparam float sizeX optional
		@tparam float sizeY optional
		@tparam int inputTextFlags optional
		@treturn string value
		@treturn bool returnBool
		@usage
			ImGui.InputTextMultiline();
		*/
		ImGui.set_function("InputTextMultiline", sol::overload(
			sol::resolve<std::tuple<std::string, bool>(const std::string&, std::string, unsigned int)>(InputTextMultiline),
			sol::resolve<std::tuple<std::string, bool>(const std::string&, std::string, unsigned int, float, float)>(InputTextMultiline),
			sol::resolve<std::tuple<std::string, bool>(const std::string&, std::string, unsigned int, float, float, int)>(InputTextMultiline)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputTextWithHint
		@tparam string label
		@tparam string text
		@tparam string hint
		@tparam int bufferSize
		@tparam int inputTextFlags optional
		@treturn string value
		@treturn bool returnBool
		@usage
			ImGui.InputTextWithHint();
		*/
		ImGui.set_function("InputTextWithHint", sol::overload(
			sol::resolve<std::tuple<std::string, bool>(const std::string&, const std::string&, std::string, unsigned int)>(InputTextWithHint),
			sol::resolve<std::tuple<std::string, bool>(const std::string&, const std::string&, std::string, unsigned int, int)>(InputTextWithHint)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputFloat
		@tparam string label
		@tparam float input
		@tparam float step optional
		@tparam float stepFast optional
		@tparam string format optional
		@tparam int inputTextFlags optional
		@treturn float value
		@treturn bool returnBool
		@usage
			ImGui.InputFloat();
		*/
		ImGui.set_function("InputFloat", sol::overload(
			sol::resolve<std::tuple<float, bool>(const std::string&, float)>(InputFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float)>(InputFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float)>(InputFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, const std::string&)>(InputFloat),
			sol::resolve<std::tuple<float, bool>(const std::string&, float, float, float, const std::string&, int)>(InputFloat)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputFloat2
		@tparam string label
		@tparam float[2] input Maximum: 2
		@tparam string format optional
		@tparam int inputTextFlags optional
		@treturn float[2] value Maximum: 2
		@treturn bool returnBool
		@usage
			ImGui.InputFloat2();
		*/
		ImGui.set_function("InputFloat2", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(InputFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, const std::string&)>(InputFloat2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, const std::string&, int)>(InputFloat2)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputFloat3
		@tparam string label
		@tparam float[3] input Maximum: 3
		@tparam string format optional
		@tparam int inputTextFlags optional
		@treturn float[3] value Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.InputFloat3();
		*/
		ImGui.set_function("InputFloat3", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(InputFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, const std::string&)>(InputFloat3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, const std::string&, int)>(InputFloat3)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputFloat4
		@tparam string label
		@tparam float[4] input Maximum: 4
		@tparam string format optional
		@tparam int inputTextFlags optional
		@treturn float[4] value Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.InputFloat4();
		*/
		ImGui.set_function("InputFloat4", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(InputFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, const std::string&)>(InputFloat4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, const std::string&, int)>(InputFloat4)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputInt
		@tparam string label
		@tparam int input
		@tparam int step optional
		@tparam int stepFast optional
		@tparam int inputTextFlags optional
		@treturn int value
		@treturn bool returnBool
		@usage
			ImGui.InputInt();
		*/
		ImGui.set_function("InputInt", sol::overload(
			sol::resolve<std::tuple<int, bool>(const std::string&, int)>(InputInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int)>(InputInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int, int)>(InputInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int, int)>(InputInt),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, int, int, int)>(InputInt)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputInt2
		@tparam string label
		@tparam int[2] input Maximum: 2
		@tparam int inputTextFlags optional
		@treturn int[2] value Maximum: 2
		@treturn bool returnBool
		@usage
			ImGui.InputInt2();
		*/
		ImGui.set_function("InputInt2", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&)>(InputInt2),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int)>(InputInt2)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputInt3
		@tparam string label
		@tparam int[3] input Maximum: 3
		@tparam int inputTextFlags optional
		@treturn int[3] value Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.InputInt3();
		*/
		ImGui.set_function("InputInt3", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&)>(InputInt3),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int)>(InputInt3)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputInt4
		@tparam string label
		@tparam int[4] input Maximum: 4
		@tparam int inputTextFlags optional
		@treturn int[4] value Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.InputInt4();
		*/
		ImGui.set_function("InputInt4", sol::overload(
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&)>(InputInt4),
			sol::resolve<std::tuple<sol::as_table_t<std::vector<int>>, bool>(const std::string&, const sol::table&, int)>(InputInt4)
		));
		/***
		Input with Keyboard.
		@function ImGui.InputDouble
		@tparam string label
		@tparam float input
		@tparam float step optional
		@tparam float stepFast optional
		@tparam string format optional
		@tparam int inputTextFlags optional
		@treturn float value
		@treturn bool returnBool
		@usage
			ImGui.InputDouble();
		*/
		ImGui.set_function("InputDouble", sol::overload(
			sol::resolve<std::tuple<double, bool>(const std::string&, double)>(InputDouble),
			sol::resolve<std::tuple<double, bool>(const std::string&, double, double)>(InputDouble),
			sol::resolve<std::tuple<double, bool>(const std::string&, double, double, double)>(InputDouble),
			sol::resolve<std::tuple<double, bool>(const std::string&, double, double, double, const std::string&)>(InputDouble),
			sol::resolve<std::tuple<double, bool>(const std::string&, double, double, double, const std::string&, int)>(InputDouble)
		));
#pragma endregion Widgets: Inputs using Keyboard

#pragma region Widgets: Color Editor / Picker
		/***
		Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
		@function ImGui.ColorEdit3
		@tparam string label
		@tparam float[3] input Maximum: 3
		@tparam int colorEditFlags optional
		@treturn float[3] color Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.ColorEdit3();
		*/
		ImGui.set_function("ColorEdit3", sol::overload(
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(ColorEdit3),
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, int)>(ColorEdit3)
		));
		/***
		Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
		@function ImGui.ColorEdit4
		@tparam string label
		@tparam float[4] input Maximum: 4
		@tparam int colorEditFlags optional
		@treturn float[4] color Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.ColorEdit4();
		*/
		ImGui.set_function("ColorEdit4", sol::overload(
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(ColorEdit4),
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, int)>(ColorEdit4)
		));
		/***
		Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
		@function ImGui.ColorPicker3
		@tparam string label
		@tparam float[3] input Maximum: 3
		@tparam int colorEditFlags optional
		@treturn float[3] color Maximum: 3
		@treturn bool returnBool
		@usage
			ImGui.ColorPicker3();
		*/
		ImGui.set_function("ColorPicker3", sol::overload(
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(ColorPicker3),
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, int)>(ColorPicker3)
		));
		/***
		Color Editor/Picker (tip: the ColorEdit functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.).
		@function ImGui.ColorPicker4
		@tparam string label
		@tparam float[4] input Maximum: 4
		@tparam int colorEditFlags optional
		@treturn float[4] color Maximum: 4
		@treturn bool returnBool
		@usage
			ImGui.ColorPicker4();
		*/
		ImGui.set_function("ColorPicker4", sol::overload(
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&)>(ColorPicker4),
			sol::resolve<std::tuple <sol::as_table_t<std::vector<float>>, bool>(const std::string&, const sol::table&, int)>(ColorPicker4)
		));
#pragma endregion Widgets: Color Editor / Picker

#pragma region Widgets: Trees
		/***
		TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents..
		@function ImGui.TreeNode
		@tparam string label
		@tparam string text helper variation to easily decorelate the id from the displayed string. optional
		@treturn bool returnBool
		@usage
			ImGui.TreeNode();
		*/
		ImGui.set_function("TreeNode", sol::overload(
			sol::resolve<bool(const std::string&)>(TreeNode),
			sol::resolve<bool(const std::string&, const std::string&)>(TreeNode)
		));
		/***
		TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
		@function ImGui.TreeNodeEx
		@tparam string label
		@tparam int treeNodeFlags optional
		@tparam string text helper variation to easily decorelate the id from the displayed string. optional
		@treturn bool returnBool
		@usage
			ImGui.TreeNodeEx();
		*/
		ImGui.set_function("TreeNodeEx", sol::overload(
			sol::resolve<bool(const std::string&)>(TreeNodeEx),
			sol::resolve<bool(const std::string&, int)>(TreeNodeEx),
			sol::resolve<bool(const std::string&, int, const std::string&)>(TreeNodeEx)
		));
		/***
		Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
		@function ImGui.TreePush
		@tparam string label
		@usage
			ImGui.TreePush();
		*/
		ImGui.set_function("TreePush", TreePush);
		/***
		Tree pop.
		@function ImGui.TreePop
		@usage
			ImGui.TreePop();
		*/
		ImGui.set_function("TreePop", TreePop);
		/***
		Horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode.
		@function ImGui.GetTreeNodeToLabelSpacing
		@treturn float spacing
		@usage
			ImGui.GetTreeNodeToLabelSpacing();
		*/
		ImGui.set_function("GetTreeNodeToLabelSpacing", GetTreeNodeToLabelSpacing);
		/***
		If returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop(). If second argument bool then display an additional small close button on upper right of the header which will set the bool to false when clicked.
		@function ImGui.CollapsingHeader
		@tparam string label
		@tparam int|bool openOrFlags
		@tparam int flags
		@treturn bool open
		@treturn bool returnBool2
		@usage
			ImGui.CollapsingHeader();
		*/
		ImGui.set_function("CollapsingHeader", sol::overload(
			sol::resolve<bool(const std::string&)>(CollapsingHeader),
			sol::resolve<bool(const std::string&, int)>(CollapsingHeader),
			sol::resolve<std::tuple<bool, bool>(const std::string&, bool)>(CollapsingHeader),
			sol::resolve<std::tuple<bool, bool>(const std::string&, bool, int)>(CollapsingHeader)
		));
		/***
		Set next TreeNode/CollapsingHeader open state.
		@function ImGui.SetNextItemOpen
		@tparam bool open
		@tparam int condition optional
		@usage
			ImGui.SetNextItemOpen();
		*/
		ImGui.set_function("SetNextItemOpen", sol::overload(
			sol::resolve<void(bool)>(SetNextItemOpen),
			sol::resolve<void(bool, int)>(SetNextItemOpen)
		));
#pragma endregion Widgets: Trees

#pragma region Widgets: Selectables
		/***
		A selectable highlights when hovered, and can display another color when selected. Neighbors selectable extend their highlight bounds in order to leave no gap between them. This is so a series of selected Selectable appear contiguous. "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
		@function ImGui.Selectable
		@tparam string label
		@tparam bool selected optional
		@tparam int selectableFlags optional
		@tparam float sizeX optional
		@tparam float sizeY optional
		@treturn bool value
		@usage
			ImGui.Selectable();
		*/
		ImGui.set_function("Selectable", sol::overload(
			sol::resolve<bool(const std::string&)>(Selectable),
			sol::resolve<bool(const std::string&, bool)>(Selectable),
			sol::resolve<bool(const std::string&, bool, int)>(Selectable),
			sol::resolve<bool(const std::string&, bool, int, float, float)>(Selectable)
		));
#pragma endregion Widgets: Selectables

#pragma region Widgets: List Boxes
		/***
		This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes. Open a framed scrolling region.
		@function ImGui.ListBox
		@tparam string label
		@tparam int currentItem
		@tparam table items
		@tparam int itemCount
		@tparam int heightInItems optional
		@usage
			ImGui.ListBox();
		*/
		ImGui.set_function("ListBox", sol::overload(
			sol::resolve<std::tuple<int, bool>(const std::string&, int, const sol::table&, int)>(ListBox),
			sol::resolve<std::tuple<int, bool>(const std::string&, int, const sol::table&, int, int)>(ListBox)
		));
		/***
		This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes.
		@function ImGui.ListBoxHeader
		@tparam float|int sizeXorCount
		@tparam float|int sizeYorHeight optional if first param is count
		@usage
			ImGui.ListBoxHeader();
		*/
		ImGui.set_function("ListBoxHeader", sol::overload(
			sol::resolve<bool(const std::string&, float, float)>(ListBoxHeader),
			sol::resolve<bool(const std::string&, int)>(ListBoxHeader),
			sol::resolve<bool(const std::string&, int, int)>(ListBoxHeader)
		));
		/***
		This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes.
		@function ImGui.ListBoxFooter
		@usage
			ImGui.ListBoxFooter();
		*/
		ImGui.set_function("ListBoxFooter", ListBoxFooter);
#pragma endregion Widgets: List Boxes

#pragma region Widgets: Value() Helpers
		/***
		This is merely a shortcut to calling Text() with a format string. Output single value in "name: value" format.
		@function ImGui.Value
		@tparam string prefix
		@tparam float|int|bool value
		@tparam string floatFormat optional
		@usage
			ImGui.Value();
		*/
		ImGui.set_function("Value", sol::overload(
			sol::resolve<void(const std::string&, bool)>(Value),
			sol::resolve<void(const std::string&, int)>(Value),
			sol::resolve<void(const std::string&, unsigned int)>(Value),
			sol::resolve<void(const std::string&, float)>(Value),
			sol::resolve<void(const std::string&, float, const std::string&)>(Value)
		));
#pragma endregion Widgets: Value() Helpers

#pragma region Widgets: Menu
		/***
		Append to menu-bar of current window (requires ImGuiWindowFlags.MenuBar flag set on parent window). Use BeginMenuBar() on a window ImGuiWindowFlags.MenuBar to append to its menu bar.
		@function ImGui.BeginMenuBar
		@treturn bool value
		@usage
			ImGui.BeginMenuBar();
		*/
		ImGui.set_function("BeginMenuBar", BeginMenuBar);
		/***
		Only call EndMenuBar() if BeginMenuBar() returns true!
		@function ImGui.EndMenuBar
		@usage
			ImGui.EndMenuBar();
		*/
		ImGui.set_function("EndMenuBar", EndMenuBar);
		/***
		Create and append to a full screen menu-bar. Use BeginMainMenuBar() to create a menu bar at the top of the screen and append to it.
		@function ImGui.BeginMainMenuBar
		@treturn bool value
		@usage
			ImGui.BeginMainMenuBar();
		*/
		ImGui.set_function("BeginMainMenuBar", BeginMainMenuBar);
		/***
		Only call EndMainMenuBar() if BeginMainMenuBar() returns true!
		@function ImGui.EndMainMenuBar
		@usage
			ImGui.EndMainMenuBar();
		*/
		ImGui.set_function("EndMainMenuBar", EndMainMenuBar);
		/***
		Create a sub-menu entry. only call EndMenu() if this returns true! Use BeginMenu() to create a menu. You can call BeginMenu() multiple time with the same identifier to append more items to it.
		@function ImGui.BeginMenu
		@tparam string id
		@tparam bool enabled optional
		@treturn bool value
		@usage
			ImGui.BeginMenu();
		*/
		ImGui.set_function("BeginMenu", sol::overload(
			sol::resolve<bool(const std::string&)>(BeginMenu),
			sol::resolve<bool(const std::string&, bool)>(BeginMenu)
		));
		/***
		Only call EndMenu() if BeginMenu() returns true!
		@function ImGui.EndMenu
		@usage
			ImGui.EndMenu();
		*/
		ImGui.set_function("EndMenu", EndMenu);
		/***
		Return true when activated.
		@function ImGui.MenuItem
		@tparam string label
		@tparam string shortcut optional
		@tparam bool selected optional
		@tparam bool enabled optional
		@treturn bool value
		@treturn bool value2 return true when activated + param selected is true
		@usage
			ImGui.MenuItem();
		*/
		ImGui.set_function("MenuItem", sol::overload(
			sol::resolve<bool(const std::string&)>(MenuItem),
			sol::resolve<bool(const std::string&, const std::string&)>(MenuItem),
			sol::resolve<std::tuple<bool, bool>(const std::string&, const std::string&, bool)>(MenuItem),
			sol::resolve<std::tuple<bool, bool>(const std::string&, const std::string&, bool, bool)>(MenuItem)
		));
#pragma endregion Widgets: Menu

#pragma region Tooltips
		/***
		Tooltip are windows following the mouse. They do not take focus away. Begin/append a tooltip window. To create full-featured tooltip (with any kind of items).
		@function ImGui.BeginTooltip
		@usage
			ImGui.BeginTooltip();
		*/
		ImGui.set_function("BeginTooltip", BeginTooltip);
		/***
		Tooltip are windows following the mouse. They do not take focus away.
		@function ImGui.EndTooltip
		@usage
			ImGui.EndTooltip();
		*/
		ImGui.set_function("EndTooltip", EndTooltip);
		/***
		Tooltip are windows following the mouse. They do not take focus away. Set a text-only tooltip, typically use with IsItemHovered(). Override any previous call to SetTooltip().
		@function ImGui.SetTooltip
		@tparam string text
		@usage
			ImGui.SetTooltip();
		*/
		ImGui.set_function("SetTooltip", SetTooltip);
#pragma endregion Tooltips

#pragma region Popups, Modals
		/***
		They block normal mouse hovering detection (and therefore most mouse interactions) behind them. They can be closed by clicking anywhere outside them, or by pressing ESCAPE. Their visibility state (~bool) is held internally instead of being held by the programmer as we are used to with regular Begin*() calls. The 3 properties above are related: we need to retain popup visibility state in the library because popups may be closed as any time. Popup identifiers are relative to the current ID stack, so OpenPopup and BeginPopup generally needs to be at the same level of the stack. BeginPopup(): query popup state, if open start appending into the window. Call EndPopup() afterwards. ImGuiWindowFlags are forwarded to the window.
		@function ImGui.BeginPopup
		@tparam string label
		@tparam int windowFlags optional
		@treturn bool value Return true if the popup is open, and you can start outputting to it.
		@usage
			ImGui.BeginPopup();
		*/
		ImGui.set_function("BeginPopup", sol::overload(
			sol::resolve<bool(const std::string&)>(BeginPopup),
			sol::resolve<bool(const std::string&, int)>(BeginPopup)
		));
		/***
		They block normal mouse hovering detection (and therefore most mouse interactions) behind them. Their visibility state (~bool) is held internally instead of being held by the programmer as we are used to with regular Begin*() calls. The 3 properties above are related: we need to retain popup visibility state in the library because popups may be closed as any time. Popup identifiers are relative to the current ID stack, so OpenPopup and BeginPopup generally needs to be at the same level of the stack. BeginPopupModal(): block every interactions behind the window, cannot be closed by user, add a dimming background, has a title bar.
		@function ImGui.BeginPopupModal
		@tparam string label
		@tparam bool open optional
		@tparam int windowFlags optional
		@treturn bool value Return true if the popup is open, and you can start outputting to it.
		@usage
			ImGui.BeginPopupModal();
		*/
		ImGui.set_function("BeginPopupModal", sol::overload(
			sol::resolve<bool(const std::string&)>(BeginPopupModal),
			sol::resolve<bool(const std::string&, bool)>(BeginPopupModal),
			sol::resolve<bool(const std::string&, bool, int)>(BeginPopupModal)
		));
		/***
		Only call EndPopup() if BeginPopupXXX() returns true!
		@function ImGui.EndPopup
		@usage
			ImGui.EndPopup();
		*/
		ImGui.set_function("EndPopup", EndPopup);
		/***
		Set popup state to open. ImGuiPopupFlags are available for opening options. If not modal they can be closed by clicking anywhere outside them, or by pressing ESCAPE. Use ImGuiPopupFlags.NoOpenOverExistingPopup to avoid opening a popup if there's already one at the same level. This is equivalent to e.g. testing for !IsAnyPopupOpen() prior to OpenPopup(). Use IsWindowAppearing() after BeginPopup() to tell if a window just opened.
		@function ImGui.OpenPopup
		@tparam string label
		@tparam int popupFlags optional
		@treturn bool value Return true if the popup is open, and you can start outputting to it.
		@usage
			ImGui.OpenPopup();
		*/
		ImGui.set_function("OpenPopup", sol::overload(
			sol::resolve<void(const std::string&)>(OpenPopup),
			sol::resolve<void(const std::string&, int)>(OpenPopup)
		));

		/***
		Manually close the popup we have begin-ed into. Use inside the BeginPopup()/EndPopup() scope to close manually. CloseCurrentPopup() is called by default by Selectable()/MenuItem() when activated.
		@function ImGui.CloseCurrentPopup
		@usage
			ImGui.CloseCurrentPopup();
		*/
		ImGui.set_function("CloseCurrentPopup", CloseCurrentPopup);
		/***
		Open+begin popup when clicked on last item. Use label nil to associate the popup to previous item. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking. They are convenient to easily create context menus, hence the name. Notice that BeginPopupContextXXX takes ImGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). We exceptionally default their flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the ImGuiPopupFlags_MouseButtonRight.
		@function ImGui.BeginPopupContextItem
		@tparam string label optional
		@tparam int popupFlags optional
		@treturn bool value
		@usage
			ImGui.BeginPopupContextItem();
		*/
		ImGui.set_function("BeginPopupContextItem", sol::overload(
			sol::resolve<bool()>(BeginPopupContextItem),
			sol::resolve<bool(const std::string&)>(BeginPopupContextItem),
			sol::resolve<bool(const std::string&, int)>(BeginPopupContextItem)
		));
		/***
		Open+begin popup when clicked on current window. OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking. They are convenient to easily create context menus, hence the name. Notice that BeginPopupContextXXX takes ImGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). We exceptionally default their flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the ImGuiPopupFlags_MouseButtonRight.
		@function ImGui.BeginPopupContextWindow
		@tparam string label optional
		@tparam int popupFlags optional
		@treturn bool value
		@usage
			ImGui.BeginPopupContextWindow();
		*/
		ImGui.set_function("BeginPopupContextWindow", sol::overload(
			sol::resolve<bool()>(BeginPopupContextWindow),
			sol::resolve<bool(const std::string&)>(BeginPopupContextWindow),
			sol::resolve<bool(const std::string&, int)>(BeginPopupContextWindow)
		));
		/***
		Open+begin popup when clicked in void (where there are no windows). Open+begin popup when clicked on current window. OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking. They are convenient to easily create context menus, hence the name. Notice that BeginPopupContextXXX takes ImGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). We exceptionally default their flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the ImGuiPopupFlags_MouseButtonRight.
		@function ImGui.BeginPopupContextVoid
		@tparam string label optional
		@tparam int popupFlags optional
		@treturn bool value
		@usage
			ImGui.BeginPopupContextVoid();
		*/
		ImGui.set_function("BeginPopupContextVoid", sol::overload(
			sol::resolve<bool()>(BeginPopupContextVoid),
			sol::resolve<bool(const std::string&)>(BeginPopupContextVoid),
			sol::resolve<bool(const std::string&, int)>(BeginPopupContextVoid)
		));
		/***
		Return true if the popup is open at the current BeginPopup() level of the popup stack. IsPopupOpen() with ImGuiPopupFlags_AnyPopupId: return true if any popup is open at the current BeginPopup() level of the popup stack. IsPopupOpen() with ImGuiPopupFlags_AnyPopupId + ImGuiPopupFlags_AnyPopupLevel: return true if any popup is open.
		@function ImGui.IsPopupOpen
		@tparam string label
		@tparam int popupFlags optional
		@treturn bool isOpen return true if the popup is open.
		@usage
			ImGui.IsPopupOpen();
		*/
		ImGui.set_function("IsPopupOpen", sol::overload(
			sol::resolve<bool(const std::string&)>(IsPopupOpen),
			sol::resolve<bool(const std::string&, int)>(IsPopupOpen)
		));
#pragma endregion Popups, Modals

#pragma region Columns
		/***
		Columns.
		@function ImGui.Columns
		@tparam int count
		@tparam string label optional
		@tparam bool border optional
		@usage
			ImGui.Columns();
		*/
		ImGui.set_function("Columns", sol::overload(
			sol::resolve<void()>(Columns),
			sol::resolve<void(int)>(Columns),
			sol::resolve<void(int, const std::string&)>(Columns),
			sol::resolve<void(int, const std::string&, bool)>(Columns)
		));
		/***
		Next column, defaults to current row or next row if the current row is finished.
		@function ImGui.NextColumn
		@usage
			ImGui.NextColumn();
		*/
		ImGui.set_function("NextColumn", NextColumn);
		/***
		Get current column index.
		@function ImGui.GetColumnIndex
		@treturn int index
		@usage
			ImGui.GetColumnIndex();
		*/
		ImGui.set_function("GetColumnIndex", GetColumnIndex);
		/***
		Get column width (in pixels). Pass -1 to use current column.
		@function ImGui.GetColumnWidth
		@tparam int index optional
		@treturn float width
		@usage
			ImGui.GetColumnWidth();
		*/
		ImGui.set_function("GetColumnWidth", sol::overload(
			sol::resolve<float()>(GetColumnWidth),
			sol::resolve<float(int)>(GetColumnWidth)
		));
		/***
		Set column width (in pixels). Pass -1 to use current column.
		@function ImGui.SetColumnWidth
		@tparam int index
		@tparam float width
		@usage
			ImGui.SetColumnWidth();
		*/
		ImGui.set_function("SetColumnWidth", SetColumnWidth);
		/***
		Get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f.
		@function ImGui.GetColumnOffset
		@tparam int index
		@tparam float offset
		@usage
			ImGui.GetColumnOffset();
		*/
		ImGui.set_function("GetColumnOffset", sol::overload(
			sol::resolve<float()>(GetColumnOffset),
			sol::resolve<float(int)>(GetColumnOffset)
		));
		/***
		Set position of column line (in pixels, from the left side of the contents region). Pass -1 to use current column.
		@function ImGui.SetColumnOffset
		@tparam int index
		@tparam float offset
		@usage
			ImGui.SetColumnOffset();
		*/
		ImGui.set_function("SetColumnOffset", SetColumnOffset);
		/***
		Get Columns Count.
		@function ImGui.GetColumnsCount
		@treturn int count
		@usage
			ImGui.GetColumnsCount();
		*/
		ImGui.set_function("GetColumnsCount", GetColumnsCount);
#pragma endregion Columns

#pragma region Tab Bars, Tabs
		/***
		Create and append into a TabBar.
		@function ImGui.BeginTabBar
		@tparam string label
		@tparam int flags optional
		@treturn bool value
		@usage
			ImGui.BeginTabBar();
		*/
		ImGui.set_function("BeginTabBar", sol::overload(
			sol::resolve<bool(const std::string&)>(BeginTabBar),
			sol::resolve<bool(const std::string&, int)>(BeginTabBar)
		));
		/***
		Only call EndTabBar() if BeginTabBar() returns true!
		@function ImGui.EndTabBar
		@usage
			ImGui.EndTabBar();
		*/
		ImGui.set_function("EndTabBar", EndTabBar);
		/***
		Create a Tab. Returns true if the Tab is selected.
		@function ImGui.BeginTabItem
		@tparam string label
		@tparam bool open optional
		@tparam int flags optional
		@treturn bool value
		@treturn bool value2
		@usage
			ImGui.BeginTabItem();
		*/
		ImGui.set_function("BeginTabItem", sol::overload(
			sol::resolve<bool(const std::string&)>(BeginTabItem),
			sol::resolve<std::tuple<bool, bool>(const std::string&, bool)>(BeginTabItem),
			sol::resolve<std::tuple<bool, bool>(const std::string&, bool, int)>(BeginTabItem)
		));
		/***
		Only call EndTabItem() if BeginTabItem() returns true!
		@function ImGui.EndTabItem
		@usage
			ImGui.EndTabItem();
		*/
		ImGui.set_function("EndTabItem", EndTabItem);
		/***
		Notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.
		@function ImGui.SetTabItemClosed
		@tparam string label
		@usage
			ImGui.SetTabItemClosed();
		*/
		ImGui.set_function("SetTabItemClosed", SetTabItemClosed);
#pragma endregion Tab Bars, Tabs

#pragma region Docking
#if defined(IMGUI_HAS_DOCK)
		ImGui.set_function("DockSpace", sol::overload(
			sol::resolve<void(unsigned int)>(DockSpace),
			sol::resolve<void(unsigned int, float, float)>(DockSpace),
			sol::resolve<void(unsigned int, float, float, int)>(DockSpace)
		));
		ImGui.set_function("SetNextWindowDockID", sol::overload(
			sol::resolve<void(unsigned int)>(SetNextWindowDockID),
			sol::resolve<void(unsigned int, int)>(SetNextWindowDockID)
		));
		ImGui.set_function("GetWindowDockID", GetWindowDockID);
		ImGui.set_function("IsWindowDocked", IsWindowDocked);
#endif
#pragma endregion Docking

#pragma region Logging / Capture
		/***
		Start logging to tty (stdout). All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
		@function ImGui.LogToTTY
		@tparam int autoOpenDepth optional
		@usage
			ImGui.LogToTTY();
		*/
		ImGui.set_function("LogToTTY", sol::overload(
			sol::resolve<void()>(LogToTTY),
			sol::resolve<void(int)>(LogToTTY)
		));
		/***
		Start logging to file. All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
		@function ImGui.LogToFile
		@tparam int autoOpenDepth
		@tparam string filename optional
		@usage
			ImGui.LogToFile();
		*/
		ImGui.set_function("LogToFile", sol::overload(
			sol::resolve<void(int)>(LogToFile),
			sol::resolve<void(int, const std::string&)>(LogToFile)
		));
		/***
		Start logging to OS clipboard. All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
		@function ImGui.LogToClipboard
		@tparam int autoOpenDepth optional
		@usage
			ImGui.LogToClipboard();
		*/
		ImGui.set_function("LogToClipboard", sol::overload(
			sol::resolve<void()>(LogToClipboard),
			sol::resolve<void(int)>(LogToClipboard)
		));
		/***
		Stop logging (close file, etc.).
		@function ImGui.LogFinish
		@usage
			ImGui.LogFinish();
		*/
		ImGui.set_function("LogFinish", LogFinish);
		/***
		Helper to display buttons for logging to tty/file/clipboard.
		@function ImGui.LogButtons
		@usage
			ImGui.LogButtons();
		*/
		ImGui.set_function("LogButtons", LogButtons);
		/***
		Pass text data straight to log (without being displayed).
		@function ImGui.LogText
		@tparam string text
		@usage
			ImGui.LogText();
		*/
		ImGui.set_function("LogText", LogText);
#pragma endregion Logging / Capture

#pragma region Clipping
		/***
		Mouse hovering is affected by PushClipRect() calls.
		@function ImGui.PushClipRect
		@tparam float minX
		@tparam float minY
		@tparam float maxX
		@tparam float maxY
		@tparam bool intersect
		@usage
			ImGui.PushClipRect();
		*/
		ImGui.set_function("PushClipRect", PushClipRect);
		/***
		Pop Clip Rect.
		@function ImGui.PopClipRect
		@usage
			ImGui.PopClipRect();
		*/
		ImGui.set_function("PopClipRect", PopClipRect);
#pragma endregion Clipping

#pragma region Focus, Activation
		/***
		Make last item the default focused item of a window. Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHereY()" when applicable to signify "this is the default item".
		@function ImGui.SetItemDefaultFocus
		@usage
			ImGui.SetItemDefaultFocus();
		*/
		ImGui.set_function("SetItemDefaultFocus", SetItemDefaultFocus);
		/***
		Focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.
		@function ImGui.SetKeyboardFocusHere
		@tparam int offset optional
		@usage
			ImGui.SetKeyboardFocusHere();
		*/
		ImGui.set_function("SetKeyboardFocusHere", sol::overload(
			sol::resolve<void()>(SetKeyboardFocusHere),
			sol::resolve<void(int)>(SetKeyboardFocusHere)
		));
#pragma endregion Focus, Activation

#pragma region Item/Widgets Utilities
		/***
		Is the last item hovered? (and usable, aka not blocked by a popup, etc.). See ImGuiHoveredFlags for more options.
		@function ImGui.IsItemHovered
		@tparam int flags optional
		@treturn bool result
		@usage
			ImGui.IsItemHovered();
		*/
		ImGui.set_function("IsItemHovered", sol::overload(
			sol::resolve<bool()>(IsItemHovered),
			sol::resolve<bool(int)>(IsItemHovered)
		));
		/***
		Is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
		@function ImGui.IsItemActive
		@treturn bool result
		@usage
			ImGui.IsItemActive();
		*/
		ImGui.set_function("IsItemActive", IsItemActive);
		/***
		Is the last item focused for keyboard/gamepad navigation?
		@function ImGui.IsItemFocused
		@treturn bool result
		@usage
			ImGui.IsItemFocused();
		*/
		ImGui.set_function("IsItemFocused", IsItemFocused);
		/***
		Is the last item hovered and mouse clicked on? (**)  == IsMouseClicked(mouse_button) && IsItemHovered()Important. (**) this it NOT equivalent to the behavior of e.g. Button().
		@function ImGui.IsItemClicked
		@tparam int flags optional
		@treturn bool result
		@usage
			ImGui.IsItemClicked();
		*/
		ImGui.set_function("IsItemClicked", sol::overload(
			sol::resolve<bool()>(IsItemClicked),
			sol::resolve<bool(int)>(IsItemClicked)
		));
		/***
		Is the last item visible? (items may be out of sight because of clipping/scrolling)
		@function ImGui.IsItemVisible
		@treturn bool result
		@usage
			ImGui.IsItemVisible();
		*/
		ImGui.set_function("IsItemVisible", IsItemVisible);
		/***
		Did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets
		@function ImGui.IsItemEdited
		@treturn bool result
		@usage
			ImGui.IsItemEdited();
		*/
		ImGui.set_function("IsItemEdited", IsItemEdited);
		/***
		Was the last item just made active (item was previously inactive).
		@function ImGui.IsItemActivated
		@treturn bool result
		@usage
			ImGui.IsItemActivated();
		*/
		ImGui.set_function("IsItemActivated", IsItemActivated);
		/***
		Was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
		@function ImGui.IsItemDeactivated
		@treturn bool result
		@usage
			ImGui.IsItemDeactivated();
		*/
		ImGui.set_function("IsItemDeactivated", IsItemDeactivated);
		/***
		Was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
		@function ImGui.IsItemDeactivatedAfterEdit
		@treturn bool result
		@usage
			ImGui.IsItemDeactivatedAfterEdit();
		*/
		ImGui.set_function("IsItemDeactivatedAfterEdit", IsItemDeactivatedAfterEdit);
		/***
		Was the last item open state toggled? set by TreeNode().
		@function ImGui.IsItemToggledOpen
		@treturn bool result
		@usage
			ImGui.IsItemToggledOpen();
		*/
		ImGui.set_function("IsItemToggledOpen", IsItemToggledOpen);
		/***
		Is any item hovered?
		@function ImGui.IsAnyItemHovered
		@treturn bool result
		@usage
			ImGui.IsAnyItemHovered();
		*/
		ImGui.set_function("IsAnyItemHovered", IsAnyItemHovered);
		/***
		Is any item active?
		@function ImGui.IsAnyItemActive
		@treturn bool result
		@usage
			ImGui.IsAnyItemActive();
		*/
		ImGui.set_function("IsAnyItemActive", IsAnyItemActive);
		/***
		Is any item focused?
		@function ImGui.IsAnyItemFocused
		@treturn bool result
		@usage
			ImGui.IsAnyItemFocused();
		*/
		ImGui.set_function("IsAnyItemFocused", IsAnyItemFocused);
		/***
		Get upper-left bounding rectangle of the last item (screen space).
		@function ImGui.GetItemRectMin
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetItemRectMin();
		*/
		ImGui.set_function("GetItemRectMin", GetItemRectMin);
		/***
		get lower-right bounding rectangle of the last item (screen space).
		@function ImGui.GetItemRectMax
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetItemRectMax();
		*/
		ImGui.set_function("GetItemRectMax", GetItemRectMax);
		/***
		Get size of last item.
		@function ImGui.GetItemRectSize
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetItemRectSize();
		*/
		ImGui.set_function("GetItemRectSize", GetItemRectSize);
		/***
		Allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.
		@function ImGui.SetItemAllowOverlap
		@usage
			ImGui.SetItemAllowOverlap();
		*/
		ImGui.set_function("SetItemAllowOverlap", SetItemAllowOverlap);
#pragma endregion Item/Widgets Utilities

#pragma region Miscellaneous Utilities
		/***
		Test if rectangle (of given size, starting from cursor position) is visible / not clipped.
		@function ImGui.IsRectVisible
		@tparam float x
		@tparam float y
		@tparam float maxX optional
		@tparam float maxY optional
		@treturn bool result
		@usage
			ImGui.IsRectVisible();
		*/
		ImGui.set_function("IsRectVisible", sol::overload(
			sol::resolve<bool(float, float)>(IsRectVisible),
			sol::resolve<bool(float, float, float, float)>(IsRectVisible)
		));
		/***
		Get global imgui time. incremented by io.DeltaTime every frame..
		@function ImGui.GetTime
		@treturn float time
		@usage
			ImGui.GetTime();
		*/
		ImGui.set_function("GetTime", GetTime);
		/***
		Get global imgui frame count. Incremented by 1 every frame.
		@function ImGui.GetFrameCount
		@treturn int count
		@usage
			ImGui.GetFrameCount();
		*/
		ImGui.set_function("GetFrameCount", GetFrameCount);
		/***
		Get a string corresponding to the enum value (for display, saving, etc.).
		@function ImGui.GetStyleColorName
		@tparam int imGuiCol use enum ImGuiCol
		@treturn string name
		@usage
			ImGui.GetStyleColorName();
		*/
		ImGui.set_function("GetStyleColorName", GetStyleColorName);
		/***
		Helper to create a child window / scrolling region that looks like a normal widget frame
		@function ImGui.BeginChildFrame
		@tparam int id
		@tparam float sizeX
		@tparam float sizeY
		@tparam int flags optional
		@treturn bool result
		@usage
			ImGui.BeginChildFrame();
		*/
		ImGui.set_function("BeginChildFrame", sol::overload(
			sol::resolve<bool(unsigned int, float, float)>(BeginChildFrame),
			sol::resolve<bool(unsigned int, float, float, int)>(BeginChildFrame)
		));
		/***
		Always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)
		@function ImGui.EndChildFrame
		@usage
			ImGui.EndChildFrame();
		*/
		ImGui.set_function("EndChildFrame", EndChildFrame);
#pragma endregion Miscellaneous Utilities

#pragma region Text Utilities
		/***
		Calculate text size.
		@function ImGui.CalcTextSize
		@tparam string text
		@tparam string textEnd optional
		@tparam bool hideAfterDoubleHash optional
		@tparam float wrapWidth optional
		@treturn float sizeX
		@treturn float sizeY
		@usage
			ImGui.CalcTextSize();
		*/
		ImGui.set_function("CalcTextSize", sol::overload(
			sol::resolve<std::tuple<float, float>(const std::string&)>(CalcTextSize),
			sol::resolve<std::tuple<float, float>(const std::string&, const std::string&)>(CalcTextSize),
			sol::resolve<std::tuple<float, float>(const std::string&, const std::string&, bool)>(CalcTextSize),
			sol::resolve<std::tuple<float, float>(const std::string&, const std::string&, bool, float)>(CalcTextSize)
		));
#pragma endregion Text Utilities

#pragma region Color Utilities
#ifdef SOL_IMGUI_USE_COLOR_U32
		ImGui.set_function("ColorConvertU32ToFloat4", ColorConvertU32ToFloat4);
		ImGui.set_function("ColorConvertFloat4ToU32", ColorConvertFloat4ToU32);
#endif
		/***
		Convert Color.
		@function ImGui.ColorConvertRGBtoHSV
		@tparam float R
		@tparam float G
		@tparam float B
		@treturn float H
		@treturn float S
		@treturn float V
		@usage
			ImGui.ColorConvertRGBtoHSV();
		*/
		ImGui.set_function("ColorConvertRGBtoHSV", ColorConvertRGBtoHSV);
		/***
		Convert Color.
		@function ImGui.ColorConvertHSVtoRGB
		@tparam float H
		@tparam float S
		@tparam float V
		@treturn float R
		@treturn float G
		@treturn float B
		@usage
			ImGui.ColorConvertHSVtoRGB();
		*/
		ImGui.set_function("ColorConvertHSVtoRGB", ColorConvertHSVtoRGB);
#pragma endregion Color Utilities

#pragma region Inputs Utilities: Keyboard
		/***
		Get key index.
		@function ImGui.GetKeyIndex
		@tparam int key use enum
		@treturn int index
		@usage
			ImGui.GetKeyIndex();
		*/
		ImGui.set_function("GetKeyIndex", GetKeyIndex);
		/***
		Is key down.
		@function ImGui.IsKeyDown
		@tparam int key use getKeyIndex
		@treturn bool result
		@usage
			ImGui.IsKeyDown();
		*/
		ImGui.set_function("IsKeyDown", IsKeyDown);
		/***
		Is key pressed.
		@function ImGui.IsKeyPressed
		@tparam int key use getKeyIndex
		@tparam bool doRepeat optional
		@treturn bool result
		@usage
			ImGui.IsKeyPressed();
		*/
		ImGui.set_function("IsKeyPressed", sol::overload(
			sol::resolve<bool(int)>(IsKeyPressed),
			sol::resolve<bool(int, bool)>(IsKeyPressed)
		));
		/***
		Is key released.
		@function ImGui.IsKeyReleased
		@tparam int key use getKeyIndex
		@treturn bool result
		@usage
			ImGui.IsKeyReleased();
		*/
		ImGui.set_function("IsKeyReleased", IsKeyReleased);
		/***
		Attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after the next NewFrame() call.
		@function ImGui.CaptureKeyboardFromApp
		@tparam bool wantCaptureKeyboardValue optional
		@usage
			ImGui.CaptureKeyboardFromApp();
		*/
		ImGui.set_function("CaptureKeyboardFromApp", sol::overload(
			sol::resolve<void()>(CaptureKeyboardFromApp),
			sol::resolve<void(bool)>(CaptureKeyboardFromApp)
		));
#pragma endregion Inputs Utilities: Keyboard

#pragma region Inputs Utilities: Mouse
		/***
		Is mouse button held?
		@function ImGui.IsMouseDown
		@tparam int button use MouseButton enum
		@treturn bool result
		@usage
			ImGui.IsMouseDown();
		*/
		ImGui.set_function("IsMouseDown", IsMouseDown);
		/***
		Did mouse button click? (went from !Down to Down)
		@function ImGui.IsMouseClicked
		@tparam int button use MouseButton enum
		@tparam bool doRepeat optional
		@treturn bool result
		@usage
			ImGui.IsMouseClicked();
		*/
		ImGui.set_function("IsMouseClicked", sol::overload(
			sol::resolve<bool(int)>(IsMouseClicked),
			sol::resolve<bool(int, bool)>(IsMouseClicked)
		));
		/***
		Did mouse button released? (went from Down to !Down)
		@function ImGui.IsMouseReleased
		@tparam int button use MouseButton enum
		@treturn bool result
		@usage
			ImGui.IsMouseReleased();
		*/
		ImGui.set_function("IsMouseReleased", IsMouseReleased);
		/***
		Did mouse button double-clicked? (note that a double-click will also report IsMouseClicked() == true).
		@function ImGui.IsMouseDoubleClicked
		@tparam int button use MouseButton enum
		@treturn bool result
		@usage
			ImGui.IsMouseDoubleClicked();
		*/
		ImGui.set_function("IsMouseDoubleClicked", IsMouseDoubleClicked);
		/***
		Is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
		@function ImGui.IsMouseHoveringRect
		@tparam float minX
		@tparam float minY
		@tparam float maxX
		@tparam float maxY
		@tparam bool clip optional
		@treturn bool result
		@usage
			ImGui.IsMouseHoveringRect();
		*/
		ImGui.set_function("IsMouseHoveringRect", sol::overload(
			sol::resolve<bool(float, float, float, float)>(IsMouseHoveringRect),
			sol::resolve<bool(float, float, float, float, bool)>(IsMouseHoveringRect)
		));
		/***
		Is any mouse button held?
		@function ImGui.IsAnyMouseDown
		@treturn bool result
		@usage
			ImGui.IsAnyMouseDown();
		*/
		ImGui.set_function("IsAnyMouseDown", IsAnyMouseDown);
		/***
		Get mouse pos.
		@function ImGui.GetMousePos
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetMousePos();
		*/
		ImGui.set_function("GetMousePos", GetMousePos);
		/***
		Retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves).
		@function ImGui.GetMousePosOnOpeningCurrentPopup
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetMousePosOnOpeningCurrentPopup();
		*/
		ImGui.set_function("GetMousePosOnOpeningCurrentPopup", GetMousePosOnOpeningCurrentPopup);
		/***
		Is mouse dragging?
		@function ImGui.IsMouseDragging
		@tparam int button use MouseButton enum
		@tparam float lockThreshold optional
		@usage
			ImGui.IsMouseDragging();
		*/
		ImGui.set_function("IsMouseDragging", sol::overload(
			sol::resolve<bool(int)>(IsMouseDragging),
			sol::resolve<bool(int, float)>(IsMouseDragging)
		));
		/***
		Return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
		@function ImGui.GetMouseDragDelta
		@tparam int button use MouseButton enum optional
		@tparam float lockThreshold optional
		@treturn float x
		@treturn float y
		@usage
			ImGui.GetMouseDragDelta();
		*/
		ImGui.set_function("GetMouseDragDelta", sol::overload(
			sol::resolve<std::tuple<float, float>()>(GetMouseDragDelta),
			sol::resolve<std::tuple<float, float>(int)>(GetMouseDragDelta),
			sol::resolve<std::tuple<float, float>(int, float)>(GetMouseDragDelta)
		));
		/***
		Reset mouse drag delta.
		@function ImGui.ResetMouseDragDelta
		@tparam int button use MouseButton enum optional
		@usage
			ImGui.ResetMouseDragDelta();
		*/
		ImGui.set_function("ResetMouseDragDelta", sol::overload(
			sol::resolve<void()>(ResetMouseDragDelta),
			sol::resolve<void(int)>(ResetMouseDragDelta)
		));
		/***
		Get desired cursor type, reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
		@function ImGui.GetMouseCursor
		@treturn any mouseCursor
		@usage
			ImGui.GetMouseCursor();
		*/
		ImGui.set_function("GetMouseCursor", GetMouseCursor);
		/***
		Set desired cursor type
		@function ImGui.SetMouseCursor
		@tparam any mouseCursor
		@usage
			ImGui.SetMouseCursor();
		*/
		ImGui.set_function("SetMouseCursor", SetMouseCursor);
		/***
		Attention: misleading name! manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle). This is equivalent to setting "io.WantCaptureMouse = wantCaptureMouseValue;" after the next NewFrame() call.
		@function ImGui.CaptureMouseFromApp
		@tparam bool wantCaptureMouseValue optional
		@usage
			ImGui.CaptureMouseFromApp();
		*/
		ImGui.set_function("CaptureMouseFromApp", sol::overload(
			sol::resolve<void()>(CaptureMouseFromApp),
			sol::resolve<void(bool)>(CaptureMouseFromApp)
		));
#pragma endregion Inputs Utilities: Mouse

#pragma region Clipboard Utilities
		/***
		Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
		@function ImGui.GetClipboardText
		@treturn string text
		@usage
			ImGui.GetClipboardText();
		*/
		ImGui.set_function("GetClipboardText", GetClipboardText);
		/***
		Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
		@function ImGui.SetClipboardText
		@tparam string text
		@usage
			ImGui.SetClipboardText();
		*/
		ImGui.set_function("SetClipboardText", SetClipboardText);
#pragma endregion Clipboard Utilities


#pragma region IO
		/***
		Access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame!
		@function ImGui.GetStyle
		@treturn ImGuiStyle style
		@usage
			ImGui.GetStyle();
		*/
		ImGui.set_function("GetStyle", ImGui::GetStyle);


		/***
		Access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags).
		@function ImGui.GetIO
		@treturn ImGuiIO io
		@usage
			ImGui.GetIO();
		*/
		ImGui.set_function("GetIO", GetIO);

		///ImVec4
		//@section ImVec4

		/***
		Basic ImVec4 table

		@tfield float x
		@tfield float y
		@tfield float z
		@tfield float w
		@table ImVec4
		*/
		sol::usertype<ImVec4> imvec4 = lua.new_usertype<ImVec4>("ImVec4");
		imvec4.set("x", &ImVec4::x);
		imvec4.set("y", &ImVec4::y);
		imvec4.set("z", &ImVec4::z);
		imvec4.set("w", &ImVec4::w);

		///ImVec2
		//@section ImVec2

		/***
		Basic ImVec2 table

		@tfield float x
		@tfield float y
		@table ImVec2
		*/
		sol::usertype<ImVec2> imvec2 = lua.new_usertype<ImVec2>("ImVec2");
		imvec2.set("x", &ImVec2::x);
		imvec2.set("y", &ImVec2::y);


		///ImGuiStyle
		//@section ImGuiStyle

		/***
		Basic ImGuiStyle table

		@tfield ImVec4[54] colors Maximum: 54
		@tfield float PopupRounding
		@tfield ImVec2 WindowPadding
		@tfield ImVec2 FramePadding
		@tfield ImVec2 ItemSpacing
		@tfield float ScrollbarSize
		@tfield float WindowBorderSize
		@tfield float ChildBorderSize
		@tfield float PopupBorderSize
		@tfield float FrameBorderSize
		@tfield float WindowRounding
		@tfield float ChildRounding
		@tfield float FrameRounding
		@tfield float ScrollbarRounding
		@tfield float GrabRounding
		@table ImGuiStyle
		*/
		sol::usertype<ImGuiStyle> imguiStyle = lua.new_usertype<ImGuiStyle>("ImGuiStyle");
		imguiStyle.set("Colors", sol::property([](ImGuiStyle& self) { return std::ref(self.Colors); }));
		imguiStyle.set("PopupRounding", &ImGuiStyle::PopupRounding);
		imguiStyle.set("WindowPadding", &ImGuiStyle::WindowPadding);
		imguiStyle.set("FramePadding", &ImGuiStyle::FramePadding);
		imguiStyle.set("ItemSpacing", &ImGuiStyle::ItemSpacing);
		imguiStyle.set("ScrollbarSize", &ImGuiStyle::ScrollbarSize);
		imguiStyle.set("WindowBorderSize", &ImGuiStyle::WindowBorderSize);
		imguiStyle.set("ChildBorderSize", &ImGuiStyle::ChildBorderSize);
		imguiStyle.set("PopupBorderSize", &ImGuiStyle::PopupBorderSize);
		imguiStyle.set("FrameBorderSize", &ImGuiStyle::FrameBorderSize);
		imguiStyle.set("WindowRounding", &ImGuiStyle::WindowRounding);
		imguiStyle.set("ChildRounding", &ImGuiStyle::ChildRounding);
		imguiStyle.set("FrameRounding", &ImGuiStyle::FrameRounding);
		imguiStyle.set("ScrollbarRounding", &ImGuiStyle::ScrollbarRounding);
		imguiStyle.set("GrabRounding", &ImGuiStyle::GrabRounding);

		///ImFont
		//@section ImFont
		/***
		Basic ImFont table
		
		@table ImFont
		*/
		sol::usertype<ImFont> Font = lua.new_usertype<ImFont>("ImFont");

		///ImFontAtlas
		//@section ImFontAtlas
		/***
		Basic ImFontAtlas table
		
		@tfield AddFontFromFileTTF AddFontFromFileTTF
		
		@table ImFontAtlas
		*/
		sol::usertype<ImFontAtlas> FontAtlas = lua.new_usertype<ImFontAtlas>("ImFontAtlas");

		/***
		Access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags).
		@function FontAtlas.AddFontFromFileTTF
		@tparam string filename
		@tparam float sizePixels
		@treturn ImGuiIO io
		@usage
			FontAtlas.AddFontFromFileTTF();
		*/
		FontAtlas.set_function("AddFontFromFileTTF", &ImFontAtlas::AddFontFromFileTTF);

		///ImGuiIO
		//@section ImGuiIO
		/***
		Basic ImGuiIO table

		@tfield float[512] KeysDownDuration Maximum: 512
		@tfield ImFontAtlas Fonts

		@table ImGuiIO
		*/
		sol::usertype<ImGuiIO>IO = lua.new_usertype<ImGuiIO>("ImGuiIO");
		IO["KeysDownDuration"] = sol::property([](ImGuiIO* self) { return std::ref(self->KeysDownDuration); });



		IO.set("Fonts", &ImGuiIO::Fonts);

#pragma endregion IO
	}
}