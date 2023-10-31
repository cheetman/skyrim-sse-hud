#include "lotd.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <utils/utils.h>

namespace lotd
{
	enum TableColumn
	{
		TableColumn_1,
		TableColumn_2,
		TableColumn_3,
		TableColumn_4,
		TableColumn_5,
		TableColumn_6,
		TableColumn_7,
		TableColumn_8,
		TableColumn_9
	};

	void buildItemInfo(int count, std::vector<LotdInfo>& items)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 3;

		if (show_items_window_refid) {
			columnCount++;
		}

		// 显示formid
		if (show_items_window_formid) {
			columnCount++;
		}
		columnCount++;

		if (show_items_window_direction) {
			columnCount++;
		}

		if (show_items_window_file) {
			columnCount++;
		}

		if (ImGui::BeginTable("tableItem3333", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
			//ImGui::TableSetupColumn("类型", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40, TableColumn_2);
			ImGui::TableSetupColumn("价值", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 35.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);
			ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, TableColumn_4);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn("方位", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_5);
			}

			if (show_items_window_refid) {
				ImGui::TableSetupColumn("REFID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_6);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_7);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn("MOD", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_8);
			}
			//if (show_items_window_settings) {
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_9);
			//}
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					LotdInfo& item = items[row_n];
					ImGui::PushID(item.formId + item.contformId + 0x5000000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					// 名称
					char buf[120];
					if (item.ptr) {
						if (item.isEnchanted) {
							snprintf(buf, 120, "%s" ICON_MDI_FLASH, item.name.c_str());
						} else {
							snprintf(buf, 120, "%s", item.name.c_str());
						}
					} else {
						if (item.count > 1) {
							if (item.isEnchanted) {
								snprintf(buf, 120, "%s" ICON_MDI_FLASH "(%d) [%s]", item.name.c_str(), item.count, item.contname.c_str());
							} else {
								snprintf(buf, 120, "%s(%d) [%s]", item.name.c_str(), item.count, item.contname.c_str());
							}
						} else {
							if (item.isEnchanted) {
								snprintf(buf, 120, "%s" ICON_MDI_FLASH " [%s]", item.name.c_str(), item.contname.c_str());
							} else {
								snprintf(buf, 120, "%s [%s]", item.name.c_str(), item.contname.c_str());
							}
						}
					}

					if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						if (ImGui::Selectable(buf, false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								if (!item.ptr->IsMarkedForDeletion()) {
									addItem(nullptr, item.ptr, 1);
								}
							}
						}
						ImGui::PopStyleColor();
					} else {
						if (ImGui::Selectable(buf, false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								if (!item.ptr->IsMarkedForDeletion()) {
									addItem(nullptr, item.ptr, 1);
								}
							}
						}
					}

					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.formTypeStr.c_str());*/

					ImGui::TableNextColumn();
					ImGui::Text("%d", item.gold);

					ImGui::TableNextColumn();
					ImGui::Text("%.1f", item.weight);

					if (show_items_window_direction) {
						ImGui::TableNextColumn();
						switch (item.direction) {
						case 1:  //前
							ImGui::Text(" \uf062%.0f", item.distance);
							break;
						case 2:  //左
							ImGui::Text(" \uf060%.0f", item.distance);
							break;
						case 3:
							ImGui::Text(" \uf063%.0f", item.distance);
							break;
						case 4:
							ImGui::Text(" \uf061%.0f", item.distance);
							break;
						default:
							ImGui::Text(" %.0f", item.distance);
							break;
						}
					}

					if (show_items_window_refid) {
						ImGui::TableNextColumn();
						ImGui::Text("%08X", item.formId);
					}
					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%08X", item.baseFormId);
					}
					if (show_items_window_file) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.filename.c_str());
					}

					ImGui::TableNextColumn();
					if (show_items_window_settings) {
						if (ImGui::SmallButton("\uf101")) {
							if (item.ptr) {
								snprintf(buf, 120, "player.moveto %08X", item.formId);
								ScriptUtil::ExecuteCommand(buf);
							} else {
								snprintf(buf, 120, "player.moveto %08X", item.contformId);
								ScriptUtil::ExecuteCommand(buf);
							}
							if (activeItems) {
								activeItems = false;
							}
						}
					}
					RE::TESObjectREFR* ptr = item.ptr;
					if (!ptr) {
						ptr = item.contptr;
					}
					if (trackPtrs.find(ptr) == trackPtrs.end()) {
						if (show_items_window_settings) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						}
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
							trackPtrs.insert(ptr);
						}
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
		}
	}

	void buildItemInfoAttached(int count, std::vector<LotdInfo>& items)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 2;

		if (show_items_window_refid) {
			columnCount++;
		}

		// 显示formid
		if (show_items_window_formid) {
			columnCount++;
		}
		columnCount++;

		if (show_items_window_direction) {
			columnCount++;
		}

		if (show_items_window_file) {
			columnCount++;
		}

		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));

		if (ImGui::BeginTable("tableItem444", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
			ImGui::TableSetupColumn("房间", ImGuiTableColumnFlags_WidthFixed , 50.0f * ImGui::GetIO().FontGlobalScale, TableColumn_2);
			/*ImGui::TableSetupColumn("价值", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 35.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);
			ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, TableColumn_4);*/

			if (show_items_window_direction) {
				ImGui::TableSetupColumn("方位", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_5);
			}

			if (show_items_window_refid) {
				ImGui::TableSetupColumn("REFID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_6);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_7);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn("MOD", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_8);
			}
			//if (show_items_window_settings) {
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_9);
			//}
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					LotdInfo& item = items[row_n];
					ImGui::PushID(item.formId + item.contformId + 0x5000000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					// 名称
					char buf[120];
					if (item.ptr) {
						if (item.isEnchanted) {
							snprintf(buf, 120, "%s" ICON_MDI_FLASH, item.name.c_str());
						} else {
							snprintf(buf, 120, "%s", item.name.c_str());
						}
					} else {
						if (item.count > 1) {
							if (item.isEnchanted) {
								snprintf(buf, 120, "%s" ICON_MDI_FLASH "(%d) [%s]", item.name.c_str(), item.count, item.contname.c_str());
							} else {
								snprintf(buf, 120, "%s(%d) [%s]", item.name.c_str(), item.count, item.contname.c_str());
							}
						} else {
							if (item.isEnchanted) {
								snprintf(buf, 120, "%s" ICON_MDI_FLASH " [%s]", item.name.c_str(), item.contname.c_str());
							} else {
								snprintf(buf, 120, "%s [%s]", item.name.c_str(), item.contname.c_str());
							}
						}
					}

					if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						if (ImGui::Selectable(buf, false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								if (!item.ptr->IsMarkedForDeletion()) {
									addItem(nullptr, item.ptr, 1);
								}
							}
						}
						ImGui::PopStyleColor();
					} else {
						if (ImGui::Selectable(buf, false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								if (!item.ptr->IsMarkedForDeletion()) {
									addItem(nullptr, item.ptr, 1);
								}
							}
						}
					}

					ImGui::TableNextColumn();
					ImGui::Text("%s", item.roomName.c_str());

					/*ImGui::TableNextColumn();
					ImGui::Text("%d", item.gold);

					ImGui::TableNextColumn();
					ImGui::Text("%.1f", item.weight);*/

					if (show_items_window_direction) {
						ImGui::TableNextColumn();
						switch (item.direction) {
						case 1:  //前
							ImGui::Text(" \uf062%.0f", item.distance);
							break;
						case 2:  //左
							ImGui::Text(" \uf060%.0f", item.distance);
							break;
						case 3:
							ImGui::Text(" \uf063%.0f", item.distance);
							break;
						case 4:
							ImGui::Text(" \uf061%.0f", item.distance);
							break;
						default:
							ImGui::Text(" %.0f", item.distance);
							break;
						}
					}

					if (show_items_window_refid) {
						ImGui::TableNextColumn();
						ImGui::Text("%08X", item.formId);
					}
					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%08X", item.baseFormId);
					}
					if (show_items_window_file) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.filename.c_str());
					}

					ImGui::TableNextColumn();
					if (show_items_window_settings) {
						if (ImGui::SmallButton("\uf101")) {
							if (item.ptr) {
								snprintf(buf, 120, "player.moveto %08X", item.formId);
								ScriptUtil::ExecuteCommand(buf);
							} else {
								snprintf(buf, 120, "player.moveto %08X", item.contformId);
								ScriptUtil::ExecuteCommand(buf);
							}
							if (activeItems) {
								activeItems = false;
							}
						}
					}
					RE::TESObjectREFR* ptr = item.ptr;
					if (!ptr) {
						ptr = item.contptr;
					}
					if (trackPtrs.find(ptr) == trackPtrs.end()) {
						if (show_items_window_settings) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						}
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
							trackPtrs.insert(ptr);
						}
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
	}
	void __fastcall render()
	{
		if (getCount("Armory") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 军械库");
			buildItemInfo(getCount("Armory"), getItems("Armory"));
		}

		if (getCount("GalleryLibrary") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 艺术馆图书室");
			buildItemInfo(getCount("GalleryLibrary"), getItems("GalleryLibrary"));
		}

		if (getCount("DaedricGallery") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 魔族大厅");
			buildItemInfo(getCount("DaedricGallery"), getItems("DaedricGallery"));
		}

		if (getCount("DragonbornHall") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 龙裔大厅");
			buildItemInfo(getCount("DragonbornHall"), getItems("DragonbornHall"));
		}

		if (getCount("HallOfHeroes") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 英雄大厅");
			buildItemInfo(getCount("HallOfHeroes"), getItems("HallOfHeroes"));
		}

		if (getCount("HallOfLostEmpires") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 失落帝国大厅");
			buildItemInfo(getCount("HallOfLostEmpires"), getItems("HallOfLostEmpires"));
		}

		if (getCount("HallOfOddities") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 奇物大厅");
			buildItemInfo(getCount("HallOfOddities"), getItems("HallOfOddities"));
		}

		if (getCount("NaturalScience") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 自然科学");
			buildItemInfo(getCount("NaturalScience"), getItems("NaturalScience"));
		}
		if (getCount("HallOfSecrets") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 秘密大厅");
			buildItemInfo(getCount("HallOfSecrets"), getItems("HallOfSecrets"));
		}

		if (getCount("MuseumStoreroom") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 博物馆储藏室");
			buildItemInfo(getCount("MuseumStoreroom"), getItems("MuseumStoreroom"));
		}
		if (getCount("Guildhouse") > 0) {
			ImGui::TableNextColumn();
			ImGui::Text(ICON_MDI_HOME " 探险家协会");
			buildItemInfo(getCount("Guildhouse"), getItems("Guildhouse"));
		}

		{
			ImGui::TableNextColumn();
			ImGui::Checkbox("更多", &show_items_window_settings);
			if (show_items_window_settings) {
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
				ImGui::BeginChild("childItemsSetting", ImVec2(ImGui::GetTextLineHeightWithSpacing() * 15, ImGui::GetTextLineHeightWithSpacing() * ((float)show_inv_window_height - 0.5f)), true, window_flags);

				ImGui::Checkbox("显示附近藏品数量", &showlocationItemCount);
				ImGui::Checkbox("排除背包物品", &isInvIgnore);
				ImGui::Checkbox("排除犯罪物品", &isCrimeIgnore);
				ImGui::Checkbox("排除军械库物品", &isArmoryIgnore);
				ImGui::Spacing();

				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Text("艺术馆物品总数量：%d", formIds.size());
				for (const auto& pair : formIdsR) {
					ImGui::Text(ICON_MDI_HOME " %s：%d(%d)", roomNames[pair.first].c_str(), pair.second.size(), displayIdsR[pair.first].size());
				}
				ImGui::EndChild();
			}
		}
	}
}
