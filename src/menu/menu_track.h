#pragma once
#include <imgui/imgui.h>



struct TrackItem
{
	std::string name = "";
	RE::FormID itemBaseFormId = 0;
	bool isLotd = false;
	bool isLotdCont = false;
};

extern std::unordered_map<RE::TESObjectREFR*, TrackItem> trackPtrs2;
extern std::unordered_map<RE::Actor*, TrackItem> trackActorPtrs2;
extern std::mutex mtxTrack;

namespace menu
{
	extern bool show_item_window_track_icon_name;
	extern bool show_item_window_track_highlight;
	extern ImVec4 colorTrack;
	extern bool isTrack;

	void renderTrack();
	void tintTrack(RE::TESObjectREFR* reff);
	void tintTrackClose(RE::TESObjectREFR* reff);
}
