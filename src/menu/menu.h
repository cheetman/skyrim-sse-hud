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
	extern int hotkey;
	bool save_settings();
	bool load_settings();
	void __fastcall render();

	//extern ImFont* font2;
}
