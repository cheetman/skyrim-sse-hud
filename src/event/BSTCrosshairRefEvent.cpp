#include "BSTCrosshairRefEvent.h"
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>

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
			logger::debug(FormIDToString(_cachedRef->GetFormID()));
			logger::debug(_cachedRef->GetFormEditorID());
			logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetDisplayFullName()));
			logger::debug(GetFormTypeName(_cachedRef->formType.underlying()));
			//logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetEditorLocation1()->GetName()));
			RE::BGSLocation* current = _cachedRef->GetCurrentLocation();
			if (current) {
				logger::debug(StringUtil::Utf8ToGbk(current->GetFullName()));
			} else {
				logger::debug("空Location");
			}
			logger::debug(std::to_string(_cachedRef->GetPositionX()) + " " + std::to_string(_cachedRef->GetPositionY()) + " " + std::to_string(_cachedRef->GetPositionZ()));
			auto editor = _cachedRef->GetEditorLocation1();
			if (editor) {
				logger::debug(StringUtil::Utf8ToGbk(editor->GetFullName()));
			} else {
				logger::debug("空Location");
			}

	/*		logger::debug("准星");
			logger::debug(StringUtil::Utf8ToGbk(_cachedRef->GetDisplayFullName()));
			logger::debug(_cachedRef->GetFormID());*/

		} else {
			_cachedRef = nullptr;
		}
		return EventResult::kContinue;
	}
