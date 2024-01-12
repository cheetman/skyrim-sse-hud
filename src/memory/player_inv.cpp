#include "player_inv.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <utils/GeneralUtil.h>

int nowIndexPI = 0;
bool show_inv_window = false;

PlayerInventoryInfo* MyInventoryInfo = new PlayerInventoryInfo[2];
std::vector<FavorInfo> favorAlchemy;
std::vector<FavorInfo> favorSmithing;

bool isFavor(const InventoryInfo& inv, const std::vector<FavorInfo>& favors)
{
	for (auto& favor : favors) {
		if (favor.formId == inv.formId) {
			if (favor.entDetail.size() == 0) {
				auto ench = inv.ptr->As<RE::TESEnchantableForm>();
				if (ench && ench->formEnchanting) {
					continue;  // 结束
				}
			} else {
				auto ench = inv.ptr->As<RE::TESEnchantableForm>();
				if (!(ench && ench->formEnchanting)) {
					continue;  // 结束
				}
				// 都要匹配
				if (ench->formEnchanting->effects.size() != favor.entDetail.size()) {
					continue;  // 结束
				}

				bool failure = false;
				for (int i = 0; i < favor.entDetail.size(); i++) {
					if (ench->formEnchanting->effects[i]->effectItem.magnitude != favor.entDetail[i].value || ench->formEnchanting->effects[i]->baseEffect->GetFormID() != favor.entDetail[i].formId) {
						failure = true;
						break;  // 结束
					}
				}
				if (failure) {
					continue;  // 结束
				}
			}

			if (favor.entDetail2.size() == 0) {
				auto ench = inv.ptr->As<RE::TESEnchantableForm>();
				if (inv.invExtraPtr) {
					const auto xEnch = inv.invExtraPtr->GetByType<RE::ExtraEnchantment>();
					if (xEnch && xEnch->enchantment) {
						continue;  // 结束
					}
				}
			} else {
				if (!inv.invExtraPtr) {
					continue;  // 结束
				}
				const auto xEnch = inv.invExtraPtr->GetByType<RE::ExtraEnchantment>();
				if (!(xEnch && xEnch->enchantment)) {
					continue;  // 结束
				}
				// 都要匹配
				if (xEnch->enchantment->effects.size() != favor.entDetail2.size()) {
					continue;  // 结束
				}

				bool failure = false;
				for (int i = 0; i < favor.entDetail2.size(); i++) {
					if (xEnch->enchantment->effects[i]->effectItem.magnitude != favor.entDetail2[i].value || xEnch->enchantment->effects[i]->baseEffect->GetFormID() != favor.entDetail2[i].formId) {
						failure = true;
						break;  // 结束
					}
				}
				if (failure) {
					continue;  // 结束
				}
			}
			//favor.isMap = true; // 标记一下
			return true;  // 找到
		}
	}
	return false;
}

