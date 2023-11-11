#include <event/BSTMenuEvent.h>
#include <memory/npc.h>
#include <utils/GeneralUtil.h>
#include <utils/utils.h>

bool show_npc_window_dead_hidden = false;
int show_npc_window_dis_meter = 30;
bool show_npc_window_dis = false;
bool show_npc_window_direction = false;
int show_npc_window_array_max_length = 98;
bool show_enemy_window = false;
bool show_inv_window = false;
bool show_npc_window = false;
bool show_npc_window_ignore = false;
bool show_npc_window_process = false;
bool show_npc_window_process_combat = true;


ImVec4 colorProgressNpc1(0.0f, 0.8f, 0.0f, 1.0f);
ImVec4 colorProgressNpc2(1.0f, 0.5f, 0.0f, 1.0f);
ImVec4 colorProgressNpc3(0.8f, 0.0f, 0.0f, 1.0f);

std::unordered_set<int> excludeNpcFormIds;
std::vector<ExcludeForm> excludeNpcForms;

Actor2Info* actorInfo = new Actor2Info[2];
int nowIndex = 0;

ActorInfo* getNpcData()
{
	return &actorInfo[!nowIndex].npcInfo[0];
}
ActorInfo* getHorseData()
{
	return &actorInfo[!nowIndex].horseInfo[0];
}
ActorInfo* getEnemy2Data()
{
	return &actorInfo[!nowIndex].enemyInfo[0];
}
ActorInfo* getTeammateData()
{
	return &actorInfo[!nowIndex].teammateInfo[0];
}

int getNpcCount()
{
	return actorInfo[!nowIndex].npcCount;
}
int getEnemyCount()
{
	return actorInfo[!nowIndex].enemyCount;
}
int getHorseCount()
{
	return actorInfo[!nowIndex].horseCount;
}
int getTeammateCount()
{
	return actorInfo[!nowIndex].teammateCount;
}

auto IsSentient2(RE::Actor* actor) -> uint32_t
{
	using func_t = decltype(&IsSentient2);
	REL::Relocation<func_t> func{ REL::ID(36889) };
	return func(actor);
}

bool compareByLevel(const ActorInfo& info1, const ActorInfo& info2)
{
	if (info1.isDead != info2.isDead) {
		return info1.isDead < info2.isDead;
	} else if (info1.level != info2.level) {
		return info1.level > info2.level;
	} else {
		return info1.formId < info2.formId;
	}
}

bool compareForInventory(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.isWorn != info2.isWorn) {
		return info1.isWorn > info2.isWorn;
	} else {
		return info1.formId < info2.formId;
	}
}

PlayerInventoryInfo* MyInventoryInfo = new PlayerInventoryInfo[2];

int getPlayerGoldCount()
{
	return MyInventoryInfo[!nowIndex].gold;
}
int getPlayerInvCount()
{
	return MyInventoryInfo[!nowIndex].inventoryCount;
}
int getPlayerInvBOOKCount()
{
	return MyInventoryInfo[!nowIndex].inventoryBOOKCount;
}
int getPlayerInvWEAPCount()
{
	return MyInventoryInfo[!nowIndex].inventoryWEAPCount;
}
int getPlayerInvARMOCount()
{
	return MyInventoryInfo[!nowIndex].inventoryARMOCount;
}
int getPlayerInvAMMOCount()
{
	return MyInventoryInfo[!nowIndex].inventoryAMMOCount;
}
int getPlayerInvFOODCount()
{
	return MyInventoryInfo[!nowIndex].inventoryFOODCount;
}
int getPlayerInvALCHCount()
{
	return MyInventoryInfo[!nowIndex].inventoryALCHCount;
}
int getPlayerInvINGRCount()
{
	return MyInventoryInfo[!nowIndex].inventoryINGRCount;
}

