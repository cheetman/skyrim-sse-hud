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
	extern ImVec4 ColorTrackLotd;
	extern bool isTrack;

	void renderTrack();
	void tintTrack(RE::TESObjectREFR* reff);
	void tintTrackClose(RE::TESObjectREFR* reff);

	extern int show_item_window_track_displayType;
	static const std::unordered_map<int, std::string> displayType = {
		{ 0, "默认" },
		{ 1, "快捷键切换" },
		{ 2, "快捷键长按" },
	};
}
