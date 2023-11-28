#include "event\BSTMenuEvent.h"
#include "event\BSTScriptEvent.h"
#include "event\BSTCrosshairRefEvent.h"

#include "utils\GeneralUtil.h"
#include "utils\NameUtil.h"
#include "utils\PlayerDataProvider.h"
#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

#include <hook/d3d11hook.h>
#include <hook/dinputhook.h>
#include <hook/hudhook.h>
#include <memory/memory.h>
#include <memory/npc.h>
#include <memory/autotake.h>
#include <menu/menu.h>
#include <event/BSTEquipEvent.h>
#include <setting/setting.h>
#include <memory/lotd.h>
#include <event/BSTContainerChangedEvent.h>
#include <event/BSTPositionPlayerEvent.h>
#include <memory/data.h>
#include <memory/sexlab.h>
#include <setting/i18n.h>
#include <event/BSTDeathEvent.h>
#include <event/MessageHandler.h>
#include <code_lotd/MessageHandler.h>


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	//MessageBox(nullptr, TEXT("测试中文."), nullptr, MB_OK);
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

#if LOTD_SPECIFIC_CODE 
		a_info->name = "lotd-finder";
#else
		a_info->name = "sse-hud";
#endif
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





extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("SKSEPlugin_Load"sv);
#ifndef NDEBUG
	//MessageBoxA(nullptr, "SKSEPlugin_Load", nullptr, MB_OK);
	Sleep(1000);
#endif

	
	SKSE::Init(a_skse);

	#if LOTD_SPECIFIC_CODE

		logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());

		auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", lotdcode::MessageHandler)) {
			logger::critical("Could not register MessageHandler"sv);
			return false;
		}

		SKSE::AllocTrampoline(1 << 6);
		logger::info("registered listener"sv);

		// 读取配置
		setting::settings_path = "data\\skse\\plugins\\lotdFinder.json";
		setting::load_settings();
		i18n::load();

		lotdcode::start();

	#else

		logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());

		auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", MessageHandler)) {
			logger::critical("Could not register MessageHandler"sv);
			return false;
		}

		SKSE::AllocTrampoline(1 << 6);
		logger::info("registered listener"sv);

		// 读取配置
		setting::load_settings();
		i18n::load();

		_beginthread(RefreshGameInfo, 0, NULL);
		_beginthread(RefreshActorInfo, 0, NULL);
		_beginthread(RefreshItemInfo, 0, NULL);
		_beginthread(TimerAutoPick, 0, NULL);
		_beginthread(installimgui, 0, NULL);
	#endif
	return true;
}

