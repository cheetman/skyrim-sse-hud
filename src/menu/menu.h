#pragma once
#include <imgui/imgui.h>

namespace menu
{
	extern bool auto_remove_ammo;
	extern bool remember_last_ammo;
	extern int hotkey;
	extern int hotkey2;
	extern float font_scale;
	void __fastcall render();

	extern int trackX;
	extern int trackY;

	extern bool show_player_debug_window;
	extern bool no_titlebar;
	extern bool no_resize;
	extern bool no_collapse;
	extern bool no_background;
	extern bool no_background_items;

	extern bool auto_resize;

	extern bool window_border;
	extern bool frame_border;
	extern bool bullet_text;

	extern bool show_player_base_info_window_sep;
	extern bool flag_process;
	extern bool flag_base_info_setting;

	struct EquipmentItem
	{
		RE::TESBoundObject* item;
		int slotMask;
	};

	extern ImVec4 colorProgressHp;
	extern ImVec4 colorProgressMp;
	extern ImVec4 colorProgressSp;


	static const std::unordered_map<int, std::string> hotkey_items = {
		{ 0, "Insert" },
		{ 1, "F11" },
		{ 2, "F12" },
		{ 3, "Shift+Q" },
		{ 4, "Alt+Q" }
	};

	extern float grabRounding;
	extern float frameRounding;
	extern float windowRounding;

	static void myTextColored(const ImVec4 col, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		ImGui::TextColoredV(col, fmt, args);
		va_end(args);
	}

}
