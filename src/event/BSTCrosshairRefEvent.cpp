#include "BSTCrosshairRefEvent.h"

	BSTCrosshairRefEvent* BSTCrosshairRefEvent::GetSingleton()
	{
		static BSTCrosshairRefEvent singleton;
		return std::addressof(singleton);
	}

	void BSTCrosshairRefEvent::Register()
	{
		auto crosshair = SKSE::GetCrosshairRefEventSource();
		if (crosshair) {
			static BSTCrosshairRefEvent singleton;
			crosshair->AddEventSink(&singleton);
			logger::info("Registered {}"sv, typeid(SKSE::CrosshairRefEvent).name());
		}
	}

	RE::TESObjectREFR* BSTCrosshairRefEvent::GetCrosshairReference()
	{
		return _cachedRef;
	}

	


	EventResult BSTCrosshairRefEvent::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*)
	{
		if (a_event && a_event->crosshairRef) {
			_cachedRef = a_event->crosshairRef.get();

	/*		logger::debug("准星");
			logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetDisplayFullName()));
			logger::debug(_cachedRef->GetFormID());*/

		} else {
			_cachedRef = nullptr;
		}
		return EventResult::kContinue;
	}
