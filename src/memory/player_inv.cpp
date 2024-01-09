#include "player_inv.h"
#include <fonts/IconsMaterialDesignIcons.h>
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
int getPlayerInvAlchemyCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysAlchemyCount;
}
int getPlayerInvSmithingCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithingCount;
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

std::vector<InventoryInfo>& getPlayerInvAlchemy()
{
	return MyInventoryInfo[!nowIndexPI].inventorysAlchemy;
}
std::vector<InventoryInfo>& getPlayerInvSmithing()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithing;
}

bool compareForInventory(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.isWorn != info2.isWorn) {
		return info1.isWorn > info2.isWorn;
	} else {
		return info1.formId < info2.formId;
	}
}

void buildPlayerAlchemy(InventoryInfo inv[], std::int32_t count)
{
	auto& listAlchemy = MyInventoryInfo[nowIndexPI].inventorysAlchemy;
	auto& listSmithing = MyInventoryInfo[nowIndexPI].inventorysSmithing;
	int tmpCountAlchemy = 0;
	int tmpCountSmithing = 0;

	for (int i = 0; i < count; i++) {
		auto& item = inv[i];
		if (item.isAlchemyEnt) {

			if (tmpCountAlchemy + 1 > listAlchemy.size()) {
				listAlchemy.resize(listAlchemy.size() + 20);
			}

			listAlchemy[tmpCountAlchemy].ptr = inv[i].ptr;
			listAlchemy[tmpCountAlchemy].invPtr = inv[i].invPtr;
			listAlchemy[tmpCountAlchemy].invExtraPtr = inv[i].invExtraPtr;
			listAlchemy[tmpCountAlchemy].count = inv[i].count;
			listAlchemy[tmpCountAlchemy].formId = inv[i].formId;
			listAlchemy[tmpCountAlchemy].name = inv[i].name;
			listAlchemy[tmpCountAlchemy].uniqueID = inv[i].uniqueID;
			listAlchemy[tmpCountAlchemy].isWorn = inv[i].isWorn;
			listAlchemy[tmpCountAlchemy].weight = inv[i].weight;
			tmpCountAlchemy++;

		} else if (item.isSmithingEnt) {
			if (tmpCountSmithing + 1 > listSmithing.size()) {
				listSmithing.resize(listSmithing.size() + 20);
			}
			listSmithing[tmpCountSmithing].ptr = inv[i].ptr;
			listSmithing[tmpCountSmithing].invPtr = inv[i].invPtr;
			listSmithing[tmpCountSmithing].invExtraPtr = inv[i].invExtraPtr;
			listSmithing[tmpCountSmithing].count = inv[i].count;
			listSmithing[tmpCountSmithing].formId = inv[i].formId;
			listSmithing[tmpCountSmithing].name = inv[i].name;
			listSmithing[tmpCountSmithing].uniqueID = inv[i].uniqueID;
			listSmithing[tmpCountSmithing].isWorn = inv[i].isWorn;
			listSmithing[tmpCountSmithing].weight = inv[i].weight;
			tmpCountSmithing++;
		}
	}

	MyInventoryInfo[nowIndexPI].inventorysAlchemyCount = tmpCountAlchemy;
	MyInventoryInfo[nowIndexPI].inventorysSmithingCount = tmpCountSmithing;
}

