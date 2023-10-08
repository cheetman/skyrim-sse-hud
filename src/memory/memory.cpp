#include "memory.h"
#include <event/BSTMenuEvent.h>
#include <setting/setting.h>
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

int screenWidth = 0;
int screenHeight = 0;
//RECT oldRect;

//#define POINTER_SKYRIMSE(className, variableName, ...) static VersionDbPtr<className> variableName(__VA_ARGS__)
//
//static float (*CameraWorldToCam)[4][4] = nullptr;
//static const RE::NiRect<float>* CameraPort = nullptr;
//bool WorldPtToScreenPt3(const RE::NiPoint3& aWorldPt, RE::NiPoint3& aScreenPt)
//{
//	return RE::NiCamera::WorldPtToScreenPt3(reinterpret_cast<float*>(CameraWorldToCam), CameraPort, &aWorldPt, &aScreenPt.x, &aScreenPt.y, &aScreenPt.z, 1e-5f);
//}

void __cdecl RefreshGameInfo(void*)
{
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

Actor2Info* actorInfo = new Actor2Info[2];
//Actor2Info actorInfo[2];
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

bool compareForItemCONT(const ItemInfoCONT& info1, const ItemInfoCONT& info2)
{
	if (info1.baseFormId != info2.baseFormId) {
		return info1.baseFormId > info2.baseFormId;
	} else {
		return info1.invCount > info2.invCount;
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

int show_npc_window_dis_meter = 30;
bool show_npc_window_dis = false;
bool show_npc_window_direction = false;
int show_npc_window_array_max_length = 98;
bool show_enemy_window = false;
bool show_inv_window = false;
bool show_npc_window = false;
bool show_items_window = false;
bool show_items_window_settings = false;
bool show_items_window_formid = false;
bool show_items_window_direction = false;
bool show_items_window_file = false;
bool show_items_window_ignore = true;
bool show_items_window_auto_ignore = true;
bool show_items_window_auto_notification = true;
bool show_items_window_auto_ammo = false;
bool show_items_window_auto_weap = false;
bool show_items_window_auto_armo = false;
bool show_items_window_auto_flor = false;
bool show_items_window_auto_tree = false;
//bool show_items_window_auto_acti = false;
bool show_items_window_auto_food = false;
bool show_items_window_auto_ingr = false;
bool show_items_window_auto_alch = false;
bool show_items_window_auto_misc = false;
bool show_items_window_auto_sgem = false;
bool show_items_window_auto_achr = false;
bool show_items_window_auto_cont = false;
bool show_items_window_auto_achr_ingr = false;
bool show_items_window_auto_achr_food = false;
bool show_items_window_auto_achr_alch = false;
bool show_items_window_auto_achr_sgem = false;
bool show_items_window_auto_achr_ammo = false;
bool show_items_window_auto_achr_scrl = false;
bool show_items_window_auto_achr_keym = false;
bool show_items_window_auto_achr_misc = false;
bool show_items_window_auto_achr_gold = false;
bool show_items_window_auto_achr_weap = false;
bool show_items_window_auto_achr_armo = false;
bool show_items_window_auto_cont_ingr = false;
bool show_items_window_auto_cont_food = false;
bool show_items_window_auto_cont_alch = false;
bool show_items_window_auto_cont_sgem = false;
bool show_items_window_auto_cont_ammo = false;
bool show_items_window_auto_cont_scrl = false;
bool show_items_window_auto_cont_keym = false;
bool show_items_window_auto_cont_misc = false;
bool show_items_window_auto_cont_gold = false;
bool show_items_window_auto_cont_weap = false;
bool show_items_window_auto_cont_armo = false;

bool show_items_window_auto_weap_enchant = false;
bool show_items_window_auto_weap_price = false;
bool show_items_window_auto_armo_enchant = false;
bool show_items_window_auto_armo_price = false;
int show_items_window_auto_weap_price_value = 500;
int show_items_window_auto_armo_price_value = 500;
//bool show_items_window_auto = true;  //暂时用不到
//bool show_items_window_auto_setting = true;
int show_items_window_auto_dis = 2;
int show_items_window_auto_dis_skyrim = 100;
int show_items_window_auto_dis_local = 160;
int show_items_window_array_max_length = 2998;

// 艺术馆
bool show_items_window_gallery = false;

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

						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].formId = actor->GetFormID();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].ptr = actor;
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].level = actor->GetLevel();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].teammateInfo[tmpTeammateCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);

						if (show_npc_window_direction) {
							actorInfo[nowIndex].teammateInfo[tmpTeammateCount].distance = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
							actorInfo[nowIndex].teammateInfo[tmpTeammateCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
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
						float dis = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}
						if (show_npc_window_direction) {
							actorInfo[nowIndex].enemyInfo[tmpEnemyCount].distance = dis;
							actorInfo[nowIndex].enemyInfo[tmpEnemyCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
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

					} else if (actor->IsHorse()) {
						if (tmpHorseCount > show_npc_window_array_max_length) {
							continue;
						}

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
						if (show_npc_window_direction) {
							actorInfo[nowIndex].horseInfo[tmpHorseCount].distance = dis;
							actorInfo[nowIndex].horseInfo[tmpHorseCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
						actorInfo[nowIndex].horseInfo[tmpHorseCount].formId = actor->GetFormID();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].horseInfo[tmpHorseCount].ptr = actor;
						actorInfo[nowIndex].horseInfo[tmpHorseCount].level = actor->GetLevel();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].horseInfo[tmpHorseCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].horseInfo[tmpHorseCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].horseInfo[tmpHorseCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].horseInfo[tmpHorseCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
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
						float dis = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;
						if (show_npc_window_dis) {
							if (dis > show_npc_window_dis_meter) {
								continue;
							}
						}

						if (show_npc_window_direction) {
							actorInfo[nowIndex].npcInfo[tmpNpcCount].distance = dis;
							actorInfo[nowIndex].npcInfo[tmpNpcCount].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
						}
						actorInfo[nowIndex].npcInfo[tmpNpcCount].formId = actor->GetFormID();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].formIdStr = FormIDToString(actor->GetFormID());
						actorInfo[nowIndex].npcInfo[tmpNpcCount].ptr = actor;
						actorInfo[nowIndex].npcInfo[tmpNpcCount].level = actor->GetLevel();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].name = actor->GetDisplayFullName();
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealthBase = actor->GetPermanentActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].isSentient = IsSentient2(actor);
						actorInfo[nowIndex].npcInfo[tmpNpcCount].kHealth = actor->GetActorValue(RE::ActorValue::kHealth);
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

Item2Info* items = new Item2Info[2];
int nowItemIndex = 0;

// 物品排除
std::unordered_set<int> excludeFormIds;
std::vector<ExcludeForm> excludeForms;
// 地点排除
std::unordered_set<int> excludeLocationFormIds;
std::vector<ExcludeForm> excludeLocationForms;
// 容器类型
std::unordered_set<int> autoContFormIds;
std::vector<IncludeForm> autoContForms;
std::unordered_set<RE::TESObjectREFR*> deleteREFRs;

// 艺术馆
std::unordered_set<int> galleryFormIds;
std::vector<GalleryForm> galleryFormData;
std::vector<GalleryModForm> galleryFormModData;
//int galleryTotalCount = 0;
//int galleryCount = 0;

//std::vector<ItemInfo> tracks;
std::unordered_set<RE::TESObjectREFR*> trackPtrs;
std::unordered_set<RE::Actor*> trackActorPtrs;
bool excludeFormsInitFlag = true;

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



bool __fastcall autoTakeArmo(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance, bool isDeleteExist)
{
	// 自动拾取
	if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!reff->IsMarkedForDeletion()) {
					// 增加判断
					if (show_items_window_auto_armo_enchant) {
						if (!reff->IsEnchanted()) {
							return false;
						}
					}

					if (show_items_window_auto_armo_price) {
						if (reff->GetObjectReference()->GetGoldValue() < show_items_window_auto_armo_price_value) {
							return false;
						}
					}

					if (show_items_window_auto_ignore) {
						// 判断地点忽略
						int formID = 0;
						if (player->currentLocation) {
							formID = player->currentLocation->GetFormID();
						}
						if (excludeLocationFormIds.find(formID) == excludeLocationFormIds.end()) {
							if (!isDeleteExist) {
								deleteREFRs.insert(reff);
							}
							return true;
						}
					} else {
						if (!isDeleteExist) {
							deleteREFRs.insert(reff);
						}
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoTakeWeap(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance, bool isDeleteExist)
{
	// 自动拾取
	if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!reff->IsMarkedForDeletion()) {
					// 增加判断
					if (show_items_window_auto_weap_enchant) {
						if (!reff->IsEnchanted()) {
							return false;
						}
					}

					if (show_items_window_auto_weap_price) {
						if (reff->GetObjectReference()->GetGoldValue() < show_items_window_auto_weap_price_value) {
							return false;
						}
					}

					if (show_items_window_auto_ignore) {
						// 判断地点忽略
						int formID = 0;
						if (player->currentLocation) {
							formID = player->currentLocation->GetFormID();
						}
						if (excludeLocationFormIds.find(formID) == excludeLocationFormIds.end()) {
							if (!isDeleteExist) {
								deleteREFRs.insert(reff);
							}
							return true;
						}
					} else {
						if (!isDeleteExist) {
							deleteREFRs.insert(reff);
						}
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoTake(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance, bool isDeleteExist)
{
	// 自动拾取
	if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!reff->IsMarkedForDeletion()) {
					if (show_items_window_auto_ignore) {
						// 判断地点忽略
						int formID = 0;
						if (player->currentLocation) {
							formID = player->currentLocation->GetFormID();
						}
						if (excludeLocationFormIds.find(formID) == excludeLocationFormIds.end()) {
							if (!isDeleteExist) {
								deleteREFRs.insert(reff);
							}
							return true;
						}
					} else {
						if (!isDeleteExist) {
							deleteREFRs.insert(reff);
						}
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoHarvest(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance, RE::TESFlora* flora)
{
	// 自动拾取
	if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!(reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
					if (show_items_window_auto_ignore) {
						// 判断地点忽略
						int formID = 0;
						if (player->currentLocation) {
							formID = player->currentLocation->GetFormID();
						}
						if (excludeLocationFormIds.find(formID) == excludeLocationFormIds.end()) {
							if (flora) {
								flora->Activate(reff, player, 0, flora->produceItem, 1);
								if (show_items_window_auto_notification) {
									char buf[40];
									snprintf(buf, 40, "%s 已自动收获", reff->GetDisplayFullName());
									RE::DebugNotification(buf, NULL, false);
								}
							}
						}
					} else {
						if (flora) {
							flora->Activate(reff, player, 0, flora->produceItem, 1);

							if (show_items_window_auto_notification) {
								char buf[40];
								snprintf(buf, 40, "%s 已自动收获", reff->GetDisplayFullName());
								RE::DebugNotification(buf, NULL, false);
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoHarvest(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance, RE::TESObjectTREE* tree)
{
	// 自动拾取
	if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!(reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested)) {
					if (show_items_window_auto_ignore) {
						// 判断地点忽略
						int formID = 0;
						if (player->currentLocation) {
							formID = player->currentLocation->GetFormID();
						}
						if (excludeLocationFormIds.find(formID) == excludeLocationFormIds.end()) {
							if (tree) {
								tree->Activate(reff, player, 0, tree->produceItem, 1);

								if (show_items_window_auto_notification) {
									char buf[40];
									snprintf(buf, 40, "%s 已自动收获", reff->GetDisplayFullName());
									RE::DebugNotification(buf, NULL, false);
								}
							}
						}
					} else {
						if (tree) {
							tree->Activate(reff, player, 0, tree->produceItem, 1);

							if (show_items_window_auto_notification) {
								char buf[40];
								snprintf(buf, 40, "%s 已自动收获", reff->GetDisplayFullName());
								RE::DebugNotification(buf, NULL, false);
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoTakeForACHR(RE::Actor* actor, RE::TESBoundObject* obj, int count, RE::PlayerCharacter* player)
{
	if (show_items_window_ignore) {
		if (excludeFormIds.find(obj->GetFormID()) != excludeFormIds.end()) {
			return false;
		}
	}

	actor->RemoveItem(obj, count, RE::ITEM_REMOVE_REASON::kRemove, 0, player);

	if (show_items_window_auto_notification) {
		char buf[90];
		if (count > 1) {
			snprintf(buf, 80, "%s(%d) 从%s尸体自动拾取", obj->GetName(), count, actor->GetDisplayFullName());
		} else {
			snprintf(buf, 80, "%s 从%s尸体自动拾取", obj->GetName(), actor->GetDisplayFullName());
		}
		RE::DebugNotification(buf, NULL, false);
	}
	player->PlayPickUpSound(obj, true, false);
	return true;
}

bool __fastcall autoTakeForCONT(RE::TESObjectREFR* reff, RE::TESBoundObject* obj, int count, RE::PlayerCharacter* player)
{
	if (show_items_window_ignore) {
		if (excludeFormIds.find(obj->GetFormID()) != excludeFormIds.end()) {
			return false;
		}
	}

	reff->RemoveItem(obj, count, RE::ITEM_REMOVE_REASON::kRemove, 0, player);

	if (show_items_window_auto_notification) {
		char buf[90];
		if (count > 1) {
			snprintf(buf, 80, "%s(%d) 从%s自动拾取", obj->GetName(), count, reff->GetDisplayFullName());
		} else {
			snprintf(buf, 80, "%s 从%s自动拾取", obj->GetName(), reff->GetDisplayFullName());
		}
		RE::DebugNotification(buf, NULL, false);
	}
	player->PlayPickUpSound(obj, true, false);
	return true;
}

void __cdecl RefreshItemInfo(void*)
{
	while (true) {
		Sleep(1000);

		if (!activeItems && !show_items_window && !show_items_window_auto_ammo && !show_items_window_auto_flor && !show_items_window_auto_food && !show_items_window_auto_ingr && !show_items_window_auto_alch && !show_items_window_auto_misc && !show_items_window_auto_tree && !show_items_window_auto_sgem && !show_items_window_auto_achr && !show_items_window_auto_cont) {
			Sleep(1000);
			continue;
		}

		if (!startflag) {
			Sleep(1000);
			continue;
		}

		if (isGameLoading) {
			Sleep(1000);
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

		// 初始化
		if (excludeFormsInitFlag) {
			excludeFormsInitFlag = false;
			for (auto id : excludeFormIds) {
				auto form = RE::TESForm::LookupByID(id);
				if (form) {
					excludeForms.push_back({ form->GetFormID(), form->GetName(), "" });
				}
			}

			for (auto id : autoContFormIds) {
				auto form = RE::TESForm::LookupByID(id);
				if (form) {
					autoContForms.push_back({ form->GetFormID(), form->GetName(), "" });
				}
			}

			for (auto id : excludeLocationFormIds) {
				if (id == 0) {
					excludeLocationForms.push_back({ 0, "天际", "" });
					continue;
				}
				auto form = RE::TESForm::LookupByID(id);
				if (form) {
					excludeLocationForms.push_back({ form->GetFormID(), form->GetName(), "" });
				}
			}

			// 初始化艺术馆
			const auto handler = RE::TESDataHandler::GetSingleton();
			for (auto item : setting::galleryList) {
				// 先检查一下有没有mod
				auto file = handler->LookupModByName(item.filename);
				if (!file || file->compileIndex == 0xFF) {
					galleryFormModData.push_back({ -1,-1, item.filename, item.name, item.formids.size(), 0 });
					continue;
				}

				size_t itemCount = 0;
				for (auto rawFormID : item.formids) {
					RE::FormID formID = file->compileIndex << (3 * 8);
					formID += file->smallFileCompileIndex << ((1 * 8) + 4);
					formID += rawFormID;
					auto form = RE::TESForm::LookupByID(formID);
					if (form) {
						galleryFormIds.insert(formID);
						galleryFormData.push_back({ formID, form->GetName(), item.filename });
						itemCount++;
					}
				}
				galleryFormModData.push_back({ file->compileIndex, file->smallFileCompileIndex, item.filename, item.name, item.formids.size(), itemCount });
			}
		}

		bool isDeleteExist = deleteREFRs.size() > 0;

		nowItemIndex = !nowItemIndex;
		Sleep(100);

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

		auto currentLocation = player->currentLocation;
		const auto& [map, lock] = RE::TESForm::GetAllForms();
		//const RE::BSReadLockGuard locker{ lock };
		if (!map) {
			continue;
		}

		bool autoPick = true;
		// 地点忽略结果
		if (show_items_window_auto_ignore) {
			// 判断地点忽略
			int formID = 0;
			if (player->currentLocation) {
				formID = player->currentLocation->GetFormID();
			}
			// 不存在
			if (excludeLocationFormIds.find(formID) != excludeLocationFormIds.end()) {
				autoPick = false;
			}
		}

		//auto& formIDs = *allForms.first;
		//for (auto elem : formIDs) {
		for (auto& [id, form] : *map) {
			//auto form = elem.second;
			if (form) {
				if (form->Is(RE::FormType::ActorCharacter)) {
					auto actor = form->As<RE::Actor>();
					if (actor && actor->IsDead() && !actor->IsSummoned()) {
						// 排除自己
						if (actor->IsPlayerRef()) {
							continue;
						}
						if (actor->GetCurrentLocation() == currentLocation) {
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
							float distance = calculateDistance(actor->GetPosition(), player->GetPosition()) / 100.0f;

							if (!currentLocation) {
								if (distance > show_items_window_auto_dis_skyrim) {
									continue;
								}
							} else {
								if (distance > show_items_window_auto_dis_local) {
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
							items[nowItemIndex].itemInfoACHR[tmpCountACHR].isCrime = actor->IsCrimeToActivate();
							//items[nowItemIndex].itemInfoACHR[tmpCountACHR].isEnchanted = actor->IsEnchanted();

							if (show_items_window_direction) {
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].distance = distance;
								items[nowItemIndex].itemInfoACHR[tmpCountACHR].direction = calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
							}

							int tmpInvCount = 0;
							auto inv = actor->GetInventory(CanDisplay);
							for (auto& [obj, data] : inv) {
								auto& [count, entry] = data;
								if (count > 0 && entry) {
									// 自动拾取

									if (show_items_window_auto_achr && distance < show_items_window_auto_dis && autoPick) {
										switch (obj->GetFormType()) {
										case RE::FormType::Weapon:
											if (show_items_window_auto_achr_weap) {
												if (show_items_window_auto_weap_enchant) {
													if (!entry.get()->IsEnchanted()) {
														break;
													}
												}

												if (show_items_window_auto_weap_price) {
													if (obj->GetGoldValue() < show_items_window_auto_weap_price_value) {
														break;
													}
												}
												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										case RE::FormType::Armor:
											if (show_items_window_auto_achr_armo) {
												if (show_items_window_auto_armo_enchant) {
													if (!entry.get()->IsEnchanted()) {
														break;
													}
												}

												if (show_items_window_auto_armo_price) {
													if (obj->GetGoldValue() < show_items_window_auto_armo_price_value) {
														break;
													}
												}

												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										case RE::FormType::Ammo:
											if (show_items_window_auto_achr_ammo) {
												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										case RE::FormType::Scroll:
											if (show_items_window_auto_achr_scrl) {
												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										case RE::FormType::Misc:
											{
												if (obj->IsGold()) {
													if (show_items_window_auto_achr_gold) {
														if (autoTakeForACHR(actor, obj, count, player)) {
															continue;
														}
													}

												} else {
													if (show_items_window_auto_achr_misc) {
														if (autoTakeForACHR(actor, obj, count, player)) {
															continue;
														}
													}
												}
												break;
											}

										case RE::FormType::KeyMaster:
											if (show_items_window_auto_achr_keym) {
												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										case RE::FormType::AlchemyItem:
											{
												auto alchemyItem = obj->As<RE::AlchemyItem>();
												if (alchemyItem->IsFood()) {
													if (show_items_window_auto_achr_food) {
														if (autoTakeForACHR(actor, obj, count, player)) {
															continue;
														}
													}

												} else {
													if (show_items_window_auto_achr_alch) {
														if (autoTakeForACHR(actor, obj, count, player)) {
															continue;
														}
													}
												}
												break;
											}
										case RE::FormType::SoulGem:
											if (show_items_window_auto_achr_sgem) {
												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										case RE::FormType::Ingredient:
											if (show_items_window_auto_achr_ingr) {
												if (autoTakeForACHR(actor, obj, count, player)) {
													continue;
												}
											}
											break;
										default:
											break;
										}
									}
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
										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}
										auto name = reff->GetDisplayFullName();
										if (strlen(name) == 0) {
											continue;
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										// 自动拾取判断
										if (autoTakeWeap(reff, player, show_items_window_auto_weap, distance, isDeleteExist)) {
											continue;
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
										items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].isCrime = reff->IsCrimeToActivate();
										items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].isEnchanted = reff->IsEnchanted();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].distance = distance;
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].filename = baseObj->GetFile()->fileName;

											//RE::TESForm::GetAllForms;
											//logger::debug("{} - {}", reff->GetFormEditorID(), baseObj->GetFormEditorID()); // 没用
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

										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}

										auto name = reff->GetDisplayFullName();
										if (strlen(name) == 0) {
											continue;
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										// 自动拾取判断
										if (autoTakeArmo(reff, player, show_items_window_auto_armo, distance, isDeleteExist)) {
											continue;
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
										items[nowItemIndex].itemInfoARMO[tmpCountARMO].isCrime = reff->IsCrimeToActivate();
										items[nowItemIndex].itemInfoARMO[tmpCountARMO].isEnchanted = reff->IsEnchanted();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].distance = distance;
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoARMO[tmpCountARMO].filename = baseObj->GetFile()->fileName;
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
										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}
										auto name = reff->GetDisplayFullName();
										if (strlen(name) == 0) {
											continue;
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										// 自动拾取判断
										if (autoTake(reff, player, show_items_window_auto_ammo, distance, isDeleteExist)) {
											continue;
										}

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].distance = distance;
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
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
										items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].isCrime = reff->IsCrimeToActivate();

										if (show_items_window_file) {
											items[nowItemIndex].itemInfoAMMO[tmpCountAMMO].filename = baseObj->GetFile()->fileName;
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
										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
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

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].distance = distance;
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoBOOK[tmpCountBOOK].filename = baseObj->GetFile()->fileName;
										}

										tmpCountBOOK++;

										break;
									}
								case RE::FormType::AlchemyItem:
									{
										if (reff->IsMarkedForDeletion()) {
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
										auto alchemyItem = baseObj->As<RE::AlchemyItem>();
										if (alchemyItem->IsFood()) {
											if (tmpCountFOOD > show_items_window_array_max_length) {
												continue;
											}

											float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

											if (!currentLocation) {
												if (distance > show_items_window_auto_dis_skyrim) {
													continue;
												}
											} else {
												if (distance > show_items_window_auto_dis_local) {
													continue;
												}
											}

											// 自动拾取判断
											if (autoTake(reff, player, show_items_window_auto_food, distance, isDeleteExist)) {
												continue;
											}

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].distance = distance;
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
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
											items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].isCrime = reff->IsCrimeToActivate();

											if (show_items_window_file) {
												items[nowItemIndex].itemInfoFOOD[tmpCountFOOD].filename = baseObj->GetFile()->fileName;
											}
											tmpCountFOOD++;
										} else {
											if (tmpCountALCH > show_items_window_array_max_length) {
												continue;
											}

											float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

											if (!currentLocation) {
												if (distance > show_items_window_auto_dis_skyrim) {
													continue;
												}
											} else {
												if (distance > show_items_window_auto_dis_local) {
													continue;
												}
											}

											// 自动拾取判断
											if (autoTake(reff, player, show_items_window_auto_alch, distance, isDeleteExist)) {
												continue;
											}

											if (show_items_window_direction) {
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].distance = distance;
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
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
											items[nowItemIndex].itemInfoALCH[tmpCountALCH].isCrime = reff->IsCrimeToActivate();

											if (show_items_window_file) {
												items[nowItemIndex].itemInfoALCH[tmpCountALCH].filename = baseObj->GetFile()->fileName;
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
										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										if (autoTake(reff, player, show_items_window_auto_ingr, distance, isDeleteExist)) {
											continue;
										}

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].distance = distance;
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
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

										if (show_items_window_file) {
											items[nowItemIndex].itemInfoINGR[tmpCountINGR].filename = baseObj->GetFile()->fileName;
										}
										tmpCountINGR++;
										break;
									}
								case RE::FormType::Misc:
									{
										if (tmpCountMISC > show_items_window_array_max_length) {
											continue;
										}

										if (reff->IsMarkedForDeletion()) {
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

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										// 自动拾取判断
										if (autoTake(reff, player, show_items_window_auto_misc, distance, isDeleteExist)) {
											continue;
										}

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoMISC[tmpCountMISC].distance = distance;
											items[nowItemIndex].itemInfoMISC[tmpCountMISC].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
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

										if (show_items_window_file) {
											items[nowItemIndex].itemInfoMISC[tmpCountMISC].filename = baseObj->GetFile()->fileName;
										}
										tmpCountMISC++;

										break;
									}
								case RE::FormType::Container:
									{
										if (tmpCountCONT > show_items_window_array_max_length) {
											continue;
										}

										if (reff->IsMarkedForDeletion()) {
											continue;
										}
										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
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
										bool isCrime = reff->IsCrimeToActivate();
										items[nowItemIndex].itemInfoCONT[tmpCountCONT].isCrime = isCrime;
										items[nowItemIndex].itemInfoCONT[tmpCountCONT].lockLevel = reff->GetLockLevel();
										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].distance = distance;
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].filename = baseObj->GetFile()->fileName;
										}

										int tmpInvCount = 0;
										auto inv = reff->GetInventory(CanDisplay);

										// 自动拾取条件1
										bool auto1 = false;
										if (show_items_window_auto_cont) {
											if (autoContFormIds.find(baseObj->GetFormID()) != autoContFormIds.end()) {
												auto1 = true;
											}
										}
										items[nowItemIndex].itemInfoCONT[tmpCountCONT].isAuto = auto1;
										// 满足自动拾取(加上距离条件和地点条件)
										if (auto1 && distance < show_items_window_auto_dis && autoPick && !isCrime) {
											for (auto& [obj, data] : inv) {
												auto& [count, entry] = data;
												if (count > 0 && entry) {
													bool stealing = player->WouldBeStealing(reff);
													bool isCrimeInv = !entry.get()->IsOwnedBy(player, !stealing);
													// 并且不犯罪 判断是否拾取
													if (!isCrimeInv) {
														switch (obj->GetFormType()) {
														case RE::FormType::Weapon:
															if (show_items_window_auto_cont_weap) {
																if (show_items_window_auto_weap_enchant) {
																	if (!entry.get()->IsEnchanted()) {
																		break;
																	}
																}

																if (show_items_window_auto_weap_price) {
																	if (obj->GetGoldValue() < show_items_window_auto_weap_price_value) {
																		break;
																	}
																}
																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														case RE::FormType::Armor:
															if (show_items_window_auto_cont_armo) {
																if (show_items_window_auto_armo_enchant) {
																	if (!entry.get()->IsEnchanted()) {
																		break;
																	}
																}

																if (show_items_window_auto_armo_price) {
																	if (obj->GetGoldValue() < show_items_window_auto_armo_price_value) {
																		break;
																	}
																}

																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														case RE::FormType::Ammo:
															if (show_items_window_auto_cont_ammo) {
																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														case RE::FormType::Scroll:
															if (show_items_window_auto_cont_scrl) {
																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														case RE::FormType::Misc:
															{
																if (obj->IsGold()) {
																	if (show_items_window_auto_cont_gold) {
																		if (autoTakeForCONT(reff, obj, count, player)) {
																			continue;
																		}
																	}

																} else {
																	if (show_items_window_auto_cont_misc) {
																		if (autoTakeForCONT(reff, obj, count, player)) {
																			continue;
																		}
																	}
																}
																break;
															}

														case RE::FormType::KeyMaster:
															if (show_items_window_auto_cont_keym) {
																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														case RE::FormType::AlchemyItem:
															{
																auto alchemyItem = obj->As<RE::AlchemyItem>();
																if (alchemyItem->IsFood()) {
																	if (show_items_window_auto_cont_food) {
																		if (autoTakeForCONT(reff, obj, count, player)) {
																			continue;
																		}
																	}

																} else {
																	if (show_items_window_auto_cont_alch) {
																		if (autoTakeForCONT(reff, obj, count, player)) {
																			continue;
																		}
																	}
																}
																break;
															}
														case RE::FormType::SoulGem:
															if (show_items_window_auto_cont_sgem) {
																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														case RE::FormType::Ingredient:
															if (show_items_window_auto_cont_ingr) {
																if (autoTakeForCONT(reff, obj, count, player)) {
																	continue;
																}
															}
															break;
														default:
															break;
														}
													}

													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].ptr = obj;
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].name = obj->GetName();
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].isCrime = isCrimeInv;
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].isEnchanted = entry.get()->IsEnchanted();
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount++].count = count;

													if (tmpInvCount == 200) {
														break;
													}
												}
											}
										} else {
											for (auto& [obj, data] : inv) {
												auto& [count, entry] = data;
												if (count > 0 && entry) {
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

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
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
										items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].isCrime = reff->IsCrimeToActivate();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].distance = distance;
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoFLOR[tmpCountFLOR].filename = baseObj->GetFile()->fileName;
										}

										// 自动拾取判断
										if (autoHarvest(reff, player, show_items_window_auto_flor, distance, flora)) {
											continue;
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

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
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
										items[nowItemIndex].itemInfoTREE[tmpCountTREE].isCrime = reff->IsCrimeToActivate();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].distance = distance;
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoTREE[tmpCountTREE].filename = baseObj->GetFile()->fileName;
										}

										// 自动拾取判断
										if (autoHarvest(reff, player, show_items_window_auto_tree, distance, tree)) {
											continue;
										}

										//// 自动拾取
										//if ((show_items_window_auto && show_items_window_auto_tree)) {
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
										//				if (tree) {
										//					tree->Activate(reff, player, 0, tree->produceItem, 1);
										//					char buf[40];
										//					snprintf(buf, 40, "%s 已自动收获", reff->GetDisplayFullName());
										//					RE::DebugNotification(buf, NULL, false);
										//				}
										//			}
										//		}
										//	}
										//}

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
										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
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
										items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].isCrime = reff->IsCrimeToActivate();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].distance = distance;
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoKEYM[tmpCountKEYM].filename = baseObj->GetFile()->fileName;
										}

										tmpCountKEYM++;
										break;
									}
								case RE::FormType::Activator:
									{
										if (tmpCountACTI > show_items_window_array_max_length) {
											continue;
										}

										if (reff->IsMarkedForDeletion()) {
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

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										items[nowItemIndex].itemInfoACTI[tmpCountACTI].ptr = reff;
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].baseFormId = baseObj->GetFormID();
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].baseFormIdStr = FormIDToString(baseObj->GetFormID());
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].formId = reff->GetFormID();
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].formIdStr = FormIDToString(reff->GetFormID());
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].formTypeStr = GetFormTypeName(baseObj->formType.underlying());
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].name = reff->GetDisplayFullName();
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].weight = reff->GetWeight();
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].gold = baseObj->GetGoldValue();
										items[nowItemIndex].itemInfoACTI[tmpCountACTI].isCrime = reff->IsCrimeToActivate();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].distance = distance;
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoACTI[tmpCountACTI].filename = baseObj->GetFile()->fileName;
										}

										items[nowItemIndex].itemInfoACTI[tmpCountACTI].isHarvested = (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested);

										tmpCountACTI++;

										break;
									}
								case RE::FormType::SoulGem:
									{
										if (tmpCountSGEM > show_items_window_array_max_length) {
											continue;
										}

										if (reff->IsMarkedForDeletion()) {
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

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
												continue;
											}
										}

										// 自动拾取判断
										if (autoTake(reff, player, show_items_window_auto_sgem, distance, isDeleteExist)) {
											continue;
										}

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].distance = distance;
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].filename = baseObj->GetFile()->fileName;
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
										items[nowItemIndex].itemInfoSGEM[tmpCountSGEM].isCrime = reff->IsCrimeToActivate();

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

										if (show_items_window_ignore) {
											if (excludeFormIds.find(baseObj->GetFormID()) != excludeFormIds.end()) {
												continue;
											}
										}

										float distance = calculateDistance(reff->GetPosition(), player->GetPosition()) / 100.0f;

										if (!currentLocation) {
											if (distance > show_items_window_auto_dis_skyrim) {
												continue;
											}
										} else {
											if (distance > show_items_window_auto_dis_local) {
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
										items[nowItemIndex].itemInfo[tmpCount].isCrime = reff->IsCrimeToActivate();

										if (show_items_window_direction) {
											items[nowItemIndex].itemInfo[tmpCount].distance = distance;
											items[nowItemIndex].itemInfo[tmpCount].direction = calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
										}
										if (show_items_window_file) {
											items[nowItemIndex].itemInfo[tmpCount].filename = baseObj->GetFile()->fileName;
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
		//}

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

		std::sort(items[nowItemIndex].itemInfoACHR, items[nowItemIndex].itemInfoACHR + tmpCountACHR, compareForItemCONT);

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

		items[nowItemIndex].itemCountACHR = tmpCountACHR;
	}
}
