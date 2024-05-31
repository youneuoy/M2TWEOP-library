///
//@module LuaPluginImGui
//@author youneuoy
//@license GPL-3.0
#include "sol_ImGui.h"
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
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY, int childFlags) { return ImGui::BeginChild(name.c_str(), { sizeX, sizeY }, childFlags); }
	inline bool BeginChild(const std::string& name, float sizeX, float sizeY, int childFlags, int windowFlags) { return ImGui::BeginChild(name.c_str(), { sizeX, sizeY }, static_cast<ImGuiChildFlags>(childFlags), static_cast<ImGuiWindowFlags>(windowFlags)); }
	inline void EndChild() { ImGui::EndChild(); }

	// Windows Utilities
	inline bool IsWindowAppearing() { return ImGui::IsWindowAppearing(); }
	inline bool IsWindowCollapsed() { return ImGui::IsWindowCollapsed(); }
	inline bool IsWindowFocused() { return ImGui::IsWindowFocused(); }
	inline bool IsWindowFocused(int flags) { return ImGui::IsWindowFocused(static_cast<ImGuiFocusedFlags>(flags)); }
	inline bool IsWindowHovered() { return ImGui::IsWindowHovered(); }
	inline bool IsWindowHovered(int flags) { return ImGui::IsWindowHovered(static_cast<ImGuiHoveredFlags>(flags)); }
	inline ImDrawList* GetWindowDrawList() { return ImGui::GetWindowDrawList();}
#if defined(IMGUI_HAS_DOCK)
	inline float GetWindowDpiScale() { return ImGui::GetWindowDpiScale(); }
	inline ImGuiViewport* GetWindowViewport() { return nullptr; /* TODO: GetWindowViewport() ==> UNSUPPORTED */ }
