//#include <d3d9.h>
//#pragma comment(lib, "d3d9.lib")
//#include "RE/P/PlayerCharacter.h"
//#include "imgui.h"
//#include "imgui_impl_dx9.h"
//#include "imgui_impl_win32.h"
//#include "main_skyrim.h"
//#define UNICODE
#include "main.h"
#include <nlohmann/json.hpp>
#include <memory/memory.h>
#include <hook/d3d11hook.h>
#include <imgui.h>
#include <dinput.h>
#include <WinUser.h>
#include <Windows.h>

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	settings = "data\\skse\\plugins\\sse-hud.json";
	fontFilePath = "data\\skse\\plugins\\msyh.ttc";


	//MessageBox(nullptr, TEXT("测试中文."), nullptr, MB_OK);
	load_settings();

	try {
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

#else
		auto path = logger::log_directory();
		if (!path) {
			return false;
		}

		//*path /= Version::PROJECT;
		*path /= "imgui hud";
		*path += ".log"sv;
		auto sink = make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif
		auto log = make_shared<spdlog::logger>("global log"s, move(sink));

#ifndef NDEBUG
		log->set_level(spdlog::level::trace);
#else
		log->set_level(spdlog::level::trace);
		log->flush_on(spdlog::level::trace);
#endif

		spdlog::set_default_logger(move(log));
		spdlog::set_pattern("[%H:%M:%S.%f] %s(%#) [%^%l%$] %v"s);

		logger::info(FMT_STRING("{} v{}"), "sse-hud", "sse-hud");

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "sse-hud";
		a_info->version = 1;

		if (a_skse->IsEditor()) {
			logger::critical("Loaded in editor, marking as incompatible"sv);
			return false;
		}

		const auto ver = a_skse->RuntimeVersion();
		if (ver < SKSE::RUNTIME_1_5_39) {
			logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
			return false;
		}
	} catch (const std::exception& e) {
		logger::critical("failed, cause {}"sv, e.what());
		return false;
	}

	return true;
}

