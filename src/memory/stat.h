#pragma once
#include <unordered_set>

namespace stats
{
	// 显示奈恩根
	extern bool showlocationNirnRootRedCount;
	extern bool showlocationNirnRootCount;
	extern bool showlocationOreCount;
	extern bool showlocationExCount;
	extern std::uint32_t locationNirnRootRedCount;
	extern std::uint32_t locationNirnRootCount;
	extern std::uint32_t locationOreCount;
	extern std::uint32_t locationExCount;
	extern std::unordered_set<RE::TESObjectREFR*> locationNirnRootRedIds;
	extern std::unordered_set<RE::TESObjectREFR*> locationNirnRootIds;
	extern std::unordered_set<RE::TESObjectREFR*> locationOreIds;
	extern std::unordered_set<RE::TESObjectREFR*> locationExIds;

	extern bool show_playtime_window;
	extern float playtime;
	extern int playtime_hours;
	extern int playtime_minutes;
	extern int playtime_seconds;

	extern int gametime_hours;
	extern int gametime_minutes;
	extern int gametime_seconds;
	extern bool show_gametime_window;

	void refreshStats();
	void refreshCount();
}