InventoryInfo* getFavor(const FavorInfo& favor, std::vector<InventoryInfo>& invs, int count)
{
	for (int i = 0; i < count; i++) {
		auto& inv = invs[i];
		if (favor.formId == inv.formId) {
			if (favor.entDetail.size() == 0) {
				auto ench = inv.ptr->As<RE::TESEnchantableForm>();
				if (ench && ench->formEnchanting) {
					continue;  // 结束
				}
			} else {
				auto ench = inv.ptr->As<RE::TESEnchantableForm>();
				if (!(ench && ench->formEnchanting)) {
					continue;  // 结束
				}
				// 都要匹配
				if (ench->formEnchanting->effects.size() != favor.entDetail.size()) {
					continue;  // 结束
				}

				bool failure = false;
				for (int i = 0; i < favor.entDetail.size(); i++) {
					if (ench->formEnchanting->effects[i]->effectItem.magnitude != favor.entDetail[i].value || ench->formEnchanting->effects[i]->baseEffect->GetFormID() != favor.entDetail[i].formId) {
						failure = true;
						break;  // 结束
					}
				}
				if (failure) {
					continue;  // 结束
				}
			}

			if (favor.entDetail2.size() == 0) {
				auto ench = inv.ptr->As<RE::TESEnchantableForm>();
				if (inv.invExtraPtr) {
					const auto xEnch = inv.invExtraPtr->GetByType<RE::ExtraEnchantment>();
					if (xEnch && xEnch->enchantment) {
						continue;  // 结束
					}
				}
			} else {
				if (!inv.invExtraPtr) {
					continue;  // 结束
				}
				const auto xEnch = inv.invExtraPtr->GetByType<RE::ExtraEnchantment>();
				if (!(xEnch && xEnch->enchantment)) {
					continue;  // 结束
				}
				// 都要匹配
				if (xEnch->enchantment->effects.size() != favor.entDetail2.size()) {
					continue;  // 结束
				}

				bool failure = false;
				for (int i = 0; i < favor.entDetail2.size(); i++) {
					if (xEnch->enchantment->effects[i]->effectItem.magnitude != favor.entDetail2[i].value || xEnch->enchantment->effects[i]->baseEffect->GetFormID() != favor.entDetail2[i].formId) {
						failure = true;
						break;  // 结束
					}
				}
				if (failure) {
					continue;  // 结束
				}
			}
			//favor.isMap = true; // 标记一下
			return &inv;  // 找到
		}
	}
	return nullptr;
}

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
int getPlayerInvAlchemyFavorCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysAlchemyFavorCount;
}
int getPlayerInvSmithingCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithingCount;
}
int getPlayerInvSmithingFavorCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithingFavorCount;
}
int getPlayerInvAlchemyPotionCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysAlchemyPotionCount;
}
int getPlayerInvSmithingPotionCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithingPotionCount;
}
int getPlayerInvEnchantingPotionCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysEnchantingPotionCount;
}
int getPlayerInvEnchantingFavorCount()
{
	return MyInventoryInfo[!nowIndexPI].inventorysEnchantingFavorCount;
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
std::vector<InventoryInfo>& getPlayerInvAlchemyPotion()
{
	return MyInventoryInfo[!nowIndexPI].inventorysAlchemyPotion;
}
std::vector<InventoryInfo>& getPlayerInvSmithingPotion()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithingPotion;
}
std::vector<InventoryInfo>& getPlayerInvEnchantingPotion()
{
	return MyInventoryInfo[!nowIndexPI].inventorysEnchantingPotion;
}
std::vector<InventoryInfo>& getPlayerInvAlchemyFavor()
{
	return MyInventoryInfo[!nowIndexPI].inventorysAlchemyFavor;
}
std::vector<InventoryInfo>& getPlayerInvSmithingFavor()
{
	return MyInventoryInfo[!nowIndexPI].inventorysSmithingFavor;
}
std::vector<InventoryInfo>& getPlayerInvEnchantingFavor()
{
	return MyInventoryInfo[!nowIndexPI].inventorysEnchantingFavor;
}

bool compareForInventory(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.isWorn != info2.isWorn) {
		return info1.isWorn > info2.isWorn;
	} else {
		return info1.formId < info2.formId;
	}
}

bool compareForInventoryAlchemy(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.isWorn != info2.isWorn) {
		return info1.isWorn > info2.isWorn;
	} else if (info1.slotMask != info2.slotMask) {
		return info1.slotMask > info2.slotMask;
	} else {
		return info1.AlchemyValue > info2.AlchemyValue;
	}
}
bool compareForInventorySmithing(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.isWorn != info2.isWorn) {
		return info1.isWorn > info2.isWorn;
	} else if (info1.slotMask != info2.slotMask) {
		return info1.slotMask > info2.slotMask;
	} else {
		return info1.SmithingValue > info2.SmithingValue;
	}
}