void __fastcall buildPlayerInvData(InventoryInfo inv[], int& i, RE::TESBoundObject* item, RE::InventoryEntryData* entry, std::int32_t count)
{
	// 要判断extras

	if (entry->extraLists) {
		for (auto& xList : *entry->extraLists) {
			if (xList) {
				inv[i].isAlchemyEnt = false;
				inv[i].isSmithingEnt = false;
				inv[i].isEnchanted = false;
				inv[i].debugName = "";
				auto xCount = xList->GetCount();

				//logger::trace("Inv Name {} {} "sv, i, StringUtil::Utf8ToGbk(entry->GetDisplayName()));
				//logger::trace("Inv2 Name {} {} "sv, xCount, StringUtil::Utf8ToGbk(xList->GetDisplayName(item)));

				inv[i].ptr = item;
				inv[i].invPtr = entry;
				inv[i].invExtraPtr = xList;
				inv[i].count = xCount;
				inv[i].formId = item->GetFormID();
				//inv[i].formIdStr = FormIDToString(item->GetFormID());

				// 附魔
				auto ench = item->As<RE::TESEnchantableForm>();
				if (ench && ench->formEnchanting) {
					inv[i].isEnchanted = true;
					for (auto effect : ench->formEnchanting->effects) {
						inv[i].debugName += "|";
						inv[i].debugName += effect->baseEffect->GetFullName();
						if (effect->baseEffect->GetFormID() == 0x8B65C) {
							inv[i].isAlchemyEnt = true;

						} else if (effect->baseEffect->GetFormID() == 0x7A102) {
							inv[i].isSmithingEnt = true;
						}
					}
				}

				const auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
				if (xEnch && xEnch->enchantment) {
					inv[i].isEnchanted = true;
					for (auto effect : xEnch->enchantment->effects) {
						inv[i].debugName += "|";
						inv[i].debugName += effect->baseEffect->GetFullName();

						if (effect->baseEffect->GetFormID() == 0x8B65C) {
							inv[i].isAlchemyEnt = true;
						} else if (effect->baseEffect->GetFormID() == 0x7A102) {
							inv[i].isSmithingEnt = true;
						}
					}

				}

		/*		const auto player = RE::PlayerCharacter::GetSingleton();
				const auto invChanges = player->GetInventoryChanges()*/

				auto uid = xList->GetByType<RE::ExtraUniqueID>();
				if (uid) {
					inv[i].uniqueID = uid->uniqueID;
				} else {
					inv[i].uniqueID = 0;
				}

				std::stringstream ss;
				ss << xList->GetDisplayName(item) << " ";
				if (xCount > 1) {
					ss << "(" << xCount << ")";
				}
				if (inv[i].isEnchanted) {
					ss << " " << ICON_MDI_FLASH;
				}

				inv[i].name = ss.str();
				inv[i].weight = item->GetWeight();
				inv[i++].isWorn = (xList->HasType<RE::ExtraWorn>() || xList->HasType<RE::ExtraWornLeft>());

				count -= xCount;
			}
		}
	}
	if (count > 0) {
		inv[i].isAlchemyEnt = false;
		inv[i].isSmithingEnt = false;
		inv[i].isEnchanted = false;
		inv[i].debugName = "";

		inv[i].ptr = item;
		inv[i].invPtr = entry;
		inv[i].invExtraPtr = nullptr;
		inv[i].count = count;
		inv[i].formId = item->GetFormID();
		//inv[i].formIdStr = FormIDToString(item->GetFormID());
		//inv[i].name = entry->GetDisplayName();

		auto ench = item->As<RE::TESEnchantableForm>();
		if (ench && ench->formEnchanting) {
			inv[i].isEnchanted = true;

			for (auto effect : ench->formEnchanting->effects) {
				inv[i].debugName += "|";
				inv[i].debugName += effect->baseEffect->GetFullName();
				if (effect->baseEffect->GetFormID() == 0x8B65C) {
					inv[i].isAlchemyEnt = true;

				} else if (effect->baseEffect->GetFormID() == 0x7A102) {
					inv[i].isSmithingEnt = true;
				}
			}
		}

		std::stringstream ss;
		ss << item->GetName() << " ";
		if (count > 1) {
			ss << "(" << count << ")";
		}
		if (inv[i].isEnchanted) {
			ss << " " << ICON_MDI_FLASH;
		}

		inv[i].name = ss.str();

		inv[i].uniqueID = 0;
		inv[i].weight = item->GetWeight();
		//inv[i++].isWorn = entry->IsWorn();
		inv[i++].isWorn = false;
	}

	//logger::trace("Inv Name {} {} "sv, i, StringUtil::Utf8ToGbk(entry->GetDisplayName()));
}

void RefreshPlayerInvInfo()
{
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
		MyInventoryInfo[nowIndexPI].inventorysAlchemyCount = 0;
		MyInventoryInfo[nowIndexPI].inventorysSmithingCount = 0;

		for (const auto& [item, invData] : inv) {
			const auto& [count, entry] = invData;
			//if (count > 0 && entry->IsWorn()) {
			//const auto armor = item->As<RE::TESObjectARMO>();

			//entry.get()->IsEnchanted();

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

		
		buildPlayerAlchemy(MyInventoryInfo[nowIndexPI].inventorysARMO, MyInventoryInfo[nowIndexPI].inventoryARMOCount);

		// 双缓冲可以不用
		//MyInventoryInfo[nowIndexPI].inventoryCount = i;

		nowIndexPI = !nowIndexPI;
	}
}
