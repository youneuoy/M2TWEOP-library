#pragma once

#include "headersSTD.h"
#include "helpers.h"
#include "imgui.h"

// Convert RGBA strings to ImVec4
auto parseColor = [](const std::string& colorStr) -> ImVec4 {
    ImVec4 color;
    sscanf_s(colorStr.c_str(), "rgba(%f, %f, %f, %f)", &color.x, &color.y, &color.z, &color.w);
    return color;
};

void tomlToStyle(std::string themeName);

void setStyle(std::string themeName);