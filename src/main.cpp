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

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	settings = "data\\skse\\plugins\\sse-hud.json";
	fontFilePath = "data\\skse\\plugins\\msyh.ttc";
	//Sleep(5000);

	//MessageBox(nullptr, "测试中文", nullptr, MB_OK);
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

		logger::info(FMT_STRING("{} v{}"), "ExampleProject", "ExampleProject");

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "ExampleProject";
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
imgui_api imgui;

static ImFont* font;

static void __stdcall render(int active)
{
	// 当打开菜单时不显示
	if (isOpenCursorMenu && !active) {
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
	imgui.igPushFont(font);

	if (show_player_info_window) {
		imgui.igBegin("人物属性", nullptr, window_flags);

		//imgui.igText("Health:%d", (int)health);
		//imgui.igText("Magicka:%d", (int)kMagicka);
		//imgui.igText("Stamina:%d", (int)kStamina);
		imgui.igBulletText("伤害: %s", playerInfo.DamageStr.c_str());
		//imgui.igBulletText("防御: %s", playerInfo.DamageResist.c_str());
		imgui.igBulletText("防御: %.2f", playerInfo.kDamageResist);
		imgui.igBulletText("毒抗: %.2f", playerInfo.kPoisonResist);
		imgui.igBulletText("火抗: %.2f", playerInfo.kResistFire);
		imgui.igBulletText("雷抗: %.2f", playerInfo.kResistShock);
		imgui.igBulletText("冰抗: %.2f", playerInfo.kResistFrost);
		imgui.igBulletText("法抗: %.2f", playerInfo.kResistMagic);
		imgui.igBulletText("抗疾病: %.2f", playerInfo.kResistDisease);
		//imgui.igText("criticalChance:%d", (int)kCriticalChance);
		//imgui.igText("armor:%d", (int)kDamageResist);
		imgui.igEnd();
	}

	if (show_player_weapon_window) {
		imgui.igBegin("武器", nullptr, window_flags);

		if (leftWeaponInfo.isExist) {
			if (imgui.igTreeNodeStr(leftWeaponInfo.treeId.c_str())) {
				imgui.igBulletText("类型: %s", leftWeaponInfo.formTypeName.c_str());
				imgui.igBulletText("ID: %s", leftWeaponInfo.formID.c_str());
				imgui.igSameLine(0, 0);
				if (imgui.igSmallButton("卸载")) {
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
						imgui.igBulletText("武器类型: %s", leftWeaponInfo.weaponTypeName.c_str());
						imgui.igBulletText("武器伤害: %d", leftWeaponInfo.damage);
						imgui.igBulletText("暴击伤害: %d", leftWeaponInfo.critDamage);
						imgui.igBulletText("价格: %d", leftWeaponInfo.goldValue);
						imgui.igBulletText("属性值: %d$", leftWeaponInfo.value);
						imgui.igBulletText("重量: %.1f", leftWeaponInfo.weight);
						break;
					}
				case RE::FormType::Spell:
					{
						imgui.igBulletText("法术类型: %s", leftWeaponInfo.castingTypeName.c_str());
						imgui.igBulletText("法术类型: %s", leftWeaponInfo.spellTypeName.c_str());
						imgui.igBulletText("花费: %.1f", leftWeaponInfo.cost);
						imgui.igBulletText("施法时间: %.1f", leftWeaponInfo.time);
						break;
					}

				default:
					break;
				}

				//imgui.igBulletText("插槽: %s", item.equipSlotName.c_str());
				imgui.igTreePop();
			}
		}

		if (rightWeaponInfo.isExist) {
			// 不显示双手武器
			if (!rightWeaponInfo.isTwoHand) {
				if (imgui.igTreeNodeStr(rightWeaponInfo.treeId.c_str())) {
					imgui.igBulletText("类型: %s", rightWeaponInfo.formTypeName.c_str());
					imgui.igBulletText("ID: %s", rightWeaponInfo.formID.c_str());
					imgui.igSameLine(0, 0);
					if (imgui.igSmallButton("卸载")) {
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
							imgui.igBulletText("武器类型: %s", rightWeaponInfo.weaponTypeName.c_str());
							imgui.igBulletText("武器伤害: %d", rightWeaponInfo.damage);
							imgui.igBulletText("暴击伤害: %d", rightWeaponInfo.critDamage);
							imgui.igBulletText("价格: %d", rightWeaponInfo.goldValue);
							imgui.igBulletText("属性值: %d$", rightWeaponInfo.value);
							imgui.igBulletText("重量: %.1f", rightWeaponInfo.weight);
							break;
						}
					case RE::FormType::Spell:
						{
							imgui.igBulletText("法术类型: %s", rightWeaponInfo.castingTypeName.c_str());
							imgui.igBulletText("法术类型: %s", rightWeaponInfo.spellTypeName.c_str());
							imgui.igBulletText("花费: %.1f", rightWeaponInfo.cost);
							imgui.igBulletText("施法时间: %.1f", rightWeaponInfo.time);
							break;
						}
					default:
						break;
					}

					imgui.igTreePop();
				}
			}
		}

		if (ammoInfo.isExist) {
			if (imgui.igTreeNodeStr(ammoInfo.treeId.c_str())) {
				imgui.igBulletText("类型: %s", ammoInfo.formTypeName.c_str());
				imgui.igBulletText("ID: %s", ammoInfo.formID.c_str());
				imgui.igSameLine(0, 0);
				if (imgui.igSmallButton("卸载")) {
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
				imgui.igBulletText("伤害: %.1f", ammoInfo.damage);
				imgui.igBulletText("价格: %d", ammoInfo.goldValue);
				imgui.igBulletText("属性值: %d$", ammoInfo.value);
				imgui.igBulletText("重量: %.1f", ammoInfo.weight);

				imgui.igTreePop();
			}
		}

		imgui.igEnd();
	}

	if (show_player_armor_window) {
		imgui.igBegin("装备", nullptr, window_flags);

		int unWornArmosAlertSlots = 0;

		for (int i = 0; i <= 31; i++) {
			auto item = wornArmos[i];

			if (item.isExist) {
				if (item.isSpeacilSlotAlert) {
					// 变黄色
					//imgui.igPushStyleColor
				}

				if (imgui.igTreeNodeStr(item.treeId.c_str())) {
					imgui.igBulletText("类型: %s", item.formTypeName.c_str());
					imgui.igBulletText("装备类型: %s", item.armorTypeName.c_str());
					imgui.igBulletText("ID: %s", item.formID.c_str());
					imgui.igSameLine(0, 0);
					if (imgui.igSmallButton("卸载")) {
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
					imgui.igBulletText("价格: %d$", item.goldValue);
					imgui.igBulletText("属性值: %d", item.value);
					imgui.igBulletText("装备等级: %.2f", item.armorRating);
					imgui.igBulletText("插槽: %s", item.equipSlotName.c_str());
					imgui.igBulletText("重量: %.1f", item.weight);
					imgui.igTreePop();
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
				imgui.igTextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "未装备 | %s", wornArmos[i].equipSlotName.c_str());
			}
		}

		imgui.igEnd();
	}

	if (show_player_debug_window) {
		imgui.igBegin("其他信息", nullptr, window_flags);
		imgui.igBulletText("人物坐标: [%.0f,%.0f,%.0f]", playerInfo.Position.x, playerInfo.Position.y, playerInfo.Position.z);
		//imgui.igSameLine(0, 0);
		imgui.igBulletText("人物视角: [%.2f,%.2f]", playerInfo.Angle.x, playerInfo.Angle.z);
		imgui.igEnd();
	}


	if (active) {
		static bool show_demo_window = false;
		static bool show_another_window = false;

		// 瀹樻柟Demo
		if (show_demo_window)
			imgui.igShowDemoWindow(&show_demo_window);

		{
			imgui.igBegin("设置", nullptr, 0);



			imgui.igCheckbox("是否显示-人物属性", &show_player_info_window);
			imgui.igCheckbox("是否显示-武器信息", &show_player_weapon_window);
			imgui.igCheckbox("是否显示-装备信息", &show_player_armor_window);
			imgui.igCheckbox("是否显示-其他信息", &show_player_debug_window);
			imgui.igCheckbox("是否自动卸除箭袋", &auto_remove_ammo);

			if (imgui.igCollapsingHeader("窗口设置", 0)) {
				imgui.igBeginGroup();

				static int style_idx = imgui_style_index;
				if (imgui.igComboStr("主题", &style_idx, "Dark\0Light\0Classic\0", -1)) {
					switch (style_idx) {
					case 0:
						imgui.igStyleColorsDark(imgui.igGetStyle());
						imgui_style_index = 0;
						break;
					case 1:
						imgui.igStyleColorsLight(imgui.igGetStyle());
						imgui_style_index = 1;
						break;
					case 2:
						imgui.igStyleColorsClassic(imgui.igGetStyle());
						imgui_style_index = 2;
						break;
					}
				}

				//imgui.igShowStyleSelector("style");
				//ImGui::TableNextColumn();
				imgui.igCheckbox("不显示标题", &no_titlebar);
				//ImGui::TableNextColumn();
				imgui.igCheckbox("不允许缩放", &no_resize);
				//ImGui::TableNextColumn();
				imgui.igCheckbox("不允许折叠", &no_collapse);
				//ImGui::TableNextColumn();
				imgui.igCheckbox("不显示背景", &no_background);
				imgui.igCheckbox("是否窗口自适应", &auto_resize);
				//ImGui::EndTable();
				imgui.igEndGroup();
			}
			if (imgui.igSmallButton("保存配置")) {
				save_settings();
			}

			//imgui.igCheckbox("官方Demo", &show_demo_window);

			/*	static float f = 0.0f;
		static int counter = 0;*/
			/*imgui.igText("Health:%d", (int)health);
		imgui.igText("Magicka:%d", (int)kMagicka);
		imgui.igText("Stamina:%d", (int)kStamina);*/

			//imgui.igText("angle: [%.2f,%.2f,%.2f]", x, y, z);
			/*imgui.igText("This is some useful text.");
		imgui.igCheckbox("Another Window", &show_another_window);*/
			/*	imgui.igSliderFloat("float", &f, 0.0f, 1.0f, "%.3f", 1.f);
		if (imgui.igButton("Button", ImVec2{}))
			counter++;
		imgui.igSameLine(0.f, -1.f);
		imgui.igText("counter = %d", counter);*/
			imgui.igText("Application average %.3f ms/frame (%.1f FPS)",
				1000.0f / imgui.igGetIO()->Framerate, imgui.igGetIO()->Framerate);
			imgui.igEnd();
		}

		/*if (show_another_window) {
		imgui.igBegin("Another Window", &show_another_window, 0);
		imgui.igText("Hello from another window!");
		if (imgui.igButton("Close Me", ImVec2{}))
			show_another_window = false;

		imgui.igCheckbox("Demo", &show_demo_window);
		imgui.igEnd();
	}*/
	}

	imgui.igPopFont();
}

static void ImguiMessageHandler(SKSE::MessagingInterface::Message* m)
{
	if (m->type != SSEIMGUI_API_VERSION)
		return;

	sseimgui_api* sseimgui = reinterpret_cast<sseimgui_api*>(m->data);
	imgui = sseimgui->make_imgui_api();

	//const char* font_path = "D:\\msyh.ttc";
	if (std::filesystem::exists(fontFilePath)) {
		auto io = imgui.igGetIO();
		//auto fonts = imgui.ImFontAtlas_ImFontAtlas();
		font = imgui.ImFontAtlas_AddFontFromFileTTF(io->Fonts, fontFilePath.c_str(), 20.0f, NULL, imgui.ImFontAtlas_GetGlyphRangesChineseFull(io->Fonts));
		imgui.ImFontAtlas_Build(io->Fonts);
	}

	switch (imgui_style_index) {
	case 0:
		imgui.igStyleColorsDark(imgui.igGetStyle());
		break;
	case 1:
		imgui.igStyleColorsLight(imgui.igGetStyle());
		break;
	case 2:
		imgui.igStyleColorsClassic(imgui.igGetStyle());
		break;
	}

	sseimgui->render_listener(&render, 0);
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostLoad:
		{
			logger::info("PostLoad"sv);
			auto messaging = SKSE::GetMessagingInterface();
			if (!messaging->RegisterListener("SSEIMGUI", ImguiMessageHandler)) {
			}
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

void __cdecl RefreshGameInfo(void*)
{
	//auto game_hwnd = FindWindowA(NULL, "Skyrim Special Edition");
	Sleep(10000);
	MenuOpenCloseEvent::Register();  //Register Bethesda Menu Event

	// 标记装备槽是否主要
	wornArmos[1].isMainSlotAlert = wornArmos[2].isMainSlotAlert = wornArmos[3].isMainSlotAlert = wornArmos[5].isMainSlotAlert = wornArmos[6].isMainSlotAlert = wornArmos[7].isMainSlotAlert = true;
	wornArmos[1].isSpeacilSlotAlert = wornArmos[2].isSpeacilSlotAlert = wornArmos[3].isSpeacilSlotAlert = wornArmos[5].isSpeacilSlotAlert = wornArmos[6].isSpeacilSlotAlert = wornArmos[7].isSpeacilSlotAlert = wornArmos[9].isSpeacilSlotAlert = false;
	for (int i = 0; i <= 31; i++) {
		wornArmos[i].equipSlotName = GetEquipSlotName(i);
	}

	while (true) {
		Sleep(300);
		if (startflag) {

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				playerInfo.Angle = player->GetAngle();
				playerInfo.Position = player->GetPosition();
				playerInfo.name = player->GetName();

				//__try {
				auto playerFormEditorID = player->GetFormEditorID();
				auto playerFormID = player->GetFormID();
				auto playerGoldValue = player->GetGoldValue();
				auto playerGoldAmount = player->GetGoldAmount();
				auto playerDisplayFullName = player->GetDisplayFullName();

				auto playerLevel = player->GetLevel();
				auto playerRace = player->GetRace();
				if (playerRace) {
					//playerRaceName = player->GetRace()->GetFullName();
				}

				auto playerAttackingWeapon = player->GetAttackingWeapon();
				auto playerEquippedEntryDataLeft = player->GetEquippedEntryData(true);
				auto playerEquippedEntryDataRight = player->GetEquippedEntryData(false);
				auto playerEquippedObjectLeft = player->GetEquippedObject(true);
				auto playerEquippedObjectRight = player->GetEquippedObject(false);

				//auto id = player->GetCrimeGoldValue();
				auto factionOwner = player->GetFactionOwner();
				if (factionOwner) {
					auto factionOwnerName = factionOwner->GetFullName();
				}
				// 轻甲等级
				//lightArmor = player->GetActorValue(RE::ActorValue::kLightArmor);
				//// 重甲等级
				//heavyArmor = player->GetActorValue(RE::ActorValue::kHeavyArmor);

				//// 生命
				//health = player->GetActorValue(RE::ActorValue::kHealth);
				//// 魔法
				//kMagicka = player->GetActorValue(RE::ActorValue::kMagicka);
				//// 体力
				//kStamina = player->GetActorValue(RE::ActorValue::kStamina);
				// 生命恢复速率
				auto kHealRate = player->GetActorValue(RE::ActorValue::kHealRate);
				// 战斗中生命恢复速率
				auto kCombatHealthRegenMultiply = player->GetActorValue(RE::ActorValue::kCombatHealthRegenMultiply);
				// 魔法恢复速率
				auto kMagickaRate = player->GetActorValue(RE::ActorValue::kMagickaRate);
				// 体力恢复速率
				auto KStaminaRate = player->GetActorValue(RE::ActorValue::KStaminaRate);

				// 武器伤害
				//kMeleeDamage = player->GetActorValue(RE::ActorValue::kMeleeDamage);
				// 空手伤害
				//kUnarmedDamage = player->GetActorValue(RE::ActorValue::kUnarmedDamage);

				// 弓箭伤害
				playerInfo.ArrowDamage = PlayerDataProvider::getArrowDamage(player);
				// 右手伤害
				playerInfo.DamageRight = PlayerDataProvider::getDamage(player, false);
				// 左手伤害
				playerInfo.DamageLeft = PlayerDataProvider::getDamage(player, true);

				// 显示伤害
				std::string tmp = "";
				if (playerInfo.DamageLeft.empty()) {
					if (playerInfo.DamageRight.empty()) {
						tmp = "0";
					} else {
						tmp = playerInfo.DamageRight;
					}
				} else {
					if (playerInfo.DamageRight.empty()) {
						tmp = playerInfo.DamageLeft;
					} else {
						tmp.append(playerInfo.DamageLeft);
						tmp.append(" | ");
						tmp.append(playerInfo.DamageRight);
					}
				}
				if (!playerInfo.ArrowDamage.empty()) {
					tmp.append("   - 箭:");
					tmp.append(playerInfo.ArrowDamage);
				}
				playerInfo.DamageStr = tmp;

				// 护甲
				playerInfo.kDamageResist = player->GetActorValue(RE::ActorValue::kDamageResist);
				//playerInfo.DamageResist = PlayerDataProvider::getDamageResistance(player, -1, "");

				// 暴击几率
				//kCriticalChance = player->GetActorValue(RE::ActorValue::kCriticalChance);
				// 毒抗
				playerInfo.kPoisonResist = player->GetActorValue(RE::ActorValue::kPoisonResist);
				// 火炕
				playerInfo.kResistFire = player->GetActorValue(RE::ActorValue::kResistFire);
				playerInfo.kResistShock = player->GetActorValue(RE::ActorValue::kResistShock);
				playerInfo.kResistMagic = player->GetActorValue(RE::ActorValue::kResistMagic);
				playerInfo.kResistFrost = player->GetActorValue(RE::ActorValue::kResistFrost);
				playerInfo.kResistDisease = player->GetActorValue(RE::ActorValue::kResistDisease);

				// 装备信息
				const auto inv = player->GetInventory([](RE::TESBoundObject& a_object) {
					return a_object.IsArmor();
				});

				int removeIndexs = 0;  // 最后需要移除的元素索引
				for (const auto& [item, invData] : inv) {
					const auto& [count, entry] = invData;
					if (count > 0 && entry->IsWorn()) {
						const auto armor = item->As<RE::TESObjectARMO>();
						// 插槽名称(所有)
						//std::string slotNames = "";
						int parts = (int)armor->GetSlotMask();

						removeIndexs += parts;

						//int slotIndex = 0;
						for (int i = 0; i <= 31; i++) {
							int mask = 1 << i;
							if ((parts & mask) == mask) {
								/*if (slotIndex == 0) {
									slotIndex = i;
									removeIndexs += parts;
									slotNames.append(GetEquipSlotName(i));
									slotNames.append(" ");
								}*/
								wornArmos[i].isExist = true;
								wornArmos[i].name = armor->GetName();
								wornArmos[i].formID = FormIDToString(armor->GetFormID());
								wornArmos[i].formTypeName = GetFormTypeName(armor->formType.underlying());
								wornArmos[i].goldValue = armor->GetGoldValue();
								wornArmos[i].value = armor->value;
								wornArmos[i].armorRating = armor->GetArmorRating();
								wornArmos[i].armorTypeName = GetArmorTypeName(armor->GetArmorType());
								//wornArmos[i].equipSlotName = slotNames;
								//wornArmos[i].equipSlotName = GetEquipSlotName(i);
								wornArmos[i].weight = armor->weight;
								std::string tmp = wornArmos[i].name;
								tmp.append(" - ");
								tmp.append(wornArmos[i].armorTypeName);
								tmp.append(" | ");
								tmp.append(wornArmos[i].equipSlotName);
								wornArmos[i].treeId = tmp;
							}
						}
						//if (slotIndex > 0) {
						//}
					}
				}
				// 删除未占用插槽的元素
				for (int i = 0; i <= 31; i++) {
					int mask = 1 << i;
					if ((removeIndexs & mask) != mask) {
						wornArmos[i].isExist = false;
					}
				}

				// 武器信息
				auto leftWeapon = player->GetEquippedObject(true);
				if (leftWeapon) {
					leftWeaponInfo.formType = leftWeapon->GetFormType();
					leftWeaponInfo.formTypeName = GetFormTypeName(leftWeapon->formType.underlying());
					leftWeaponInfo.name = leftWeapon->GetName();
					leftWeaponInfo.formID = FormIDToString(leftWeapon->GetFormID());
					if (leftWeapon->IsWeapon()) {
						auto item = leftWeapon->As<RE::TESObjectWEAP>();
						leftWeaponInfo.isExist = true;
						leftWeaponInfo.goldValue = item->GetGoldValue();
						leftWeaponInfo.value = item->value;
						leftWeaponInfo.weaponTypeName = GetWeaponAnimationTypeName(item->GetWeaponType());
						leftWeaponInfo.damage = item->GetAttackDamage();
						leftWeaponInfo.critDamage = item->GetCritDamage();
						leftWeaponInfo.weight = item->weight;
						std::string tmp = std::string(leftWeaponInfo.name);
						tmp.append(" - ");
						tmp.append(leftWeaponInfo.weaponTypeName);
						leftWeaponInfo.treeId = tmp;
					} else if (leftWeapon->Is(RE::FormType::Spell)) {
						auto item = leftWeapon->As<RE::MagicItem>();
						leftWeaponInfo.isExist = true;
						leftWeaponInfo.goldValue = item->GetGoldValue();
						leftWeaponInfo.castingTypeName = GetCastingTypeName(item->GetCastingType());
						leftWeaponInfo.spellTypeName = GetSpellTypeName(item->GetSpellType());
						leftWeaponInfo.cost = item->CalculateMagickaCost(player);
						leftWeaponInfo.time = item->GetChargeTime();
						auto tmp = std::string(leftWeaponInfo.name);
						tmp.append(" - ");
						tmp.append(leftWeaponInfo.castingTypeName);
						leftWeaponInfo.treeId = tmp;
					} else {
						leftWeaponInfo.isExist = false;
					}
				} else {
					leftWeaponInfo.isExist = false;
				}

				auto rightWeapon = player->GetEquippedObject(false);
				if (rightWeapon) {
					rightWeaponInfo.formType = rightWeapon->GetFormType();
					rightWeaponInfo.formTypeName = GetFormTypeName(rightWeapon->formType.underlying());
					rightWeaponInfo.name = rightWeapon->GetName();
					rightWeaponInfo.formID = FormIDToString(rightWeapon->GetFormID());
					if (rightWeapon->IsWeapon()) {
						auto item = rightWeapon->As<RE::TESObjectWEAP>();
						rightWeaponInfo.isExist = true;
						rightWeaponInfo.goldValue = item->GetGoldValue();
						rightWeaponInfo.value = item->value;
						rightWeaponInfo.weaponTypeName = GetWeaponAnimationTypeName(item->GetWeaponType());
						rightWeaponInfo.damage = item->GetAttackDamage();
						rightWeaponInfo.critDamage = item->GetCritDamage();
						rightWeaponInfo.weight = item->weight;

						if (item->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandSword ||
							item->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandAxe ||
							item->GetWeaponType() == RE::WEAPON_TYPE::kBow ||
							item->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow) {
							rightWeaponInfo.isTwoHand = true;
						} else {
							rightWeaponInfo.isTwoHand = false;
						}

						auto tmp = std::string(rightWeaponInfo.name);
						tmp.append(" - ");
						tmp.append(rightWeaponInfo.weaponTypeName);
						rightWeaponInfo.treeId = tmp;
					} else if (rightWeapon->Is(RE::FormType::Spell)) {
						auto item = rightWeapon->As<RE::MagicItem>();

						rightWeaponInfo.isExist = true;
						rightWeaponInfo.goldValue = item->GetGoldValue();
						rightWeaponInfo.castingTypeName = GetCastingTypeName(item->GetCastingType());
						rightWeaponInfo.spellTypeName = GetSpellTypeName(item->GetSpellType());
						rightWeaponInfo.cost = item->CalculateMagickaCost(player);
						rightWeaponInfo.time = item->GetChargeTime();
						rightWeaponInfo.isTwoHand = item->IsTwoHanded();

						auto tmp = std::string(rightWeaponInfo.name);
						tmp.append(" - ");
						tmp.append(rightWeaponInfo.castingTypeName);
						rightWeaponInfo.treeId = tmp;
					} else {
						rightWeaponInfo.isExist = false;
					}

				} else {
					rightWeaponInfo.isExist = false;
				}

				// 弹药
				auto ammo = player->GetCurrentAmmo();
				if (ammo) {
					const auto item = ammo->As<RE::TESAmmo>();
					ammoInfo.isExist = true;
					ammoInfo.formType = item->GetFormType();
					ammoInfo.formTypeName = GetFormTypeName(item->formType.underlying());
					ammoInfo.name = item->GetName();
					ammoInfo.formID = FormIDToString(item->GetFormID());
					ammoInfo.goldValue = item->GetGoldValue();
					ammoInfo.value = item->value;
					ammoInfo.damage = item->data.damage;
					ammoInfo.weight = item->weight;
					auto tmp = std::string(ammoInfo.name);
					/*tmp.append(" - ");
					tmp.append(rightWeaponInfo.castingTypeName);*/
					ammoInfo.treeId = tmp;
				} else {
					ammoInfo.isExist = false;
				}

				/*	} __except (SehFilter(GetExceptionCode())) {
					logger::info("catch (...)");
				}*/
			} else {
				logger::info("no player"sv);
			}
		}
	}
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
	//MessageBoxA(nullptr, "SKSEPlugin_Load", nullptr, MB_OK);
	//Sleep(10000);
	logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());
	SKSE::Init(a_skse);

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}


	_beginthread(RefreshGameInfo, 0, NULL);
	_beginthread(RefreshAutoUnequipAmmo, 0, NULL);
	return true;
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

							  } },
			{ "windowSetting", {

								   { "playerInfo", {
													   { "isShow", show_player_info_window },
												   } },

								   { "ArmorInfo", {
													  { "isShow", show_player_armor_window },
												  } },

								   { "WeaponInfo", {
													   { "isShow", show_player_weapon_window },
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
