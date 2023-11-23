#pragma once
#include <imgui/imgui.h>

namespace menu
{
	void buildStyle();
	void initFonts();
	extern int imgui_style_index;
	extern int imgui_font_index;
	extern std::vector<ImFont*> fonts;

	static const std::unordered_map<int, std::string> theme_items = {
		{ 0, "Style Dark" },
		{ 1, "Style Light" },
		{ 2, "Style Classic" },
		{ 3, "Style Dark M" },
		{ 4, "Style 4" },
		{ 5, "Style 5" },
		{ 6, "Style 6" },
	};

	static const std::unordered_map<int, std::string> font_items = {
		{ 0, "Default" },
		{ 1, "Honkai: Star Rail" },
		{ 2, "Genshin Impact" },
		{ 3, "Honkai Impact 3" },
	};

}