InventoryInfo* getPlayerInvData()
{
	return &MyInventoryInfo[!nowIndex].inventorys[0];
}
InventoryInfo* getPlayerInvARMOData()
{
	return &MyInventoryInfo[!nowIndex].inventorysARMO[0];
}
InventoryInfo* getPlayerInvWEAPData()
{
	return &MyInventoryInfo[!nowIndex].inventorysWEAP[0];
}
InventoryInfo* getPlayerInvBOOKData()
{
	return &MyInventoryInfo[!nowIndex].inventorysBOOK[0];
}
InventoryInfo* getPlayerInvAMMOData()
{
	return &MyInventoryInfo[!nowIndex].inventorysAMMO[0];
}
InventoryInfo* getPlayerInvFOODData()
{
	return &MyInventoryInfo[!nowIndex].inventorysFOOD[0];
}
InventoryInfo* getPlayerInvALCHData()
{
	return &MyInventoryInfo[!nowIndex].inventorysALCH[0];
}
InventoryInfo* getPlayerInvINGRData()
{
	return &MyInventoryInfo[!nowIndex].inventorysINGR[0];
}

InventoryInfo* getPlayerInvData(int i)
{
	return &MyInventoryInfo[!nowIndex].inventorys[i];
}

void __fastcall buildPlayerInvData(InventoryInfo inv[], int& i, RE::TESBoundObject* item, RE::InventoryEntryData* entry, std::int32_t count)
{
	// 要判断extras

	if (entry->extraLists) {
		for (auto& xList : *entry->extraLists) {
			if (xList) {
				auto xCount = xList->GetCount();

				//logger::trace("Inv Name {} {} "sv, i, StringUtil::Utf8ToGbk(entry->GetDisplayName()));
				//logger::trace("Inv2 Name {} {} "sv, xCount, StringUtil::Utf8ToGbk(xList->GetDisplayName(item)));

				inv[i].ptr = item;
				inv[i].invPtr = entry;
				inv[i].invExtraPtr = xList;
				inv[i].count = xCount;
				inv[i].formId = item->GetFormID();
				inv[i].formIdStr = FormIDToString(item->GetFormID());
				inv[i].name = xList->GetDisplayName(item);
				inv[i].weight = item->GetWeight();
				inv[i++].isWorn = (xList->HasType<RE::ExtraWorn>() || xList->HasType<RE::ExtraWornLeft>());

				count -= xCount;
			}
		}
	}
	if (count > 0) {
		inv[i].ptr = item;
		inv[i].invPtr = entry;
		inv[i].invExtraPtr = nullptr;
		inv[i].count = count;
		inv[i].formId = item->GetFormID();
		inv[i].formIdStr = FormIDToString(item->GetFormID());
		//inv[i].name = entry->GetDisplayName();
		inv[i].name = item->GetName();

		inv[i].weight = item->GetWeight();
		//inv[i++].isWorn = entry->IsWorn();
		inv[i++].isWorn = false;
	}

	//logger::trace("Inv Name {} {} "sv, i, StringUtil::Utf8ToGbk(entry->GetDisplayName()));
}

void RefreshInventory(RE::Actor* actor, ActorInfo* actorInfo, int tmpIndex)
{
	int i = 0;

	// 装备信息
	//const auto inv = actor->GetInventory([](RE::TESBoundObject& a_object) {
	//	//return !a_object.IsDynamicForm() && !a_object.IsDeleted() && !a_object.IsIgnored();
	//	return a_object.IsArmor();
	//});

	const auto inv = actor->GetInventory();

	for (const auto& [item, invData] : inv) {
		const auto& [count, entry] = invData;
		//if (count > 0 && entry->IsWorn()) {
		if (count > 0) {
			if (item->GetWeight() >= 0) {
				actorInfo[tmpIndex].Inventorys[i].ptr = item;
				actorInfo[tmpIndex].Inventorys[i].count = count;
				actorInfo[tmpIndex].Inventorys[i].formId = item->GetFormID();
				actorInfo[tmpIndex].Inventorys[i].formIdStr = FormIDToString(item->GetFormID());
				actorInfo[tmpIndex].Inventorys[i].name = entry.get()->GetDisplayName();
				//actorInfo[tmpIndex].Inventorys[i].name = armor->GetFullName();
				actorInfo[tmpIndex].Inventorys[i].weight = item->GetWeight();
				actorInfo[tmpIndex].Inventorys[i++].isWorn = entry.get()->IsWorn();
			}
		}
	}

	if (i > 1) {
		std::sort(actorInfo[tmpIndex].Inventorys, actorInfo[tmpIndex].Inventorys + i, compareForInventory);
	}

	actorInfo[tmpIndex].inventoryCount = i;
}

