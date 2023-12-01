#pragma once
#include <PCH.h>
#include <code_lotd/code_lotd.h>
#include <event/BSTContainerChangedEvent.h>
#include <event/BSTDeathEvent.h>
#include <event/BSTMenuEvent.h>
#include <event/BSTPositionPlayerEvent.h>
#include <hook/d3d11hook.h>
#include <hook/dinputhook.h>
#include <memory/data.h>
#include <memory/memory.h>
#include <memory/sexlab.h>
#include <menu/menu_track.h>
#include <setting/setting.h>
#include <setting/i18n.h>
#include <menu/theme.h>
#include <code_lotd/dinputhook.h>

namespace lotdcode
{
	bool isLoadItemfinderplus = false;
	void __cdecl install(void*)
	{
		d3d11hook::Install(1);
		lotdcodedinputhook ::Install();
		MenuOpenCloseEvent::Register();
		BSTContainerChangedEvent::Register();
		BSTDeathEvent::Register();
	}

	void start()
	{
		_beginthread(timerRefresh, 0, NULL);
		_beginthread(install, 0, NULL);

	}

	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kPostLoad:
			{
				logger::info("PostLoad"sv);

				HMODULE hModule = LoadLibrary("skyrim-sse-hud.dll");
				if (hModule != NULL) {
					isLoadItemfinderplus = true;
					logger::critical("已加载ItemFinderPlus,不允许重复加载！"sv);
					FreeLibrary(hModule);
				}

				if (!isLoadItemfinderplus) {
					// 读取配置
					menu::isTrack = false;
					menu::hotkeySetting = 88;
					menu::hotkeySettingModifier = 0x1;
					menu::hotkeyTrack = 59;
					menu::hotkeyTrackModifier = 0x0;
					lotd::isAutoTrackLotdItems = true;
					setting::settings_path = "data\\skse\\plugins\\LotdFinder.json";
					setting::load_settings();
					i18n::i18nPath = "data\\skse\\plugins\\LotdFinder\\i18n";
					i18n::load();
					menu::fontFilePath = "data\\skse\\plugins\\LotdFinder\\fonts\\";
					lotdcode::start();
				}
				//auto messaging = SKSE::GetMessagingInterface();
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

				if (!isLoadItemfinderplus) {
					isGameLoading = true;

					{
						std::lock_guard<std::mutex> lock(mtxTrack);
						if (trackPtrs2.size() > 0) {
							trackPtrs2.clear();
						}
						if (trackActorPtrs2.size() > 0) {
							trackActorPtrs2.clear();
						}
					}
				}
				break;
			}
		case SKSE::MessagingInterface::kPostLoadGame:
			{
				logger::info("kPostLoadGame"sv);

				if (!isLoadItemfinderplus) {
					isGameLoading = false;
					startflag = true;
					isPlayerInvChanged = true;
					islotdContChanged = true;
					if (lotd::isAutoTrackLotdItems) {
						lotd::isAutoTrackLotdItemsFlag = true;
					}
				}
				break;
			}
		case SKSE::MessagingInterface::kNewGame:
			{
				logger::info("kNewGame"sv);

				if (!isLoadItemfinderplus) {
					isGameLoading = false;
					startflag = true;
					if (trackPtrs2.size() > 0) {
						trackPtrs2.clear();
					}
					if (trackActorPtrs2.size() > 0) {
						trackActorPtrs2.clear();
					}

					isPlayerInvChanged = true;
					islotdContChanged = true;
				}
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
				if (!isLoadItemfinderplus) {
					data::init();
					setting::settings_path_lotd_item_list = "data\\skse\\plugins\\LotdFinder\\lotd\\ItemFinderPlus-LotdItemList.json";
					setting::settings_path_lotd_item_display_list = "data\\skse\\plugins\\LotdFinder\\lotd\\ItemFinderPlus-LotdItemDisplayList.json";
					setting::settings_path_lotd_item_display_list_modnames = "data\\skse\\plugins\\LotdFinder\\lotd\\ItemFinderPlus-LotdItemList-ModNames.json";
					lotd::init();
					BSTPositionPlayerEvent::Register();
					// 修补
					lotd::isCrimeIgnore = lotd::isAutoTrackLotdItemsCrimeIgnore;
				}
				break;
			}
		default:
			logger::info("??"sv);
			break;
		}
	}
}
