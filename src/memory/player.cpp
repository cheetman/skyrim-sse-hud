#include "player.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <memory/stat.h>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
#include <utils/PlayerDataProvider.h>
#include <utils/utils.h>

bool show_player_base_info_window = false;
bool show_player_base_info_window_max_fix = true;
bool show_player_mod_window = false;
bool show_player_info_window = false;
bool show_player_armor_window = false;
bool show_player_weapon_window = false;
bool show_player_gold_window = false;
bool show_player_carryweight_window = false;
bool show_player_xp_window = false;
bool show_player_effects_window = false;
bool show_player_effects_ignore_permanent = false;
bool show_player_effects_negative = true;
bool show_player_effects_listignore = false;
bool show_player_effects_ignore_spell = false;
bool show_player_effects_process = false;

ImVec4 colorProgressEffect1(0.0f, 0.8f, 0.0f, 1.0f);
//ImVec4 colorProgressEffect2(1.0f, 0.5f, 0.0f, 1.0f);
ImVec4 colorProgressEffect3(0.8f, 0.0f, 0.0f, 1.0f);

PlayerInfo playerInfo;
WeaponInfo leftWeaponInfo;
WeaponInfo rightWeaponInfo;
WeaponInfo ammoInfo;
WeaponInfo powerInfo;
ArmorInfo wornArmos[32];

Effects2Info* effectsInfo = new Effects2Info[2];
int nowEffectsIndex = 0;

std::vector<EffectInfo>& getEffects()
{
	return effectsInfo[!nowEffectsIndex].list;
}

int getEffectsCount()
{
	return effectsInfo[!nowEffectsIndex].count;
}

std::unordered_set<ExcludeFormEffectIds, ExcludeFormEffectIdsHash> excludeEffectFormIds;
std::vector<ExcludeFormEffect> excludeEffectForms;

