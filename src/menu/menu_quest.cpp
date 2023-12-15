#include "menu_quest.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <menu/menu.h>
#include <menu/menu_track.h>
#include <utils/utils.h>

namespace menu
{
	void buildQuestItem(int count, std::vector<QuestInfo>& items)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 3;

		// 显示formid
		if (show_items_window_formid) {
			columnCount++;
		}

		if (show_items_window_file) {
			columnCount++;
		}
		columnCount++;

		ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(1.0f, 0.843f, 0.0f, 0.75f));
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(1.0f, 0.843f, 0.0f, 0.7f));

		if (ImGui::BeginTable("tableQuest", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn("进度", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 150.0f * ImGui::GetIO().FontGlobalScale, TableColumn_2);
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 60.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);

			if (show_items_window_formid) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_7);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn("MOD", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_8);
			}
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_9);
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					QuestInfo& item = items[row_n];
					ImGui::PushID(row_n);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					char buf[32];
					snprintf(buf, 32, "%d/%d", item.completedStage, item.allStage);
					ImGui::ProgressBar(item.progressStage, ImVec2(0.f, 0.f), buf);

					ImGui::TableNextColumn();
					if (item.isActive) {
						ImGui::Text("%s", item.name.c_str());
					} else {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", item.name.c_str());
					}

					ImGui::TableNextColumn();
					ImGui::Text("%s", item.editorId.c_str());

					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%08X", item.formId);
					}
					if (show_items_window_file) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.filename.c_str());
					}

					ImGui::TableNextColumn();
					if (show_items_window_settings) {
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

}
