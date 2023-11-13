#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/stat.h>
#include <menu/menu.h>
#include <menu/menu_stat.h>
#include <utils/utils.h>
#include <memory/player.h>

namespace menu
{

	char show_playtime_window_pretext[32] = "";
	char show_gametime_window_pretext[32] = "";
	char show_computertime_window_pretext[32] = "";

	bool show_playtime_window_pre = false;
	bool show_gametime_window_pre = false;
	bool show_computertime_window_pre = false;

	void renderStat(ImGuiWindowFlags flags)
	{
		if (stats::show_playtime_window) {
			ImGui::Begin("游玩时间", nullptr, flags);
			ImGui::Text(ICON_MDI_CLOCK_OUTLINE " %s%02d:%02d:%02d", show_playtime_window_pretext,  stats::playtime_hours, stats::playtime_minutes, stats::playtime_seconds);
			ImGui::End();
		}

		if (stats::show_gametime_window) {
			ImGui::Begin("游戏时间", nullptr, flags);
			ImGui::Text(ICON_MDI_CLOCK_OUTLINE " %s%02d:%02d:%02d", show_gametime_window_pretext, stats::gametime_hours, stats::gametime_minutes, stats::gametime_seconds);
			ImGui::End();
		}

		if (stats::show_computertime_window) {
			ImGui::Begin("计算机时间", nullptr, flags);
			ImGui::Text(ICON_MDI_CLOCK_OUTLINE " %s%s", show_computertime_window_pretext, stats::computertime);
			ImGui::End();
		}
	}

	void renderStatSettings()
	{
		ImGui::TableNextColumn();
		ImGui::Checkbox("显示金钱", &show_player_gold_window);
		ImGui::TableNextColumn();
		ImGui::Checkbox("显示游玩时间", &stats::show_playtime_window);
		/*if (stats::show_playtime_window) {
			if (ImGui::TreeNodeEx("设置##51", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("显示前缀:", &show_playtime_window_pre);
				ImGui::TreePop();
			}
		}*/
		ImGui::TableNextColumn();
		ImGui::Checkbox("显示负重", &show_player_carryweight_window);
		ImGui::TableNextColumn();
		ImGui::Checkbox("显示游戏时间", &stats::show_gametime_window);
		/*if (stats::show_gametime_window) {
			if (ImGui::TreeNodeEx("设置##52", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("显示前缀:", &show_gametime_window_pre);
				ImGui::TreePop();
			}
		}*/
		ImGui::TableNextColumn();
		ImGui::Checkbox("显示经验", &show_player_xp_window);
		ImGui::TableNextColumn();
		ImGui::Checkbox("显示计算机时间", &stats::show_computertime_window);
		/*if (stats::show_computertime_window) {
			if (ImGui::TreeNodeEx("设置##53", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("显示前缀:", &show_computertime_window_pre);
				ImGui::TreePop();
			}
		}*/
	}
}