int SehFilter(DWORD dwExceptionCode)
{
	switch (dwExceptionCode) {
	case EXCEPTION_ACCESS_VIOLATION:
		return EXCEPTION_EXECUTE_HANDLER;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

// Table with pointers
//imgui_api imgui;
//
//static ImFont* font;

//static void myText(const char* fmt, ...)
//{
//	va_list args;
//	va_start(args, fmt);
//	if (!bullet_text) {
//		imgui.igBulletTextV(fmt, args);
//	} else {
//		imgui.igTextV(fmt, args);
//	}
//	va_end(args);
//}
//
//static void __stdcall render(int active)
//{
//	// 当打开菜单时不显示
//	if (isOpenCursorMenu && !active) {
//		return;
//	}
//
//	ImGuiWindowFlags window_flags = 0;
//	if (no_titlebar)
//		window_flags |= ImGuiWindowFlags_NoTitleBar;
//	if (no_resize)
//		window_flags |= ImGuiWindowFlags_NoResize;
//	if (no_collapse)
//		window_flags |= ImGuiWindowFlags_NoCollapse;
//	if (no_background)
//		window_flags |= ImGuiWindowFlags_NoBackground;
//	if (auto_resize)
//		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
//
//	// 载入字体
//	imgui.igPushFont(font);
//
//	auto style = imgui.igGetStyle();
//	style->WindowBorderSize = window_border ? 1.0f : 0.0f;
//	style->FrameBorderSize = frame_border ? 1.0f : 0.0f;
//
//	if (show_player_base_info_window) {
//		imgui.igBegin("人物状态", nullptr, window_flags);
//		
//		progress = playerInfo.kHealth / (playerInfo.kHealthBase == 0 ? 1 : playerInfo.kHealthBase);
//		progress2 = playerInfo.kStamina / (playerInfo.kStaminaBase == 0 ? 1 : playerInfo.kStaminaBase);
//		progress3 = playerInfo.kMagicka / (playerInfo.kMagickaBase == 0 ? 1 : playerInfo.kMagickaBase);
//
//		if (flag_process) {
//			myText("生命:");
//			imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//			char buf[32];
//			sprintf(buf, "%d/%d", (int)playerInfo.kHealth, (int)playerInfo.kHealthBase);
//			imgui.igProgressBar(progress, ImVec2(0.f, 0.f), buf);
//
//			myText("耐力:");
//			imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//			char buf2[32];
//			sprintf(buf2, "%d/%d", (int)playerInfo.kStamina, (int)playerInfo.kStaminaBase);
//			imgui.igProgressBar(progress2, ImVec2(0.f, 0.f), buf2);
//
//			myText("魔法:");
//			imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//			char buf3[32];
//			sprintf(buf3, "%d/%d", (int)playerInfo.kMagicka, (int)playerInfo.kMagickaBase);
//			imgui.igProgressBar(progress3, ImVec2(0.f, 0.f), buf3);
//
//		} else {
//			myText("生命:");
//			imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//			if (progress > 0.15) {
//				imgui.igText("%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
//			} else {
//				imgui.igTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kHealth, playerInfo.kHealthBase);
//			}
//			myText("耐力:");
//			imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//			if (progress2 > 0.15) {
//				imgui.igText("%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
//			} else {
//				imgui.igTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kStamina, playerInfo.kStaminaBase);
//			}
//			myText("魔法:");
//			imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//			if (progress3 > 0.15) {
//				imgui.igText("%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
//			} else {
//				imgui.igTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.1f / %.0f ", playerInfo.kMagicka, playerInfo.kMagickaBase);
//			}
//		}
//		imgui.igEnd();
//	}
//
//	if (show_player_info_window) {
//		imgui.igBegin("人物属性", nullptr, window_flags);
//
//		//imgui.igText("Health:%d", (int)health);
//		//imgui.igText("Magicka:%d", (int)kMagicka);
//		//imgui.igText("Stamina:%d", (int)kStamina);
//		myText("伤害: %s", playerInfo.DamageStr.c_str());
//		//myText("防御: %s", playerInfo.DamageResist.c_str());
//		myText("防御: %.2f", playerInfo.kDamageResist);
//		myText("毒抗: %.1f%%", playerInfo.kPoisonResist);
//		myText("火抗: %.1f%%", playerInfo.kResistFire);
//		myText("雷抗: %.1f%%", playerInfo.kResistShock);
//		myText("冰抗: %.1f%%", playerInfo.kResistFrost);
//		myText("法抗: %.1f%%", playerInfo.kResistMagic);
//		myText("抗疾病: %.1f%%", playerInfo.kResistDisease);
//		//imgui.igText("criticalChance:%d", (int)kCriticalChance);
//		//imgui.igText("armor:%d", (int)kDamageResist);
//		imgui.igEnd();
//	}
//
//	if (show_player_mod_window) {
//		imgui.igBegin("属性加成", nullptr, window_flags);
//
//		myText("单手: %.1f%%", playerInfo.kOneHandedModifier);
//		myText("双手: %.1f%%", playerInfo.kTwoHandedModifier);
//		myText("弓箭: %.1f%%", playerInfo.kMarksmanModifier);
//		myText("锻造: %.1f%%", playerInfo.kSmithingModifier);
//		myText("锻造(power): %.1f%%", playerInfo.kSmithingPowerModifier);
//		myText("锻造(skill): %.1f%%", playerInfo.kSmithingSkillAdvance);
//		myText("炼金: %.1f%%", playerInfo.kAlchemyModifier);
//		myText("炼金(power): %.1f%%", playerInfo.kAlchemyPowerModifier);
//		myText("炼金(skill): %.1f%%", playerInfo.kAlchemySkillAdvance);
//		myText("附魔: %.1f%%", playerInfo.kEnchantingModifier);
//		myText("附魔(power): %.1f%%", playerInfo.kEnchantingPowerModifier);
//		myText("附魔(skill): %.1f%%", playerInfo.kEnchantingSkillAdvance);
//		imgui.igEnd();
//	}
//
//	if (show_player_weapon_window) {
//		imgui.igBegin("武器", nullptr, window_flags);
//
//		if (leftWeaponInfo.isExist) {
//			if (imgui.igTreeNodeStr(leftWeaponInfo.treeId.c_str())) {
//				myText("类型: %s", leftWeaponInfo.formTypeName.c_str());
//				myText("ID: %s", leftWeaponInfo.formID.c_str());
//				imgui.igSameLine(0, 0);
//				if (imgui.igSmallButton("卸载")) {
//					std::string commandStr = "player.unequipItem ";
//					commandStr.append(leftWeaponInfo.formID);
//
//					// 调用控制台
//					const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
//					const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
//					if (script) {
//						const auto selectedRef = RE::Console::GetSelectedRef();
//						script->SetCommand(commandStr);
//						script->CompileAndRun(selectedRef.get());
//						delete script;
//					}
//				}
//
//				switch (leftWeaponInfo.formType) {
//				case RE::FormType::Weapon:
//					{
//						myText("武器类型: %s", leftWeaponInfo.weaponTypeName.c_str());
//						myText("武器伤害: %d", leftWeaponInfo.damage);
//						myText("暴击伤害: %d", leftWeaponInfo.critDamage);
//						myText("价格: %d", leftWeaponInfo.goldValue);
//						myText("属性值: %d$", leftWeaponInfo.value);
//						myText("重量: %.1f", leftWeaponInfo.weight);
//						break;
//					}
//				case RE::FormType::Spell:
//					{
//						myText("法术类型: %s", leftWeaponInfo.castingTypeName.c_str());
//						myText("法术类型: %s", leftWeaponInfo.spellTypeName.c_str());
//						myText("花费: %.1f", leftWeaponInfo.cost);
//						myText("施法时间: %.1f", leftWeaponInfo.time);
//						break;
//					}
//
//				default:
//					break;
//				}
//
//				//myText("插槽: %s", item.equipSlotName.c_str());
//				imgui.igTreePop();
//			}
//		}
//
//		if (rightWeaponInfo.isExist) {
//			// 不显示双手武器
//			if (!rightWeaponInfo.isTwoHand) {
//				if (imgui.igTreeNodeStr(rightWeaponInfo.treeId.c_str())) {
//					myText("类型: %s", rightWeaponInfo.formTypeName.c_str());
//					myText("ID: %s", rightWeaponInfo.formID.c_str());
//					imgui.igSameLine(0, 0);
//					if (imgui.igSmallButton("卸载")) {
//						std::string commandStr = "player.unequipItem ";
//						commandStr.append(rightWeaponInfo.formID);
//
//						// 调用控制台
//						const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
//						const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
//						if (script) {
//							const auto selectedRef = RE::Console::GetSelectedRef();
//							script->SetCommand(commandStr);
//							script->CompileAndRun(selectedRef.get());
//							delete script;
//						}
//					}
//
//					switch (rightWeaponInfo.formType) {
//					case RE::FormType::Weapon:
//						{
//							myText("武器类型: %s", rightWeaponInfo.weaponTypeName.c_str());
//							myText("武器伤害: %d", rightWeaponInfo.damage);
//							myText("暴击伤害: %d", rightWeaponInfo.critDamage);
//							myText("价格: %d", rightWeaponInfo.goldValue);
//							myText("属性值: %d$", rightWeaponInfo.value);
//							myText("重量: %.1f", rightWeaponInfo.weight);
//							break;
//						}
//					case RE::FormType::Spell:
//						{
//							myText("法术类型: %s", rightWeaponInfo.castingTypeName.c_str());
//							myText("法术类型: %s", rightWeaponInfo.spellTypeName.c_str());
//							myText("花费: %.1f", rightWeaponInfo.cost);
//							myText("施法时间: %.1f", rightWeaponInfo.time);
//							break;
//						}
//					default:
//						break;
//					}
//
//					imgui.igTreePop();
//				}
//			}
//		}
//
//		if (ammoInfo.isExist) {
//			if (imgui.igTreeNodeStr(ammoInfo.treeId.c_str())) {
//				myText("类型: %s", ammoInfo.formTypeName.c_str());
//				myText("ID: %s", ammoInfo.formID.c_str());
//				imgui.igSameLine(0, 0);
//				if (imgui.igSmallButton("卸载")) {
//					std::string commandStr = "player.unequipItem ";
//					commandStr.append(ammoInfo.formID);
//
//					// 调用控制台
//					const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
//					const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
//					if (script) {
//						const auto selectedRef = RE::Console::GetSelectedRef();
//						script->SetCommand(commandStr);
//						script->CompileAndRun(selectedRef.get());
//						delete script;
//					}
//				}
//				myText("伤害: %.1f", ammoInfo.damage);
//				myText("价格: %d", ammoInfo.goldValue);
//				myText("属性值: %d$", ammoInfo.value);
//				myText("重量: %.1f", ammoInfo.weight);
//
//				imgui.igTreePop();
//			}
//		}
//
//		imgui.igEnd();
//	}
//
//	if (show_player_armor_window) {
//		imgui.igBegin("装备", nullptr, window_flags);
//
//		int unWornArmosAlertSlots = 0;
//
//		for (int i = 0; i <= 31; i++) {
//			auto item = wornArmos[i];
//
//			if (item.isExist) {
//				if (item.isSpeacilSlotAlert) {
//					// 变黄色
//					//imgui.igPushStyleColor
//				}
//
//				if (imgui.igTreeNodeStr(item.treeId.c_str())) {
//					myText("类型: %s", item.formTypeName.c_str());
//					myText("装备类型: %s", item.armorTypeName.c_str());
//					myText("ID: %s", item.formID.c_str());
//					imgui.igSameLine(0, 0);
//					if (imgui.igSmallButton("卸载")) {
//						std::string commandStr = "player.unequipItem ";
//						commandStr.append(item.formID);
//
//						// 调用控制台
//						const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
//						const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
//						if (script) {
//							const auto selectedRef = RE::Console::GetSelectedRef();
//							script->SetCommand(commandStr);
//							script->CompileAndRun(selectedRef.get());
//							delete script;
//						}
//					}
//					myText("价格: %d$", item.goldValue);
//					myText("属性值: %d", item.value);
//					myText("装备等级: %.2f", item.armorRating);
//					myText("插槽: %s", item.equipSlotName.c_str());
//					myText("重量: %.1f", item.weight);
//					imgui.igTreePop();
//				}
//			} else {
//				if (item.isMainSlotAlert) {
//					unWornArmosAlertSlots += 1 << i;
//					// 标记未装备
//				}
//			}
//		}
//
//		// 未装备插槽显示红色未装备
//		for (int i = 0; i <= 31; i++) {
//			int mask = 1 << i;
//			if ((unWornArmosAlertSlots & mask) == mask) {
//				imgui.igTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "未装备 | %s", wornArmos[i].equipSlotName.c_str());
//			}
//		}
//
//		imgui.igEnd();
//	}
//
//	if (show_player_debug_window) {
//		imgui.igBegin("其他信息", nullptr, window_flags);
//		myText("人物坐标: [%.0f,%.0f,%.0f]", playerInfo.Position.x, playerInfo.Position.y, playerInfo.Position.z);
//		//imgui.igSameLine(0, 0);
//		myText("人物视角: [%.2f,%.2f]", playerInfo.Angle.x, playerInfo.Angle.z);
//		imgui.igEnd();
//	}
//
//	if (show_enemy_window) {
//		time_t now1 = time(NULL);
//		imgui.igBegin("敌人信息", nullptr, window_flags);
//		EnemyInfo* enemyInfo = getEnemyData();
//		for (int i = 0; i < 50; i++) {
//			EnemyInfo& item = enemyInfo[i];
//			if (item.kHealth > 0) {
//				if (now1 - item.updateTime < 40) {
//					float enemyHealthRate = item.kHealth / (item.kHealthBase == 0 ? 1 : item.kHealthBase);
//
//					myText("[%d] %s [", item.level, item.name.c_str());
//					imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//					if (enemyHealthRate > 0.85f) {
//						imgui.igTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%.0f/%.0f ", item.kHealth, item.kHealthBase);
//					} else if (enemyHealthRate < 0.20f) {
//						imgui.igTextColored(ImVec4(1, 0, 0.0f, 1.0f), "%.0f/%.0f ", item.kHealth, item.kHealthBase);
//					} else {
//						imgui.igText("%.0f/%.0f ", item.kHealth, item.kHealthBase);
//					}
//					imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//					imgui.igText("%.0f ", item.kStamina);
//					imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//					imgui.igTextColored(ImVec4(0.0f, 0, 1, 1.0f), "%.0f", item.kMagicka);
//					imgui.igSameLine(0.0f, style->ItemInnerSpacing.x);
//					//imgui.igText("] %d", item.isSentient);
//					imgui.igText("]");
//				}
//			}
//		}
//		imgui.igEnd();
//	}
//
//	if (show_crosshair) {
//		imgui.igBegin("准星信息", nullptr, window_flags);
//		RE::TESObjectREFR* _cachedRef = BSTCrosshairRefEvent::GetSingleton()->GetCrosshairReference();
//		if (_cachedRef) {
//			logger::debug("准星");
//			logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetDisplayFullName()));
//			logger::debug(_cachedRef->GetFormID());
//		}
//
//		imgui.igEnd();
//	}
//
//	if (active) {
//		static bool show_demo_window = false;
//		static bool show_another_window = false;
//
//		// 瀹樻柟Demo
//		if (show_demo_window)
//			imgui.igShowDemoWindow(&show_demo_window);
//
//		{
//			imgui.igBegin("控制台", nullptr, 0);
//
//			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
//			if (imgui.igBeginTabBar("MyTabBar", tab_bar_flags)) {
//				if (imgui.igBeginTabItem("插件设置", 0, 0)) {
//					if (imgui.igCollapsingHeader("HUD设置", ImGuiTreeNodeFlags_DefaultOpen)) {
//						imgui.igBeginGroup();
//
//						imgui.igCheckbox("人物基本属性", &show_player_base_info_window);
//
//						if (show_player_base_info_window) {
//							if (imgui.igTreeNodeExStr("人物基本属性 - 设置", ImGuiTreeNodeFlags_DefaultOpen)) {
//								imgui.igCheckbox("显示进度条", &flag_process);
//								if (flag_process) {
//									imgui.igColorEdit4("进度条颜色", (float*)&(style->Colors[ImGuiCol_PlotHistogram]), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
//								}
//								imgui.igTreePop();
//							}
//						}
//
//						imgui.igCheckbox("人物属性", &show_player_info_window);
//						imgui.igCheckbox("武器信息", &show_player_weapon_window);
//						imgui.igCheckbox("装备信息", &show_player_armor_window);
//						imgui.igCheckbox("人物属性加成", &show_player_mod_window);
//						imgui.igCheckbox("其他信息", &show_player_debug_window);
//						imgui.igCheckbox("敌人信息", &show_enemy_window);
//
//						imgui.igCheckbox("是否自动卸除箭袋", &auto_remove_ammo);
//
//						imgui.igEndGroup();
//					}
//
//					if (imgui.igCollapsingHeader("窗口设置", 0)) {
//						imgui.igBeginGroup();
//
//						static int style_idx = imgui_style_index;
//						if (imgui.igComboStr("主题", &style_idx, "Dark\0Light\0Classic\0", -1)) {
//							switch (style_idx) {
//							case 0:
//								imgui.igStyleColorsDark(imgui.igGetStyle());
//								imgui_style_index = 0;
//								break;
//							case 1:
//								imgui.igStyleColorsLight(imgui.igGetStyle());
//								imgui_style_index = 1;
//								break;
//							case 2:
//								imgui.igStyleColorsClassic(imgui.igGetStyle());
//								imgui_style_index = 2;
//								break;
//							}
//						}
//
//						//imgui.igShowStyleSelector("style");
//						//ImGui::TableNextColumn();
//						imgui.igCheckbox("不显示标题", &no_titlebar);
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						imgui.igCheckbox("不允许缩放", &no_resize);
//						imgui.igCheckbox("不允许折叠", &no_collapse);
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						imgui.igCheckbox("不显示背景", &no_background);
//						imgui.igCheckbox("窗体边框", &window_border);
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						imgui.igCheckbox("控件边框", &frame_border);
//
//						imgui.igCheckbox("是否窗口自适应", &auto_resize);
//						imgui.igCheckbox("不显示圆形", &bullet_text);
//						imgui.igDragFloat("窗体缩放", &imgui.igGetIO()->FontGlobalScale, 0.005f, 0.5f, 1.8f, "%.2f", 1);
//						imgui.igDragInt("数据刷新(ms)", &refresh_time_data, 1, 100, 500, "%d ms");
//
//						//ImGui::EndTable();
//						imgui.igEndGroup();
//					}
//					if (imgui.igButton("保存配置", ImVec2(0, 0))) {
//						auto colorPlotHistogram = style->Colors[ImGuiCol_PlotHistogram];
//						colorPlotHistogramX = colorPlotHistogram.x;
//						colorPlotHistogramY = colorPlotHistogram.y;
//						colorPlotHistogramZ = colorPlotHistogram.z;
//						colorPlotHistogramW = colorPlotHistogram.w;
//
//						save_settings();
//					}
//					imgui.igEndTabItem();
//				}
//				if (imgui.igBeginTabItem("快捷操作", 0, 0)) {
//					RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
//					if (imgui.igTreeNodeExStr("属性修改(临时)", ImGuiTreeNodeFlags_DefaultOpen)) {
//						myText("修改项:");
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						imgui.igCombo("##cbActorValue", &stateMod_selectIndex, actorValues, ((int)(sizeof(actorValues) / sizeof(*(actorValues)))), 6);
//						if (player) {
//							if (stateMod_selectIndex >= 0) {
//								stateMod_nowValue = player->GetActorValue(actorValuesIndex[stateMod_selectIndex]);
//							}
//						}
//						myText("当前值: [%.1f]", stateMod_nowValue);
//						myText("修改值:");
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						if (stateMod_lastSelectIndex != stateMod_selectIndex) {
//							stateMod_newValue = stateMod_nowValue;
//							stateMod_lastSelectIndex = stateMod_selectIndex;
//						}
//						imgui.igInputFloat("##newValue", &stateMod_newValue, 10, 100, "%.1f", 0);
//
//						if (imgui.igButton("确认修改", ImVec2())) {
//							if (player) {
//								if (stateMod_selectIndex >= 0) {
//									player->SetActorValue(actorValuesIndex[stateMod_selectIndex], stateMod_newValue);
//								}
//							}
//						}
//						imgui.igTreePop();
//					}
//					if (imgui.igTreeNodeStr("属性修改(永久)")) {
//						myText("修改项:");
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						imgui.igCombo("##cbActorValuePer", &statePerMod_selectIndex, perActorValues, ((int)(sizeof(perActorValues) / sizeof(*(perActorValues)))), 6);
//						if (player) {
//							if (statePerMod_selectIndex >= 0) {
//								statePerMod_nowValue = player->GetActorValue(perActorValuesIndex[statePerMod_selectIndex]);
//							}
//						}
//						myText("当前值: [%.1f]", statePerMod_nowValue);
//						myText("修改值:");
//						imgui.igSameLine(0.0f, imgui.igGetStyle()->ItemInnerSpacing.x);
//						if (statePerMod_lastSelectIndex != statePerMod_selectIndex) {
//							statePerMod_newValue = statePerMod_nowValue;
//							statePerMod_lastSelectIndex = statePerMod_selectIndex;
//						}
//						imgui.igInputFloat("##newValuePer", &statePerMod_newValue, 10, 100, "%.1f", 0);
//
//						if (imgui.igButton("确认修改", ImVec2())) {
//							if (player) {
//								if (statePerMod_selectIndex >= 0) {
//									player->SetActorValue(perActorValuesIndex[statePerMod_selectIndex], statePerMod_newValue);
//								}
//							}
//						}
//						imgui.igTreePop();
//					}
//
//					imgui.igEndTabItem();
//				}
//
//				imgui.igEndTabBar();
//			}
//
//			//imgui.igText("Application average %.3f ms/frame (%.1f FPS)",
//			//	1000.0f / imgui.igGetIO()->Framerate, imgui.igGetIO()->Framerate);
//			imgui.igEnd();
//		}
//	}
//
//	imgui.igPopFont();
//}

//static void ImguiMessageHandler(SKSE::MessagingInterface::Message* m)
//{
//	if (m->type != SSEIMGUI_API_VERSION)
//		return;
//
//	sseimgui_api* sseimgui = reinterpret_cast<sseimgui_api*>(m->data);
//	imgui = sseimgui->make_imgui_api();
//
//	//const char* font_path = "D:\\msyh.ttc";
//	if (std::filesystem::exists(fontFilePath)) {
//		auto io = imgui.igGetIO();
//		//auto fonts = imgui.ImFontAtlas_ImFontAtlas();
//		font = imgui.ImFontAtlas_AddFontFromFileTTF(io->Fonts, fontFilePath.c_str(), 20.0f, NULL, imgui.ImFontAtlas_GetGlyphRangesChineseFull(io->Fonts));
//		imgui.ImFontAtlas_Build(io->Fonts);
//	}
//
//	switch (imgui_style_index) {
//	case 0:
//		imgui.igStyleColorsDark(imgui.igGetStyle());
//		break;
//	case 1:
//		imgui.igStyleColorsLight(imgui.igGetStyle());
//		break;
//	case 2:
//		imgui.igStyleColorsClassic(imgui.igGetStyle());
//		break;
//	}
//	imgui.igGetStyle()->Colors[ImGuiCol_PlotHistogram] = ImVec4(colorPlotHistogramX, colorPlotHistogramY, colorPlotHistogramZ, colorPlotHistogramW);
//
//	sseimgui->render_listener(&render, 0);
//}


void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostLoad:
		{
			logger::info("PostLoad"sv);
			auto messaging = SKSE::GetMessagingInterface();
	/*		if (!messaging->RegisterListener("SSEIMGUI", ImguiMessageHandler)) {
			}*/
			break;
		}
	case SKSE::MessagingInterface::kPostPostLoad:
		{
			logger::info("PostPostLoad"sv);
			break;
		}
	case SKSE::MessagingInterface::kPreLoadGame:
		{
			logger::info("kPreLoadGame"sv);
			break;
		}
	case SKSE::MessagingInterface::kPostLoadGame:
		{
			logger::info("kPostLoadGame"sv);
			startflag = true;
			break;
		}
	case SKSE::MessagingInterface::kSaveGame:
		{
			logger::info("kSaveGame"sv);
			break;
		}
	case SKSE::MessagingInterface::kDeleteGame:
		{
			logger::info("kDeleteGame"sv);
			break;
		}
	case SKSE::MessagingInterface::kInputLoaded:
		{
			logger::info("kInputLoaded"sv);
			break;
		}
	case SKSE::MessagingInterface::kDataLoaded:
		{
			logger::info("kDataLoaded"sv);
			//MessageBox(nullptr, "SKSE kDataLoaded", nullptr, MB_OK);

			/*		RE::GFxValue obj;
		

			RE::GFxValue fxValue;
			fxValue.SetNumber(90);
			obj.SetMember("goldNumber", fxValue);

			RE::GFxValue fxValue2;
			fxValue2.SetNumber(100);
			obj.SetMember("encumbranceNumber", fxValue);*/

			break;
		}
	}
}


#define IM_VK_KEYPAD_ENTER (VK_RETURN + 256)
static ImGuiKey ImGui_ImplWin32_VirtualKeyToImGuiKey(WPARAM wParam)
{
	switch (wParam) {
	case VK_TAB:
		return ImGuiKey_Tab;
	case VK_LEFT:
		return ImGuiKey_LeftArrow;
	case VK_RIGHT:
		return ImGuiKey_RightArrow;
	case VK_UP:
		return ImGuiKey_UpArrow;
	case VK_DOWN:
		return ImGuiKey_DownArrow;
	case VK_PRIOR:
		return ImGuiKey_PageUp;
	case VK_NEXT:
		return ImGuiKey_PageDown;
	case VK_HOME:
		return ImGuiKey_Home;
	case VK_END:
		return ImGuiKey_End;
	case VK_INSERT:
		return ImGuiKey_Insert;
	case VK_DELETE:
		return ImGuiKey_Delete;
	case VK_BACK:
		return ImGuiKey_Backspace;
	case VK_SPACE:
		return ImGuiKey_Space;
	case VK_RETURN:
		return ImGuiKey_Enter;
	case VK_ESCAPE:
		return ImGuiKey_Escape;
	case VK_OEM_7:
		return ImGuiKey_Apostrophe;
	case VK_OEM_COMMA:
		return ImGuiKey_Comma;
	case VK_OEM_MINUS:
		return ImGuiKey_Minus;
	case VK_OEM_PERIOD:
		return ImGuiKey_Period;
	case VK_OEM_2:
		return ImGuiKey_Slash;
	case VK_OEM_1:
		return ImGuiKey_Semicolon;
	case VK_OEM_PLUS:
		return ImGuiKey_Equal;
	case VK_OEM_4:
		return ImGuiKey_LeftBracket;
	case VK_OEM_5:
		return ImGuiKey_Backslash;
	case VK_OEM_6:
		return ImGuiKey_RightBracket;
	case VK_OEM_3:
		return ImGuiKey_GraveAccent;
	case VK_CAPITAL:
		return ImGuiKey_CapsLock;
	case VK_SCROLL:
		return ImGuiKey_ScrollLock;
	case VK_NUMLOCK:
		return ImGuiKey_NumLock;
	case VK_SNAPSHOT:
		return ImGuiKey_PrintScreen;
	case VK_PAUSE:
		return ImGuiKey_Pause;
	case VK_NUMPAD0:
		return ImGuiKey_Keypad0;
	case VK_NUMPAD1:
		return ImGuiKey_Keypad1;
	case VK_NUMPAD2:
		return ImGuiKey_Keypad2;
	case VK_NUMPAD3:
		return ImGuiKey_Keypad3;
	case VK_NUMPAD4:
		return ImGuiKey_Keypad4;
	case VK_NUMPAD5:
		return ImGuiKey_Keypad5;
	case VK_NUMPAD6:
		return ImGuiKey_Keypad6;
	case VK_NUMPAD7:
		return ImGuiKey_Keypad7;
	case VK_NUMPAD8:
		return ImGuiKey_Keypad8;
	case VK_NUMPAD9:
		return ImGuiKey_Keypad9;
	case VK_DECIMAL:
		return ImGuiKey_KeypadDecimal;
	case VK_DIVIDE:
		return ImGuiKey_KeypadDivide;
	case VK_MULTIPLY:
		return ImGuiKey_KeypadMultiply;
	case VK_SUBTRACT:
		return ImGuiKey_KeypadSubtract;
	case VK_ADD:
		return ImGuiKey_KeypadAdd;
	case IM_VK_KEYPAD_ENTER:
		return ImGuiKey_KeypadEnter;
	case VK_LSHIFT:
		return ImGuiKey_LeftShift;
	case VK_LCONTROL:
		return ImGuiKey_LeftCtrl;
	case VK_LMENU:
		return ImGuiKey_LeftAlt;
	case VK_LWIN:
		return ImGuiKey_LeftSuper;
	case VK_RSHIFT:
		return ImGuiKey_RightShift;
	case VK_RCONTROL:
		return ImGuiKey_RightCtrl;
	case VK_RMENU:
		return ImGuiKey_RightAlt;
	case VK_RWIN:
		return ImGuiKey_RightSuper;
	case VK_APPS:
		return ImGuiKey_Menu;
	case '0':
		return ImGuiKey_0;
	case '1':
		return ImGuiKey_1;
	case '2':
		return ImGuiKey_2;
	case '3':
		return ImGuiKey_3;
	case '4':
		return ImGuiKey_4;
	case '5':
		return ImGuiKey_5;
	case '6':
		return ImGuiKey_6;
	case '7':
		return ImGuiKey_7;
	case '8':
		return ImGuiKey_8;
	case '9':
		return ImGuiKey_9;
	case 'A':
		return ImGuiKey_A;
	case 'B':
		return ImGuiKey_B;
	case 'C':
		return ImGuiKey_C;
	case 'D':
		return ImGuiKey_D;
	case 'E':
		return ImGuiKey_E;
	case 'F':
		return ImGuiKey_F;
	case 'G':
		return ImGuiKey_G;
	case 'H':
		return ImGuiKey_H;
	case 'I':
		return ImGuiKey_I;
	case 'J':
		return ImGuiKey_J;
	case 'K':
		return ImGuiKey_K;
	case 'L':
		return ImGuiKey_L;
	case 'M':
		return ImGuiKey_M;
	case 'N':
		return ImGuiKey_N;
	case 'O':
		return ImGuiKey_O;
	case 'P':
		return ImGuiKey_P;
	case 'Q':
		return ImGuiKey_Q;
	case 'R':
		return ImGuiKey_R;
	case 'S':
		return ImGuiKey_S;
	case 'T':
		return ImGuiKey_T;
	case 'U':
		return ImGuiKey_U;
	case 'V':
		return ImGuiKey_V;
	case 'W':
		return ImGuiKey_W;
	case 'X':
		return ImGuiKey_X;
	case 'Y':
		return ImGuiKey_Y;
	case 'Z':
		return ImGuiKey_Z;
	case VK_F1:
		return ImGuiKey_F1;
	case VK_F2:
		return ImGuiKey_F2;
	case VK_F3:
		return ImGuiKey_F3;
	case VK_F4:
		return ImGuiKey_F4;
	case VK_F5:
		return ImGuiKey_F5;
	case VK_F6:
		return ImGuiKey_F6;
	case VK_F7:
		return ImGuiKey_F7;
	case VK_F8:
		return ImGuiKey_F8;
	case VK_F9:
		return ImGuiKey_F9;
	case VK_F10:
		return ImGuiKey_F10;
	case VK_F11:
		return ImGuiKey_F11;
	case VK_F12:
		return ImGuiKey_F12;
	default:
		return ImGuiKey_None;
	}
}

class CharEvent : public RE::InputEvent
{
public:
	uint32_t keyCode;  // 18 (ascii code)
};

static enum : std::uint32_t {
	kInvalid = static_cast<std::uint32_t>(-1),
	kKeyboardOffset = 0,
	kMouseOffset = 256,
	kGamepadOffset = 266
};

static inline std::uint32_t GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key)
{
	using Key = RE::BSWin32GamepadDevice::Key;

	std::uint32_t index;
	switch (a_key) {
	case Key::kUp:
		index = 0;
		break;
	case Key::kDown:
		index = 1;
		break;
	case Key::kLeft:
		index = 2;
		break;
	case Key::kRight:
		index = 3;
		break;
	case Key::kStart:
		index = 4;
		break;
	case Key::kBack:
		index = 5;
		break;
	case Key::kLeftThumb:
		index = 6;
		break;
	case Key::kRightThumb:
		index = 7;
		break;
	case Key::kLeftShoulder:
		index = 8;
		break;
	case Key::kRightShoulder:
		index = 9;
		break;
	case Key::kA:
		index = 10;
		break;
	case Key::kB:
		index = 11;
		break;
	case Key::kX:
		index = 12;
		break;
	case Key::kY:
		index = 13;
		break;
	case Key::kLeftTrigger:
		index = 14;
		break;
	case Key::kRightTrigger:
		index = 15;
		break;
	default:
		index = kInvalid;
		break;
	}

	return index != kInvalid ? index + kGamepadOffset : kInvalid;
}


