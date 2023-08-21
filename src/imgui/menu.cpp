#include <event/BSTCrosshairRefEvent.h>
#include <event/BSTMenuEvent.h>
#include <hook/hudhook.h>
#include <imgui.h>
#include <imgui/menu.h>
#include <memory/memory.h>
#include <nlohmann/json.hpp>

std::filesystem::path settings = "";
std::string fontFilePath = "";

namespace menu
{
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

	// 默认配置
	static bool show_player_base_info_window = false;
	static bool flag_process = false;
	static bool flag_base_info_setting = true;

	static bool show_player_info_window = true;
	static bool show_player_mod_window = false;
	static bool show_player_armor_window = true;
	static bool show_player_weapon_window = true;
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

	static bool show_enemy_window = false;
	static bool show_npc_window = false;
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

	std::vector<RE::Actor*> actors;

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

	void __fastcall buildNpcInfo(int active, ActorInfo* actorInfo, int actorCount)
	{
		if (active) {
			for (int i = 0; i < actorCount; i++) {
				auto item = actorInfo[i];

				char hp[16] = "已死亡";
				if (item.lifeState != RE::ACTOR_LIFE_STATE::kDead) {
					sprintf(hp, "%.0f/%.0f", item.kHealth, item.kHealthBase);
				}

				if (ImGui::TreeNodeEx(item.formIdStr.c_str(), 0, "%s - [%d] %s [ %s ]", item.formIdStr.c_str(), item.level, item.name.c_str(), hp)) {
					if (ImGui::SmallButton("传送到目标")) {
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

					if (item.lifeState != RE::ACTOR_LIFE_STATE::kDead) {
						if (ImGui::SmallButton("传送到玩家")) {
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
					}

					for (int i2 = 0; i2 < item.inventoryCount; i2++) {
						auto inv = item.Inventorys[i2];
						//myText("%s - %s [%d] %.1f ", inv.formIdStr, inv.name, inv.isWorn, inv.weight);
						//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					}

					ImGui::TreePop();
				}
			}
		} else {
			for (int i = 0; i < actorCount; i++) {
				auto item = actorInfo[i];

				//myText("%s ", item.formId.c_str());
				//ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				myText("[%d] %s [", item.level, item.name.c_str());
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
						myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "%.1f/%.0f", item.kHealth, item.kHealthBase);
					} else if (enemyHealthRate <= 0) {
						myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0/%.0f", item.kHealthBase);
					} else {
						myText2("%.1f/%.0f", item.kHealth, item.kHealthBase);
					}
				}

				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myText2("]");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myText(" [%.1f米]", item.distance);
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myText(" %s", item.isTeammate ? "队友" : "");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myText(" %s", item.idHostile ? "敌对" : "");
			}
		}
	}

