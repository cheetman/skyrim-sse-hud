#pragma once
#include <PCH.h>
#include "SKSE\API.h"
#include "utils/utils.h"
#include <string>

using EventResult = RE::BSEventNotifyControl;

static class BSTCrosshairRefEvent : public RE::BSTEventSink<SKSE::CrosshairRefEvent>
{
public:
	static BSTCrosshairRefEvent* GetSingleton();

	static void Register();

	RE::TESObjectREFR* GetCrosshairReference();

	//static RE::TESObjectREFR* getRef() {
	//	return _cachedRef;
	//}

protected:
	EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*);

private:
	BSTCrosshairRefEvent() = default;
	BSTCrosshairRefEvent(const BSTCrosshairRefEvent&) = delete;
	BSTCrosshairRefEvent(BSTCrosshairRefEvent&&) = delete;
	~BSTCrosshairRefEvent() = default;

	BSTCrosshairRefEvent& operator=(const BSTCrosshairRefEvent&) = delete;
	BSTCrosshairRefEvent& operator=(BSTCrosshairRefEvent&&) = delete;

	RE::TESObjectREFR* _cachedRef;
};
