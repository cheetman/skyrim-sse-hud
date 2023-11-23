#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/stat.h>
#include <menu/menu.h>
#include <menu/menu_stat.h>
#include <utils/utils.h>
#include <memory/player.h>
#include <setting/i18n.h>

namespace menu
{

	void renderStat(ImGuiWindowFlags flags)
	{
		if (stats::show_playtime_window) {
			ImGui::Begin("游玩时间", nullptr, flags);
			ImGui::Text(I18Ni(ICON_MDI_CLOCK_OUTLINE, "hud.stat.hud-playtime") , stats::playtime_hours, stats::playtime_minutes, stats::playtime_seconds);
			ImGui::End();
		}

		if (stats::show_gametime_window) {
			ImGui::Begin("游戏时间", nullptr, flags);
			ImGui::Text(I18Ni(ICON_MDI_CLOCK_OUTLINE, "hud.stat.hud-gametime"), stats::gametime_hours, stats::gametime_minutes, stats::gametime_seconds);
			ImGui::End();
		}

		if (stats::show_computertime_window) {
			ImGui::Begin("计算机时间", nullptr, flags);
			ImGui::Text(I18Ni(ICON_MDI_CLOCK_OUTLINE, "hud.stat.hud-computertime"), stats::computertime);
			ImGui::End();
		}
	}

	void renderStatSettings()
	{
		ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayMoney"), &show_player_gold_window);
		ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayCarryweight"), &show_player_carryweight_window);
		ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayXp"), &show_player_xp_window);
		ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayPlaytime"), &stats::show_playtime_window);
		ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayGametime"), &stats::show_gametime_window);
		ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayComputertime"), &stats::show_computertime_window);
	}
}
