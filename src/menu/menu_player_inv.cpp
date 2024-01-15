#include "menu_player_inv.h"
#include <Windows.h>
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/player.h>
#include <memory/player_inv.h>
#include <menu/menu.h>

namespace menu
{
	void __cdecl RefreshInfo1(void*)
	{
		Sleep(300);
		RefreshPlayerInvInfo();
	}

	void RefreshInfo()
	{
		_beginthread(RefreshInfo1, 0, NULL);
	}

	void addFavor(InventoryInfo& item, int type)
	{
		FavorInfo info;
		info.formId = item.formId;
		info.name = item.name;
		/* info.invExtraPtr = item.invExtraPtr;
		info.invPtr = item.invPtr;
		info.ptr = item.ptr;
		info.slotMaskStr = item.slotMaskStr;
		info.slotMask = item.slotMask;
		info.AlchemyValue = item.AlchemyValue;
		info.SmithingValue = item.SmithingValue;
		info.EnchantingValue = item.EnchantingValue;
		info.isAlchemyEnt = item.isAlchemyEnt;
		info.isEnchantingEnt = item.isEnchantingEnt;
		info.isSmithingEnt = item.isSmithingEnt;*/

		auto ench = item.ptr->As<RE::TESEnchantableForm>();
		if (ench && ench->formEnchanting) {
			for (auto& effect : ench->formEnchanting->effects) {
				FavorEntDetailInfo detail;
				detail.formId = effect->baseEffect->GetFormID();
				detail.value = effect->effectItem.magnitude;
				info.entDetail.push_back(detail);
			}
		}
		if (item.invExtraPtr) {
			const auto xEnch = item.invExtraPtr->GetByType<RE::ExtraEnchantment>();
			if (xEnch && xEnch->enchantment) {
				for (auto& effect : xEnch->enchantment->effects) {
					FavorEntDetailInfo detail;
					detail.formId = effect->baseEffect->GetFormID();
					detail.value = effect->effectItem.magnitude;
					info.entDetail2.push_back(detail);
				}
			}
		}
		if (type == 1) {
			favorAlchemy.push_back(info);
		} else if (type == 2) {
			favorSmithing.push_back(info);
		}
	}

	void __fastcall buildPlayerInvInfo(int count, InventoryInfo inv[])
	{
		static ImGuiTableFlags flags =
			/*	ImGuiTableFlags_Reorderable
			| ImGuiTableFlags_Hideable*/
			ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody
			/*	| ImGuiTableFlags_NoBordersInBody
				| ImGuiTableFlags_ScrollX
				| ImGuiTableFlags_SizingFixedFit*/
			;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		if (ImGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed, 15.0f, PlayerInfoColumnID_ID);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 100.0f, PlayerInfoColumnID_1);
			//ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed, 40, PlayerInfoColumnID_3);
			//ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 100.0f, PlayerInfoColumnID_4);
			//ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_WidthFixed, 0.0f, PlayerInfoColumnID_5);
			ImGui::TableSetupColumn("uniqueId", ImGuiTableColumnFlags_WidthFixed, 40, PlayerInfoColumnID_4);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			char buff[50];
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					// Display a data item
					//InventoryInfo* item = getPlayerInvData(row_n);
					InventoryInfo& item = inv[row_n];
					//if (item) {
					ImGui::PushID(row_n + 7000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.isWorn ? "×" : "");
					ImGui::TableNextColumn();

