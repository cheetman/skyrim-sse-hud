#include <event/BSTCrosshairRefEvent.h>
#include <event/BSTMenuEvent.h>
#include <fonts/IconsMaterialDesignIcons.h>
#include <hook/hudhook.h>
#include <imgui/imgui.h>
#include <memory/memory.h>
#include <menu/menu.h>
#include <nlohmann/json.hpp>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>

std::filesystem::path settings = "";
std::string fontFilePath = "";

namespace menu
{
	//ImFont* font2 = nullptr;
	//static float health = 0.0f;
	//static float kMagicka = 0.0f;
	//static float kStamina = 0.0f;
	//static float armor = 0.0f;
	//static float lightArmor = 0.0f;
	//static float heavyArmor = 0.0f;
	//static float kMeleeDamage = 0.0f;
	//static float kUnarmedDamage = 0.0f;
	//static float kCriticalChance = 0.0f;
	//static float kDamageResist = 0.0f;
	//static float lightArmor2 = 0.0f;
	//static float heavyArmor2 = 0.0f;
	//std::string playerRaceName = "";
	bool auto_remove_ammo = false;
	bool remember_last_ammo = false;
	int hotkey = 0;
	int hotkey2 = 3;
	float font_scale = 1.0f;

	// 默认配置
	static bool show_player_base_info_window = false;
	static bool show_player_base_info_window_sep = false;
	static bool flag_process = false;
	static bool flag_base_info_setting = true;

	static bool show_player_info_window = false;
	static bool show_player_mod_window = false;
	static bool show_player_armor_window = false;
	static bool show_player_weapon_window = false;
	static bool show_player_debug_window = false;
	static bool no_titlebar = true;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_background = false;
	static bool auto_resize = true;
	static int imgui_style_index = 2;

	// 20220428 追加
	static bool window_border = true;
	static bool frame_border = true;
	static bool bullet_text = true;

	//static int refresh_time_show = 1000;
	static float colorPlotHistogramX = 0.9f;
	static float colorPlotHistogramY = 0.7f;
	static float colorPlotHistogramZ = 0;
	static float colorPlotHistogramW = 1;

	//static bool show_npc_window_dis = false;
	//static int show_npc_window_dis_meter = 30;
	extern bool show_npc_window_formid = false;
	static int show_inv_window_height = 15;
	static float show_inv_window_height_half = 6.7f;
	static bool show_inv_window_active = true;
	static bool show_crosshair = false;

	// 基础属性进度条
	static float progress = 0;
	static float progress2 = 0;
	static float progress3 = 0;
	static int stateMod_lastSelectIndex = -1;
	static int stateMod_selectIndex = 0;
	static float stateMod_nowValue = 0;
	static float stateMod_newValue = 0;
	static int statePerMod_lastSelectIndex = -1;
	static int statePerMod_selectIndex = 0;
	static float statePerMod_nowValue = 0;
	static float statePerMod_newValue = 0;

	static int getInv_nowValue = 100;
	static int getInv_selectIndex = 0;

	ImVec4 colorProgress(1.0f, 0.5f, 0.0f, 1.0f);

	const char* actorValues[] = {
		"负重 [kCarryWeight]",
		"生命恢复 [kHealRate]",
		"魔法恢复 [kMagickaRate]",
		"耐力恢复 [KStaminaRate]",
		"行走速度 [kSpeedMult]",
		"武器基础伤害 [kMeleeDamage]",
		"空手基础伤害 [kUnarmedDamage]"
	};
	RE::ActorValue actorValuesIndex[] = {
		RE::ActorValue::kCarryWeight,
		RE::ActorValue::kHealRate,
		RE::ActorValue::kMagickaRate,
		RE::ActorValue::KStaminaRate,
		RE::ActorValue::kSpeedMult,
		RE::ActorValue::kMeleeDamage,
		RE::ActorValue::kUnarmedDamage,
	};

	const char* perActorValues[] = {
		"毒抗 [kPoisonResist]",
		"火抗 [kResistFire]",
		"冰抗 [kResistFrost]",
		"雷抗 [kResistShock]",
		"法抗 [kResistMagic]",
		"抗疾病 [kResistDisease]",
		"远程基础伤害 [kMarksmanPowerModifier]",
		"法术基础伤害 [kDestructionPowerModifier]"
	};
	RE::ActorValue perActorValuesIndex[] = {
		RE::ActorValue::kPoisonResist,
		RE::ActorValue::kResistFire,
		RE::ActorValue::kResistFrost,
		RE::ActorValue::kResistShock,
		RE::ActorValue::kResistMagic,
		RE::ActorValue::kResistDisease,
		RE::ActorValue::kMarksmanPowerModifier,
		RE::ActorValue::kDestructionPowerModifier,
	};

	const char* invValues[] = {
		"铁锭 [5ACE4]",
		"钢锭 [5ACE5]",
		"黑檀锭 [5AD9D]",
		"皮带 [800E4]"
	};

	const char* invValuesCode[] = {
		"5ACE4",
		"5ACE5",
		"5AD9D",
		"800E4"
	};

	std::vector<RE::Actor*> actors;

	enum TableColumn
	{
		TableColumn_1,
		TableColumn_2,
		TableColumn_3,
	};
	enum PlayerInfoColumnID
	{
		PlayerInfoColumnID_ID,
		PlayerInfoColumnID_1,
		PlayerInfoColumnID_2,
		PlayerInfoColumnID_3,
		PlayerInfoColumnID_4,
		PlayerInfoColumnID_5,
		PlayerInfoColumnID_6,
		PlayerInfoColumnID_7,
		PlayerInfoColumnID_8

	};

	static void myText(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		if (!bullet_text) {
			ImGui::BulletTextV(fmt, args);
		} else {
			ImGui::TextV(fmt, args);
		}
		va_end(args);
	}

	static void myText2(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		ImGui::TextV(fmt, args);
		va_end(args);
	}

	void myTextColored(const ImVec4 col, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		ImGui::TextColoredV(col, fmt, args);
		va_end(args);
	}

	static bool show_npc = true;
	static bool show_enemy = true;
	static bool show_teammate = true;
	static bool show_horse = true;

