#pragma once
#include <memory/memory.h>


class BSTPositionPlayerEvent : public RE::BSTEventSink<RE::PositionPlayerEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::PositionPlayerEvent* a_event, RE::BSTEventSource<RE::PositionPlayerEvent>* a_eventSource)
	{
		//logger::info("{} !", a_event->type);
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (a_event->type == RE::PositionPlayerEvent::EVENT_TYPE::kPost) {
			{
				std::lock_guard<std::mutex> lock(mtxTrack);
				if (trackPtrs.size() > 0) {
					trackPtrs.clear();
				}
				if (trackActorPtrs.size() > 0) {
					trackActorPtrs.clear();
				}
			}

			clearItemInfoCache();
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


