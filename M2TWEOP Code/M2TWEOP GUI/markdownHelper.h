#pragma once
#include "helpers.h"
#include "imgui_md.h"
#include "Shellapi.h"
#include "headersSTD.h"
class markdownHelper
{
public:
	static void setFonts();
	static void drawMarkdown(const string& text);
};

