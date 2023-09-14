#include "memory.h"
#include <unordered_set>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
#include <utils/PlayerDataProvider.h>

bool active = false;
bool activeItems = false;
int refresh_time_data = 300;
bool startflag = false;
WeaponInfo leftWeaponInfo;
WeaponInfo rightWeaponInfo;
WeaponInfo ammoInfo;
ArmorInfo wornArmos[32];
PlayerInfo playerInfo;

bool isGameLoading = false;
bool show_npc_window_dead_hidden = false;

void __cdecl RefreshGameInfo(void*)
{
	//auto game_hwnd = FindWindowA(NULL, "Skyrim Special Edition");
	Sleep(10000);

	// 标记装备槽是否主要
	//wornArmos[1].isMainSlotAlert =
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
			/*	RE::UI* ui = RE::UI::GetSingleton();
			const auto menu = ui ? ui->GetMenu<RE::ContainerMenu>() : nullptr;
			const auto movie = menu ? menu->uiMovie : nullptr;
			if (movie) {

			}*/
			Sleep(3000);
			continue;
		}
		if (isGameLoading) {
			Sleep(3000);
			continue;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (player) {
			playerInfo.Angle = player->GetAngle();
			playerInfo.Position = player->GetPosition();
			playerInfo.name = player->GetName();
			if (player->currentLocation) {
				auto locationName = player->currentLocation->GetFullName();
				playerInfo.location = locationName;
			} else {
				playerInfo.location = "天际";
			}

			//__try {
			auto playerFormEditorID = player->GetFormEditorID();
			auto playerFormID = player->GetFormID();
			auto playerGoldValue = player->GetGoldValue();
			// 用不到
			//auto playerGoldAmount = player->GetGoldAmount();
			auto playerDisplayFullName = player->GetDisplayFullName();

			auto playerLevel = player->GetLevel();
			auto playerRace = player->GetRace();
			if (playerRace) {
				//playerRaceName = player->GetRace()->GetFullName();
			}

			auto playerAttackingWeapon = player->GetAttackingWeapon();
			auto playerEquippedEntryDataLeft = player->GetEquippedEntryData(true);
			auto playerEquippedEntryDataRight = player->GetEquippedEntryData(false);
			auto playerEquippedObjectLeft = player->GetEquippedObject(true);
			auto playerEquippedObjectRight = player->GetEquippedObject(false);

			//auto id = player->GetCrimeGoldValue();
			auto factionOwner = player->GetFactionOwner();
			if (factionOwner) {
				auto factionOwnerName = factionOwner->GetFullName();
			}
			// 轻甲等级
			//lightArmor = player->GetActorValue(RE::ActorValue::kLightArmor);
			//// 重甲等级
			//heavyArmor = player->GetActorValue(RE::ActorValue::kHeavyArmor);
			//
			//player->GetLevel();
			playerInfo.equippedWeight = player->equippedWeight;
			playerInfo.carryWeight = player->GetActorValue(RE::ActorValue::kCarryWeight);

			playerInfo.kHealth = player->GetActorValue(RE::ActorValue::kHealth);
			playerInfo.kMagicka = player->GetActorValue(RE::ActorValue::kMagicka);
			playerInfo.kStamina = player->GetActorValue(RE::ActorValue::kStamina);

			playerInfo.kHealthBase = player->GetPermanentActorValue(RE::ActorValue::kHealth);
			playerInfo.kStaminaBase = player->GetPermanentActorValue(RE::ActorValue::kStamina);
			playerInfo.kMagickaBase = player->GetPermanentActorValue(RE::ActorValue::kMagicka);

			// 生命恢复速率
			auto kHealRate = player->GetActorValue(RE::ActorValue::kHealRate);
			// 战斗中生命恢复速率
			auto kCombatHealthRegenMultiply = player->GetActorValue(RE::ActorValue::kCombatHealthRegenMultiply);
			// 魔法恢复速率
			auto kMagickaRate = player->GetActorValue(RE::ActorValue::kMagickaRate);
			// 体力恢复速率
			auto KStaminaRate = player->GetActorValue(RE::ActorValue::KStaminaRate);

			playerInfo.kOneHandedModifier = player->GetActorValue(RE::ActorValue::kOneHandedModifier);
			playerInfo.kTwoHandedModifier = player->GetActorValue(RE::ActorValue::kTwoHandedModifier);
			playerInfo.kMarksmanModifier = player->GetActorValue(RE::ActorValue::kMarksmanModifier);
			playerInfo.kSmithingModifier = player->GetActorValue(RE::ActorValue::kSmithingModifier);
			playerInfo.kSmithingPowerModifier = player->GetActorValue(RE::ActorValue::kSmithingPowerModifier);
			playerInfo.kSmithingSkillAdvance = player->GetActorValue(RE::ActorValue::kSmithingSkillAdvance);
			playerInfo.kAlchemyModifier = player->GetActorValue(RE::ActorValue::kAlchemyModifier);
			playerInfo.kAlchemySkillAdvance = player->GetActorValue(RE::ActorValue::kAlchemySkillAdvance);
			playerInfo.kAlchemyPowerModifier = player->GetActorValue(RE::ActorValue::kAlchemyPowerModifier);
			playerInfo.kEnchantingModifier = player->GetActorValue(RE::ActorValue::kEnchantingModifier);
			playerInfo.kEnchantingPowerModifier = player->GetActorValue(RE::ActorValue::kEnchantingPowerModifier);
			playerInfo.kEnchantingSkillAdvance = player->GetActorValue(RE::ActorValue::kEnchantingSkillAdvance);

			// 武器伤害
			//kMeleeDamage = player->GetActorValue(RE::ActorValue::kMeleeDamage);
			// 空手伤害
			//kUnarmedDamage = player->GetActorValue(RE::ActorValue::kUnarmedDamage);

			// 弓箭伤害
			playerInfo.ArrowDamage = PlayerDataProvider::getArrowDamage(player);
			// 右手伤害
			playerInfo.DamageRight = PlayerDataProvider::getDamage(player, false);
			// 左手伤害
			playerInfo.DamageLeft = PlayerDataProvider::getDamage(player, true);

			// 显示伤害
			std::string tmp = "";
			if (playerInfo.DamageLeft.empty()) {
				if (playerInfo.DamageRight.empty()) {
					tmp = "0";
				} else {
					tmp = playerInfo.DamageRight;
				}
			} else {
				if (playerInfo.DamageRight.empty()) {
					tmp = playerInfo.DamageLeft;
				} else {
					tmp.append(playerInfo.DamageLeft);
					tmp.append(" | ");
					tmp.append(playerInfo.DamageRight);
				}
			}
			if (!playerInfo.ArrowDamage.empty()) {
				tmp.append("   - 箭:");
				tmp.append(playerInfo.ArrowDamage);
			}
			playerInfo.DamageStr = tmp;

			// 护甲
			playerInfo.kDamageResist = player->GetActorValue(RE::ActorValue::kDamageResist);
			//playerInfo.DamageResist = PlayerDataProvider::getDamageResistance(player, -1, "");

			// 暴击几率
			//kCriticalChance = player->GetActorValue(RE::ActorValue::kCriticalChance);
			// 毒抗
			playerInfo.kPoisonResist = player->GetActorValue(RE::ActorValue::kPoisonResist);
			// 火炕
			playerInfo.kResistFire = player->GetActorValue(RE::ActorValue::kResistFire);
			playerInfo.kResistShock = player->GetActorValue(RE::ActorValue::kResistShock);
			playerInfo.kResistMagic = player->GetActorValue(RE::ActorValue::kResistMagic);
			playerInfo.kResistFrost = player->GetActorValue(RE::ActorValue::kResistFrost);
			playerInfo.kResistDisease = player->GetActorValue(RE::ActorValue::kResistDisease);

			// 装备信息
			const auto inv = player->GetInventory([](RE::TESBoundObject& a_object) {
				return a_object.IsArmor();
			});

			int removeIndexs = 0;  // 最后需要移除的元素索引
			for (const auto& [item, invData] : inv) {
				const auto& [count, entry] = invData;
				if (count > 0 && entry->IsWorn()) {
					const auto armor = item->As<RE::TESObjectARMO>();
					// 插槽名称(所有)
					//std::string slotNames = "";
					int parts = (int)armor->GetSlotMask();

					removeIndexs += parts;

					//int slotIndex = 0;
					for (int i = 0; i <= 31; i++) {
						int mask = 1 << i;
						if ((parts & mask) == mask) {
							/*if (slotIndex == 0) {
									slotIndex = i;
									removeIndexs += parts;
									slotNames.append(GetEquipSlotName(i));
									slotNames.append(" ");
								}*/
							wornArmos[i].isExist = true;
							wornArmos[i].name = armor->GetName();
							wornArmos[i].formID = FormIDToString(armor->GetFormID());
							wornArmos[i].formTypeName = GetFormTypeName(armor->formType.underlying());
							wornArmos[i].goldValue = armor->GetGoldValue();
							wornArmos[i].value = armor->value;
							wornArmos[i].armorRating = armor->GetArmorRating();
							wornArmos[i].armorTypeName = GetArmorTypeName(armor->GetArmorType());
							//wornArmos[i].equipSlotName = slotNames;
							//wornArmos[i].equipSlotName = GetEquipSlotName(i);
							wornArmos[i].weight = armor->weight;
							std::string tmp = wornArmos[i].name;
							tmp.append(" - ");
							tmp.append(wornArmos[i].armorTypeName);
							tmp.append(" | ");
							tmp.append(wornArmos[i].equipSlotName);
							wornArmos[i].treeId = tmp;
						}
					}
					//if (slotIndex > 0) {
					//}
				}
			}
			// 删除未占用插槽的元素
			for (int i = 0; i <= 31; i++) {
				int mask = 1 << i;
				if ((removeIndexs & mask) != mask) {
					wornArmos[i].isExist = false;
				}
			}

			// 武器信息
			auto leftWeapon = player->GetEquippedObject(true);
			if (leftWeapon) {
				leftWeaponInfo.formType = leftWeapon->GetFormType();
				leftWeaponInfo.formTypeName = GetFormTypeName(leftWeapon->formType.underlying());
				leftWeaponInfo.name = leftWeapon->GetName();
				leftWeaponInfo.formID = FormIDToString(leftWeapon->GetFormID());
				if (leftWeapon->IsWeapon()) {
					auto item = leftWeapon->As<RE::TESObjectWEAP>();
					leftWeaponInfo.isExist = true;
					leftWeaponInfo.goldValue = item->GetGoldValue();
					leftWeaponInfo.value = item->value;
					leftWeaponInfo.weaponTypeName = GetWeaponAnimationTypeName(item->GetWeaponType());
					leftWeaponInfo.damage = item->GetAttackDamage();
					leftWeaponInfo.critDamage = item->GetCritDamage();
					leftWeaponInfo.weight = item->weight;
					std::string tmp = std::string(leftWeaponInfo.name);
					tmp.append(" - ");
					tmp.append(leftWeaponInfo.weaponTypeName);
					leftWeaponInfo.treeId = tmp;
				} else if (leftWeapon->Is(RE::FormType::Spell)) {
					auto item = leftWeapon->As<RE::MagicItem>();
					leftWeaponInfo.isExist = true;
					leftWeaponInfo.goldValue = item->GetGoldValue();
					leftWeaponInfo.castingTypeName = GetCastingTypeName(item->GetCastingType());
					leftWeaponInfo.spellTypeName = GetSpellTypeName(item->GetSpellType());
					leftWeaponInfo.cost = item->CalculateMagickaCost(player);
					leftWeaponInfo.time = item->GetChargeTime();
					auto tmp = std::string(leftWeaponInfo.name);
					tmp.append(" - ");
					tmp.append(leftWeaponInfo.castingTypeName);
					leftWeaponInfo.treeId = tmp;
				} else {
					leftWeaponInfo.isExist = false;
				}
			} else {
				leftWeaponInfo.isExist = false;
			}

			auto rightWeapon = player->GetEquippedObject(false);
			if (rightWeapon) {
				rightWeaponInfo.formType = rightWeapon->GetFormType();
				rightWeaponInfo.formTypeName = GetFormTypeName(rightWeapon->formType.underlying());
				rightWeaponInfo.name = rightWeapon->GetName();
				rightWeaponInfo.formID = FormIDToString(rightWeapon->GetFormID());
				if (rightWeapon->IsWeapon()) {
					auto item = rightWeapon->As<RE::TESObjectWEAP>();
					rightWeaponInfo.isExist = true;
					rightWeaponInfo.goldValue = item->GetGoldValue();
					rightWeaponInfo.value = item->value;
					rightWeaponInfo.weaponTypeName = GetWeaponAnimationTypeName(item->GetWeaponType());
					rightWeaponInfo.damage = item->GetAttackDamage();
					rightWeaponInfo.critDamage = item->GetCritDamage();
					rightWeaponInfo.weight = item->weight;

					if (item->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandSword ||
						item->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandAxe ||
						item->GetWeaponType() == RE::WEAPON_TYPE::kBow ||
						item->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow) {
						rightWeaponInfo.isTwoHand = true;
					} else {
						rightWeaponInfo.isTwoHand = false;
					}

					auto tmp = std::string(rightWeaponInfo.name);
					tmp.append(" - ");
					tmp.append(rightWeaponInfo.weaponTypeName);
					rightWeaponInfo.treeId = tmp;
				} else if (rightWeapon->Is(RE::FormType::Spell)) {
					auto item = rightWeapon->As<RE::MagicItem>();

					rightWeaponInfo.isExist = true;
					rightWeaponInfo.goldValue = item->GetGoldValue();
					rightWeaponInfo.castingTypeName = GetCastingTypeName(item->GetCastingType());
					rightWeaponInfo.spellTypeName = GetSpellTypeName(item->GetSpellType());
					rightWeaponInfo.cost = item->CalculateMagickaCost(player);
					rightWeaponInfo.time = item->GetChargeTime();
					rightWeaponInfo.isTwoHand = item->IsTwoHanded();

					auto tmp = std::string(rightWeaponInfo.name);
					tmp.append(" - ");
					tmp.append(rightWeaponInfo.castingTypeName);
					rightWeaponInfo.treeId = tmp;
				} else {
					rightWeaponInfo.isExist = false;
				}

			} else {
				rightWeaponInfo.isExist = false;
			}
			// 弹药
			auto ammo = player->GetCurrentAmmo();
			if (ammo) {
				const auto item = ammo->As<RE::TESAmmo>();
				ammoInfo.isExist = true;
				ammoInfo.formType = item->GetFormType();
				ammoInfo.formTypeName = GetFormTypeName(item->formType.underlying());
				ammoInfo.name = item->GetName();
				ammoInfo.formID = FormIDToString(item->GetFormID());
				ammoInfo.goldValue = item->GetGoldValue();
				ammoInfo.value = item->value;
				ammoInfo.damage = item->data.damage;
				ammoInfo.weight = item->weight;
				auto tmp = std::string(ammoInfo.name);
				/*tmp.append(" - ");
					tmp.append(rightWeaponInfo.castingTypeName);*/
				ammoInfo.treeId = tmp;
			} else {
				ammoInfo.isExist = false;
			}

			/*	} __except (SehFilter(GetExceptionCode())) {
					logger::info("catch (...)");
				}*/
		} else {
			logger::info("no player"sv);
		}
	}
}

