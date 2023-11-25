#include "menu_items.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/player.h>
#include <memory/stat.h>
#include <menu/lotd.h>
#include <menu/menu.h>
#include <menu/menu_track.h>
#include <setting/i18n.h>
#include <setting/setting.h>
#include <utils/utils.h>

namespace menu
{
	void __fastcall buildItemCONTInfo(int count, ItemInfoCONT* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		int columnCount = 4;

		if (show_items_window_direction) {
			columnCount++;
		}
		if (show_items_window_refid) {
			columnCount++;
		}
		if (show_items_window_formid) {
			columnCount++;
		}
		columnCount++;

		if (show_items_window_file) {
			columnCount++;
		}

		float tableHeight = TEXT_BASE_HEIGHT * show_inv_window_height;
		if (show_items_window_settings && show_items_window_auto_cont) {
			if (show_items_window_auto_conttype) {
				tableHeight = TEXT_BASE_HEIGHT * (((float)show_inv_window_height) / 2 + 0.5f);
			} else {
				tableHeight = TEXT_BASE_HEIGHT * ((float)show_inv_window_height - 1.5f);
			}
		}

		if (ImGui::BeginTable("tableItemCONT", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, tableHeight), 0.0f)) {
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 60 * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-type"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-num"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-orientation"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_7);
			}

