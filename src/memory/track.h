#pragma once

namespace track
{
	/// <summary>
	/// 刷新追踪标记
	/// </summary>
	void refreshAutoTrackItem();

	std::string getTrackerName(RE::TESBoundObject* obj, std::string name);
	extern bool isAutoTrackItems;
	extern bool isAutoTrackItemsFlag;
}
