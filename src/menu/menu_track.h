#pragma once
#include <imgui/imgui.h>

namespace menu
{
	extern bool show_item_window_track_icon_name;
	extern bool show_item_window_track_highlight;
	extern ImVec4 colorTrack;
	void renderTrack();
	void tintTrack(RE::TESObjectREFR* reff);
	void tintTrackClose(RE::TESObjectREFR* reff);
}
