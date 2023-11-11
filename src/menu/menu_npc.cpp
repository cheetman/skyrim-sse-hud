#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/npc.h>
#include <menu/menu.h>
#include <menu/menu_npc.h>
#include <utils/utils.h>

namespace menu
{
	bool show_npc_window_formid = false;
	bool show_npc_window_split = false;
	bool show_npc = true;
	bool show_enemy = true;
	bool show_teammate = true;
	bool show_horse = true;

	void __fastcall buildNpcInfo(int active2, ActorInfo* actor2Info, int actorCount)
	{
		if (active2) {
			for (int i = 0; i < actorCount; i++) {
				auto& item = actor2Info[i];

				char hp[16] = "已死亡";
				if (item.lifeState != RE::ACTOR_LIFE_STATE::kDead) {
					snprintf(hp, 16, "%.0f/%.0f", item.kHealth, item.kHealthBase);
				}

				bool treeNodeExResult;
				if (show_npc_window_formid) {
					treeNodeExResult = ImGui::TreeNodeEx(item.formIdStr.c_str(), 0, "%s - [%d] %s [ %s ]", item.formIdStr.c_str(), item.level, item.name.c_str(), hp);
				} else {
					treeNodeExResult = ImGui::TreeNodeEx(item.formIdStr.c_str(), 0, "[%d] %s [ %s ]", item.level, item.name.c_str(), hp);
				}

				if (treeNodeExResult) {
					ImGui::PushID(i + 1000);
					if (ImGui::SmallButton("\uf101 传送玩家")) {
						std::string commandStr = "player.moveto ";
						commandStr.append(item.formIdStr);
						ScriptUtil::ExecuteCommand(commandStr);
					}
					ImGui::PopID();

					if (item.lifeState != RE::ACTOR_LIFE_STATE::kDead) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::PushID(i + 2000);
						if (ImGui::SmallButton("\uf100 传送目标")) {
							std::string commandStr = "moveto player";

							// 调用控制台
							const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
							const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
							if (script) {
								script->SetCommand(commandStr);
								script->CompileAndRun(item.ptr);
								delete script;
							}
						}
						ImGui::PopID();
					}

					if (show_npc_window_ignore) {
						ImGui::PushID(i + 5000);
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_EYE_REMOVE_OUTLINE)) {
							bool exist = false;
							for (const auto& excludeForm : excludeNpcForms) {
								if (excludeForm.formId == item.baseFormId) {
									exist = true;
									break;
								}
							}
							if (!exist) {
								excludeNpcForms.push_back({ item.baseFormId, item.name, "" });
							}
							excludeNpcFormIds.insert(item.baseFormId);
						}
						ImGui::PopID();
					}

					if (item.inventoryCount > 0) {
						ImGui::Separator();
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
						for (int i2 = 0; i2 < item.inventoryCount; i2++) {
							auto& inv = item.Inventorys[i2];

							char buf[80];
							if (inv.count > 1) {
								if (show_npc_window_formid) {
									snprintf(buf, 80, "%s %s - %s %s (%d) %.1f ", u8"\uf01c", inv.formIdStr.c_str(), inv.isWorn ? "[装备中]" : "", inv.name.c_str(), inv.count, inv.weight);
								} else {
									snprintf(buf, 80, "%s %s %s (%d) %.1f ", u8"\uf01c", inv.isWorn ? "[装备中]" : "", inv.name.c_str(), inv.count, inv.weight);
								}
							} else {
								if (show_npc_window_formid) {
									snprintf(buf, 80, "%s %s - %s %s %.1f ", u8"\uf01c", inv.formIdStr.c_str(), inv.isWorn ? "[装备中]" : "", inv.name.c_str(), inv.weight);
								} else {
									snprintf(buf, 80, "%s %s %s %.1f ", u8"\uf01c", inv.isWorn ? "[装备中]" : "", inv.name.c_str(), inv.weight);
								}
							}
							if (ImGui::Selectable(buf, false)) {
								auto player = RE::PlayerCharacter::GetSingleton();
								item.ptr->RemoveItem(inv.ptr, 1, RE::ITEM_REMOVE_REASON::kSelling, 0, player);
							}
						}
						ImGui::PopStyleColor();
					}

					ImGui::TreePop();
				}
			}
		} else {
			for (int i = 0; i < actorCount; i++) {
				auto& item = actor2Info[i];

				if (item.isInCombat) {
					myTextColored(ImVec4(1, 1, 0, 1), "\uf071");

				} else {
					ImGui::Text("   ");
				}
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

				if (show_npc_window_process && (!show_npc_window_process_combat || item.isInCombat)) {
					ImGui::Text("[%d] %s", item.level, item.name.c_str());
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

					auto progressNpc = item.kHealth / (item.kHealthBase == 0 ? 1 : item.kHealthBase);
					char buf[32];
					snprintf(buf, 32, "%d/%d", (int)item.kHealth, (int)item.kHealthBase);

					ImGui::PushItemWidth(ImGui::GetFontSize() * 4);

					float healthRate = item.kHealth / (item.kHealthBase == 0 ? 1 : item.kHealthBase);
					if (healthRate > 0.75f) {
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressNpc1);
					} else if (healthRate < 0.20f) {
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressNpc3);
					} else {
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressNpc2);
					}
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					ImGui::ProgressBar(progressNpc, ImVec2(0.f, 0.f), buf);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();
					ImGui::PopItemWidth();

				} else {
					ImGui::Text("[%d] %s [", item.level, item.name.c_str());
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

					if (item.lifeState == RE::ACTOR_LIFE_STATE::kDead) {
						myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "已死亡");
					} else {
						float enemyHealthRate = item.kHealth / (item.kHealthBase == 0 ? 1 : item.kHealthBase);
						if (enemyHealthRate > 0.85f) {
							if (item.kHealthBase == item.kHealth) {
								myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%.0f/%.0f", item.kHealth, item.kHealthBase);
							} else {
								myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%.1f/%.0f", item.kHealth, item.kHealthBase);
							}
						} else if (enemyHealthRate < 0.20f) {
							myTextColored(ImVec4(1, 0.5f, 0.0f, 1.0f), "%.1f/%.0f", item.kHealth, item.kHealthBase);
						} else if (enemyHealthRate <= 0) {
							myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0/%.0f", item.kHealthBase);
						} else {
							ImGui::Text("%.1f/%.0f", item.kHealth, item.kHealthBase);
						}
					}

					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::Text("]");
				}

				if (show_npc_window_direction) {
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
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

				if (activeItems) {
					if (trackActorPtrs2.find(item.ptr) == trackActorPtrs2.end()) {
						//auto player = RE::PlayerCharacter::GetSingleton();
						//if (item.ptr->GetCurrentLocation() == player->currentLocation) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::PushID(i + 3000);
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS)) {
							trackActorPtrs2.insert(std::make_pair(item.ptr,item.name));
						}
						ImGui::PopID();
						//}
					}
				}
			}
		}
	}

	void renderNpc(ImGuiWindowFlags flags)
	{
		if (show_npc_window_split) {
			ImGui::Begin("npc-1", nullptr, flags);
			if (getNpcCount() > 0) {
				if (active) {
					ImGui::Checkbox(ICON_MDI_ACCOUNT " npc", &show_npc);
				} else {
					if (show_npc) {
						ImGui::Text(ICON_MDI_ACCOUNT " npc");
					}
				}
				if (show_npc) {
					auto actorInfo = getNpcData();
					buildNpcInfo(active, actorInfo, getNpcCount());
				}
			}
			ImGui::End();

			ImGui::Begin("npc-2", nullptr, flags);
			if (getHorseCount() > 0) {
				if (active) {
					ImGui::Checkbox(ICON_MDI_HORSE_VARIANT " horse", &show_horse);
				} else {
					if (show_horse) {
						ImGui::Text(ICON_MDI_HORSE_VARIANT " horse");
					}
				}
				if (show_horse) {
					auto actorInfo = getHorseData();
					buildNpcInfo(active, actorInfo, getHorseCount());
				}
			}
			ImGui::End();

			ImGui::Begin("npc-3", nullptr, flags);
			if (getEnemyCount() > 0) {
				if (active) {
					ImGui::Checkbox(ICON_MDI_SWORD_CROSS " enemy", &show_enemy);
				} else {
					if (show_enemy) {
						ImGui::Text(ICON_MDI_SWORD_CROSS " enemy");
					}
				}
				if (show_enemy) {
					auto actorInfo = getEnemy2Data();
					buildNpcInfo(active, actorInfo, getEnemyCount());
				}
			}
			ImGui::End();

			ImGui::Begin("npc-4", nullptr, flags);
			if (getTeammateCount() > 0) {
				if (active) {
					ImGui::Checkbox(ICON_MDI_SHIELD_ACCOUNT " team", &show_teammate);
				} else {
					if (show_teammate) {
						ImGui::Text(ICON_MDI_SHIELD_ACCOUNT " team");
					}
				}
				if (show_teammate) {
					auto actorInfo = getTeammateData();
					buildNpcInfo(active, actorInfo, getTeammateCount());
				}
			}

			ImGui::End();

		} else {
			ImGui::Begin("npc信息", nullptr, flags);
			if (getNpcCount() > 0) {
				if (active) {
					ImGui::Checkbox(ICON_MDI_ACCOUNT " npc", &show_npc);
				} else {
					if (show_npc) {
						ImGui::Text(ICON_MDI_ACCOUNT " npc");
					}
				}
				if (show_npc) {
					auto actorInfo = getNpcData();
					buildNpcInfo(active, actorInfo, getNpcCount());
				}
			}

			if (getHorseCount() > 0) {
				if (getNpcCount() > 0) {
					if (!no_background) {
						ImGui::Separator();
					} else {
						ImGui::Text(" ");
					}
				}
				if (active) {
					ImGui::Checkbox(ICON_MDI_HORSE_VARIANT " horse", &show_horse);
				} else {
					if (show_horse) {
						ImGui::Text(ICON_MDI_HORSE_VARIANT " horse");
					}
				}
				if (show_horse) {
					auto actorInfo = getHorseData();
					buildNpcInfo(active, actorInfo, getHorseCount());
				}
			}

			if (getEnemyCount() > 0) {
				if (getNpcCount() || getHorseCount()) {
					if (!no_background) {
						ImGui::Separator();
					} else {
						ImGui::Text(" ");
					}
				}
				if (active) {
					ImGui::Checkbox(ICON_MDI_SWORD_CROSS " enemy", &show_enemy);
				} else {
					if (show_enemy) {
						ImGui::Text(ICON_MDI_SWORD_CROSS " enemy");
					}
				}
				if (show_enemy) {
					auto actorInfo = getEnemy2Data();
					buildNpcInfo(active, actorInfo, getEnemyCount());
				}
			}

			if (getTeammateCount() > 0) {
				if (getNpcCount() || getEnemyCount() || getHorseCount()) {
					if (!no_background) {
						ImGui::Separator();
					} else {
						ImGui::Text(" ");
					}
				}

				if (active) {
					ImGui::Checkbox(ICON_MDI_SHIELD_ACCOUNT " team", &show_teammate);
				} else {
					if (show_teammate) {
						ImGui::Text(ICON_MDI_SHIELD_ACCOUNT " team");
					}
				}
				if (show_teammate) {
					auto actorInfo = getTeammateData();
					buildNpcInfo(active, actorInfo, getTeammateCount());
				}
			}

			ImGui::End();
		}
	}

	void renderNpcSettings()
	{
		ImGui::Checkbox("显示NPC信息", &show_npc_window);
		if (show_npc_window) {
			if (ImGui::TreeNodeEx("设置##1", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("只显示距离内NPC", &show_npc_window_dis);
				if (show_npc_window_dis) {
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
					ImGui::DragInt("##距离", &show_npc_window_dis_meter, 1, 10, 100, "%d米内");
					ImGui::PopItemWidth();
				}
				ImGui::Checkbox("分割显示", &show_npc_window_split);
				ImGui::Checkbox("显示FORMID", &show_npc_window_formid);
				ImGui::Checkbox("显示方向和距离", &show_npc_window_direction);
				ImGui::Checkbox("死亡不显示", &show_npc_window_dead_hidden);
				ImGui::Checkbox("显示血条", &show_npc_window_process);
				if (show_npc_window_process) {
					ImGui::Checkbox("战斗显示", &show_npc_window_process_combat);
					ImGui::ColorEdit4("颜色1", &colorProgressNpc1.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
					ImGui::ColorEdit4("颜色2", &colorProgressNpc2.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
					ImGui::ColorEdit4("颜色3", &colorProgressNpc3.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
				}

				ImGui::Checkbox("忽略NPC", &show_npc_window_ignore);
				if (show_npc_window_ignore) {
					static ImGuiTableFlags flagsItem =
						ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

					const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
					if (ImGui::BeginTable("tableItemNpcIngore", 3, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 12, TEXT_BASE_HEIGHT * 6), 0.0f)) {
						ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, 1);
						ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 60.0f * ImGui::GetIO().FontGlobalScale, 2);
						ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetIO().FontGlobalScale, 3);

						ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
						ImGui::TableHeadersRow();

						int deleteFormId = 0;

						ImGuiListClipper clipper;
						clipper.Begin(excludeNpcForms.size());
						while (clipper.Step())
							for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
								ExcludeForm& item = excludeNpcForms[row_n];
								ImGui::PushID(item.formId + 0x2000000);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("%08X", item.formId);
								ImGui::TableNextColumn();
								ImGui::Text("%s", item.name.c_str());
								ImGui::TableNextColumn();

								if (ImGui::SmallButton(ICON_MDI_CLOSE)) {
									deleteFormId = item.formId;
								}

								ImGui::PopID();
							}
						ImGui::EndTable();
						if (deleteFormId) {
							excludeNpcFormIds.erase(deleteFormId);
							excludeNpcForms.erase(std::remove_if(excludeNpcForms.begin(), excludeNpcForms.end(),
													  [&deleteFormId](const ExcludeForm& x) {
														  return x.formId == deleteFormId;
													  }),
								excludeNpcForms.end());
						}
					}
				}

				ImGui::TreePop();
			}
		}
	}

}