bool compareForInventoryEnchantingPotion(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.EnchantingValue != info2.EnchantingValue) {
		return info1.EnchantingValue > info2.EnchantingValue;
	} else {
		return info1.formId > info2.formId;
	}
}
bool compareForInventorySmithingPotion(const InventoryInfo& info1, const InventoryInfo& info2)
{
	if (info1.SmithingValue != info2.SmithingValue) {
		return info1.SmithingValue > info2.SmithingValue;
	} else {
		return info1.formId > info2.formId;
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
			if (isFavor(inv[i], favorAlchemy)) {
				listAlchemy[tmpCountAlchemy].isFavor = true;
				listAlchemy[tmpCountAlchemy].name = ICON_MDI_STAR;
				listAlchemy[tmpCountAlchemy].name += inv[i].name;
			} else {
				listAlchemy[tmpCountAlchemy].isFavor = false;
				listAlchemy[tmpCountAlchemy].name = inv[i].name;
			}

			listAlchemy[tmpCountAlchemy].uniqueID = inv[i].uniqueID;
			listAlchemy[tmpCountAlchemy].isWorn = inv[i].isWorn;
			listAlchemy[tmpCountAlchemy].weight = inv[i].weight;
			listAlchemy[tmpCountAlchemy].AlchemyValue = inv[i].AlchemyValue;

			std::string slotMaskStr = "";
			for (int j = 0; j <= 31; j++) {
				int mask = 1 << j;
				if ((inv[i].slotMask & mask) == mask) {
					if (slotMaskStr != "") {
						slotMaskStr += ".";
					}
					slotMaskStr += std::to_string(j + 30);
				}
			}
			listAlchemy[tmpCountAlchemy].slotMaskStr = slotMaskStr;
			listAlchemy[tmpCountAlchemy].slotMask = inv[i].slotMask;

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
			if (isFavor(inv[i], favorSmithing)) {
				listSmithing[tmpCountSmithing].isFavor = true;
				listSmithing[tmpCountSmithing].name = ICON_MDI_STAR;
				listSmithing[tmpCountSmithing].name += inv[i].name;
			} else {
				listSmithing[tmpCountSmithing].isFavor = false;
				listSmithing[tmpCountSmithing].name = inv[i].name;
			}

			listSmithing[tmpCountSmithing].uniqueID = inv[i].uniqueID;
			listSmithing[tmpCountSmithing].isWorn = inv[i].isWorn;
			listSmithing[tmpCountSmithing].weight = inv[i].weight;
			listSmithing[tmpCountSmithing].SmithingValue = inv[i].SmithingValue;

			std::string slotMaskStr = "";
			for (int j = 0; j <= 31; j++) {
				int mask = 1 << j;
				if ((inv[i].slotMask & mask) == mask) {
					if (slotMaskStr != "") {
						slotMaskStr += ".";
					}
					slotMaskStr += std::to_string(j + 30);
				}
			}
			listSmithing[tmpCountSmithing].slotMaskStr = slotMaskStr;
			listSmithing[tmpCountSmithing].slotMask = inv[i].slotMask;

			tmpCountSmithing++;
		}
	}

	MyInventoryInfo[nowIndexPI].inventorysAlchemyCount = tmpCountAlchemy;
	MyInventoryInfo[nowIndexPI].inventorysSmithingCount = tmpCountSmithing;
}

void buildPlayerPotion(InventoryInfo inv[], std::int32_t count)
{
	auto& listEnchanting = MyInventoryInfo[nowIndexPI].inventorysEnchantingPotion;
	auto& listSmithing = MyInventoryInfo[nowIndexPI].inventorysSmithingPotion;
	int tmpCountEnchanting = 0;
	int tmpCountSmithing = 0;

	for (int i = 0; i < count; i++) {
		auto& item = inv[i];
		if (item.isSmithingEnt) {
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
			listSmithing[tmpCountSmithing].SmithingValue = inv[i].SmithingValue;

			tmpCountSmithing++;
		} else if (item.isEnchantingEnt) {
			if (tmpCountEnchanting + 1 > listEnchanting.size()) {
				listEnchanting.resize(listEnchanting.size() + 20);
			}
			listEnchanting[tmpCountEnchanting].ptr = inv[i].ptr;
			listEnchanting[tmpCountEnchanting].invPtr = inv[i].invPtr;
			listEnchanting[tmpCountEnchanting].invExtraPtr = inv[i].invExtraPtr;
			listEnchanting[tmpCountEnchanting].count = inv[i].count;
			listEnchanting[tmpCountEnchanting].formId = inv[i].formId;
			listEnchanting[tmpCountEnchanting].name = inv[i].name;
			listEnchanting[tmpCountEnchanting].uniqueID = inv[i].uniqueID;
			listEnchanting[tmpCountEnchanting].isWorn = inv[i].isWorn;
			listEnchanting[tmpCountEnchanting].weight = inv[i].weight;
			listEnchanting[tmpCountEnchanting].EnchantingValue = inv[i].EnchantingValue;

			tmpCountEnchanting++;
		}
	}

	MyInventoryInfo[nowIndexPI].inventorysEnchantingPotionCount = tmpCountEnchanting;
	MyInventoryInfo[nowIndexPI].inventorysSmithingPotionCount = tmpCountSmithing;
}

