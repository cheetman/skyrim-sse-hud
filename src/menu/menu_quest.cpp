#include "menu_quest.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/memory.h>
#include <memory/quest.h>
#include <menu/menu.h>
#include <utils/utils.h>

namespace menu
{
	bool isShowQuestText = false;
	bool isShowQuestStageAll = false;

	ImVec4 colorQuestTableHeaderBg(0.0f, 0.7f, 0.0f, 0.75f);
	ImVec4 colorQuestTableBorderStrong(0.286f, 1, 0, 0.7f);

	std::regex pattern("<Alias=(\\w+)>|<Alias.ShortName=(\\w+)>");

	void addItem(RE::TESBoundObject* objPtr)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		char buf[80];
		snprintf(buf, 80, "%s 已获取", objPtr->GetName());
		RE::DebugNotification(buf, NULL, false);
		ScriptUtil::AddItem(nullptr, 0, player, objPtr, 1, true);
	}

	void moveToItem(RE::TESObjectREFR* refPtr)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		player->MoveTo(refPtr);
		if (activeItems) {
			activeItems = false;
		}
	}

	void moveToNpc(RE::TESNPC* npcPtr)
	{
		RE::Actor* actorTarget = nullptr;
		{
			const auto& [map, lock] = RE::TESForm::GetAllForms();
			const RE::BSReadWriteLock locker{ lock };
			for (auto& [id, form] : *map) {
				if (form && form->Is(RE::FormType::ActorCharacter)) {
					auto actor = form->As<RE::Actor>();
					if (actor && !actor->IsPlayerRef()) {
						if (actor->GetActorBase() == npcPtr) {
							actorTarget = actor;
							break;
						}
					}
				}
			}
		}
		if (actorTarget) {
			auto player = RE::PlayerCharacter::GetSingleton();
			player->MoveTo(actorTarget);
			if (activeItems) {
				activeItems = false;
			}
		}
	}

	QuestAliasInfo* findAlias(QuestInfo& item, std::uint32_t aliasID)
	{
		for (int j = 0; j < item.aliasCount; j++) {
			auto& alias = item.aliases[j];
			if (alias.aliasID == aliasID) {
				alias.isLink = true;
				if (!alias.refPtr && !alias.npcPtr && !alias.objPtr) {
					return nullptr;
				}
				return &alias;
			}
		}
		return nullptr;
	}

	void buildQuestItem(int count, std::vector<QuestInfo>& items)
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

		if (show_items_window_file) {
			columnCount++;
		}
		columnCount++;

		ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, colorQuestTableHeaderBg);
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, colorQuestTableBorderStrong);

		if (ImGui::BeginTable("tableQuest", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			//ImGui::TableSetupColumn("进度", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_1);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 150.0f * ImGui::GetIO().FontGlobalScale, TableColumn_2);
			ImGui::TableSetupColumn("任务类型", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_3);
			if (show_items_window_refid) {
				ImGui::TableSetupColumn("EDITORID", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, TableColumn_6);
			}
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

					if (ImGui::BeginPopupContextItem("itemQuestPopMenu")) {
						if (ImGui::Button(ICON_MDI_EYE_REMOVE_OUTLINE " 忽略")) {
							quest::excludeQuestFormIds.insert(item.formId);
						}
						ImGui::EndPopup();
					}

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
							ImGui::SameLine();
							if (isShowQuestText) {
								if (ImGui::SmallButton(ICON_MDI_PLAYLIST_REMOVE)) {
									isShowQuestText = false;
								}
							} else {
								if (ImGui::SmallButton(ICON_MDI_PLAYLIST_CHECK)) {
									isShowQuestText = true;
								}
							}

							ImGui::SameLine();
							if (isShowQuestStageAll) {
								if (ImGui::SmallButton(ICON_MDI_CHECKBOX_INTERMEDIATE_VARIANT)) {
									isShowQuestStageAll = false;
								}
							} else {
								if (ImGui::SmallButton(ICON_MDI_CHECKBOX_INTERMEDIATE)) {
									isShowQuestStageAll = true;
								}
							}

							ImGui::Separator();

							int columnCount = 0;
							if (isShowQuestText) {
								columnCount = 6;
							} else {
								columnCount = 5;
							}

							{
								ImGui::BeginGroup();

								if (ImGui::BeginTable("tableQuestData", columnCount)) {
									int i = 0;
									auto quest = item.ptr;
									auto currentStage = quest->GetCurrentStageID();
									for (auto item2 : *quest->waitingStages) {
										i++;

										bool existObjectives = false;
										// 没内容就不显示
										if (!isShowQuestStageAll) {
											for (auto obj : quest->objectives) {
												if (obj->index == item2->data.index) {
													existObjectives = true;
													break;
												}
											}

											if (!existObjectives && currentStage != item2->data.index) {
												continue;
											}
										}

										if (currentStage == item2->data.index) {
											ImGui::TableNextColumn();
											myTextColored(ImVec4(1, 1, 0, 1.0f), "阶段-%d", i);
											ImGui::TableNextColumn();
											myTextColored(ImVec4(1, 1, 0, 1.0f), "%d " ICON_MDI_FLAG_TRIANGLE, item2->data.index);
										} else {
											ImGui::TableNextColumn();
											ImGui::Text("阶段-%d", i);
											ImGui::TableNextColumn();
											ImGui::Text("%d", item2->data.index);
										}

										for (auto obj : quest->objectives) {
											if (obj->index == item2->data.index) {
												if (isShowQuestText) {
													ImGui::TableNextColumn();
													std::string text = obj->displayText.c_str();
													std::smatch match;

													while (std::regex_search(text, match, pattern)) {
														if (match.str(1).length() > 0) {
															std::string str = match.str(1);
															bool exist = false;
															for (int i = 0; i < item.aliasCount; i++) {
																auto& alias = item.aliases[i];
																if (alias.aliasName == str) {
																	text = match.prefix().str() + "【" + alias.targetName + "】" + match.suffix().str();
																	exist = true;
																	break;
																}
															}
															if (!exist) {
																text = match.prefix().str() + "【空】" + match.suffix().str();
															}
														} else if (match.str(2).length() > 0) {
															std::string str = match.str(2);
															bool exist = false;
															for (int i = 0; i < item.aliasCount; i++) {
																auto& alias = item.aliases[i];
																if (alias.aliasName == str) {
																	text = match.prefix().str() + "【" + alias.targetName + "】" + match.suffix().str();
																	exist = true;
																	break;
																}
															}
															if (!exist) {
																text = match.prefix().str() + "【空】" + match.suffix().str();
															}
														}
													}

													if (item2->data.flags.underlying() == 1) {
														if (currentStage == item2->data.index) {
															myTextColored(ImVec4(1, 1, 0, 1.0f), "%s", text.c_str());
														} else {
															ImGui::Text("%s", text.c_str());
														}
													} else {
														ImGui::Text("%s", text.c_str());
													}
												}

												ImGui::TableNextColumn();
												if (obj->targets && obj->numTargets > 0) {
													for (int i2 = 0; i2 < obj->numTargets; i2++) {
														auto alias = findAlias(item, obj->targets[i2]->alias);

														if (alias) {
															if (i2 > 0) {
																if (!isShowQuestText) {
																	break;
																}
																ImGui::SameLine();
																ImGui::Text("|");
																ImGui::SameLine();
															}
															ImGui::PushID(obj->index * 100 + i2);
															if (alias->refPtr) {
																bool active = false;
																if (isShowQuestText) {
																	active = ImGui::SmallButton(alias->targetName.c_str());
																} else {
																	active = ImGui::Selectable(alias->targetName.c_str());
																}
																if (active) {
																	moveToItem(alias->refPtr);
																}
															} else if (alias->npcPtr) {
																bool active = false;
																if (isShowQuestText) {
																	active = ImGui::SmallButton(alias->targetName.c_str());
																} else {
																	active = ImGui::Selectable(alias->targetName.c_str());
																}
																if (active) {
																	moveToNpc(alias->npcPtr);
																}
															} else if (alias->objPtr) {
																std::string targetName = ICON_MDI_HAND_COIN + std::string(" ") + alias->targetName;
																bool active = false;
																if (isShowQuestText) {
																	active = ImGui::SmallButton(alias->targetName.c_str());
																} else {
																	active = ImGui::Selectable(alias->targetName.c_str());
																}
																if (active) {
																	addItem(alias->objPtr);
																}

															} else {
																// 调试
																ImGui::Text("%s:%s:%s:%08x", alias->type.c_str(), alias->fillType.c_str(), alias->targetName.c_str(), alias->targetFormId);
															}

															ImGui::PopID();
															break;
														}
													}
												}

												existObjectives = true;
												break;
											}
										}

										if (!existObjectives) {
											if (isShowQuestText) {
												ImGui::TableNextColumn();
												ImGui::TableNextColumn();
											} else {
												ImGui::TableNextColumn();
											}
										}

										ImGui::TableNextColumn();
										if (item2->data.flags.underlying() == 1) {
											if (currentStage == item2->data.index) {
												myTextColored(ImVec4(1, 1, 0, 1.0f), "进行中");
											} else {
												myTextColored(ImVec4(0, 1, 0, 1.0f), "已完成");
											}
										} else {
											myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "-");
										}
										ImGui::TableNextColumn();
										if (item2->data.index > currentStage) {
											if (ImGui::SmallButton(ICON_MDI_CHECK)) {
												int id = item2->data.index;
												std::string commandStr = "setstage  ";
												commandStr.append(item.editorId);
												commandStr.append(" ");
												commandStr.append(std::to_string(id));
												ScriptUtil::ExecuteCommand(commandStr);
											}
										}
									}

									ImGui::EndTable();
								}
								ImGui::EndGroup();
							}
							//ImGui::Separator();

							if (isShowQuestText) {
								//ImGui::Separator();
								ImGui::Spacing();
								ImGui::BeginGroup();

								ImGui::Text("其他目标:");
								if (ImGui::BeginTable("tableQuestData2", 3)) {
									std::unordered_set<RE::FormID> filter;
									for (int i = 0; i < item.aliasCount; i++) {
										auto& alias = item.aliases[i];
										if (item.aliases[i].targetName.empty() || alias.isLink) {
											continue;
										}

										if (alias.refPtr) {
											if (alias.refPtr->IsPlayerRef()) {
												continue;
											}
											if (filter.find(alias.refPtr->GetFormID()) != filter.end()) {
												continue;
											}
											filter.insert(alias.refPtr->GetFormID());
											ImGui::TableNextColumn();
											ImGui::PushID(i);
											if (ImGui::SmallButton(item.aliases[i].targetName.c_str())) {
												moveToItem(alias.refPtr);
											}
											ImGui::PopID();
										} else if (alias.npcPtr) {
											if (alias.npcPtr->IsPlayer()) {
												continue;
											}
											if (filter.find(alias.npcPtr->GetFormID()) != filter.end()) {
												continue;
											}
											filter.insert(alias.npcPtr->GetFormID());

											ImGui::TableNextColumn();
											ImGui::PushID(i);
											if (ImGui::SmallButton(item.aliases[i].targetName.c_str())) {
												moveToNpc(alias.npcPtr);
											}
											ImGui::PopID();
										}

										/*ImGui::Text("id:%d", item.aliases[i].aliasID);
									ImGui::TableNextColumn();
									ImGui::Text("%s", item.aliases[i].aliasName.c_str());
									ImGui::TableNextColumn();
									ImGui::Text("%s:%s", item.aliases[i].type.c_str(), item.aliases[i].fillType.c_str());
									ImGui::TableNextColumn();
									ImGui::Text("%08X", item.aliases[i].targetFormId);
									ImGui::TableNextColumn();
									ImGui::Text("%s", item.aliases[i].targetName.c_str());*/
									}
									ImGui::EndTable();
								}
								ImGui::EndGroup();
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

					ImGui::OpenPopupOnItemClick("itemQuestPopMenu", ImGuiPopupFlags_MouseButtonRight);


					ImGui::TableNextColumn();
					if (item.isActive) {
						ImGui::Text("%s", item.questTypeName.c_str());
						//ImGui::Text("%d", item.flag);
					} else {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", item.questTypeName.c_str());
						//ImGui::Text("%d", item.flag);
					}

					if (show_items_window_refid) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.editorId.c_str());
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