	void __fastcall render(int active)
	{
		// 当打开菜单时不显示
		if (isOpenCursorMenu && !active) {
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

		// 载入字体
		//imgui.igPushFont(font);

		auto style = ImGui::GetStyle();
		style.WindowBorderSize = window_border ? 1.0f : 0.0f;
		style.FrameBorderSize = frame_border ? 1.0f : 0.0f;

		if (show_player_base_info_window) {
			ImGui::Begin("人物状态", nullptr, window_flags);

			progress = playerInfo.kHealth / (playerInfo.kHealthBase == 0 ? 1 : playerInfo.kHealthBase);
			progress2 = playerInfo.kStamina / (playerInfo.kStaminaBase == 0 ? 1 : playerInfo.kStaminaBase);
			progress3 = playerInfo.kMagicka / (playerInfo.kMagickaBase == 0 ? 1 : playerInfo.kMagickaBase);

			if (flag_process) {
				myText("生命:");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				char buf[32];
				sprintf(buf, "%d/%d", (int)playerInfo.kHealth, (int)playerInfo.kHealthBase);
				ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

				myText("耐力:");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				char buf2[32];
				sprintf(buf2, "%d/%d", (int)playerInfo.kStamina, (int)playerInfo.kStaminaBase);
				ImGui::ProgressBar(progress2, ImVec2(0.f, 0.f), buf2);

				myText("魔法:");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				char buf3[32];
				sprintf(buf3, "%d/%d", (int)playerInfo.kMagicka, (int)playerInfo.kMagickaBase);
				ImGui::ProgressBar(progress3, ImVec2(0.f, 0.f), buf3);

			} else {
				myText("生命:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress > 0.15) {
					myText2("%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
				}
				myText("耐力:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress2 > 0.15) {
					myText2("%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
				}
				myText("魔法:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress3 > 0.15) {
					myText2("%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
				}
			}
			ImGui::End();
		}

		if (show_player_info_window) {
			ImGui::Begin("人物属性", nullptr, window_flags);

			//myText2("Health:%d", (int)health);
			//myText2("Magicka:%d", (int)kMagicka);
			//myText2("Stamina:%d", (int)kStamina);
			myText("伤害: %s", playerInfo.DamageStr.c_str());
			//myText("防御: %s", playerInfo.DamageResist.c_str());
			myText("防御: %.2f", playerInfo.kDamageResist);
			myText("毒抗: %.1f%%", playerInfo.kPoisonResist);
			myText("火抗: %.1f%%", playerInfo.kResistFire);
			myText("雷抗: %.1f%%", playerInfo.kResistShock);
			myText("冰抗: %.1f%%", playerInfo.kResistFrost);
			myText("法抗: %.1f%%", playerInfo.kResistMagic);
			myText("抗疾病: %.1f%%", playerInfo.kResistDisease);
			//myText2("criticalChance:%d", (int)kCriticalChance);
			//myText2("armor:%d", (int)kDamageResist);
			ImGui::End();
		}

		if (show_player_mod_window) {
			ImGui::Begin("属性加成", nullptr, window_flags);

			myText("单手: %.1f%%", playerInfo.kOneHandedModifier);
			myText("双手: %.1f%%", playerInfo.kTwoHandedModifier);
			myText("弓箭: %.1f%%", playerInfo.kMarksmanModifier);
			myText("锻造: %.1f%%", playerInfo.kSmithingModifier);
			myText("锻造(power): %.1f%%", playerInfo.kSmithingPowerModifier);
			myText("锻造(skill): %.1f%%", playerInfo.kSmithingSkillAdvance);
			myText("炼金: %.1f%%", playerInfo.kAlchemyModifier);
			myText("炼金(power): %.1f%%", playerInfo.kAlchemyPowerModifier);
			myText("炼金(skill): %.1f%%", playerInfo.kAlchemySkillAdvance);
			myText("附魔: %.1f%%", playerInfo.kEnchantingModifier);
			myText("附魔(power): %.1f%%", playerInfo.kEnchantingPowerModifier);
			myText("附魔(skill): %.1f%%", playerInfo.kEnchantingSkillAdvance);
			ImGui::End();
		}

		if (show_player_weapon_window) {
			ImGui::Begin("武器", nullptr, window_flags);

			if (leftWeaponInfo.isExist) {
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
			}

			if (rightWeaponInfo.isExist) {
				// 不显示双手武器
				if (!rightWeaponInfo.isTwoHand) {
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
				}
			}

			if (ammoInfo.isExist) {
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
			myText("人物坐标: [%.0f,%.0f,%.0f]", playerInfo.Position.x, playerInfo.Position.y, playerInfo.Position.z);
			//ImGui::SameLine(0, 0);
			myText("人物视角: [%.2f,%.2f]", playerInfo.Angle.x, playerInfo.Angle.z);
			ImGui::End();
		}

		if (show_enemy_window) {
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
			if (npcCount > 0) {
				myText("npc：");
				auto actorInfo = getNpcData();
				buildNpcInfo(active, actorInfo, npcCount);
				ImGui::Separator();
			}

			if (npcCount > 0) {
				myText("enemy：");
				auto actorInfo = getEnemy2Data();
				buildNpcInfo(active, actorInfo, enemyCount);
				ImGui::Separator();
			}

			if (npcCount > 0) {
				myText("team：");
				auto actorInfo = getTeammateData();
				buildNpcInfo(active, actorInfo, teammateCount);
				ImGui::End();
			}
		}

		if (show_crosshair) {
			ImGui::Begin("准星信息", nullptr, window_flags);
			RE::TESObjectREFR* _cachedRef = BSTCrosshairRefEvent::GetSingleton()->GetCrosshairReference();
			if (_cachedRef) {
				logger::debug("准星");
				logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetDisplayFullName()));
				logger::debug(_cachedRef->GetFormID());
			}

			ImGui::End();
		}

		if (active) {
			static bool show_demo_window = false;
			static bool show_another_window = false;

			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			{
				ImGui::Begin("控制台", nullptr, 0);

				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
					if (ImGui::BeginTabItem("插件设置", 0, 0)) {
						if (ImGui::CollapsingHeader("HUD设置", ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::BeginGroup();

							ImGui::Checkbox("人物基本属性", &show_player_base_info_window);

							if (show_player_base_info_window) {
								if (ImGui::TreeNodeEx("人物基本属性 - 设置", ImGuiTreeNodeFlags_DefaultOpen)) {
									ImGui::Checkbox("显示进度条", &flag_process);
									if (flag_process) {
										ImGui::ColorEdit4("进度条颜色", &style.Colors[ImGuiCol_PlotHistogram].x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
									}
									ImGui::TreePop();
								}
							}

							ImGui::Checkbox("人物属性", &show_player_info_window);
							ImGui::Checkbox("武器信息", &show_player_weapon_window);
							ImGui::Checkbox("装备信息", &show_player_armor_window);
							ImGui::Checkbox("人物属性加成", &show_player_mod_window);
							ImGui::Checkbox("其他信息", &show_player_debug_window);
							ImGui::Checkbox("敌人信息", &show_enemy_window);
							ImGui::Checkbox("npc信息", &show_npc_window);

							ImGui::Checkbox("是否自动卸除箭袋", &auto_remove_ammo);
							// 测试
							ImGui::Checkbox("Demo", &show_demo_window);
							/*		if (ImGui::Button("测试获取人物")) {
								actors.clear();
								auto pl = RE::ProcessLists::GetSingleton();
								for (auto& handle : pl->highActorHandles) {
									auto actor = handle.get().get();
									if (actor) {
										actors.push_back(actor);
									}
								}
							}*/

							ImGui::EndGroup();
						}

						ImGui::Separator();

						if (ImGui::CollapsingHeader("窗口设置", ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::BeginGroup();

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

							//imgui.igShowStyleSelector("style");
							//ImGui::TableNextColumn();
							ImGui::Checkbox("不显示标题", &no_titlebar);
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Checkbox("不允许缩放", &no_resize);
							ImGui::Checkbox("不允许折叠", &no_collapse);
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Checkbox("不显示背景", &no_background);
							ImGui::Checkbox("窗体边框", &window_border);
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Checkbox("控件边框", &frame_border);

							ImGui::Checkbox("是否窗口自适应", &auto_resize);
							ImGui::Checkbox("不显示圆形", &bullet_text);
							ImGui::DragFloat("窗体缩放", &ImGui::GetIO().FontGlobalScale, 0.005f, 0.5f, 1.8f, "%.2f", 1);
							ImGui::DragInt("数据刷新(ms)", &refresh_time_data, 1, 100, 500, "%d ms");

							//ImGui::EndTable();
							ImGui::EndGroup();
						}
						if (ImGui::Button("保存配置", ImVec2(0, 0))) {
							auto colorPlotHistogram = style.Colors[ImGuiCol_PlotHistogram];
							colorPlotHistogramX = colorPlotHistogram.x;
							colorPlotHistogramY = colorPlotHistogram.y;
							colorPlotHistogramZ = colorPlotHistogram.z;
							colorPlotHistogramW = colorPlotHistogram.w;

							save_settings();
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("快捷操作", 0, 0)) {
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

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				//myText2("Application average %.3f ms/frame (%.1f FPS)",
				//	1000.0f / imgui.igGetIO()->Framerate, imgui.igGetIO()->Framerate);
				ImGui::End();
			}
		}

		//ImGui::PopFont();
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
			}

			if (json.contains("gameSetting")) {
				auto const& j = json["gameSetting"];
				if (j.contains("auto_remove_ammo")) {
					auto_remove_ammo = j["auto_remove_ammo"].get<bool>();
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
				if (j.contains("EnemyInfo")) {
					auto const& j2 = j["EnemyInfo"];
					show_enemy_window = j2["isShow"].get<bool>();
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
				}
			}
		} catch (std::exception const& ex) {
			//log() << "Unable to save settings file: " << ex.what() << std::endl;
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

								  } },

				{ "gameSetting", {
									 { "auto_remove_ammo", auto_remove_ammo },
								 } },
				{ "windowSetting", {

									   { "playerBaseInfo", {
															   { "isShow", show_player_base_info_window },
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
													  } }

								   } }

			};

			std::ofstream o(settings);
			o << std::setw(4) << json << std::endl;

		} catch (std::exception const& ex) {
			//log() << "Unable to save settings file: " << ex.what() << std::endl;
			return false;
		}
		return true;
	}

}
