#pragma once
#include <imgui/imgui.h>

namespace menu
{
	void buildStyle();
	void initFonts();
	extern int imgui_style_index;
	extern int imgui_font_index;
	static const char* theme_items = "Dark\0Light\0Classic\0Dark修改\0Theme4\0Theme5\0Theme6\0";
	extern std::vector<ImFont*> fonts;
	static const char* font_items = "默认字体\0崩铁\0原神\0崩三\0";
}
