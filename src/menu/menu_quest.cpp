#include "menu_quest.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <menu/menu.h>
#include <utils/utils.h>
#include <memory/memory.h>

namespace menu
{
	ImVec4 colorQuestTableHeaderBg(0.0f, 0.7f, 0.0f, 0.75f);
	ImVec4 colorQuestTableBorderStrong(0.286f, 1, 0, 0.7f);

	void buildQuestItem(int count, std::vector<QuestInfo>& items)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 2;

		// 显示formid
		if (show_items_window_formid) {
			columnCount++;
		}

		if (show_items_window_file) {
			columnCount++;
		}
		columnCount++;

		ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, colorQuestTableHeaderBg);
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, colorQuestTableBorderStrong);

		if (ImGui::BeginTable("tableQuest", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			//ImGui::TableSetupColumn("进度", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
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

					ImGui::PushItemWidth(ImGui::GetFontSize() * 2.5);
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorQuestTableHeaderBg);
					ImGui::ProgressBar(item.progressStage, ImVec2(0, 0), buf);
					ImGui::PopStyleColor();
					ImGui::PopItemWidth();

					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

					if (item.isActive) {
						if (ImGui::BeginPopupContextItem("questData")) {
							ImGui::Text("%s", item.name.c_str());
							ImGui::Separator();
							if (ImGui::BeginTable("tableQuestData", 4)) {
								int i = 1;
								auto quest = item.ptr;
								auto currentStage = quest->GetCurrentStageID();
								for (auto item2 : *quest->waitingStages) {
									if (currentStage == item2->data.index) {
										ImGui::TableNextColumn();
										myTextColored(ImVec4(0, 1, 0, 1.0f), "阶段-%d", i);
										ImGui::TableNextColumn();
										myTextColored(ImVec4(0, 1, 0, 1.0f), "%d " ICON_MDI_FLAG_TRIANGLE, item2->data.index);
									} else {
										ImGui::TableNextColumn();
										ImGui::Text("阶段-%d", i);
										ImGui::TableNextColumn();
										ImGui::Text("%d", item2->data.index);
									}

									ImGui::TableNextColumn();
									if (item2->data.flags.underlying() == 1) {
										myTextColored(ImVec4(0, 1, 0, 1.0f), "已完成");
									} else {
										myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "-");
									}
									ImGui::TableNextColumn();
									if (item2->data.index > currentStage) {
										if (ImGui::SmallButton(ICON_MDI_CHECK)) {
											int id = item2->data.index;
											/*if (!ScriptUtil::SetCurrentStageID(nullptr, 0, quest, id)) {
											}*/
											std::string commandStr = "setstage  ";
											commandStr.append(item.editorId);
											commandStr.append(" ");
											commandStr.append(std::to_string(id));
											ScriptUtil::ExecuteCommand(commandStr);
											//ImGui::CloseCurrentPopup();
										}
									}
									i++;
								}

								ImGui::EndTable();
							}
							ImGui::EndPopup();
						}

						if (ImGui::Selectable(item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							ImGui::OpenPopup("questData");
						}
						//ImGui::Text("%s", item.name.c_str());
					} else {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", item.name.c_str());
					}

					ImGui::TableNextColumn();
					if (item.isActive) {
						ImGui::Text("%s", item.editorId.c_str());
					} else {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", item.editorId.c_str());
					}

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
