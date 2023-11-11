#pragma once
#include <imgui/imgui.h>


namespace menu
{
	extern bool show_npc_window_formid;
	extern bool show_npc_window_split;
	extern bool show_npc;
	extern bool show_enemy;
	extern bool show_teammate;
	extern bool show_horse;


	

	
	void renderNpc(ImGuiWindowFlags flags);

		void renderNpcSettings();
}