void buildPlayerFavor(std::vector<InventoryInfo>& inv, std::int32_t count, std::vector<InventoryInfo>& list, std::int32_t& listCount, std::vector<FavorInfo>& favors)
{
	int tmpCount = 0;

	for (auto& favor : favors) {
		if (tmpCount + 1 > list.size()) {
			list.resize(list.size() + 20);
		}

		auto inv2 = getFavor(favor, inv, count);
		if (inv2) {
			list[tmpCount].ptr = inv2->ptr;
			list[tmpCount].invPtr = inv2->invPtr;
			list[tmpCount].invExtraPtr = inv2->invExtraPtr;
			list[tmpCount].count = inv2->count;
			list[tmpCount].formId = inv2->formId;
			list[tmpCount].name = inv2->name;
			list[tmpCount].uniqueID = inv2->uniqueID;
			list[tmpCount].isWorn = inv2->isWorn;
			list[tmpCount].weight = inv2->weight;
			list[tmpCount].SmithingValue = inv2->SmithingValue;
			list[tmpCount].isSmithingEnt = inv2->isSmithingEnt;
			list[tmpCount].AlchemyValue = inv2->AlchemyValue;
			list[tmpCount].isAlchemyEnt = inv2->isAlchemyEnt;
			list[tmpCount].slotMask = inv2->slotMask;
			list[tmpCount].slotMaskStr = inv2->slotMaskStr;
			list[tmpCount].isWorn = inv2->isWorn;
		} else {
			list[tmpCount].ptr = nullptr;
			list[tmpCount].name = "丢失";
		}

		tmpCount++;
	}

	listCount = tmpCount;
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
							inv[i].AlchemyValue = effect->effectItem.magnitude;

						} else if (effect->baseEffect->GetFormID() == 0x7A102) {
							inv[i].isSmithingEnt = true;
							inv[i].SmithingValue = effect->effectItem.magnitude;
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
							inv[i].AlchemyValue = effect->effectItem.magnitude;
						} else if (effect->baseEffect->GetFormID() == 0x7A102) {
							inv[i].isSmithingEnt = true;
							inv[i].SmithingValue = effect->effectItem.magnitude;
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
					ss << ICON_MDI_FLASH;
				}

				inv[i].name = ss.str();
				inv[i].weight = item->GetWeight();
				if (item->IsArmor()) {
					const auto armor = item->As<RE::TESObjectARMO>();
					if (armor) {
						inv[i].slotMask = (int)armor->GetSlotMask();
					}
				}

				if (item->Is(RE::FormType::AlchemyItem)) {
					const auto alch = item->As<RE::AlchemyItem>();
					if (alch) {
						for (auto effect : alch->effects) {
							inv[i].debugName += "|";
							inv[i].debugName += effect->baseEffect->GetFullName();

							if (effect->baseEffect->GetFormID() == 0x3EB18) {
								inv[i].isAlchemyEnt = true;
								inv[i].AlchemyValue = effect->effectItem.magnitude;
							} else if (effect->baseEffect->GetFormID() == 0x3EB1D) {
								inv[i].isSmithingEnt = true;
								inv[i].SmithingValue = effect->effectItem.magnitude;
							} else if (effect->baseEffect->GetFormID() == 0x3EB29) {
								inv[i].isEnchantingEnt = true;
								inv[i].EnchantingValue = effect->effectItem.magnitude;
							}
						}
					}
				}

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
					inv[i].AlchemyValue = effect->effectItem.magnitude;

				} else if (effect->baseEffect->GetFormID() == 0x7A102) {
					inv[i].isSmithingEnt = true;
					inv[i].SmithingValue = effect->effectItem.magnitude;
				}
			}
		}

		std::stringstream ss;
		ss << item->GetName() << " ";
		if (count > 1) {
			ss << "(" << count << ")";
		}
		if (inv[i].isEnchanted) {
			ss << ICON_MDI_FLASH;
		}

		inv[i].name = ss.str();

		inv[i].uniqueID = 0;
		inv[i].weight = item->GetWeight();
		//inv[i++].isWorn = entry->IsWorn();

		if (item->IsArmor()) {
			const auto armor = item->As<RE::TESObjectARMO>();
			if (armor) {
				inv[i].slotMask = (int)armor->GetSlotMask();
			}
		}

		if (item->Is(RE::FormType::AlchemyItem)) {
			const auto alch = item->As<RE::AlchemyItem>();
			if (alch) {
				for (auto effect : alch->effects) {
					inv[i].debugName += "|";
					inv[i].debugName += effect->baseEffect->GetFullName();

					if (effect->baseEffect->GetFormID() == 0x3EB18) {
						inv[i].isAlchemyEnt = true;
						inv[i].AlchemyValue = effect->effectItem.magnitude;
					} else if (effect->baseEffect->GetFormID() == 0x3EB1D) {
						inv[i].isSmithingEnt = true;
						inv[i].SmithingValue = effect->effectItem.magnitude;
					} else if (effect->baseEffect->GetFormID() == 0x3EB29) {
						inv[i].isEnchantingEnt = true;
						inv[i].EnchantingValue = effect->effectItem.magnitude;
					}
				}
			}
		}

		inv[i++].isWorn = false;
	}

	//logger::trace("Inv Name {} {} "sv, i, StringUtil::Utf8ToGbk(entry->GetDisplayName()));
}

