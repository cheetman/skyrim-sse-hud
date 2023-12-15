#pragma once
#include <PCH.h>
#include <memory/memory.h>
#include <menu/menu_track.h>
#include <event/BSTContainerChangedEvent.h>
#include <memory/data.h>
#include <memory/sexlab.h>
#include <event/BSTPositionPlayerEvent.h>
#include <event/BSTBGSActorCellEvent.h>




void __cdecl installimgui(void*)
{
	d3d11hook::Install(1);
	dinputhook ::Install();

	MenuOpenCloseEvent::Register();
	EquipEvent::Register();
	BSTContainerChangedEvent::Register();
	BSTDeathEvent::Register();
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
			if (lotd::isAutoTrackLotdExcavation) {
				lotd::isAutoTrackLotdExcavationFlag = true;
			}
			if (lotd::isAutoTrackLotdCards) {
				lotd::isAutoTrackLotdCardsFlag = true;
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
			sexlab::init();
			BSTPositionPlayerEvent::Register();
			BSTBGSActorCellEvent::Register();

			// input event
			//auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
			//inputDeviceManager->AddEventSink();
			break;
		}
	default:
		logger::info("??"sv);
		break;
	}
}
