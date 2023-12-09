#pragma once

#include <memory/lotd.h>

class BSTBGSActorCellEvent : public RE::BSTEventSink<RE::BGSActorCellEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (a_event->flags == RE::BGSActorCellEvent::kFlags_Enter) {
			auto playercell = RE::TESForm::LookupByID<RE::TESObjectCELL>(a_event->cellID);
			if (!playercell) {
				return RE::BSEventNotifyControl::kContinue;
			}
			if (!playercell->IsInteriorCell()) {
				if (lotd::isAutoTrackLotdItems) {
					lotd::isAutoTrackLotdItemsFlag = true;
				}
			}
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	static bool Register()
	{
		static BSTBGSActorCellEvent singleton;

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			SKSE::log::warn("PlayerCharacter not found!");
			return false;
		}
		((RE::BSTEventSource<RE::BGSActorCellEvent>*)player)->AddEventSink(&singleton);

		SKSE::log::info("Register {}", typeid(singleton).name());

		return true;
	}
};
