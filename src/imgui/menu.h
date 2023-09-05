#pragma once


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
}
