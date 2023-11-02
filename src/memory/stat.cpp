#include "stat.h"
#include <memory/memory.h>
#include <utils/utils.h>
#include <memory/lotd.h>

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

											// 挖掘点
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
