#include "setting.h"
#include <fstream>
#include <iostream>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/npc.h>
#include <memory/player.h>
#include <memory/stat.h>
#include <menu/menu.h>
#include <menu/menu_npc.h>
#include <menu/menu_track.h>
#include <menu/theme.h>
#include <nlohmann/json.hpp>
#include <memory/sexlab.h>
#include <setting/i18n.h>

namespace setting
{
	std::filesystem::path settings_path = "data\\skse\\plugins\\sse-hud.json";
	std::filesystem::path settings_path_gallery = "";
	std::filesystem::path settings_path_lotd_item_list = "data\\skse\\plugins\\ItemFinderPlus\\lotd\\ItemFinderPlus-LotdItemList.json";
	std::filesystem::path settings_path_lotd_item_display_list = "data\\skse\\plugins\\ItemFinderPlus\\lotd\\ItemFinderPlus-LotdItemDisplayList.json";
	std::filesystem::path settings_path_lotd_item_display_list_modnames = "data\\skse\\plugins\\ItemFinderPlus\\lotd\\ItemFinderPlus-LotdItemList-ModNames.json";
	//std::vector<GalleryData> galleryList;
	std::vector<LotdItemList> lotdItemLists;
	std::vector<LotdItemDisplayCont> lotdItemDisplayLists;
	std::vector<std::uint32_t> displayConts;

	std::unordered_map<std::string, std::string> LotdItemListModNames;

	std::string getLotdItemListModName(const std::string& key)
	{
		if (LotdItemListModNames.find(key) != LotdItemListModNames.end()) {
			return LotdItemListModNames[key];
		} else {
			return key;
		}
	}

	bool load_settings_lotd()
	{
		displayConts.clear();
		displayConts.push_back(0x00126087);
		displayConts.push_back(0x002ACDD9);
		displayConts.push_back(0x006C22C2);
		displayConts.push_back(0x002F8E89);
		//displayConts.push_back(0x00000911);

		try {
			std::ifstream i(settings_path_lotd_item_list);
			nlohmann::json json;
			i >> json;

			for (const auto& obj : json) {
				if (obj.contains("listEditorId")) {
					LotdItemList data;
					data.listEditorId = obj["listEditorId"];

					if (obj.contains("roomName")) {
						data.roomName = obj["roomName"];
					}

					if (obj.contains("modName")) {
						data.modName = obj["modName"];
					}

					if (obj.contains("listFormId")) {
						std::string formidstr = obj["listFormId"];
						data.listFormId = std::stoi(formidstr, 0, 16);
					}

					lotdItemLists.push_back(data);
				}
			}

		} catch (std::exception const& ex) {
			logger::error(ex.what());
			return false;
		}

		try {
			std::ifstream i(settings_path_lotd_item_display_list_modnames);
			nlohmann::json json;
			i >> json;

			for (const auto& obj : json) {
				if (obj.contains("modName") && obj.contains("name")) {
					std::string modName = obj["modName"];
					std::string name = obj["name"];
					LotdItemListModNames.insert(std::pair(modName, name));
				}
			}

		} catch (std::exception const& ex) {
			logger::error(ex.what());
			return false;
		}

		return true;
	}

	bool load_settings_lotd_display()
	{
		try {
			std::ifstream i(settings_path_lotd_item_display_list);
			nlohmann::json json;
			i >> json;

			for (const auto& obj : json) {
				if (obj.contains("contId")) {
					LotdItemDisplayCont data;
					std::string formidstr = obj["contId"];
					data.contId = std::stoi(formidstr, 0, 16);

					if (obj.contains("modName")) {
						data.modName = obj["modName"];
					}

					lotdItemDisplayLists.push_back(data);
				}
			}

		} catch (std::exception const& ex) {
			logger::error(ex.what());
			return false;
		}
		return true;
	}

