#include "stat.h"
#include <memory/lotd.h>
#include <memory/memory.h>
#include <utils/utils.h>

namespace stats
{
	bool showlocationNirnRootRedCount = false;
	bool showlocationNirnRootCount = false;
	bool showlocationOreCount = false;
	bool showlocationExCount = false;
	std::unordered_set<RE::TESObjectREFR*> locationNirnRootRedIds;
	std::unordered_set<RE::TESObjectREFR*> locationNirnRootIds;
	std::unordered_set<RE::TESObjectREFR*> locationOreIds;
	std::unordered_set<RE::TESObjectREFR*> locationExIds;

	std::uint32_t locationNirnRootRedCount;
	std::uint32_t locationNirnRootCount;
	std::uint32_t locationOreCount;
	std::uint32_t locationExCount;

	float playtime = 0;
	int playtime_hours = 0;
	int playtime_minutes = 0;
	int playtime_seconds = 0;
	bool show_playtime_window = false;

	int gametime_hours = 0;
	int gametime_minutes = 0;
	int gametime_seconds = 0;
	bool show_gametime_window = false;

	char computertime[100];
	bool show_computertime_window = false;

	void refreshStats()
	{
		// 时间
		if (stats::show_playtime_window) {
			float time = ScriptUtil::GetRealHoursPassed(nullptr, 0);
			//stats::playtime = time;
			stats::playtime_hours = static_cast<int>(time);
			stats::playtime_minutes = static_cast<int>((time - stats::playtime_hours) * 60);
			stats::playtime_seconds = static_cast<int>((time - stats::playtime_hours - stats::playtime_minutes / 60.0) * 3600);
		}

		// 时间
		if (stats::show_gametime_window) {
			auto calendar = RE::Calendar::GetSingleton();
			if (calendar) {
				auto time = calendar->gameHour->value;
				stats::gametime_hours = static_cast<int>(time);
				stats::gametime_minutes = static_cast<int>((time - stats::gametime_hours) * 60);
				stats::gametime_seconds = static_cast<int>((time - stats::gametime_hours - stats::gametime_minutes / 60.0) * 3600);
			}
		}

		if (stats::show_computertime_window) {
			time_t currentTime;
			struct tm* timeInfo;
			// 获取当前时间
			time(&currentTime);
			// 将当前时间转换为时间结构
			timeInfo = localtime(&currentTime);
			// 将时间结构格式化为可读的时间字符串
			strftime(computertime, sizeof(computertime), "%H:%M:%S", timeInfo);
		}
	}

	void refreshCount()
	{
		if (showlocationNirnRootRedCount || showlocationNirnRootCount || showlocationOreCount || showlocationExCount) {
			locationNirnRootRedIds.clear();
			locationNirnRootIds.clear();
			locationOreIds.clear();
			locationExIds.clear();

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (!player) {
				return;
			}

			int currentLocationFormId = 0;
			auto currentLocation = player->currentLocation;
			if (currentLocation) {
				currentLocationFormId = currentLocation->GetFormID();
			}

			const auto& [map, lock] = RE::TESForm::GetAllForms();
			const RE::BSReadWriteLock locker{ lock };
			if (!map) {
				return;
			}
			for (auto& [id, form] : *map) {
				if (form) {
					if (form->Is(RE::FormType::Reference)) {
						auto reff = form->AsReference();
						if (reff) {
							if (reff->GetCurrentLocation() == currentLocation) {
								auto baseObj = reff->GetBaseObject();
								if (baseObj) {
									switch (baseObj->GetFormType()) {
									case RE::FormType::Activator:
										{
											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											if (!reff->Is3DLoaded()) {
												continue;
											}

											auto name = reff->GetDisplayFullName();
											if (strlen(name) == 0) {
												continue;
											}

											float distance = ValueUtil::calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

											if (!currentLocation) {
												if (distance > show_items_window_auto_dis_skyrim) {
													continue;
												}
											} else {
												if (distance > show_items_window_auto_dis_local) {
													continue;
												}
											}

											if (baseObj->GetFormID() == 0x000E1FB2) {
												locationNirnRootIds.insert(reff);
												continue;
											}

											if (baseObj->GetFormID() == 0x000B6FB9) {
												locationNirnRootRedIds.insert(reff);
												continue;
											}

											// 挖掘点 自动标记
											if (lotd::excavationIds.find(baseObj->GetFormID()) != lotd::excavationIds.end()) {
												locationExIds.insert(reff);
												continue;
											}

											auto linkedReff = reff->GetLinkedRef(nullptr);
											if (linkedReff) {
												auto linkedBaseObj = linkedReff->GetBaseObject();
												if (linkedBaseObj) {
													if (oreFormIds.find(linkedBaseObj->GetFormID()) != oreFormIds.end()) {
														locationOreIds.insert(reff);
														continue;
													}
												}
											}

											break;
										}
									default:
										{
											continue;
										}
									}
								}
							}
						}
					}
				}
			}

			locationNirnRootRedCount = locationNirnRootRedIds.size();
			locationNirnRootCount = locationNirnRootIds.size();
			locationOreCount = locationOreIds.size();
			locationExCount = locationExIds.size();
		}
	}

}