void modPlayerInv(std::vector<InventoryInfo>& invs, std::int32_t count,int type)
{
	std::unordered_map<int, float> cache;
	for (int i = 0; i < count; i++) {
		auto& inv = invs[i];
		if (cache.find(inv.slotMask) == cache.end()) {
			cache[inv.slotMask] = 0;
		}
		if (type == 1) {
			if (inv.AlchemyValue > cache[inv.slotMask]) {
				cache[inv.slotMask] = inv.AlchemyValue;
			}
		} else if (type == 2) {
			if (inv.SmithingValue > cache[inv.slotMask]) {
				cache[inv.slotMask] = inv.SmithingValue;
			}
		}
	}

	for (int i = 0; i < count; i++) {
		auto& inv = invs[i];
		if (type == 1) {
			if (inv.AlchemyValue == cache[inv.slotMask]) {
				inv.isBest = true;
			} else {
				inv.isBest = false;
			}
		} else if (type == 2) {
			if (inv.SmithingValue == cache[inv.slotMask]) {
				inv.isBest = true;
			} else {
				inv.isBest = false;
			}
		}
	}
}

void modPlayerInvPotion(std::vector<InventoryInfo>& invs, std::int32_t count, int type)
{
	float cache = 0;
	for (int i = 0; i < count; i++) {
		auto& inv = invs[i];
		if (type == 1) {
			if (inv.AlchemyValue > cache) {
				cache = inv.AlchemyValue;
			}
		} else if (type == 2) {
			if (inv.SmithingValue > cache) {
				cache = inv.SmithingValue;
			}
		} else if (type == 3) {
			if (inv.EnchantingValue > cache) {
				cache = inv.EnchantingValue;
			}
		}
	}

	for (int i = 0; i < count; i++) {
		auto& inv = invs[i];
		if (type == 1) {
			if (inv.AlchemyValue == cache) {
				inv.isBest = true;
			} else {
				inv.isBest = false;
			}
		} else if (type == 2) {
			if (inv.SmithingValue == cache) {
				inv.isBest = true;
			} else {
				inv.isBest = false;
			}
		} else if (type == 3) {
			if (inv.EnchantingValue == cache) {
				inv.isBest = true;
			} else {
				inv.isBest = false;
			}
		}
	}
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
		buildPlayerPotion(MyInventoryInfo[nowIndexPI].inventorysALCH, MyInventoryInfo[nowIndexPI].inventoryALCHCount);

		std::partial_sort(MyInventoryInfo[nowIndexPI].inventorysAlchemy.begin(), MyInventoryInfo[nowIndexPI].inventorysAlchemy.begin() + MyInventoryInfo[nowIndexPI].inventorysAlchemyCount, MyInventoryInfo[nowIndexPI].inventorysAlchemy.begin() + MyInventoryInfo[nowIndexPI].inventorysAlchemyCount, compareForInventoryAlchemy);
		std::partial_sort(MyInventoryInfo[nowIndexPI].inventorysSmithing.begin(), MyInventoryInfo[nowIndexPI].inventorysSmithing.begin() + MyInventoryInfo[nowIndexPI].inventorysSmithingCount, MyInventoryInfo[nowIndexPI].inventorysSmithing.begin() + MyInventoryInfo[nowIndexPI].inventorysSmithingCount, compareForInventorySmithing);
		std::partial_sort(MyInventoryInfo[nowIndexPI].inventorysEnchantingPotion.begin(), MyInventoryInfo[nowIndexPI].inventorysEnchantingPotion.begin() + MyInventoryInfo[nowIndexPI].inventorysEnchantingPotionCount, MyInventoryInfo[nowIndexPI].inventorysEnchantingPotion.begin() + MyInventoryInfo[nowIndexPI].inventorysEnchantingPotionCount, compareForInventoryEnchantingPotion);
		std::partial_sort(MyInventoryInfo[nowIndexPI].inventorysSmithingPotion.begin(), MyInventoryInfo[nowIndexPI].inventorysSmithingPotion.begin() + MyInventoryInfo[nowIndexPI].inventorysSmithingPotionCount, MyInventoryInfo[nowIndexPI].inventorysSmithingPotion.begin() + MyInventoryInfo[nowIndexPI].inventorysSmithingPotionCount, compareForInventorySmithingPotion);

		buildPlayerFavor(MyInventoryInfo[nowIndexPI].inventorysAlchemy, MyInventoryInfo[nowIndexPI].inventorysAlchemyCount, MyInventoryInfo[nowIndexPI].inventorysAlchemyFavor, MyInventoryInfo[nowIndexPI].inventorysAlchemyFavorCount, favorAlchemy);
		buildPlayerFavor(MyInventoryInfo[nowIndexPI].inventorysSmithing, MyInventoryInfo[nowIndexPI].inventorysSmithingCount, MyInventoryInfo[nowIndexPI].inventorysSmithingFavor, MyInventoryInfo[nowIndexPI].inventorysSmithingFavorCount, favorSmithing);
		std::partial_sort(MyInventoryInfo[nowIndexPI].inventorysAlchemy.begin(), MyInventoryInfo[nowIndexPI].inventorysAlchemy.begin() + MyInventoryInfo[nowIndexPI].inventorysAlchemyFavorCount, MyInventoryInfo[nowIndexPI].inventorysAlchemy.begin() + MyInventoryInfo[nowIndexPI].inventorysAlchemyFavorCount, compareForInventoryAlchemy);
		std::partial_sort(MyInventoryInfo[nowIndexPI].inventorysSmithing.begin(), MyInventoryInfo[nowIndexPI].inventorysSmithing.begin() + MyInventoryInfo[nowIndexPI].inventorysSmithingFavorCount, MyInventoryInfo[nowIndexPI].inventorysSmithing.begin() + MyInventoryInfo[nowIndexPI].inventorysSmithingFavorCount, compareForInventorySmithing);

		modPlayerInv(MyInventoryInfo[nowIndexPI].inventorysAlchemy, MyInventoryInfo[nowIndexPI].inventorysAlchemyCount,1);
		modPlayerInv(MyInventoryInfo[nowIndexPI].inventorysSmithing, MyInventoryInfo[nowIndexPI].inventorysSmithingCount,2);
		modPlayerInv(MyInventoryInfo[nowIndexPI].inventorysAlchemyFavor, MyInventoryInfo[nowIndexPI].inventorysAlchemyFavorCount,1);
		modPlayerInv(MyInventoryInfo[nowIndexPI].inventorysSmithingFavor, MyInventoryInfo[nowIndexPI].inventorysSmithingFavorCount,2);
		modPlayerInvPotion(MyInventoryInfo[nowIndexPI].inventorysSmithingPotion, MyInventoryInfo[nowIndexPI].inventorysSmithingPotionCount,2);
		modPlayerInvPotion(MyInventoryInfo[nowIndexPI].inventorysEnchantingPotion, MyInventoryInfo[nowIndexPI].inventorysEnchantingPotionCount,3);

		// 双缓冲可以不用
		//MyInventoryInfo[nowIndexPI].inventoryCount = i;

		nowIndexPI = !nowIndexPI;
	}
}
