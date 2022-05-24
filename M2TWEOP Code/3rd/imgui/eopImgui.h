#pragma once
#include "imgui.h"

namespace ImGui
{
	void StyleGrey();
	void LoadingIndicatorCircle(const char* label, const float indicator_radius,
		const ImVec4& main_color, const ImVec4& backdrop_color,
		const int circle_count, const float speed);
};