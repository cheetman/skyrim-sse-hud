#pragma once
#include <imgui/imgui.h>
//#define IMGUI_USE_WCHAR32
//#define IMGUIUSEWCHAR32

	// 文件路径
extern std::string fontFilePath;

namespace menu
{
	extern bool auto_remove_ammo;
	extern bool remember_last_ammo;
	extern int hotkey;
	extern int hotkey2;
	extern float font_scale;
	extern int imgui_style_index;
	void __fastcall render();

	void initStyle();
	extern int trackX;
	extern int trackY;
	//extern ImFont* font2;

	extern bool show_player_info_window ;
	extern bool show_player_mod_window ;
	extern bool show_player_armor_window ;
	extern bool show_player_weapon_window ;
	extern bool show_player_debug_window ;
	extern bool no_titlebar ;
	extern bool no_resize ;
	extern bool no_collapse ;
	extern bool no_background ;
	extern bool auto_resize ;

	extern bool window_border ;
	extern bool frame_border ;
	extern bool bullet_text ;

	extern bool show_npc ;
	extern bool show_enemy ;
	extern bool show_teammate ;
	extern bool show_horse ;

	extern float colorPlotHistogramX ;
	extern float colorPlotHistogramY;
	extern float colorPlotHistogramZ;
	extern float colorPlotHistogramW;

	extern bool show_player_base_info_window;
	extern bool show_player_base_info_window_sep;
	extern bool flag_process;
	extern bool flag_base_info_setting;

}
