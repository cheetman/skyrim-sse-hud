#pragma once
#include <PCH.h>

using EventResult = RE::BSEventNotifyControl;

class OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
{
public:
	virtual EventResult ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource) {
			SKSE::log::info("Event Source Not Found!");
			return EventResult::kContinue;
		}

		SKSE::log::info("OnHit Event Trigger!");

		SKSE::log::info("Hit flag is {}, source is {:x}", a_event->flags.get(), a_event->source);

		return EventResult::kContinue;
	}

	static bool Register()
	{
		static OnHitEventHandler singleton;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

		if (!ScriptEventSource) {
			SKSE::log::info("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&singleton);

		SKSE::log::info("Register {}", typeid(singleton).name());

		return true;
	}

private:
	OnHitEventHandler() = default;

	~OnHitEventHandler() = default;

	OnHitEventHandler(const OnHitEventHandler&) = delete;

	OnHitEventHandler(OnHitEventHandler&&) = delete;

	OnHitEventHandler& operator=(const OnHitEventHandler&) = delete;

	OnHitEventHandler& operator=(OnHitEventHandler&&) = delete;
};