#endif
	inline void PushClipRectDraw(ImDrawList* drawlist, float posXmin, float posYmin, float posXmax, float posYmax)
	{
		drawlist->PushClipRect({posXmin, posYmin}, {posXmax, posYmax});
	}
	inline void PushClipRectDraw(ImDrawList* drawlist, float posXmin, float posYmin, float posXmax, float posYmax, bool intersect)
	{
		drawlist->PushClipRect({posXmin, posYmin}, {posXmax, posYmax}, intersect);
	}
	inline void PopClipRectDraw(ImDrawList* drawlist)
	{
		drawlist->PopClipRect();
	}
	inline ImVec2 GetClipRectMin(ImDrawList* drawlist)
	{
		return drawlist->GetClipRectMin();
	}
	inline ImVec2 GetClipRectMax(ImDrawList* drawlist)
	{
		return drawlist->GetClipRectMax();
	}
	inline void AddLine(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color)
	{
		drawlist->AddLine({p1X, p1Y}, {p2X, p2y}, color);
	}
	inline void AddLine(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float thickness)
	{
		drawlist->AddLine({p1X, p1Y}, {p2X, p2y}, color, thickness);
	}
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color)
	{
		drawlist->AddRect({p1X, p1Y}, {p2X, p2y}, color);
	}
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding)
	{
		drawlist->AddRect({p1X, p1Y}, {p2X, p2y}, color, rounding);
	}
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding, int flags)
	{
		drawlist->AddRect({p1X, p1Y}, {p2X, p2y}, color, rounding, flags);
	}
	inline void AddRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding, int flags, float thickness)
	{
		drawlist->AddRect({p1X, p1Y}, {p2X, p2y}, color, rounding, flags, thickness);
	}
	inline void AddRectFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color)
	{
		drawlist->AddRectFilled({p1X, p1Y}, {p2X, p2y}, color);
	}
	inline void AddRectFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding)
	{
		drawlist->AddRectFilled({p1X, p1Y}, {p2X, p2y}, color, rounding);
	}
	inline void AddRectFilled(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y, uint32_t color, float rounding, int flags)
	{
		drawlist->AddRectFilled({p1X, p1Y}, {p2X, p2y}, color, rounding, flags);
	}
	inline void AddRectFilledMultiColor(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2y
		, uint32_t colorUpperLeft
		, uint32_t colorUpperRight
		, uint32_t colorBottomRight
		, uint32_t colorBottomLeft
		)
	{
		drawlist->AddRectFilledMultiColor({p1X, p1Y}, {p2X, p2y}, colorUpperLeft, colorUpperRight, colorBottomRight, colorBottomLeft);
	}
	inline void AddQuad(ImDrawList* drawlist,
		float p1X, float p1Y, float p2X, float p2y,
		float p3X, float p3Y, float p4X, float p4y,
		uint32_t color)
	{
		drawlist->AddQuad({p1X, p1Y}, {p2X, p2y}, {p3X, p3Y}, {p4X, p4y}, color);
	}
	inline void AddQuad(ImDrawList* drawlist,
		float p1X, float p1Y, float p2X, float p2y,
		float p3X, float p3Y, float p4X, float p4y,
		uint32_t color, float thickness)
	{
		drawlist->AddQuad({p1X, p1Y}, {p2X, p2y}, {p3X, p3Y}, {p4X, p4y}, color, thickness);
	}
	inline void AddQuadFilled(ImDrawList* drawlist,
		float p1X, float p1Y, float p2X, float p2y,
		float p3X, float p3Y, float p4X, float p4y,
		uint32_t color)
	{
		drawlist->AddQuadFilled({p1X, p1Y}, {p2X, p2y}, {p3X, p3Y}, {p4X, p4y}, color);
	}
	inline void AddTriangle(ImDrawList* drawlist,
		float p1X, float p1Y, float p2X, float p2y,
		float p3X, float p3Y,
		uint32_t color)
	{
		drawlist->AddTriangle({p1X, p1Y}, {p2X, p2y}, {p3X, p3Y}, color);
	}
	inline void AddTriangle(ImDrawList* drawlist,
		float p1X, float p1Y, float p2X, float p2y,
		float p3X, float p3Y,
		uint32_t color, float thickness)
	{
		drawlist->AddTriangle({p1X, p1Y}, {p2X, p2y}, {p3X, p3Y}, color, thickness);
	}
	inline void AddTriangleFilled(ImDrawList* drawlist,
		float p1X, float p1Y, float p2X, float p2y,
		float p3X, float p3Y,
		uint32_t color)
	{
		drawlist->AddTriangleFilled({p1X, p1Y}, {p2X, p2y}, {p3X, p3Y}, color);
	}
	inline void AddCircle(ImDrawList* drawlist, float centerX, float centerY, float radius, uint32_t color)
	{
		drawlist->AddCircle({centerX, centerY}, radius, color);
	}
	inline void AddCircle(ImDrawList* drawlist, float centerX, float centerY, float radius,
		uint32_t color, int numSegments)
	{
		drawlist->AddCircle({centerX, centerY}, radius, color, numSegments);
	}
	inline void AddCircle(ImDrawList* drawlist, float centerX, float centerY, float radius,
		uint32_t color, int numSegments, float thickness)
	{
		drawlist->AddCircle({centerX, centerY}, radius, color, numSegments, thickness);
	}
	inline void AddCircleFilled(ImDrawList* drawlist, float centerX, float centerY, float radius, uint32_t color)
	{
		drawlist->AddCircleFilled({centerX, centerY}, radius, color);
	}
	inline void AddCircleFilled(ImDrawList* drawlist, float centerX, float centerY, float radius,
		uint32_t color, int numSegments)
	{
		drawlist->AddCircleFilled({centerX, centerY}, radius, color, numSegments);
	}
	inline void AddNgon(ImDrawList* drawlist, float centerX, float centerY, float radius,
		uint32_t color, int numSegments)
	{
		drawlist->AddNgon({centerX, centerY}, radius, color, numSegments);
	}
	inline void AddNgon(ImDrawList* drawlist, float centerX, float centerY, float radius,
		uint32_t color, int numSegments, float thickness)
	{
		drawlist->AddNgon({centerX, centerY}, radius, color, numSegments, thickness);
	}
	inline void AddNgonFilled(ImDrawList* drawlist, float centerX, float centerY, float radius,
		uint32_t color, int numSegments)
	{
		drawlist->AddNgonFilled({centerX, centerY}, radius, color, numSegments);
	}
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color)
	{
		drawlist->AddEllipse({centerX, centerY}, radiusX, radiusY, color);
	}
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY,
		 uint32_t color, float rotation)
	{
		drawlist->AddEllipse({centerX, centerY}, radiusX, radiusY, color, rotation);
	}
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY,
		 uint32_t color, float rotation, int numSegments)
	{
		drawlist->AddEllipse({centerX, centerY}, radiusX, radiusY, color, rotation, numSegments);
	}
	inline void AddEllipse(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY,
		 uint32_t color, float rotation, int numSegments, float thickness)
	{
		drawlist->AddEllipse({centerX, centerY}, radiusX, radiusY, color, rotation, numSegments, thickness);
	}
	inline void AddEllipseFilled(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, uint32_t color)
	{
		drawlist->AddEllipseFilled({centerX, centerY}, radiusX, radiusY, color);
	}
	inline void AddEllipseFilled(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY,
		 uint32_t color, float rotation)
	{
		drawlist->AddEllipseFilled({centerX, centerY}, radiusX, radiusY, color, rotation);
	}
	inline void AddEllipseFilled(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY,
		 uint32_t color, float rotation, int numSegments)
	{
		drawlist->AddEllipseFilled({centerX, centerY}, radiusX, radiusY, color, rotation, numSegments);
	}
	inline void AddText(ImDrawList* drawlist, float posX, float posY, uint32_t color, const char* textBegin)
	{
		drawlist->AddText({posX, posY}, color, textBegin);
	}
	inline void AddText(ImDrawList* drawlist, float posX, float posY, uint32_t color, const char* textBegin, const char* textEnd)
	{
		drawlist->AddText({posX, posY}, color, textBegin, textEnd);
	}
	inline void AddBezierCubic(ImDrawList* drawlist, float p1X, float p1Y, 
	    float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y,
	    uint32_t color, float thickness)
	{
		drawlist->AddBezierCubic({p1X, p1Y}, {p2X, p2Y}, {p3X, p3Y}, {p4X, p4Y}, color, thickness);
	}
	inline void AddBezierCubic(ImDrawList* drawlist, float p1X, float p1Y, 
	    float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y,
	    uint32_t color, float thickness, int numSegments)
	{
		drawlist->AddBezierCubic({p1X, p1Y}, {p2X, p2Y}, {p3X, p3Y}, {p4X, p4Y}, color, thickness, numSegments);
	}
	inline void AddBezierQuadratic(ImDrawList* drawlist, float p1X, float p1Y, 
	    float p2X, float p2Y, float p3X, float p3Y,
	    uint32_t color, float thickness)
	{
		drawlist->AddBezierQuadratic({p1X, p1Y}, {p2X, p2Y}, {p3X, p3Y}, color, thickness);
	}
	inline void AddBezierQuadratic(ImDrawList* drawlist, float p1X, float p1Y, 
	    float p2X, float p2Y, float p3X, float p3Y,
	    uint32_t color, float thickness, int numSegments)
	{
		drawlist->AddBezierQuadratic({p1X, p1Y}, {p2X, p2Y}, {p3X, p3Y}, color, thickness, numSegments);
	}
	inline void PathClear(ImDrawList* drawlist)
	{
		drawlist->PathClear();
	}
	inline void PathLineTo(ImDrawList* drawlist, float posX, float posY)
	{
		drawlist->PathLineTo({posX, posY});
	}
	inline void PathLineToMergeDuplicate(ImDrawList* drawlist, float posX, float posY)
	{
		drawlist->PathLineToMergeDuplicate({posX, posY});
	}
	inline void PathFillConvex(ImDrawList* drawlist, uint32_t color)
	{
		drawlist->PathFillConvex(color);
	}
	inline void PathStroke(ImDrawList* drawlist, uint32_t color)
	{
		drawlist->PathStroke(color);
	}
	inline void PathStroke(ImDrawList* drawlist, uint32_t color, int flags)
	{
		drawlist->PathStroke(color, flags);
	}
	inline void PathStroke(ImDrawList* drawlist, uint32_t color, int flags, float thickness)
	{
		drawlist->PathStroke(color, flags, thickness);
	}
	inline void PathArcTo(ImDrawList* drawlist, float centerX, float centerY, float radius, float aMin, float aMax)
	{
		drawlist->PathArcTo({centerX, centerY}, radius, aMin, aMax);
	}
	inline void PathArcTo(ImDrawList* drawlist, float centerX, float centerY, float radius, float aMin, float aMax, int numSegments)
	{
		drawlist->PathArcTo({centerX, centerY}, radius, aMin, aMax, numSegments);
	}
	inline void PathArcToFast(ImDrawList* drawlist, float centerX, float centerY, float radius, int aMinOf12, int aMaxOf12)
	{
		drawlist->PathArcToFast({centerX, centerY}, radius, aMinOf12, aMaxOf12);
	}
	inline void PathEllipticalArcTo(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, float rotation,
	     float aMin, float aMax)
	{
		drawlist->PathEllipticalArcTo({centerX, centerY}, radiusX, radiusY, rotation, aMin, aMax);
	}
	inline void PathEllipticalArcTo(ImDrawList* drawlist, float centerX, float centerY, float radiusX, float radiusY, float rotation,
	     float aMin, float aMax, int numSegments)
	{
		drawlist->PathEllipticalArcTo({centerX, centerY}, radiusX, radiusY, rotation, aMin, aMax, numSegments);
	}
	inline void PathBezierCubicCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y)
	{
		drawlist->PathBezierCubicCurveTo({p2X, p2Y}, {p3X, p3Y}, {p4X, p4Y});
	}
	inline void PathBezierCubicCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y, int numSegments)
	{
		drawlist->PathBezierCubicCurveTo({p2X, p2Y}, {p3X, p3Y}, {p4X, p4Y}, numSegments);
	}
	inline void PathBezierQuadraticCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y)
	{
		drawlist->PathBezierQuadraticCurveTo({p2X, p2Y}, {p3X, p3Y});
	}
	inline void PathBezierQuadraticCurveTo(ImDrawList* drawlist, float p2X, float p2Y, float p3X, float p3Y, int numSegments)
	{
		drawlist->PathBezierQuadraticCurveTo({p2X, p2Y}, {p3X, p3Y}, numSegments);
	}
	inline void PathRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2Y)
	{
		drawlist->PathRect({p1X, p1Y}, {p2X, p2Y});
	}
	inline void PathRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2Y, float rounding)
	{
		drawlist->PathRect({p1X, p1Y}, {p2X, p2Y}, rounding);
	}
	inline void PathRect(ImDrawList* drawlist, float p1X, float p1Y, float p2X, float p2Y, float rounding, int flags)
	{
		drawlist->PathRect({p1X, p1Y}, {p2X, p2Y}, rounding, flags);
	}
	
	

	inline std::tuple<float, float> GetWindowPos() { const auto vec2{ ImGui::GetWindowPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetWindowSize() { const auto vec2{ ImGui::GetWindowSize() };  return std::make_tuple(vec2.x, vec2.y); }
	inline float GetWindowWidth() { return ImGui::GetWindowWidth(); }
	inline float GetWindowHeight() { return ImGui::GetWindowHeight(); }

	// Window manipulation
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
	inline float GetWindowContentRegionWidth() { return ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x; }

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
	inline void PushTabStop(bool tabStop) { ImGui::PushTabStop(tabStop); }
	inline void PopTabStop() { ImGui::PopTabStop(); }
	inline void PushButtonRepeat(bool repeat) { ImGui::PushButtonRepeat(repeat); }
	inline void PopButtonRepeat() { ImGui::PopButtonRepeat(); }
	inline void PushItemWidth(float itemWidth) { ImGui::PushItemWidth(itemWidth); }
	// Parameters stacks (current window)
	inline void PopItemWidth() { ImGui::PopItemWidth(); }
	inline void SetNextItemWidth(float itemWidth) { ImGui::SetNextItemWidth(itemWidth); }
	inline float CalcItemWidth() { return ImGui::CalcItemWidth(); }
	inline void PushTextWrapPos() { ImGui::PushTextWrapPos(); }
	inline void PushTextWrapPos(float wrapLocalPosX) { ImGui::PushTextWrapPos(wrapLocalPosX); }
	inline void PopTextWrapPos() { ImGui::PopTextWrapPos(); }

	// Style read access
// - Use the ShowStyleEditor() function to interactively see/edit the colors.
	inline ImFont* GetFont() { return ImGui::GetFont(); }
	inline float GetFontSize() { return ImGui::GetFontSize(); }
	inline std::tuple<float, float> GetFontTexUvWhitePixel() { const auto vec2{ ImGui::GetFontTexUvWhitePixel() };	return std::make_tuple(vec2.x, vec2.y); }
#ifdef SOL_IMGUI_USE_COLOR_U32
	inline int GetColorU32(int idx, float alphaMul) { return ImGui::GetColorU32(static_cast<ImGuiCol>(idx), alphaMul); }
	inline int GetColorU32(int col) { return ImGui::GetColorU32(ImU32(col)); }
#endif
	inline std::tuple<float, float, float, float> GetStyleColorVec4(int idx) { const auto col{ ImGui::GetStyleColorVec4(static_cast<ImGuiCol>(idx)) };	return std::make_tuple(col.x, col.y, col.z, col.w); }

	inline int GetColorU32(float colR, float colG, float colB, float colA) { return ImGui::ColorConvertFloat4ToU32({ colR, colG, colB, colA }); }

	// Layout cursor positioning
	inline std::tuple<float, float> GetCursorScreenPos() { const auto vec2{ ImGui::GetCursorScreenPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline void SetCursorScreenPos(float posX, float posY) { ImGui::SetCursorScreenPos({ posX, posY }); }
	inline std::tuple<float, float> GetCursorPos() { const auto vec2{ ImGui::GetCursorPos() };  return std::make_tuple(vec2.x, vec2.y); }
	inline float GetCursorPosX() { return ImGui::GetCursorPosX(); }
	inline float GetCursorPosY() { return ImGui::GetCursorPosY(); }
	inline void SetCursorPos(float localX, float localY) { ImGui::SetCursorPos({ localX, localY }); }
	inline void SetCursorPosX(float localX) { ImGui::SetCursorPosX(localX); }
	inline void SetCursorPosY(float localY) { ImGui::SetCursorPosY(localY); }
	inline std::tuple<float, float> GetCursorStartPos() { const auto vec2{ ImGui::GetCursorStartPos() };  return std::make_tuple(vec2.x, vec2.y); }

	// Other layout functions
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
	inline void SeparatorText(const std::string& text) { ImGui::SeparatorText(text.c_str()); }

	// Widgets: Main
	inline bool Button(const std::string& label)
	{
		return ImGui::Button(label.c_str());
	}
	inline bool Button(const std::string& label, float sizeX, float sizeY) { return ImGui::Button(label.c_str(), { sizeX, sizeY }); }
	inline bool SmallButton(const std::string& label) { return ImGui::SmallButton(label.c_str()); }
	inline bool InvisibleButton(const std::string& stringID, float sizeX, float sizeY) { return ImGui::InvisibleButton(stringID.c_str(), { sizeX, sizeY }); }
	inline bool ArrowButton(const std::string& stringID, int dir) { return ImGui::ArrowButton(stringID.c_str(), static_cast<ImGuiDir>(dir)); }
	inline std::tuple<bool, bool> Checkbox(const std::string& label, bool v)
	{
		bool value{ v };
		bool pressed = ImGui::Checkbox(label.c_str(), &value);

		return std::make_tuple(value, pressed);
	}
	inline bool CheckboxFlags(const std::string& label, int flags, int flags_value) { return ImGui::CheckboxFlags(label.c_str(), &flags, flags_value); }
	inline bool RadioButton(const std::string& label, bool active) { return ImGui::RadioButton(label.c_str(), active); }
	inline std::tuple<int, bool> RadioButton(const std::string& label, int v, int vButton) { bool ret{ ImGui::RadioButton(label.c_str(), &v, vButton) }; return std::make_tuple(v, ret); }
	inline void ProgressBar(float fraction) { ImGui::ProgressBar(fraction); }
	inline void ProgressBar(float fraction, float sizeX, float sizeY) { ImGui::ProgressBar(fraction, { sizeX, sizeY }); }
	inline void ProgressBar(float fraction, float sizeX, float sizeY, const std::string& overlay) { ImGui::ProgressBar(fraction, { sizeX, sizeY }, overlay.c_str()); }
	inline void Bullet() { ImGui::Bullet(); }

	// Widgets: Images
	inline void Image(void* texture, float x, float y)
	{
		ImGui::Image(texture, ImVec2(x, y));
	}
	inline bool ImageButton(const std::string& label, void* texture, float sizeX, float sizeY)
	{
		ImVec2 buttonSize;
		buttonSize.x = sizeX;
		buttonSize.y = sizeY;
		return ImGui::ImageButton(label.c_str(), texture, buttonSize);
	}

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

	// Widgets: Drag Sliders
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

	// Widgets: Regular Sliders
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
	inline bool BeginListBox(const std::string& label, float sizeX, float sizeY) { return ImGui::BeginListBox(label.c_str(), { sizeX, sizeY }); }
	inline void EndListBox() { ImGui::EndListBox(); }
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
	inline bool BeginTooltip() { return ImGui::BeginTooltip(); }
	inline void EndTooltip() { ImGui::EndTooltip(); }
	inline void SetTooltip(const std::string& fmt) { ImGui::SetTooltip(fmt.c_str()); }
	inline void SetTooltipV() { /* TODO: SetTooltipV(...) ==> UNSUPPORTED */ }

	// Tooltips: helpers for showing a tooltip when hovering an item
	inline bool BeginItemTooltip() { return ImGui::BeginItemTooltip(); };
	inline void SetItemTooltip(const std::string& fmt) { return ImGui::SetItemTooltip(fmt.c_str()); };

	// Popups, Modals
	inline bool BeginPopup(const std::string& str_id) { return ImGui::BeginPopup(str_id.c_str()); }
	inline bool BeginPopup(const std::string& str_id, int flags) { return ImGui::BeginPopup(str_id.c_str(), static_cast<ImGuiWindowFlags>(flags)); }
	inline bool BeginPopupModal(const std::string& name) { return ImGui::BeginPopupModal(name.c_str()); }
	inline bool BeginPopupModal(const std::string& name, bool open) { return ImGui::BeginPopupModal(name.c_str(), &open); }
	inline bool BeginPopupModal(const std::string& name, bool open, int flags) { return ImGui::BeginPopupModal(name.c_str(), &open, static_cast<ImGuiWindowFlags>(flags)); }
	inline void EndPopup() { ImGui::EndPopup(); }

	// Popups: open/close functions
	inline void OpenPopup(const std::string& str_id) { ImGui::OpenPopup(str_id.c_str()); }
	inline void OpenPopup(const std::string& str_id, int popup_flags) { ImGui::OpenPopup(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline void OpenPopupOnItemClick() { ImGui::OpenPopupOnItemClick(); };
	inline void OpenPopupOnItemClick(const std::string& str_id) { ImGui::OpenPopupOnItemClick(str_id.c_str()); };
	inline void OpenPopupOnItemClick(const std::string& str_id, int popup_flags) { ImGui::OpenPopupOnItemClick(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); };
	inline void CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }

	// Popups: open+begin combined functions helpers
	inline bool BeginPopupContextItem() { return ImGui::BeginPopupContextItem(); }
	inline bool BeginPopupContextItem(const std::string& str_id) { return ImGui::BeginPopupContextItem(str_id.c_str()); }
	inline bool BeginPopupContextItem(const std::string& str_id, int popup_flags) { return ImGui::BeginPopupContextItem(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline bool BeginPopupContextWindow() { return ImGui::BeginPopupContextWindow(); }
	inline bool BeginPopupContextWindow(const std::string& str_id) { return ImGui::BeginPopupContextWindow(str_id.c_str()); }
	inline bool BeginPopupContextWindow(const std::string& str_id, int popup_flags) { return ImGui::BeginPopupContextWindow(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }
	inline bool BeginPopupContextVoid() { return ImGui::BeginPopupContextVoid(); }
	inline bool BeginPopupContextVoid(const std::string& str_id) { return ImGui::BeginPopupContextVoid(str_id.c_str()); }
	inline bool BeginPopupContextVoid(const std::string& str_id, int popup_flags) { return ImGui::BeginPopupContextVoid(str_id.c_str(), static_cast<ImGuiPopupFlags>(popup_flags)); }

	// Popups: query functions
	inline bool IsPopupOpen(const std::string& str_id) { return ImGui::IsPopupOpen(str_id.c_str()); }
	inline bool IsPopupOpen(const std::string& str_id, int popup_flags) { return ImGui::IsPopupOpen(str_id.c_str(), popup_flags); }

	// Tables
	inline bool BeginTable(const std::string& str_id, int column) { return ImGui::BeginTable(str_id.c_str(), column); }
	inline bool BeginTable(const std::string& str_id, int column, int flags) { return ImGui::BeginTable(str_id.c_str(), column, flags); }
	inline bool BeginTable(const std::string& str_id, int column, int flags, float sizeX, float sizeY) { return ImGui::BeginTable(str_id.c_str(), column, flags, {sizeX, sizeY}); }
	inline bool BeginTable(const std::string& str_id, int column, int flags, float sizeX, float sizeY, float inner_width) { return ImGui::BeginTable(str_id.c_str(), column, flags, { sizeX, sizeY }, inner_width); }
	inline void EndTable() { ImGui::EndTable(); }
	inline void TableNextRow() { ImGui::TableNextRow(); }
	inline void TableNextRow(int row_flags) { ImGui::TableNextRow(row_flags); }
	inline void TableNextRow(int row_flags, float min_row_height) { ImGui::TableNextRow(row_flags, min_row_height); }
	inline void TableNextColumn() { ImGui::TableNextColumn(); }
	inline void TableSetColumnIndex(int column_n) { ImGui::TableSetColumnIndex(column_n); }

	// Tables: Headers & Columns declaration
	inline void TableSetupColumn(const std::string& str_id) { ImGui::TableSetupColumn(str_id.c_str()); }
	inline void TableSetupColumn(const std::string& str_id, int flags) { ImGui::TableSetupColumn(str_id.c_str(), flags); }
	inline void TableSetupColumn(const std::string& str_id, int flags, float init_width_or_weight) { ImGui::TableSetupColumn(str_id.c_str(), flags, init_width_or_weight); }
	inline void TableSetupColumn(const std::string& str_id, int flags, float init_width_or_weight, int id) { ImGui::TableSetupColumn(str_id.c_str(), flags, init_width_or_weight, id); }
	inline void TableSetupScrollFreeze(int cols, int rows) { ImGui::TableSetupScrollFreeze(cols, rows); }
	inline void TableHeader(const std::string& str_id) { ImGui::TableHeader(str_id.c_str()); }
	inline void TableHeadersRow() { ImGui::TableHeadersRow(); }
	inline void TableAngledHeadersRow() { ImGui::TableAngledHeadersRow(); }

	// Tables: Sorting & Miscellaneous functions
	inline const ImGuiTableColumnSortSpecs* GetSortSpecs(ImGuiTableSortSpecs* tablesort, int index){return &tablesort->Specs[index];}
	inline ImGuiTableSortSpecs* TableGetSortSpecs(){ return ImGui::TableGetSortSpecs(); }
	inline int TableGetColumnCount(){ return ImGui::TableGetColumnCount(); }
	inline int TableGetColumnIndex(){ return ImGui::TableGetColumnIndex(); }
	inline int TableGetRowIndex(){ return ImGui::TableGetRowIndex(); }
	inline const char* TableGetColumnName(){ return ImGui::TableGetColumnName(); }
	inline const char* TableGetColumnName(int column){ return ImGui::TableGetColumnName(column); }
	inline int TableGetColumnFlags(){ return ImGui::TableGetColumnFlags(); }
	inline int TableGetColumnFlags(int column){ return ImGui::TableGetColumnFlags(column); }
	inline void TableSetColumnEnabled(int column, bool v){ return ImGui::TableSetColumnEnabled(column, v); }



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

	// Overlapping mode
	inline void SetNextItemAllowOverlap() { ImGui::SetNextItemAllowOverlap(); }

	// Item/Widgets Utilities and Query Functions
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
	inline int GetItemID() { return ImGui::GetItemID(); }
	inline std::tuple<float, float> GetItemRectMin() { const auto vec2{ ImGui::GetItemRectMin() }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetItemRectMax() { const auto vec2{ ImGui::GetItemRectMax() }; return std::make_tuple(vec2.x, vec2.y); }
	inline std::tuple<float, float> GetItemRectSize() { const auto vec2{ ImGui::GetItemRectSize() }; return std::make_tuple(vec2.x, vec2.y); }

	// Miscellaneous Utilities
	inline bool IsRectVisible(float sizeX, float sizeY) { return ImGui::IsRectVisible({ sizeX, sizeY }); }
	inline bool IsRectVisible(float minX, float minY, float maxX, float maxY) { return ImGui::IsRectVisible({ minX, minY }, { maxX, maxY }); }
	inline double GetTime() { return ImGui::GetTime(); }
	inline int GetFrameCount() { return ImGui::GetFrameCount(); }
	/* TODO: GetBackgroundDrawList(), GetForeGroundDrawList(), GetDrawListSharedData() ==> UNSUPPORTED */
	inline std::string GetStyleColorName(int idx) { return std::string(ImGui::GetStyleColorName(static_cast<ImGuiCol>(idx))); }
	/* TODO: SetStateStorage(), GetStateStorage(), CalcListClipping() ==> UNSUPPORTED */

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

	// Inputs Utilities: Keyboard/Mouse/Gamepad
	inline bool IsKeyDown(int user_key_index) { return ImGui::IsKeyDown(static_cast<ImGuiKey>(user_key_index)); }
	inline bool IsKeyPressed(int user_key_index) { return ImGui::IsKeyPressed(static_cast<ImGuiKey>(user_key_index));}
	inline bool IsKeyPressed(int user_key_index, bool repeat) { return ImGui::IsKeyPressed(static_cast<ImGuiKey>(user_key_index), repeat); }
	inline bool IsKeyReleased(int user_key_index) { return ImGui::IsKeyReleased(static_cast<ImGuiKey>(user_key_index)); }
	inline int GetKeyPressedAmount(int key_index, float repeat_delay, float rate) { return ImGui::GetKeyPressedAmount(static_cast<ImGuiKey>(key_index), repeat_delay, rate); }
	inline void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard) { return ImGui::SetNextFrameWantCaptureKeyboard(want_capture_keyboard); }

	// Inputs Utilities: Mouse specific
	inline bool IsMouseDown(int button) { return ImGui::IsMouseDown(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseClicked(int button) { return ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseClicked(int button, bool repeat) { return ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(button), repeat); }
	inline bool IsMouseReleased(int button) { return ImGui::IsMouseReleased(static_cast<ImGuiMouseButton>(button)); }
	inline bool IsMouseDoubleClicked(int button) { return ImGui::IsMouseDoubleClicked(static_cast<ImGuiMouseButton>(button)); }
	inline int GetMouseClickedCount(int button) { return ImGui::GetMouseClickedCount(button); }
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
	inline void SetNextFrameWantCaptureMouse(bool want_capture_mouse_value) { ImGui::SetNextFrameWantCaptureMouse(want_capture_mouse_value); }
	inline ImFont* AddFontFromFileTTF(ImFontAtlas* fontAtlas, const char* filename, float size_pixels)
	{
		return fontAtlas->AddFontFromFileTTF(filename, size_pixels);
	}

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
		@tfield int NoLabel ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
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

	void Init(sol::state& lua)
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
		@tfield PushTabStop PushTabStop
		@tfield PopTabStop PopTabStop
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
		@tfield SeparatorText SeparatorText
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
		@tfield ColorButton ColorButton
		@tfield SetColorEditOptions SetColorEditOptions
		@tfield TreeNode TreeNode
		@tfield TreeNodeEx TreeNodeEx
		@tfield TreePush TreePush
		@tfield TreePop TreePop
		@tfield GetTreeNodeToLabelSpacing GetTreeNodeToLabelSpacing
		@tfield CollapsingHeader CollapsingHeader
		@tfield SetNextItemOpen SetNextItemOpen
		@tfield Selectable Selectable
		@tfield BeginListBox BeginListBox
		@tfield EndListBox EndListBox
		@tfield ListBox ListBox
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
		@tfield BeginItemTooltip BeginItemTooltip
		@tfield SetItemTooltip SetItemTooltip
		@tfield BeginPopup BeginPopup
		@tfield BeginPopupModal BeginPopupModal
		@tfield EndPopup EndPopup
		@tfield OpenPopup OpenPopup
		@tfield OpenPopupOnItemClick OpenPopupOnItemClick
		@tfield CloseCurrentPopup CloseCurrentPopup
		@tfield BeginPopupContextItem BeginPopupContextItem
		@tfield BeginPopupContextWindow BeginPopupContextWindow
		@tfield BeginPopupContextVoid BeginPopupContextVoid
		@tfield IsPopupOpen IsPopupOpen
		@tfield BeginTable BeginTable
		@tfield EndTable EndTable
		@tfield TableNextRow TableNextRow
		@tfield TableNextColumn TableNextColumn
		@tfield TableSetColumnIndex TableSetColumnIndex
		@tfield TableSetupColumn TableSetupColumn
		@tfield TableSetupScrollFreeze TableSetupScrollFreeze
		@tfield TableHeader TableHeader
		@tfield TableGetSortSpecs TableGetSortSpecs
		@tfield TableGetColumnCount TableGetColumnCount
		@tfield TableGetColumnIndex TableGetColumnIndex
		@tfield TableGetRowIndex TableGetRowIndex
		@tfield TableGetColumnName TableGetColumnName
		@tfield TableGetColumnFlags TableGetColumnFlags
		@tfield TableSetColumnEnabled TableSetColumnEnabled
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
		@tfield SetNextItemAllowOverlap SetNextItemAllowOverlap
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
		@tfield GetItemID GetItemID
		@tfield GetItemRectMin GetItemRectMin
		@tfield GetItemRectMax GetItemRectMax
		@tfield GetItemRectSize GetItemRectSize
		@tfield IsRectVisible IsRectVisible
		@tfield GetTime GetTime
		@tfield GetFrameCount GetFrameCount
		@tfield GetStyleColorName GetStyleColorName
		@tfield CalcTextSize CalcTextSize
		@tfield ColorConvertRGBtoHSV ColorConvertRGBtoHSV
		@tfield ColorConvertHSVtoRGB ColorConvertHSVtoRGB
		@tfield IsKeyDown IsKeyDown
		@tfield IsKeyPressed IsKeyPressed
		@tfield IsKeyReleased IsKeyReleased
		@tfield GetKeyPressedAmount GetKeyPressedAmount
		@tfield SetNextFrameWantCaptureKeyboard SetNextFrameWantCaptureKeyboard
		@tfield IsMouseDown IsMouseDown
		@tfield IsMouseClicked IsMouseClicked
		@tfield IsMouseReleased IsMouseReleased
		@tfield IsMouseDoubleClicked IsMouseDoubleClicked
		@tfield GetMouseClickedCount GetMouseClickedCount
		@tfield IsMouseHoveringRect IsMouseHoveringRect
		@tfield IsAnyMouseDown IsAnyMouseDown
		@tfield GetMousePos GetMousePos
		@tfield GetMousePosOnOpeningCurrentPopup GetMousePosOnOpeningCurrentPopup
		@tfield IsMouseDragging IsMouseDragging
		@tfield GetMouseDragDelta GetMouseDragDelta
		@tfield ResetMouseDragDelta ResetMouseDragDelta
		@tfield GetMouseCursor GetMouseCursor
		@tfield SetMouseCursor SetMouseCursor
		@tfield SetNextFrameWantCaptureMouse SetNextFrameWantCaptureMouse
		@tfield GetClipboardText GetClipboardText
		@tfield SetClipboardText SetClipboardText
		@tfield GetStyle GetStyle
		@tfield GetIO GetIO
		@tfield GetWindowDrawList GetWindowDrawList
		@tfield GetColorU32 GetColorU32
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
		@tparam int childFlags optional
		@tparam int windowFlags optional
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
			sol::resolve<bool(const std::string&, float, float, int)>(BeginChild),
			sol::resolve<bool(const std::string&, float, float, int, int)>(BeginChild)
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
		Is current window hovered (and typically: not blocked by a popup/modal)?. IMPORTANT: If you are trying to check whether your mouse should be dispatched to Dear ImGui or to your underlying app, you should not use this function! Use the 'io.WantCaptureMouse' boolean for that!
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
		Get current window position in screen space  (note: it is unlikely you need to use this. Consider using current layout pos instead, GetCursorScreenPos()).
		@function ImGui.GetWindowPos
		@treturn float posX
		@treturn float posY
		@usage
			ImGui.GetWindowPos();
		*/
		ImGui.set_function("GetWindowPos", GetWindowPos);
		/***
		Get current window size. (note: it is unlikely you need to use this. Consider using GetCursorScreenPos() and e.g. GetContentRegionAvail() instead)
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
		Retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in
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

		
		/***
		Get Color U32.
		@function ImGui.GetColorU32
		@tparam float R
		@tparam float G
		@tparam float B
		@tparam float A
		@treturn int color
		@usage
			ImGui.GetColorU32();
		*/
		ImGui.set_function("GetColorU32", sol::overload(
			sol::resolve<int(float, float, float, float)>(GetColorU32)
		));
		
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
		@tparam float wrapLocalPosX optional
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
		@function ImGui.PushTabStop
		@tparam bool tabStop
		@usage
			ImGui.PushTabStop(false);
		*/
		ImGui.set_function("PushTabStop", PushTabStop);
		/***
		Pop Tab Stop.
		@function ImGui.PopTabStop
		@usage
			ImGui.PopTabStop();
		*/
		ImGui.set_function("PopTabStop", PopTabStop);
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
		/***
		Currently: formatted text with an horizontal line
		@function ImGui.SeparatorText
		@tparam string text
		@usage
			ImGui.SeparatorText("text");
		*/
		ImGui.set_function("SeparatorText", SeparatorText);
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
		Checkbox.
		@function ImGui.CheckboxFlags
		@tparam string label
		@tparam int flags
		@treturn int flagsValue
		@treturn bool result
		@usage
			ImGui.CheckboxFlags();
		*/
		ImGui.set_function("CheckboxFlags", CheckboxFlags);
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
		@tparam string label
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
		@tparam string hint
		@tparam string text
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
		/***
		Display a color square/button, hover for details, return true when pressed.
		@function ImGui.ColorButton
		@tparam string label
		@tparam float[4] input Maximum: 4
		@tparam int colorEditFlags optional
		@tparam float sizeX optional
		@tparam float sizeY optional
		@treturn bool result
		@usage
			ImGui.ColorButton();
		*/
		ImGui.set_function("ColorButton", sol::overload(
			sol::resolve<bool(const std::string&, const sol::table&)>(ColorButton),
			sol::resolve<bool(const std::string&, const sol::table&, int colorEditFlags)>(ColorButton),
			sol::resolve<bool(const std::string&, const sol::table&, int colorEditFlags, float sizeX, float sizeY)>(ColorButton)));
		/***
		Initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.
		@function ImGui.SetColorEditOptions
		@tparam int colorEditFlags
		@usage
			ImGui.SetColorEditOptions();
		*/
		ImGui.set_function("SetColorEditOptions", SetColorEditOptions);
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
		Open a framed scrolling region. This is essentially a thin wrapper to using BeginChild/EndChild with the ImGuiChildFlags_FrameStyle flag for stylistic changes + displaying a label. You can submit contents and manage your selection state however you want it, by creating e.g. Selectable() or any other items. 
		@function ImGui.BeginListBox
		@tparam string label
		@tparam float sizeX
		@tparam float sizeY
		@treturn bool result
		@usage
			ImGui.BeginListBox();
		*/
		ImGui.set_function("BeginListBox", BeginListBox);
		/***
		Only call EndListBox() if BeginListBox() returned true!
		@function ImGui.EndListBox
		@usage
			ImGui.EndListBox();
		*/
		ImGui.set_function("EndListBox", EndListBox);
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
		@treturn bool result
		@usage
			ImGui.BeginTooltip();
		*/
		ImGui.set_function("BeginTooltip", BeginTooltip);
		/***
		Only call EndTooltip() if BeginTooltip()/BeginItemTooltip() returns true!
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
		/***
		Begin/append a tooltip window if preceding item was hovered. BeginItemTooltip() is a shortcut for the 'if (IsItemHovered(ImGuiHoveredFlags_ForTooltip) && BeginTooltip())' idiom. Where 'ImGuiHoveredFlags_ForTooltip' itself is a shortcut to use 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' depending on active input type. For mouse it defaults to 'ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayShort'.
		@function ImGui.BeginItemTooltip
		@treturn bool result
		@usage
			ImGui.BeginItemTooltip();
		*/
		ImGui.set_function("BeginItemTooltip", BeginItemTooltip);
		/***
		Set a text-only tooltip if preceeding item was hovered. override any previous call to SetTooltip(). SetItemTooltip() is a shortcut for the 'if (IsItemHovered(ImGuiHoveredFlags_ForTooltip)) { SetTooltip(...); }' idiom.
		@function ImGui.SetItemTooltip
		@tparam string text
		@usage
			ImGui.SetItemTooltip();
		*/
		ImGui.set_function("SetItemTooltip", SetItemTooltip);
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
		@usage
			ImGui.OpenPopup();
		*/
		ImGui.set_function("OpenPopup", sol::overload(
			sol::resolve<void(const std::string&)>(OpenPopup),
			sol::resolve<void(const std::string&, int)>(OpenPopup)
		));
		/***
		Helper to open popup when clicked on last item. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)
		@function ImGui.OpenPopupOnItemClick
		@tparam string label optional
		@tparam int popupFlags optional
		@usage
			ImGui.OpenPopupOnItemClick();
		*/
		ImGui.set_function("OpenPopupOnItemClick", sol::overload(
			sol::resolve<void()>(OpenPopupOnItemClick),
			sol::resolve<void(const std::string&)>(OpenPopupOnItemClick),
			sol::resolve<void(const std::string&, int)>(OpenPopupOnItemClick)
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
#pragma region Tables

		/***
		Full-featured replacement for old Columns API. See ImGuiTableFlags_ and ImGuiTableColumnFlags_ enums for a description of available flags. In most situations you can use TableNextRow() + TableSetColumnIndex(N) to start appending into a column. If you are using tables as a sort of grid, where every column is holding the same type of contents, you may prefer using TableNextColumn() instead of TableNextRow() + TableSetColumnIndex(). TableNextColumn() will automatically wrap-around into the next row if needed. IMPORTANT: Comparatively to the old Columns() API, we need to call TableNextColumn() for the first column!
		@function ImGui.BeginTable
		@tparam string str_id
		@tparam int column
		@tparam int flags optional
		@tparam float sizeX optional
		@tparam float sizeY optional
		@tparam float innerWidth optional
		@usage
			ImGui.BeginTable();
		*/
		ImGui.set_function("BeginTable", sol::overload(
			sol::resolve<bool(const std::string&, int)>(BeginTable),
			sol::resolve<bool(const std::string&, int, int)>(BeginTable),
			sol::resolve<bool(const std::string&, int, int, float, float)>(BeginTable),
			sol::resolve<bool(const std::string&, int, int, float, float, float)>(BeginTable)));
		/***
		End the ImGui table. Only call EndTable() if BeginTable() returns true!
		@function ImGui.EndTable
		@usage
			ImGui.EndTable();
		*/
		ImGui.set_function("EndTable", EndTable);
		/***
		Append into the first cell of a new row.
		@function ImGui.TableNextRow
		@tparam int flags optional
		@tparam float minRowHeight optional
		@usage
			ImGui.TableNextRow();
		*/
		ImGui.set_function("TableNextRow", sol::overload(
			sol::resolve<void(int)>(TableNextRow),
			sol::resolve<void(int, float)>(TableNextRow)));
		/***
		Append into the next column (or first column of next row if currently in last column). Return true when column is visible.
		@function ImGui.TableNextColumn
		@treturn bool result
		@usage
			ImGui.TableNextColumn();
		*/
		ImGui.set_function("TableNextColumn", TableNextColumn);
		/***
		Append into the specified column. Return true when column is visible.
		@function ImGui.TableSetColumnIndex
		@tparam int index
		@treturn bool result
		@usage
			ImGui.TableSetColumnIndex();
		*/
		ImGui.set_function("TableSetColumnIndex", TableSetColumnIndex);
		/***
		Use TableSetupColumn() to specify label, resizing policy, default width/weight, id, various other flags etc. The context menu can also be made available in columns body using ImGuiTableFlags_ContextMenuInBody.
		@function ImGui.TableSetupColumn
		@tparam string label
		@tparam int flags optional
		@tparam float initWidthOrWeight optional
		@tparam int userID optional
		@usage
			ImGui.TableSetupColumn();
		*/
		ImGui.set_function("TableSetupColumn", sol::overload(
			sol::resolve<void(const std::string&)>(TableSetupColumn),
			sol::resolve<void(const std::string&, int)>(TableSetupColumn),
			sol::resolve<void(const std::string&, int, float)>(TableSetupColumn),
			sol::resolve<void(const std::string&, int, float, int)>(TableSetupColumn)
		));
		/***
		Lock columns/rows so they stay visible when scrolled.
		@function ImGui.TableSetupScrollFreeze
		@tparam int cols
		@tparam int rows
		@usage
			ImGui.TableSetupScrollFreeze();
		*/
		ImGui.set_function("TableSetupScrollFreeze", TableSetupScrollFreeze);
		/***
		Submit a row with headers cells based on data provided to TableSetupColumn() + submit context menu. Use TableHeadersRow() to create a header row and automatically submit a TableHeader() for each column.
		@function ImGui.TableHeader
		@tparam string label
		@usage
			ImGui.TableHeader();
		*/
		ImGui.set_function("TableHeader", TableHeader);
		/***
		Submit a row with headers cells based on data provided to TableSetupColumn() + submit context menu. Use TableHeadersRow() to create a header row and automatically submit a TableHeader() for each column.
		@function ImGui.TableHeadersRow
		@usage
			ImGui.TableHeadersRow();
		*/
		ImGui.set_function("TableHeadersRow",TableHeadersRow);
		/***
		Submit a row with angled headers for every column with the ImGuiTableColumnFlags_AngledHeader flag. MUST BE FIRST ROW.
		@function ImGui.TableAngledHeadersRow
		@usage
			ImGui.TableAngledHeadersRow();
		*/
		ImGui.set_function("TableAngledHeadersRow",TableAngledHeadersRow);
		/***
		Get latest sort specs for the table (NULL if not sorting).  Lifetime: don't hold on this pointer over multiple frames or past any subsequent call to BeginTable()
		@function ImGui.TableGetSortSpecs
		@treturn ImGuiTableSortSpecs specs
		@usage
			ImGui.TableGetSortSpecs();
		*/
		ImGui.set_function("TableGetSortSpecs",TableGetSortSpecs);
		/***
		Return number of columns (value passed to BeginTable)
		@function ImGui.TableGetColumnCount
		@treturn int count
		@usage
			ImGui.TableGetColumnCount();
		*/
		ImGui.set_function("TableGetColumnCount",TableGetColumnCount);
		/***
		Return current column index.
		@function ImGui.TableGetColumnIndex
		@treturn int index
		@usage
			ImGui.TableGetColumnIndex();
		*/
		ImGui.set_function("TableGetColumnIndex",TableGetColumnIndex);
		/***
		Return current row index.
		@function ImGui.TableGetRowIndex
		@treturn int index
		@usage
			ImGui.TableGetRowIndex();
		*/
		ImGui.set_function("TableGetRowIndex",TableGetRowIndex);
		/***
		Return "" if column didn't have a name declared by TableSetupColumn(). Pass -1 to use current column.
		@function ImGui.TableGetColumnName
		@tparam int index optional
		@treturn string name
		@usage
			ImGui.TableGetColumnName();
		*/
		ImGui.set_function("TableGetColumnName", sol::overload(
			sol::resolve<const char*()>(TableGetColumnName),
			sol::resolve<const char*(int)>(TableGetColumnName)));
		/***
		Return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass -1 to use current column.
		@function ImGui.TableGetColumnFlags
		@tparam int index optional
		@treturn int flags
		@usage
			ImGui.TableGetColumnFlags();
		*/
		ImGui.set_function("TableGetColumnFlags", sol::overload(
			sol::resolve<int()>(TableGetColumnFlags),
			sol::resolve<int(int)>(TableGetColumnFlags)));
		/***
		Change user accessible enabled/disabled state of a column. Set to false to hide the column. User can use the context menu to change this themselves (right-click in headers, or right-click in columns body with ImGuiTableFlags_ContextMenuInBody)
		@function ImGui.TableSetColumnEnabled
		@tparam int index
		@tparam bool value
		@usage
			ImGui.TableSetColumnEnabled();
		*/
		ImGui.set_function("TableSetColumnEnabled", TableSetColumnEnabled);

#pragma endregion Tables

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
			sol::resolve<void(int)>(SetKeyboardFocusHere)));
		/***
		Allow next item to be overlapped by a subsequent item. Useful with invisible buttons, selectable, treenode covering an area where subsequent items may need to be added. Note that both Selectable() and TreeNode() have dedicated flags doing this.
		@function ImGui.SetNextItemAllowOverlap
		@usage
			ImGui.SetNextItemAllowOverlap();
		*/
		ImGui.set_function("SetNextItemAllowOverlap", SetNextItemAllowOverlap);
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
		Get ID of last item (~~ often same ImGui::GetID(label) beforehand)
		@function ImGui.GetItemID
		@treturn int id
		@usage
			ImGui.GetItemID();
		*/
		ImGui.set_function("GetItemID", GetItemID);
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
		Is key down.
		@function ImGui.IsKeyDown
		@tparam int key
		@treturn bool result
		@usage
			ImGui.IsKeyDown();
		*/
		ImGui.set_function("IsKeyDown", IsKeyDown);
		/***
		Is key pressed.
		@function ImGui.IsKeyPressed
		@tparam int key
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
		@tparam int key
		@treturn bool result
		@usage
			ImGui.IsKeyReleased();
		*/
		ImGui.set_function("IsKeyReleased", IsKeyReleased);
		/***
		Uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
		@function ImGui.GetKeyPressedAmount
		@tparam int key
		@tparam float repeatDelay
		@tparam float rate
		@treturn int amount
		@usage
			ImGui.GetKeyPressedAmount();
		*/
		ImGui.set_function("GetKeyPressedAmount", GetKeyPressedAmount);
		/***
		Override io.WantCaptureKeyboard flag next frame (said flag is left for your application to handle, typically when true it instructs your app to ignore inputs). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard"; after the next NewFrame() call.
		@function ImGui.SetNextFrameWantCaptureKeyboard
		@tparam bool wantCaptureKeyboard
		@usage
			ImGui.SetNextFrameWantCaptureKeyboard();
		*/
		ImGui.set_function("SetNextFrameWantCaptureKeyboard", SetNextFrameWantCaptureKeyboard);
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
		Return the number of successive mouse-clicks at the time where a click happen (otherwise 0).
		@function ImGui.GetMouseClickedCount
		@tparam int button use MouseButton enum
		@treturn int count
		@usage
			ImGui.GetMouseClickedCount();
		*/
		ImGui.set_function("GetMouseClickedCount", GetMouseClickedCount);
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
		Override io.WantCaptureMouse flag next frame (said flag is left for your application to handle, typical when true it instucts your app to ignore inputs). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse;" after the next NewFrame() call.
		@function ImGui.SetNextFrameWantCaptureMouse
		@tparam bool wantCaptureMouse
		@usage
			ImGui.SetNextFrameWantCaptureMouse();
		*/
		ImGui.set_function("SetNextFrameWantCaptureMouse", SetNextFrameWantCaptureMouse);
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
		
		/***
		Get window draw list.
		@function ImGui.GetWindowDrawList
		@treturn ImDrawList drawList
		@usage
			ImGui.GetWindowDrawList();
		*/
		ImGui.set_function("GetWindowDrawList", GetWindowDrawList);

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

		@tfield float Alpha Global alpha applies to everything in Dear ImGui.
		@tfield float DisabledAlpha Additional alpha multiplier applied by BeginDisabled(). Multiply over current value of Alpha.
		@tfield ImVec2 WindowPadding Padding within a window.
		@tfield float WindowRounding Radius of window corners rounding. Set to 0.0f to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.
		@tfield float WindowBorderSize Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		@tfield ImVec2 WindowMinSize Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().
		@tfield ImVec2 WindowTitleAlign Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
		@tfield int WindowMenuButtonPosition Aside of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir.Left.
		@tfield float ChildRounding Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
		@tfield float ChildBorderSize Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		@tfield float PopupRounding Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
		@tfield float PopupBorderSize Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		@tfield ImVec2 FramePadding Padding within a framed rectangle (used by most widgets).
		@tfield float FrameRounding Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
		@tfield float FrameBorderSize Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		@tfield ImVec2 ItemSpacing Horizontal and vertical spacing between widgets/lines.
		@tfield ImVec2 ItemInnerSpacing Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
		@tfield ImVec2 CellPadding Padding within a table cell. CellPadding.y may be altered between different rows.
		@tfield ImVec2 TouchExtraPadding Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
		@tfield float IndentSpacing Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
		@tfield float ColumnsMinSpacing Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
		@tfield float ScrollbarSize Width of the vertical scrollbar, Height of the horizontal scrollbar.
		@tfield float ScrollbarRounding Radius of grab corners for scrollbar.
		@tfield float GrabMinSize Minimum width/height of a grab box for slider/scrollbar.
		@tfield float GrabRounding Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
		@tfield float LogSliderDeadzone The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.
		@tfield float TabRounding Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
		@tfield float TabBorderSize Thickness of border around tabs.
		@tfield float TabMinWidthForCloseButton Minimum width for close button to appear on an unselected tab when hovered. Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected.
		@tfield float TabBarBorderSize Thickness of tab-bar separator, which takes on the tab active color to denote focus.
		@tfield float TableAngledHeadersAngle Angle of angled headers (supported values range from -50.0f degrees to +50.0f degrees).
		@tfield int ColorButtonPosition Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir.Right.
		@tfield ImVec2 ButtonTextAlign Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
		@tfield ImVec2 SelectableTextAlign Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.
		@tfield float SeparatorTextBorderSize Thickkness of border in SeparatorText()
		@tfield ImVec2 SeparatorTextAlign Alignment of text within the separator. Defaults to (0.0f, 0.5f) (left aligned, center).
		@tfield ImVec2 SeparatorTextPadding Horizontal offset of text from each edge of the separator + spacing on other axis. Generally small values. .y is recommended to be == FramePadding.y.
		@tfield ImVec2 DisplayWindowPadding Window position are clamped to be visible within the display area or monitors by at least this amount. Only applies to regular windows.
		@tfield ImVec2 DisplaySafeAreaPadding If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
		@tfield float MouseCursorScale Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
		@tfield bool AntiAliasedLines Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
		@tfield bool AntiAliasedLinesUseTex Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to ImDrawList).
		@tfield bool AntiAliasedFill Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
		@tfield float CurveTessellationTol Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
		@tfield float CircleTessellationMaxError Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
		@tfield ImVec4[52] Colors Maximum: 52
		@tfield float HoverStationaryDelay Delay for IsItemHovered(ImGuiHoveredFlags_Stationary). Time required to consider mouse stationary.
		@tfield float HoverDelayShort Delay for IsItemHovered(ImGuiHoveredFlags_DelayShort). Usually used along with HoverStationaryDelay.
		@tfield float HoverDelayNormal Delay for IsItemHovered(ImGuiHoveredFlags_DelayNormal). 
		@tfield int HoverFlagsForTooltipMouse Default flags when using IsItemHovered(ImGuiHoveredFlags_ForTooltip) or BeginItemTooltip()/SetItemTooltip() while using mouse.
		@tfield int HoverFlagsForTooltipNav Default flags when using IsItemHovered(ImGuiHoveredFlags_ForTooltip) or BeginItemTooltip()/SetItemTooltip() while using keyboard/gamepad.


		@table ImGuiStyle
		*/
		sol::usertype<ImGuiStyle> imguiStyle = lua.new_usertype<ImGuiStyle>("ImGuiStyle");
		imguiStyle.set("Alpha", &ImGuiStyle::Alpha);
		imguiStyle.set("DisabledAlpha", &ImGuiStyle::DisabledAlpha);
		imguiStyle.set("WindowPadding", &ImGuiStyle::WindowPadding);
		imguiStyle.set("WindowRounding", &ImGuiStyle::WindowRounding);
		imguiStyle.set("WindowBorderSize", &ImGuiStyle::WindowBorderSize);
		imguiStyle.set("WindowMinSize", &ImGuiStyle::WindowMinSize);
		imguiStyle.set("WindowTitleAlign", &ImGuiStyle::WindowTitleAlign);
		imguiStyle.set("WindowMenuButtonPosition", &ImGuiStyle::WindowMenuButtonPosition);
		imguiStyle.set("ChildRounding", &ImGuiStyle::ChildRounding);
		imguiStyle.set("ChildBorderSize", &ImGuiStyle::ChildBorderSize);
		imguiStyle.set("PopupRounding", &ImGuiStyle::PopupRounding);
		imguiStyle.set("PopupBorderSize", &ImGuiStyle::PopupBorderSize);
		imguiStyle.set("FramePadding", &ImGuiStyle::FramePadding);
		imguiStyle.set("FrameRounding", &ImGuiStyle::FrameRounding);
		imguiStyle.set("FrameBorderSize", &ImGuiStyle::FrameBorderSize);
		imguiStyle.set("ItemSpacing", &ImGuiStyle::ItemSpacing);
		imguiStyle.set("ItemInnerSpacing", &ImGuiStyle::ItemInnerSpacing);
		imguiStyle.set("CellPadding", &ImGuiStyle::CellPadding);
		imguiStyle.set("TouchExtraPadding", &ImGuiStyle::TouchExtraPadding);
		imguiStyle.set("IndentSpacing", &ImGuiStyle::IndentSpacing);
		imguiStyle.set("ColumnsMinSpacing", &ImGuiStyle::ColumnsMinSpacing);
		imguiStyle.set("ScrollbarSize", &ImGuiStyle::ScrollbarSize);
		imguiStyle.set("ScrollbarRounding", &ImGuiStyle::ScrollbarRounding);
		imguiStyle.set("GrabMinSize", &ImGuiStyle::GrabMinSize);
		imguiStyle.set("GrabRounding", &ImGuiStyle::GrabRounding);
		imguiStyle.set("LogSliderDeadzone", &ImGuiStyle::LogSliderDeadzone);
		imguiStyle.set("TabRounding", &ImGuiStyle::TabRounding);
		imguiStyle.set("TabBorderSize", &ImGuiStyle::TabBorderSize);
		imguiStyle.set("TabMinWidthForCloseButton", &ImGuiStyle::TabMinWidthForCloseButton);
		imguiStyle.set("TabBarBorderSize", &ImGuiStyle::TabBarBorderSize);
		imguiStyle.set("TableAngledHeadersAngle", &ImGuiStyle::TableAngledHeadersAngle);
		imguiStyle.set("ColorButtonPosition", &ImGuiStyle::ColorButtonPosition);
		imguiStyle.set("ButtonTextAlign", &ImGuiStyle::ButtonTextAlign);
		imguiStyle.set("SelectableTextAlign", &ImGuiStyle::SelectableTextAlign);
		imguiStyle.set("SeparatorTextBorderSize", &ImGuiStyle::SeparatorTextBorderSize);
		imguiStyle.set("SeparatorTextAlign", &ImGuiStyle::SeparatorTextAlign);
		imguiStyle.set("SeparatorTextPadding", &ImGuiStyle::SeparatorTextPadding);
		imguiStyle.set("DisplayWindowPadding", &ImGuiStyle::DisplayWindowPadding);
		imguiStyle.set("DisplaySafeAreaPadding", &ImGuiStyle::DisplaySafeAreaPadding);
		imguiStyle.set("MouseCursorScale", &ImGuiStyle::MouseCursorScale);
		imguiStyle.set("AntiAliasedLines", &ImGuiStyle::AntiAliasedLines);
		imguiStyle.set("AntiAliasedLinesUseTex", &ImGuiStyle::AntiAliasedLinesUseTex);
		imguiStyle.set("AntiAliasedFill", &ImGuiStyle::AntiAliasedFill);
		imguiStyle.set("CurveTessellationTol", &ImGuiStyle::CurveTessellationTol);
		imguiStyle.set("CircleTessellationMaxError", &ImGuiStyle::CircleTessellationMaxError);
		imguiStyle.set("Colors", sol::property([](ImGuiStyle& self) { return std::ref(self.Colors); }));
		imguiStyle.set("HoverStationaryDelay", &ImGuiStyle::HoverStationaryDelay);
		imguiStyle.set("HoverDelayShort", &ImGuiStyle::HoverDelayShort);
		imguiStyle.set("HoverDelayNormal", &ImGuiStyle::HoverDelayNormal);
		imguiStyle.set("HoverFlagsForTooltipMouse", &ImGuiStyle::HoverFlagsForTooltipMouse);
		imguiStyle.set("HoverFlagsForTooltipNav", &ImGuiStyle::HoverFlagsForTooltipNav);

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
		@function FontAtlas:AddFontFromFileTTF
		@tparam string filename
		@tparam float sizePixels
		@treturn ImFont font
		@usage
			FontAtlas:AddFontFromFileTTF();
		*/
		FontAtlas.set_function("AddFontFromFileTTF", &AddFontFromFileTTF);

		///ImDrawList
		//@section ImDrawList
		/***
		Basic ImDrawList table
		
		@tfield PushClipRect PushClipRect
		@tfield PopClipRect PopClipRect
		@tfield GetClipRectMin GetClipRectMin
		@tfield GetClipRectMax GetClipRectMax
		@tfield AddLine AddLine
		@tfield AddRect AddRect
		@tfield AddRectFilled AddRectFilled
		@tfield AddRectFilledMultiColor AddRectFilledMultiColor
		@tfield AddQuad AddQuad
		@tfield AddQuadFilled AddQuadFilled
		@tfield AddTriangle AddTriangle
		@tfield AddTriangleFilled AddTriangleFilled
		@tfield AddCircle AddCircle
		@tfield AddCircleFilled AddCircleFilled
		@tfield AddNgon AddNgon
		@tfield AddNgonFilled AddNgonFilled
		@tfield AddEllipse AddEllipse
		@tfield AddEllipseFilled AddEllipseFilled
		@tfield AddText AddText
		@tfield AddBezierCubic AddBezierCubic
		@tfield AddBezierQuadratic AddBezierQuadratic
		@tfield PathClear PathClear
		@tfield PathLineTo PathLineTo
		@tfield PathLineToMergeDuplicate PathLineToMergeDuplicate
		@tfield PathFillConvex PathFillConvex
		@tfield PathStroke PathStroke
		@tfield PathArcTo PathArcTo
		@tfield PathArcToFast PathArcToFast
		@tfield PathEllipticalArcTo PathEllipticalArcTo
		@tfield PathBezierCubicCurveTo PathBezierCubicCurveTo
		@tfield PathBezierQuadraticCurveTo PathBezierQuadraticCurveTo
		@tfield PathRect PathRect
		
		
		@table ImDrawList
		*/
		sol::usertype<ImDrawList> DrawList = lua.new_usertype<ImDrawList>("ImDrawList");

		/***
		Push Clip Rect.
		@function DrawList:PushClipRect
		@tparam float posXmin
		@tparam float posYmin
		@tparam float posXmax
		@tparam float posYmax
		@tparam bool intersect optional
		@usage
			DrawList:PushClipRect();
		*/
		DrawList.set_function("PushClipRect", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float)>(PushClipRectDraw),
			sol::resolve<void(ImDrawList*, float, float, float, float, bool)>(PushClipRectDraw)
		));
		
		/***
		Pop Clip Rect.
		@function DrawList:PopClipRect
		@usage
			DrawList:PopClipRect();
		*/
		DrawList.set_function("PopClipRect", &PopClipRectDraw);
		
		/***
		Get Clip Rect Min.
		@function DrawList:GetClipRectMin
		@treturn ImVec2 clipRectMin
		@usage
			DrawList:GetClipRectMin();
		*/
		DrawList.set_function("GetClipRectMin", &GetClipRectMin);
		
		/***
		Get Clip Rect Max.
		@function DrawList:GetClipRectMax
		@treturn ImVec2 clipRectMax
		@usage
			DrawList:GetClipRectMax();
		*/
		DrawList.set_function("GetClipRectMax", &GetClipRectMax);
		
		/***
		Add Line.
		@function DrawList:AddLine
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam int color
		@tparam float thickness optional
		@usage
			DrawList:AddLine();
		*/
		DrawList.set_function("AddLine", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t)>(AddLine),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float)>(AddLine)
		));
		
		/***
		Add Rect.
		@function DrawList:AddRect
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam int color
		@tparam float rounding optional
		@tparam int flags optional
		@tparam float thickness optional
		@usage
			DrawList:AddRect();
		*/
		DrawList.set_function("AddRect", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t)>(AddRect),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float)>(AddRect),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float, int)>(AddRect),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float, int, float)>(AddRect)
		));
		
		/***
		Add Rect Filled.
		@function DrawList:AddRectFilled
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam int color
		@tparam float rounding optional
		@tparam int flags optional
		@usage
			DrawList:AddRectFilled();
		*/
		DrawList.set_function("AddRectFilled", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t)>(AddRectFilled),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float)>(AddRectFilled),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float, int)>(AddRectFilled)
		));
		
		/***
		Add Rect Filled Multi Color.
		@function DrawList:AddRectFilledMultiColor
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam int colorUpperLeft
		@tparam int colorUpperRight
		@tparam int colorBottomRight
		@tparam int colorBottomLeft
		@usage
			DrawList:AddRectFilledMultiColor();
		*/
		DrawList.set_function("AddRectFilledMultiColor", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, uint32_t, uint32_t, uint32_t)>(AddRectFilledMultiColor)
			));
		
		/***
		Add Quad.
		@function DrawList:AddQuad
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam float point3X
		@tparam float point3Y
		@tparam float point4X
		@tparam float point4Y
		@tparam int color
		@tparam float thickness optional
		@usage
			DrawList:AddQuad();
		*/
		DrawList.set_function("AddQuad", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float, float, uint32_t)>(AddQuad),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float, float, uint32_t, float)>(AddQuad)
		));
		
		/***
		Add Quad Filled.
		@function DrawList:AddQuadFilled
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam float point3X
		@tparam float point3Y
		@tparam float point4X
		@tparam float point4Y
		@tparam int color
		@usage
			DrawList:AddQuadFilled();
		*/
		DrawList.set_function("AddQuadFilled", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float, float, uint32_t)>(AddQuadFilled)
		));
		
		/***
		Add Triangle.
		@function DrawList:AddTriangle
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam float point3X
		@tparam float point3Y
		@tparam int color
		@tparam float thickness optional
		@usage
			DrawList:AddTriangle();
		*/
		DrawList.set_function("AddTriangle", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, uint32_t)>(AddTriangle),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, uint32_t, float)>(AddTriangle)
		));
		
		/***
		Add Triangle Filled.
		@function DrawList:AddTriangleFilled
		@tparam float point1X
		@tparam float point1Y
		@tparam float point2X
		@tparam float point2Y
		@tparam float point3X
		@tparam float point3Y
		@tparam int color
		@usage
			DrawList:AddTriangleFilled();
		*/
		DrawList.set_function("AddTriangleFilled", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, uint32_t)>(AddTriangleFilled)
		));
		
		/***
		Add Circle.
		@function DrawList:AddCircle
		@tparam float point1X
		@tparam float point1Y
		@tparam float radius
		@tparam int color
		@tparam int numSegments optional
		@tparam float thickness optional
		@usage
			DrawList:AddCircle();
		*/
		DrawList.set_function("AddCircle", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t)>(AddCircle),
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t, int)>(AddCircle),
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t, int, float)>(AddCircle)
		));
		
		/***
		Add Circle Filled.
		@function DrawList:AddCircleFilled
		@tparam float point1X
		@tparam float point1Y
		@tparam float radius
		@tparam int color
		@tparam int numSegments optional
		@usage
			DrawList:AddCircle();
		*/
		DrawList.set_function("AddCircleFilled", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t)>(AddCircleFilled),
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t, int)>(AddCircleFilled)
		));
		
		/***
		Add Ngon.
		@function DrawList:AddNgon
		@tparam float point1X
		@tparam float point1Y
		@tparam float radius
		@tparam int color
		@tparam int numSegments
		@tparam float thickness optional
		@usage
			DrawList:AddNgon();
		*/
		DrawList.set_function("AddNgon", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t, int)>(AddNgon),
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t, int, float)>(AddNgon)
		));
		
		/***
		Add Ngon Filled.
		@function DrawList:AddNgonFilled
		@tparam float point1X
		@tparam float point1Y
		@tparam float radius
		@tparam int color
		@tparam int numSegments
		@usage
			DrawList:AddNgonFilled();
		*/
		DrawList.set_function("AddNgonFilled", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, uint32_t, int)>(AddNgonFilled)
		));
		
		/***
		Add Ellipse.
		@function DrawList:AddEllipse
		@tparam float centerX
		@tparam float centerY
		@tparam float radiusX
		@tparam float radiusY
		@tparam int color
		@tparam float rotation optional
		@tparam int numSegments optional
		@tparam float thickness optional
		@usage
			DrawList:AddEllipse();
		*/
		DrawList.set_function("AddEllipse", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t)>(AddEllipse),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float)>(AddEllipse),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float, int)>(AddEllipse),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float, int, float)>(AddEllipse)
		));
		
		/***
		Add Ellipse Filled.
		@function DrawList:AddEllipseFilled
		@tparam float centerX
		@tparam float centerY
		@tparam float radiusX
		@tparam float radiusY
		@tparam int color
		@tparam float rotation optional
		@tparam int numSegments optional
		@usage
			DrawList:AddEllipseFilled();
		*/
		DrawList.set_function("AddEllipseFilled", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t)>(AddEllipseFilled),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float)>(AddEllipseFilled),
			sol::resolve<void(ImDrawList*, float, float, float, float, uint32_t, float, int)>(AddEllipseFilled)
		));
		
		/***
		Add Text.
		@function DrawList:AddText
		@tparam float posX
		@tparam float posY
		@tparam int color
		@tparam string textBegin
		@tparam string textEnd optional
		@usage
			DrawList:AddText();
		*/
		DrawList.set_function("AddText", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, uint32_t, const char*)>(AddText),
			sol::resolve<void(ImDrawList*, float, float, uint32_t, const char*, const char*)>(AddText)
		));
		
		/***
		Add Bezier Cubic.
		@function DrawList:AddBezierCubic
		@tparam float p1X
		@tparam float p1Y
		@tparam float p2X
		@tparam float p2Y
		@tparam float p3X
		@tparam float p3Y
		@tparam float p4X
		@tparam float p4Y
		@tparam int color
		@tparam float thickness
		@tparam int numSegments optional
		@usage
			DrawList:AddBezierCubic();
		*/
		DrawList.set_function("AddBezierCubic", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float, float, uint32_t, float)>(AddBezierCubic),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float, float, uint32_t, float, int)>(AddBezierCubic)
		));
		
		/***
		Add Bezier Quadratic.
		@function DrawList:AddBezierQuadratic
		@tparam float p1X
		@tparam float p1Y
		@tparam float p2X
		@tparam float p2Y
		@tparam float p3X
		@tparam float p3Y
		@tparam int color
		@tparam float thickness
		@tparam int numSegments optional
		@usage
			DrawList:AddBezierQuadratic();
		*/
		DrawList.set_function("AddBezierQuadratic", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, uint32_t, float)>(AddBezierQuadratic),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, uint32_t, float, int)>(AddBezierQuadratic)
		));
		
		/***
		Path Clear.
		@function DrawList:PathClear
		@usage
			DrawList:PathClear();
		*/
		DrawList.set_function("PathClear", &PathClear);
		
		/***
		Path Line To.
		@function DrawList:PathLineTo
		@tparam float posX
		@tparam float posY
		@usage
			DrawList:PathLineTo();
		*/
		DrawList.set_function("PathLineTo", sol::overload(
			sol::resolve<void(ImDrawList*, float, float)>(PathLineTo)
		));
		
		/***
		Path Line To Merge Duplicate.
		@function DrawList:PathLineToMergeDuplicate
		@tparam float posX
		@tparam float posY
		@usage
			DrawList:PathLineToMergeDuplicate();
		*/
		DrawList.set_function("PathLineToMergeDuplicate", sol::overload(
			sol::resolve<void(ImDrawList*, float, float)>(PathLineToMergeDuplicate)
		));
		
		/***
		Path Fill Convex.
		@function DrawList:PathFillConvex
		@tparam int color
		@usage
			DrawList:PathFillConvex();
		*/
		DrawList.set_function("PathFillConvex", sol::overload(
			sol::resolve<void(ImDrawList*, uint32_t)>(PathFillConvex)
		));
		
		/***
		Path Stroke.
		@function DrawList:PathStroke
		@tparam int color
		@tparam int flags optional
		@usage
			DrawList:PathStroke();
		*/
		DrawList.set_function("PathStroke", sol::overload(
			sol::resolve<void(ImDrawList*, uint32_t)>(PathStroke),
			sol::resolve<void(ImDrawList*, uint32_t, int)>(PathStroke)
		));
		
		/***
		Path Arc To.
		@function DrawList:PathArcTo
		@tparam float centerX
		@tparam float centerY
		@tparam float radius
		@tparam float aMin
		@tparam float aMax
		@tparam int numSegments optional
		@usage
			DrawList:PathArcTo();
		*/
		DrawList.set_function("PathArcTo", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float)>(PathArcTo),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, int)>(PathArcTo)
		));
		
		/***
		Path Arc To Fast.
		@function DrawList:PathArcToFast
		@tparam float centerX
		@tparam float centerY
		@tparam float radius
		@tparam int aMin
		@tparam int aMax
		@usage
			DrawList:PathArcToFast();
		*/
		DrawList.set_function("PathArcToFast", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, int, int)>(PathArcToFast)
		));
		
		/***
		Path Elliptical Arc To.
		@function DrawList:PathEllipticalArcTo
		@tparam float centerX
		@tparam float centerY
		@tparam float radiusX
		@tparam float radiusY
		@tparam float rotation
		@tparam float aMin
		@tparam float aMax
		@tparam int numSegments optional
		@usage
			DrawList:PathEllipticalArcTo();
		*/
		DrawList.set_function("PathEllipticalArcTo", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float)>(PathEllipticalArcTo),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, float, int)>(PathEllipticalArcTo)
		));
		
		/***
		Path Bezier Cubic Curve To.
		@function DrawList:PathBezierCubicCurveTo
		@tparam float p2X
		@tparam float p2Y
		@tparam float p3X
		@tparam float p3Y
		@tparam float p4X
		@tparam float p4Y
		@tparam int numSegments optional
		@usage
			DrawList:PathBezierCubicCurveTo();
		*/
		DrawList.set_function("PathBezierCubicCurveTo", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float)>(PathBezierCubicCurveTo),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, float, int)>(PathBezierCubicCurveTo)
		));
		
		/***
		Path Bezier Quadratic Curve To.
		@function DrawList:PathBezierQuadraticCurveTo
		@tparam float p2X
		@tparam float p2Y
		@tparam float p3X
		@tparam float p3Y
		@tparam int numSegments optional
		@usage
			DrawList:PathBezierQuadraticCurveTo();
		*/
		DrawList.set_function("PathBezierQuadraticCurveTo", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float)>(PathBezierQuadraticCurveTo),
			sol::resolve<void(ImDrawList*, float, float, float, float, int)>(PathBezierQuadraticCurveTo)
		));
		
		/***
		Path Rect.
		@function DrawList:PathRect
		@tparam float p1X
		@tparam float p1Y
		@tparam float p2X
		@tparam float p2Y
		@tparam float rounding optional
		@tparam int flags optional
		@usage
			DrawList:PathRect();
		*/
		DrawList.set_function("PathRect", sol::overload(
			sol::resolve<void(ImDrawList*, float, float, float, float)>(PathRect),
			sol::resolve<void(ImDrawList*, float, float, float, float, float)>(PathRect),
			sol::resolve<void(ImDrawList*, float, float, float, float, float, int)>(PathRect)
		));

		///ImGuiIO
		//@section ImGuiIO
		/***
		Basic ImGuiIO table
		
		@tfield ImFontAtlas Fonts
		@tfield ImVec2 DisplaySize Main display size, in pixels (generally == GetMainViewport()->Size). May change every frame.
		@tfield float DeltaTime Time elapsed since last frame, in seconds. May change every frame.
		@tfield float IniSavingRate Minimum time between saving positions/sizes to .ini file, in seconds.
		@tfield string IniFilename Path to .ini file (important: default "imgui.ini" is relative to current working dir!). Set NULL to disable automatic .ini loading/saving or if you want to manually call LoadIniSettingsXXX() / SaveIniSettingsXXX() functions.
		@tfield string LogFilename Path to .log file (default parameter to ImGui::LogToFile when no file is specified).
		@tfield float FontGlobalScale Global scale all fonts
		@tfield bool FontAllowUserScaling Allow user scaling text of individual window with CTRL+Wheel.
		@tfield ImFont FontDefault Font to use on NewFrame(). Use NULL to uses Fonts->Fonts[0].
		@tfield ImVec2 DisplayFramebufferScale For retina display or other situations where window coordinates are different from framebuffer coordinates. This generally ends up in ImDrawData::FramebufferScale.
		@tfield bool MouseDrawCursor Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor). Cannot be easily renamed to 'io.ConfigXXX' because this is frequently used by backend implementations.
		@tfield float MouseDoubleClickTime Time for a double-click, in seconds.
		@tfield float MouseDoubleClickMaxDist Distance threshold to stay in to validate a double-click, in pixels.
		@tfield float MouseDragThreshold Distance threshold before considering we are dragging.
		@tfield float KeyRepeatDelay When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
		@tfield float KeyRepeatRate When holding a key/button, rate at which it repeats, in seconds.
		@tfield ImVec2 MousePos Mouse position, in pixels. Set to ImVec2(-FLT_MAX, -FLT_MAX) if mouse is unavailable (on another screen, etc.)
		@tfield float MouseWheel Mouse wheel Vertical: 1 unit scrolls about 5 lines text. >0 scrolls Up, <0 scrolls Down. Hold SHIFT to turn vertical scroll into horizontal scroll.
		@tfield bool KeyCtrl Keyboard modifier down: Control
		@tfield bool KeyShift Keyboard modifier down: Shift
		@tfield bool KeyAlt Keyboard modifier down: Alt
		@tfield bool KeySuper Keyboard modifier down: Cmd/Super/Windows
		@tfield bool WantCaptureMouseUnlessPopupClose Alternative to WantCaptureMouse: (WantCaptureMouse == true && WantCaptureMouseUnlessPopupClose == false) when a click over void is expected to close a popup.

		@table ImGuiIO
		*/
		sol::usertype<ImGuiIO>IO = lua.new_usertype<ImGuiIO>("ImGuiIO");


		IO.set("Fonts", &ImGuiIO::Fonts);
		IO.set("DisplaySize", &ImGuiIO::DisplaySize);
		IO.set("DeltaTime", &ImGuiIO::DeltaTime);
		IO.set("IniSavingRate", &ImGuiIO::IniSavingRate);
		IO.set("IniFilename", &ImGuiIO::IniFilename);
		IO.set("LogFilename", &ImGuiIO::LogFilename);
		IO.set("FontGlobalScale", &ImGuiIO::FontGlobalScale);
		IO.set("FontAllowUserScaling", &ImGuiIO::FontAllowUserScaling);
		IO.set("FontDefault", &ImGuiIO::FontDefault);
		IO.set("DisplayFramebufferScale", &ImGuiIO::DisplayFramebufferScale);
		IO.set("MouseDrawCursor", &ImGuiIO::MouseDrawCursor);
		IO.set("MouseDoubleClickTime", &ImGuiIO::MouseDoubleClickTime);
		IO.set("MouseDoubleClickMaxDist", &ImGuiIO::MouseDoubleClickMaxDist);
		IO.set("MouseDragThreshold", &ImGuiIO::MouseDragThreshold);
		IO.set("KeyRepeatDelay", &ImGuiIO::KeyRepeatDelay);
		IO.set("KeyRepeatRate", &ImGuiIO::KeyRepeatRate);
		IO.set("MousePos", &ImGuiIO::MousePos);
		IO.set("MouseWheel", &ImGuiIO::MouseWheel);
		IO.set("KeyCtrl", &ImGuiIO::KeyCtrl);
		IO.set("KeyShift", &ImGuiIO::KeyShift);
		IO.set("KeyAlt", &ImGuiIO::KeyAlt);
		IO.set("KeySuper", &ImGuiIO::KeySuper);
		IO.set("WantCaptureMouseUnlessPopupClose", &ImGuiIO::WantCaptureMouseUnlessPopupClose);

		///ImGuiTableSortSpecs
		//@section ImGuiTableSortSpecs
		/***
		Basic ImGuiTableSortSpecs table
		
		@tfield int SpecsCount
		@tfield bool SpecsDirty

		@table ImGuiTableSortSpecs
		*/
		sol::usertype<ImGuiTableSortSpecs>TableSortSpecs = lua.new_usertype<ImGuiTableSortSpecs>("ImGuiTableSortSpecs");
		TableSortSpecs.set("SpecsCount", &ImGuiTableSortSpecs::SpecsCount);
		TableSortSpecs.set("SpecsDirty", &ImGuiTableSortSpecs::SpecsDirty);

#pragma endregion IO
	}
}