Actor2Info actorInfo[2];
int nowIndex = 0;

ActorInfo* getNpcData()
{
	return &actorInfo[!nowIndex].npcInfo[0];
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

float calculateDistance(const RE::NiPoint3& p1, const RE::NiPoint3& p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	float dz = p2.z - p1.z;

	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

#define M_PI 3.14159265358979323846

int calculateDirection(const RE::NiPoint3& p1, const RE::NiPoint3& p2, const RE::NiPoint3& a2)
{
	float x_diff = p1.x - p2.x;
	float y_diff = p1.y - p2.y;

	float angle = atan2(y_diff, x_diff) * 180 / M_PI;
	if (angle < 0) {
		angle += 360;
	}

	float relative_angle = angle + (a2.z * 180 / M_PI);

	if (relative_angle < 0) {
		relative_angle += 360;
	}
	if (relative_angle > 360) {
		relative_angle -= 360;
	}

	//SKSE::log::error("angle {} 视角 {} 夹角 {}", (int)angle, (int)(a2.z * 180 / M_PI), (int)relative_angle);

	if (relative_angle >= 45 && relative_angle < 135) {
		return 1;  // 右边
	} else if (relative_angle >= 135 && relative_angle < 225) {
		return 2;  // 下
	} else if (relative_angle >= 225 && relative_angle < 315) {
		return 3;  // 左
	} else {
		return 4;  // 前
	}
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

bool compareForItem(const ItemInfo& info1, const ItemInfo& info2)
{
	if (info1.gold != info2.gold) {
		return info1.gold > info2.gold;
	} else {
		return info1.baseFormId < info2.baseFormId;
	}
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
		//const auto armor = item->As<RE::TESObjectARMO>();
		if (count > 0) {
			//actorInfo[tmpIndex].Inventorys[i].formIdStr = FormIDToString(armor->GetFormID());
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

PlayerInventoryInfo* MyInventoryInfo = new PlayerInventoryInfo[2];
//PlayerInventoryInfo MyInventoryInfo[2];
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

int show_npc_window_dis_meter = 30;
bool show_npc_window_dis = false;
bool show_enemy_window = false;
bool show_inv_window = false;
bool show_npc_window = false;
bool show_items_window = false;

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

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			Sleep(3000);
			continue;
		}
		auto pl = RE::ProcessLists::GetSingleton();

		nowIndex = !nowIndex;

		{
			if (show_inv_window) {
				// 刷新自己的装备
				const auto inv = player->GetInventory();
				MyInventoryInfo[nowIndex].inventoryARMOCount = 0;
				MyInventoryInfo[nowIndex].inventoryBOOKCount = 0;
				MyInventoryInfo[nowIndex].inventoryWEAPCount = 0;
				MyInventoryInfo[nowIndex].inventoryAMMOCount = 0;
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

				// 双缓冲可以不用
				//MyInventoryInfo[nowIndex].inventoryCount = i;
			}
		}

		if (show_npc_window) {
			int tmpNpcCount = 0;
			int tmpEnemyCount = 0;
			int tmpTeammateCount = 0;

			for (auto& handle : pl->highActorHandles) {
				//for (int i = 0; i < actorCount; i++) {
				auto actor = handle.get().get();
				if (actor) {
					if (actor->IsPlayerTeammate()) {
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].formId = actor->GetFormID();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].ptr = actor;
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].level = actor->GetLevel();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].distance = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());

						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].isInCombat = actor->IsInCombat();

						//teammateInfo[i].idHostile = actor->IsHostileToActor(player);
						RefreshInventory(actor, actorInfo[nowIndex].teammateInfo, tmpTeammateCount++);

					} else if (actor->IsHostileToActor(player)) {
						if (show_npc_window_dead_hidden) {
							if (actor->GetLifeState() == RE::ACTOR_LIFE_STATE::kDead) {
								continue;
							}
						}
						float dis = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].distance = dis;
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].formId = actor->GetFormID();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].ptr = actor;
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].level = actor->GetLevel();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].enemyInfo[tmpEnemyCount].isInCombat = actor->IsInCombat();

						RefreshInventory(actor, actorInfo[nowIndex].enemyInfo, tmpEnemyCount++);

					} else {
						if (show_npc_window_dead_hidden) {
							if (actor->GetLifeState() == RE::ACTOR_LIFE_STATE::kDead) {
								continue;
							}
						}
						float dis = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}
						actorInfo[nowIndex].npcInfo[tmpNpcCount].distance = dis;
						actorInfo[nowIndex].npcInfo[tmpNpcCount].formId = actor->GetFormID();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].npcInfo[tmpNpcCount].ptr = actor;
						actorInfo[nowIndex].npcInfo[tmpNpcCount].level = actor->GetLevel();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						actorInfo[nowIndex].npcInfo[tmpNpcCount].lifeState = actor->GetLifeState();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].isInCombat = actor->IsInCombat();

						RefreshInventory(actor, actorInfo[nowIndex].npcInfo, tmpNpcCount++);
					}
				}
			}

			std::sort(actorInfo[nowIndex].teammateInfo, actorInfo[nowIndex].teammateInfo + tmpTeammateCount, compareByLevel);
			std::sort(actorInfo[nowIndex].enemyInfo, actorInfo[nowIndex].enemyInfo + tmpEnemyCount, compareByLevel);
			std::sort(actorInfo[nowIndex].npcInfo, actorInfo[nowIndex].npcInfo + tmpNpcCount, compareByLevel);

			actorInfo[nowIndex].npcCount = tmpNpcCount;
			actorInfo[nowIndex].enemyCount = tmpEnemyCount;
			actorInfo[nowIndex].teammateCount = tmpTeammateCount;
		}
	}
}

