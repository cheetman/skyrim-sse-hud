#pragma once
#include <imgui/imgui.h>

namespace menu
{
	extern char show_playtime_window_pretext[32];
	extern char show_gametime_window_pretext[32];
	extern char show_computertime_window_pretext[32];
	extern bool show_playtime_window_pre ;
	extern bool show_gametime_window_pre ;
	extern bool show_computertime_window_pre ;

	void renderStat(ImGuiWindowFlags flags);

	void renderStatSettings();
}
