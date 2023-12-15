#include "memory.h"
#include <event/BSTMenuEvent.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/npc.h>
#include <memory/player.h>
#include <memory/stat.h>
#include <setting/setting.h>
#include <unordered_set>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
#include <utils/PlayerDataProvider.h>

bool isShowQuest = false;
bool isCrimeIgnore = false;

bool active = false;
bool activeItems = false;
int refresh_time_data = 500;
bool startflag = false;

bool isGameLoading = false;

int screenWidth = 0;
int screenHeight = 0;

int show_inv_window_height = 14;

bool show_items_window = false;
bool show_items_window_settings = false;
bool show_items_window_formid = false;
bool show_items_window_refid = false;
bool show_items_window_direction = false;
bool show_items_window_file = false;
bool show_items_window_3D = false;
bool show_items_window_ignore = true;
int show_items_window_array_max_length = 2998;

int show_items_window_auto_dis_skyrim = 100;
int show_items_window_auto_dis_local = 200;

Item2Info* items = new Item2Info[2];
int nowItemIndex = 0;

// 物品排除
std::unordered_set<int> excludeFormIds;
std::vector<ExcludeForm> excludeForms;

// 商贩容器
std::unordered_set<RE::FormID> merchantContFormIds;
bool merchantContIgnore = true;

// 龙爪
std::unordered_set<RE::FormID> clawFormIds;

// 矿脉
std::unordered_set<RE::FormID> oreFormIds;

// 天气
std::vector<WeatherForm> weatherForms;
RE::FormID currentWeather = 0;