	void __fastcall buildNpcInfo(int active, ActorInfo* actorInfo, int actorCount)
	{
		if (active) {
			for (int i = 0; i < actorCount; i++) {
				auto item = actorInfo[i];

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
					if (ImGui::SmallButton("\uf101传送到目标")) {
						std::string commandStr = "player.moveto ";
						commandStr.append(item.formIdStr);

						// 调用控制台
						const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
						const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
						if (script) {
							const auto selectedRef = RE::Console::GetSelectedRef();
							script->SetCommand(commandStr);
							script->CompileAndRun(selectedRef.get());
							delete script;
						}
					}
					ImGui::PopID();

					if (item.lifeState != RE::ACTOR_LIFE_STATE::kDead) {
						ImGui::PushID(i + 2000);
						if (ImGui::SmallButton("\uf100传送到玩家")) {
							std::string commandStr = "moveto player";
							//commandStr.append(item.formId);
							//commandStr.append("moveto player");

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

					if (item.inventoryCount > 0) {
						ImGui::Separator();
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
						for (int i2 = 0; i2 < item.inventoryCount; i2++) {
							auto inv = item.Inventorys[i2];

							char buf[60];
							if (show_npc_window_formid) {
								snprintf(buf, 60, "%s %s - %s %s (%d) %.1f ", u8"\uf01c", inv.formIdStr.c_str(), inv.isWorn ? "[装备中]" : "", inv.name.c_str(), inv.count, inv.weight);
							} else {
								snprintf(buf, 60, "%s %s %s (%d) %.1f ", u8"\uf01c", inv.isWorn ? "[装备中]" : "", inv.name.c_str(), inv.count, inv.weight);
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
				auto item = actorInfo[i];

				if (item.isInCombat) {
					myTextColored(ImVec4(1, 1, 0, 1), "\uf071");

				} else {
					ImGui::Text("   ");
				}
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myText2("[%d] %s [", item.level, item.name.c_str());
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
						myText2("%.1f/%.0f", item.kHealth, item.kHealthBase);
					}
				}

				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myText2("]");

				if (show_npc_window_direction) {
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					switch (item.direction) {
					case 1:  //前
						myText(" \uf062%.0f", item.distance);
						break;
					case 2:  //左
						myText(" \uf060%.0f", item.distance);
						break;
					case 3:
						myText(" \uf063%.0f", item.distance);
						break;
					case 4:
						myText(" \uf061%.0f", item.distance);
						break;
					default:
						myText(" %.0f", item.distance);
						break;
					}
				}
			}
		}
	}

	void __fastcall buildItemCONTInfo(int count, ItemInfo* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		int columnCount = 3;

		if (show_items_window_direction) {
			columnCount++;
		}
		if (show_items_window_formid) {
			columnCount++;
		}
		if (show_items_window_settings) {
			columnCount++;
		}

		float tableHeight = TEXT_BASE_HEIGHT * show_inv_window_height;
		if (show_items_window_settings && show_items_window_auto_cont) {
			tableHeight = TEXT_BASE_HEIGHT * (show_inv_window_height_half + 1);
		}

		if (ImGui::BeginTable("tableItemCONT", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, tableHeight), 0.0f)) {
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 60, PlayerInfoColumnID_1);
			ImGui::TableSetupColumn("类型", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 30, PlayerInfoColumnID_3);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn("方位", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_7);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_4);
			}

			if (show_items_window_settings) {
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_5);
			}

			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			for (int row_n = 0; row_n < count; row_n++) {
				ItemInfo item = items[row_n];
				ImGui::PushID(item.formId + 0x1000000);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (item.invCount > 0) {
					bool openFlag;
					if (item.isCrime) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s", item.name.c_str());
					} else {
						if (item.isAuto) {
							openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s" ICON_MDI_AUTORENEW, item.name.c_str());
						} else {
							openFlag = ImGui::TreeNode(item.formIdStr.c_str(), "%s", item.name.c_str());
						}
					}

					if (item.isCrime) {
						ImGui::PopStyleColor();
					} else {
					}

					if (openFlag) {
						for (int i2 = 0; i2 < item.invCount; i2++) {
							auto inv = item.invs[i2];
							char buf[80];

							if (inv.count > 1) {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH " (%d)" : "%s (%d)", inv.name.c_str(), inv.count);
							} else {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH : "%s", inv.name.c_str());
							}

							if (inv.isCrime) {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kSteal, 0, player);
								}
								ImGui::PopStyleColor();
							} else {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kRemove, 0, player);
								}
								ImGui::PopStyleColor();
							}
						}
						ImGui::TreePop();
					}

				} else {
					myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s[空]", item.name.c_str());
				}

				ImGui::TableNextColumn();
				switch (item.lockLevel) {
				case RE::LOCK_LEVEL::kVeryEasy:
					ImGui::Text("新手锁");
					break;
				case RE::LOCK_LEVEL::kEasy:
					ImGui::Text("熟练锁");
					break;
				case RE::LOCK_LEVEL::kAverage:
					ImGui::Text("老手锁");
					break;
				case RE::LOCK_LEVEL::kHard:
					ImGui::Text("专家锁");
					break;
				case RE::LOCK_LEVEL::kVeryHard:
					ImGui::Text("大师锁");
					break;
				case RE::LOCK_LEVEL::kRequiresKey:
					ImGui::Text("加密锁");
					break;
				case RE::LOCK_LEVEL::kUnlocked:
				default:
					ImGui::Text("-");
					break;
				}

				ImGui::TableNextColumn();
				ImGui::Text("%d", item.invCount);

				if (show_items_window_direction) {
					ImGui::TableNextColumn();
					switch (item.direction) {
					case 1:  //前
						myText(" \uf062%.0f", item.distance);
						break;
					case 2:  //左
						myText(" \uf060%.0f", item.distance);
						break;
					case 3:
						myText(" \uf063%.0f", item.distance);
						break;
					case 4:
						myText(" \uf061%.0f", item.distance);
						break;
					default:
						myText(" %.0f", item.distance);
						break;
					}
				}

				if (show_items_window_formid) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.baseFormIdStr.c_str());
				}

				if (show_items_window_settings) {
					ImGui::TableNextColumn();

					if (ImGui::SmallButton("\uf101")) {
						std::string commandStr = "player.moveto ";
						commandStr.append(item.formIdStr);

						// 调用控制台
						const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
						const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
						if (script) {
							const auto selectedRef = RE::Console::GetSelectedRef();
							script->SetCommand(commandStr);
							script->CompileAndRun(selectedRef.get());
							delete script;
							if (activeItems) {
								activeItems = false;
							}
						}
					}

					if (show_items_window_auto_cont) {
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

				ImGui::PopID();
			}
			ImGui::EndTable();
		}

		if (show_items_window_settings && show_items_window_auto_cont) {
			float tableHeight2 = TEXT_BASE_HEIGHT * (show_inv_window_height_half - 1);
			ImGui::Text(ICON_MDI_AUTORENEW " 自动拾取容器类型");
			int columnCount2 = 3;
			if (ImGui::BeginTable("tableItemCONTauto", columnCount2, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, tableHeight2), 0.0f)) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80, TableColumn_1);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 60, TableColumn_2);
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40, TableColumn_3);

				ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
				ImGui::TableHeadersRow();

				int deleteFormId = 0;

				ImGuiListClipper clipper;
				clipper.Begin(autoContForms.size());
				while (clipper.Step())
					for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
						IncludeForm item = autoContForms[row_n];
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
		}
		//}
	}

	void __fastcall buildItemACHRInfo(int count, ItemInfo* items, RE::FormType formType)
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
		if (show_items_window_settings) {
			columnCount++;
		}
		if (ImGui::BeginTable("tableItemACHR", columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_1);
			//ImGui::TableSetupColumn("类型", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 30, PlayerInfoColumnID_3);

			if (show_items_window_direction) {
				ImGui::TableSetupColumn("方位", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_7);
			}
			if (show_items_window_formid) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_4);
			}

			if (show_items_window_settings) {
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_5);
			}

			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			for (int row_n = 0; row_n < count; row_n++) {
				ItemInfo item = items[row_n];
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
							auto inv = item.invs[i2];
							char buf[80];

							if (inv.count > 1) {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH " (%d)" : "%s (%d)", inv.name.c_str(), inv.count);
							} else {
								snprintf(buf, 80, item.isEnchanted ? "%s" ICON_MDI_FLASH : "%s", inv.name.c_str());
							}

							if (inv.isCrime) {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kSteal, 0, player);
								}
								ImGui::PopStyleColor();
							} else {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
								if (ImGui::Selectable(buf, false)) {
									auto player = RE::PlayerCharacter::GetSingleton();
									item.ptr->RemoveItem(inv.ptr, inv.count, RE::ITEM_REMOVE_REASON::kRemove, 0, player);
								}
								ImGui::PopStyleColor();
							}
						}
						ImGui::TreePop();
					}

				} else {
					myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s[空]", item.name.c_str());
				}

				ImGui::TableNextColumn();
				ImGui::Text("%d", item.invCount);

				if (show_items_window_direction) {
					ImGui::TableNextColumn();
					switch (item.direction) {
					case 1:  //前
						myText(" \uf062%.0f", item.distance);
						break;
					case 2:  //左
						myText(" \uf060%.0f", item.distance);
						break;
					case 3:
						myText(" \uf063%.0f", item.distance);
						break;
					case 4:
						myText(" \uf061%.0f", item.distance);
						break;
					default:
						myText(" %.0f", item.distance);
						break;
					}
				}

				if (show_items_window_formid) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.baseFormIdStr.c_str());
				}

				if (show_items_window_settings) {
					ImGui::TableNextColumn();
					if (ImGui::SmallButton("\uf101")) {
						std::string commandStr = "player.moveto ";
						commandStr.append(item.formIdStr);

						// 调用控制台
						const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
						const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
						if (script) {
							const auto selectedRef = RE::Console::GetSelectedRef();
							script->SetCommand(commandStr);
							script->CompileAndRun(selectedRef.get());
							delete script;
							if (activeItems) {
								activeItems = false;
							}
						}
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
				ItemInfo item = items[i];
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
				ItemInfo item = items[i];
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
				ItemInfo item = items[i];
				if (!item.isCrime) {
					if (item.ptr) {
						if (!item.isDeleted) {
							if (!item.ptr->IsMarkedForDeletion()) {
								player->PickUpObject(item.ptr, 1);
								char buf[32];
								snprintf(buf, 32, "%s 已拾取", item.ptr->GetDisplayFullName());
								RE::DebugNotification(buf, NULL, false);
								item.isDeleted = true;
							}
						}
					}
				}
			}
		}
	}

	void __fastcall buildItemInfo(int count, ItemInfo* items, RE::FormType formType)
	{
		static ImGuiTableFlags flagsItem =
			/*	ImGuiTableFlags_Reorderable
			| ImGuiTableFlags_Hideable*/
			ImGuiTableFlags_Resizable
			//| ImGuiTableFlags_RowBg
			| ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody
			/*	| ImGuiTableFlags_NoBordersInBody
				| ImGuiTableFlags_ScrollX
				| ImGuiTableFlags_SizingFixedFit*/
			;

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		int columnCount = 3;
		switch (formType) {
		case RE::FormType::Flora:
		case RE::FormType::Tree:
		case RE::FormType::Activator:
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
		// 显示formid
		if (show_items_window_formid) {
			columnCount++;
		}
		if (show_items_window_settings) {
			columnCount++;
		}
		if (show_items_window_direction) {
			columnCount++;
		}

		if (ImGui::BeginTable(("tableItem3" + std::to_string((int)formType)).c_str(), columnCount, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 15, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			//ImGui::TableSetupColumn("已装备", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_ID);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_1);
			switch (formType) {
			case RE::FormType::Flora:
			case RE::FormType::Tree:
			case RE::FormType::Activator:
			case RE::FormType::KeyMaster:
				break;
			default:
				ImGui::TableSetupColumn("价值", ImGuiTableColumnFlags_WidthFixed, 35, PlayerInfoColumnID_2);
				break;
			}
			switch (formType) {
			case RE::FormType::Flora:
			case RE::FormType::Tree:
			case RE::FormType::Activator:
			case RE::FormType::Ammo:
			case RE::FormType::None:
			case RE::FormType::KeyMaster:
				break;
			default:
				ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed, 30, PlayerInfoColumnID_3);
				break;
			}
			switch (formType) {
			case RE::FormType::Container:
				ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 30, PlayerInfoColumnID_2);
				break;
			default:
				break;
			}

			if (show_items_window_direction) {
				ImGui::TableSetupColumn("方位", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_8);
			}

			switch (formType) {
			case RE::FormType::None:
				ImGui::TableSetupColumn("类型", ImGuiTableColumnFlags_WidthFixed, 40, PlayerInfoColumnID_5);
				break;
			default:
				break;
			}

			if (show_items_window_formid) {
				ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_4);
			}
			if (show_items_window_settings) {
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_6);
			}
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					ItemInfo item = items[row_n];
					ImGui::PushID(item.formId + 0x1000000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if (item.isHarvested && (formType == RE::FormType::Flora || formType == RE::FormType::Tree)) {
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s[已收获]", item.name.c_str());
					} else if (item.isCrime) {
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
								} else {
									if (!item.isDeleted) {
										if (!item.ptr->IsMarkedForDeletion()) {
											auto player = RE::PlayerCharacter::GetSingleton();
											player->PickUpObject(item.ptr, 1);
											char buf[32];
											snprintf(buf, 32, "%s 已拾取", item.ptr->GetDisplayFullName());
											RE::DebugNotification(buf, NULL, false);
											item.isDeleted = true;
										}
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
								} else if (formType == RE::FormType::Activator) {
									if (!(item.ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
										auto acti = item.ptr->GetBaseObject()->As<RE::TESObjectACTI>();
										if (acti) {
											auto player = RE::PlayerCharacter::GetSingleton();
											acti->Activate(item.ptr, player, 0, acti, 1);
										}
									}
								} else {
									if (!item.isDeleted) {
										if (!item.ptr->IsMarkedForDeletion()) {
											auto player = RE::PlayerCharacter::GetSingleton();
											player->PickUpObject(item.ptr, 1);
											char buf[32];
											snprintf(buf, 32, "%s 已拾取", item.ptr->GetDisplayFullName());
											RE::DebugNotification(buf, NULL, false);
											item.isDeleted = true;
										}
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
					case RE::FormType::Activator:
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
					case RE::FormType::Activator:
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

					if (show_items_window_formid) {
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.baseFormIdStr.c_str());
					}

					if (show_items_window_settings) {
						ImGui::TableNextColumn();
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

							// 调用控制台
							const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
							const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
							if (script) {
								const auto selectedRef = RE::Console::GetSelectedRef();
								script->SetCommand(commandStr);
								script->CompileAndRun(selectedRef.get());
								delete script;
							}
							if (activeItems) {
								activeItems = false;
							}
						}
					}

					ImGui::PopID();
				}
			ImGui::EndTable();
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
		if (ImGui::BeginTable("table_sorting", 6, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * show_inv_window_height), 0.0f)) {
			ImGui::TableSetupColumn("已装备", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_ID);
			ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 100.0f, PlayerInfoColumnID_1);
			ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 40.0f, PlayerInfoColumnID_2);
			ImGui::TableSetupColumn("重量", ImGuiTableColumnFlags_WidthFixed, 40, PlayerInfoColumnID_3);
			ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 100.0f, PlayerInfoColumnID_4);
			//ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_WidthFixed, 0.0f, PlayerInfoColumnID_5);
			ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(count);
			while (clipper.Step())
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
					// Display a data item
					//InventoryInfo* item = getPlayerInvData(row_n);
					InventoryInfo item = inv[row_n];
					//if (item) {
					ImGui::PushID(row_n + 7000);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.isWorn ? "×" : "");
					ImGui::TableNextColumn();

					if (ImGui::Selectable(item.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
						//if (ImGui::IsMouseDoubleClicked(0)) {
						if (item.ptr->IsAmmo() || item.ptr->IsArmor() || item.ptr->IsWeapon()) {
							RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
							auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
							if (item.isWorn) {
								actorEquipManager->UnequipObject(player, item.ptr, item.invExtraPtr, 1);
								item.isWorn = false;
							} else {
								actorEquipManager->EquipObject(player, item.ptr, item.invExtraPtr, 1);
								item.isWorn = true;
							}
						}
						//}
					}
					ImGui::TableNextColumn();
					//if (isShowFriend && isShowEnemy) {
					ImGui::Text("%d", item.count);
					//}
					ImGui::TableNextColumn();
					//if (isAim) {
					ImGui::Text("%.1f", item.weight);
					//}
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.formIdStr.c_str());
					//ImGui::TableNextColumn();
					//ImGui::Text("0x%X", item->address);
					ImGui::PopID();
					//}
				}
			ImGui::EndTable();
		}
	}

	void __fastcall render()
	{
		//第一次加载游戏后
		if (!startflag && !active) {
			return;
		}

		// 当打开菜单时不显示
		if (isOpenCursorMenu && !active) {
			return;
		}
		// 当打开菜单时不显示
		if (isMainMenu && !active) {
			return;
		}
		// 当打开菜单时不显示
		if (isLoadWaitSpinner && !active) {
			return;
		}
		// 当打开菜单时不显示
		if (isFaderMenu && !active) {
			return;
		}

		if (isGameLoading && !active) {
			return;
		}

		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)
			window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_resize)
			window_flags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)
			window_flags |= ImGuiWindowFlags_NoCollapse;
		if (no_background)
			window_flags |= ImGuiWindowFlags_NoBackground;
		if (auto_resize)
			window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		auto style = ImGui::GetStyle();
		style.WindowBorderSize = window_border ? 1.0f : 0.0f;
		style.FrameBorderSize = frame_border ? 1.0f : 0.0f;

		if (show_player_base_info_window) {
			if (show_player_base_info_window_sep) {
				progress = playerInfo.kHealth / (playerInfo.kHealthBase == 0 ? 1 : playerInfo.kHealthBase);
				progress2 = playerInfo.kStamina / (playerInfo.kStaminaBase == 0 ? 1 : playerInfo.kStaminaBase);
				progress3 = playerInfo.kMagicka / (playerInfo.kMagickaBase == 0 ? 1 : playerInfo.kMagickaBase);

				ImGui::Begin("生命", nullptr, window_flags);

				ImGui::Text(ICON_MDI_HEART_HALF_FULL " 生命:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress > 0.15) {
					myText2("%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
				}

				ImGui::End();
				ImGui::Begin("耐力", nullptr, window_flags);

				ImGui::Text(ICON_MDI_RUN_FAST " 耐力:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress2 > 0.15) {
					myText2("%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
				}
				ImGui::End();
				ImGui::Begin("魔法", nullptr, window_flags);

				ImGui::Text(ICON_MDI_STAR_FOUR_POINTS_OUTLINE " 魔法:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress3 > 0.15) {
					myText2("%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
				}
				ImGui::End();

			} else {
				ImGui::Begin("人物状态", nullptr, window_flags);

				progress = playerInfo.kHealth / (playerInfo.kHealthBase == 0 ? 1 : playerInfo.kHealthBase);
				progress2 = playerInfo.kStamina / (playerInfo.kStaminaBase == 0 ? 1 : playerInfo.kStaminaBase);
				progress3 = playerInfo.kMagicka / (playerInfo.kMagickaBase == 0 ? 1 : playerInfo.kMagickaBase);

				if (flag_process) {
					myText(ICON_MDI_HEART_HALF_FULL " 生命:");
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					char buf[32];
					sprintf(buf, "%d/%d", (int)playerInfo.kHealth, (int)playerInfo.kHealthBase);
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgress);
					ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

					myText(ICON_MDI_RUN_FAST " 耐力:");
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					char buf2[32];
					sprintf(buf2, "%d/%d", (int)playerInfo.kStamina, (int)playerInfo.kStaminaBase);
					ImGui::ProgressBar(progress2, ImVec2(0.f, 0.f), buf2);

					myText(ICON_MDI_STAR_FOUR_POINTS_OUTLINE " 魔法:");
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					char buf3[32];
					sprintf(buf3, "%d/%d", (int)playerInfo.kMagicka, (int)playerInfo.kMagickaBase);
					ImGui::ProgressBar(progress3, ImVec2(0.f, 0.f), buf3);
					ImGui::PopStyleColor();

				} else {
					myText(ICON_MDI_HEART_HALF_FULL " 生命:");
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
					if (progress > 0.15) {
						myText2("%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
					} else {
						myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
					}
					myText(ICON_MDI_RUN_FAST " 耐力:");
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
					if (progress2 > 0.15) {
						myText2("%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
					} else {
						myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
					}
					myText(ICON_MDI_STAR_FOUR_POINTS_OUTLINE " 魔法:");
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
					if (progress3 > 0.15) {
						myText2("%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
					} else {
						myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
					}
				}
				ImGui::End();
			}
		}

		if (show_player_info_window) {
			ImGui::Begin("人物属性", nullptr, window_flags);

			//myText2("Health:%d", (int)health);
			//myText2("Magicka:%d", (int)kMagicka);
			//myText2("Stamina:%d", (int)kStamina);
			ImGui::Text(ICON_MDI_SWORD " 伤害: %s", playerInfo.DamageStr.c_str());
			//myText("防御: %s", playerInfo.DamageResist.c_str());
			ImGui::Text(ICON_MDI_SHIELD_HALF_FULL " 防御: %.2f", playerInfo.kDamageResist);
			ImGui::Text(ICON_MDI_WATER " 毒抗: %.1f%%", playerInfo.kPoisonResist);
			ImGui::Text(ICON_MDI_FIRE " 火抗: %.1f%%", playerInfo.kResistFire);
			ImGui::Text(ICON_MDI_LIGHTNING_BOLT_OUTLINE " 雷抗: %.1f%%", playerInfo.kResistShock);
			ImGui::Text(ICON_MDI_SNOWFLAKE " 冰抗: %.1f%%", playerInfo.kResistFrost);
			ImGui::Text(ICON_MDI_MAGIC_STAFF " 法抗: %.1f%%", playerInfo.kResistMagic);
			ImGui::Text(ICON_MDI_PILL " 抵抗力: %.1f%%", playerInfo.kResistDisease);
			//myText2("criticalChance:%d", (int)kCriticalChance);
			//myText2("armor:%d", (int)kDamageResist);
			ImGui::End();
		}

		if (show_player_mod_window) {
			ImGui::Begin("属性加成", nullptr, window_flags);

			ImGui::Text(ICON_MDI_AXE " 单手: %.1f%%", playerInfo.kOneHandedModifier);
			ImGui::Text(ICON_MDI_AXE_BATTLE " 双手: %.1f%%", playerInfo.kTwoHandedModifier);
			ImGui::Text(ICON_MDI_BOW_ARROW " 弓箭: %.1f%%", playerInfo.kMarksmanModifier);
			ImGui::Text(ICON_MDI_ANVIL " 锻造: %.1f%%", playerInfo.kSmithingModifier);
			ImGui::Text(ICON_MDI_ANVIL " 锻造(power): %.1f%%", playerInfo.kSmithingPowerModifier);
			ImGui::Text(ICON_MDI_ANVIL " 锻造(skill): %.1f%%", playerInfo.kSmithingSkillAdvance);
			ImGui::Text(ICON_MDI_BOWL_MIX " 炼金: %.1f%%", playerInfo.kAlchemyModifier);
			ImGui::Text(ICON_MDI_BOWL_MIX " 炼金(power): %.1f%%", playerInfo.kAlchemyPowerModifier);
			ImGui::Text(ICON_MDI_BOWL_MIX " 炼金(skill): %.1f%%", playerInfo.kAlchemySkillAdvance);
			ImGui::Text(ICON_MDI_FLASH " 附魔: %.1f%%", playerInfo.kEnchantingModifier);
			ImGui::Text(ICON_MDI_FLASH " 附魔(power): %.1f%%", playerInfo.kEnchantingPowerModifier);
			ImGui::Text(ICON_MDI_FLASH " 附魔(skill): %.1f%%", playerInfo.kEnchantingSkillAdvance);
			ImGui::End();
		}

		if (show_player_weapon_window) {
			ImGui::Begin("武器", nullptr, window_flags);

			if (leftWeaponInfo.isExist) {
				if (active) {
					if (ImGui::TreeNode(leftWeaponInfo.treeId.c_str())) {
						myText("类型: %s", leftWeaponInfo.formTypeName.c_str());
						myText("ID: %s", leftWeaponInfo.formID.c_str());
						ImGui::SameLine(0, 0);
						if (ImGui::SmallButton("卸载")) {
							std::string commandStr = "player.unequipItem ";
							commandStr.append(leftWeaponInfo.formID);

							// 调用控制台
							const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
							const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
							if (script) {
								const auto selectedRef = RE::Console::GetSelectedRef();
								script->SetCommand(commandStr);
								script->CompileAndRun(selectedRef.get());
								delete script;
							}
						}

						switch (leftWeaponInfo.formType) {
						case RE::FormType::Weapon:
							{
								myText("武器类型: %s", leftWeaponInfo.weaponTypeName.c_str());
								myText("武器伤害: %d", leftWeaponInfo.damage);
								myText("暴击伤害: %d", leftWeaponInfo.critDamage);
								myText("价格: %d", leftWeaponInfo.goldValue);
								myText("属性值: %d$", leftWeaponInfo.value);
								myText("重量: %.1f", leftWeaponInfo.weight);
								break;
							}
						case RE::FormType::Spell:
							{
								myText("法术类型: %s", leftWeaponInfo.castingTypeName.c_str());
								myText("法术类型: %s", leftWeaponInfo.spellTypeName.c_str());
								myText("花费: %.1f", leftWeaponInfo.cost);
								myText("施法时间: %.1f", leftWeaponInfo.time);
								break;
							}

						default:
							break;
						}

						//myText("插槽: %s", item.equipSlotName.c_str());
						ImGui::TreePop();
					}
				} else {
					myText(ICON_MDI_SWORD " %s", leftWeaponInfo.treeId.c_str());
				}
			}

			if (rightWeaponInfo.isExist) {
				// 不显示双手武器
				if (!rightWeaponInfo.isTwoHand) {
					if (active) {
						if (ImGui::TreeNode(rightWeaponInfo.treeId.c_str())) {
							myText("类型: %s", rightWeaponInfo.formTypeName.c_str());
							myText("ID: %s", rightWeaponInfo.formID.c_str());
							ImGui::SameLine(0, 0);
							if (ImGui::SmallButton("卸载")) {
								std::string commandStr = "player.unequipItem ";
								commandStr.append(rightWeaponInfo.formID);

								// 调用控制台
								const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
								const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
								if (script) {
									const auto selectedRef = RE::Console::GetSelectedRef();
									script->SetCommand(commandStr);
									script->CompileAndRun(selectedRef.get());
									delete script;
								}
							}

							switch (rightWeaponInfo.formType) {
							case RE::FormType::Weapon:
								{
									myText("武器类型: %s", rightWeaponInfo.weaponTypeName.c_str());
									myText("武器伤害: %d", rightWeaponInfo.damage);
									myText("暴击伤害: %d", rightWeaponInfo.critDamage);
									myText("价格: %d", rightWeaponInfo.goldValue);
									myText("属性值: %d$", rightWeaponInfo.value);
									myText("重量: %.1f", rightWeaponInfo.weight);
									break;
								}
							case RE::FormType::Spell:
								{
									myText("法术类型: %s", rightWeaponInfo.castingTypeName.c_str());
									myText("法术类型: %s", rightWeaponInfo.spellTypeName.c_str());
									myText("花费: %.1f", rightWeaponInfo.cost);
									myText("施法时间: %.1f", rightWeaponInfo.time);
									break;
								}
							default:
								break;
							}

							ImGui::TreePop();
						}
					} else {
						myText(ICON_MDI_SWORD " %s", rightWeaponInfo.treeId.c_str());
					}
				}
			}

			if (ammoInfo.isExist) {
				if (active) {
					if (ImGui::TreeNode(ammoInfo.treeId.c_str())) {
						myText("类型: %s", ammoInfo.formTypeName.c_str());
						myText("ID: %s", ammoInfo.formID.c_str());
						ImGui::SameLine(0, 0);
						if (ImGui::SmallButton("卸载")) {
							std::string commandStr = "player.unequipItem ";
							commandStr.append(ammoInfo.formID);

							// 调用控制台
							const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
							const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
							if (script) {
								const auto selectedRef = RE::Console::GetSelectedRef();
								script->SetCommand(commandStr);
								script->CompileAndRun(selectedRef.get());
								delete script;
							}
						}
						myText("伤害: %.1f", ammoInfo.damage);
						myText("价格: %d", ammoInfo.goldValue);
						myText("属性值: %d$", ammoInfo.value);
						myText("重量: %.1f", ammoInfo.weight);

						ImGui::TreePop();
					}
				} else {
					myText(ICON_MDI_ARROW_PROJECTILE " %s", ammoInfo.treeId.c_str());
				}
			}

			ImGui::End();
		}

		if (show_player_armor_window) {
			ImGui::Begin("装备", nullptr, window_flags);

			int unWornArmosAlertSlots = 0;

			for (int i = 0; i <= 31; i++) {
				auto item = wornArmos[i];

				if (item.isExist) {
					if (item.isSpeacilSlotAlert) {
						// 变黄色
						//imgui.igPushStyleColor
					}

					if (active) {
						if (ImGui::TreeNode(item.treeId.c_str())) {
							myText("类型: %s", item.formTypeName.c_str());
							myText("装备类型: %s", item.armorTypeName.c_str());
							myText("ID: %s", item.formID.c_str());
							ImGui::SameLine(0, 0);
							if (ImGui::SmallButton("卸载")) {
								std::string commandStr = "player.unequipItem ";
								commandStr.append(item.formID);

								// 调用控制台
								const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
								const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
								if (script) {
									const auto selectedRef = RE::Console::GetSelectedRef();
									script->SetCommand(commandStr);
									script->CompileAndRun(selectedRef.get());
									delete script;
								}
							}
							myText("价格: %d$", item.goldValue);
							myText("属性值: %d", item.value);
							myText("装备等级: %.2f", item.armorRating);
							myText("插槽: %s", item.equipSlotName.c_str());
							myText("重量: %.1f", item.weight);
							ImGui::TreePop();
						}
					} else {
						myText("%s", item.treeId.c_str());
					}

				} else {
					if (item.isMainSlotAlert) {
						unWornArmosAlertSlots += 1 << i;
						// 标记未装备
					}
				}
			}

			// 未装备插槽显示红色未装备
			for (int i = 0; i <= 31; i++) {
				int mask = 1 << i;
				if ((unWornArmosAlertSlots & mask) == mask) {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "未装备 | %s", wornArmos[i].equipSlotName.c_str());
				}
			}

			ImGui::End();
		}

		if (show_player_debug_window) {
			ImGui::Begin("其他信息", nullptr, window_flags);
			ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS " 位置: %s", playerInfo.location.c_str());
			// z高度
			ImGui::Text(ICON_MDI_AXIS_ARROW " 坐标: [%.0f,%.0f,%.0f]", playerInfo.Position.x, playerInfo.Position.y, playerInfo.Position.z);
			//ImGui::SameLine(0, 0);
			// z 是0~2Π
			ImGui::Text(ICON_MDI_ANGLE_ACUTE " 视角: [%.2f,%.2f]", playerInfo.Angle.x, playerInfo.Angle.z);
			ImGui::End();
		}

		// 弃用
		if (show_enemy_window && false) {
			time_t now1 = time(NULL);
			ImGui::Begin("敌人信息", nullptr, window_flags);
			EnemyInfo* enemyInfo = getEnemyData();
			for (int i = 0; i < 50; i++) {
				EnemyInfo& item = enemyInfo[i];
				if (item.kHealth > 0) {
					if (now1 - item.updateTime < 40) {
						float enemyHealthRate = item.kHealth / (item.kHealthBase == 0 ? 1 : item.kHealthBase);

						myText("[%d] %s [", item.level, item.name.c_str());
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						if (enemyHealthRate > 0.85f) {
							myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%.0f/%.0f ", item.kHealth, item.kHealthBase);
						} else if (enemyHealthRate < 0.20f) {
							myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "%.0f/%.0f ", item.kHealth, item.kHealthBase);
						} else {
							myText2("%.0f/%.0f ", item.kHealth, item.kHealthBase);
						}
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						myText2("%.0f ", item.kStamina);
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						myTextColored(ImVec4(0.0f, 0, 1, 1.0f), "%.0f", item.kMagicka);
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						myText2("] %d", item.isSentient);
						myText2("]");
					}
				}
			}
			ImGui::End();
		}

		if (show_npc_window) {
			ImGui::Begin("npc信息", nullptr, window_flags);
			//if (!isRefreshActorInfo) {
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
			//}
			ImGui::End();
		}

		if (show_inv_window) {
			if (active || !show_inv_window_active) {
				//static ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("data\\skse\\plugins\\materialdesignicons-webfont.ttf", 14);

				ImGui::Begin("防具信息", nullptr, window_flags);  //window_flags&(~ImGuiWindowFlags_AlwaysAutoResize)

				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
					//if (ImGui::BeginTabItem("\uF04E5"  " 武器", 0, 0)) {

					/*		ImGui::Text(ICON_MDI_ABUGIDA_DEVANAGARI "  Paint");
					ImGui::Text("\U000F132A" "  Paint");*/

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

				myText("\uf0d6 %d", getPlayerGoldCount());
				//myText("负重：%0.1f/%0.0f", playerInfo.equippedWeight, playerInfo.carryWeight);

				ImGui::End();
			}
		}

		if (show_items_window) {
			ImGui::Begin("物品信息", nullptr, window_flags);

			ImGui::End();
		}

		if (activeItems) {
			//if (show_items_window && activeItems) {
			ImGui::Begin("附近物品信息", nullptr, window_flags);
			myText(ICON_MDI_MAP_MARKER_RADIUS " %s", playerInfo.location.c_str());
			if (ImGui::BeginTable("tableItem", 5)) {
				if (getItemCountWEAP() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_SWORD " 武器(%d)", getItemCountWEAP());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##41")) {
							takeAllItem(getItemCountWEAP(), getItemsWEAP(), RE::FormType::Weapon);
						}
					}
					buildItemInfo(getItemCountWEAP(), getItemsWEAP(), RE::FormType::Weapon);
				}
				if (getItemCountAMMO() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_ARROW_PROJECTILE " 弹药(%d)", getItemCountAMMO());
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
				}
				if (getItemCountBOOK() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_BOOK_OPEN_VARIANT " 书信(%d)", getItemCountBOOK());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##43")) {
							takeAllItem(getItemCountBOOK(), getItemsBOOK(), RE::FormType::Book);
						}
					}
					buildItemInfo(getItemCountBOOK(), getItemsBOOK(), RE::FormType::Book);
				}
				if (getItemCountALCH() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 药水(%d)", getItemCountALCH());
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
				}
				if (getItemCountFOOD() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_FOOD_DRUMSTICK " 食物(%d)", getItemCountFOOD());
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
				}
				if (getItemCountARMO() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_SHIELD " 装备(%d)", getItemCountARMO());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##47")) {
							takeAllItem(getItemCountARMO(), getItemsARMO(), RE::FormType::Armor);
						}
					}
					buildItemInfo(getItemCountARMO(), getItemsARMO(), RE::FormType::Armor);
				}
				if (getItemCountINGR() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_SOURCE_BRANCH " 材料(%d)", getItemCountINGR());
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
				}
				if (getItemCountSGEM() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_CARDS_DIAMOND " 灵魂石(%d)", getItemCountSGEM());
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
					ImGui::Text(ICON_MDI_KEY " 钥匙(%d)", getItemCountKEYM());

					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_ARCHIVE_ARROW_DOWN "##52")) {
							takeAllItem(getItemCountKEYM(), getItemsKEYM(), RE::FormType::KeyMaster);
						}
					}
					buildItemInfo(getItemCountKEYM(), getItemsKEYM(), RE::FormType::KeyMaster);
				}
				if (getItemCountMISC() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_PACKAGE_VARIANT_CLOSED " 杂项(%d)", getItemCountMISC());
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
				}
				if (getItemCountFLOR() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_BASKET_OUTLINE " 可收获(%d)", getItemCountFLOR());
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
				}
				if (getItemCountTREE() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_FLOWER_TULIP_OUTLINE " 植物(%d)", getItemCountTREE());
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
				}
				if (getItemCountACTI() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_COGS " 可互动(%d)", getItemCountACTI());

					buildItemInfo(getItemCountACTI(), getItemsACTI(), RE::FormType::Activator);
				}
				if (getItemCountACHR() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_HUMAN_MALE " 尸体(%d)", getItemCountACHR());

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
				}
				if (getItemCountCONT() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text(ICON_MDI_ARCHIVE_OUTLINE " 箱子(%d)", getItemCountCONT());
					if (show_items_window_settings) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::Checkbox("自动拾取##62", &show_items_window_auto_cont);
					} else {
						if (show_items_window_auto_cont) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(ICON_MDI_AUTORENEW);
						}
					}
					buildItemCONTInfo(getItemCountCONT(), getItemsCONT(), RE::FormType::Container);
				}
				if (getItemCount() > 0) {
					ImGui::TableNextColumn();
					ImGui::Text("其他：");
					buildItemInfo(getItemCount(), getItems(), RE::FormType::None);
				}

				ImGui::TableNextColumn();
				ImGui::Checkbox("更多", &show_items_window_settings);
				if (show_items_window_settings) {
					ImGui::SameLine(0.0f, 9.1f * ImGui::GetTextLineHeightWithSpacing());

					//if (ImGui::Button("保存设置##2", ImVec2(0, 0))) {
					if (ImGui::Button(ICON_MDI_CONTENT_SAVE " 保存设置##2")) {
						auto colorPlotHistogram = style.Colors[ImGuiCol_PlotHistogram];
						colorPlotHistogramX = colorPlotHistogram.x;
						colorPlotHistogramY = colorPlotHistogram.y;
						colorPlotHistogramZ = colorPlotHistogram.z;
						colorPlotHistogramW = colorPlotHistogram.w;

						save_settings();
					}

					{
						ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

						//ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
						ImGui::BeginChild("childItemsSetting", ImVec2(ImGui::GetTextLineHeightWithSpacing() * 15, ImGui::GetTextLineHeightWithSpacing() * (14.5f)), true, window_flags);

						if (ImGui::BeginTable("tableItemsSetting", 2)) {
							ImGui::TableNextColumn();
							ImGui::Checkbox("显示FORMID", &show_items_window_formid);
							ImGui::TableNextColumn();
							ImGui::Checkbox("显示方位", &show_items_window_direction);
							ImGui::EndTable();
						}

						ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
						ImGui::DragInt("扫描范围(本地)", &show_items_window_auto_dis_local, 1, 10, 2000, "%d米");
						ImGui::DragInt("扫描范围(天际)", &show_items_window_auto_dis_skyrim, 1, 20, 10000, "%d米");
						ImGui::PopItemWidth();

						ImGui::Checkbox("忽略项目", &show_items_window_ignore);
						if (show_items_window_ignore) {
							static ImGuiTableFlags flagsItem =
								ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

							const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
							if (ImGui::BeginTable("tableItemIngore", 3, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 12, TEXT_BASE_HEIGHT * 8), 0.0f)) {
								ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_1);
								ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 60, PlayerInfoColumnID_2);
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40, PlayerInfoColumnID_3);

								ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
								ImGui::TableHeadersRow();

								int deleteFormId = 0;

								ImGuiListClipper clipper;
								clipper.Begin(excludeForms.size());
								while (clipper.Step())
									for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
										ExcludeForm item = excludeForms[row_n];
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

						if (ImGui::TreeNodeEx(ICON_MDI_AUTORENEW " 自动拾取", ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
							ImGui::DragInt("拾取范围", &show_items_window_auto_dis, 1, 1, 100, "%d米");
							ImGui::PopItemWidth();

							if (ImGui::TreeNodeEx(ICON_MDI_HUMAN_MALE " 尸体物品过滤", ImGuiTreeNodeFlags_DefaultOpen)) {
								if (ImGui::BeginTable("tableItemsSettingACHR", 3)) {
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_ARROW_PROJECTILE " 弹药", &show_items_window_auto_achr_ammo);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 药水", &show_items_window_auto_achr_alch);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_FOOD_DRUMSTICK " 食物", &show_items_window_auto_achr_food);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_SOURCE_BRANCH " 材料", &show_items_window_auto_achr_ingr);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_CARDS_DIAMOND " 灵魂石", &show_items_window_auto_achr_sgem);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_CASH " 金钱", &show_items_window_auto_achr_gold);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_SCRIPT_TEXT " 卷轴", &show_items_window_auto_achr_scrl);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_KEY " 钥匙", &show_items_window_auto_achr_keym);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_PACKAGE_VARIANT_CLOSED " 杂项", &show_items_window_auto_achr_misc);
									ImGui::EndTable();
								}

								ImGui::TreePop();
							}

							if (ImGui::TreeNodeEx(ICON_MDI_ARCHIVE_OUTLINE " 容器物品过滤", ImGuiTreeNodeFlags_DefaultOpen)) {
								if (ImGui::BeginTable("tableItemsSettingCONT", 3)) {
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_ARROW_PROJECTILE " 弹药", &show_items_window_auto_cont_ammo);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " 药水", &show_items_window_auto_cont_alch);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_FOOD_DRUMSTICK " 食物", &show_items_window_auto_cont_food);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_SOURCE_BRANCH " 材料", &show_items_window_auto_cont_ingr);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_CARDS_DIAMOND " 灵魂石", &show_items_window_auto_cont_sgem);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_CASH " 金钱", &show_items_window_auto_cont_gold);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_SCRIPT_TEXT " 卷轴", &show_items_window_auto_cont_scrl);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_KEY " 钥匙", &show_items_window_auto_cont_keym);
									ImGui::TableNextColumn();
									ImGui::Checkbox(ICON_MDI_PACKAGE_VARIANT_CLOSED " 杂项", &show_items_window_auto_cont_misc);
									ImGui::EndTable();
								}

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
									ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 80, PlayerInfoColumnID_1);
									ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 60, PlayerInfoColumnID_2);
									ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40, PlayerInfoColumnID_3);

									ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
									ImGui::TableHeadersRow();

									int deleteFormId = 0;

									ImGuiListClipper clipper;
									clipper.Begin(excludeLocationForms.size());
									while (clipper.Step())
										for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
											ExcludeForm item = excludeLocationForms[row_n];
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

						ImGui::EndChild();
					}
				}

				ImGui::EndTable();
			}

			ImGui::End();
		}

		//if (show_crosshair) {
		//	ImGui::Begin("准星信息", nullptr, window_flags);
		//	RE::TESObjectREFR* _cachedRef = BSTCrosshairRefEvent::GetSingleton()->GetCrosshairReference();
		//	if (_cachedRef) {
		//		logger::debug("准星");
		//		logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetDisplayFullName()));
		//		logger::debug(_cachedRef->GetFormID());
		//	}

		//	ImGui::End();
		//}

		if (active) {
			static bool show_demo_window = false;
			static bool show_another_window = false;

			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			{
				ImGui::Begin("Setting##0", nullptr, 0);

				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
					if (ImGui::BeginTabItem(ICON_MDI_TOOLS " MOD设置", 0, 0)) {
						if (ImGui::TreeNodeEx(ICON_MDI_TABLE_OF_CONTENTS " HUD设置", ImGuiTreeNodeFlags_DefaultOpen)) {
							//ImGui::BeginGroup();

							if (ImGui::BeginTable("split", 2)) {
								ImGui::TableNextColumn();
								ImGui::Checkbox("人物基本属性", &show_player_base_info_window);

								if (show_player_base_info_window) {
									if (ImGui::TreeNodeEx("设置##3", ImGuiTreeNodeFlags_DefaultOpen)) {
										ImGui::Checkbox("独立显示", &show_player_base_info_window_sep);
										if (!show_player_base_info_window_sep) {
											ImGui::Checkbox("显示进度条", &flag_process);
											if (flag_process) {
												if (ImGui::ColorEdit4("进度条颜色", &colorProgress.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf))
													;
											}
										}
										ImGui::TreePop();
									}
								}

								ImGui::TableNextColumn();
								ImGui::TableNextColumn();
								ImGui::Checkbox("人物属性", &show_player_info_window);
								ImGui::TableNextColumn();
								ImGui::Checkbox("人物属性2", &show_player_mod_window);
								ImGui::TableNextColumn();
								ImGui::Checkbox("武器信息", &show_player_weapon_window);
								ImGui::TableNextColumn();
								ImGui::Checkbox("防具信息", &show_player_armor_window);
								ImGui::TableNextColumn();
								ImGui::Checkbox("其他信息", &show_player_debug_window);
								ImGui::TableNextColumn();
								ImGui::Checkbox("物品信息(预留)", &show_items_window);
								ImGui::TableNextColumn();
								//ImGui::Checkbox("敌人信息", &show_enemy_window);
								ImGui::Checkbox("NPC信息", &show_npc_window);
								if (show_npc_window) {
									if (ImGui::TreeNodeEx("设置##1", ImGuiTreeNodeFlags_DefaultOpen)) {
										ImGui::Checkbox("只显示距离内NPC", &show_npc_window_dis);
										if (show_npc_window_dis) {
											ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
											ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
											ImGui::DragInt("##距离", &show_npc_window_dis_meter, 1, 10, 100, "%d米内");
											ImGui::PopItemWidth();
										}
										ImGui::Checkbox("显示FORMID", &show_npc_window_formid);
										ImGui::Checkbox("显示方向和距离", &show_npc_window_direction);
										ImGui::Checkbox("死亡不显示", &show_npc_window_dead_hidden);

										ImGui::TreePop();
									}
								}
								ImGui::TableNextColumn();
								ImGui::Checkbox("物品栏信息", &show_inv_window);
								if (show_inv_window) {
									if (ImGui::TreeNodeEx("设置##2", ImGuiTreeNodeFlags_DefaultOpen)) {
										ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
										ImGui::DragInt("显示数据", &show_inv_window_height, 1, 15, 80, "%d条");
										ImGui::Checkbox("激活菜单时显示", &show_inv_window_active);
										ImGui::PopItemWidth();
										ImGui::TreePop();
									}
								}

								ImGui::EndTable();
							}

							ImGui::Separator();
							ImGui::Checkbox("是否自动卸除箭袋", &auto_remove_ammo);
							if (auto_remove_ammo) {
								ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
								ImGui::Checkbox("是否记住箭矢类型", &remember_last_ammo);
							}
							// 测试
							//ImGui::Checkbox("Demo", &show_demo_window);
							//if (ImGui::Button("测试", ImVec2())) {
							//	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
							//	auto currentLocation = player->currentLocation;
							//	// 输出类型
							//	logger::debug(StringUtil::Utf8ToGbk(GetFormTypeName(currentLocation->formType.underlying()).c_str()));

							//}
							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::TreeNodeEx(ICON_MDI_DOCK_WINDOW " 窗口设置", ImGuiTreeNodeFlags_DefaultOpen)) {
							//ImGui::BeginGroup();

							static int style_idx = imgui_style_index;
							if (ImGui::Combo("主题", &style_idx, "Dark\0Light\0Classic\0", -1)) {
								switch (style_idx) {
								case 0:
									ImGui::StyleColorsDark(&ImGui::GetStyle());
									imgui_style_index = 0;
									break;
								case 1:
									ImGui::StyleColorsLight(&ImGui::GetStyle());
									imgui_style_index = 1;
									break;
								case 2:
									ImGui::StyleColorsClassic(&ImGui::GetStyle());
									imgui_style_index = 2;
									break;
								}
							}

							if (ImGui::BeginTable("split", 3)) {
								ImGui::TableNextColumn();
								ImGui::Checkbox("不显示标题", &no_titlebar);
								ImGui::TableNextColumn();
								ImGui::Checkbox("不允许缩放", &no_resize);
								ImGui::TableNextColumn();
								ImGui::Checkbox("不允许折叠", &no_collapse);
								ImGui::TableNextColumn();
								ImGui::Checkbox("不显示背景", &no_background);
								ImGui::TableNextColumn();
								ImGui::Checkbox("窗体边框", &window_border);
								ImGui::TableNextColumn();
								ImGui::Checkbox("控件边框", &frame_border);
								ImGui::TableNextColumn();
								ImGui::Checkbox("是否窗口自适应", &auto_resize);
								/*ImGui::TableNextColumn();
								ImGui::Checkbox("不显示圆形", &bullet_text);*/
								ImGui::EndTable();
							}

							ImGui::PushItemWidth(ImGui::GetFontSize() * 6);

							ImGui::DragFloat("窗体缩放", &ImGui::GetIO().FontGlobalScale, 0.005f, 0.5f, 1.8f, "%.2f", 1);
							ImGui::Combo("窗口热键", &hotkey, "Insert\0F11\0F12\0Shift+Q\0Alt+Q\0", -1);
							ImGui::Combo("物品菜单热键", &hotkey2, "Insert\0F11\0F12\0Shift+Q\0Alt+Q\0", -1);
							//ImGui::DragInt("数据刷新(ms)", &refresh_time_data, 1, 100, 500, "%d ms");
							ImGui::PopItemWidth();

							//ImGui::EndGroup();
							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::Button(ICON_MDI_CONTENT_SAVE " 保存配置", ImVec2(0, 0))) {
							auto colorPlotHistogram = style.Colors[ImGuiCol_PlotHistogram];
							colorPlotHistogramX = colorPlotHistogram.x;
							colorPlotHistogramY = colorPlotHistogram.y;
							colorPlotHistogramZ = colorPlotHistogram.z;
							colorPlotHistogramW = colorPlotHistogram.w;

							save_settings();
						}
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("v0.3.2-beta").x - 2);
						myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "v0.3.2-beta");
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(ICON_MDI_ACCOUNT_EDIT " 属性修改", 0, 0)) {
						RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
						if (ImGui::TreeNodeEx("属性修改(临时)", ImGuiTreeNodeFlags_DefaultOpen)) {
							myText("修改项:");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Combo("##cbActorValue", &stateMod_selectIndex, actorValues, ((int)(sizeof(actorValues) / sizeof(*(actorValues)))), 6);
							if (player) {
								if (stateMod_selectIndex >= 0) {
									stateMod_nowValue = player->GetActorValue(actorValuesIndex[stateMod_selectIndex]);
								}
							}
							myText("当前值: [%.1f]", stateMod_nowValue);
							myText("修改值:");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							if (stateMod_lastSelectIndex != stateMod_selectIndex) {
								stateMod_newValue = stateMod_nowValue;
								stateMod_lastSelectIndex = stateMod_selectIndex;
							}
							ImGui::InputFloat("##newValue", &stateMod_newValue, 10, 100, "%.1f", 0);

							if (ImGui::Button("确认修改", ImVec2())) {
								if (player) {
									if (stateMod_selectIndex >= 0) {
										player->SetActorValue(actorValuesIndex[stateMod_selectIndex], stateMod_newValue);
									}
								}
							}
							ImGui::TreePop();
						}
						if (ImGui::TreeNodeEx("属性修改(永久)", ImGuiTreeNodeFlags_DefaultOpen)) {
							myText("修改项:");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Combo("##cbActorValuePer", &statePerMod_selectIndex, perActorValues, ((int)(sizeof(perActorValues) / sizeof(*(perActorValues)))), 6);
							if (player) {
								if (statePerMod_selectIndex >= 0) {
									statePerMod_nowValue = player->GetActorValue(perActorValuesIndex[statePerMod_selectIndex]);
								}
							}
							myText("当前值: [%.1f]", statePerMod_nowValue);
							myText("修改值:");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							if (statePerMod_lastSelectIndex != statePerMod_selectIndex) {
								statePerMod_newValue = statePerMod_nowValue;
								statePerMod_lastSelectIndex = statePerMod_selectIndex;
							}
							ImGui::InputFloat("##newValuePer", &statePerMod_newValue, 10, 100, "%.1f", 0);

							if (ImGui::Button("确认修改", ImVec2())) {
								if (player) {
									if (statePerMod_selectIndex >= 0) {
										player->SetActorValue(perActorValuesIndex[statePerMod_selectIndex], statePerMod_newValue);
									}
								}
							}
							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("常用素材获取", ImGuiTreeNodeFlags_DefaultOpen)) {
							myText("获取项:");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Combo("##cbInvGet", &getInv_selectIndex, invValues, ((int)(sizeof(invValues) / sizeof(*(invValues)))), 6);
							myText("数量:");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::InputInt("##InvCount", &getInv_nowValue, 10, 9999);

							if (ImGui::Button("确认获取", ImVec2())) {
								std::string commandStr = "player.additem ";
								commandStr.append(invValuesCode[getInv_selectIndex]);
								commandStr.append(" ");
								commandStr.append(std::to_string(getInv_nowValue));

								// 调用控制台
								const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
								const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
								if (script) {
									const auto selectedRef = RE::Console::GetSelectedRef();
									script->SetCommand(commandStr);
									script->CompileAndRun(selectedRef.get());
									delete script;
								}
							}
							ImGui::TreePop();
						}

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				ImGui::End();
			}
		}

		if (deleteREFRs.size() > 0) {
			RE::PlayerCharacter* player2 = RE::PlayerCharacter::GetSingleton();
			for (auto item : deleteREFRs) {
				player2->PickUpObject(item, 1);
				//sprintf(buf, "%s 已自动拾取", item->GetDisplayFullName());
				char buf[40];
				snprintf(buf, 40, "%s 已自动拾取", item->GetDisplayFullName());
				RE::DebugNotification(buf, NULL, false);
			}
			deleteREFRs.clear();
		}
	}

	bool load_settings()
	{
		try {
			std::ifstream i(settings);
			nlohmann::json json;
			i >> json;

			if (json.contains("imguiSetting")) {
				auto const& j = json["imguiSetting"];
				if (j.contains("no_titlebar")) {
					no_titlebar = j["no_titlebar"].get<bool>();
					no_collapse = j["no_collapse"].get<bool>();
					no_background = j["no_background"].get<bool>();
					no_resize = j["no_resize"].get<bool>();
					imgui_style_index = j["imgui_style_index"].get<int>();
				}
				if (j.contains("auto_resize")) {
					auto_resize = j["auto_resize"].get<bool>();
				}
				if (j.contains("window_border")) {
					window_border = j["window_border"].get<bool>();
				}
				if (j.contains("frame_border")) {
					frame_border = j["frame_border"].get<bool>();
				}
				if (j.contains("bullet_text")) {
					bullet_text = j["bullet_text"].get<bool>();
				}
				if (j.contains("hotkey")) {
					hotkey = j["hotkey"].get<int>();
				}
				if (j.contains("hotkey2")) {
					hotkey2 = j["hotkey2"].get<int>();
				}
				if (j.contains("font_scale")) {
					font_scale = j["font_scale"].get<float>();
				}
			}

			if (json.contains("gameSetting")) {
				auto const& j = json["gameSetting"];
				if (j.contains("auto_remove_ammo")) {
					auto_remove_ammo = j["auto_remove_ammo"].get<bool>();
				}
				if (j.contains("remember_last_ammo")) {
					remember_last_ammo = j["remember_last_ammo"].get<bool>();
				}
			}

			if (json.contains("windowSetting")) {
				auto const& j = json["windowSetting"];
				if (j.contains("ArmorInfo")) {
					auto const& j2 = j["ArmorInfo"];
					show_player_armor_window = j2["isShow"].get<bool>();
				}
				if (j.contains("WeaponInfo")) {
					auto const& j2 = j["WeaponInfo"];
					show_player_weapon_window = j2["isShow"].get<bool>();
				}
				if (j.contains("playerInfo")) {
					auto const& j2 = j["playerInfo"];
					show_player_info_window = j2["isShow"].get<bool>();
				}
				if (j.contains("DebugInfo")) {
					auto const& j2 = j["DebugInfo"];
					show_player_debug_window = j2["isShow"].get<bool>();
				}
				if (j.contains("playerModInfo")) {
					auto const& j2 = j["playerModInfo"];
					show_player_mod_window = j2["isShow"].get<bool>();
				}
				/*			if (j.contains("EnemyInfo")) {
					auto const& j2 = j["EnemyInfo"];
					show_enemy_window = j2["isShow"].get<bool>();
				}*/
				if (j.contains("NpcInfo")) {
					auto const& j2 = j["NpcInfo"];
					show_npc_window = j2["isShow"].get<bool>();

					if (j2.contains("show_npc_window_dis")) {
						show_npc_window_dis = j2["show_npc_window_dis"].get<bool>();
					}
					if (j2.contains("show_npc_window_direction")) {
						show_npc_window_direction = j2["show_npc_window_direction"].get<bool>();
					}
					if (j2.contains("show_npc_window_dead_hidden")) {
						show_npc_window_dead_hidden = j2["show_npc_window_dead_hidden"].get<bool>();
					}

					if (j2.contains("show_npc")) {
						show_npc = j2["show_npc"].get<bool>();
					}
					if (j2.contains("show_enemy")) {
						show_enemy = j2["show_enemy"].get<bool>();
					}
					if (j2.contains("show_teammate")) {
						show_teammate = j2["show_teammate"].get<bool>();
					}
					if (j2.contains("show_horse")) {
						show_horse = j2["show_horse"].get<bool>();
					}
				}
				if (j.contains("ItemMenuInfo")) {
					auto const& j2 = j["ItemMenuInfo"];

					if (j2.contains("show_items_window_auto_dis")) {
						show_items_window_auto_dis = j2["show_items_window_auto_dis"].get<int>();
					}
					if (j2.contains("show_items_window_auto_dis_skyrim")) {
						show_items_window_auto_dis_skyrim = j2["show_items_window_auto_dis_skyrim"].get<int>();
					}
					if (j2.contains("show_items_window_auto_dis_local")) {
						show_items_window_auto_dis_local = j2["show_items_window_auto_dis_local"].get<int>();
					}

					if (j2.contains("show_items_window_direction")) {
						show_items_window_direction = j2["show_items_window_direction"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_ammo")) {
						show_items_window_auto_ammo = j2["show_items_window_auto_ammo"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_flor")) {
						show_items_window_auto_flor = j2["show_items_window_auto_flor"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_tree")) {
						show_items_window_auto_tree = j2["show_items_window_auto_tree"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_food")) {
						show_items_window_auto_food = j2["show_items_window_auto_food"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_ingr")) {
						show_items_window_auto_ingr = j2["show_items_window_auto_ingr"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_alch")) {
						show_items_window_auto_alch = j2["show_items_window_auto_alch"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_misc")) {
						show_items_window_auto_misc = j2["show_items_window_auto_misc"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_sgem")) {
						show_items_window_auto_sgem = j2["show_items_window_auto_sgem"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr")) {
						show_items_window_auto_achr = j2["show_items_window_auto_achr"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_ingr")) {
						show_items_window_auto_achr_ingr = j2["show_items_window_auto_achr_ingr"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_achr_food")) {
						show_items_window_auto_achr_food = j2["show_items_window_auto_achr_food"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_alch")) {
						show_items_window_auto_achr_alch = j2["show_items_window_auto_achr_alch"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_ammo")) {
						show_items_window_auto_achr_ammo = j2["show_items_window_auto_achr_ammo"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_scrl")) {
						show_items_window_auto_achr_scrl = j2["show_items_window_auto_achr_scrl"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_keym")) {
						show_items_window_auto_achr_keym = j2["show_items_window_auto_achr_keym"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_misc")) {
						show_items_window_auto_achr_misc = j2["show_items_window_auto_achr_misc"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_gold")) {
						show_items_window_auto_achr_gold = j2["show_items_window_auto_achr_gold"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_sgem")) {
						show_items_window_auto_achr_sgem = j2["show_items_window_auto_achr_sgem"].get<bool>();
					}
				}

				if (j.contains("playerBaseInfo")) {
					auto const& j2 = j["playerBaseInfo"];
					show_player_base_info_window = j2["isShow"].get<bool>();

					if (j2.contains("flag_process")) {
						flag_process = j2["flag_process"].get<bool>();
						if (j2.contains("colorPlotHistogramX")) {
							colorPlotHistogramX = j2["colorPlotHistogramX"].get<float>();
							colorPlotHistogramY = j2["colorPlotHistogramY"].get<float>();
							colorPlotHistogramZ = j2["colorPlotHistogramZ"].get<float>();
							colorPlotHistogramW = j2["colorPlotHistogramW"].get<float>();
						}
					}

					if (j2.contains("show_player_base_info_window_sep")) {
						show_player_base_info_window_sep = j2["show_player_base_info_window_sep"].get<bool>();
					}
				}
			}

			if (json.contains("excludeFormIDs")) {
				for (auto const& j : json["excludeFormIDs"]) {
					excludeFormIds.insert(j.get<int>());  // 酒杯
				}
			} else {
				// 默认值
				excludeFormIds.insert(0x000319E3);  // 酒杯
				excludeFormIds.insert(0x00012FE6);  // 鼎
				excludeFormIds.insert(0x00012FE7);  // 筐
				excludeFormIds.insert(0x00012FE8);  // 筐
				excludeFormIds.insert(0x00012FE9);  // 筐
				excludeFormIds.insert(0x00012FEA);  // 筐
				excludeFormIds.insert(0x00012FEB);  // 筐
				excludeFormIds.insert(0x00012FEC);  // 筐
				excludeFormIds.insert(0x000318FA);  // 铸铁锅
				excludeFormIds.insert(0x000318FB);  // 铸铁锅
				excludeFormIds.insert(0x000318EC);  // 油灯
				excludeFormIds.insert(0x00012FDF);  // 桶
				excludeFormIds.insert(0x000747FB);  // 桶
				excludeFormIds.insert(0x000747FE);  // 桶
				excludeFormIds.insert(0x00031941);  // 木盘子
				excludeFormIds.insert(0x000B9BD0);  // 盘子
				excludeFormIds.insert(0x000E2617);  // 盘子
				excludeFormIds.insert(0x000E2618);  // 盘子
				excludeFormIds.insert(0x0003199A);  // 木碗
				excludeFormIds.insert(0x000319E5);  // 木杓
				excludeFormIds.insert(0x0006717F);  // 扫帚
				excludeFormIds.insert(0x000AF5FD);  // 头骨
				excludeFormIds.insert(0x000B9BD8);  // 碗
				excludeFormIds.insert(0x000B9BCC);  // 碗
				excludeFormIds.insert(0x00098627);  // 碗
				excludeFormIds.insert(0x00098626);  // 碗
				excludeFormIds.insert(0x00098623);  // 水壶
				excludeFormIds.insert(0x00098621);  // 高脚杯
				excludeFormIds.insert(0x000B9BDE);  // 水壶
				excludeFormIds.insert(0x000B9BD6);  // 水壶
				excludeFormIds.insert(0x00044E70);  // 漏壶
				excludeFormIds.insert(0x000B9BDA);  // 高脚杯
			}

			if (json.contains("excludeLocationFormIDs")) {
				for (auto const& j : json["excludeLocationFormIDs"]) {
					excludeLocationFormIds.insert(j.get<int>());  // 酒杯
				}
			}

		} catch (std::exception const& ex) {
			logger::error(ex.what());
			//log() << "Unable to save settings file: " << ex.what() << std::endl;

			// 默认值
			excludeFormIds.insert(0x000319E3);  // 酒杯
			excludeFormIds.insert(0x00012FE6);  // 鼎
			excludeFormIds.insert(0x00012FE7);  // 筐
			excludeFormIds.insert(0x00012FE8);  // 筐
			excludeFormIds.insert(0x00012FE9);  // 筐
			excludeFormIds.insert(0x00012FEA);  // 筐
			excludeFormIds.insert(0x00012FEB);  // 筐
			excludeFormIds.insert(0x00012FEC);  // 筐
			excludeFormIds.insert(0x000318FA);  // 铸铁锅
			excludeFormIds.insert(0x000318FB);  // 铸铁锅
			excludeFormIds.insert(0x000318EC);  // 油灯
			excludeFormIds.insert(0x00012FDF);  // 桶
			excludeFormIds.insert(0x000747FB);  // 桶
			excludeFormIds.insert(0x000747FE);  // 桶
			excludeFormIds.insert(0x00031941);  // 木盘子
			excludeFormIds.insert(0x000B9BD0);  // 盘子
			excludeFormIds.insert(0x000E2617);  // 盘子
			excludeFormIds.insert(0x000E2618);  // 盘子
			excludeFormIds.insert(0x0003199A);  // 木碗
			excludeFormIds.insert(0x000319E5);  // 木杓
			excludeFormIds.insert(0x0006717F);  // 扫帚
			excludeFormIds.insert(0x000AF5FD);  // 头骨
			excludeFormIds.insert(0x000B9BD8);  // 碗
			excludeFormIds.insert(0x000B9BCC);  // 碗
			excludeFormIds.insert(0x00098627);  // 碗
			excludeFormIds.insert(0x00098626);  // 碗
			excludeFormIds.insert(0x00098623);  // 水壶
			excludeFormIds.insert(0x00098621);  // 高脚杯
			excludeFormIds.insert(0x000B9BDE);  // 水壶
			excludeFormIds.insert(0x000B9BD6);  // 水壶
			excludeFormIds.insert(0x00044E70);  // 漏壶
			excludeFormIds.insert(0x000B9BDA);  // 高脚杯
			return false;
		}

		return true;
	}

	//

	bool save_settings()
	{
		try {
			nlohmann::json json = {
				{ "imguiSetting", {
									  { "no_titlebar", no_titlebar },
									  { "no_resize", no_resize },
									  { "no_collapse", no_collapse },
									  { "no_background", no_background },
									  { "imgui_style_index", imgui_style_index },
									  { "auto_resize", auto_resize },
									  { "window_border", window_border },
									  { "frame_border", frame_border },
									  { "bullet_text", bullet_text },
									  { "hotkey", hotkey },
									  { "hotkey2", hotkey2 },
									  { "font_scale", ImGui::GetIO().FontGlobalScale },

								  } },

				{ "gameSetting", {
									 { "auto_remove_ammo", auto_remove_ammo },
									 { "remember_last_ammo", remember_last_ammo },
								 } },
				{ "windowSetting", {

									   { "playerBaseInfo", {
															   { "isShow", show_player_base_info_window },
															   { "show_player_base_info_window_sep", show_player_base_info_window_sep },
															   { "flag_process", flag_process },
															   { "colorPlotHistogramX", colorPlotHistogramX },
															   { "colorPlotHistogramY", colorPlotHistogramY },
															   { "colorPlotHistogramZ", colorPlotHistogramZ },
															   { "colorPlotHistogramW", colorPlotHistogramW },
														   } },
									   { "playerModInfo", {
															  { "isShow", show_player_mod_window },
														  } },

									   { "playerInfo", {
														   { "isShow", show_player_info_window },
													   } },

									   { "ArmorInfo", {
														  { "isShow", show_player_armor_window },
													  } },

									   { "WeaponInfo", {
														   { "isShow", show_player_weapon_window },
													   } },
									   { "EnemyInfo", {
														  { "isShow", show_enemy_window },
													  } },
									   { "DebugInfo", {
														  { "isShow", show_player_debug_window },
													  } },
									   { "NpcInfo", {
														{ "isShow", show_npc_window },
														{ "show_npc_window_dis", show_npc_window_dis },
														{ "show_npc_window_dead_hidden", show_npc_window_dead_hidden },
														{ "show_npc_window_direction", show_npc_window_direction },
														{ "show_enemy", show_enemy },
														{ "show_teammate", show_teammate },
														{ "show_npc", show_npc },
														{ "show_horse", show_horse },
													} },
									   { "ItemMenuInfo", {
															 { "show_items_window_auto_dis", show_items_window_auto_dis },
															 { "show_items_window_auto_dis_skyrim", show_items_window_auto_dis_skyrim },
															 { "show_items_window_auto_dis_local", show_items_window_auto_dis_local },
															 { "show_items_window_direction", show_items_window_direction },
															 { "show_items_window_auto_ammo", show_items_window_auto_ammo },
															 { "show_items_window_auto_flor", show_items_window_auto_flor },
															 { "show_items_window_auto_tree", show_items_window_auto_tree },
															 { "show_items_window_auto_food", show_items_window_auto_food },
															 { "show_items_window_auto_ingr", show_items_window_auto_ingr },
															 { "show_items_window_auto_alch", show_items_window_auto_alch },
															 { "show_items_window_auto_misc", show_items_window_auto_misc },
															 { "show_items_window_auto_sgem", show_items_window_auto_sgem },
															 { "show_items_window_auto_achr", show_items_window_auto_achr },
															 { "show_items_window_auto_ignore", show_items_window_auto_ignore },
															 { "show_items_window_auto_achr_ingr", show_items_window_auto_achr_ingr },
															 { "show_items_window_auto_achr_food", show_items_window_auto_achr_food },
															 { "show_items_window_auto_achr_alch", show_items_window_auto_achr_alch },
															 { "show_items_window_auto_achr_sgem", show_items_window_auto_achr_sgem },
															 { "show_items_window_auto_achr_ammo", show_items_window_auto_achr_ammo },
															 { "show_items_window_auto_achr_misc", show_items_window_auto_achr_misc },
															 { "show_items_window_auto_achr_gold", show_items_window_auto_achr_gold },
															 { "show_items_window_auto_achr_scrl", show_items_window_auto_achr_scrl },
															 { "show_items_window_auto_achr_keym", show_items_window_auto_achr_keym },

														 } }

								   } }

			};

			// 忽略项目
			nlohmann::json arr = nlohmann::json::array();
			nlohmann::json arrLocation = nlohmann::json::array();

			for (auto id : excludeFormIds) {
				arr.push_back(id);
			}
			json["excludeFormIDs"] = arr;

			for (auto id : excludeLocationFormIds) {
				arrLocation.push_back(id);
			}
			json["excludeLocationFormIDs"] = arrLocation;

			std::ofstream o(settings);
			o << std::setw(4) << json << std::endl;

		} catch (std::exception const& ex) {
			logger::error(ex.what());

			//log() << "Unable to save settings file: " << ex.what() << std::endl;
			return false;
		}
		return true;
	}
}