Item2Info* items = new Item2Info[2];
int nowItemIndex = 0;

std::unordered_set<int> excludeFormIds;

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
ItemInfo* getItemsBOOK()
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
ItemInfo* getItemsCONT()
{
	return items[!nowItemIndex].itemInfoCONT;
}
ItemInfo* getItemsFLOR()
{
	return items[!nowItemIndex].itemInfoFLOR;
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

[[nodiscard]] static inline bool CanDisplay(const RE::TESBoundObject& a_object)
{
	switch (a_object.GetFormType()) {
	case RE::FormType::Scroll:
	case RE::FormType::Armor:
	case RE::FormType::Book:
	case RE::FormType::Ingredient:
	case RE::FormType::Misc:
	case RE::FormType::Weapon:
	case RE::FormType::Ammo:
	case RE::FormType::KeyMaster:
	case RE::FormType::AlchemyItem:
	case RE::FormType::Note:
	case RE::FormType::SoulGem:
		break;
	case RE::FormType::Light:
		{
			auto& light = static_cast<const RE::TESObjectLIGH&>(a_object);
			if (!light.CanBeCarried()) {
				return false;
			}
		}
		break;
	default:
		return false;
	}

	if (!a_object.GetPlayable()) {
		return false;
	}

	auto name = a_object.GetName();
	if (!name || name[0] == '\0') {
		return false;
	}

	return true;
}

void __cdecl RefreshItemInfo(void*)
{
	excludeFormIds.insert(0x000319E3);  // 酒杯
	excludeFormIds.insert(0x00012FE6);  // 鼎
	excludeFormIds.insert(0x00012FE7);  // 筐
	excludeFormIds.insert(0x00012FE8);  // 筐
	excludeFormIds.insert(0x00012FE9);  // 筐
	excludeFormIds.insert(0x00012FEA);  // 筐
	excludeFormIds.insert(0x00012FEB);  // 筐
	excludeFormIds.insert(0x00012FEC);  // 筐
	excludeFormIds.insert(0x000318FA);  // 铸铁锅
	excludeFormIds.insert(0x000318FB);  // 铸铁锅
	excludeFormIds.insert(0x000318EC);  // 油灯
	excludeFormIds.insert(0x00012FDF);  // 桶
	excludeFormIds.insert(0x00031941);  // 木盘子
	excludeFormIds.insert(0x0003199A);  // 木碗
	excludeFormIds.insert(0x000319E5);  // 木杓
	excludeFormIds.insert(0x0006717F);  // 扫帚

	while (true) {
		Sleep(1000);

		if (!activeItems) {
			Sleep(2000);
			continue;
		}

		if (!startflag) {
			Sleep(3000);
			continue;
		}

		if (isGameLoading) {
			Sleep(3000);
			continue;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			Sleep(3000);
			continue;
		}

		nowItemIndex = !nowItemIndex;

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
		auto currentLocation = player->currentLocation;
		if (currentLocation) {
			auto allForms = RE::TESForm::GetAllForms();
			auto& formIDs = *allForms.first;
			//int i = formIDs.size();
			for (auto elem : formIDs) {
				auto form = elem.second;
				if (form->Is(RE::FormType::Reference)) {
					auto reff = elem.second->AsReference();
					if (reff) {
						if (reff->GetCurrentLocation() == currentLocation) {
							auto baseObj = reff->GetBaseObject();
							if (baseObj) {
								// 这些都过滤
								/*		if (baseObj->Is(RE::FormType::Static) || baseObj->Is(RE::FormType::Furniture) || baseObj->Is(RE::FormType::Tree) || baseObj->Is(RE::FormType::IdleMarker) || baseObj->Is(RE::FormType::Light) || baseObj->Is(RE::FormType::MovableStatic) || baseObj->Is(RE::FormType::Door)) {
									continue;
								}*/

								switch (baseObj->GetFormType()) {
								case RE::FormType::Weapon:
									if (reff->IsMarkedForDeletion()) {
										continue;
									}
									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
									}
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].ptr = reff;
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].baseFormId = baseObj->GetFormID();
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].baseFormIdStr = FormIDToString(baseObj->GetFormID());
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].formId = reff->GetFormID();
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].formIdStr = FormIDToString(reff->GetFormID());
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].name = reff->GetDisplayFullName();
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].weight = reff->GetWeight();
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].gold = baseObj->GetGoldValue();
									items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].isCrime = reff->IsCrimeToActivate();

									/*		logger::debug(GetFormTypeName(baseObj->formType.underlying()));
									logger::debug(std::to_string(tmpCountWEAP) + " " + StringUtil::Utf8ToGbk(reff->GetDisplayFullName()));
									logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));*/

									tmpCountWEAP++;

									break;
								case RE::FormType::Armor:

									if (reff->IsMarkedForDeletion()) {
										continue;
									}
									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
									}
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].ptr = reff;
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].baseFormId = baseObj->GetFormID();
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].baseFormIdStr = FormIDToString(baseObj->GetFormID());
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].formId = reff->GetFormID();
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].formIdStr = FormIDToString(reff->GetFormID());
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].name = reff->GetDisplayFullName();
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].weight = reff->GetWeight();
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].gold = baseObj->GetGoldValue();
									items[nowItemIndex].itemInfoARMO[tmpCountARMO].isCrime = reff->IsCrimeToActivate();

									/*					logger::debug(GetFormTypeName(baseObj->formType.underlying()));
									logger::debug(std::to_string(tmpCountARMO) + " " + StringUtil::Utf8ToGbk(reff->GetDisplayFullName()));
									logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));*/

									tmpCountARMO++;

									break;
								case RE::FormType::Ammo:

									if (reff->IsMarkedForDeletion()) {
										continue;
									}
									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
									}
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].ptr = reff;
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].baseFormId = baseObj->GetFormID();
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].baseFormIdStr = FormIDToString(baseObj->GetFormID());
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].formId = reff->GetFormID();
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].formIdStr = FormIDToString(reff->GetFormID());
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].name = reff->GetDisplayFullName();
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].weight = reff->GetWeight();
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].gold = baseObj->GetGoldValue();
									items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].isCrime = reff->IsCrimeToActivate();

									/*			logger::debug(GetFormTypeName(baseObj->formType.underlying()));
									logger::debug(std::to_string(tmpCountAMMO) + " " + StringUtil::Utf8ToGbk(reff->GetDisplayFullName()));
									logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));*/

									tmpCountAMMO++;

									break;
								case RE::FormType::Book:

									if (reff->IsMarkedForDeletion()) {
										continue;
									}
									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
									}
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].ptr = reff;
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].baseFormId = baseObj->GetFormID();
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].baseFormIdStr = FormIDToString(baseObj->GetFormID());
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formId = reff->GetFormID();
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formIdStr = FormIDToString(reff->GetFormID());
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].name = reff->GetDisplayFullName();
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].weight = reff->GetWeight();
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].gold = baseObj->GetGoldValue();
									items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].isCrime = reff->IsCrimeToActivate();

									/*		logger::debug(GetFormTypeName(baseObj->formType.underlying()));
									logger::debug(std::to_string(tmpCountBOOK) + " " + StringUtil::Utf8ToGbk(reff->GetDisplayFullName()));
									logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));*/

									tmpCountBOOK++;

									break;
								case RE::FormType::AlchemyItem:

									if (reff->IsMarkedForDeletion()) {
										continue;
									}
									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
									}
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].ptr = reff;
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].baseFormId = baseObj->GetFormID();
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].baseFormIdStr = FormIDToString(baseObj->GetFormID());
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].formId = reff->GetFormID();
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].formIdStr = FormIDToString(reff->GetFormID());
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].name = reff->GetDisplayFullName();
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].weight = reff->GetWeight();
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].gold = baseObj->GetGoldValue();
									items[nowItemIndex].itemInfoALCH[tmpCountALCH].isCrime = reff->IsCrimeToActivate();

									tmpCountALCH++;

									break;
								case RE::FormType::Ingredient:

									if (reff->IsMarkedForDeletion()) {
										continue;
									}
									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
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
									items[nowItemIndex].itemInfoINGR[tmpCountINGR].isCrime = reff->IsCrimeToActivate();

									tmpCountINGR++;
									break;
								case RE::FormType::Misc:
									{
										if (reff->IsMarkedForDeletion()) {
											continue;
										}
										if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
											continue;
										}
										auto name = reff->GetDisplayFullName();
										if (strlen(name) == 0) {
											continue;
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
										items[nowItemIndex].itemInfoMISC[tmpCountMISC].isCrime = reff->IsCrimeToActivate();

										tmpCountMISC++;

										break;
									}
								case RE::FormType::Container:
									{
										if (reff->IsMarkedForDeletion()) {
											continue;
										}
										if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
											continue;
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
										items[nowItemIndex].itemInfoCONT[tmpCountCONT].isCrime = reff->IsCrimeToActivate();
										items[nowItemIndex].itemInfoCONT[tmpCountCONT].lockLevel = reff->GetLockLevel();

										int tmpInvCount = 0;
										auto inv = reff->GetInventory(CanDisplay);
										for (auto& [obj, data] : inv) {
											auto& [count, entry] = data;
											if (count > 0 && entry) {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].ptr = entry.get();
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
										if (reff->IsMarkedForDeletion()) {
											continue;
										}
										if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
											continue;
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
										items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].isCrime = reff->IsCrimeToActivate();
										items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].isHarvested = (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested);
										if (reff->HasContainer()) {
											int tmpInvCount = 0;
										}

							/*			int tmpInvCount = 0;
										auto inv = reff->GetInventory(CanDisplay);
										for (auto& [obj, data] : inv) {
											auto& [count, entry] = data;
											if (count > 0 && entry) {
												items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].invs[tmpInvCount].ptr = entry.get();
												items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].invs[tmpInvCount++].count = count;
												if (tmpInvCount == 200) {
													break;
												}
											}
										}
										items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].invCount = tmpInvCount;*/

										tmpCountFLOR++;

										break;
									}
								case RE::FormType::Static:
								case RE::FormType::Furniture:
								case RE::FormType::Tree:
								case RE::FormType::IdleMarker:
								case RE::FormType::Light:
								case RE::FormType::MovableStatic:
								case RE::FormType::Door:
								case RE::FormType::Activator:
								case RE::FormType::TextureSet:
								case RE::FormType::Sound:
								case RE::FormType::Explosion:
								case RE::FormType::AcousticSpace:  // 升学空间
									continue;
								default:

									if (reff->IsMarkedForDeletion()) { /*
										logger::debug(StringUtil::Utf8ToGbk(reff->GetDisplayFullName()) + " " + GetFormTypeName(baseObj->formType.underlying()));
										logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));*/
										continue;
									}

									if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
										continue;
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
									items[nowItemIndex].itemInfo[tmpCount].isCrime = reff->IsCrimeToActivate();

									//logger::debug(GetFormTypeName(baseObj->formType.underlying()));
									//logger::debug(std::to_string(tmpCount) + " " + StringUtil::Utf8ToGbk(reff->GetDisplayFullName()));
									//logger::debug(std::to_string(baseObj->GetFormID()) + " " + FormIDToString(baseObj->GetFormID()));

									tmpCount++;
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
		std::sort(items[nowItemIndex].itemInfoCONT, items[nowItemIndex].itemInfoCONT + tmpCountCONT, compareForItem);
		std::sort(items[nowItemIndex].itemInfoFLOR, items[nowItemIndex].itemInfoFLOR + tmpCountFLOR, compareForItem);

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
	}
}
