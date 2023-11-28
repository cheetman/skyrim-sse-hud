#pragma once
#include <PCH.h>
#include <memory/lotd.h>
#include <menu/menu_track.h>
#include <utils/utils.h>

class BSTDeathEvent : public RE::BSTEventSink<RE::TESDeathEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(RE::TESDeathEvent const* a_event, RE::BSTEventSource<RE::TESDeathEvent>* a_eventSource)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (lotd::isLoad) {
			if (a_event->dead && !a_event->actorDying->IsPlayerRef()) {
				auto actor = a_event->actorDying->As<RE::Actor>();
				if (actor && !actor->IsSummoned() && actor->Is3DLoaded()) {
					auto inv = actor->GetInventory(FormUtil::CanDisplay);
					for (auto& [obj, data] : inv) {
						auto& [count, entry] = data;
						if (count > 0 && entry) {
							// 放到对应的数组里
							bool success = lotd::checkItem(obj->GetFormID());
							if (success) {
								if (trackPtrs2.find(actor) == trackPtrs2.end()) {
									TrackItem trackItem;
									trackItem.name = lotd::getTrackerName(obj, obj->GetName());
									trackItem.isLotd = true;
									trackItem.itemBaseFormId = obj->GetFormID();
									trackItem.isLotdCont = true;
									trackPtrs2.insert(std::make_pair(actor, trackItem));
									//menu::tintTrack(actor);
									break;
								}
							}
						}
					}
				}
			}
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	static bool Register()
	{
		static BSTDeathEvent singleton;

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
