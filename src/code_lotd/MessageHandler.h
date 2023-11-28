#pragma once
#include <PCH.h>
#include <memory/memory.h>
#include <menu/menu_track.h>
#include <event/BSTContainerChangedEvent.h>
#include <memory/data.h>
#include <memory/sexlab.h>
#include <event/BSTPositionPlayerEvent.h>
#include <event/BSTMenuEvent.h>
#include <hook/d3d11hook.h>
#include <code_lotd/code_lotd.h>


namespace lotdcode
{
	void __cdecl install(void*)
	{
		d3d11hook::Install(1);
		dinputhook ::Install();

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

				auto messaging = SKSE::GetMessagingInterface();

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
				break;
			}
		case SKSE::MessagingInterface::kPostLoadGame:
			{
				logger::info("kPostLoadGame"sv);
				isGameLoading = false;
				startflag = true;
				isPlayerInvChanged = true;
				islotdContChanged = true;
				if (lotd::isAutoTrackLotdItems) {
					lotd::isAutoTrackLotdItemsFlag = true;
				}

				break;
			}
		case SKSE::MessagingInterface::kNewGame:
			{
				logger::info("kNewGame"sv);
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
				data::init();
				lotd::init();
				BSTPositionPlayerEvent::Register();

				break;
			}
		default:
			logger::info("??"sv);
			break;
		}
	}
}
