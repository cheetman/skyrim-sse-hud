#pragma once
#include <imgui/imgui.h>

namespace menu
{
	void buildStyle();
	void initFonts();
	extern int imgui_style_index;
	extern int imgui_font_index;
	//static const char* theme_items = "Dark\0Light\0Classic\0Dark修改\0Theme4\0Theme5\0Theme6\0";
	extern std::vector<ImFont*> fonts;
	//static const char* font_items = "默认字体\0崩铁\0原神\0崩三\0";

	static const std::unordered_map<int, std::string> theme_items = {
		{ 0, "Dark" },
		{ 1, "0Light" },
		{ 2, "0Classic" },
		{ 3, "Dark修改" },
		{ 4, "主题4" },
		{ 5, "主题5" },
		{ 6, "主题6" },
	};

	static const std::unordered_map<int, std::string> font_items = {
		{ 0, "默认字体" },
		{ 1, "崩铁" },
		{ 2, "原神" },
		{ 3, "崩坏三" },
	};

}