void ProcessEvent(RE::InputEvent** a_event)
{
	if (!a_event)
		return;

	auto& io = ImGui::GetIO();

	for (auto event = *a_event; event; event = event->next) {
		if (event->eventType == RE::INPUT_EVENT_TYPE::kChar) {
			io.AddInputCharacter(static_cast<CharEvent*>(event)->keyCode);
		} else if (event->eventType == RE::INPUT_EVENT_TYPE::kButton) {
			const auto button = static_cast<RE::ButtonEvent*>(event);
			if (!button || (button->IsPressed() && !button->IsDown()))
				continue;

			auto scan_code = button->GetIDCode();

			using DeviceType = RE::INPUT_DEVICE;
			auto input = scan_code;
			switch (button->device.get()) {
			case DeviceType::kMouse:
				input += kMouseOffset;
				break;
			case DeviceType::kKeyboard:
				input += kKeyboardOffset;
				break;
			case DeviceType::kGamepad:
				input = GetGamepadIndex((RE::BSWin32GamepadDevice::Key)input);
				break;
			default:
				continue;
			}
			//ModSettings::submitInput(input);

			uint32_t key = MapVirtualKeyEx(scan_code, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));
			switch (scan_code) {
			case DIK_LEFTARROW:
				key = VK_LEFT;
				break;
			case DIK_RIGHTARROW:
				key = VK_RIGHT;
				break;
			case DIK_UPARROW:
				key = VK_UP;
				break;
			case DIK_DOWNARROW:
				key = VK_DOWN;
				break;
			case DIK_DELETE:
				key = VK_DELETE;
				break;
			case DIK_END:
				key = VK_END;
				break;
			case DIK_HOME:
				key = VK_HOME;
				break;  // pos1
			case DIK_PRIOR:
				key = VK_PRIOR;
				break;  // page up
			case DIK_NEXT:
				key = VK_NEXT;
				break;  // page down
			case DIK_INSERT:
				key = VK_INSERT;
				break;
			case DIK_NUMPAD0:
				key = VK_NUMPAD0;
				break;
			case DIK_NUMPAD1:
				key = VK_NUMPAD1;
				break;
			case DIK_NUMPAD2:
				key = VK_NUMPAD2;
				break;
			case DIK_NUMPAD3:
				key = VK_NUMPAD3;
				break;
			case DIK_NUMPAD4:
				key = VK_NUMPAD4;
				break;
			case DIK_NUMPAD5:
				key = VK_NUMPAD5;
				break;
			case DIK_NUMPAD6:
				key = VK_NUMPAD6;
				break;
			case DIK_NUMPAD7:
				key = VK_NUMPAD7;
				break;
			case DIK_NUMPAD8:
				key = VK_NUMPAD8;
				break;
			case DIK_NUMPAD9:
				key = VK_NUMPAD9;
				break;
			case DIK_DECIMAL:
				key = VK_DECIMAL;
				break;
			case DIK_NUMPADENTER:
				key = IM_VK_KEYPAD_ENTER;
				break;
			case DIK_RMENU:
				key = VK_RMENU;
				break;  // right alt
			case DIK_RCONTROL:
				key = VK_RCONTROL;
				break;  // right control
			case DIK_LWIN:
				key = VK_LWIN;
				break;  // left win
			case DIK_RWIN:
				key = VK_RWIN;
				break;  // right win
			case DIK_APPS:
				key = VK_APPS;
				break;
			default:
				break;
			}

			switch (button->device.get()) {
			case RE::INPUT_DEVICE::kMouse:
				if (scan_code > 7)  // middle scroll
					io.AddMouseWheelEvent(0, button->Value() * (scan_code == 8 ? 1 : -1));
				else {
					if (scan_code > 5)
						scan_code = 5;
					io.AddMouseButtonEvent(scan_code, button->IsPressed());
				}
				break;
			case RE::INPUT_DEVICE::kKeyboard:
				io.AddKeyEvent(ImGui_ImplWin32_VirtualKeyToImGuiKey(key), button->IsPressed());
				//if (button->GetIDCode() == 199) {  // home
				//	if (button->IsDown()) {
				//		Renderer::flip();
				//	}
				//}
				break;
			case RE::INPUT_DEVICE::kGamepad:
				// not implemented yet
				// key = GetGamepadIndex((RE::BSWin32GamepadDevice::Key)key);
				break;
			default:
				continue;
			}
		}
	}
	return;
}



