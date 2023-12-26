#include <event/BSTCrosshairRefEvent.h>
#include <event/BSTMenuEvent.h>
#include <fonts/IconsMaterialDesignIcons.h>
#include <hook/hudhook.h>
#include <imgui/imgui.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/npc.h>
#include <memory/player.h>
#include <memory/sexlab.h>
#include <memory/stat.h>
#include <menu/lotd.h>
#include <menu/menu.h>
#include <menu/menu_items.h>
#include <menu/menu_npc.h>
#include <menu/menu_stat.h>
#include <menu/menu_track.h>
#include <menu/theme.h>
#include <nlohmann/json.hpp>
#include <setting/i18n.h>
#include <setting/setting.h>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
//#include <wincodec.h>
#include <hook/BSRenderManager.h>
#include <hook/dinputhook.h>
#include <utils/WICTextureLoader/WICTextureLoader11.h>
#include <memory/data.h>
//#include <utils/WICTextureLoader/WICTextureLoader11.cpp>

namespace menu
{
	// 创建纹理和纹理视图
	/*ID3D11ShaderResourceView* textureView;
	ID3D11Resource* texture;
	bool istestload = false;*/

	bool showOther = false;

	std::map<int, std::vector<EquipmentItem>> equipments;

	bool auto_remove_ammo = false;
	bool remember_last_ammo = false;
	int hotkey = 0;
	int hotkey2 = 3;
	int hotkey3 = 4;

	int hotkeySetting = 210;
	int hotkeySettingModifier = 0;
	int hotkeyItemFinder = 16;
	int hotkeyItemFinderModifier = 0x2;
	int hotkeyTrack = 59;
	int hotkeyTrackModifier = 0x2;

	bool isWaitHotkeySetting = false;
	bool isWaitItemFinder = false;
	bool isWaitTrack = false;

	float font_scale = 1.0f;

	// 默认配置
	bool show_player_base_info_window_sep = false;
	bool flag_process = false;
	bool flag_base_info_setting = true;
	bool show_player_debug_window = false;
	bool no_titlebar = true;
	bool no_resize = false;
	bool no_collapse = false;
	bool no_background = true;
	bool no_background_items = false;
	bool auto_resize = true;

	// 20220428 追加
	bool window_border = true;
	bool frame_border = true;
	bool bullet_text = true;

	// 20231108 追加
	float grabRounding = 0.0f;
	float frameRounding = 0.0f;
	float windowRounding = 0.0f;

	//static int refresh_time_show = 1000;

	//static bool show_npc_window_dis = false;
	//static int show_npc_window_dis_meter = 30;
	//static float show_inv_window_height_half = 6.7f;
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

	static bool setvalue_Permanent = false;
	static int getInv_nowValue = 100;
	static int getInv_selectIndex = 0;

	ImVec4 colorProgressHp(1.0f, 0.5f, 0.0f, 1.0f);
	ImVec4 colorProgressMp(1.0f, 0.5f, 0.0f, 1.0f);
	ImVec4 colorProgressSp(1.0f, 0.5f, 0.0f, 1.0f);

	const char* actorValues[] = {
		"负重 [kCarryWeight]",
		"生命恢复 [kHealRate]",
		"魔法恢复 [kMagickaRate]",
		"耐力恢复 [KStaminaRate]",
		"行走速度 [kSpeedMult]",
		"武器基础伤害 [kMeleeDamage]",
		"空手基础伤害 [kUnarmedDamage]",
		"生命值 [kHealth]",
		"魔法值 [kMagicka]",
		"耐力值 [kStamina]"
	};
	RE::ActorValue actorValuesIndex[] = {
		RE::ActorValue::kCarryWeight,
		RE::ActorValue::kHealRate,
		RE::ActorValue::kMagickaRate,
		RE::ActorValue::KStaminaRate,
		RE::ActorValue::kSpeedMult,
		RE::ActorValue::kMeleeDamage,
		RE::ActorValue::kUnarmedDamage,
		RE::ActorValue::kHealth,
		RE::ActorValue::kMagicka,
		RE::ActorValue::kStamina,
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
					InventoryInfo& item = inv[row_n];
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
		//if (auto_resize)
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		auto& style = ImGui::GetStyle();
		style.WindowBorderSize = window_border ? 1.0f : 0.0f;
		style.FrameBorderSize = frame_border ? 1.0f : 0.0f;

		// 追踪
		renderTrack();

		if (show_player_base_info_window) {
			if (show_player_base_info_window_sep) {
				progress = playerInfo.kHealth / (playerInfo.kHealthBase == 0 ? 1 : playerInfo.kHealthBase);
				progress2 = playerInfo.kStamina / (playerInfo.kStaminaBase == 0 ? 1 : playerInfo.kStaminaBase);
				progress3 = playerInfo.kMagicka / (playerInfo.kMagickaBase == 0 ? 1 : playerInfo.kMagickaBase);

				ImGui::Begin("生命", nullptr, window_flags);

				ImGui::Text(ICON_MDI_HEART_HALF_FULL " 生命:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress > 0.15) {
					ImGui::Text("%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
				}

				ImGui::End();
				ImGui::Begin("耐力", nullptr, window_flags);

				ImGui::Text(ICON_MDI_RUN_FAST " 耐力:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress2 > 0.15) {
					ImGui::Text("%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
				} else {
					myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
				}

				ImGui::End();
				ImGui::Begin("魔法", nullptr, window_flags);

				ImGui::Text(ICON_MDI_STAR_FOUR_POINTS_OUTLINE " 魔法:");
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				if (progress3 > 0.15) {
					ImGui::Text("%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
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
					ImGui::Text(ICON_MDI_HEART_HALF_FULL " 生命:");
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					char buf[32];
					snprintf(buf, 32, "%d/%d", (int)playerInfo.kHealth, (int)playerInfo.kHealthBase);
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressHp);
					ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);
					ImGui::PopStyleColor();

					ImGui::Text(ICON_MDI_RUN_FAST " 耐力:");
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					char buf2[32];
					snprintf(buf2, 32, "%d/%d", (int)playerInfo.kStamina, (int)playerInfo.kStaminaBase);
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressSp);
					ImGui::ProgressBar(progress2, ImVec2(0.f, 0.f), buf2);
					ImGui::PopStyleColor();

					ImGui::Text(ICON_MDI_STAR_FOUR_POINTS_OUTLINE " 魔法:");
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					char buf3[32];
					snprintf(buf3, 32, "%d/%d", (int)playerInfo.kMagicka, (int)playerInfo.kMagickaBase);
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressMp);
					ImGui::ProgressBar(progress3, ImVec2(0.f, 0.f), buf3);
					ImGui::PopStyleColor();

				} else {
					ImGui::Text(ICON_MDI_HEART_HALF_FULL " 生命:");
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
					if (progress > 0.15) {
						ImGui::Text("%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
					} else {
						myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
					}
					ImGui::Text(ICON_MDI_RUN_FAST " 耐力:");
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
					if (progress2 > 0.15) {
						ImGui::Text("%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
					} else {
						myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
					}
					ImGui::Text(ICON_MDI_STAR_FOUR_POINTS_OUTLINE " 魔法:");
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
					if (progress3 > 0.15) {
						ImGui::Text("%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
					} else {
						myTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
					}
				}
				ImGui::End();
			}
		}

		if (show_player_info_window) {
			ImGui::Begin("人物属性", nullptr, window_flags);

			ImGui::Text(I18Ni(ICON_MDI_SWORD, "hud.player.hud-damage"), playerInfo.DamageStr.c_str());
			ImGui::Text(I18Ni(ICON_MDI_SHIELD_HALF_FULL, "hud.player.hud-damageResist"), playerInfo.kDamageResist);
			ImGui::Text(I18Ni(ICON_MDI_WATER, "hud.player.hud-poisonResist"), playerInfo.kPoisonResist);
			ImGui::Text(I18Ni(ICON_MDI_FIRE, "hud.player.hud-resistFire"), playerInfo.kResistFire);
			ImGui::Text(I18Ni(ICON_MDI_LIGHTNING_BOLT_OUTLINE, "hud.player.hud-resistShock"), playerInfo.kResistShock);
			ImGui::Text(I18Ni(ICON_MDI_SNOWFLAKE, "hud.player.hud-resistFrost"), playerInfo.kResistFrost);
			ImGui::Text(I18Ni(ICON_MDI_MAGIC_STAFF, "hud.player.hud-resistMagic"), playerInfo.kResistMagic);
			ImGui::Text(I18Ni(ICON_MDI_PILL, "hud.player.hud-resistDisease"), playerInfo.kResistDisease);
			ImGui::End();
		}

		if (show_player_mod_window) {
			ImGui::Begin("属性加成", nullptr, window_flags);

			ImGui::Text(I18Ni(ICON_MDI_AXE, "hud.player.hud-oneHandedModifier"), playerInfo.kOneHandedModifier);
			ImGui::Text(I18Ni(ICON_MDI_AXE_BATTLE, "hud.player.hud-twoHandedModifier"), playerInfo.kTwoHandedModifier);
			ImGui::Text(I18Ni(ICON_MDI_BOW_ARROW, "hud.player.hud-marksmanModifier"), playerInfo.kMarksmanModifier);
			ImGui::Text(I18Ni(ICON_MDI_ANVIL, "hud.player.hud-smithingModifier"), playerInfo.kSmithingModifier);
			if (playerInfo.kSmithingPowerModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "+ %.0f%% " ICON_MDI_TRANSFER_UP, playerInfo.kSmithingPowerModifier);
			} else if (playerInfo.kSmithingPowerModifier < 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "- %.0f%% " ICON_MDI_TRANSFER_DOWN, playerInfo.kSmithingPowerModifier);
			}
			ImGui::Text(I18Ni(ICON_MDI_ANVIL, "hud.player.hud-smithingSkillAdvance"), playerInfo.kSmithingSkillAdvance);
			ImGui::Text(I18Ni(ICON_MDI_BOWL_MIX, "hud.player.hud-alchemyModifier"), playerInfo.kAlchemyModifier);

			if (playerInfo.kAlchemyPowerModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "+ %.0f%% " ICON_MDI_TRANSFER_UP, playerInfo.kAlchemyPowerModifier);
			} else if (playerInfo.kAlchemyPowerModifier < 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "- %.0f%% " ICON_MDI_TRANSFER_DOWN, playerInfo.kAlchemyPowerModifier);
			}

