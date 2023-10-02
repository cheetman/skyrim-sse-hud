#pragma once
#include <imgui/imgui.h>
//#define IMGUI_USE_WCHAR32
//#define IMGUIUSEWCHAR32

	// 文件路径
extern std::filesystem::path settings;
extern std::string fontFilePath;

namespace menu
{
	extern bool auto_remove_ammo;
	extern bool remember_last_ammo;
	extern int hotkey;
	extern int hotkey2;
	extern float font_scale;
	extern int imgui_style_index;
	bool save_settings();
	bool load_settings();
	void __fastcall render();

	void initStyle();
	//extern ImFont* font2;
}