void __cdecl RefreshActorInfo(void*)
{
	while (true) {
		Sleep(500);

		if (!startflag) {
			Sleep(3000);
			continue;
		}

		if (isGameLoading) {
			Sleep(3000);
			continue;
		}

		if (isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu) {
			continue;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			Sleep(3000);
			continue;
		}
		auto pl = RE::ProcessLists::GetSingleton();

		nowIndex = !nowIndex;
		Sleep(100);

		{
			if (show_inv_window) {
				// 刷新自己的装备
				const auto inv = player->GetInventory();
				MyInventoryInfo[nowIndex].inventoryARMOCount = 0;
				MyInventoryInfo[nowIndex].inventoryBOOKCount = 0;
				MyInventoryInfo[nowIndex].inventoryWEAPCount = 0;
				MyInventoryInfo[nowIndex].inventoryAMMOCount = 0;
				MyInventoryInfo[nowIndex].inventoryALCHCount = 0;
				MyInventoryInfo[nowIndex].inventoryFOODCount = 0;
				MyInventoryInfo[nowIndex].inventoryINGRCount = 0;
				MyInventoryInfo[nowIndex].inventoryCount = 0;
				MyInventoryInfo[nowIndex].gold = 0;

				for (const auto& [item, invData] : inv) {
					const auto& [count, entry] = invData;
					//if (count > 0 && entry->IsWorn()) {
					//const auto armor = item->As<RE::TESObjectARMO>();
					if (count > 0) {
						//actorInfo[tmpIndex].Inventorys[i].formIdStr = FormIDToString(armor->GetFormID());
						if (item->GetWeight() >= 0) {
							if (item->IsGold()) {
								MyInventoryInfo[nowIndex].gold = count;
							} else if (item->IsArmor()) {
								buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysARMO, MyInventoryInfo[nowIndex].inventoryARMOCount, item, entry.get(), count);
							} else if (item->IsWeapon()) {
								buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysWEAP, MyInventoryInfo[nowIndex].inventoryWEAPCount, item, entry.get(), count);
							} else if (item->IsAmmo()) {
								buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysAMMO, MyInventoryInfo[nowIndex].inventoryAMMOCount, item, entry.get(), count);
							} else if (item->IsBook()) {
								buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysBOOK, MyInventoryInfo[nowIndex].inventoryBOOKCount, item, entry.get(), count);
							} else if (item->GetFormType() == RE::FormType::AlchemyItem) {
								auto alchemyItem = item->As<RE::AlchemyItem>();
								if (alchemyItem->IsFood()) {
									buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysFOOD, MyInventoryInfo[nowIndex].inventoryFOODCount, item, entry.get(), count);
								} else {
									buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysALCH, MyInventoryInfo[nowIndex].inventoryALCHCount, item, entry.get(), count);
								}
							} else if (item->GetFormType() == RE::FormType::Ingredient) {
								buildPlayerInvData(MyInventoryInfo[nowIndex].inventorysINGR, MyInventoryInfo[nowIndex].inventoryINGRCount, item, entry.get(), count);
							} else {
								buildPlayerInvData(MyInventoryInfo[nowIndex].inventorys, MyInventoryInfo[nowIndex].inventoryCount, item, entry.get(), count);
							}
						}
					}
				}

				if (MyInventoryInfo[nowIndex].inventoryARMOCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysARMO, MyInventoryInfo[nowIndex].inventorysARMO + MyInventoryInfo[nowIndex].inventoryARMOCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryWEAPCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysWEAP, MyInventoryInfo[nowIndex].inventorysWEAP + MyInventoryInfo[nowIndex].inventoryWEAPCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryBOOKCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysBOOK, MyInventoryInfo[nowIndex].inventorysBOOK + MyInventoryInfo[nowIndex].inventoryBOOKCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorys, MyInventoryInfo[nowIndex].inventorys + MyInventoryInfo[nowIndex].inventoryCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryAMMOCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysAMMO, MyInventoryInfo[nowIndex].inventorysAMMO + MyInventoryInfo[nowIndex].inventoryAMMOCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryALCHCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysALCH, MyInventoryInfo[nowIndex].inventorysALCH + MyInventoryInfo[nowIndex].inventoryALCHCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryFOODCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysFOOD, MyInventoryInfo[nowIndex].inventorysFOOD + MyInventoryInfo[nowIndex].inventoryFOODCount, compareForInventory);
				}
				if (MyInventoryInfo[nowIndex].inventoryINGRCount > 1) {
					std::sort(MyInventoryInfo[nowIndex].inventorysINGR, MyInventoryInfo[nowIndex].inventorysINGR + MyInventoryInfo[nowIndex].inventoryINGRCount, compareForInventory);
				}

				// 双缓冲可以不用
				//MyInventoryInfo[nowIndex].inventoryCount = i;
			}
		}

		if (show_npc_window) {
			int tmpNpcCount = 0;
			int tmpEnemyCount = 0;
			int tmpTeammateCount = 0;
			int tmpHorseCount = 0;

			for (auto& handle : pl->highActorHandles) {
				//for (int i = 0; i < actorCount; i++) {
				auto actor = handle.get().get();
				if (actor) {
					if (actor->IsPlayerTeammate()) {
						if (tmpTeammateCount > show_npc_window_array_max_length) {
							continue;
						}

						auto baseObj = actor->GetActorBase();
						if (baseObj) {
							if (show_npc_window_ignore) {
								if (excludeNpcFormIds.find(baseObj->GetFormID()) != excludeNpcFormIds.end()) {
									continue;
								}
							}
							actorInfo[nowIndex].teammateInfo[tmpTeammateCount].baseFormId = baseObj->GetFormID();
							//baseObj->GetRace();
						}

						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].formId = actor->GetFormID();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].ptr = actor;
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].level = actor->GetLevel();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].isSentient = IsSentient2(actor);

						if (show_npc_window_direction) {
							actorInfo[nowIndex].teammateInfo[tmpTeammateCount].distance = ValueUtil::calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
							actorInfo[nowIndex].teammateInfo[tmpTeammateCount].direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].isInCombat = actor->IsInCombat();

						//teammateInfo[i].idHostile = actor->IsHostileToActor(player);
						RefreshInventory(actor, actorInfo[nowIndex].teammateInfo, tmpTeammateCount++);

					} else if (actor->IsHostileToActor(player)) {
						if (tmpEnemyCount > show_npc_window_array_max_length) {
							continue;
						}

						if (show_npc_window_dead_hidden) {
							if (actor->GetLifeState() == RE::ACTOR_LIFE_STATE::kDead) {
								continue;
							}
						}
						float dis = ValueUtil::calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}

						auto baseObj = actor->GetActorBase();
						if (baseObj) {
							if (show_npc_window_ignore) {
								if (excludeNpcFormIds.find(baseObj->GetFormID()) != excludeNpcFormIds.end()) {
									continue;
								}
							}
							actorInfo[nowIndex].enemyInfo[tmpEnemyCount].baseFormId = baseObj->GetFormID();
						}

						if (show_npc_window_direction) {
							actorInfo[nowIndex].enemyInfo[tmpEnemyCount].distance = dis;
							actorInfo[nowIndex].enemyInfo[tmpEnemyCount].direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].formId = actor->GetFormID();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].ptr = actor;
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].level = actor->GetLevel();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].isInCombat = actor->IsInCombat();

						RefreshInventory(actor, actorInfo[nowIndex].enemyInfo, tmpEnemyCount++);

					} else if (actor->IsHorse()) {
						if (tmpHorseCount > show_npc_window_array_max_length) {
							continue;
						}

						if (show_npc_window_dead_hidden) {
							if (actor->GetLifeState() == RE::ACTOR_LIFE_STATE::kDead) {
								continue;
							}
						}
						float dis = ValueUtil::calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}

						auto baseObj = actor->GetActorBase();
						if (baseObj) {
							if (show_npc_window_ignore) {
								if (excludeNpcFormIds.find(baseObj->GetFormID()) != excludeNpcFormIds.end()) {
									continue;
								}
							}
							actorInfo[nowIndex].horseInfo[tmpHorseCount].baseFormId = baseObj->GetFormID();
						}

						if (show_npc_window_direction) {
							actorInfo[nowIndex].horseInfo[tmpHorseCount].distance = dis;
							actorInfo[nowIndex].horseInfo[tmpHorseCount].direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
						actorInfo[nowIndex].horseInfo[tmpHorseCount].formId = actor->GetFormID();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].horseInfo[tmpHorseCount].ptr = actor;
						actorInfo[nowIndex].horseInfo[tmpHorseCount].level = actor->GetLevel();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].horseInfo[tmpHorseCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].horseInfo[tmpHorseCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].horseInfo[tmpHorseCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].isInCombat = actor->IsInCombat();

						RefreshInventory(actor, actorInfo[nowIndex].horseInfo, tmpHorseCount++);

					} else {
						if (tmpNpcCount > show_npc_window_array_max_length) {
							continue;
						}

						if (show_npc_window_dead_hidden) {
							if (actor->GetLifeState() == RE::ACTOR_LIFE_STATE::kDead) {
								continue;
							}
						}
						float dis = ValueUtil::calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}

						auto baseObj = actor->GetActorBase();
						if (baseObj) {
							if (show_npc_window_ignore) {
								if (excludeNpcFormIds.find(baseObj->GetFormID()) != excludeNpcFormIds.end()) {
									continue;
								}
							}
							actorInfo[nowIndex].npcInfo[tmpNpcCount].baseFormId = baseObj->GetFormID();
						}

						if (show_npc_window_direction) {
							actorInfo[nowIndex].npcInfo[tmpNpcCount].distance = dis;
							actorInfo[nowIndex].npcInfo[tmpNpcCount].direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
						actorInfo[nowIndex].npcInfo[tmpNpcCount].formId = actor->GetFormID();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].npcInfo[tmpNpcCount].ptr = actor;
						actorInfo[nowIndex].npcInfo[tmpNpcCount].level = actor->GetLevel();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].isInCombat = actor->IsInCombat();

						RefreshInventory(actor, actorInfo[nowIndex].npcInfo, tmpNpcCount++);
					}
				}
			}

			std::sort(actorInfo[nowIndex].teammateInfo, actorInfo[nowIndex].teammateInfo + tmpTeammateCount, compareByLevel);
			std::sort(actorInfo[nowIndex].enemyInfo, actorInfo[nowIndex].enemyInfo + tmpEnemyCount, compareByLevel);
			std::sort(actorInfo[nowIndex].horseInfo, actorInfo[nowIndex].horseInfo + tmpHorseCount, compareByLevel);
			std::sort(actorInfo[nowIndex].npcInfo, actorInfo[nowIndex].npcInfo + tmpNpcCount, compareByLevel);

			actorInfo[nowIndex].horseCount = tmpHorseCount;
			actorInfo[nowIndex].npcCount = tmpNpcCount;
			actorInfo[nowIndex].enemyCount = tmpEnemyCount;
			actorInfo[nowIndex].teammateCount = tmpTeammateCount;
		}
	}
}


RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm)
{
	RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
	auto npcRoot = npc;
	while (npcRoot->faceNPC && npcRoot->formID >= 0xFF000000) {
		npcRoot = npcRoot->faceNPC;
	}

	return npcRoot;
}