	bool load_settings()
	{
		try {
			std::ifstream i(settings_path);
			nlohmann::json json;
			i >> json;

			if (json.contains("imguiSetting")) {
				auto const& j = json["imguiSetting"];
				if (j.contains("no_titlebar")) {
					menu::no_titlebar = j["no_titlebar"].get<bool>();
					menu::no_collapse = j["no_collapse"].get<bool>();
					menu::no_background = j["no_background"].get<bool>();
					menu::no_resize = j["no_resize"].get<bool>();
					menu::imgui_style_index = j["imgui_style_index"].get<int>();
				}
				if (j.contains("auto_resize")) {
					menu::auto_resize = j["auto_resize"].get<bool>();
				}
				if (j.contains("window_border")) {
					menu::window_border = j["window_border"].get<bool>();
				}
				if (j.contains("frame_border")) {
					menu::frame_border = j["frame_border"].get<bool>();
				}
				if (j.contains("bullet_text")) {
					menu::bullet_text = j["bullet_text"].get<bool>();
				}
				if (j.contains("hotkey")) {
					menu::hotkey = j["hotkey"].get<int>();
				}
				if (j.contains("hotkey2")) {
					menu::hotkey2 = j["hotkey2"].get<int>();
				}
				if (j.contains("hotkey3")) {
					menu::hotkey3 = j["hotkey3"].get<int>();
				}
				if (j.contains("hotkeySetting")) {
					menu::hotkeySetting = j["hotkeySetting"].get<int>();
				}
				if (j.contains("hotkeyTrack")) {
					menu::hotkeyTrack = j["hotkeyTrack"].get<int>();
				}
				if (j.contains("hotkeyItemFinder")) {
					menu::hotkeyItemFinder = j["hotkeyItemFinder"].get<int>();
				}
				if (j.contains("hotkeySettingModifier")) {
					menu::hotkeySettingModifier = j["hotkeySettingModifier"].get<int>();
				}
				if (j.contains("hotkeyItemFinderModifier")) {
					menu::hotkeyItemFinderModifier = j["hotkeyItemFinderModifier"].get<int>();
				}
				if (j.contains("hotkeyTrackModifier")) {
					menu::hotkeyTrackModifier = j["hotkeyTrackModifier"].get<int>();
				}

				if (j.contains("languageName")) {
					std::string languageName = j["languageName"];
					i18n::languageName = languageName;
				}
				
				if (j.contains("font_scale")) {
					menu::font_scale = j["font_scale"].get<float>();
				}

				if (j.contains("no_background_items")) {
					menu::no_background_items = j["no_background_items"].get<bool>();
				}
				if (j.contains("imgui_font_index")) {
					menu::imgui_font_index = j["imgui_font_index"].get<int>();
				}

				if (j.contains("grabRounding")) {
					menu::grabRounding = j["grabRounding"].get<float>();
					menu::frameRounding = j["frameRounding"].get<float>();
					menu::windowRounding = j["windowRounding"].get<float>();
				}
			}

			if (json.contains("gameSetting")) {
				auto const& j = json["gameSetting"];
				if (j.contains("auto_remove_ammo")) {
					menu::auto_remove_ammo = j["auto_remove_ammo"].get<bool>();
				}
				if (j.contains("remember_last_ammo")) {
					menu::remember_last_ammo = j["remember_last_ammo"].get<bool>();
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

					if (j2.contains("show_player_gold_window")) {
						show_player_gold_window = j2["show_player_gold_window"].get<bool>();
					}

					if (j2.contains("show_player_carryweight_window")) {
						show_player_carryweight_window = j2["show_player_carryweight_window"].get<bool>();
					}
					if (j2.contains("show_player_xp_window")) {
						show_player_xp_window = j2["show_player_xp_window"].get<bool>();
					}
					if (j2.contains("show_player_effects_window")) {
						show_player_effects_window = j2["show_player_effects_window"].get<bool>();
					}
					if (j2.contains("show_player_effects_ignore_permanent")) {
						show_player_effects_ignore_permanent = j2["show_player_effects_ignore_permanent"].get<bool>();
					}
					if (j2.contains("show_player_effects_negative")) {
						show_player_effects_negative = j2["show_player_effects_negative"].get<bool>();
					}
					if (j2.contains("show_player_effects_listignore")) {
						show_player_effects_listignore = j2["show_player_effects_listignore"].get<bool>();
					}
					if (j2.contains("colorProgressEffect1X")) {
						colorProgressEffect1.x = j2["colorProgressEffect1X"].get<float>();
						colorProgressEffect1.y = j2["colorProgressEffect1Y"].get<float>();
						colorProgressEffect1.z = j2["colorProgressEffect1Z"].get<float>();
						colorProgressEffect1.w = j2["colorProgressEffect1W"].get<float>();
						colorProgressEffect3.x = j2["colorProgressEffect3X"].get<float>();
						colorProgressEffect3.y = j2["colorProgressEffect3Y"].get<float>();
						colorProgressEffect3.z = j2["colorProgressEffect3Z"].get<float>();
						colorProgressEffect3.w = j2["colorProgressEffect3W"].get<float>();
					}

					if (j2.contains("show_player_effects_ignore_spell")) {
						show_player_effects_ignore_spell = j2["show_player_effects_ignore_spell"].get<bool>();
					}
					if (j2.contains("show_player_effects_process")) {
						show_player_effects_process = j2["show_player_effects_process"].get<bool>();
					}
					if (j2.contains("isShowPlayOSLArousal")) {
						sexlab::isShowPlayOSLArousal = j2["isShowPlayOSLArousal"].get<bool>();
					}
				}
				if (j.contains("DebugInfo")) {
					auto const& j2 = j["DebugInfo"];
					menu::show_player_debug_window = j2["isShow"].get<bool>();
				}
				if (j.contains("playerModInfo")) {
					auto const& j2 = j["playerModInfo"];
					show_player_mod_window = j2["isShow"].get<bool>();
				}

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
						menu::show_npc = j2["show_npc"].get<bool>();
					}
					if (j2.contains("show_enemy")) {
						menu::show_enemy = j2["show_enemy"].get<bool>();
					}
					if (j2.contains("show_teammate")) {
						menu::show_teammate = j2["show_teammate"].get<bool>();
					}
					if (j2.contains("show_horse")) {
						menu::show_horse = j2["show_horse"].get<bool>();
					}

					if (j2.contains("show_npc_window_ignore")) {
						show_npc_window_ignore = j2["show_npc_window_ignore"].get<bool>();
					}
					if (j2.contains("show_npc_window_process")) {
						show_npc_window_process = j2["show_npc_window_process"].get<bool>();
					}
					if (j2.contains("show_npc_window_process_combat")) {
						show_npc_window_process_combat = j2["show_npc_window_process_combat"].get<bool>();
					}
					if (j2.contains("show_npc_window_dis_meter")) {
						show_npc_window_dis_meter = j2["show_npc_window_dis_meter"].get<int>();
					}

					if (j2.contains("colorProgressNpc1X")) {
						colorProgressNpc1.x = j2["colorProgressNpc1X"].get<float>();
						colorProgressNpc1.y = j2["colorProgressNpc1Y"].get<float>();
						colorProgressNpc1.z = j2["colorProgressNpc1Z"].get<float>();
						colorProgressNpc1.w = j2["colorProgressNpc1W"].get<float>();
						colorProgressNpc2.x = j2["colorProgressNpc2X"].get<float>();
						colorProgressNpc2.y = j2["colorProgressNpc2Y"].get<float>();
						colorProgressNpc2.z = j2["colorProgressNpc2Z"].get<float>();
						colorProgressNpc2.w = j2["colorProgressNpc2W"].get<float>();
						colorProgressNpc3.x = j2["colorProgressNpc3X"].get<float>();
						colorProgressNpc3.y = j2["colorProgressNpc3Y"].get<float>();
						colorProgressNpc3.z = j2["colorProgressNpc3Z"].get<float>();
						colorProgressNpc3.w = j2["colorProgressNpc3W"].get<float>();
					}
					if (j2.contains("show_npc_window_split")) {
						menu::show_npc_window_split = j2["show_npc_window_split"].get<bool>();
					}
				}
				if (j.contains("ItemMenuInfo")) {
					auto const& j2 = j["ItemMenuInfo"];

					if (j2.contains("show_inv_window_height")) {
						show_inv_window_height = j2["show_inv_window_height"].get<int>();
					}
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
					if (j2.contains("show_items_window_auto_weap")) {
						show_items_window_auto_weap = j2["show_items_window_auto_weap"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_armo")) {
						show_items_window_auto_armo = j2["show_items_window_auto_armo"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_ston")) {
						show_items_window_auto_ston = j2["show_items_window_auto_ston"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_anvi")) {
						show_items_window_auto_anvi = j2["show_items_window_auto_anvi"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_ingr")) {
						show_items_window_auto_achr_ingr = j2["show_items_window_auto_achr_ingr"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_anhd")) {
						show_items_window_auto_anhd = j2["show_items_window_auto_anhd"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_anpa")) {
						show_items_window_auto_anpa = j2["show_items_window_auto_anpa"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_tool")) {
						show_items_window_auto_tool = j2["show_items_window_auto_tool"].get<bool>();
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
					if (j2.contains("show_items_window_auto_achr_weap")) {
						show_items_window_auto_achr_weap = j2["show_items_window_auto_achr_weap"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_armo")) {
						show_items_window_auto_achr_armo = j2["show_items_window_auto_achr_armo"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_anvi")) {
						show_items_window_auto_achr_anvi = j2["show_items_window_auto_achr_anvi"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_lock")) {
						show_items_window_auto_achr_lock = j2["show_items_window_auto_achr_lock"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_ston")) {
						show_items_window_auto_achr_ston = j2["show_items_window_auto_achr_ston"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_anhd")) {
						show_items_window_auto_achr_anhd = j2["show_items_window_auto_achr_anhd"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_anpa")) {
						show_items_window_auto_achr_anpa = j2["show_items_window_auto_achr_anpa"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_achr_tool")) {
						show_items_window_auto_achr_tool = j2["show_items_window_auto_achr_tool"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_cont")) {
						show_items_window_auto_cont = j2["show_items_window_auto_cont"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_cont_ingr")) {
						show_items_window_auto_cont_ingr = j2["show_items_window_auto_cont_ingr"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_cont_food")) {
						show_items_window_auto_cont_food = j2["show_items_window_auto_cont_food"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_alch")) {
						show_items_window_auto_cont_alch = j2["show_items_window_auto_cont_alch"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_ammo")) {
						show_items_window_auto_cont_ammo = j2["show_items_window_auto_cont_ammo"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_scrl")) {
						show_items_window_auto_cont_scrl = j2["show_items_window_auto_cont_scrl"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_keym")) {
						show_items_window_auto_cont_keym = j2["show_items_window_auto_cont_keym"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_misc")) {
						show_items_window_auto_cont_misc = j2["show_items_window_auto_cont_misc"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_gold")) {
						show_items_window_auto_cont_gold = j2["show_items_window_auto_cont_gold"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_sgem")) {
						show_items_window_auto_cont_sgem = j2["show_items_window_auto_cont_sgem"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_notification")) {
						show_items_window_auto_notification = j2["show_items_window_auto_notification"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_conttype")) {
						show_items_window_auto_conttype = j2["show_items_window_auto_conttype"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_cont_weap")) {
						show_items_window_auto_cont_weap = j2["show_items_window_auto_cont_weap"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_armo")) {
						show_items_window_auto_cont_armo = j2["show_items_window_auto_cont_armo"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_lock")) {
						show_items_window_auto_cont_lock = j2["show_items_window_auto_cont_lock"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_ston")) {
						show_items_window_auto_cont_ston = j2["show_items_window_auto_cont_ston"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_anvi")) {
						show_items_window_auto_cont_anvi = j2["show_items_window_auto_cont_anvi"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_anhd")) {
						show_items_window_auto_cont_anhd = j2["show_items_window_auto_cont_anhd"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_anpa")) {
						show_items_window_auto_cont_anpa = j2["show_items_window_auto_cont_anpa"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_cont_tool")) {
						show_items_window_auto_cont_tool = j2["show_items_window_auto_cont_tool"].get<bool>();
					}

					if (j2.contains("show_items_window_auto_armo_price")) {
						show_items_window_auto_armo_price = j2["show_items_window_auto_armo_price"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_weap_price")) {
						show_items_window_auto_weap_price = j2["show_items_window_auto_weap_price"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_weap_priceweight")) {
						show_items_window_auto_weap_priceweight = j2["show_items_window_auto_weap_priceweight"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_armo_priceweight")) {
						show_items_window_auto_armo_priceweight = j2["show_items_window_auto_armo_priceweight"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_weap_enchant")) {
						show_items_window_auto_weap_enchant = j2["show_items_window_auto_weap_enchant"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_armo_enchant")) {
						show_items_window_auto_armo_enchant = j2["show_items_window_auto_armo_enchant"].get<bool>();
					}
					if (j2.contains("show_items_window_auto_weap_price_value")) {
						show_items_window_auto_weap_price_value = j2["show_items_window_auto_weap_price_value"].get<int>();
					}
					if (j2.contains("show_items_window_auto_armo_price_value")) {
						show_items_window_auto_armo_price_value = j2["show_items_window_auto_armo_price_value"].get<int>();
					}
					if (j2.contains("show_items_window_auto_weap_priceweight_value")) {
						show_items_window_auto_weap_priceweight_value = j2["show_items_window_auto_weap_priceweight_value"].get<int>();
					}
					if (j2.contains("show_items_window_auto_armo_priceweight_value")) {
						show_items_window_auto_armo_priceweight_value = j2["show_items_window_auto_armo_priceweight_value"].get<int>();
					}

					if (j2.contains("show_item_window_track_icon_scale")) {
						show_item_window_track_icon_scale = j2["show_item_window_track_icon_scale"].get<int>();
					}

					if (j2.contains("showlocationExCount")) {
						stats::showlocationExCount = j2["showlocationExCount"].get<bool>();
					}
					if (j2.contains("showlocationNirnRootCount")) {
						stats::showlocationNirnRootCount = j2["showlocationNirnRootCount"].get<bool>();
					}
					if (j2.contains("showlocationNirnRootRedCount")) {
						stats::showlocationNirnRootRedCount = j2["showlocationNirnRootRedCount"].get<bool>();
					}
					if (j2.contains("showlocationOreCount")) {
						stats::showlocationOreCount = j2["showlocationOreCount"].get<bool>();
					}
					if (j2.contains("isCrimeIgnore")) {
						isCrimeIgnore = j2["isCrimeIgnore"].get<bool>();
					}
					if (j2.contains("merchantContIgnore")) {
						merchantContIgnore = j2["merchantContIgnore"].get<bool>();
					}

					if (j2.contains("show_item_window_track_icon_name")) {
						menu::show_item_window_track_icon_name = j2["show_item_window_track_icon_name"].get<bool>();
					}
					if (j2.contains("show_item_window_track_highlight")) {
						menu::show_item_window_track_highlight = j2["show_item_window_track_highlight"].get<bool>();
					}
					if (j2.contains("show_item_window_track_displayType")) {
						menu::show_item_window_track_displayType = j2["show_item_window_track_displayType"].get<bool>();
					}
					if (j2.contains("colorTrackX")) {
						menu::colorTrack.x = j2["colorTrackX"].get<float>();
						menu::colorTrack.y = j2["colorTrackY"].get<float>();
						menu::colorTrack.z = j2["colorTrackZ"].get<float>();
						menu::colorTrack.w = j2["colorTrackW"].get<float>();
					}
					if (j2.contains("ColorTrackLotdX")) {
						menu::ColorTrackLotd.x = j2["ColorTrackLotdX"].get<float>();
						menu::ColorTrackLotd.y = j2["ColorTrackLotdY"].get<float>();
						menu::ColorTrackLotd.z = j2["ColorTrackLotdZ"].get<float>();
						menu::ColorTrackLotd.w = j2["ColorTrackLotdW"].get<float>();
					}
				}

				if (j.contains("LotdInfo")) {
					auto const& j2 = j["LotdInfo"];
					if (j2.contains("isShowAttached")) {
						lotd::isShowAttached = j2["isShowAttached"].get<bool>();
					}
					if (j2.contains("isInvIgnore")) {
						lotd::isInvIgnore = j2["isInvIgnore"].get<bool>();
					}
					if (j2.contains("isCrimeIgnore")) {
						lotd::isCrimeIgnore = j2["isCrimeIgnore"].get<bool>();
					}

					if (j2.contains("isArmoryIgnore")) {
						lotd::isArmoryIgnore = j2["isArmoryIgnore"].get<bool>();
					}
					if (j2.contains("showlocationItemCount")) {
						lotd::showlocationItemCount = j2["showlocationItemCount"].get<bool>();
					}
					if (j2.contains("isAutoTrackLotdItems")) {
						lotd::isAutoTrackLotdItems = j2["isAutoTrackLotdItems"].get<bool>();
					}
					if (j2.contains("isAutoTrackLotdItemsCrimeIgnore")) {
						lotd::isAutoTrackLotdItemsCrimeIgnore = j2["isAutoTrackLotdItemsCrimeIgnore"].get<bool>();
					}
					
					if (j2.contains("showDisplayItemCount")) {
						lotd::showDisplayItemCount = j2["showDisplayItemCount"].get<bool>();
					}

				}

				if (j.contains("StatInfo")) {
					auto const& j2 = j["StatInfo"];
					if (j2.contains("show_gametime_window")) {
						stats::show_gametime_window = j2["show_gametime_window"].get<bool>();
					}
					if (j2.contains("show_playtime_window")) {
						stats::show_playtime_window = j2["show_playtime_window"].get<bool>();
					}
					if (j2.contains("show_computertime_window")) {
						stats::show_computertime_window = j2["show_computertime_window"].get<bool>();
					}
				}

				if (j.contains("playerBaseInfo")) {
					auto const& j2 = j["playerBaseInfo"];
					show_player_base_info_window = j2["isShow"].get<bool>();

					if (j2.contains("flag_process")) {
						menu::flag_process = j2["flag_process"].get<bool>();
						if (j2.contains("colorProgressHpX")) {
							menu::colorProgressHp.x = j2["colorProgressHpX"].get<float>();
							menu::colorProgressHp.y = j2["colorProgressHpY"].get<float>();
							menu::colorProgressHp.z = j2["colorProgressHpZ"].get<float>();
							menu::colorProgressHp.w = j2["colorProgressHpW"].get<float>();
							menu::colorProgressMp.x = j2["colorProgressMpX"].get<float>();
							menu::colorProgressMp.y = j2["colorProgressMpY"].get<float>();
							menu::colorProgressMp.z = j2["colorProgressMpZ"].get<float>();
							menu::colorProgressMp.w = j2["colorProgressMpW"].get<float>();
							menu::colorProgressSp.x = j2["colorProgressSpX"].get<float>();
							menu::colorProgressSp.y = j2["colorProgressSpY"].get<float>();
							menu::colorProgressSp.z = j2["colorProgressSpZ"].get<float>();
							menu::colorProgressSp.w = j2["colorProgressSpW"].get<float>();
						}
					}

					if (j2.contains("show_player_base_info_window_sep")) {
						menu::show_player_base_info_window_sep = j2["show_player_base_info_window_sep"].get<bool>();
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
					excludeLocationFormIds.insert(j.get<int>());
				}
			}

			if (json.contains("autoContFormIDs")) {
				for (auto const& j : json["autoContFormIDs"]) {
					autoContFormIds.insert(j.get<int>());
				}
			}

			if (json.contains("excludeNpcFormIds")) {
				for (auto const& j : json["excludeNpcFormIds"]) {
					excludeNpcFormIds.insert(j.get<int>());
				}
			}

			if (json.contains("excludeEffectFormIds")) {
				for (auto const& j : json["excludeEffectFormIds"]) {
					if (j.contains("effectId") && j.contains("spellId")) {
						ExcludeFormEffectIds item{ j["effectId"].get<RE::FormID>(), j["spellId"].get<RE::FormID>() };
						excludeEffectFormIds.insert(item);
					}
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

	bool save_settings()
	{
		try {
			nlohmann::json json = {
				{ "imguiSetting", {
									  { "no_titlebar", menu::no_titlebar },
									  { "no_resize", menu::no_resize },
									  { "no_collapse", menu::no_collapse },
									  { "no_background", menu::no_background },
									  { "imgui_style_index", menu::imgui_style_index },
									  { "auto_resize", menu::auto_resize },
									  { "window_border", menu::window_border },
									  { "frame_border", menu::frame_border },
									  { "bullet_text", menu::bullet_text },
									  { "hotkey", menu::hotkey },
									  { "hotkey2", menu::hotkey2 },
									  { "hotkey3", menu::hotkey3 },
									  { "hotkeySetting", menu::hotkeySetting },
									  { "hotkeySettingModifier", menu::hotkeySettingModifier },
									  { "hotkeyItemFinder", menu::hotkeyItemFinder },
									  { "hotkeyItemFinderModifier", menu::hotkeyItemFinderModifier },
									  { "hotkeyTrack", menu::hotkeyTrack },
									  { "hotkeyTrackModifier", menu::hotkeyTrackModifier },
									  { "font_scale", ImGui::GetIO().FontGlobalScale },
									  { "no_background_items", menu::no_background_items },
									  { "imgui_font_index", menu::imgui_font_index },
									  { "grabRounding", ImGui::GetStyle().GrabRounding },
									  { "frameRounding", ImGui::GetStyle().FrameRounding },
									  { "windowRounding", ImGui::GetStyle().WindowRounding },
									  { "languageName", i18n::languageName },

								  } },

				{ "gameSetting", {
									 { "auto_remove_ammo", menu::auto_remove_ammo },
									 { "remember_last_ammo", menu::remember_last_ammo },
								 } },
				{ "windowSetting", {

									   { "playerBaseInfo", {
															   { "isShow", show_player_base_info_window },
															   { "show_player_base_info_window_sep", menu::show_player_base_info_window_sep },
															   { "flag_process", menu::flag_process },
															   { "colorProgressHpX", menu::colorProgressHp.x },
															   { "colorProgressHpY", menu::colorProgressHp.y },
															   { "colorProgressHpZ", menu::colorProgressHp.z },
															   { "colorProgressHpW", menu::colorProgressHp.w },
															   { "colorProgressMpX", menu::colorProgressMp.x },
															   { "colorProgressMpY", menu::colorProgressMp.y },
															   { "colorProgressMpZ", menu::colorProgressMp.z },
															   { "colorProgressMpW", menu::colorProgressMp.w },
															   { "colorProgressSpX", menu::colorProgressSp.x },
															   { "colorProgressSpY", menu::colorProgressSp.y },
															   { "colorProgressSpZ", menu::colorProgressSp.z },
															   { "colorProgressSpW", menu::colorProgressSp.w },

														   } },
									   { "playerModInfo", {
															  { "isShow", show_player_mod_window },
														  } },

									   { "playerInfo", {
														   { "isShow", show_player_info_window },
														   { "show_player_gold_window", show_player_gold_window },
														   { "show_player_carryweight_window", show_player_carryweight_window },
														   { "show_player_xp_window", show_player_xp_window },
														   { "show_player_effects_window", show_player_effects_window },
														   { "show_player_effects_ignore_permanent", show_player_effects_ignore_permanent },
														   { "show_player_effects_negative", show_player_effects_negative },
														   { "show_player_effects_listignore", show_player_effects_listignore },
														   { "colorProgressEffect1X", colorProgressEffect1.x },
														   { "colorProgressEffect1Y", colorProgressEffect1.y },
														   { "colorProgressEffect1Z", colorProgressEffect1.z },
														   { "colorProgressEffect1W", colorProgressEffect1.w },
														   { "colorProgressEffect3X", colorProgressEffect3.x },
														   { "colorProgressEffect3Y", colorProgressEffect3.y },
														   { "colorProgressEffect3Z", colorProgressEffect3.z },
														   { "colorProgressEffect3W", colorProgressEffect3.w },
														   { "show_player_effects_ignore_spell", show_player_effects_ignore_spell },
														   { "show_player_effects_process", show_player_effects_process },
														   { "isShowPlayOSLArousal", sexlab::isShowPlayOSLArousal },

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
														  { "isShow", menu::show_player_debug_window },
													  } },
									   { "NpcInfo", {
														{ "isShow", show_npc_window },
														{ "show_npc_window_dis", show_npc_window_dis },
														{ "show_npc_window_dead_hidden", show_npc_window_dead_hidden },
														{ "show_npc_window_direction", show_npc_window_direction },
														{ "show_enemy", menu::show_enemy },
														{ "show_teammate", menu::show_teammate },
														{ "show_npc", menu::show_npc },
														{ "show_horse", menu::show_horse },
														{ "show_npc_window_ignore", show_npc_window_ignore },
														{ "show_npc_window_process", show_npc_window_process },
														{ "show_npc_window_process_combat", show_npc_window_process_combat },
														{ "colorProgressNpc1X", colorProgressNpc1.x },
														{ "colorProgressNpc1Y", colorProgressNpc1.y },
														{ "colorProgressNpc1Z", colorProgressNpc1.z },
														{ "colorProgressNpc1W", colorProgressNpc1.w },
														{ "colorProgressNpc2X", colorProgressNpc2.x },
														{ "colorProgressNpc2Y", colorProgressNpc2.y },
														{ "colorProgressNpc2Z", colorProgressNpc2.z },
														{ "colorProgressNpc2W", colorProgressNpc2.w },
														{ "colorProgressNpc3X", colorProgressNpc3.x },
														{ "colorProgressNpc3Y", colorProgressNpc3.y },
														{ "colorProgressNpc3Z", colorProgressNpc3.z },
														{ "colorProgressNpc3W", colorProgressNpc3.w },
														{ "show_npc_window_split", menu::show_npc_window_split },
														{ "show_npc_window_dis_meter", show_npc_window_dis_meter },

													} },
									   { "ItemMenuInfo", {
															 { "show_items_window_auto_dis", show_items_window_auto_dis },
															 { "show_inv_window_height", show_inv_window_height },
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
															 { "show_items_window_auto_cont", show_items_window_auto_cont },
															 { "show_items_window_auto_weap", show_items_window_auto_weap },
															 { "show_items_window_auto_armo", show_items_window_auto_armo },
															 { "show_items_window_auto_anvi", show_items_window_auto_anvi },
															 { "show_items_window_auto_ston", show_items_window_auto_ston },
															 { "show_items_window_auto_anhd", show_items_window_auto_anhd },
															 { "show_items_window_auto_anpa", show_items_window_auto_anpa },
															 { "show_items_window_auto_tool", show_items_window_auto_tool },
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
															 { "show_items_window_auto_achr_lock", show_items_window_auto_achr_lock },
															 { "show_items_window_auto_achr_ston", show_items_window_auto_achr_ston },
															 { "show_items_window_auto_achr_anvi", show_items_window_auto_achr_anvi },
															 { "show_items_window_auto_achr_anhd", show_items_window_auto_achr_anhd },
															 { "show_items_window_auto_achr_anpa", show_items_window_auto_achr_anpa },
															 { "show_items_window_auto_achr_tool", show_items_window_auto_achr_tool },
															 { "show_items_window_auto_cont_ingr", show_items_window_auto_cont_ingr },
															 { "show_items_window_auto_cont_food", show_items_window_auto_cont_food },
															 { "show_items_window_auto_cont_alch", show_items_window_auto_cont_alch },
															 { "show_items_window_auto_cont_sgem", show_items_window_auto_cont_sgem },
															 { "show_items_window_auto_cont_ammo", show_items_window_auto_cont_ammo },
															 { "show_items_window_auto_cont_misc", show_items_window_auto_cont_misc },
															 { "show_items_window_auto_cont_gold", show_items_window_auto_cont_gold },
															 { "show_items_window_auto_cont_scrl", show_items_window_auto_cont_scrl },
															 { "show_items_window_auto_cont_keym", show_items_window_auto_cont_keym },
															 { "show_items_window_auto_cont_lock", show_items_window_auto_cont_lock },
															 { "show_items_window_auto_cont_ston", show_items_window_auto_cont_ston },
															 { "show_items_window_auto_cont_anvi", show_items_window_auto_cont_anvi },
															 { "show_items_window_auto_cont_anhd", show_items_window_auto_cont_anhd },
															 { "show_items_window_auto_cont_anpa", show_items_window_auto_cont_anpa },
															 { "show_items_window_auto_cont_tool", show_items_window_auto_cont_tool },
															 { "show_items_window_auto_notification", show_items_window_auto_notification },
															 { "show_items_window_auto_cont_weap", show_items_window_auto_cont_weap },
															 { "show_items_window_auto_achr_weap", show_items_window_auto_achr_weap },
															 { "show_items_window_auto_cont_armo", show_items_window_auto_cont_armo },
															 { "show_items_window_auto_achr_armo", show_items_window_auto_achr_armo },
															 { "show_items_window_auto_weap_enchant", show_items_window_auto_weap_enchant },
															 { "show_items_window_auto_weap_price", show_items_window_auto_weap_price },
															 { "show_items_window_auto_armo_enchant", show_items_window_auto_armo_enchant },
															 { "show_items_window_auto_armo_price", show_items_window_auto_armo_price },
															 { "show_items_window_auto_weap_price_value", show_items_window_auto_weap_price_value },
															 { "show_items_window_auto_armo_price_value", show_items_window_auto_armo_price_value },
															 { "show_items_window_auto_weap_priceweight_value", show_items_window_auto_weap_priceweight_value },
															 { "show_items_window_auto_armo_priceweight_value", show_items_window_auto_armo_priceweight_value },
															 { "show_items_window_auto_weap_priceweight", show_items_window_auto_weap_priceweight },
															 { "show_items_window_auto_armo_priceweight", show_items_window_auto_armo_priceweight },
															 { "show_items_window_auto_conttype", show_items_window_auto_conttype },
															 { "showlocationOreCount", stats::showlocationOreCount },
															 { "showlocationNirnRootRedCount", stats::showlocationNirnRootRedCount },
															 { "showlocationNirnRootCount", stats::showlocationNirnRootCount },
															 { "showlocationExCount", stats::showlocationExCount },
															 { "show_item_window_track_icon_scale", show_item_window_track_icon_scale },
															 { "isCrimeIgnore", isCrimeIgnore },
															 { "merchantContIgnore", merchantContIgnore },
															 { "show_item_window_track_icon_name", menu::show_item_window_track_icon_name },
															 { "show_item_window_track_highlight", menu::show_item_window_track_highlight },
															 { "show_item_window_track_displayType", menu::show_item_window_track_displayType },
															 { "colorTrackX", menu::colorTrack.x },
															 { "colorTrackY", menu::colorTrack.y },
															 { "colorTrackZ", menu::colorTrack.z },
															 { "colorTrackW", menu::colorTrack.w },
															 { "ColorTrackLotdX", menu::ColorTrackLotd.x },
															 { "ColorTrackLotdY", menu::ColorTrackLotd.y },
															 { "ColorTrackLotdZ", menu::ColorTrackLotd.z },
															 { "ColorTrackLotdW", menu::ColorTrackLotd.w },

														 } },
									   { "LotdInfo", {
														 { "showlocationItemCount", lotd::showlocationItemCount },
														 { "isArmoryIgnore", lotd::isArmoryIgnore },
														 { "isCrimeIgnore", lotd::isCrimeIgnore },
														 { "isInvIgnore", lotd::isInvIgnore },
														 { "isShowAttached", lotd::isShowAttached },
														 { "isAutoTrackLotdItems", lotd::isAutoTrackLotdItems },
														 { "isAutoTrackLotdItemsCrimeIgnore", lotd::isAutoTrackLotdItemsCrimeIgnore },
														 { "showDisplayItemCount", lotd::showDisplayItemCount },

													 } },
									   { "StatInfo", {
														 { "show_gametime_window", stats::show_gametime_window },
														 { "show_playtime_window", stats::show_playtime_window },
														 { "show_computertime_window", stats::show_computertime_window },
													 } }

								   } }

			};

			// 忽略项目
			nlohmann::json arr = nlohmann::json::array();
			nlohmann::json arrLocation = nlohmann::json::array();
			nlohmann::json arrAutoCont = nlohmann::json::array();
			nlohmann::json arrNpc = nlohmann::json::array();
			nlohmann::json arrBuff = nlohmann::json::array();

			for (auto id : excludeFormIds) {
				arr.push_back(id);
			}
			json["excludeFormIDs"] = arr;

			for (auto id : excludeLocationFormIds) {
				arrLocation.push_back(id);
			}
			json["excludeLocationFormIDs"] = arrLocation;

			for (auto id : autoContFormIds) {
				arrAutoCont.push_back(id);
			}
			json["autoContFormIDs"] = arrAutoCont;

			for (auto id : excludeNpcFormIds) {
				arrNpc.push_back(id);
			}
			json["excludeNpcFormIds"] = arrNpc;

			for (auto ids : excludeEffectFormIds) {
				nlohmann::json obj;
				obj["effectId"] = ids.effectId;
				obj["spellId"] = ids.spellId;
				arrBuff.push_back(obj);
			}
			json["excludeEffectFormIds"] = arrBuff;

			std::ofstream o(settings_path);
			o << std::setw(4) << json << std::endl;

		} catch (std::exception const& ex) {
			logger::error(ex.what());

			//log() << "Unable to save settings file: " << ex.what() << std::endl;
			return false;
		}
		return true;
	}

}