					if (item.count > 1) {
						snprintf(buff, 50, "%s(%d)", item.name.c_str(), item.count);
					} else {
						snprintf(buff, 50, "%s", item.name.c_str());
					}
					if (ImGui::Selectable(buff, false, ImGuiSelectableFlags_AllowDoubleClick)) {
						if (item.ptr->IsAmmo() || item.ptr->IsArmor() || item.ptr->IsWeapon()) {
							auto player = RE::PlayerCharacter::GetSingleton();
							auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
							if (item.isWorn) {
								actorEquipManager->UnequipObject(player, item.ptr, item.invExtraPtr, 1);
								item.isWorn = false;
							} else {
								actorEquipManager->EquipObject(player, item.ptr, item.invExtraPtr, 1);
								item.isWorn = true;
							}
						}
					}
					ImGui::TableNextColumn();
					//if (isAim) {
					ImGui::Text("%.1f", item.weight);
					//}
					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.formIdStr.c_str());*/
					//ImGui::TableNextColumn();
					//ImGui::Text("0x%X", item->address);

					ImGui::TableNextColumn();
					ImGui::Text("%d", item.uniqueID);
					ImGui::PopID();
					//}
				}
			ImGui::EndTable();
		}
	}

	void __fastcall buildPlayerInvAlchemyInfo(std::vector<InventoryInfo>& inv, int count, int type)
	{
		char buff[50];
		static ImGuiTableFlags flags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		snprintf(buff, 50, "tablePlayerInvAlchemy %d", type);
		if (ImGui::BeginTable(buff, 5, flags, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 15.0f * ImGui::GetIO().FontGlobalScale, TableColumn_2);
			ImGui::TableSetupColumn("插槽", ImGuiTableColumnFlags_WidthFixed, 30.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 120.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);
			ImGui::TableSetupColumn("加成", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_4);
			//ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed, 40 * ImGui::GetIO().FontGlobalScale, TableColumn_5);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					InventoryInfo& item = inv[row_n];

					ImGui::PushID(row_n);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.isWorn ? "×" : "");
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.slotMaskStr.c_str());
					ImGui::TableNextColumn();
					if (ImGui::BeginPopupContextItem("itemPopMenuInvAlchemy")) {
						if (item.isFavor) {
							if (ImGui::Button(ICON_MDI_STAR_OFF " 取消收藏")) {
								if (type == 1) {
									// 再检查一下
									auto result = getFavor(item, favorAlchemy);
									if (result >= 0) {
										favorAlchemy.erase(favorAlchemy.begin() + result);
										ImGui::CloseCurrentPopup();
										RefreshInfo();
									}
								} else if (type == 2) {
									// 再检查一下
									auto result = getFavor(item, favorSmithing);
									if (result >= 0) {
										favorSmithing.erase(favorSmithing.begin() + result);
										ImGui::CloseCurrentPopup();
										RefreshInfo();
									}
								}
							}
						} else {
							if (ImGui::Button(ICON_MDI_STAR " 收藏")) {
								if (type == 1) {
									if (getFavor(item, favorAlchemy) == -1) {
										addFavor(item, type);
										ImGui::CloseCurrentPopup();
										RefreshInfo();
									}
								} else if (type == 2) {
									if (getFavor(item, favorSmithing) == -1) {
										addFavor(item, type);
										ImGui::CloseCurrentPopup();
										RefreshInfo();
									}
								}
							}
						}
						ImGui::EndPopup();
					}
					snprintf(buff, 50, "%s", item.name.c_str());
					if (ImGui::Selectable(buff, false, ImGuiSelectableFlags_AllowDoubleClick)) {
						if (item.ptr->IsAmmo() || item.ptr->IsArmor() || item.ptr->IsWeapon()) {
							auto player = RE::PlayerCharacter::GetSingleton();
							auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
							if (item.isWorn) {
								actorEquipManager->UnequipObject(player, item.ptr, item.invExtraPtr, 1);
								//item.isWorn = false;
								RefreshInfo();
							} else {
								actorEquipManager->EquipObject(player, item.ptr, item.invExtraPtr, 1);
								//item.isWorn = true;
								RefreshInfo();
							}
						}
					}
					ImGui::OpenPopupOnItemClick("itemPopMenuInvAlchemy", ImGuiPopupFlags_MouseButtonRight);

					ImGui::TableNextColumn();
					if (type == 1) {
						if (item.isBest) {
							myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.AlchemyValue);
						} else {
							myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.AlchemyValue);
						}
					} else if (type == 2) {
						if (item.isBest) {
							myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.SmithingValue);
						} else {
							myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.SmithingValue);
						}
					} else if (type == 3) {
						if (item.isBest) {
							myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.EnchantingValue);
						} else {
							myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.EnchantingValue);
						}
					}

					ImGui::TableNextColumn();
					ImGui::Text("%.1f", item.weight);
					//}
					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.formIdStr.c_str());*/
					//ImGui::TableNextColumn();
					//ImGui::Text("0x%X", item->address);

					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.debugName.c_str());*/
					ImGui::PopID();
					//}
				}
			ImGui::EndTable();
		}
	}

	void __fastcall buildPlayerInvFavorInfo(std::vector<InventoryInfo>& inv, int count, int type)
	{
		char buff[50];
		snprintf(buff, 50, "tablePlayerInvFavor %d", type);
		static ImGuiTableFlags flags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		if (ImGui::BeginTable(buff, 5, flags, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 15.0f * ImGui::GetIO().FontGlobalScale, TableColumn_2);
			ImGui::TableSetupColumn("插槽", ImGuiTableColumnFlags_WidthFixed, 30.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 120.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);
			ImGui::TableSetupColumn("加成", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_4);
			//ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_5);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			char buff[50];
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					InventoryInfo& item = inv[row_n];
					//if (item) {
					ImGui::PushID(row_n);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if (item.ptr) {
						ImGui::Text("%s", item.isWorn ? "×" : "");
					}
					ImGui::TableNextColumn();
					if (item.ptr) {
						ImGui::Text("%s", item.slotMaskStr.c_str());
					}
					ImGui::TableNextColumn();

					if (ImGui::BeginPopupContextItem("itemPopMenuInvFavor")) {
						if (ImGui::Button(ICON_MDI_STAR_OFF " 取消收藏")) {
							if (type == 1) {
								// 再检查一下
								if (item.ptr) {
									auto result = getFavor(item, favorAlchemy);
									if (result >= 0) {
										favorAlchemy.erase(favorAlchemy.begin() + result);
										ImGui::CloseCurrentPopup();
										RefreshInfo();
									}
								} else {
									auto pointerToDelete = item.favPtr;
									favorAlchemy.erase(std::remove_if(favorAlchemy.begin(), favorAlchemy.end(),
														   [pointerToDelete](const FavorInfo& info) {
															   return &info == pointerToDelete;
														   }),
										favorAlchemy.end());

									ImGui::CloseCurrentPopup();
									RefreshInfo();
								}
							} else if (type == 2) {
								if (item.ptr) {
									// 再检查一下
									auto result = getFavor(item, favorSmithing);
									if (result >= 0) {
										favorSmithing.erase(favorSmithing.begin() + result);
										ImGui::CloseCurrentPopup();
										RefreshInfo();
									}
								} else {
									auto pointerToDelete = item.favPtr;
									favorSmithing.erase(std::remove_if(favorSmithing.begin(), favorSmithing.end(),
														   [pointerToDelete](const FavorInfo& info) {
															   return &info == pointerToDelete;
														   }),
										favorSmithing.end());

									ImGui::CloseCurrentPopup();
									RefreshInfo();
								}
							}
						}
						ImGui::EndPopup();
					}
					snprintf(buff, 50, "%s", item.name.c_str());
					if (item.ptr) {
						if (ImGui::Selectable(buff, false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (item.ptr->IsAmmo() || item.ptr->IsArmor() || item.ptr->IsWeapon()) {
								auto player = RE::PlayerCharacter::GetSingleton();
								auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
								if (item.isWorn) {
									actorEquipManager->UnequipObject(player, item.ptr, item.invExtraPtr, 1);
									RefreshInfo();
								} else {
									actorEquipManager->EquipObject(player, item.ptr, item.invExtraPtr, 1);
									RefreshInfo();
								}
							}
						}
					} else {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), buff);
					}
					ImGui::OpenPopupOnItemClick("itemPopMenuInvFavor", ImGuiPopupFlags_MouseButtonRight);

					ImGui::TableNextColumn();
					if (item.ptr) {
						if (type == 1) {
							if (item.isBest) {
								myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.AlchemyValue);
							} else {
								myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.AlchemyValue);
							}
						} else if (type == 2) {
							if (item.isBest) {
								myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.SmithingValue);
							} else {
								myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.SmithingValue);
							}
						}
					}

					ImGui::TableNextColumn();
					if (item.ptr) {
						ImGui::Text("%.1f", item.weight);
					}
					//}
					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.formIdStr.c_str());*/
					//ImGui::TableNextColumn();
					//ImGui::Text("0x%X", item->address);

					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.debugName.c_str());*/
					ImGui::PopID();
					//}
				}
			ImGui::EndTable();
		}
	}

	void __fastcall buildPlayerInvPotionInfo(std::vector<InventoryInfo>& inv, int count, int type)
	{
		static ImGuiTableFlags flags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		if (ImGui::BeginTable("tablePlayerInvPotion", 2, flags, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			//ImGui::TableSetupColumn("装备", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 30.0f, TableColumn_2);
			//ImGui::TableSetupColumn("插槽", ImGuiTableColumnFlags_WidthFixed, 40.0f, TableColumn_1);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 120.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);
			ImGui::TableSetupColumn("加成", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f * ImGui::GetIO().FontGlobalScale, TableColumn_4);
			//ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			//ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed, 40, TableColumn_5);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			char buff[50];
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					InventoryInfo& item = inv[row_n];
					//if (item) {
					ImGui::PushID(row_n);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					snprintf(buff, 50, "%s", item.name.c_str());
					if (ImGui::Selectable(buff, false, ImGuiSelectableFlags_AllowDoubleClick)) {
						if (item.ptr->Is(RE::FormType::AlchemyItem)) {
							const auto alch = item.ptr->As<RE::AlchemyItem>();
							if (alch) {
								auto player = RE::PlayerCharacter::GetSingleton();
								auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
								actorEquipManager->EquipObject(player, item.ptr, item.invExtraPtr, 1);
								RefreshInfo();
							}
						}
					}

					ImGui::TableNextColumn();

					if (type == 1) {
						if (item.isBest) {
							myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.AlchemyValue);
						} else {
							myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.AlchemyValue);
						}
					} else if (type == 2) {
						if (item.isBest) {
							myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.SmithingValue);
						} else {
							myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.SmithingValue);
						}
					} else if (type == 3) {
						if (item.isBest) {
							myTextColored(ImVec4(0, 0.8f, 0, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.EnchantingValue);
						} else {
							myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "%s%0.0f%%", ICON_MDI_TRANSFER_UP, item.EnchantingValue);
						}
					}

					/*ImGui::TableNextColumn();
					ImGui::Text("%.1f", item.weight);*/
					//}
					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.formIdStr.c_str());*/
					//ImGui::TableNextColumn();
					//ImGui::Text("0x%X", item->address);

					/*ImGui::TableNextColumn();
					ImGui::Text("%s", item.debugName.c_str());*/
					ImGui::PopID();
					//}
				}
			ImGui::EndTable();
		}
	}

	void renderPlayerInv2()
	{
		/*if (ImGui::Button("刷新列表")) {
			RefreshPlayerInvInfo();
		}
		ImGui::Separator();*/

		{
			ImGui::Text(ICON_MDI_BOWL_MIX " 炼金");
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kAlchemyModifier + playerInfo.kAlchemySkillAdvance + playerInfo.kAlchemyPowerModifier);
			ImGui::Separator();

			ImGui::BeginGroup();

			ImGui::Text(ICON_MDI_SHIELD_HALF_FULL " 装备(%d)", getPlayerInvAlchemyCount());
			if (playerInfo.kAlchemyModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kAlchemyModifier);
			}
			buildPlayerInvAlchemyInfo(getPlayerInvAlchemy(), getPlayerInvAlchemyCount(), 1);
			ImGui::EndGroup();
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::BeginGroup();
			ImGui::Text(ICON_MDI_STAR " 收藏(%d)", getPlayerInvAlchemyFavorCount());
			buildPlayerInvFavorInfo(getPlayerInvAlchemyFavor(), getPlayerInvAlchemyFavorCount(), 1);
			ImGui::EndGroup();
		}
		ImGui::Spacing();
		{
			ImGui::Text(ICON_MDI_ANVIL " 锻造");
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kSmithingModifier + playerInfo.kSmithingSkillAdvance + playerInfo.kSmithingPowerModifier);
			ImGui::Separator();

			ImGui::BeginGroup();

			ImGui::Text(ICON_MDI_SHIELD_HALF_FULL " 装备(%d)", getPlayerInvSmithingCount());
			if (playerInfo.kSmithingModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kSmithingModifier);
			}
			buildPlayerInvAlchemyInfo(getPlayerInvSmithing(), getPlayerInvSmithingCount(), 2);
			ImGui::EndGroup();
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::BeginGroup();
			ImGui::Text(ICON_MDI_STAR " 收藏(%d)", getPlayerInvSmithingFavorCount());
			buildPlayerInvFavorInfo(getPlayerInvSmithingFavor(), getPlayerInvSmithingFavorCount(), 2);
			ImGui::EndGroup();
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::BeginGroup();
			ImGui::Text(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 锻造药水(%d)", getPlayerInvSmithingPotionCount());
			if (playerInfo.kSmithingPowerModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%% ", playerInfo.kSmithingPowerModifier);
			}

			buildPlayerInvPotionInfo(getPlayerInvSmithingPotion(), getPlayerInvSmithingPotionCount(), 2);
			ImGui::EndGroup();
		}
		ImGui::Spacing();
		/*ImGui::Separator();
		ImGui::Spacing();*/
		{
			ImGui::Text(ICON_MDI_FLASH " 附魔");
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kEnchantingModifier + playerInfo.kEnchantingSkillAdvance + playerInfo.kEnchantingPowerModifier);
			ImGui::Separator();
			ImGui::BeginGroup();

			ImGui::Text(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 附魔药水(%d)", getPlayerInvEnchantingPotionCount());
			if (playerInfo.kEnchantingPowerModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%% ", playerInfo.kEnchantingPowerModifier);
			}

			buildPlayerInvPotionInfo(getPlayerInvEnchantingPotion(), getPlayerInvEnchantingPotionCount(), 3);
			ImGui::EndGroup();
		}
	}
	void renderPlayerInv()
	{
		ImGui::Begin("玩家物品信息", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		static int selected = 0;
		ImGui::BeginGroup();
		{
			ImVec2 size(ImGui::GetFontSize() * 6, 0);

			if (ImGui::Selectable("所有装备", selected == 0, 0, size)) {
				selected = 0;
			}

			if (ImGui::Selectable("三神", selected == 1, 0, size)) {
				selected = 1;
			}

			/*	if (ImGui::Selectable("锻造装备", selected == 2, 0, size)) {
				selected = 2;
			}*/
		}
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		{
			if (ImGui::Button("刷新列表")) {
				RefreshPlayerInvInfo();
			}
			ImGui::Separator();
			if (selected == 0) {
				/*ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
					if (ImGui::BeginTabItem(ICON_MDI_SWORD " 武器", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvWEAPCount(), getPlayerInvWEAPData());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_SHIELD " 装备", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvARMOCount(), getPlayerInvARMOData());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_ARROW_PROJECTILE " 弹药", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvAMMOCount(), getPlayerInvAMMOData());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_SOURCE_BRANCH " 材料", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvINGRCount(), getPlayerInvINGRData());
						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 药水", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvALCHCount(), getPlayerInvALCHData());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_FOOD_DRUMSTICK " 食物", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvFOODCount(), getPlayerInvFOODData());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_BOOK_OPEN_VARIANT " 书信", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvBOOKCount(), getPlayerInvBOOKData());
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_AXIS_ARROW " 杂项", 0, 0)) {
						buildPlayerInvInfo(getPlayerInvCount(), getPlayerInvData());
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				ImGui::Text("\uf0d6 %d", getPlayerGoldCount());*/
				//ImGui::Text("负重：%0.1f/%0.0f", playerInfo.equippedWeight, playerInfo.carryWeight);
			} else if (selected == 1) {
				{
					ImGui::BeginGroup();
					ImGui::Text(ICON_MDI_BOWL_MIX " 炼金", getPlayerInvAlchemyCount());
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kAlchemyModifier + playerInfo.kAlchemySkillAdvance);
					buildPlayerInvAlchemyInfo(getPlayerInvAlchemy(), getPlayerInvAlchemyCount(), 1);
					ImGui::EndGroup();
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::BeginGroup();
					ImGui::Text(ICON_MDI_STAR " 收藏", getPlayerInvAlchemyFavorCount());
					buildPlayerInvFavorInfo(getPlayerInvAlchemyFavor(), getPlayerInvAlchemyFavorCount(), 1);
					ImGui::EndGroup();
				}
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				{
					ImGui::BeginGroup();
					ImGui::Text(ICON_MDI_ANVIL " 锻造", getPlayerInvSmithingCount());
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%%", playerInfo.kSmithingModifier + playerInfo.kSmithingSkillAdvance);
					buildPlayerInvAlchemyInfo(getPlayerInvSmithing(), getPlayerInvSmithingCount(), 2);
					ImGui::EndGroup();
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::BeginGroup();
					ImGui::Text(ICON_MDI_STAR " 收藏", getPlayerInvSmithingFavorCount());
					buildPlayerInvFavorInfo(getPlayerInvSmithingFavor(), getPlayerInvSmithingFavorCount(), 2);
					ImGui::EndGroup();
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::BeginGroup();
					ImGui::Text(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 锻造药水", getPlayerInvSmithingPotionCount());
					if (playerInfo.kSmithingPowerModifier > 0.0f) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%% ", playerInfo.kSmithingPowerModifier);
					}

					buildPlayerInvPotionInfo(getPlayerInvSmithingPotion(), getPlayerInvSmithingPotionCount(), 2);
					ImGui::EndGroup();
				}
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				{
					ImGui::BeginGroup();
					ImGui::Text(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 附魔药水", getPlayerInvEnchantingPotionCount());
					if (playerInfo.kEnchantingPowerModifier > 0.0f) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ICON_MDI_TRANSFER_UP "%.0f%% ", playerInfo.kEnchantingPowerModifier);
					}

					buildPlayerInvPotionInfo(getPlayerInvEnchantingPotion(), getPlayerInvEnchantingPotionCount(), 3);
					ImGui::EndGroup();
				}
			}
		}
		ImGui::EndGroup();
		ImGui::End();
	}

}