void refreshPlayerInfo()
{
	//__try {
	//auto playerFormEditorID = player->GetFormEditorID();
	//auto playerFormID = player->GetFormID();
	//auto playerGoldValue = player->GetGoldValue();
	// 用不到
	//auto playerGoldAmount = player->GetGoldAmount();
	//auto playerDisplayFullName = player->GetDisplayFullName();

	//auto playerLevel = player->GetLevel();
	//auto playerRace = player->GetRace();
	//if (playerRace) {
	//playerRaceName = player->GetRace()->GetFullName();
	//}

	//auto playerAttackingWeapon = player->GetAttackingWeapon();
	//auto playerEquippedEntryDataLeft = player->GetEquippedEntryData(true);
	//auto playerEquippedEntryDataRight = player->GetEquippedEntryData(false);
	//auto playerEquippedObjectLeft = player->GetEquippedObject(true);
	//auto playerEquippedObjectRight = player->GetEquippedObject(false);

	//auto id = player->GetCrimeGoldValue();
	//auto factionOwner = player->GetFactionOwner();
	//if (factionOwner) {
	//	auto factionOwnerName = factionOwner->GetFullName();
	//}
	// 轻甲等级
	//lightArmor = player->GetActorValue(RE::ActorValue::kLightArmor);
	//// 重甲等级
	//heavyArmor = player->GetActorValue(RE::ActorValue::kHeavyArmor);
	//
	//player->GetLevel();

	auto player = RE::PlayerCharacter::GetSingleton();
	if (player) {
		auto currentLocation = player->currentLocation;
		if (currentLocation) {
			playerInfo.location = currentLocation->GetFullName();
			playerInfo.locationId = currentLocation->GetFormID();
			if (currentLocation->parentLoc) {
				playerInfo.parentLocation = currentLocation->parentLoc->GetFullName();
				playerInfo.parentLocationId = currentLocation->parentLoc->GetFormID();
			} else {
				playerInfo.parentLocationId = -1;
			}
		} else {
			playerInfo.location = "天际";
			playerInfo.parentLocationId = -1;
		}

		auto cell = player->GetParentCell();
		playerInfo.cellPtr = cell;
		if (cell) {
			playerInfo.cellName = cell->GetFullName();
		} else {
			playerInfo.cellName = "";
		}

		playerInfo.Angle = player->GetAngle();
		playerInfo.Position = player->GetPosition();

		// 基础信息
		if (show_player_base_info_window) {
			//playerInfo.equippedWeight = player->equippedWeight;
			//playerInfo.carryWeight = player->GetActorValue(RE::ActorValue::kCarryWeight);

			playerInfo.kHealth = player->GetActorValue(RE::ActorValue::kHealth);
			playerInfo.kMagicka = player->GetActorValue(RE::ActorValue::kMagicka);
			playerInfo.kStamina = player->GetActorValue(RE::ActorValue::kStamina);

			if (show_player_base_info_window_max_fix) {
				playerInfo.kHealthBase = player->GetPermanentActorValue(RE::ActorValue::kHealth) + player->healthModifiers.modifiers[1];
				playerInfo.kStaminaBase = player->GetPermanentActorValue(RE::ActorValue::kStamina) + player->staminaModifiers.modifiers[1];
				playerInfo.kMagickaBase = player->GetPermanentActorValue(RE::ActorValue::kMagicka) + player->magickaModifiers.modifiers[1];
			} else {
				playerInfo.kHealthBase = player->GetPermanentActorValue(RE::ActorValue::kHealth);
				playerInfo.kStaminaBase = player->GetPermanentActorValue(RE::ActorValue::kStamina);
				playerInfo.kMagickaBase = player->GetPermanentActorValue(RE::ActorValue::kMagicka);
			}

			//player->healthModifiers.modifiers;
			//(*player)->actorValueOwner.GetMaximum(24);

			//playerInfo.kHealthBase = player->GetBaseActorValue(RE::ActorValue::kHealth);
			//playerInfo.kStaminaBase = player->GetBaseActorValue(RE::ActorValue::kStamina);
			//playerInfo.kMagickaBase = player->GetBaseActorValue(RE::ActorValue::kMagicka);

			//playerInfo.kHealthBase = player->GetClampedActorValue(RE::ActorValue::kHealth);
			//playerInfo.kStaminaBase = player->GetClampedActorValue(RE::ActorValue::kStamina);
			//playerInfo.kMagickaBase = player->GetClampedActorValue(RE::ActorValue::kMagicka);

			// 生命恢复速率
			//auto kHealRate = player->GetActorValue(RE::ActorValue::kHealRate);
			// 战斗中生命恢复速率
			//auto kCombatHealthRegenMultiply = player->GetActorValue(RE::ActorValue::kCombatHealthRegenMultiply);
			// 魔法恢复速率
			//auto kMagickaRate = player->GetActorValue(RE::ActorValue::kMagickaRate);
			// 体力恢复速率
			//auto KStaminaRate = player->GetActorValue(RE::ActorValue::KStaminaRate);
		}

		// 技能信息
		if (show_player_mod_window) {
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
		}

		// 属性信息
		if (show_player_info_window) {
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

			// 武器伤害
			//kMeleeDamage = player->GetActorValue(RE::ActorValue::kMeleeDamage);
			// 空手伤害
			//kUnarmedDamage = player->GetActorValue(RE::ActorValue::kUnarmedDamage);
		}

		// 装备信息
		if (show_player_armor_window) {
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
		}

		// 武器信息
		if (show_player_weapon_window) {
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

			// 龙吼
			auto power = player->selectedPower;
			if (power) {
				if (power->Is(RE::FormType::Spell)) {
					auto item = power->As<RE::SpellItem>();
					powerInfo.isExist = true;
					powerInfo.name = item->GetName();
					powerInfo.goldValue = item->GetGoldValue();
					powerInfo.castingTypeName = GetCastingTypeName(item->GetCastingType());
					powerInfo.spellTypeName = GetSpellTypeName(item->GetSpellType());
					powerInfo.cost = item->CalculateMagickaCost(player);
					powerInfo.time = item->GetChargeTime();
					auto tmp = std::string(powerInfo.name);
					tmp.append(" - ");
					tmp.append(powerInfo.castingTypeName);
					powerInfo.treeId = tmp;

				} else if (power->Is(RE::FormType::Shout)) {
					const auto item = power->As<RE::TESShout>();
					powerInfo.isExist = true;
					powerInfo.name = item->GetName();
					powerInfo.formType = item->GetFormType();
					powerInfo.formTypeName = GetFormTypeName(item->formType.underlying());
					powerInfo.formID = FormIDToString(item->GetFormID());
					auto tmp = std::string(powerInfo.name);
					powerInfo.treeId = tmp;

				} else {
					powerInfo.isExist = false;
				}

			} else {
				powerInfo.isExist = false;
			}
		}

		// 金钱
		if (show_player_gold_window) {
			playerInfo.gold = player->GetGoldAmount();
		}

		// 负重
		if (show_player_carryweight_window) {
			//playerInfo.equippedWeight = player->GetWeight();
			playerInfo.equippedWeight = player->GetWeightInContainer();
			playerInfo.carryWeight = player->GetActorValue(RE::ActorValue::kCarryWeight);
		}

		// 经验
		if (show_player_xp_window) {
			playerInfo.xp = player->skills->data->xp;
			playerInfo.levelThreshold = player->skills->data->levelThreshold;
		}

		// buff
		/*auto effects = player->GetActiveEffectList();
		if (effects) {
			for (const auto& effect : *effects) {
				auto formid = effect->GetBaseObject()->GetFormID();

				effect->magnitude;
			}
		}*/

		if (show_player_effects_window) {
			auto& effects2Info = effectsInfo[nowEffectsIndex].list;
			int tmpCount = 0;
			auto effects = player->GetActiveEffectList();
			if (effects) {
				for (const auto& effect : *effects) {
					auto duration = effect->duration;
					float magnitude = effect->magnitude;
					if (duration > 0 || 1 == 1) {
						RE::EffectSetting* effectSetting = effect->GetBaseObject();
						if (effectSetting->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kHideInUI)) {
							continue;
						}
						if (effect->flags.any(RE::ActiveEffect::Flag::kInactive)) {
							continue;
						}
						if (show_player_effects_ignore_permanent) {
							if (duration == 0) {
								if (magnitude >= 0 || !show_player_effects_negative) {
									continue;
								}
							}
						}
						RE::MagicItem* magicItem = effect->spell;
						// 忽略
						if (show_player_effects_listignore) {
							ExcludeFormEffectIds ids{ effectSetting->GetFormID(), magicItem->GetFormID() };
							if (excludeEffectFormIds.find(ids) != excludeEffectFormIds.end()) {
								continue;
							}
						}

						if (effectSetting && magicItem) {
							if (tmpCount + 1 > effects2Info.size()) {
								effects2Info.resize(effects2Info.size() + 20);
							}
							effects2Info[tmpCount].duration = duration;
							effects2Info[tmpCount].magnitude = magnitude;
							effects2Info[tmpCount].elapsedSeconds = effect->elapsedSeconds;
							effects2Info[tmpCount].name = effectSetting->GetFullName();
							effects2Info[tmpCount].spellName = magicItem->GetFullName();
							effects2Info[tmpCount].effectId = effectSetting->GetFormID();
							effects2Info[tmpCount].spellId = magicItem->GetFormID();

							std::stringstream ss;

							auto spellType = magicItem->GetSpellType();
							switch (spellType) {
							case RE::MagicSystem::SpellType::kSpell:
								ss << ICON_MDI_CREATION << " ";
								//ss << ICON_MDI_MAGIC_STAFF << " ";
								break;
							case RE::MagicSystem::SpellType::kDisease:
								ss << ICON_MDI_BACTERIA_OUTLINE << " ";
								break;
							case RE::MagicSystem::SpellType::kPower:
								break;
							case RE::MagicSystem::SpellType::kLesserPower:
								break;
							case RE::MagicSystem::SpellType::kAbility:
								ss << ICON_MDI_ACCOUNT_ARROW_UP << " ";
								break;
							case RE::MagicSystem::SpellType::kPoison:
								ss << ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE << " ";
								break;
							case RE::MagicSystem::SpellType::kEnchantment:
								ss << ICON_MDI_SHIELD_HALF_FULL << " ";
								break;
							case RE::MagicSystem::SpellType::kAlchemy:
								ss << ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE << " ";
								break;
							case RE::MagicSystem::SpellType::kIngredient:
								ss << ICON_MDI_BOWL_MIX << " ";
								break;
							case RE::MagicSystem::SpellType::kLeveledSpell:
								break;
							case RE::MagicSystem::SpellType::kAddiction:
								break;
							case RE::MagicSystem::SpellType::kVoicePower:
								break;
							case RE::MagicSystem::SpellType::kStaffEnchantment:
								break;
							case RE::MagicSystem::SpellType::kScroll:
								ss << ICON_MDI_SCRIPT_TEXT << " ";
								break;
							default:
								break;
							}

							if (show_player_effects_ignore_spell) {
								ss << effectSetting->GetFullName() << " ";
							} else {
								ss << effectSetting->GetFullName() << "[" << magicItem->GetFullName() << "] ";
							}
							effects2Info[tmpCount].text = ss.str();

							std::stringstream ss2;
							if (magnitude > 0) {
								ss2 << ICON_MDI_TRANSFER_UP << std::fixed << std::setprecision(0) << magnitude << "%";
							} else if (magnitude == 0) {
								ss2 << ICON_MDI_TRANSFER_UP;
							} else {
								magnitude = -magnitude;
								ss2 << ICON_MDI_TRANSFER_DOWN << std::fixed << std::setprecision(0) << magnitude << "%";
							}
							if (duration > 0) {
								if (!show_player_effects_process) {
									float sy = (duration - effect->elapsedSeconds);
									if (sy > 999.0f) {
										sy = 999.0f;
									}
									ss2 << " " << std::fixed << std::setprecision(0) << sy << "s";
								}
							}
							effects2Info[tmpCount].text2 = ss2.str();
							tmpCount++;
						}
					}
				}
			}
			effectsInfo[nowEffectsIndex].count = tmpCount;
			nowEffectsIndex = !nowEffectsIndex;
		}
	}
}