			ImGui::Text(I18Ni(ICON_MDI_BOWL_MIX, "hud.player.hud-alchemySkillAdvance"), playerInfo.kAlchemySkillAdvance);
			ImGui::Text(I18Ni(ICON_MDI_FLASH, "hud.player.hud-enchantingPowerModifier"), playerInfo.kEnchantingModifier);

			if (playerInfo.kEnchantingPowerModifier > 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "+ %.0f%% " ICON_MDI_TRANSFER_UP, playerInfo.kEnchantingPowerModifier);
			} else if (playerInfo.kEnchantingPowerModifier < 0.0f) {
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				myTextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "- %.0f%% " ICON_MDI_TRANSFER_DOWN, playerInfo.kEnchantingPowerModifier);
			}

			ImGui::Text(I18Ni(ICON_MDI_FLASH, "hud.player.hud-enchantingSkillAdvance"), playerInfo.kEnchantingSkillAdvance);
			ImGui::End();
		}

		if (show_player_weapon_window) {
			ImGui::Begin("武器", nullptr, window_flags);

			if (leftWeaponInfo.isExist) {
				if (active) {
					if (ImGui::TreeNode(leftWeaponInfo.treeId.c_str())) {
						ImGui::Text("类型: %s", leftWeaponInfo.formTypeName.c_str());
						ImGui::Text("ID: %s", leftWeaponInfo.formID.c_str());
						ImGui::SameLine(0, 0);
						if (ImGui::SmallButton("卸载")) {
							std::string commandStr = "player.unequipItem ";
							commandStr.append(leftWeaponInfo.formID);
							ScriptUtil::ExecuteCommand(commandStr);
						}

						switch (leftWeaponInfo.formType) {
						case RE::FormType::Weapon:
							{
								ImGui::Text("武器类型: %s", leftWeaponInfo.weaponTypeName.c_str());
								ImGui::Text("武器伤害: %d", leftWeaponInfo.damage);
								ImGui::Text("暴击伤害: %d", leftWeaponInfo.critDamage);
								ImGui::Text("价格: %d", leftWeaponInfo.goldValue);
								ImGui::Text("属性值: %d$", leftWeaponInfo.value);
								ImGui::Text("重量: %.1f", leftWeaponInfo.weight);
								break;
							}
						case RE::FormType::Spell:
							{
								ImGui::Text("法术类型: %s", leftWeaponInfo.castingTypeName.c_str());
								ImGui::Text("法术类型: %s", leftWeaponInfo.spellTypeName.c_str());
								ImGui::Text("花费: %.1f", leftWeaponInfo.cost);
								ImGui::Text("施法时间: %.1f", leftWeaponInfo.time);
								break;
							}

						default:
							break;
						}

						//ImGui::Text("插槽: %s", item.equipSlotName.c_str());
						ImGui::TreePop();
					}
				} else {
					switch (leftWeaponInfo.formType) {
					case RE::FormType::Weapon:
						{
							ImGui::Text(ICON_MDI_SWORD " %s", leftWeaponInfo.treeId.c_str());
							break;
						}
					case RE::FormType::Spell:
						{
							ImGui::Text(ICON_MDI_MAGIC_STAFF " %s", leftWeaponInfo.treeId.c_str());
							break;
						}
					}
				}
			}

			if (rightWeaponInfo.isExist) {
				// 不显示双手武器
				if (!rightWeaponInfo.isTwoHand) {
					if (active) {
						if (ImGui::TreeNode(rightWeaponInfo.treeId.c_str())) {
							ImGui::Text("类型: %s", rightWeaponInfo.formTypeName.c_str());
							ImGui::Text("ID: %s", rightWeaponInfo.formID.c_str());
							ImGui::SameLine(0, 0);
							if (ImGui::SmallButton("卸载")) {
								std::string commandStr = "player.unequipItem ";
								commandStr.append(rightWeaponInfo.formID);
								ScriptUtil::ExecuteCommand(commandStr);
							}

							switch (rightWeaponInfo.formType) {
							case RE::FormType::Weapon:
								{
									ImGui::Text("武器类型: %s", rightWeaponInfo.weaponTypeName.c_str());
									ImGui::Text("武器伤害: %d", rightWeaponInfo.damage);
									ImGui::Text("暴击伤害: %d", rightWeaponInfo.critDamage);
									ImGui::Text("价格: %d", rightWeaponInfo.goldValue);
									ImGui::Text("属性值: %d$", rightWeaponInfo.value);
									ImGui::Text("重量: %.1f", rightWeaponInfo.weight);
									break;
								}
							case RE::FormType::Spell:
								{
									ImGui::Text("法术类型: %s", rightWeaponInfo.castingTypeName.c_str());
									ImGui::Text("法术类型: %s", rightWeaponInfo.spellTypeName.c_str());
									ImGui::Text("花费: %.1f", rightWeaponInfo.cost);
									ImGui::Text("施法时间: %.1f", rightWeaponInfo.time);
									break;
								}
							default:
								break;
							}

							ImGui::TreePop();
						}
					} else {
						switch (rightWeaponInfo.formType) {
						case RE::FormType::Weapon:
							{
								ImGui::Text(ICON_MDI_SWORD " %s", rightWeaponInfo.treeId.c_str());
								break;
							}
						case RE::FormType::Spell:
							{
								ImGui::Text(ICON_MDI_MAGIC_STAFF " %s", rightWeaponInfo.treeId.c_str());
								break;
							}
						}
					}
				}
			}

			if (ammoInfo.isExist) {
				if (active) {
					if (ImGui::TreeNode(ammoInfo.treeId.c_str())) {
						ImGui::Text("类型: %s", ammoInfo.formTypeName.c_str());
						ImGui::Text("ID: %s", ammoInfo.formID.c_str());
						ImGui::SameLine(0, 0);
						if (ImGui::SmallButton("卸载")) {
							std::string commandStr = "player.unequipItem ";
							commandStr.append(ammoInfo.formID);
							ScriptUtil::ExecuteCommand(commandStr);
						}
						ImGui::Text("伤害: %.1f", ammoInfo.damage);
						ImGui::Text("价格: %d", ammoInfo.goldValue);
						ImGui::Text("属性值: %d$", ammoInfo.value);
						ImGui::Text("重量: %.1f", ammoInfo.weight);

						ImGui::TreePop();
					}
				} else {
					ImGui::Text(ICON_MDI_ARROW_PROJECTILE " %s", ammoInfo.treeId.c_str());
				}
			}

			if (powerInfo.isExist) {
				ImGui::Text(ICON_MDI_ACCOUNT_VOICE " %s", powerInfo.treeId.c_str());
			}

			ImGui::End();
		}

		if (show_player_armor_window) {
			ImGui::Begin("装备", nullptr, window_flags);

			int unWornArmosAlertSlots = 0;

			for (int i = 0; i <= 31; i++) {
				auto& item = wornArmos[i];

				if (item.isExist) {
					if (item.isSpeacilSlotAlert) {
						// 变黄色
						//imgui.igPushStyleColor
					}

					if (active) {
						if (ImGui::TreeNode(item.treeId.c_str())) {
							ImGui::Text("类型: %s", item.formTypeName.c_str());
							ImGui::Text("装备类型: %s", item.armorTypeName.c_str());
							ImGui::Text("ID: %s", item.formID.c_str());
							ImGui::SameLine(0, 0);
							if (ImGui::SmallButton("卸载")) {
								std::string commandStr = "player.unequipItem ";
								commandStr.append(item.formID);
								ScriptUtil::ExecuteCommand(commandStr);
							}
							ImGui::Text("价格: %d$", item.goldValue);
							ImGui::Text("属性值: %d", item.value);
							ImGui::Text("装备等级: %.2f", item.armorRating);
							ImGui::Text("插槽: %s", item.equipSlotName.c_str());
							ImGui::Text("重量: %.1f", item.weight);
							ImGui::TreePop();
						}
					} else {
						ImGui::Text("%s", item.treeId.c_str());
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
			//ImGui::Text(ICON_MDI_AXIS_ARROW " 坐标: [%.1f,%.1f,%.1f]", playerInfo.Position.x / 100.0f, playerInfo.Position.y / 100.0f, playerInfo.Position.z / 100.0f);
			// z 是0~2Π
			//ImGui::Text(ICON_MDI_ANGLE_ACUTE " 视角: [%.2f,%.2f]", playerInfo.Angle.x, playerInfo.Angle.z);
			ImGui::End();
		}

		if (sexlab::isShowPlayOSLArousal && sexlab::isLoadOSLAroused) {
			ImGui::Begin("欲望信息", nullptr, window_flags);

			ImGui::Text(ICON_MDI_HEART " 性欲:");
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

			auto progress = sexlab::OSLArousal / 100.0f;
			char buf[32];
			snprintf(buf, 32, "%0.1f/%d", sexlab::OSLArousal, 100);

			ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
			float rate = sexlab::OSLArousal / (sexlab::OSLArousalBaseline == 0.0f ? 1.0f : sexlab::OSLArousalBaseline);
			if (rate > 0.80f) {
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressNpc3);
			} else if (rate < 0.45f) {
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressNpc1);
			} else {
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressNpc2);
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopItemWidth();

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

						ImGui::Text("[%d] %s [", item.level, item.name.c_str());
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						if (enemyHealthRate > 0.85f) {
							myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%.0f/%.0f ", item.kHealth, item.kHealthBase);
						} else if (enemyHealthRate < 0.20f) {
							myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "%.0f/%.0f ", item.kHealth, item.kHealthBase);
						} else {
							ImGui::Text("%.0f/%.0f ", item.kHealth, item.kHealthBase);
						}
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						ImGui::Text("%.0f ", item.kStamina);
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						myTextColored(ImVec4(0.0f, 0, 1, 1.0f), "%.0f", item.kMagicka);
						ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
						ImGui::Text("] %d", item.isSentient);
						ImGui::Text("]");
					}
				}
			}
			ImGui::End();
		}

		if (show_npc_window) {
			renderNpc(window_flags);
		}

		if (show_inv_window) {
			if (active || !show_inv_window_active) {
				ImGui::Begin("防具信息", nullptr, window_flags);
				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
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

				ImGui::Text("\uf0d6 %d", getPlayerGoldCount());
				//ImGui::Text("负重：%0.1f/%0.0f", playerInfo.equippedWeight, playerInfo.carryWeight);

				ImGui::End();
			}
		}

		if (show_items_window) {
			/*ImGui::Begin("物品信息", nullptr, window_flags);

			ImGui::End();*/
		}

		if (show_player_effects_window) {
			if (getEffectsCount()) {
				ImGui::Begin("BUFF", nullptr, window_flags);

				auto& effects = getEffects();
				for (int i = 0; i < getEffectsCount(); i++) {
					auto& item = effects[i];
					if (item.magnitude >= 0) {
						if (item.duration == 0) {
							ImGui::Text("%s", item.text.c_str());
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%s", item.text2.c_str());
						} else {
							myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%s", item.text.c_str());
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%s", item.text2.c_str());

							if (show_player_effects_process) {
								auto sy = item.duration - item.elapsedSeconds;
								if (sy < 999) {
									auto progressEffect = sy / item.duration;
									char buf[32];
									snprintf(buf, 32, "%0.0fs", sy);
									//sprintf(buf, "%0.1f/%0.0f", (item.duration - item.elapsedSeconds), item.duration);

									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
									ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressEffect1);
									ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
									ImGui::ProgressBar(progressEffect, ImVec2(0.f, 0.f), buf);
									ImGui::PopStyleVar();
									ImGui::PopStyleColor();
									ImGui::PopItemWidth();
								}
							}
						}
					} else {
						myTextColored(ImVec4(1, 0.0f, 0.0f, 1.0f), "%s", item.text.c_str());
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						myTextColored(ImVec4(1, 0.0f, 0.0f, 1.0f), "%s", item.text2.c_str());

						if (item.duration != 0) {
							if (show_player_effects_process) {
								auto sy = item.duration - item.elapsedSeconds;
								if (sy < 999) {
									auto progressEffect = sy / item.duration;
									char buf[32];
									snprintf(buf, 32, "%0.0fs", sy);

									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
									ImGui::PushStyleColor(ImGuiCol_PlotHistogram, colorProgressEffect3);
									ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
									ImGui::ProgressBar(progressEffect, ImVec2(0.f, 0.f), buf);
									ImGui::PopStyleVar();
									ImGui::PopStyleColor();
									ImGui::PopItemWidth();
								}
							}
						}
					}
					if (active && show_player_effects_listignore) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::PushID(i);
						if (ImGui::SmallButton(ICON_MDI_EYE_REMOVE_OUTLINE)) {
							bool exist = false;
							for (const auto& excludeForm : excludeEffectForms) {
								if (excludeForm.effectId == item.effectId && excludeForm.spellId == item.spellId) {
									exist = true;
									break;
								}
							}
							if (!exist) {
								excludeEffectForms.push_back({ item.effectId, item.spellId, item.name, item.spellName });
							}
							ExcludeFormEffectIds ids{ item.effectId, item.spellId };
							excludeEffectFormIds.insert(ids);
						}
						ImGui::PopID();
					}
				}
				ImGui::End();
			}
		}

		if (show_player_gold_window) {
			ImGui::Begin("金钱", nullptr, window_flags);
			ImGui::Text(ICON_MDI_CURRENCY_USD " %d", playerInfo.gold);
			ImGui::End();
		}

		if (show_player_carryweight_window) {
			ImGui::Begin("负重", nullptr, window_flags);
			ImGui::Text(ICON_MDI_WEIGHT " %.1f / %.0f", playerInfo.equippedWeight, playerInfo.carryWeight);
			ImGui::End();
		}

		if (show_player_xp_window) {
			ImGui::Begin("经验值", nullptr, window_flags);
			ImGui::Text(ICON_MDI_TRANSFER_UP " %.0f / %.0f", playerInfo.xp, playerInfo.levelThreshold);
			ImGui::End();
		}

		renderStat(window_flags);

		if (lotd::showlocationItemCount || stats::showlocationExCount) {
			ImGui::Begin("附近艺术馆藏品数量", nullptr, window_flags);

			if (lotd::showlocationItemCount) {
				ImGui::Text(I18Ni(ICON_MDI_TREASURE_CHEST_OUTLINE, "hud.stat.hud-nearbyLotdItemsQuantity"));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (lotd::locationItemCount > 0) {
					myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", lotd::locationItemCount);
				} else {
					myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			if (stats::showlocationExCount) {
				ImGui::Text(I18Ni(ICON_MDI_SPADE, "hud.stat.hud-nearbyLotdExcavationQuantity"));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (stats::locationExCount > 0) {
					myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", stats::locationExCount);
					if (activeItems) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS "##ex")) {
							for (auto item : stats::locationExIds) {
								TrackItem trackItem;
								trackItem.name = item->GetDisplayFullName();
								trackPtrs2.insert(std::make_pair(item, trackItem));
								menu::isTrack = true;
							}
						}
					}
				} else {
					myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			ImGui::End();
		}

		if (lotd::showDisplayItemCount) {
			ImGui::Begin("艺术馆藏品数量", nullptr, window_flags);

			ImGui::Text(I18Ni(ICON_MDI_TREASURE_CHEST_OUTLINE, "hud.stat.hud-lotdItemsQuantity"));
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%0.0f", lotd::displayCount);

			ImGui::End();
		}

		if (stats::showlocationNirnRootCount || stats::showlocationNirnRootRedCount || stats::showlocationOreCount || stats::showlocationExCount) {
			ImGui::Begin("附近物品数量", nullptr, window_flags);

			if (stats::showlocationNirnRootCount) {
				ImGui::Text(I18Ni(ICON_MDI_SPA_OUTLINE, "hud.stat.hud-nirnRootQuantity"));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (stats::locationNirnRootCount > 0) {
					myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", stats::locationNirnRootCount);
				} else {
					myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			if (stats::showlocationNirnRootRedCount) {
				ImGui::Text(I18Ni(ICON_MDI_SPA, "hud.stat.hud-nirnRootRedQuantity"));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (stats::locationNirnRootRedCount > 0) {
					myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", stats::locationNirnRootRedCount);
				} else {
					myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			if (stats::showlocationOreCount) {
				//ImGui::Text(ICON_MDI_GOLD " 矿脉数量：");
				ImGui::Text(I18Ni(ICON_MDI_TERRAIN, "hud.stat.hud-veinQuantity"));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (stats::locationOreCount > 0) {
					myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", stats::locationOreCount);
					if (activeItems) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS "##ore")) {
							for (auto item : stats::locationOreIds) {
								TrackItem trackItem;
								trackItem.name = item->GetDisplayFullName();
								trackPtrs2.insert(std::make_pair(item, trackItem));
								menu::isTrack = true;
							}
						}
					}
				} else {
					myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			ImGui::End();
		}

		renderItems();

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

#ifndef NDEBUG

			{
				ImGui::Begin("测试", nullptr, ImGuiWindowFlags_NoCollapse);

				static int selected2 = 0;
				{
					ImGui::BeginGroup();
					ImVec2 size(ImGui::GetFontSize() * 6, 0);
					if (ImGui::Selectable("测试", selected2 == 0, 0, size)) {
						selected2 = 0;
					}
					ImGui::EndGroup();

					ImGui::SameLine();

					ImGui::BeginGroup();

					ImGui::Spacing();
					ImGui::Text("快速攻略");
					ImGui::Separator();
					ImGui::TextWrapped("1.听听黎明守卫吸血鬼猎人的故事。");
					ImGui::TextWrapped("2.找到黎明守卫堡。 ");
					ImGui::TextWrapped("3.与伊斯兰和警惕的托兰交谈。");

					ImGui::Spacing();
					ImGui::Text("详细攻略");
					ImGui::Separator();
					ImGui::TextWrapped("听说吸血鬼猎人");
					ImGui::TextWrapped("安装黎明守卫附加组件后，你可能会听到城市守卫关于黎明守卫的新谣言：“听说他们正在改革黎明守卫。吸血鬼猎人什么的，在裂谷附近的旧堡垒里。我可能会考虑加入。” 如果这是您第一次听说黎明守卫，您的地图将更新为黎明守卫堡的位置，并且任务将正式出现在您的日记中。");
					ImGui::TextWrapped("如果你的等级是 10 级或以上，无论你是否听说过这个谣言，一个名叫杜拉克的兽人都会找到你，并提议加入黎明守卫：“你在那里。黎明守卫正在寻找任何愿意对抗黎明守卫的人。”吸血鬼的威胁越来越大。你说呢？” 杂项日记目标“找出黎明守卫”将会出现，如果你让他解释，他会告诉你他们是一群吸血鬼猎人。他会提到警戒者大厅被吸血鬼摧毁是因为警戒者没有认真对待吸血鬼的威胁。如果你问他在说什么，他会告诉你他们在对抗吸血鬼时被撕成碎片，这就是为什么他的领袖伊斯兰正在重建黎明守卫。当你询问如何报名时，杜拉克会告诉你去裂谷东南的黎明守卫堡与伊斯兰交谈。如果您还没有听到守卫的谣言，您的地图现在将被更新，并且任务将正式出现在您的日记中。");
					ImGui::TextWrapped("如果你碰巧告诉杜拉克你对被招募不感兴趣，他会说“每个人都这么说，直到他们发现自己的喉咙被一群饥饿的吸血鬼撕开。” 对话就会结束。如果你第二次接近他，他会回答“是的，你改变主意了？黎明守卫可以使用像你这样的人。” 其他对话选项与第一次相同。如果你告诉他你这次不感兴趣，他会让你和伊斯兰谈谈，并告诉你黎明守卫堡的位置，以防你改变主意。");
					ImGui::TextWrapped("或者，您可以以另一种方式开始此任务，详情如下。");

					ImGui::TextWrapped("找到黎明守卫堡");
					ImGui::TextWrapped("走从裂谷向东的路。道路转向东南并穿过一座小桥后，开始观察左侧的悬崖。你会在悬崖边发现一条小裂缝——黎明峡谷的入口。它位于一根中空的绿色原木和一棵折断的桦树之间。一条狭窄蜿蜒的小路将带您前往黎明守卫堡。在您发现地图标记之前，这是到达堡垒的唯一途径。");
					ImGui::TextWrapped("一路上，你可能会遇到黎明守卫的另一位新兵阿格玛尔。如果你还没有听说过黎明守卫，他的问候将触发出现在你的日记中的任务：“哦，嘿！你也是来加入黎明守卫的吗？” 他表达了他对独自进入堡垒的焦虑，并陪伴你走完了剩下的路。他要求你不要向伊斯兰提及他的紧张情绪，但在你散步时他会发表一些评论，表达他的不自信。尽管他表示害怕自己进去，但如果你在到达入口前的台阶后不立即进入，他就会跑进堡垒而不等你。");
					ImGui::TextWrapped("你可能会在堡垒外面经过杜拉克，练习他的十字弓技能。如果你问他在用什么，他会回答“以前从未见过十字弓？并不惊讶。有点像黎明守卫的专长。没有什么比这更适合镇压吸血鬼了。” 然后他会给你一把自己的十字弓，以及 45 个钢螺栓（有关如何获得第二个螺栓的信息，请参阅下面的注释）。");
					ImGui::TextWrapped("当你到达堡垒的台阶时，你会发现塞兰守卫着入口。如果你选择与他交谈，他会说他对伊斯兰的来信感到惊讶，并评论说伊斯兰向他寻求帮助的情况一定很糟糕。如果你问塞兰过去与伊斯兰的合作，他会告诉你，他们都是斯坦达尔警戒者的成员，他们的“心是在正确的地方”，但让他们都不满意。两人一起离开，但塞兰不同意伊斯兰的“方法”，因此这种伙伴关系并没有持续多久。在那张纸条上，他会告诉你“进去吧，伊斯兰会想和你说话。");

					ImGui::TextWrapped("加入黎明守卫");
					ImGui::TextWrapped("进去后，你会发现黎明守卫领袖伊斯兰正在与警惕者的牧师托兰交谈。");
					ImGui::TextWrapped("伊斯兰：“托兰，你为什么在这里？我和警戒者早就结束了。” ");
					ImGui::TextWrapped("托兰：“你知道我为什么在这里，警戒者到处都受到攻击。吸血鬼比我们想象的要危险得多。” ");
					ImGui::TextWrapped("伊斯兰：“现在你想和黎明守卫一起逃到安全的地方，是这样吗？我记得守护者卡塞特反复告诉我，黎明守卫是一个摇摇欲坠的废墟，不值得花费金钱和人力来修复。现在你已经搅拌了吸血鬼来攻击你，你来乞求我的保护吗？” ");
					ImGui::TextWrapped("托兰：“伊斯兰，卡塞特死了。警戒厅……每个人……他们都死了。你是对的，我们错了。这对你来说还不够吗？” ");
					ImGui::TextWrapped("Isran：“是的，嗯……我从来不想发生这样的事情。我试图警告你们所有人……我很抱歉，你知道。”");
					ImGui::TextWrapped("此时，伊斯兰会转向你，问“那么你是谁，你想要什么？” 你可以告诉他你只是环顾四周并询问有关堡垒的情况，他会回答说：“为什么我要派警卫？这是黎明守卫的总部。嗯，很久以前它曾经是”。如果你告诉他你是来加入黎明守卫的，或者只是你听说过吸血鬼猎人，他会很高兴你感兴趣，但评论说他们目前的大部分工作都是为了恢复吸血鬼猎人的身份。堡。");
					ImGui::TextWrapped("如果你在第一次与伊斯兰对话时已经完全变成了吸血鬼，并且你回答说：“我听说你在寻找吸血鬼猎人”，伊斯兰会回答“你没听错，我很高兴这个词是终于开始四处走动了。但这意味着用不了多久吸血鬼也会开始注意到这一点。” 如果你回答：“你究竟打算对这些吸血鬼做什么？” 然后伊斯兰回答道：“首先要做的就是让这座堡垒恢复原状。现在吸血鬼几乎可以直接走进前门。但既然你问了，我有件事要你做”，然后他继续谈论暗空地穴。");
					ImGui::TextWrapped("当你问你如何提供帮助时，他会回答“我需要有人在战场上，与该死的吸血鬼战斗，同时我们正在让堡垒恢复原状。托兰告诉我一些洞穴的事警戒者正在四处探查。似乎认为这是……”并请托兰告诉你关于暗空地穴的事情。托兰会告诉你“阿达尔瓦尔德兄弟确信它藏有某种失落已久的吸血鬼神器。我们不再听他的话，就像我们听伊斯兰一样。当大厅遭到袭击时，他正在大厅里……”");
					ImGui::TextWrapped("然后伊斯兰会给你分配第一个任务：“去看看吸血鬼在这个昏暗地穴里寻找什么。如果运气好的话，他们仍然在那里。” 然后邀请您去探索并从黎明守卫堡获取您需要的任何东西。在你离开之前，托兰会主动提出在暗空地穴与你会面，并说道：“这是我能为死去的战友报仇的最低限度。” 伊斯兰和托兰会争论一下，因为伊斯兰不相信警戒者接受过执行此类任务的训练，而托兰对伊斯兰认为警戒者软弱、懦弱和软弱的暗示感到生气。不过，他会坚持自己的决定，告诉你“我要去暗空地穴。也许我可以为你提供一些小帮助。”");
					ImGui::TextWrapped("然后，伊斯兰会给你一把十字弓和一些钢螺栓（如果你还没有从杜拉克那里收到它们），并说：“在这里，你应该拿一把十字弓。在那些恶魔靠得太近之前，可以把它们猎杀掉。” 并送你去执行你的任务。任务觉醒立即开始。");

					ImGui::Spacing();
					ImGui::Text("注释");
					ImGui::Separator();
					ImGui::TextWrapped("开始此任务后，无论你的等级如何，警惕大厅都将被摧毁");
					ImGui::TextWrapped("打开控制台输入密码即可将大厅恢复原状 xx003531.enable");
					ImGui::TextWrapped("你可以完全绕过这个任务并开始觉醒，而这个任务不会出现在你的任务日志中。为了做到这一点，你必须在进入黎明峡谷时没有从守卫或杜拉克那里收到有关黎明守卫的信息。然后，在进入黎明峡谷和接近黎明守卫堡之间，一定不要进入阿格玛尔的近距离范围。当你进入要塞时，即使没有开始任务，与任务相关的事件仍然会以相同的方式发生。");
					ImGui::TextWrapped("如果你已经从杜拉克那里收到了十字弓，你可以在与伊斯兰交谈之前将其交给追随者。然后他会给你第二把十字弓和 45 个钢螺栓。");

					ImGui::Spacing();
					ImGui::Text("BUG");
					ImGui::Separator();

					ImGui::TextWrapped("在某些情况下，地图和指南针上的任务标记不会指向正确的方向。这是因为该标记与黎明峡谷的错误门相关联。");
					ImGui::TextWrapped("该错误已由非官方黎明守卫补丁1.2.2 版修复。");
					ImGui::TextWrapped("如果你有狼人，黎明守卫的成员可能会攻击你。");
					ImGui::TextWrapped("重新加载之前的保存，使用野兽形态，然后等待它过期。然后前往黎明守卫堡。");
					ImGui::TextWrapped("如果您在进入黎明峡谷之前攻击和/或可能杀死了任何 Stendarr 的警戒者，则所有黎明守卫成员都可能会怀有敌意，从而阻止您加入，因为黎明守卫和 Stendarr 的守夜者是结盟派系。 ");
					ImGui::TextWrapped("通过避免与斯坦达尔警戒者的任何敌对互动，也许可以绕过这个问题。");
					ImGui::TextWrapped("完成此任务后，可能不会触发下一个任务“觉醒”");
					ImGui::TextWrapped("实时等待 15 分钟可能会解决此问题。");
					ImGui::TextWrapped("一个常见的解决方法是完全跳过该任务。然而，这会导致稍后的任务中断。");
					ImGui::TextWrapped("该任务可能无法正常工作（例如阿格玛尔没有向你打招呼，伊斯兰和托兰在进入黎明守卫堡时没有进行对话，或者没有启动觉醒）。后续所有新游戏也可能会出现这种情况");
					ImGui::TextWrapped("完全重新安装《天际》可能会修复它");
					ImGui::TextWrapped("杜拉克可能会靠近并绕圈行走，从而阻止对话。他可能会无限期地停留在原地");
					ImGui::TextWrapped("使用控制台命令setstage DLC1VQ01MiscObjective 10。之后就可以正常对话了。然而，此选项有时会将日志中的任务条目留空。另一种选择是使用以下命令将 Durak 移至您身边：prid xx015c16then moveto player。然后他似乎正常开始任务");
					ImGui::TextWrapped("如果你在裂谷有赏金，黎明守卫的成员在见面时可能会怀有敌意，阻止加入。为了避免这种情况，请在进入黎明峡谷之前处理好赏金");

					ImGui::Spacing();
					ImGui::Text("任务阶段");
					ImGui::Separator();

					ImGui::EndGroup();
				}

				ImGui::End();
			}
#endif

			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			{
				ImGui::SetNextWindowPos(ImVec2(70, 35), ImGuiCond_FirstUseEver);

				ImGui::Begin("上古卷轴物品定位小助手", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

				static int clickCount = 0;

				static int selected = 0;
				{
					ImGui::BeginGroup();
					ImVec2 size(ImGui::GetFontSize() * 6, 0);
					if (ImGui::Selectable(I18Ni(ICON_MDI_TOOLS, "common.setting.tab-common"), selected == 0, 0, size)) {
						clickCount = 0;
						selected = 0;
					}
					if (lotd::isLoad) {
						if (ImGui::Selectable(I18Ni(ICON_MDI_GREENHOUSE, "common.setting.tab-lotd"), selected == 1, 0, size)) {
							clickCount = 0;
							selected = 1;
						}
					}
					if (ImGui::Selectable(I18Ni(ICON_MDI_TABLE_OF_CONTENTS, "common.setting.tab-hud"), selected == 2, 0, size)) {
						selected = 2;
						clickCount++;
						if (clickCount > 3) {
							showOther = true;
						}
					}
					if (showOther) {
						if (ImGui::Selectable(I18Ni(ICON_MDI_ACCOUNT_EDIT, "common.setting.tab-other"), selected == 99, 0, size)) {
							selected = 99;
						}
					}

					if (ImGui::BeginPopupModal(I18Nc("common.setting.tab-about"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

						
						ImGui::Text("游戏版本：v%s", data::gameVersion.string().c_str());
						ImGui::Text("插件版本：v1.2.5");

						if (ImGui::Button(I18Nc("common.setting.btn-ok"), ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					if (ImGui::Selectable(I18Ni(ICON_MDI_INFORMATION_OUTLINE, "common.setting.tab-about"), false, 0, size)) {
						clickCount = 0;
						ImGui::OpenPopup(I18Nc("common.setting.tab-about")); 
					}

					ImGui::EndGroup();
				}

				ImGui::SameLine();

				ImGui::BeginGroup();

				{
					if (selected == 0) {
						ImGui::Text(I18Ni(ICON_MDI_COG, "common.setting.label-normal"));
						{
							ImGui::Indent();

							ImGui::AlignTextToFramePadding();
							if (ImGui::BeginPopupContextItem("PopupI18n")) {
								for (auto& item : i18n::maps) {
									if (ImGui::Selectable(item.first.c_str())) {
										i18n::languageName = item.first;
									}
								}
								ImGui::EndPopup();
							}

							ImGui::Text(I18Nc("common.setting.label-i18n"));
							ImGui::SameLine();
							if (ImGui::Button(i18n::languageName.c_str(), ImVec2(ImGui::GetFontSize() * 6, 0))) {
								ImGui::OpenPopup("PopupI18n");
							}

							ImGui::AlignTextToFramePadding();
							if (ImGui::BeginPopupContextItem("PopupFonts")) {
								for (auto& item : font_items) {
									if (ImGui::Selectable(item.second.c_str())) {
										ImGuiIO& io = ImGui::GetIO();
										imgui_font_index = item.first;
										io.FontDefault = fonts[imgui_font_index];
									}
								}
								ImGui::EndPopup();
							}
							ImGui::Text(I18Nc("common.setting.label-font"));
							ImGui::SameLine();
							if (ImGui::Button(font_items.at(imgui_font_index).c_str(), ImVec2(ImGui::GetFontSize() * 6, 0))) {
								ImGui::OpenPopup("PopupFonts");
							}

							ImGui::SameLine();
							ImGui::Text(I18Nc("common.setting.label-fontScale"));
							ImGui::SameLine();
							ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
							ImGui::DragFloat("##UI缩放", &ImGui::GetIO().FontGlobalScale, 0.005f, 0.5f, 1.8f, "%.2f", ImGuiSliderFlags_NoInput);
							ImGui::PopItemWidth();

							ImGui::Unindent();
						}
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::Text(I18Ni(ICON_MDI_TABLE_COG, "common.setting.label-uiStyle"));

						{
							ImGui::Indent();

							if (ImGui::BeginPopupContextItem("PopupThemes")) {
								for (auto& item : theme_items) {
									if (ImGui::Selectable(item.second.c_str())) {
										imgui_style_index = item.first;
										buildStyle();
									}
								}
								ImGui::EndPopup();
							}

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-theme"));
							ImGui::SameLine();

							if (ImGui::Button(theme_items.at(imgui_style_index).c_str(), ImVec2(ImGui::GetFontSize() * 6, 0))) {
								ImGui::OpenPopup("PopupThemes");
							}

							ImGui::BeginGroup();

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.checkbox-frameBorder"));
							ImGui::SameLine();
							ImGui::Checkbox("##checkbox-frameBorder", &frame_border);

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.checkbox-windowBorder"));
							ImGui::SameLine();
							ImGui::Checkbox("##checkbox-windowBorder", &window_border);

							ImGui::EndGroup();

							ImGui::SameLine();

							ImGui::BeginGroup();

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-windowRounding"));
							ImGui::SameLine();
							ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
							ImGui::SliderFloat("##WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
							ImGui::PopItemWidth();

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-frameRounding"));
							ImGui::SameLine();
							ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
							ImGui::SliderFloat("##FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
							ImGui::PopItemWidth();

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-grabRounding"));
							ImGui::SameLine();
							ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
							ImGui::SliderFloat("##GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
							ImGui::PopItemWidth();

							ImGui::EndGroup();
							ImGui::Unindent();
						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::Text(I18Ni(ICON_MDI_KEYBOARD_VARIANT, "common.setting.label-hotkey"));

						{
							ImGui::Indent();

							/*if (ImGui::BeginPopupContextItem("PopupHotkey")) {
								for (auto& item : hotkey_items) {
									if (ImGui::Selectable(item.second.c_str())) {
										hotkey = item.first;
									}
								}
								ImGui::EndPopup();
							}*/

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-hotkeySetting"));
							ImGui::SameLine();
							if (ImGui::Button(dinputhook::getKeyName(hotkeySetting, hotkeySettingModifier, isWaitHotkeySetting).c_str(), ImVec2(ImGui::GetFontSize() * 7, 0))) {
								//ImGui::OpenPopup("PopupHotkey");
								isWaitHotkeySetting = true;
							}

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-hotkeyFinder"));
							ImGui::SameLine();
							if (ImGui::Button(dinputhook::getKeyName(hotkeyItemFinder, hotkeyItemFinderModifier, isWaitItemFinder).c_str(), ImVec2(ImGui::GetFontSize() * 7, 0))) {
								//ImGui::OpenPopup("PopupHotkey2");
								isWaitItemFinder = true;
							}

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("common.setting.label-hotkeyFinderTrack"));
							ImGui::SameLine();
							if (ImGui::Button(dinputhook::getKeyName(hotkeyTrack, hotkeyTrackModifier, isWaitTrack).c_str(), ImVec2(ImGui::GetFontSize() * 7, 0))) {
								//ImGui::OpenPopup("PopupHotkey3");
								isWaitTrack = true;
							}

							ImGui::Unindent();
						}
						ImGui::Spacing();
#ifndef NDEBUG
						static float dddf = 0;
						static bool result = 0;
						if (ImGui::Button(" 测试globe", ImVec2(0, 0))) {
							//RE::TESGlobal* DBM_Count = RE::TESForm::LookupByEditorID<RE::TESGlobal>("DBM_Count");
							/*RE::TESGlobal* DBM_DisplayCount = RE::TESForm::LookupByEditorID<RE::TESGlobal>("DBM_DisplayCount");
							if (DBM_DisplayCount) {
								dddf++;
							}

							RE::TESGlobal* DBM_SortWait = RE::TESForm::LookupByEditorID<RE::TESGlobal>("DBM_SortWait");
							if (DBM_SortWait) {
								dddf++;
							}*/
						}
						ImGui::Text("%0.1f", dddf);

						ImGui::Text("%d", result);

						ImGui::Checkbox("demo", &show_demo_window);
						static char buf1[32] = "0x0092DE7";
						ImGui::InputText("default", buf1, 32);

						static char buf2[32] = "0xFF000E09";
						ImGui::InputText("default2", buf2, 32);

						static int iii = 0;
						ImGui::InputInt("索引测试", &iii, 1);

						static float fff = 0;
						ImGui::SliderFloat("时间", &fff, 0.0f, 10000.0f, "%.0f");

						static int shadername = 0;
						ImGui::Text("%08X", shadername);

						if (ImGui::Button(" 测试", ImVec2(0, 0))) {
							//const auto handler = RE::TESDataHandler::GetSingleton();
							//auto shaders = handler->GetFormArray<RE::TESEffectShader>();
							std::string formidstr = buf1;
							std::string formidstr2 = buf2;
							//RE::TESEffectShader* shader = shaders[iii];

							//RE::TESEffectShader cloneShader;
							//cloneShader.Copy(shader);

							//shader->Copy()
							RE::TESEffectShader* shader = RE::TESForm::LookupByID<RE::TESEffectShader>(std::stoi(formidstr, 0, 16));
							if (shader) {
								RE::TESObjectREFR* reff = RE::TESForm::LookupByID<RE::TESObjectREFR>(std::stoul(formidstr2, 0, 16));
								if (reff) {
									auto ambientSound = shader->data.ambientSound;
									shader->data.ambientSound = nullptr;
									shadername = shader->GetFormID();
									ScriptUtil::EffectShaderPlay(nullptr, 0, shader, reff, fff);
									shader->data.ambientSound = ambientSound;
								}
							}
						}
#endif
					}

					if (lotd::isLoad) {
						if (selected == 1) {
							ImGui::Text(I18Nc("lotd.setting.label-totalItemsIdentified"), lotd::formIds.size());

							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();

							static bool showAll = false;

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("lotd.setting.label-ModStatistics"));
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							/*ImGui::Text("显示所有");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);*/
							ImGui::Checkbox("##显示所有", &showAll);

							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Text(" | ");
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

							ImGui::Text("识别方式:");
							ImGui::SameLine();

							static bool displayWarning_po3_Tweaks = false;

							if (ImGui::BeginPopupContextItem("识别方式")) {
								for (auto& item : lotd::scanType_items) {
									if (ImGui::Selectable(item.second.c_str())) {
										if (item.first == 2) {
											if (!lotd::isLoad_po3_Tweaks) {
												displayWarning_po3_Tweaks = true;
												break;
											}
										}

										displayWarning_po3_Tweaks = false;
										if (item.first != lotd::scanType) {
											lotd::scanType = item.first;
											lotd::scanAll();
										}
									}
								}
								ImGui::EndPopup();
							}
							if (ImGui::Button(lotd::scanType_items.at(lotd::scanType).c_str(), ImVec2(ImGui::GetFontSize() * 6, 0))) {
								ImGui::OpenPopup("识别方式");
							}
							if (!lotd::isLoad_po3_Tweaks && displayWarning_po3_Tweaks) {
								ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
								myTextColored(ImVec4(1.0f, 0, 0, 1.0f), " 前置依赖：powerofthree's Tweaks 引擎修复模组 [po3_Tweaks.dll]");
							}

							if (ImGui::BeginTable("splitLotdMod", 8)) {
								for (const auto& pair : lotd::formIdsM) {
									if (pair.second.size() == 0 && lotd::loadCountsM[pair.first] == 0) {
										if (showAll) {
											ImGui::TableNextColumn();
											ImGui::Text("%s", setting::getLotdItemListModName(pair.first).c_str());

											if (ImGui::IsItemHovered()) {
												ImGui::BeginTooltip();
												ImGui::Text("%s", pair.first.c_str());
												ImGui::EndTooltip();
											}

											ImGui::TableNextColumn();
											myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), I18Nc("lotd.setting.label-notInstalled"));
										}
									} else {
										ImGui::TableNextColumn();
										ImGui::Text("%s", setting::getLotdItemListModName(pair.first).c_str());
										if (ImGui::IsItemHovered()) {
											ImGui::BeginTooltip();
											ImGui::Text("%s", pair.first.c_str());
											ImGui::EndTooltip();
										}

										ImGui::TableNextColumn();
										myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), I18Nc("lotd.setting.label-installed"), pair.second.size());
									}
								}
								ImGui::EndTable();
							}

#ifndef NDEBUG

							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
							ImGui::Text("房间物品统计");

							if (ImGui::BeginTable("splitLotdRoom", 2)) {
								for (const auto& pair : lotd::formIdsR) {
									ImGui::TableNextColumn();
									ImGui::Text(" %s", lotd::roomNames[pair.first].c_str());

									ImGui::TableNextColumn();
									ImGui::Text("%d(%d)个", pair.second.size(), lotd::displayIdsR[pair.first].size());
								}
								ImGui::EndTable();
							}

							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
							ImGui::Text("[调试]艺术馆物品清单：");
							static ImGuiTableFlags flagsItem =
								ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;
							const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
							if (ImGui::BeginTable("tableLotd", 2, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 20, TEXT_BASE_HEIGHT * 7), 0.0f)) {
								ImGui::TableSetupColumn("List", ImGuiTableColumnFlags_WidthFixed, 170.0f * ImGui::GetIO().FontGlobalScale, 1);
								ImGui::TableSetupColumn("数量", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, 2);
								ImGui::TableSetupScrollFreeze(0, 1);
								ImGui::TableHeadersRow();

								ImGuiListClipper clipper;
								clipper.Begin(lotd::lists.size());
								while (clipper.Step())
									for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
										lotd::List& item = lotd::lists[row_n];
										ImGui::PushID(item.name.c_str());
										ImGui::TableNextRow();
										ImGui::TableNextColumn();
										ImGui::Text("%s", item.name.c_str());
										ImGui::TableNextColumn();
										ImGui::Text("%d-%d-%d_%d-%d-%d", item.size, item.sizeACTI, item.sizeFLST, item.size2, item.sizeACTI2, item.sizeFLST2);
										ImGui::PopID();
									}
								ImGui::EndTable();
							}

#endif

							//ImGui::EndTabItem();
						}
					}

					if (selected == 2) {
						{
							ImGui::Text(I18Ni(ICON_MDI_TABLE_OF_CONTENTS, "common.setting.tab-hud"));

							ImGui::Indent();

							ImGui::BeginGroup();

							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayPlayerBaseInfo"), &show_player_base_info_window);

							if (show_player_base_info_window) {
								ImGui::Indent();
								ImGui::Checkbox(I18Nc("hud.setting.checkbox-playerBaseSplitDisplay"), &show_player_base_info_window_sep);
								static bool dd;
								ImGui::Checkbox("最大值修正", &show_player_base_info_window_max_fix);
								if (!show_player_base_info_window_sep) {
									ImGui::Checkbox(I18Nc("hud.setting.checkbox-playerBaseDisplayProgressBar"), &flag_process);
									if (flag_process) {
										ImGui::Indent();
										ImGui::ColorEdit4(I18Nc("hud.setting.checkbox-playerBaseColorHP"), &colorProgressHp.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
										ImGui::ColorEdit4(I18Nc("hud.setting.checkbox-playerBaseColorSP"), &colorProgressSp.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
										ImGui::ColorEdit4(I18Nc("hud.setting.checkbox-playerBaseColorMP"), &colorProgressMp.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
										ImGui::Unindent();
									}
								}
								ImGui::Unindent();
							}
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayPlayerInfo"), &show_player_info_window);
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayPlayerInfo2"), &show_player_mod_window);
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayWeaponInfo"), &show_player_weapon_window);
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayPositionInfo"), &show_player_debug_window);
							renderStatSettings();
							//ImGui::TableNextColumn();
							/*ImGui::TableNextColumn();
								ImGui::Checkbox("物品栏信息", &show_inv_window);
								if (show_inv_window) {
									if (ImGui::TreeNodeEx("设置##2", ImGuiTreeNodeFlags_DefaultOpen)) {
										ImGui::PushItemWidth(ImGui::GetFontSize() * 6);
										ImGui::DragInt("显示数据", &show_inv_window_height, 1, 15, 80, "%d条");
										ImGui::Checkbox("激活菜单时显示", &show_inv_window_active);
										ImGui::PopItemWidth();
										ImGui::TreePop();
									}
								}*/

							//ImGui::EndTable();
							//}

							ImGui::EndGroup();

							ImGui::SameLine();

							ImGui::BeginGroup();
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayArmorInfo"), &show_player_armor_window);
							renderNpcSettings();
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayPlayerBuffInfo"), &show_player_effects_window);
							if (show_player_effects_window) {
								ImGui::Indent();
								ImGui::Checkbox(I18Nc("hud.setting.checkbox-buffPermanentHidden"), &show_player_effects_ignore_permanent);
								ImGui::Checkbox(I18Nc("hud.setting.checkbox-buffNegativeRetained"), &show_player_effects_negative);
								ImGui::Checkbox(I18Nc("hud.setting.checkbox-buffSourceHidden"), &show_player_effects_ignore_spell);

								ImGui::Checkbox(I18Nc("hud.setting.checkbox-buffDisplayProgressBar"), &show_player_effects_process);
								if (show_player_effects_process) {
									ImGui::Indent();
									ImGui::ColorEdit4(I18Nc("hud.setting.checkbox-buffColor1"), &colorProgressEffect1.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
									ImGui::ColorEdit4(I18Nc("hud.setting.checkbox-buffColor2"), &colorProgressEffect3.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
									ImGui::Unindent();
								}
								ImGui::Checkbox(I18Nc("hud.setting.checkbox-buffIgnoreSetting"), &show_player_effects_listignore);

								if (show_player_effects_listignore) {
									static ImGuiTableFlags flagsItem =
										ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

									const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
									if (ImGui::BeginTable("tableItemBuffIngore", 3, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 12, TEXT_BASE_HEIGHT * 6), 0.0f)) {
										ImGui::TableSetupColumn(I18Nc("hud.setting.column-buffEffect"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);
										ImGui::TableSetupColumn(I18Nc("hud.setting.column-buffSource"), ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
										ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);

										ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
										ImGui::TableHeadersRow();

										int deleteEffectId = 0;
										int deleteSpellId = 0;

										ImGuiListClipper clipper;
										clipper.Begin(excludeEffectForms.size());
										while (clipper.Step())
											for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
												ExcludeFormEffect& item = excludeEffectForms[row_n];
												ImGui::PushID(row_n);
												ImGui::TableNextRow();
												ImGui::TableNextColumn();
												ImGui::Text("%s", item.spellName.c_str());
												ImGui::TableNextColumn();
												ImGui::Text("%s", item.effectName.c_str());
												ImGui::TableNextColumn();

												if (ImGui::SmallButton(ICON_MDI_CLOSE)) {
													deleteEffectId = item.effectId;
													deleteSpellId = item.spellId;
												}

												ImGui::PopID();
											}
										ImGui::EndTable();
										if (deleteEffectId || deleteSpellId) {
											ExcludeFormEffectIds d{ deleteEffectId, deleteSpellId };
											excludeEffectFormIds.erase(d);
											excludeEffectForms.erase(std::remove_if(excludeEffectForms.begin(), excludeEffectForms.end(),
																		 [&deleteEffectId, &deleteSpellId](const ExcludeFormEffect& x) {
																			 return x.effectId == deleteEffectId && x.spellId == deleteSpellId;
																		 }),
												excludeEffectForms.end());
										}
									}
								}

								ImGui::Unindent();
							}

							ImGui::EndGroup();

							ImGui::Unindent();
						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::Text(I18Ni(ICON_MDI_MAP_SEARCH_OUTLINE, "common.setting.label-itemStatistics"));
						{
							ImGui::Indent();
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyNirnRootQuantity"), &stats::showlocationNirnRootCount);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyNirnRootRedQuantity"), &stats::showlocationNirnRootRedCount);
							ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyVeinQuantity"), &stats::showlocationOreCount);
							ImGui::Unindent();
						}
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						if (lotd::isLoad) {
							ImGui::Text(I18Ni(ICON_MDI_HOME_MODERN, "finder.setting.label-lotd"));
							{
								ImGui::Indent();
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyLotdItemsQuantity"), &lotd::showlocationItemCount);
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyLotdExcavationQuantity"), &stats::showlocationExCount);
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayLotdItemsQuantity"), &lotd::showDisplayItemCount);
								ImGui::Unindent();
							}
							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
						}

						ImGui::Text(I18Ni(ICON_MDI_HEART, "common.setting.label-sexlab"));
						{
							ImGui::Indent();
							ImGui::Checkbox(I18Nc("hud.setting.checkbox-displayOSLArousal"), &sexlab::isShowPlayOSLArousal);
							ImGui::Unindent();
						}
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::Text(I18Ni(ICON_MDI_TABLE_COG, "common.setting.label-uiStyle"));

						{
							ImGui::Indent();

							ImGui::Checkbox(I18Nc("hud.setting.checkbox-transparentBackground"), &no_background);

							ImGui::Unindent();
						}
						ImGui::Spacing();
					}

					if (selected == 99) {
						RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

						ImGui::Checkbox("是否自动卸除箭袋", &auto_remove_ammo);
						if (auto_remove_ammo) {
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
							ImGui::Checkbox("是否记住箭矢类型", &remember_last_ammo);
						}

						ImGui::Separator();

						if (ImGui::TreeNodeEx("修改属性一", ImGuiTreeNodeFlags_DefaultOpen)) {
							if (ImGui::BeginTable("split99-1", 2)) {
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale);
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 0.0f);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("修改项:");

								ImGui::TableNextColumn();
								ImGui::Combo("##cbActorValue", &stateMod_selectIndex, actorValues, ((int)(sizeof(actorValues) / sizeof(*(actorValues)))), 6);
								if (player) {
									if (stateMod_selectIndex >= 0) {
										stateMod_nowValue = player->GetActorValue(actorValuesIndex[stateMod_selectIndex]);
									}
								}
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("当前值(总):");
								ImGui::TableNextColumn();
								ImGui::Text("[%.1f]", stateMod_nowValue);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								if (setvalue_Permanent) {
									ImGui::Text("增加值:");
								} else {
									ImGui::Text("修改值(临时):");
								}
								ImGui::TableNextColumn();
								if (stateMod_lastSelectIndex != stateMod_selectIndex) {
									stateMod_newValue = stateMod_nowValue;
									stateMod_lastSelectIndex = stateMod_selectIndex;
								}
								ImGui::InputFloat("##newValue", &stateMod_newValue, 10, 100, "%.1f", 0);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								if (ImGui::Button(ICON_MDI_CHECK " 确认修改")) {
									if (player) {
										if (stateMod_selectIndex >= 0) {
											if (setvalue_Permanent) {
												player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValuesIndex[stateMod_selectIndex], stateMod_newValue);
											} else {
												player->SetActorValue(actorValuesIndex[stateMod_selectIndex], stateMod_newValue);
											}
										}
									}
								}
								ImGui::TableNextColumn();
								ImGui::Checkbox("永久", &setvalue_Permanent);

								ImGui::EndTable();
							}

							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::TreeNodeEx("修改属性二(永久)", ImGuiTreeNodeFlags_DefaultOpen)) {
							if (ImGui::BeginTable("split99-2", 2, 0)) {
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale);
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 0.0f);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();

								ImGui::Text("修改项:");
								ImGui::TableNextColumn();
								ImGui::Combo("##cbActorValuePer", &statePerMod_selectIndex, perActorValues, ((int)(sizeof(perActorValues) / sizeof(*(perActorValues)))), 6);
								if (player) {
									if (statePerMod_selectIndex >= 0) {
										statePerMod_nowValue = player->GetActorValue(perActorValuesIndex[statePerMod_selectIndex]);
									}
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();

								ImGui::Text("当前值(总):");
								ImGui::TableNextColumn();
								ImGui::Text("[%.1f]", statePerMod_nowValue);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("修改值(基础):");
								ImGui::TableNextColumn();
								if (statePerMod_lastSelectIndex != statePerMod_selectIndex) {
									statePerMod_newValue = statePerMod_nowValue;
									statePerMod_lastSelectIndex = statePerMod_selectIndex;
								}
								ImGui::InputFloat("##newValuePer", &statePerMod_newValue, 10, 100, "%.1f", 0);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								if (ImGui::Button(ICON_MDI_CHECK " 确认修改")) {
									if (player) {
										if (statePerMod_selectIndex >= 0) {
											player->SetActorValue(perActorValuesIndex[statePerMod_selectIndex], statePerMod_newValue);
										}
									}
								}

								ImGui::EndTable();
							}
							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::TreeNodeEx("常用素材获取", ImGuiTreeNodeFlags_DefaultOpen)) {
							if (ImGui::BeginTable("split99-3", 2, 0)) {
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f * ImGui::GetIO().FontGlobalScale);
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 0.0f);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("获取项:");
								ImGui::TableNextColumn();
								ImGui::Combo("##cbInvGet", &getInv_selectIndex, invValues, ((int)(sizeof(invValues) / sizeof(*(invValues)))), 6);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("数量:");
								ImGui::TableNextColumn();
								ImGui::InputInt("##InvCount", &getInv_nowValue, 10, 9999);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								if (ImGui::Button(ICON_MDI_CHECK " 确认获取")) {
									std::string commandStr = "player.additem ";
									commandStr.append(invValuesCode[getInv_selectIndex]);
									commandStr.append(" ");
									commandStr.append(std::to_string(getInv_nowValue));
									ScriptUtil::ExecuteCommand(commandStr);
								}

								ImGui::EndTable();
							}
							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::TreeNodeEx("天气设置", ImGuiTreeNodeFlags_DefaultOpen)) {
							auto sky = RE::Sky::GetSingleton();
							auto calendar = RE::Calendar::GetSingleton();

							ImGui::Text(ICON_MDI_CLOCK_OUTLINE " 当前时间:");
							if (calendar) {
								auto time = &(calendar->gameHour->value);
								if (time) {
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
									ImGui::SliderFloat("##Time", time, 0.0f, 24.f, "%.2f");
									ImGui::PopItemWidth();
								}
							}
							if (sky) {
								auto weather = sky->currentWeather;
								if (weather) {
									ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
									auto flags = weather->data.flags;

									if (flags.any(RE::TESWeather::WeatherDataFlag::kPleasant)) {
										ImGui::Text(ICON_MDI_WEATHER_SUNNY " 天气:%s", weather->GetFormEditorID());
									} else if (flags.any(RE::TESWeather::WeatherDataFlag::kCloudy)) {
										ImGui::Text(ICON_MDI_WEATHER_CLOUDY " 天气:%s", weather->GetFormEditorID());
									} else if (flags.any(RE::TESWeather::WeatherDataFlag::kRainy)) {
										ImGui::Text(ICON_MDI_WEATHER_POURING " 天气:%s", weather->GetFormEditorID());
									} else if (flags.any(RE::TESWeather::WeatherDataFlag::kSnow)) {
										ImGui::Text(ICON_MDI_WEATHER_SNOWY " 天气:%s", weather->GetFormEditorID());
									} else if (flags.any(RE::TESWeather::WeatherDataFlag::kPermAurora)) {
										ImGui::Text(ICON_MDI_AURORA " 天气:%s", weather->GetFormEditorID());
									} else if (flags.any(RE::TESWeather::WeatherDataFlag::kAuroraFollowsSun)) {
										ImGui::Text(ICON_MDI_WEATHER_SUNNY " 天气:%s", weather->GetFormEditorID());
									} else {
										ImGui::Text(ICON_MDI_WEATHER_PARTLY_CLOUDY " 天气:%s", weather->GetFormEditorID());
									}
								}
							}

							static ImGuiTableFlags flagsItem =
								ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_NoBordersInBody;

							const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
							if (ImGui::BeginTable("tableWeather", 4, flagsItem, ImVec2(TEXT_BASE_HEIGHT * 16, TEXT_BASE_HEIGHT * 9), 0.0f)) {
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 20.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_3);
								ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 20.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_4);
								ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_WidthFixed, 130.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_2);
								ImGui::TableSetupColumn("FORMID", ImGuiTableColumnFlags_WidthFixed, 90.0f * ImGui::GetIO().FontGlobalScale, PlayerInfoColumnID_1);

								ImGui::TableSetupScrollFreeze(0, 1);  // Make row always visible
								ImGui::TableHeadersRow();

								ImGuiListClipper clipper;
								clipper.Begin(weatherForms.size());
								while (clipper.Step())
									for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
										WeatherForm& item = weatherForms[row_n];
										ImGui::PushID(item.formId + 0x1000000);
										ImGui::TableNextRow();

										ImGui::TableNextColumn();
										if (currentWeather != item.formId) {
											if (ImGui::SmallButton(ICON_MDI_CHECK)) {
												auto sky = RE::Sky::GetSingleton();
												if (sky) {
													auto weather = RE::TESForm::LookupByID<RE::TESWeather>(item.formId);
													if (weather) {
														sky->ForceWeather(weather, true);
														currentWeather = item.formId;
													}
												}
											}
										}
										ImGui::TableNextColumn();
										switch (item.flag) {
										case RE::TESWeather::WeatherDataFlag::kPleasant:
											ImGui::Text(ICON_MDI_WEATHER_SUNNY);
											break;
										case RE::TESWeather::WeatherDataFlag::kCloudy:
											ImGui::Text(ICON_MDI_WEATHER_CLOUDY);
											break;
										case RE::TESWeather::WeatherDataFlag::kRainy:
											ImGui::Text(ICON_MDI_WEATHER_POURING);
											break;
										case RE::TESWeather::WeatherDataFlag::kSnow:
											ImGui::Text(ICON_MDI_WEATHER_SNOWY);
											break;
										case RE::TESWeather::WeatherDataFlag::kPermAurora:
											ImGui::Text(ICON_MDI_AURORA);
											break;
										case RE::TESWeather::WeatherDataFlag::kAuroraFollowsSun:
											ImGui::Text(ICON_MDI_WEATHER_SUNNY);
											break;
										default:
											ImGui::Text(ICON_MDI_WEATHER_PARTLY_CLOUDY);
											break;
										}

										ImGui::TableNextColumn();
										ImGui::Text("%s", item.editorId.c_str());
										ImGui::TableNextColumn();
										ImGui::Text("%08X", item.formId);

										ImGui::PopID();
									}
								ImGui::EndTable();
							}

							ImGui::TreePop();
						}
						//ImGui::EndTabItem();

						//LoadFromWICFile
						//ImGui::Image();
						//HRESULT hr = DirectX::LoadFromWICFile(L"your_image.png", DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
						//WCHAR strFile[40];
						//wsprintfW(strFile, L"E:\\test.png");

						/*if (ImGui::SmallButton("dsfdfdf")) {
							auto manager = RE::BSRenderManager::GetSingleton();
							auto device = reinterpret_cast<ID3D11Device*>(manager->forwarder);
							HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"E:\\test.png", &texture, &textureView);
							if (SUCCEEDED(hr)) {
								istestload = true;
							}
						}
						if (istestload) {
							ImGui::Image(textureView, ImVec2(400, 400));
						}*/
					}
				}

				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::BeginPopupModal(I18Nc("common.setting.popup-info"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
					ImGui::Text(I18Nc("common.setting.label-configSaved"));
					if (ImGui::Button(I18Nc("common.setting.btn-ok"), ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (ImGui::Button(I18Ni(ICON_MDI_CONTENT_SAVE, "common.setting.btn-saveConfig"))) {
					setting::save_settings();
					ImGui::OpenPopup(I18Nc("common.setting.popup-info"));
				}

				ImGui::EndGroup();
				ImGui::End();
			}
		}
	}
}