void __cdecl RefreshGameInfo(void*)
{
	// 标记装备槽是否主要
	wornArmos[2].isMainSlotAlert = wornArmos[3].isMainSlotAlert = wornArmos[5].isMainSlotAlert = wornArmos[6].isMainSlotAlert = wornArmos[7].isMainSlotAlert = true;
	wornArmos[1].isSpeacilSlotAlert = wornArmos[2].isSpeacilSlotAlert = wornArmos[3].isSpeacilSlotAlert = wornArmos[5].isSpeacilSlotAlert = wornArmos[6].isSpeacilSlotAlert = wornArmos[7].isSpeacilSlotAlert = wornArmos[9].isSpeacilSlotAlert = false;
	for (int i = 0; i <= 31; i++) {
		wornArmos[i].equipSlotName = GetEquipSlotName(i);
	}

	while (true) {
		if (refresh_time_data < 50) {
			refresh_time_data = 50;
		}
		Sleep(refresh_time_data);
		if (!startflag) {
			continue;
		}
		if (isGameLoading) {
			continue;
		}

		if (isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu) {
			continue;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (player) {
			/*sky->currentWeather;
				if (weather) {
					weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kCloudy);*/

			// 更新玩家信息
			refreshPlayerInfo();

			// 更新其他信息
			stats::refreshStats();
		}
	}
}

bool compareForItem(const ItemInfo& info1, const ItemInfo& info2)
{
	if (info1.gold != info2.gold) {
		return info1.gold > info2.gold;
	} else {
		return info1.baseFormId < info2.baseFormId;
	}
}

bool compareForItemCONT(const ItemInfoCONT& info1, const ItemInfoCONT& info2)
{
	if (info1.baseFormId != info2.baseFormId) {
		return info1.baseFormId > info2.baseFormId;
	} else {
		return info1.invCount > info2.invCount;
	}
}

ItemInfo* getItems()
{
	return items[!nowItemIndex].itemInfo;
}
ItemInfo* getItemsWEAP()
{
	return items[!nowItemIndex].itemInfoWEAP;
}
ItemInfo* getItemsARMO()
{
	return items[!nowItemIndex].itemInfoARMO;
}
ItemInfo* getItemsAMMO()
{
	return items[!nowItemIndex].itemInfoAMMO;
}
ItemInfoBOOK* getItemsBOOK()
{
	return items[!nowItemIndex].itemInfoBOOK;
}
ItemInfo* getItemsALCH()
{
	return items[!nowItemIndex].itemInfoALCH;
}
ItemInfo* getItemsFOOD()
{
	return items[!nowItemIndex].itemInfoFOOD;
}
ItemInfo* getItemsINGR()
{
	return items[!nowItemIndex].itemInfoINGR;
}
ItemInfo* getItemsMISC()
{
	return items[!nowItemIndex].itemInfoMISC;
}
ItemInfoCONT* getItemsCONT()
{
	return items[!nowItemIndex].itemInfoCONT;
}
ItemInfo* getItemsFLOR()
{
	return items[!nowItemIndex].itemInfoFLOR;
}
ItemInfo* getItemsKEYM()
{
	return items[!nowItemIndex].itemInfoKEYM;
}
ItemInfo* getItemsTREE()
{
	return items[!nowItemIndex].itemInfoTREE;
}
ItemInfo* getItemsSGEM()
{
	return items[!nowItemIndex].itemInfoSGEM;
}
ItemInfo* getItemsACTI()
{
	return items[!nowItemIndex].itemInfoACTI;
}
ItemInfoCONT* getItemsACHR()
{
	return items[!nowItemIndex].itemInfoACHR;
}
ItemInfo* getItemsSTON()
{
	return items[!nowItemIndex].itemInfoSTON;
}
ItemInfo* getItemsANVI()
{
	return items[!nowItemIndex].itemInfoANVI;
}
ItemInfo* getItemsANHD()
{
	return items[!nowItemIndex].itemInfoANHD;
}
ItemInfo* getItemsANPA()
{
	return items[!nowItemIndex].itemInfoANPA;
}
ItemInfo* getItemsTOOL()
{
	return items[!nowItemIndex].itemInfoTOOL;
}
std::vector<QuestInfo>& getQuests()
{
	return items[!nowItemIndex].quests;
}

int getItemCount()
{
	return items[!nowItemIndex].itemCount;
}
int getItemCountWEAP()
{
	return items[!nowItemIndex].itemCountWEAP;
}
int getItemCountARMO()
{
	return items[!nowItemIndex].itemCountARMO;
}
int getItemCountAMMO()
{
	return items[!nowItemIndex].itemCountAMMO;
}
int getItemCountBOOK()
{
	return items[!nowItemIndex].itemCountBOOK;
}
int getItemCountALCH()
{
	return items[!nowItemIndex].itemCountALCH;
}
int getItemCountFOOD()
{
	return items[!nowItemIndex].itemCountFOOD;
}
int getItemCountINGR()
{
	return items[!nowItemIndex].itemCountINGR;
}
int getItemCountMISC()
{
	return items[!nowItemIndex].itemCountMISC;
}
int getItemCountCONT()
{
	return items[!nowItemIndex].itemCountCONT;
}
int getItemCountFLOR()
{
	return items[!nowItemIndex].itemCountFLOR;
}
int getItemCountKEYM()
{
	return items[!nowItemIndex].itemCountKEYM;
}
int getItemCountTREE()
{
	return items[!nowItemIndex].itemCountTREE;
}
int getItemCountSGEM()
{
	return items[!nowItemIndex].itemCountSGEM;
}
int getItemCountACTI()
{
	return items[!nowItemIndex].itemCountACTI;
}
int getItemCountACHR()
{
	return items[!nowItemIndex].itemCountACHR;
}
int getItemCountSTON()
{
	return items[!nowItemIndex].itemCountSTON;
}
int getItemCountANVI()
{
	return items[!nowItemIndex].itemCountANVI;
}

int getItemCountANHD()
{
	return items[!nowItemIndex].itemCountANHD;
}

int getItemCountANPA()
{
	return items[!nowItemIndex].itemCountANPA;
}

int getItemCountTOOL()
{
	return items[!nowItemIndex].itemCountTOOL;
}

int getQuestCount()
{
	return items[!nowItemIndex].questCount;
}

void clearItemInfoCache()
{
	items[0].itemCount = 0;
	items[0].itemCountWEAP = 0;
	items[0].itemCountARMO = 0;
	items[0].itemCountAMMO = 0;
	items[0].itemCountBOOK = 0;
	items[0].itemCountALCH = 0;
	items[0].itemCountINGR = 0;
	items[0].itemCountMISC = 0;
	items[0].itemCountCONT = 0;
	items[0].itemCountFLOR = 0;
	items[0].itemCountFOOD = 0;
	items[0].itemCountKEYM = 0;
	items[0].itemCountTREE = 0;
	items[0].itemCountSGEM = 0;
	items[0].itemCountACTI = 0;
	items[0].itemCountACHR = 0;
	items[0].itemCountSTON = 0;
	items[0].itemCountANVI = 0;
	items[0].itemCountANHD = 0;
	items[0].itemCountANPA = 0;
	items[0].itemCountTOOL = 0;

	items[1].itemCount = 0;
	items[1].itemCountWEAP = 0;
	items[1].itemCountARMO = 0;
	items[1].itemCountAMMO = 0;
	items[1].itemCountBOOK = 0;
	items[1].itemCountALCH = 0;
	items[1].itemCountINGR = 0;
	items[1].itemCountMISC = 0;
	items[1].itemCountCONT = 0;
	items[1].itemCountFLOR = 0;
	items[1].itemCountFOOD = 0;
	items[1].itemCountKEYM = 0;
	items[1].itemCountTREE = 0;
	items[1].itemCountSGEM = 0;
	items[1].itemCountACTI = 0;
	items[1].itemCountACHR = 0;
	items[1].itemCountSTON = 0;
	items[1].itemCountANVI = 0;
	items[1].itemCountANHD = 0;
	items[1].itemCountANPA = 0;
	items[1].itemCountTOOL = 0;
}

bool compareForQuest(const QuestInfo& info1, const QuestInfo& info2)
{
	if (info1.isActive != info2.isActive) {
		return info1.isActive > info2.isActive;
	} else if (info1.progressStage != info2.progressStage) {
		return info1.progressStage > info2.progressStage;
	} else {
		return info1.formId > info2.formId;
	}
}

void __cdecl RefreshItemInfo(void*)
{
	while (true) {
		Sleep(700);
		if (!startflag) {
			Sleep(1000);
			continue;
		}

		if (isGameLoading) {
			Sleep(1000);
			continue;
		}

		if (!activeItems) {
			continue;
		}

		if (isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu) {
			continue;
		}

		// 以区分艺术馆
		if (lotd::isLoad && lotd::isShow) {
			lotd::refreshItemInfo();
			continue;
		}

		if (lotd::isLoad && lotd::isShowAttached) {
			lotd::refreshItemInfoAttached();
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			Sleep(1000);
			continue;
		}

		Sleep(100);

		int tmpQuestCount = 0;
		if (isShowQuest) {
			auto& item = items[nowItemIndex];

			if (item.quests.size() == 0) {
				item.quests.resize(10000);
			}

			for (const auto quest : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESQuest>()) {
				/*if (quest->IsActive()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					tmpQuestCount++;
				}*/

				/*if (quest->IsStarting()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					tmpQuestCount++;
				}

				if (quest->IsCompleted()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					tmpQuestCount++;
				}*/

				if (quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) && !quest->IsCompleted()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					item.quests[tmpQuestCount].isActive = quest->IsActive();
					int completedStage = 0;
					int allStage = 0;
					for (auto item : *quest->waitingStages) {
						allStage++;
						if (item->data.flags.underlying() == 1) {
							completedStage++;
						}
					}
					item.quests[tmpQuestCount].allStage = allStage;
					item.quests[tmpQuestCount].completedStage = completedStage;
					item.quests[tmpQuestCount].progressStage = (float)completedStage / (float)(allStage == 0 ? 1 : allStage);

					tmpQuestCount++;
				}
			}

			std::partial_sort(item.quests.begin(), item.quests.begin() + tmpQuestCount, item.quests.begin() + tmpQuestCount, compareForQuest);
			items[nowItemIndex].questCount = tmpQuestCount;
		}

		int tmpCount = 0;
		int tmpCountWEAP = 0;
		int tmpCountARMO = 0;
		int tmpCountAMMO = 0;
		int tmpCountBOOK = 0;
		int tmpCountALCH = 0;
		int tmpCountINGR = 0;
		int tmpCountMISC = 0;
		int tmpCountCONT = 0;
		int tmpCountFLOR = 0;
		int tmpCountFOOD = 0;
		int tmpCountKEYM = 0;
		int tmpCountTREE = 0;
		int tmpCountSGEM = 0;
		int tmpCountACTI = 0;
		int tmpCountACHR = 0;
		int tmpCountSTON = 0;
		int tmpCountANVI = 0;
		int tmpCountANHD = 0;
		int tmpCountANPA = 0;
		int tmpCountTOOL = 0;

		auto currentLocation = player->currentLocation;

		{
			const auto& [map, lock] = RE::TESForm::GetAllForms();
			const RE::BSReadWriteLock locker{ lock };
			if (!map) {
				continue;
			}

			for (auto& [id, form] : *map) {
				if (form) {
					if (form->Is(RE::FormType::ActorCharacter)) {
						auto actor = form->As<RE::Actor>();
						if (actor && actor->IsDead() && !actor->IsSummoned()) {
							// 排除自己
							if (actor->IsPlayerRef()) {
								continue;
							}
							if (actor->GetCurrentLocation() == currentLocation) {
								if (!show_items_window_3D) {
									if (!actor->Is3DLoaded()) {
										continue;
									}
								}

								if (tmpCountACHR > show_items_window_array_max_length) {
									continue;
								}

								if (actor->IsMarkedForDeletion()) {
									continue;
								}
								auto name = actor->GetDisplayFullName();
								if (strlen(name) == 0) {
									continue;
								}
								float distance = ValueUtil::calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;

								if (!currentLocation) {
									if (distance > show_items_window_auto_dis_skyrim) {
										continue;
									}
								} else {
									if (distance > show_items_window_auto_dis_local) {
										continue;
									}
								}

								bool isCrime = actor->IsCrimeToActivate();
								if (isCrimeIgnore) {
									if (isCrime) {
										continue;
									}
								}

								items[nowItemIndex].itemInfoACHR[tmpCountACHR].ptr = actor;
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].baseFormId = actor->GetFormID();
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].baseFormIdStr = FormIDToString(actor->GetFormID());
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].formId = actor->GetFormID();
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].formIdStr = FormIDToString(actor->GetFormID());
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].formTypeStr = GetFormTypeName(actor->formType.underlying());
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].name = name;
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].weight = actor->GetWeight();
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].gold = actor->GetGoldValue();
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].isCrime = isCrime;
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].isEnchanted = actor->IsEnchanted();

								if (show_items_window_direction) {
									items[nowItemIndex].itemInfoACHR[tmpCountACHR].distance = distance;
									items[nowItemIndex].itemInfoACHR[tmpCountACHR].direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
								}

								int tmpInvCount = 0;
								auto inv = actor->GetInventory(FormUtil::CanDisplay);
								for (auto& [obj, data] : inv) {
									auto& [count, entry] = data;
									if (count > 0 && entry) {
#ifndef NDEBUG
										if (obj->IsIgnored()) {
											MessageBox(nullptr, obj->GetName(), nullptr, MB_OK);
										}
#endif

										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].ptr = obj;
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].name = obj->GetName();
										bool stealing = player->WouldBeStealing(actor);
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].isCrime = !entry.get()->IsOwnedBy(player, !stealing);
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].isEnchanted = entry.get()->IsEnchanted();
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount++].count = count;
										if (tmpInvCount == 200) {
											break;
										}
									}
								}
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].invCount = tmpInvCount;

								tmpCountACHR++;
							}
						}
					}

					else if (form->Is(RE::FormType::Reference)) {
						auto reff = form->AsReference();
						if (reff) {
							if (reff->GetCurrentLocation() == currentLocation) {
								if (!show_items_window_3D) {
									if (!reff->Is3DLoaded()) {
										continue;
									}
								}

								auto baseObj = reff->GetBaseObject();
								if (baseObj) {
									switch (baseObj->GetFormType()) {
									case RE::FormType::Weapon:
										{
											if (tmpCountWEAP > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].ptr = reff;
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].name = name;
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].isCrime = isCrime;
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].isEnchanted = reff->IsEnchanted();

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].distance = distance;
												items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].filename = baseObj->GetFile(0)->fileName;
											}

											tmpCountWEAP++;

											break;
										}
									case RE::FormType::Armor:
										{
											if (tmpCountARMO > show_items_window_array_max_length) {
												continue;
											}
											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoARMO[tmpCountARMO].ptr = reff;
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].name = name;
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].isCrime = isCrime;
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].isEnchanted = reff->IsEnchanted();

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoARMO[tmpCountARMO].distance = distance;
												items[nowItemIndex].itemInfoARMO[tmpCountARMO].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoARMO[tmpCountARMO].filename = baseObj->GetFile(0)->fileName;
											}
											tmpCountARMO++;

											break;
										}
									case RE::FormType::Ammo:
										{
											if (tmpCountAMMO > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].distance = distance;
												items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}

											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].ptr = reff;
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].name = name;
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].isCrime = isCrime;

											if (show_items_window_file) {
												items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].filename = baseObj->GetFile(0)->fileName;
											}
											tmpCountAMMO++;

											break;
										}
									case RE::FormType::Book:
										{
											if (tmpCountBOOK > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();

											bool isRead = false;
											auto book = baseObj->As<RE::TESObjectBOOK>();
											if (book) {
												isRead = book->IsRead();
											}

											if (isCrimeIgnore) {
												if (isCrime && isRead) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].ptr = reff;
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].isCrime = isCrime;

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].distance = distance;
												items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].filename = baseObj->GetFile(0)->fileName;
											}

											if (book) {
												if (book->TeachesSpell()) {
													items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formTypeStr = "魔法";
												} else if (book->TeachesSkill()) {
													items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formTypeStr = "技能";
												} else if (book->IsNoteScroll()) {
													items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formTypeStr = "笔记";
												} else {
													items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formTypeStr = "-";
												}

												items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].isRead = isRead;
											}

											tmpCountBOOK++;

											break;
										}
									case RE::FormType::AlchemyItem:
										{
											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
											}
											auto name = reff->GetDisplayFullName();
											if (strlen(name) == 0) {
												continue;
											}
											auto alchemyItem = baseObj->As<RE::AlchemyItem>();
											if (alchemyItem->IsFood()) {
												if (tmpCountFOOD > show_items_window_array_max_length) {
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

												bool isCrime = reff->IsCrimeToActivate();
												if (isCrimeIgnore) {
													if (isCrime) {
														continue;
													}
												}

												if (show_items_window_direction) {
													items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].distance = distance;
													items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
												}

												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].ptr = reff;
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].baseFormId = baseObj->GetFormID();
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].baseFormIdStr = FormIDToString(baseObj->GetFormID());
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].formId = reff->GetFormID();
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].formIdStr = FormIDToString(reff->GetFormID());
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].name = name;
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].weight = reff->GetWeight();
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].gold = baseObj->GetGoldValue();
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].isCrime = isCrime;

												if (show_items_window_file) {
													items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].filename = baseObj->GetFile(0)->fileName;
												}
												tmpCountFOOD++;
											} else {
												if (tmpCountALCH > show_items_window_array_max_length) {
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

												bool isCrime = reff->IsCrimeToActivate();
												if (isCrimeIgnore) {
													if (isCrime) {
														continue;
													}
												}

												if (show_items_window_direction) {
													items[nowItemIndex].itemInfoALCH[tmpCountALCH].distance = distance;
													items[nowItemIndex].itemInfoALCH[tmpCountALCH].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
												}

												items[nowItemIndex].itemInfoALCH[tmpCountALCH].ptr = reff;
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].baseFormId = baseObj->GetFormID();
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].baseFormIdStr = FormIDToString(baseObj->GetFormID());
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].formId = reff->GetFormID();
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].formIdStr = FormIDToString(reff->GetFormID());
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].name = name;
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].weight = reff->GetWeight();
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].gold = baseObj->GetGoldValue();
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].isCrime = isCrime;

												if (show_items_window_file) {
													items[nowItemIndex].itemInfoALCH[tmpCountALCH].filename = baseObj->GetFile(0)->fileName;
												}
												tmpCountALCH++;
											}

											break;
										}
									case RE::FormType::Ingredient:
										{
											if (tmpCountINGR > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoINGR[tmpCountINGR].distance = distance;
												items[nowItemIndex].itemInfoINGR[tmpCountINGR].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}

											items[nowItemIndex].itemInfoINGR[tmpCountINGR].ptr = reff;
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].isCrime = isCrime;

											if (show_items_window_file) {
												items[nowItemIndex].itemInfoINGR[tmpCountINGR].filename = baseObj->GetFile(0)->fileName;
											}
											tmpCountINGR++;
											break;
										}
									case RE::FormType::Misc:
										{
											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											if (!baseObj->GetPlayable()) {
												continue;
											}

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											auto name = reff->GetDisplayFullName();
											if (strlen(name) == 0) {
												continue;
											}

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											// 判断宝石和矿锭

											auto misc = baseObj->As<RE::TESObjectMISC>();
											if (misc) {
												// 宝石
												if (FormUtil::HasKeyword(misc, VendorItemGem)) {
													if (tmpCountSTON > show_items_window_array_max_length) {
														continue;
													}

													if (show_items_window_direction) {
														items[nowItemIndex].itemInfoSTON[tmpCountSTON].distance = distance;
														items[nowItemIndex].itemInfoSTON[tmpCountSTON].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
													}

													items[nowItemIndex].itemInfoSTON[tmpCountSTON].ptr = reff;
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].baseFormId = baseObj->GetFormID();
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].baseFormIdStr = FormIDToString(baseObj->GetFormID());
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].formId = reff->GetFormID();
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].formIdStr = FormIDToString(reff->GetFormID());
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].name = name;
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].weight = reff->GetWeight();
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].gold = baseObj->GetGoldValue();
													items[nowItemIndex].itemInfoSTON[tmpCountSTON].isCrime = isCrime;

													if (show_items_window_file) {
														items[nowItemIndex].itemInfoSTON[tmpCountSTON].filename = baseObj->GetFile(0)->fileName;
													}
													tmpCountSTON++;

													// 矿锭
												} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
													if (tmpCountANVI > show_items_window_array_max_length) {
														continue;
													}

													if (show_items_window_direction) {
														items[nowItemIndex].itemInfoANVI[tmpCountANVI].distance = distance;
														items[nowItemIndex].itemInfoANVI[tmpCountANVI].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
													}

													items[nowItemIndex].itemInfoANVI[tmpCountANVI].ptr = reff;
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].baseFormId = baseObj->GetFormID();
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].baseFormIdStr = FormIDToString(baseObj->GetFormID());
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].formId = reff->GetFormID();
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].formIdStr = FormIDToString(reff->GetFormID());
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].name = name;
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].weight = reff->GetWeight();
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].gold = baseObj->GetGoldValue();
													items[nowItemIndex].itemInfoANVI[tmpCountANVI].isCrime = isCrime;

													if (show_items_window_file) {
														items[nowItemIndex].itemInfoANVI[tmpCountANVI].filename = baseObj->GetFile(0)->fileName;
													}
													tmpCountANVI++;
													// 毛皮
												} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
													if (tmpCountANHD > show_items_window_array_max_length) {
														continue;
													}

													if (show_items_window_direction) {
														items[nowItemIndex].itemInfoANHD[tmpCountANHD].distance = distance;
														items[nowItemIndex].itemInfoANHD[tmpCountANHD].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
													}

													items[nowItemIndex].itemInfoANHD[tmpCountANHD].ptr = reff;
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].baseFormId = baseObj->GetFormID();
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].baseFormIdStr = FormIDToString(baseObj->GetFormID());
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].formId = reff->GetFormID();
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].formIdStr = FormIDToString(reff->GetFormID());
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].name = name;
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].weight = reff->GetWeight();
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].gold = baseObj->GetGoldValue();
													items[nowItemIndex].itemInfoANHD[tmpCountANHD].isCrime = isCrime;

													if (show_items_window_file) {
														items[nowItemIndex].itemInfoANHD[tmpCountANHD].filename = baseObj->GetFile(0)->fileName;
													}
													tmpCountANHD++;

													// 组织
												} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
													if (tmpCountANPA > show_items_window_array_max_length) {
														continue;
													}

													if (show_items_window_direction) {
														items[nowItemIndex].itemInfoANPA[tmpCountANPA].distance = distance;
														items[nowItemIndex].itemInfoANPA[tmpCountANPA].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
													}

													items[nowItemIndex].itemInfoANPA[tmpCountANPA].ptr = reff;
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].baseFormId = baseObj->GetFormID();
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].baseFormIdStr = FormIDToString(baseObj->GetFormID());
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].formId = reff->GetFormID();
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].formIdStr = FormIDToString(reff->GetFormID());
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].name = name;
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].weight = reff->GetWeight();
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].gold = baseObj->GetGoldValue();
													items[nowItemIndex].itemInfoANPA[tmpCountANPA].isCrime = isCrime;

													if (show_items_window_file) {
														items[nowItemIndex].itemInfoANPA[tmpCountANPA].filename = baseObj->GetFile(0)->fileName;
													}
													tmpCountANPA++;
													// 工具
												} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
													if (tmpCountTOOL > show_items_window_array_max_length) {
														continue;
													}

													if (show_items_window_direction) {
														items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].distance = distance;
														items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
													}

													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].ptr = reff;
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].baseFormId = baseObj->GetFormID();
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].baseFormIdStr = FormIDToString(baseObj->GetFormID());
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].formId = reff->GetFormID();
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].formIdStr = FormIDToString(reff->GetFormID());
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].name = name;
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].weight = reff->GetWeight();
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].gold = baseObj->GetGoldValue();
													items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].isCrime = isCrime;

													if (show_items_window_file) {
														items[nowItemIndex].itemInfoTOOL[tmpCountTOOL].filename = baseObj->GetFile(0)->fileName;
													}
													tmpCountTOOL++;
												} else {
													if (tmpCountMISC > show_items_window_array_max_length) {
														continue;
													}

													if (show_items_window_direction) {
														items[nowItemIndex].itemInfoMISC[tmpCountMISC].distance = distance;
														items[nowItemIndex].itemInfoMISC[tmpCountMISC].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
													}

													items[nowItemIndex].itemInfoMISC[tmpCountMISC].ptr = reff;
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].baseFormId = baseObj->GetFormID();
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].baseFormIdStr = FormIDToString(baseObj->GetFormID());
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].formId = reff->GetFormID();
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].formIdStr = FormIDToString(reff->GetFormID());
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].name = name;
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].weight = reff->GetWeight();
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].gold = baseObj->GetGoldValue();
													items[nowItemIndex].itemInfoMISC[tmpCountMISC].isCrime = isCrime;

													if (show_items_window_file) {
														items[nowItemIndex].itemInfoMISC[tmpCountMISC].filename = baseObj->GetFile(0)->fileName;
													}
													tmpCountMISC++;
												}
											}

											break;
										}
									case RE::FormType::Container:
										{
											if (tmpCountCONT > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
											}

											if (merchantContIgnore) {
												if (merchantContFormIds.find(reff->GetFormID()) != merchantContFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoCONT[tmpCountCONT].ptr = reff;
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].isCrime = isCrime;
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].lockLevel = reff->GetLockLevel();
											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].distance = distance;
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].filename = baseObj->GetFile(0)->fileName;
											}
											auto owner = reff->GetOwner();
											if (owner) {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].ownerName = owner->GetName();
											} else {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].ownerName = "-";
											}

											/*auto factionOwner = reff->GetFactionOwner();
											if (factionOwner) {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].ownerName = factionOwner->GetFormID();
											} else {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].ownerName = "-";
											}*/

											int tmpInvCount = 0;
											auto inv = reff->GetInventory(FormUtil::CanDisplay);

											// 自动拾取条件1
											bool auto1 = false;
											if (show_items_window_auto_cont) {
												if (autoContFormIds.find(baseObj->GetFormID()) != autoContFormIds.end()) {
													auto1 = true;
												}
											}
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].isAuto = auto1;

											for (auto& [obj, data] : inv) {
												auto& [count, entry] = data;
												if (count > 0 && entry) {
#ifndef NDEBUG
													if (obj->IsIgnored()) {
														MessageBox(nullptr, obj->GetName(), nullptr, MB_OK);
													}
#endif

													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].ptr = obj;
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].name = obj->GetName();
													bool stealing = player->WouldBeStealing(reff);
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].isCrime = !entry.get()->IsOwnedBy(player, !stealing);
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].isEnchanted = entry.get()->IsEnchanted();
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount++].count = count;
													if (tmpInvCount == 200) {
														break;
													}
												}
											}

											items[nowItemIndex].itemInfoCONT[tmpCountCONT].invCount = tmpInvCount;

											tmpCountCONT++;

											break;
										}
									case RE::FormType::Flora:
										{
											if (tmpCountFLOR > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif
											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
											}

											auto flora = baseObj->As<RE::TESFlora>();
											if (flora) {
												if (!flora->produceItem) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].ptr = reff;
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].isCrime = isCrime;

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].distance = distance;
												items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].filename = baseObj->GetFile(0)->fileName;
											}

											//// 自动拾取
											//if ((show_items_window_auto && show_items_window_auto_flor)) {
											//	if (show_items_window_auto_ignore) {
											//		// 判断地点忽略
											//		int formID = 0;
											//		if (player->currentLocation) {
											//			formID = player->currentLocation->GetFormID();
											//		}
											//		if (excludeLocationFormIds.find(formID) != excludeLocationFormIds.end()) {
											//			continue;
											//		}
											//	}
											//	if (distance < show_items_window_auto_dis) {
											//		if (!reff->IsCrimeToActivate()) {
											//			if (!(reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
											//				if (flora) {
											//					flora->Activate(reff, player, 0, flora->produceItem, 1);
											//					char buf[40];
											//					snprintf(buf, 40, "%s 已自动收获", reff->GetDisplayFullName());
											//					RE::DebugNotification(buf, NULL, false);
											//				}
											//			}
											//		}
											//	}
											//}

											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].isHarvested = (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested);

											tmpCountFLOR++;

											break;
										}
									case RE::FormType::Tree:
										{
											if (tmpCountTREE > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif
											auto name = reff->GetDisplayFullName();
											if (strlen(name) == 0) {
												continue;
											}

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
											}

											auto tree = baseObj->As<RE::TESObjectTREE>();
											if (tree) {
												if (!tree->produceItem) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoTREE[tmpCountTREE].ptr = reff;
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].isCrime = isCrime;

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoTREE[tmpCountTREE].distance = distance;
												items[nowItemIndex].itemInfoTREE[tmpCountTREE].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoTREE[tmpCountTREE].filename = baseObj->GetFile(0)->fileName;
											}

											items[nowItemIndex].itemInfoTREE[tmpCountTREE].isHarvested = (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested);

											tmpCountTREE++;

											break;
										}
									case RE::FormType::KeyMaster:
										{
											if (tmpCountKEYM > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif
											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].ptr = reff;
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].isCrime = isCrime;

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].distance = distance;
												items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].filename = baseObj->GetFile(0)->fileName;
											}

											tmpCountKEYM++;
											break;
										}
									case RE::FormType::Activator:
										{
											if (tmpCountACTI > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											auto name = reff->GetDisplayFullName();
											if (strlen(name) == 0) {
												continue;
											}

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfoACTI[tmpCountACTI].ptr = reff;
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].isCrime = isCrime;

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoACTI[tmpCountACTI].distance = distance;
												items[nowItemIndex].itemInfoACTI[tmpCountACTI].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoACTI[tmpCountACTI].filename = baseObj->GetFile(0)->fileName;
											}

											items[nowItemIndex].itemInfoACTI[tmpCountACTI].isHarvested = (reff->formFlags & RE::TESForm::RecordFlags::kDestroyed);
											// 分类
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].formTypeStr = FormUtil::GetActiTypeName(reff, baseObj);

											tmpCountACTI++;

											break;
										}
									case RE::FormType::SoulGem:
										{
											if (tmpCountSGEM > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].distance = distance;
												items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].filename = baseObj->GetFile(0)->fileName;
											}

											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].ptr = reff;
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].formId = reff->GetFormID();
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].name = name;
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].weight = reff->GetWeight();
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].gold = baseObj->GetGoldValue();
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].isCrime = isCrime;

											tmpCountSGEM++;

											break;
										}
									case RE::FormType::Static:
									case RE::FormType::Furniture:
									case RE::FormType::IdleMarker:
									case RE::FormType::Light:
									case RE::FormType::MovableStatic:
									case RE::FormType::Door:
									case RE::FormType::TextureSet:
									case RE::FormType::Sound:
									case RE::FormType::Explosion:
									case RE::FormType::AcousticSpace:  // 升学空间
									case RE::FormType::TalkingActivator:
									case RE::FormType::Apparatus:  // 设备 暂时去掉
										continue;
									default:
										{
											if (tmpCount > show_items_window_array_max_length) {
												continue;
											}

											if (reff->IsMarkedForDeletion()) { /*
										logger::debug(StringUtil::Utf8ToGbk(reff->GetDisplayFullName()) + " " + GetFormTypeName(baseObj->formType.underlying()));
										logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));*/
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

											if (show_items_window_ignore) {
												if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
													continue;
												}
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

											bool isCrime = reff->IsCrimeToActivate();
											if (isCrimeIgnore) {
												if (isCrime) {
													continue;
												}
											}

											items[nowItemIndex].itemInfo[tmpCount].ptr = reff;
											items[nowItemIndex].itemInfo[tmpCount].baseFormId = baseObj->GetFormID();
											items[nowItemIndex].itemInfo[tmpCount].baseFormIdStr = FormIDToString(baseObj->GetFormID());
											items[nowItemIndex].itemInfo[tmpCount].formId = reff->GetFormID();
											items[nowItemIndex].itemInfo[tmpCount].formIdStr = FormIDToString(reff->GetFormID());
											items[nowItemIndex].itemInfo[tmpCount].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
											items[nowItemIndex].itemInfo[tmpCount].name = reff->GetDisplayFullName();
											items[nowItemIndex].itemInfo[tmpCount].weight = reff->GetWeight();
											items[nowItemIndex].itemInfo[tmpCount].gold = baseObj->GetGoldValue();
											//items[nowItemIndex].itemInfo[tmpCount].lockLevel = reff->GetLockLevel();
											items[nowItemIndex].itemInfo[tmpCount].isCrime = isCrime;

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfo[tmpCount].distance = distance;
												items[nowItemIndex].itemInfo[tmpCount].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfo[tmpCount].filename = baseObj->GetFile(0)->fileName;
											}

											//logger::debug(GetFormTypeName(baseObj->formType.underlying()));
											//logger::debug(std::to_string(tmpCount) + " " + StringUtil::Utf8ToGbk(reff->GetDisplayFullName()));
											//logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));

											tmpCount++;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		std::sort(items[nowItemIndex].itemInfo, items[nowItemIndex].itemInfo + tmpCount, compareForItem);
		std::sort(items[nowItemIndex].itemInfoWEAP, items[nowItemIndex].itemInfoWEAP + tmpCountWEAP, compareForItem);
		std::sort(items[nowItemIndex].itemInfoARMO, items[nowItemIndex].itemInfoARMO + tmpCountARMO, compareForItem);
		std::sort(items[nowItemIndex].itemInfoAMMO, items[nowItemIndex].itemInfoAMMO + tmpCountAMMO, compareForItem);
		std::sort(items[nowItemIndex].itemInfoBOOK, items[nowItemIndex].itemInfoBOOK + tmpCountBOOK, compareForItem);
		std::sort(items[nowItemIndex].itemInfoALCH, items[nowItemIndex].itemInfoALCH + tmpCountALCH, compareForItem);
		std::sort(items[nowItemIndex].itemInfoINGR, items[nowItemIndex].itemInfoINGR + tmpCountINGR, compareForItem);
		std::sort(items[nowItemIndex].itemInfoMISC, items[nowItemIndex].itemInfoMISC + tmpCountMISC, compareForItem);
		std::sort(items[nowItemIndex].itemInfoCONT, items[nowItemIndex].itemInfoCONT + tmpCountCONT, compareForItemCONT);
		std::sort(items[nowItemIndex].itemInfoFLOR, items[nowItemIndex].itemInfoFLOR + tmpCountFLOR, compareForItem);
		std::sort(items[nowItemIndex].itemInfoFOOD, items[nowItemIndex].itemInfoFOOD + tmpCountFOOD, compareForItem);
		std::sort(items[nowItemIndex].itemInfoKEYM, items[nowItemIndex].itemInfoKEYM + tmpCountKEYM, compareForItem);
		std::sort(items[nowItemIndex].itemInfoTREE, items[nowItemIndex].itemInfoTREE + tmpCountTREE, compareForItem);
		std::sort(items[nowItemIndex].itemInfoSGEM, items[nowItemIndex].itemInfoSGEM + tmpCountSGEM, compareForItem);
		std::sort(items[nowItemIndex].itemInfoACTI, items[nowItemIndex].itemInfoACTI + tmpCountACTI, compareForItem);
		std::sort(items[nowItemIndex].itemInfoSTON, items[nowItemIndex].itemInfoSTON + tmpCountSTON, compareForItem);
		std::sort(items[nowItemIndex].itemInfoANVI, items[nowItemIndex].itemInfoANVI + tmpCountANVI, compareForItem);
		std::sort(items[nowItemIndex].itemInfoACHR, items[nowItemIndex].itemInfoACHR + tmpCountACHR, compareForItemCONT);
		std::sort(items[nowItemIndex].itemInfoANHD, items[nowItemIndex].itemInfoANHD + tmpCountANHD, compareForItem);
		std::sort(items[nowItemIndex].itemInfoANPA, items[nowItemIndex].itemInfoANPA + tmpCountANPA, compareForItem);
		std::sort(items[nowItemIndex].itemInfoTOOL, items[nowItemIndex].itemInfoTOOL + tmpCountTOOL, compareForItem);

		items[nowItemIndex].itemCount = tmpCount;
		items[nowItemIndex].itemCountWEAP = tmpCountWEAP;
		items[nowItemIndex].itemCountARMO = tmpCountARMO;
		items[nowItemIndex].itemCountAMMO = tmpCountAMMO;
		items[nowItemIndex].itemCountBOOK = tmpCountBOOK;
		items[nowItemIndex].itemCountALCH = tmpCountALCH;
		items[nowItemIndex].itemCountINGR = tmpCountINGR;
		items[nowItemIndex].itemCountMISC = tmpCountMISC;
		items[nowItemIndex].itemCountCONT = tmpCountCONT;
		items[nowItemIndex].itemCountFLOR = tmpCountFLOR;
		items[nowItemIndex].itemCountFOOD = tmpCountFOOD;
		items[nowItemIndex].itemCountKEYM = tmpCountKEYM;
		items[nowItemIndex].itemCountTREE = tmpCountTREE;
		items[nowItemIndex].itemCountSGEM = tmpCountSGEM;
		items[nowItemIndex].itemCountACTI = tmpCountACTI;
		items[nowItemIndex].itemCountSTON = tmpCountSTON;
		items[nowItemIndex].itemCountANVI = tmpCountANVI;
		items[nowItemIndex].itemCountACHR = tmpCountACHR;
		items[nowItemIndex].itemCountANHD = tmpCountANHD;
		items[nowItemIndex].itemCountANPA = tmpCountANPA;
		items[nowItemIndex].itemCountTOOL = tmpCountTOOL;

		nowItemIndex = !nowItemIndex;
	}
}