void OnInputEventDispatch::DispatchInputEvent(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent** a_evns)
{
	static RE::InputEvent* dummy[] = { nullptr };
	if (!a_evns) {
		_DispatchInputEvent(a_dispatcher, a_evns);
		return;
	}
	//InputListener::GetSingleton()->ProcessEvent(a_evns);
	if (d3d11hook::active) {
		_DispatchInputEvent(a_dispatcher, dummy);
		return;
	} else {
		_DispatchInputEvent(a_dispatcher, a_evns);
	}
}



	void __cdecl installimgui(void*)
{
	Sleep(5000);
	d3d11hook::Install();

	OnInputEventDispatch ::Install();
}


void __cdecl RefreshAutoUnequipAmmo(void*)
{
	while (true) {
		Sleep(1000);
		if (auto_remove_ammo) {
			if (startflag) {
				RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
				if (player) {
					auto ammo = player->GetCurrentAmmo();
					if (ammo) {
						// 武器信息
						auto weapon = player->GetEquippedObject(true);
						if (weapon && weapon->IsWeapon()) {
							auto item = weapon->As<RE::TESObjectWEAP>();
							if (item->GetWeaponType() == RE::WEAPON_TYPE::kBow || item->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow) {
								continue;
							}
						}

						std::string commandStr = "player.unequipItem ";
						commandStr.append(FormIDToString(ammo->GetFormID()));

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
				}
			}
		}
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("SKSEPlugin_Load"sv);
#ifndef NDEBUG
	MessageBoxA(nullptr, "SKSEPlugin_Load", nullptr, MB_OK);
	Sleep(1000);
#endif

	logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());
	SKSE::Init(a_skse);

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		logger::critical("Could not register MessageHandler"sv);
		return false;
	}
	logger::info("registered listener"sv);

	// 测试
	SKSE::AllocTrampoline(1 << 6);
	hookInstall();

	_beginthread(RefreshGameInfo, 0, NULL);
	_beginthread(RefreshAutoUnequipAmmo, 0, NULL);

	
	_beginthread(installimgui, 0, NULL);


	return true;

	//RE::BSRenderManager::GetSingleton();
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
