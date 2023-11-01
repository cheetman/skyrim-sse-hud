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

	void refreshCount();
}

