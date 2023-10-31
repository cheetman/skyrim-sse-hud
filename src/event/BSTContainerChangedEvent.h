#pragma once
#include <PCH.h>
#include <memory/lotd.h>

extern bool isPlayerInvChanged;
extern bool islotdContChanged;

class BSTContainerChangedEvent : public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(RE::TESContainerChangedEvent const* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (a_event->newContainer == 0x00000014 || a_event->oldContainer == 0x00000014) {
			isPlayerInvChanged = true;
		}

		for (auto& pair : lotd::displayIdsC) {
			if (pair.first == a_event->newContainer || a_event->oldContainer == pair.first) {
				islotdContChanged = true;
			}
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	static bool Register()
	{
		static BSTContainerChangedEvent singleton;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
		if (!ScriptEventSource) {
			SKSE::log::warn("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&singleton);
		SKSE::log::info("Register {}", typeid(singleton).name());

		return true;
	}
};
