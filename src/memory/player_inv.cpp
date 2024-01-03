#include "player_inv.h"
#include <utils/GeneralUtil.h>




int nowIndexPI = 0;
bool show_inv_window = false;

PlayerInventoryInfo* MyInventoryInfo = new PlayerInventoryInfo[2];

int getPlayerGoldCount()
{
	return MyInventoryInfo[!nowIndexPI].gold;
}
int getPlayerInvCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryCount;
}
int getPlayerInvBOOKCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryBOOKCount;
}
int getPlayerInvWEAPCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryWEAPCount;
}
int getPlayerInvARMOCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryARMOCount;
}
int getPlayerInvAMMOCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryAMMOCount;
}
int getPlayerInvFOODCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryFOODCount;
}
int getPlayerInvALCHCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryALCHCount;
}
int getPlayerInvINGRCount()
{
	return MyInventoryInfo[!nowIndexPI].inventoryINGRCount;
}

InventoryInfo* getPlayerInvData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorys[0];
}
InventoryInfo* getPlayerInvARMOData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysARMO[0];
}
InventoryInfo* getPlayerInvWEAPData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysWEAP[0];
}
InventoryInfo* getPlayerInvBOOKData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysBOOK[0];
}
InventoryInfo* getPlayerInvAMMOData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysAMMO[0];
}
InventoryInfo* getPlayerInvFOODData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysFOOD[0];
}
InventoryInfo* getPlayerInvALCHData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysALCH[0];
}
InventoryInfo* getPlayerInvINGRData()
{
	return &MyInventoryInfo[!nowIndexPI].inventorysINGR[0];
}

InventoryInfo* getPlayerInvData(int i)
{
	return &MyInventoryInfo[!nowIndexPI].inventorys[i];
}


bool compareForInventory(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.isWorn != info2.isWorn) {
		return info1.isWorn > info2.isWorn;
	} else {
		return info1.formId < info2.formId;
	}
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



void RefreshPlayerInvInfo() {

	if (show_inv_window) {
		// 刷新自己的装备
		auto player = RE::PlayerCharacter::GetSingleton();
		const auto inv = player->GetInventory();
		MyInventoryInfo[nowIndexPI].inventoryARMOCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryBOOKCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryWEAPCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryAMMOCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryALCHCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryFOODCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryINGRCount = 0;
		MyInventoryInfo[nowIndexPI].inventoryCount = 0;
		MyInventoryInfo[nowIndexPI].gold = 0;

		for (const auto& [item, invData] : inv) {
			const auto& [count, entry] = invData;
			//if (count > 0 && entry->IsWorn()) {
			//const auto armor = item->As<RE::TESObjectARMO>();
			if (count > 0) {
				//actorInfo[tmpIndex].Inventorys[i].formIdStr = FormIDToString(armor->GetFormID());
				if (item->GetWeight() >= 0) {
					if (item->IsGold()) {
						MyInventoryInfo[nowIndexPI].gold = count;
					} else if (item->IsArmor()) {
						buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysARMO, MyInventoryInfo[nowIndexPI].inventoryARMOCount, item, entry.get(), count);
					} else if (item->IsWeapon()) {
						buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysWEAP, MyInventoryInfo[nowIndexPI].inventoryWEAPCount, item, entry.get(), count);
					} else if (item->IsAmmo()) {
						buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysAMMO, MyInventoryInfo[nowIndexPI].inventoryAMMOCount, item, entry.get(), count);
					} else if (item->IsBook()) {
						buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysBOOK, MyInventoryInfo[nowIndexPI].inventoryBOOKCount, item, entry.get(), count);
					} else if (item->GetFormType() == RE::FormType::AlchemyItem) {
						auto alchemyItem = item->As<RE::AlchemyItem>();
						if (alchemyItem->IsFood()) {
							buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysFOOD, MyInventoryInfo[nowIndexPI].inventoryFOODCount, item, entry.get(), count);
						} else {
							buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysALCH, MyInventoryInfo[nowIndexPI].inventoryALCHCount, item, entry.get(), count);
						}
					} else if (item->GetFormType() == RE::FormType::Ingredient) {
						buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorysINGR, MyInventoryInfo[nowIndexPI].inventoryINGRCount, item, entry.get(), count);
					} else {
						buildPlayerInvData(MyInventoryInfo[nowIndexPI].inventorys, MyInventoryInfo[nowIndexPI].inventoryCount, item, entry.get(), count);
					}
				}
			}
		}

		if (MyInventoryInfo[nowIndexPI].inventoryARMOCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysARMO, MyInventoryInfo[nowIndexPI].inventorysARMO + MyInventoryInfo[nowIndexPI].inventoryARMOCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryWEAPCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysWEAP, MyInventoryInfo[nowIndexPI].inventorysWEAP + MyInventoryInfo[nowIndexPI].inventoryWEAPCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryBOOKCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysBOOK, MyInventoryInfo[nowIndexPI].inventorysBOOK + MyInventoryInfo[nowIndexPI].inventoryBOOKCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorys, MyInventoryInfo[nowIndexPI].inventorys + MyInventoryInfo[nowIndexPI].inventoryCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryAMMOCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysAMMO, MyInventoryInfo[nowIndexPI].inventorysAMMO + MyInventoryInfo[nowIndexPI].inventoryAMMOCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryALCHCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysALCH, MyInventoryInfo[nowIndexPI].inventorysALCH + MyInventoryInfo[nowIndexPI].inventoryALCHCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryFOODCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysFOOD, MyInventoryInfo[nowIndexPI].inventorysFOOD + MyInventoryInfo[nowIndexPI].inventoryFOODCount, compareForInventory);
		}
		if (MyInventoryInfo[nowIndexPI].inventoryINGRCount > 1) {
			std::sort(MyInventoryInfo[nowIndexPI].inventorysINGR, MyInventoryInfo[nowIndexPI].inventorysINGR + MyInventoryInfo[nowIndexPI].inventoryINGRCount, compareForInventory);
		}

		// 双缓冲可以不用
		//MyInventoryInfo[nowIndexPI].inventoryCount = i;
	}



}


