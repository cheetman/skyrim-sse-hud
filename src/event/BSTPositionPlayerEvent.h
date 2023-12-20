#pragma once
#include <memory/memory.h>
#include <menu/menu.h>
#include <menu/menu_track.h>
#include <memory/lotd.h>
#include <memory/data.h>
#include <memory/track.h>



class BSTPositionPlayerEvent : public RE::BSTEventSink<RE::PositionPlayerEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::PositionPlayerEvent* a_event, RE::BSTEventSource<RE::PositionPlayerEvent>* a_eventSource)
	{
		//logger::info("{} !", a_event->type);
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (a_event->type == RE::PositionPlayerEvent::EVENT_TYPE::kPre) {
			std::lock_guard<std::mutex> lock(mtxTrack);
			for (auto& item : trackPtrs2) {
				menu::tintTrackClose(item.first);
			}

			if (trackPtrs2.size() > 0) {
				trackPtrs2.clear();
			}
			if (trackActorPtrs2.size() > 0) {
				trackActorPtrs2.clear();
			}
		}
		else if (a_event->type == RE::PositionPlayerEvent::EVENT_TYPE::kPost) {
			{
			/*	std::lock_guard<std::mutex> lock(mtxTrack);
				if (trackPtrs2.size() > 0) {
					trackPtrs2.clear();
				}
				if (trackActorPtrs2.size() > 0) {
					trackActorPtrs2.clear();
				}*/
			}

			clearItemInfoCache();
		} else if (a_event->type == RE::PositionPlayerEvent::EVENT_TYPE::kFinish) {
			{
				if (lotd::isAutoTrackLotdItems) {
					lotd::isAutoTrackLotdItemsFlag = true;
				}

				if (lotd::isAutoTrackLotdExcavation) {
					lotd::isAutoTrackLotdExcavationFlag = true;
				}

				if (lotd::isAutoTrackLotdCards) {
					lotd::isAutoTrackLotdCardsFlag = true;
				}
				if (track::isAutoTrackItems && data::autoTrackFormIds.size() > 0) {
					track::isAutoTrackItemsFlag = true;
				}

			}


		}
		return RE::BSEventNotifyControl::kContinue;
	}


	
	static bool Register()
	{
		static BSTPositionPlayerEvent singleton;

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			SKSE::log::warn("PlayerCharacter not found!");
			return false;
		}
		((RE::BSTEventSource<RE::PositionPlayerEvent>*)player)->AddEventSink(&singleton);

		SKSE::log::info("Register {}", typeid(singleton).name());

		return true;
	}
};