			ImGui::TableSetupColumn(I18Nc("finder.ui.column-owner"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_9);
			if (show_items_window_refid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-reffid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_8);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_4);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-modname"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_6);
			}

			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_5);

			/*if (show_items_window_settings) {
			}*/

			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			for (int row_n = 0; row_n < count; row_n++) {
				ItemInfoCONT& item = items[row_n];
				ImGui::PushID(item.formId + 0x1000000);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (item.invCount > 0) {
					bool openFlag;
					if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s", item.name.c_str());
					} else {
						if (show_items_window_auto_conttype) {
							if (item.isAuto) {
								openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s" ICON_MDI_AUTORENEW, item.name.c_str());
							} else {
								openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s", item.name.c_str());
							}
						} else {
							openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s" ICON_MDI_AUTORENEW, item.name.c_str());
						}
					}

					if (item.isCrime) {
						ImGui::PopStyleColor();
					} else {
					}

					if (openFlag) {
						for (int i2 = 0; i2 < item.invCount; i2++) {
							auto& inv = item.invs[i2];
							char buf[80];

							if (inv.count > 1) {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH " (%d)" : "%s (%d)", inv.name.c_str(), inv.count);
							} else {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH : "%s", inv.name.c_str());
							}

							if (inv.isCrime) {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									/*auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kSteal, 0, player);*/

									RemoveItemCONT(nullptr, item.ptr, inv.ptr, inv.count, false);
								}
								ImGui::PopStyleColor();
							} else {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									/*auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kRemove, 0, player);*/

									RemoveItemCONT(nullptr, item.ptr, inv.ptr, inv.count, false);
								}
								ImGui::PopStyleColor();
							}
						}
						ImGui::TreePop();
					}

				} else {
					myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), I18Nc("finder.ui.cell-null"), item.name.c_str());
				}

				ImGui::TableNextColumn();
				switch (item.lockLevel) {
				case RE::LOCK_LEVEL::kVeryEasy:
					ImGui::Text(I18Nc("finder.ui.cell-lockVeryEasy"));
					break;
				case RE::LOCK_LEVEL::kEasy:
					ImGui::Text(I18Nc("finder.ui.cell-lockEasy"));
					break;
				case RE::LOCK_LEVEL::kAverage:
					ImGui::Text(I18Nc("finder.ui.cell-lockAverage"));
					break;
				case RE::LOCK_LEVEL::kHard:
					ImGui::Text(I18Nc("finder.ui.cell-lockHard"));
					break;
				case RE::LOCK_LEVEL::kVeryHard:
					ImGui::Text(I18Nc("finder.ui.cell-lockVeryHard"));
					break;
				case RE::LOCK_LEVEL::kRequiresKey:
					ImGui::Text(I18Nc("finder.ui.cell-lockRequiresKey"));
					break;
				case RE::LOCK_LEVEL::kUnlocked:
				default:
					ImGui::Text(I18Nc("finder.ui.cell-lockUnlocked"));
					break;
				}

				ImGui::TableNextColumn();
				ImGui::Text("%d", item.invCount);

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

				ImGui::TableNextColumn();
				ImGui::Text("%s", item.ownerName.c_str());

				if (show_items_window_refid) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.formIdStr.c_str());
				}

				if (show_items_window_formid) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.baseFormIdStr.c_str());
				}

				if (show_items_window_file) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.filename.c_str());
				}

				ImGui::TableNextColumn();
				if (show_items_window_settings) {
					if (ImGui::SmallButton("\uf101")) {
						std::string commandStr = "player.moveto ";
						commandStr.append(item.formIdStr);
						ScriptUtil::ExecuteCommand(commandStr);
						if (activeItems) {
							activeItems = false;
						}
					}

					if (show_items_window_auto_cont && show_items_window_auto_conttype) {
						if (!item.isAuto) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							if (ImGui::SmallButton(ICON_MDI_AUTORENEW)) {
								bool exist = false;
								for (const auto& includeForm : autoContForms) {
									if (includeForm.formId == item.baseFormId) {
										exist = true;
										break;
									}
								}
								if (!exist) {
									autoContForms.push_back({ item.baseFormId, item.name, item.formTypeStr });
								}
								autoContFormIds.insert(item.baseFormId);
							}
						}
					}
				}

				if (trackPtrs2.find(item.ptr) == trackPtrs2.end()) {
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					}
					if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
						TrackItem trackItem;
						trackItem.name = item.name;
						trackPtrs2.insert(std::make_pair(item.ptr, trackItem));
						tintTrack(item.ptr);
						menu::isTrack = true;
					}
				}

				ImGui::PopID();
			}
			ImGui::EndTable();
		}

		if (show_items_window_settings && show_items_window_auto_cont) {
			if (show_items_window_auto_conttype) {
				float tableHeight2 = TEXT_BASE_HEIGHT * (((float)show_inv_window_height / 2) - 2.0f);
				ImGui::Checkbox(ICON_MDI_AUTORENEW " 容器类型过滤(点击" ICON_MDI_AUTORENEW "按钮添加白名单)", &show_items_window_auto_conttype);

				int columnCount2 = 3;
				if (ImGui::BeginTable("tableItemCONTauto", columnCount2, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, tableHeight2), 0.0f)) {
					ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
					ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 60.0f * ImGui::GetIO().FontGlobalScale, TableColumn_2);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);

					ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
					ImGui::TableHeadersRow();

					int deleteFormId = 0;

					ImGuiListClipper clipper;
					clipper.Begin(autoContForms.size());
					while (clipper.Step())
						for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
							IncludeForm& item = autoContForms[row_n];
							ImGui::PushID(item.formId + 0x2000000);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("%08X", item.formId);
							ImGui::TableNextColumn();
							ImGui::Text("%s", item.name.c_str());
							ImGui::TableNextColumn();
							//ImGui::Text("%s", item.formTypeStr.c_str());

							if (ImGui::SmallButton(ICON_MDI_CLOSE)) {
								deleteFormId = item.formId;
							}

							ImGui::PopID();
						}
					ImGui::EndTable();
					if (deleteFormId) {
						autoContFormIds.erase(deleteFormId);
						autoContForms.erase(std::remove_if(autoContForms.begin(), autoContForms.end(),
												[&deleteFormId](const IncludeForm& x) {  // 这种位置写错后报错很难追溯，做个标记以后找解决方法
													return x.formId == deleteFormId;
												}),
							autoContForms.end());
					}
				}
			} else {
				ImGui::Checkbox(ICON_MDI_AUTORENEW " 容器类型过滤", &show_items_window_auto_conttype);
			}
		}
	}

	void __fastcall buildItemACHRInfo(int count, ItemInfoCONT* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		int columnCount = 2;

		if (show_items_window_direction) {
			columnCount++;
		}
		if (show_items_window_formid) {
			columnCount++;
		}
		columnCount++;

		if (ImGui::BeginTable("tableItemACHR", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
			//ImGui::TableSetupColumn(I18Nc("finder.ui.column-type"), ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-num"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-orientation"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_7);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_4);
			}

			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_5);
			/*if (show_items_window_settings) {
			}*/

			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			for (int row_n = 0; row_n < count; row_n++) {
				ItemInfoCONT& item = items[row_n];
				ImGui::PushID(item.formId + 0x1000000);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (item.invCount > 0) {
					if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					} else {
					}
					auto openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s", item.name.c_str());

					if (item.isCrime) {
						ImGui::PopStyleColor();
					} else {
					}

					if (openFlag) {
						for (int i2 = 0; i2 < item.invCount; i2++) {
							auto& inv = item.invs[i2];
							char buf[80];

							if (inv.count > 1) {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH " (%d)" : "%s (%d)", inv.name.c_str(), inv.count);
							} else {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH : "%s", inv.name.c_str());
							}

							if (inv.isCrime) {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									//auto player = RE::PlayerCharacter::GetSingleton();
									//item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kSteal, 0, player);
									RemoveItemACHR(nullptr, item.ptr, inv.ptr, inv.count, false);
								}
								ImGui::PopStyleColor();
							} else {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									/*auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kRemove, 0, player);*/
									RemoveItemACHR(nullptr, item.ptr, inv.ptr, inv.count, false);
								}
								ImGui::PopStyleColor();
							}
						}
						ImGui::TreePop();
					}

				} else {
					myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), I18Nc("finder.ui.cell-null"), item.name.c_str());
				}

				ImGui::TableNextColumn();
				ImGui::Text("%d", item.invCount);

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

				if (show_items_window_formid) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.baseFormIdStr.c_str());
				}

				ImGui::TableNextColumn();
				if (show_items_window_settings) {
					if (ImGui::SmallButton("\uf101")) {
						std::string commandStr = "player.moveto ";
						commandStr.append(item.formIdStr);
						ScriptUtil::ExecuteCommand(commandStr);
						if (activeItems) {
							activeItems = false;
						}
					}
				}

				if (trackPtrs2.find(item.ptr) == trackPtrs2.end()) {
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					}
					if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
						TrackItem trackItem;
						trackItem.name = item.name;
						trackPtrs2.insert(std::make_pair(item.ptr, trackItem));
						tintTrack(item.ptr);
						menu::isTrack = true;
					}
				}

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}

	void __fastcall buildItemInfoBOOK(int count, ItemInfoBOOK* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 3;

		if (show_items_window_refid) {
			columnCount++;
		}

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

		if (ImGui::BeginTable(("tableItem3" + std::to_string((int)formType)).c_str(), columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-type"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_5);
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-value"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 35.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
			//ImGui::TableSetupColumn(I18Nc("finder.ui.column-weight"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-orientation"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_8);
			}

			if (show_items_window_refid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-reffid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_9);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_4);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-modname"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_7);
			}
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_6);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					ItemInfoBOOK& item = items[row_n];
					ImGui::PushID(item.formId + 0x1000000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						if (ImGui::Selectable(item.isRead ? (item.name + " " + ICON_MDI_EYE).c_str() : item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								//if (!item.isDeleted) {
								if (!item.ptr->IsMarkedForDeletion()) {
									addItem(nullptr, item.ptr, 1);
									//item.isDeleted = true;
								}
								//}
							}
						}
						ImGui::PopStyleColor();
					} else {
						if (ImGui::Selectable(item.isRead ? (item.name + " " + ICON_MDI_EYE).c_str() : item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								//if (!item.isDeleted) {
								if (!item.ptr->IsMarkedForDeletion()) {
									//auto player = RE::PlayerCharacter::GetSingleton();
									addItem(nullptr, item.ptr, 1);
									//item.isDeleted = true;
								}
								//}
							}
						}
					}

					ImGui::TableNextColumn();
					ImGui::Text("%s", item.formTypeStr.c_str());

					ImGui::TableNextColumn();
					ImGui::Text("%d", item.gold);

					/*ImGui::TableNextColumn();
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
						ImGui::Text("%s", item.formIdStr.c_str());
					}
					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.baseFormIdStr.c_str());
					}
					if (show_items_window_file) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.filename.c_str());
					}

					ImGui::TableNextColumn();
					if (show_items_window_settings) {
						if (ImGui::SmallButton(ICON_MDI_EYE_REMOVE_OUTLINE)) {
							bool exist = false;
							for (const auto& excludeForm : excludeForms) {
								if (excludeForm.formId == item.baseFormId) {
									exist = true;
									break;
								}
							}
							if (!exist) {
								excludeForms.push_back({ item.baseFormId, item.name, item.formTypeStr });
							}
							excludeFormIds.insert(item.baseFormId);
						}

						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton("\uf101")) {
							/*	char buf[80];
							snprintf(buf, 80, "%0.2f, %0.2f, %0.2f", x * screenWidth, y * screenHeight, z);
							RE::DebugNotification(buf, NULL, false);*/

							//return;
							std::string commandStr = "player.moveto ";
							commandStr.append(item.formIdStr);

							ScriptUtil::ExecuteCommand(commandStr);
							if (activeItems) {
								activeItems = false;
							}
						}
					}

					if (trackPtrs2.find(item.ptr) == trackPtrs2.end()) {
						if (show_items_window_settings) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						}
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
							TrackItem trackItem;
							trackItem.name = item.name;
							trackPtrs2.insert(std::make_pair(item.ptr, trackItem));
							tintTrack(item.ptr);
							menu::isTrack = true;
						}
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
		}
	}

	void __fastcall buildItemInfoACTI(int count, ItemInfo* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 2;

		if (show_items_window_refid) {
			columnCount++;
		}

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

		if (ImGui::BeginTable("tableItemActi", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);

			ImGui::TableSetupColumn(I18Nc("finder.ui.column-type"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_5);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-orientation"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_8);
			}

			if (show_items_window_refid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-reffid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_9);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_4);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-modname"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_7);
			}
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_6);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					ItemInfo& item = items[row_n];
					ImGui::PushID(item.formId + 0x1000000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if (item.isHarvested) {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), I18Nc("finder.ui.cell-depleted"), item.name.c_str());
					}

					else if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						if (ImGui::Selectable(item.isEnchanted ? (item.name + ICON_MDI_FLASH).c_str() : item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								char buf[120];
								snprintf(buf, 120, "%X.Activate player", item.formId);
								ScriptUtil::ExecuteCommand(buf);
							}
						}
						ImGui::PopStyleColor();
					} else {
						if (ImGui::Selectable(item.isEnchanted ? (item.name + ICON_MDI_FLASH).c_str() : item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								/*std::uint32_t formFlags = item.ptr->formFlags;

								auto acti = item.ptr->GetBaseObject()->As<RE::TESObjectACTI>();
								if (acti) {
									std::uint16_t flags = acti->flags.underlying();
									auto soundActivate = acti->soundActivate;

									auto iswater = item.ptr->IsWater();
									iswater = acti->IsWater();
									auto waterForm = acti->waterForm;
									acti->GetWaterType();
								}

								auto linkedRef = item.ptr->GetLinkedRef(nullptr);
								if (linkedRef) {
									RE::FormType formtype = linkedRef->formType.get();
									auto baseobj = linkedRef->GetBaseObject();
									if (baseobj) {
										formtype = baseobj->formType.get();
										int i = 1;
									}
									int i = 1;
								}*/

								char buf[120];
								snprintf(buf, 120, "%X.Activate player", item.formId);
								ScriptUtil::ExecuteCommand(buf);
							}
						}
					}

					ImGui::TableNextColumn();
					ImGui::Text("%s", item.formTypeStr.c_str());

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
						ImGui::Text("%s", item.formIdStr.c_str());
					}
					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.baseFormIdStr.c_str());
					}
					if (show_items_window_file) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.filename.c_str());
					}

					ImGui::TableNextColumn();
					if (show_items_window_settings) {
						if (ImGui::SmallButton(ICON_MDI_EYE_REMOVE_OUTLINE)) {
							bool exist = false;
							for (const auto& excludeForm : excludeForms) {
								if (excludeForm.formId == item.baseFormId) {
									exist = true;
									break;
								}
							}
							if (!exist) {
								excludeForms.push_back({ item.baseFormId, item.name, item.formTypeStr });
							}
							excludeFormIds.insert(item.baseFormId);
						}

						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton("\uf101")) {
							std::string commandStr = "player.moveto ";
							commandStr.append(item.formIdStr);
							ScriptUtil::ExecuteCommand(commandStr);
							if (activeItems) {
								activeItems = false;
							}
						}
					}

					if (!item.isHarvested) {
						if (trackPtrs2.find(item.ptr) == trackPtrs2.end()) {
							if (show_items_window_settings) {
								ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							}
							if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
								TrackItem trackItem;
								trackItem.name = item.name;
								trackPtrs2.insert(std::make_pair(item.ptr, trackItem));
								tintTrack(item.ptr);
								menu::isTrack = true;
							}
						}
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
		}
	}

	void __fastcall buildItemInfo(int count, ItemInfo* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 3;
		switch (formType) {
		case RE::FormType::Flora:
		case RE::FormType::Tree:
		case RE::FormType::KeyMaster:
			columnCount = 1;
			break;
		case RE::FormType::Ammo:
			columnCount = 2;
			break;
		case RE::FormType::None:
			columnCount = 4;
			break;
		default:
			break;
		}

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

		if (ImGui::BeginTable(("tableItem3" + std::to_string((int)formType)).c_str(), columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			//ImGui::TableSetupColumn("已装备", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_ID);
			ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
			switch (formType) {
			case RE::FormType::Flora:
			case RE::FormType::Tree:
			case RE::FormType::KeyMaster:
				break;
			default:
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-value"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 35.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
				break;
			}
			switch (formType) {
			case RE::FormType::Flora:
			case RE::FormType::Tree:
			case RE::FormType::Ammo:
			case RE::FormType::None:
			case RE::FormType::KeyMaster:
				break;
			default:
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-weight"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);
				break;
			}
			switch (formType) {
			case RE::FormType::Container:
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-num"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
				break;
			default:
				break;
			}

			if (show_items_window_direction) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-orientation"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_8);
			}

			switch (formType) {
			case RE::FormType::None:
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-type"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_5);
				break;
			default:
				break;
			}

			if (show_items_window_refid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-reffid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_9);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_4);
			}
			if (show_items_window_file) {
				ImGui::TableSetupColumn(I18Nc("finder.ui.column-modname"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_7);
			}
			//if (show_items_window_settings) {
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_6);
			//}
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					ItemInfo& item = items[row_n];
					ImGui::PushID(item.formId + 0x1000000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if (item.isHarvested && (formType == RE::FormType::Flora || formType == RE::FormType::Tree)) {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), I18Nc("finder.ui.cell-harvested"), item.name.c_str());
					}

					else if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						if (ImGui::Selectable(item.isEnchanted ? (item.name + ICON_MDI_FLASH).c_str() : item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								if (formType == RE::FormType::Flora) {
									if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
										auto flora = item.ptr->GetBaseObject()->As<RE::TESFlora>();
										if (flora) {
											auto player = RE::PlayerCharacter::GetSingleton();
											flora->Activate(item.ptr, player, 0, flora->produceItem, 1);
										}
									}
								} else if (formType == RE::FormType::Tree) {
									if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
										auto tree = item.ptr->GetBaseObject()->As<RE::TESObjectTREE>();
										if (tree) {
											auto player = RE::PlayerCharacter::GetSingleton();
											tree->Activate(item.ptr, player, 0, tree->produceItem, 1);
										}
									}
								} else {
									if (!item.ptr->IsMarkedForDeletion()) {
										//auto player = RE::PlayerCharacter::GetSingleton();
										//player->StealAlarm(item.ptr, item.ptr->GetObjectReference(), 1, item.ptr->GetGoldValue() * 2, item.ptr->GetOwner(), true);
										addItem(nullptr, item.ptr, 1);
									}
								}
							}
						}
						ImGui::PopStyleColor();
					} else {
						if (ImGui::Selectable(item.isEnchanted ? (item.name + ICON_MDI_FLASH).c_str() : item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr) {
								if (formType == RE::FormType::Flora) {
									if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
										auto flora = item.ptr->GetBaseObject()->As<RE::TESFlora>();
										if (flora) {
											auto player = RE::PlayerCharacter::GetSingleton();
											flora->Activate(item.ptr, player, 0, flora->produceItem, 1);
										}
									}
								} else if (formType == RE::FormType::Tree) {
									if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
										auto tree = item.ptr->GetBaseObject()->As<RE::TESObjectTREE>();
										if (tree) {
											auto player = RE::PlayerCharacter::GetSingleton();
											tree->Activate(item.ptr, player, 0, tree->produceItem, 1);
										}
									}
								} else {
									if (!item.ptr->IsMarkedForDeletion()) {
										addItem(nullptr, item.ptr, 1);
									}
								}
							}

							/*			logger::debug(
								std::to_string(item.ptr->IsMarkedForDeletion()) 
								+ " - " + std::to_string(item.ptr->IsDeleted()) 
								+ " - " + std::to_string(item.ptr->IsDisabled()) 
								+ " - " + std::to_string(item.ptr->Is3rdPersonVisible()) 
								+ " - " + std::to_string(item.ptr->IsBoundObject()) 
								+ " - " + std::to_string(item.ptr->IsInitiallyDisabled()) 
							);*/
						}
					}
					switch (formType) {
					case RE::FormType::Flora:
					case RE::FormType::Tree:
					case RE::FormType::KeyMaster:
						break;
					default:
						ImGui::TableNextColumn();
						ImGui::Text("%d", item.gold);
						break;
					}
					switch (formType) {
					case RE::FormType::Flora:
					case RE::FormType::Tree:
					case RE::FormType::KeyMaster:
					case RE::FormType::Ammo:
					case RE::FormType::None:
						break;
					default:
						ImGui::TableNextColumn();
						ImGui::Text("%.1f", item.weight);
						break;
					}

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

					switch (formType) {
					case RE::FormType::None:
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.formTypeStr.c_str());
						break;
					default:
						break;
					}

					if (show_items_window_refid) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.formIdStr.c_str());
					}
					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.baseFormIdStr.c_str());
					}
					if (show_items_window_file) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.filename.c_str());
					}

					ImGui::TableNextColumn();
					if (show_items_window_settings) {
						if (ImGui::SmallButton(ICON_MDI_EYE_REMOVE_OUTLINE)) {
							bool exist = false;
							for (const auto& excludeForm : excludeForms) {
								if (excludeForm.formId == item.baseFormId) {
									exist = true;
									break;
								}
							}
							if (!exist) {
								excludeForms.push_back({ item.baseFormId, item.name, item.formTypeStr });
							}
							excludeFormIds.insert(item.baseFormId);
						}

						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton("\uf101")) {
							std::string commandStr = "player.moveto ";
							commandStr.append(item.formIdStr);
							ScriptUtil::ExecuteCommand(commandStr);
							if (activeItems) {
								activeItems = false;
							}
						}
					}

					if (trackPtrs2.find(item.ptr) == trackPtrs2.end()) {
						if (show_items_window_settings) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						}
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
							TrackItem trackItem;
							trackItem.name = item.name;
							trackPtrs2.insert(std::make_pair(item.ptr, trackItem));
							tintTrack(item.ptr);
							menu::isTrack = true;
						}
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
		}
	}

	void __fastcall takeAllItem(int count, ItemInfo* items, RE::FormType formType)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (formType == RE::FormType::Flora) {
			for (int i = 0; i < count; i++) {
				ItemInfo& item = items[i];
				if (!item.isCrime) {
					if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
						auto flora = item.ptr->GetBaseObject()->As<RE::TESFlora>();
						if (flora) {
							flora->Activate(item.ptr, player, 0, flora->produceItem, 1);
						}
					}
				}
			}
		} else if (formType == RE::FormType::Tree) {
			for (int i = 0; i < count; i++) {
				ItemInfo& item = items[i];
				if (!item.isCrime) {
					if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
						auto tree = item.ptr->GetBaseObject()->As<RE::TESObjectTREE>();
						if (tree) {
							tree->Activate(item.ptr, player, 0, tree->produceItem, 1);
						}
					}
				}
			}
		} else {
			for (int i = 0; i < count; i++) {
				ItemInfo& item = items[i];
				if (!item.isCrime) {
					if (item.ptr) {
						//if (!item.isDeleted) {
						if (!item.ptr->IsMarkedForDeletion()) {
							addItem(nullptr, item.ptr, 1);
							//item.isDeleted = true;
						}
						//}
					}
				}
			}
		}
	}

	void __fastcall takeAllItemBOOK(int count, ItemInfoBOOK* items, RE::FormType formType)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		for (int i = 0; i < count; i++) {
			ItemInfoBOOK& item = items[i];
			if (!item.isCrime) {
				if (item.ptr) {
					if (!item.ptr->IsMarkedForDeletion()) {
						addItem(nullptr, item.ptr, 1);
					}
				}
			}
		}
	}

	void renderItems()
	{
		if (activeItems) {
			ImGuiWindowFlags window_flags2 = 0;
			if (no_titlebar)
				window_flags2 |= ImGuiWindowFlags_NoTitleBar;
			if (no_resize)
				window_flags2 |= ImGuiWindowFlags_NoResize;
			if (no_collapse)
				window_flags2 |= ImGuiWindowFlags_NoCollapse;
			if (no_background_items)
				window_flags2 |= ImGuiWindowFlags_NoBackground;
			if (auto_resize)
				window_flags2 |= ImGuiWindowFlags_AlwaysAutoResize;

			ImGui::Begin("附近物品信息", nullptr, window_flags2);
			ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS " %s", playerInfo.location.c_str());
			if (show_items_window_formid) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				ImGui::Text("[%08X]", playerInfo.locationId);
				if (playerInfo.parentLocationId != -1) {
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::Text(" - %s", playerInfo.parentLocation.c_str());
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::Text("[%08X]", playerInfo.parentLocationId);
				}
			}

			/*if (lotd::isLoad) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x * 3);
				if (lotd::isShow) {
					if (ImGui::SmallButton("艺术馆模式")) {
						lotd::isShow = !lotd::isShow;
					}

				} else {
					if (ImGui::SmallButton("普通模式")) {
						lotd::isShow = !lotd::isShow;
					}
				}
			}*/

			{
				std::lock_guard<std::mutex> lock(mtxTrack);
				if (trackPtrs2.size() > 0 || trackActorPtrs2.size() > 0) {
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					if (ImGui::SmallButton(I18Nc("finder.ui.btn-deleteMarker"))) {
						for (auto& item : trackPtrs2) {
							tintTrackClose(item.first);
						}
						trackPtrs2.clear();
						trackActorPtrs2.clear();
					}
				}
			}

			if (ImGui::BeginTable("tableItem", 5)) {
				//if (lotd::isShow) {
				//lotd::render();

				//} else {
				/*	{
						ImGui::TableNextColumn();

						char buff[20];
						if (ImGui::BeginTable("loadsave", 3)) {
							for (int i = 0; i < 8; i++) {
								ImGui::TableNextColumn();
								ImGui::Text(ICON_MDI_HOME "%d", i);

								ImGui::TableNextColumn();
								snprintf(buff, 20, ICON_MDI_UPLOAD " 写入 %d", i);
								if (ImGui::Button(buff)) {
									equipments[i].clear();
									auto player = RE::PlayerCharacter::GetSingleton();
									const auto inv = player->GetInventory([](RE::TESBoundObject& a_object) {
										return a_object.IsArmor();
									});
									for (const auto& [item, invData] : inv) {
										const auto& [count, entry] = invData;
										if (count > 0 && entry->IsWorn()) {
											const auto armor = item->As<RE::TESObjectARMO>();
											EquipmentItem item2;
											item2.item = item;
											item2.slotMask = (int)armor->GetSlotMask();
											equipments[i].push_back(item2);
										}
									}
								}

								ImGui::TableNextColumn();

								snprintf(buff, 20, ICON_MDI_DOWNLOAD " 读取 %d", i);
								if (ImGui::Button(buff)) {
									std::vector<EquipmentItem> copiedVector(equipments[i]);
									auto player = RE::PlayerCharacter::GetSingleton();
									auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
									const auto inv = player->GetInventory([](RE::TESBoundObject& a_object) {
										return a_object.IsArmor();
									});

									for (const auto& [item, invData] : inv) {
										const auto& [count, entry] = invData;
										if (count > 0 && entry->IsWorn()) {
											const auto armor = item->As<RE::TESObjectARMO>();
											actorEquipManager->UnequipObject(player, item);
										}
									}

									for (const auto& [item, invData] : inv) {
										const auto& [count, entry] = invData;
										if (count > 0 && !entry->IsWorn()) {
											const auto armor = item->As<RE::TESObjectARMO>();

											RE::TESBoundObject* deletePtr = nullptr;
											for (const auto& item2 : copiedVector) {
												if (item2.item == item) {
													actorEquipManager->EquipObject(player, item);
													deletePtr = item;
													break;
												}
											}
											if (deletePtr) {
												copiedVector.erase(std::remove_if(copiedVector.begin(), copiedVector.end(),
																	   [&deletePtr](const EquipmentItem& x) {
																		   return x.item == deletePtr;
																	   }),
													copiedVector.end());
											}
										}
									}
								}
							}

							ImGui::EndTable();
						}
					}*/

				if (lotd::isLoad && lotd::isShowAttached) {
					if (lotd::getCountAttached()) {
						ImGui::TableNextColumn();
						ImGui::AlignTextToFramePadding();

						ImGui::Text(I18Ni(ICON_MDI_GREENHOUSE, "finder.ui.label-lotdCollection"), lotd::getCountAttached());
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS "##99")) {
							trackAllItem(lotd::getCountAttached(), lotd::getItemsAttached());
						}
						lotd::buildItemInfoAttached(lotd::getCountAttached(), lotd::getItemsAttached());
						ImGui::Spacing();
					}
				}

				if (getItemCountWEAP() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_SWORD, "finder.ui.label-weap"), getItemCountWEAP());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##41")) {
							takeAllItem(getItemCountWEAP(), getItemsWEAP(), RE::FormType::Weapon);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##41", &show_items_window_auto_weap);
					} else {
						if (show_items_window_auto_weap) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountWEAP(), getItemsWEAP(), RE::FormType::Weapon);
					ImGui::Spacing();
				}
				if (getItemCountARMO() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_SHIELD, "finder.ui.label-armo"), getItemCountARMO());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##47")) {
							takeAllItem(getItemCountARMO(), getItemsARMO(), RE::FormType::Armor);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##26", &show_items_window_auto_armo);
					} else {
						if (show_items_window_auto_armo) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountARMO(), getItemsARMO(), RE::FormType::Armor);
					ImGui::Spacing();
				}
				if (getItemCountAMMO() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_ARROW_PROJECTILE, "finder.ui.label-ammo"), getItemCountAMMO());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##42")) {
							takeAllItem(getItemCountAMMO(), getItemsAMMO(), RE::FormType::Ammo);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##47", &show_items_window_auto_ammo);
					} else {
						if (show_items_window_auto_ammo) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountAMMO(), getItemsAMMO(), RE::FormType::Ammo);
					ImGui::Spacing();
				}
				if (getItemCountBOOK() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_BOOK_OPEN_VARIANT, "finder.ui.label-book"), getItemCountBOOK());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##43")) {
							takeAllItemBOOK(getItemCountBOOK(), getItemsBOOK(), RE::FormType::Book);
						}
					}
					buildItemInfoBOOK(getItemCountBOOK(), getItemsBOOK(), RE::FormType::Book);
				}
				if (getItemCountALCH() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE, "finder.ui.label-alch"), getItemCountALCH());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##44")) {
							takeAllItem(getItemCountALCH(), getItemsALCH(), RE::FormType::AlchemyItem);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##44", &show_items_window_auto_alch);
					} else {
						if (show_items_window_auto_alch) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountALCH(), getItemsALCH(), RE::FormType::AlchemyItem);
					ImGui::Spacing();
				}
				if (getItemCountFOOD() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_FOOD_DRUMSTICK, "finder.ui.label-food"), getItemCountFOOD());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##45")) {
							takeAllItem(getItemCountFOOD(), getItemsFOOD(), RE::FormType::PluginInfo);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##45", &show_items_window_auto_food);
					} else {
						if (show_items_window_auto_food) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountFOOD(), getItemsFOOD(), RE::FormType::PluginInfo);  // 临时用PluginInfo
					ImGui::Spacing();
				}
				if (getItemCountINGR() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_SOURCE_BRANCH, "finder.ui.label-ingr"), getItemCountINGR());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##48")) {
							takeAllItem(getItemCountINGR(), getItemsINGR(), RE::FormType::Ingredient);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##48", &show_items_window_auto_ingr);
					} else {
						if (show_items_window_auto_ingr) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountINGR(), getItemsINGR(), RE::FormType::Ingredient);
					ImGui::Spacing();
				}
				if (getItemCountSGEM() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_CARDS_DIAMOND, "finder.ui.label-sgen"), getItemCountSGEM());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##53")) {
							takeAllItem(getItemCountSGEM(), getItemsSGEM(), RE::FormType::SoulGem);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##53", &show_items_window_auto_sgem);
					} else {
						if (show_items_window_auto_sgem) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountSGEM(), getItemsSGEM(), RE::FormType::SoulGem);
				}
				if (getItemCountKEYM() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_KEY, "finder.ui.label-keym"), getItemCountKEYM());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##52")) {
							takeAllItem(getItemCountKEYM(), getItemsKEYM(), RE::FormType::KeyMaster);
						}
					}
					buildItemInfo(getItemCountKEYM(), getItemsKEYM(), RE::FormType::KeyMaster);
					ImGui::Spacing();
				}
				if (getItemCountSTON() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_DIAMOND_STONE, "finder.ui.label-ston"), getItemCountSTON());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##32-1")) {
							takeAllItem(getItemCountSTON(), getItemsSTON(), RE::FormType::Misc);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##32-1", &show_items_window_auto_ston);
					} else {
						if (show_items_window_auto_ston) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountSTON(), getItemsSTON(), RE::FormType::Misc);
					ImGui::Spacing();
				}
				if (getItemCountANVI() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_ANVIL, "finder.ui.label-anvi"), getItemCountANVI());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##32-2")) {
							takeAllItem(getItemCountANVI(), getItemsANVI(), RE::FormType::Misc);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##32-2", &show_items_window_auto_anvi);
					} else {
						if (show_items_window_auto_anvi) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountANVI(), getItemsANVI(), RE::FormType::Misc);
					ImGui::Spacing();
				}
				if (getItemCountANHD() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_BOX_CUTTER, "finder.ui.label-anhd"), getItemCountANHD());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##32-3")) {
							takeAllItem(getItemCountANHD(), getItemsANHD(), RE::FormType::Misc);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##32-3", &show_items_window_auto_anhd);
					} else {
						if (show_items_window_auto_anhd) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountANHD(), getItemsANHD(), RE::FormType::Misc);
					ImGui::Spacing();
				}
				if (getItemCountANPA() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_RABBIT, "finder.ui.label-anpa"), getItemCountANPA());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##32-4")) {
							takeAllItem(getItemCountANPA(), getItemsANPA(), RE::FormType::Misc);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##32-4", &show_items_window_auto_anpa);
					} else {
						if (show_items_window_auto_anpa) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountANPA(), getItemsANPA(), RE::FormType::Misc);
					ImGui::Spacing();
				}
				if (getItemCountTOOL() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_TOOLS, "finder.ui.label-tool"), getItemCountTOOL());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##32-5")) {
							takeAllItem(getItemCountTOOL(), getItemsTOOL(), RE::FormType::Misc);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##32-5", &show_items_window_auto_tool);
					} else {
						if (show_items_window_auto_tool) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountTOOL(), getItemsTOOL(), RE::FormType::Misc);
					ImGui::Spacing();
				}
				if (getItemCountMISC() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_PACKAGE_VARIANT_CLOSED, "finder.ui.label-misc"), getItemCountMISC());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##49")) {
							takeAllItem(getItemCountMISC(), getItemsMISC(), RE::FormType::Misc);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##49", &show_items_window_auto_misc);
					} else {
						if (show_items_window_auto_misc) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountMISC(), getItemsMISC(), RE::FormType::Misc);
					ImGui::Spacing();
				}
				if (getItemCountFLOR() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_BASKET_OUTLINE, "finder.ui.label-flor"), getItemCountFLOR());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##50")) {
							takeAllItem(getItemCountFLOR(), getItemsFLOR(), RE::FormType::Flora);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动收获##50", &show_items_window_auto_flor);
					} else {
						if (show_items_window_auto_flor) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountFLOR(), getItemsFLOR(), RE::FormType::Flora);
					ImGui::Spacing();
				}
				if (getItemCountTREE() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_FLOWER_TULIP_OUTLINE, "finder.ui.label-tree"), getItemCountTREE());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##53")) {
							takeAllItem(getItemCountTREE(), getItemsTREE(), RE::FormType::Tree);
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动收获##53", &show_items_window_auto_tree);
					} else {
						if (show_items_window_auto_tree) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemInfo(getItemCountTREE(), getItemsTREE(), RE::FormType::Tree);
					ImGui::Spacing();
				}
				if (getItemCountACHR() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_HUMAN_MALE, "finder.ui.label-achr"), getItemCountACHR());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##62", &show_items_window_auto_achr);
					} else {
						if (show_items_window_auto_achr) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemACHRInfo(getItemCountACHR(), getItemsACHR(), RE::FormType::ActorCharacter);
					ImGui::Spacing();
				}
				if (getItemCountCONT() > 0) {
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::Text(I18Ni(ICON_MDI_ARCHIVE_OUTLINE, "finder.ui.label-cont"), getItemCountCONT());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##28", &show_items_window_auto_cont);
					} else {
						if (show_items_window_auto_cont) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemCONTInfo(getItemCountCONT(), getItemsCONT(), RE::FormType::Container);
					ImGui::Spacing();
				}
				if (getItemCountACTI() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(I18Ni(ICON_MDI_COGS, "finder.ui.label-acti"), getItemCountACTI());

					buildItemInfoACTI(getItemCountACTI(), getItemsACTI(), RE::FormType::Activator);
					ImGui::Spacing();
				}
				if (getItemCount() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text("其他：");
					buildItemInfo(getItemCount(), getItems(), RE::FormType::None);
					ImGui::Spacing();
				}

				ImGui::TableNextColumn();
				ImGui::Checkbox("更多", &show_items_window_settings);
				if (show_items_window_settings) {
					ImGui::SameLine(0.0f, 8.0f * ImGui::GetTextLineHeightWithSpacing());

					if (ImGui::Button(ICON_MDI_CONTENT_SAVE " 保存设置##2")) {
						setting::save_settings();
					}

					{
						ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

						ImGui::BeginChild("childItemsSetting", ImVec2(ImGui::GetTextLineHeightWithSpacing() * 15, ImGui::GetTextLineHeightWithSpacing() * ((float)show_inv_window_height - 0.5f)), true, window_flags);

						// 界面设置
						if (ImGui::TreeNodeEx(I18Ni(ICON_MDI_TABLE, "finder.setting.label-displaySetting"), ImGuiTreeNodeFlags_DefaultOpen)) {
							if (ImGui::BeginTable("tableItemsSetting", 2)) {
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-transparentBackground"), &no_background_items);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayFormid"), &show_items_window_formid);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayRefid"), &show_items_window_refid);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayPosition"), &show_items_window_direction);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayModname"), &show_items_window_file);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNo3D"), &show_items_window_3D);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-privateItemHidden"), &isCrimeIgnore);
								ImGui::TableNextColumn();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-merchantContIgnore"), &merchantContIgnore);

								ImGui::EndTable();
							}

							if (lotd::isLoad) {
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayLotdSection"), &lotd::isShowAttached);
								if (lotd::isShowAttached) {
									ImGui::Indent();
									//ImGui::Checkbox(I18Nc("finder.setting.checkbox-excludePlayerItems"), &lotd::isInvIgnore);
									ImGui::Checkbox(I18Nc("finder.setting.checkbox-lotdPrivateItemHidden"), &lotd::isCrimeIgnore);
									ImGui::Checkbox(I18Nc("finder.setting.checkbox-excludeArmoryItems"), &lotd::isArmoryIgnore);
									ImGui::Unindent();
								}
							}

							ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
							ImGui::SliderInt(I18Nc("finder.setting.slider-rangelocal"), &show_items_window_auto_dis_local, 10, 2000, "%d米");
							ImGui::SliderInt(I18Nc("finder.setting.slider-rangeskyrim"), &show_items_window_auto_dis_skyrim, 20, 10000, "%d米");
							ImGui::SliderInt(I18Nc("finder.setting.slider-tableHeight"), &show_inv_window_height, 7, 18, "%d行");
							ImGui::PopItemWidth();

							ImGui::Checkbox(I18Nc("finder.setting.checkbox-ignoreItem"), &show_items_window_ignore);
							if (show_items_window_ignore) {
								static ImGuiTableFlags flagsItem =
									ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

								const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
								if (ImGui::BeginTable("tableItemIngore", 3, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 12, TEXT_BASE_HEIGHT * 6), 0.0f)) {
									ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
									ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 60.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
									ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);

									ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
									ImGui::TableHeadersRow();

									int deleteFormId = 0;

									ImGuiListClipper clipper;
									clipper.Begin(excludeForms.size());
									while (clipper.Step())
										for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
											ExcludeForm& item = excludeForms[row_n];
											ImGui::PushID(item.formId + 0x2000000);
											ImGui::TableNextRow();
											ImGui::TableNextColumn();
											ImGui::Text("%08X", item.formId);
											ImGui::TableNextColumn();
											ImGui::Text("%s", item.name.c_str());
											ImGui::TableNextColumn();
											//ImGui::Text("%s", item.formTypeStr.c_str());

											if (ImGui::SmallButton(ICON_MDI_CLOSE)) {
												deleteFormId = item.formId;
											}

											ImGui::PopID();
										}
									ImGui::EndTable();
									if (deleteFormId) {
										excludeFormIds.erase(deleteFormId);
										excludeForms.erase(std::remove_if(excludeForms.begin(), excludeForms.end(),
															   [&deleteFormId](const ExcludeForm& x) {
																   return x.formId == deleteFormId;
															   }),
											excludeForms.end());
									}
								}
							}

							ImGui::TreePop();
						}

						ImGui::Spacing();
						if (ImGui::TreeNodeEx(I18Ni(ICON_MDI_AUTORENEW, "finder.setting.label-autoPickup"), ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
							ImGui::SliderInt(I18Nc("finder.setting.slider-pickupRange"), &show_items_window_auto_dis, 1, 100, "%d米");
							ImGui::SliderInt(I18Nc("finder.setting.slider-pickupFrequency"), &refresh_time_auto, 1, 10, "%d秒");
							ImGui::SliderInt(I18Nc("finder.setting.slider-pickupQuantity"), &show_items_window_auto_every_max, 1, 10, "%d个");
							ImGui::PopItemWidth();
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupPrompt"), &show_items_window_auto_notification);

							if (ImGui::TreeNodeEx(I18Ni(ICON_MDI_HUMAN_MALE, "finder.setting.label-achrItemPickupType"), ImGuiTreeNodeFlags_DefaultOpen)) {
								if (ImGui::BeginTable("tableItemsSettingACHR", 3)) {
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SWORD, "finder.setting.checkbox-weap"), &show_items_window_auto_achr_weap);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SHIELD_HALF_FULL, "finder.setting.checkbox-armo"), &show_items_window_auto_achr_armo);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_ARROW_PROJECTILE, "finder.setting.checkbox-ammo"), &show_items_window_auto_achr_ammo);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE, "finder.setting.checkbox-alch"), &show_items_window_auto_achr_alch);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_FOOD_DRUMSTICK, "finder.setting.checkbox-food"), &show_items_window_auto_achr_food);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SOURCE_BRANCH, "finder.setting.checkbox-ingr"), &show_items_window_auto_achr_ingr);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_CARDS_DIAMOND, "finder.setting.checkbox-sgen"), &show_items_window_auto_achr_sgem);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_CASH, "finder.setting.checkbox-gold"), &show_items_window_auto_achr_gold);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SCRIPT_TEXT, "finder.setting.checkbox-scrl"), &show_items_window_auto_achr_scrl);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_KEY, "finder.setting.checkbox-keym"), &show_items_window_auto_achr_keym);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_DIAMOND_STONE, "finder.setting.checkbox-ston"), &show_items_window_auto_achr_ston);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_ANVIL, "finder.setting.checkbox-anvi"), &show_items_window_auto_achr_anvi);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_LOCK_OPEN, "finder.setting.checkbox-lock"), &show_items_window_auto_achr_lock);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_BOX_CUTTER, "finder.setting.checkbox-anhd"), &show_items_window_auto_achr_anhd);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_RABBIT, "finder.setting.checkbox-anpa"), &show_items_window_auto_achr_anpa);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_TOOLS, "finder.setting.checkbox-tool"), &show_items_window_auto_achr_tool);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_PACKAGE_VARIANT_CLOSED, "finder.setting.checkbox-misc"), &show_items_window_auto_achr_misc);
									ImGui::EndTable();
								}

								ImGui::TreePop();
							}

							if (ImGui::TreeNodeEx(I18Ni(ICON_MDI_ARCHIVE_OUTLINE, "finder.setting.label-contItemPickupType"), ImGuiTreeNodeFlags_DefaultOpen)) {
								if (ImGui::BeginTable("tableItemsSettingCONT", 3)) {
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SWORD, "finder.setting.checkbox-weap"), &show_items_window_auto_cont_weap);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SHIELD_HALF_FULL, "finder.setting.checkbox-armo"), &show_items_window_auto_cont_armo);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_ARROW_PROJECTILE, "finder.setting.checkbox-ammo"), &show_items_window_auto_cont_ammo);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE, "finder.setting.checkbox-alch"), &show_items_window_auto_cont_alch);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_FOOD_DRUMSTICK, "finder.setting.checkbox-food"), &show_items_window_auto_cont_food);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SOURCE_BRANCH, "finder.setting.checkbox-ingr"), &show_items_window_auto_cont_ingr);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_CARDS_DIAMOND, "finder.setting.checkbox-sgen"), &show_items_window_auto_cont_sgem);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_CASH, "finder.setting.checkbox-gold"), &show_items_window_auto_cont_gold);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_SCRIPT_TEXT, "finder.setting.checkbox-scrl"), &show_items_window_auto_cont_scrl);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_KEY, "finder.setting.checkbox-keym"), &show_items_window_auto_cont_keym);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_DIAMOND_STONE, "finder.setting.checkbox-ston"), &show_items_window_auto_cont_ston);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_ANVIL, "finder.setting.checkbox-anvi"), &show_items_window_auto_cont_anvi);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_LOCK_OPEN, "finder.setting.checkbox-lock"), &show_items_window_auto_cont_lock);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_BOX_CUTTER, "finder.setting.checkbox-anhd"), &show_items_window_auto_cont_anhd);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_RABBIT, "finder.setting.checkbox-anpa"), &show_items_window_auto_cont_anpa);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_TOOLS, "finder.setting.checkbox-tool"), &show_items_window_auto_cont_tool);
									ImGui::TableNextColumn();
									ImGui::Checkbox(I18Ni(ICON_MDI_PACKAGE_VARIANT_CLOSED, "finder.setting.checkbox-misc"), &show_items_window_auto_cont_misc);

									ImGui::EndTable();
								}

								ImGui::TreePop();
							}

							if (ImGui::TreeNodeEx(ICON_MDI_SWORD " 武器过滤", ImGuiTreeNodeFlags_DefaultOpen)) {
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupOnlyEnchanting"), &show_items_window_auto_weap_enchant);
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupValueSetting"), &show_items_window_auto_weap_price);
								ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
								if (show_items_window_auto_weap_price) {
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::SliderInt("##设置价格", &show_items_window_auto_weap_price_value, 100, 10000, " 超过%d");
								}
								ImGui::PopItemWidth();

								ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupValueRadioSetting"), &show_items_window_auto_weap_priceweight);
								ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
								if (show_items_window_auto_weap_priceweight) {
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::SliderInt("##设置价重比", &show_items_window_auto_weap_priceweight_value, 2, 500, " 超过%d");
								}
								ImGui::PopItemWidth();

								ImGui::TreePop();
							}

							if (ImGui::TreeNodeEx(ICON_MDI_SHIELD_HALF_FULL " 装备过滤", ImGuiTreeNodeFlags_DefaultOpen)) {
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupOnlyEnchanting"), &show_items_window_auto_armo_enchant);

								ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupValueSetting"), &show_items_window_auto_armo_price);
								ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
								if (show_items_window_auto_armo_price) {
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::SliderInt("##设置价格2", &show_items_window_auto_armo_price_value, 100, 10000, " 超过%d");
								}
								ImGui::PopItemWidth();

								ImGui::Checkbox(I18Nc("finder.setting.checkbox-pickupValueRadioSetting"), &show_items_window_auto_armo_priceweight);
								ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
								if (show_items_window_auto_armo_priceweight) {
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::SliderInt("##设置价重比2", &show_items_window_auto_armo_priceweight_value, 2, 500, " 超过%d");
								}
								ImGui::PopItemWidth();

								ImGui::TreePop();
							}

							ImGui::Checkbox("地点过滤", &show_items_window_auto_ignore);
							if (show_items_window_auto_ignore) {
								ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
								if (ImGui::SmallButton("排除当前位置")) {
									auto player = RE::PlayerCharacter::GetSingleton();
									auto currentLocation = player->currentLocation;
									RE::FormID formid = 0;
									std::string name = "天际";
									if (currentLocation) {
										formid = currentLocation->GetFormID();
										name = currentLocation->GetFullName();
									}
									bool exist = false;
									for (const auto& excludeForm : excludeLocationForms) {
										if (excludeForm.formId == formid) {
											exist = true;
											break;
										}
									}
									if (!exist) {
										excludeLocationForms.push_back({ formid, name, "" });
									}
									excludeLocationFormIds.insert(formid);
								}
								ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
								ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS " %s", playerInfo.location.c_str());
								static ImGuiTableFlags flagsItem =
									ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

								const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
								if (ImGui::BeginTable("tableItemLocationIngore", 3, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 12, TEXT_BASE_HEIGHT * 6), 0.0f)) {
									ImGui::TableSetupColumn(I18Nc("finder.ui.column-formid"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
									ImGui::TableSetupColumn(I18Nc("finder.ui.column-name"), ImGuiTableColumnFlags_WidthFixed, 60.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
									ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);

									ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
									ImGui::TableHeadersRow();

									int deleteFormId = 0;

									ImGuiListClipper clipper;
									clipper.Begin(excludeLocationForms.size());
									while (clipper.Step())
										for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
											ExcludeForm& item = excludeLocationForms[row_n];
											ImGui::PushID(item.formId + 0x3000000);
											ImGui::TableNextRow();
											ImGui::TableNextColumn();
											ImGui::Text("%08X", item.formId);
											ImGui::TableNextColumn();
											ImGui::Text("%s", item.name.c_str());
											ImGui::TableNextColumn();
											//ImGui::Text("%s", item.formTypeStr.c_str());

											if (ImGui::SmallButton(ICON_MDI_CLOSE)) {
												deleteFormId = item.formId == 0 ? -1 : item.formId;
											}

											ImGui::PopID();
										}
									ImGui::EndTable();
									if (deleteFormId != 0) {
										if (deleteFormId == -1) {
											deleteFormId = 0;
										}
										excludeLocationFormIds.erase(deleteFormId);
										excludeLocationForms.erase(std::remove_if(excludeLocationForms.begin(), excludeLocationForms.end(),
																	   [&deleteFormId](const ExcludeForm& x) {
																		   return x.formId == deleteFormId;
																	   }),
											excludeLocationForms.end());
									}
								}
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx(I18Ni(ICON_MDI_TABLE_OF_CONTENTS, "finder.setting.label-hud"), ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyNirnRootQuantity"), &stats::showlocationNirnRootCount);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyNirnRootRedQuantity"), &stats::showlocationNirnRootRedCount);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyVeinQuantity"), &stats::showlocationOreCount);

							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyLotdItemsQuantity"), &lotd::showlocationItemCount);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyLotdExcavationQuantity"), &stats::showlocationExCount);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayLotdItemsQuantity"), &lotd::showDisplayItemCount);

							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx(I18Ni(ICON_MDI_MAP_SEARCH_OUTLINE, "finder.setting.label-marker"), ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-markerNameTag"), &show_item_window_track_icon_name);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-markerHighLight"), &show_item_window_track_highlight);
							if (lotd::isLoad) {
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-autoMarkLotdItems"), &lotd::isAutoTrackLotdItems);
								if (lotd::isAutoTrackLotdItems) {
									ImGui::Indent();
									ImGui::Checkbox(I18Nc("finder.setting.checkbox-privateItemHidden"), &lotd::isAutoTrackLotdItemsCrimeIgnore);
									ImGui::Unindent();
								}
							}
							ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
							ImGui::SliderInt(I18Nc("finder.setting.slider-markerZoom"), &show_item_window_track_icon_scale, 0, 10, "+%d");
							ImGui::PopItemWidth();

							ImGui::TreePop();
						}
						ImGui::EndChild();
					}
				}
				//}

				ImGui::EndTable();
			}

			ImGui::End();
		}
	}
}
