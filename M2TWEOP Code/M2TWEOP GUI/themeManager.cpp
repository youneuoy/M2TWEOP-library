#include "helpers.h"
#include "dataG.h"
#include "imgui.h"
#include "cpptoml.h"

// Convert RGBA strings to ImVec4
auto parseColor = [](const std::string& colorStr) -> ImVec4 {
	ImVec4 color;
	sscanf_s(colorStr.c_str(), "rgba(%f, %f, %f, %f)", &color.x, &color.y, &color.z, &color.w);
	color.x = color.x/255;
	color.y = color.y/255;
	color.z = color.z/255;
	return color;
};

auto arrayToImVec2 = [](const std::vector<double> themeValues) -> ImVec2 {
	return ImVec2(static_cast<float>(themeValues[0]), static_cast<float>(themeValues[1]));
};

void tomlToStyle(string themeName)
{
	// Light style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	// Load the TOML file
	std::string fPath = ".\\eopData\\themes\\"+themeName+".toml";
	auto config = cpptoml::parse_file(fPath);

	// Assign the styles
	style.Alpha 								= *config->get_as<double>("alpha");
	style.WindowPadding 			      =  arrayToImVec2(*config->get_array_of<double>("windowPadding"));
	style.WindowRounding				 = *config->get_as<double>("windowRounding");
	style.WindowBorderSize 		         = *config->get_as<double>("windowBorderSize");
	style.WindowMinSize 			       = arrayToImVec2(*config->get_array_of<double>("windowMinSize"));
	style.WindowTitleAlign 		 		    =  arrayToImVec2(*config->get_array_of<double>("windowTitleAlign"));
	// style.WindowMenuButtonPosition 	=  *config->get_as<double>("windowMenuButtonPosition");
	style.ChildRounding 		         = *config->get_as<double>("childRounding");
	style.ChildBorderSize 		         = *config->get_as<double>("childBorderSize");
	style.PopupRounding 		         = *config->get_as<double>("popupRounding");
	style.PopupBorderSize 		         = *config->get_as<double>("popupBorderSize");
	style.FramePadding 		         = arrayToImVec2(*config->get_array_of<double>("framePadding"));
	style.FrameRounding 		         = *config->get_as<double>("frameRounding");
	style.FrameBorderSize 		         = *config->get_as<double>("frameBorderSize");
	style.ItemSpacing 		         = arrayToImVec2(*config->get_array_of<double>("itemSpacing"));
	style.ItemInnerSpacing 		         = arrayToImVec2(*config->get_array_of<double>("itemInnerSpacing"));
	style.CellPadding 		         = arrayToImVec2(*config->get_array_of<double>("cellPadding"));
	style.IndentSpacing 		         = *config->get_as<double>("indentSpacing");
	style.ColumnsMinSpacing 		         = *config->get_as<double>("columnsMinSpacing");
	style.ScrollbarSize 		         = *config->get_as<double>("scrollbarSize");
	style.ScrollbarSize 		         = *config->get_as<double>("scrollbarRounding");
	style.GrabMinSize 		         = *config->get_as<double>("grabMinSize");
	style.GrabRounding 		         = *config->get_as<double>("grabRounding");
	style.TabRounding 		         = *config->get_as<double>("tabRounding");
	style.TabBorderSize 		         = *config->get_as<double>("tabBorderSize");
	style.TabMinWidthForCloseButton 		         = *config->get_as<double>("tabMinWidthForCloseButton");
	// style.TabMinWidthForCloseButton 		         = *config->get_as<double>("colorButtonPosition");
	style.ButtonTextAlign 		         = arrayToImVec2(*config->get_array_of<double>("buttonTextAlign"));
	style.SelectableTextAlign 		         = arrayToImVec2(*config->get_array_of<double>("selectableTextAlign"));

    // style.DisabledAlpha = 0.6000000238418579f;
    // style.WindowPadding = ImVec2(6.0f, 3.0f);
    // style.WindowRounding = 0.0f;
    // style.WindowBorderSize = 1.0f;
    // style.WindowMinSize = ImVec2(32.0f, 32.0f);
    // style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    // style.WindowMenuButtonPosition = ImGuiDir_Left;
    // style.ChildRounding = 0.0f;
    // style.ChildBorderSize = 1.0f;
    // style.PopupRounding = 0.0f;
    // style.PopupBorderSize = 1.0f;
    // style.FramePadding = ImVec2(5.0f, 1.0f);
    // style.FrameRounding = 3.0f;
    // style.FrameBorderSize = 1.0f;
    // style.ItemSpacing = ImVec2(8.0f, 4.0f);
    // style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    // style.CellPadding = ImVec2(4.0f, 2.0f);
    // style.IndentSpacing = 21.0f;
    // style.ColumnsMinSpacing = 6.0f;
    // style.ScrollbarSize = 13.0f;
    // style.ScrollbarRounding = 16.0f;
    // style.GrabMinSize = 20.0f;
    // style.GrabRounding = 2.0f;
    // style.TabRounding = 4.0f;
    // style.TabBorderSize = 1.0f;
    // style.TabMinWidthForCloseButton = 0.0f;
    // style.ColorButtonPosition = ImGuiDir_Right;
    // style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    // style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	// Access the 'colors' section in the TOML file
    auto colorsTable = config->get_table("colors");

   	// Expected Output Format
	style.Colors[ImGuiCol_Text] = parseColor(*colorsTable->get_as<std::string>("Text"));
	style.Colors[ImGuiCol_TextDisabled] = parseColor(*colorsTable->get_as<std::string>("TextDisabled"));
	style.Colors[ImGuiCol_WindowBg] = parseColor(*colorsTable->get_as<std::string>("WindowBg"));
	style.Colors[ImGuiCol_ChildBg] = parseColor(*colorsTable->get_as<std::string>("ChildBg"));
	style.Colors[ImGuiCol_PopupBg] = parseColor(*colorsTable->get_as<std::string>("PopupBg"));
	style.Colors[ImGuiCol_Border] = parseColor(*colorsTable->get_as<std::string>("Border"));
	style.Colors[ImGuiCol_PopupBg] = parseColor(*colorsTable->get_as<std::string>("PopupBg"));
	style.Colors[ImGuiCol_BorderShadow] = parseColor(*colorsTable->get_as<std::string>("BorderShadow"));
	style.Colors[ImGuiCol_FrameBg] = parseColor(*colorsTable->get_as<std::string>("FrameBg"));
	style.Colors[ImGuiCol_FrameBgHovered] = parseColor(*colorsTable->get_as<std::string>("FrameBgHovered"));
	style.Colors[ImGuiCol_FrameBgActive] = parseColor(*colorsTable->get_as<std::string>("FrameBgActive"));
	style.Colors[ImGuiCol_TitleBg] = parseColor(*colorsTable->get_as<std::string>("TitleBg"));
	style.Colors[ImGuiCol_TitleBgActive] = parseColor(*colorsTable->get_as<std::string>("TitleBgActive"));
	style.Colors[ImGuiCol_TitleBgCollapsed] = parseColor(*colorsTable->get_as<std::string>("TitleBgCollapsed"));
	style.Colors[ImGuiCol_MenuBarBg] = parseColor(*colorsTable->get_as<std::string>("MenuBarBg"));
	style.Colors[ImGuiCol_ScrollbarBg] = parseColor(*colorsTable->get_as<std::string>("ScrollbarBg"));
	style.Colors[ImGuiCol_ScrollbarGrab] = parseColor(*colorsTable->get_as<std::string>("ScrollbarGrab"));
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = parseColor(*colorsTable->get_as<std::string>("ScrollbarGrabHovered"));
	style.Colors[ImGuiCol_ScrollbarGrabActive] = parseColor(*colorsTable->get_as<std::string>("ScrollbarGrabActive"));
	style.Colors[ImGuiCol_SliderGrab] = parseColor(*colorsTable->get_as<std::string>("SliderGrab"));
	style.Colors[ImGuiCol_SliderGrabActive] = parseColor(*colorsTable->get_as<std::string>("SliderGrabActive"));
	style.Colors[ImGuiCol_Button] = parseColor(*colorsTable->get_as<std::string>("Button"));
	style.Colors[ImGuiCol_ButtonHovered] = parseColor(*colorsTable->get_as<std::string>("ButtonHovered"));
	style.Colors[ImGuiCol_ButtonActive] = parseColor(*colorsTable->get_as<std::string>("ButtonActive"));
	style.Colors[ImGuiCol_Header] = parseColor(*colorsTable->get_as<std::string>("Header"));
	style.Colors[ImGuiCol_HeaderHovered] = parseColor(*colorsTable->get_as<std::string>("HeaderHovered"));
	style.Colors[ImGuiCol_HeaderActive] = parseColor(*colorsTable->get_as<std::string>("HeaderActive"));
	style.Colors[ImGuiCol_Separator] = parseColor(*colorsTable->get_as<std::string>("Separator"));
	style.Colors[ImGuiCol_SeparatorHovered] = parseColor(*colorsTable->get_as<std::string>("SeparatorHovered"));
	style.Colors[ImGuiCol_SeparatorActive] = parseColor(*colorsTable->get_as<std::string>("SeparatorActive"));
	style.Colors[ImGuiCol_ResizeGrip] = parseColor(*colorsTable->get_as<std::string>("ResizeGrip"));
	style.Colors[ImGuiCol_ResizeGripHovered] = parseColor(*colorsTable->get_as<std::string>("ResizeGripHovered"));
	style.Colors[ImGuiCol_ResizeGripActive] = parseColor(*colorsTable->get_as<std::string>("ResizeGripActive"));
	style.Colors[ImGuiCol_Tab] = parseColor(*colorsTable->get_as<std::string>("Tab"));
	style.Colors[ImGuiCol_TabHovered] = parseColor(*colorsTable->get_as<std::string>("TabHovered"));
	style.Colors[ImGuiCol_TabActive] = parseColor(*colorsTable->get_as<std::string>("TabActive"));
	style.Colors[ImGuiCol_TabUnfocused] = parseColor(*colorsTable->get_as<std::string>("TabUnfocused"));
	style.Colors[ImGuiCol_TabUnfocusedActive] = parseColor(*colorsTable->get_as<std::string>("TabUnfocusedActive"));
	style.Colors[ImGuiCol_PlotLines] = parseColor(*colorsTable->get_as<std::string>("PlotLines"));
	style.Colors[ImGuiCol_PlotLinesHovered] = parseColor(*colorsTable->get_as<std::string>("PlotLinesHovered"));
	style.Colors[ImGuiCol_PlotHistogram] = parseColor(*colorsTable->get_as<std::string>("PlotHistogram"));
	style.Colors[ImGuiCol_PlotHistogramHovered] = parseColor(*colorsTable->get_as<std::string>("PlotHistogramHovered"));
	style.Colors[ImGuiCol_TableHeaderBg] = parseColor(*colorsTable->get_as<std::string>("TableHeaderBg"));
	style.Colors[ImGuiCol_TableBorderStrong] = parseColor(*colorsTable->get_as<std::string>("TableBorderStrong"));
	style.Colors[ImGuiCol_TableBorderLight] = parseColor(*colorsTable->get_as<std::string>("TableBorderLight"));
	style.Colors[ImGuiCol_TableRowBg] = parseColor(*colorsTable->get_as<std::string>("TableRowBg"));
	style.Colors[ImGuiCol_TableRowBgAlt] = parseColor(*colorsTable->get_as<std::string>("TableRowBgAlt"));
	style.Colors[ImGuiCol_TextSelectedBg] = parseColor(*colorsTable->get_as<std::string>("TextSelectedBg"));
	style.Colors[ImGuiCol_DragDropTarget] = parseColor(*colorsTable->get_as<std::string>("DragDropTarget"));
	style.Colors[ImGuiCol_NavHighlight] = parseColor(*colorsTable->get_as<std::string>("NavHighlight"));
	style.Colors[ImGuiCol_NavWindowingHighlight] = parseColor(*colorsTable->get_as<std::string>("NavWindowingHighlight"));
	style.Colors[ImGuiCol_NavWindowingDimBg] = parseColor(*colorsTable->get_as<std::string>("NavWindowingDimBg"));
	style.Colors[ImGuiCol_ModalWindowDimBg] = parseColor(*colorsTable->get_as<std::string>("ModalWindowDimBg"));
}

void setStyle(string themeName)
{
	if (themeName == "youneouy.toml")
	{
		tomlToStyle("youneouy");
	}
	else if (themeName == "mithridates.toml")
	{
		tomlToStyle("mithridates");
	}
	else if (themeName == "sulla.toml")
	{
		tomlToStyle("sulla");
	}
	else 
	{
		tomlToStyle("default");
	}
}