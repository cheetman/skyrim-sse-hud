#include "autotake.h"
#include "memory.h"
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
#include <utils/PlayerDataProvider.h>

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
bool show_items_window_auto_ston = false;
bool show_items_window_auto_anvi = false;
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
bool show_items_window_auto_achr_ston = false;
bool show_items_window_auto_achr_anvi = false;
bool show_items_window_auto_achr_lock = false;
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
bool show_items_window_auto_cont_ston = false;
bool show_items_window_auto_cont_anvi = false;
bool show_items_window_auto_cont_lock = false;

bool show_items_window_auto_weap_enchant = false;
bool show_items_window_auto_weap_price = false;
bool show_items_window_auto_armo_enchant = false;
bool show_items_window_auto_armo_price = false;
int show_items_window_auto_weap_price_value = 500;
int show_items_window_auto_armo_price_value = 500;

int show_items_window_auto_dis = 2;
int show_items_window_auto_dis_skyrim = 100;
int show_items_window_auto_dis_local = 160;

// 地点排除
std::unordered_set<int> excludeLocationFormIds;
std::vector<ExcludeForm> excludeLocationForms;
// 容器类型
std::unordered_set<int> autoContFormIds;
std::vector<IncludeForm> autoContForms;
std::unordered_set<RE::TESObjectREFR*> deleteREFRs;

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

void refreshItemAuto()
{
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	if (!player) {
		Sleep(1000);
		return;
	}

	bool isDeleteExist = deleteREFRs.size() > 0;

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
		return;
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
			return;
		}
	}

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

											} else if (obj->IsLockpick()) {
												if (show_items_window_auto_achr_lock) {
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
											} else {
												auto misc = obj->As<RE::TESObjectMISC>();
												if (misc) {
													if (HasKeyword(misc, VendorItemGem)) {
														if (show_items_window_auto_achr_ston) {
															if (autoTakeForACHR(actor, obj, count, player)) {
																continue;
															}
														}

													} else if (HasKeyword(misc, VendorItemOreIngot)) {
														if (show_items_window_auto_achr_anvi) {
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

								// 画面不显示则直接跳过
								if (!activeItems) {
									continue;
								}

								tmpInvCount++;
								if (tmpInvCount == 200) {
									break;
								}
							}
						}

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

									tmpCountAMMO++;

									break;
								}
							case RE::FormType::Book:
								{
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

									auto misc = baseObj->As<RE::TESObjectMISC>();
									if (misc) {
										if (HasKeyword(misc, VendorItemGem)) {
											if (show_items_window_auto_ston) {
												// 自动拾取判断
												if (autoTake(reff, player, show_items_window_auto_ston, distance, isDeleteExist)) {
													continue;
												}
											}

										} else if (HasKeyword(misc, VendorItemOreIngot)) {
											if (show_items_window_auto_anvi) {
												// 自动拾取判断
												if (autoTake(reff, player, show_items_window_auto_anvi, distance, isDeleteExist)) {
													continue;
												}
											}
										} else {
											if (show_items_window_auto_cont_misc) {
												// 自动拾取判断
												if (autoTake(reff, player, show_items_window_auto_misc, distance, isDeleteExist)) {
													continue;
												}
											}
										}
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

									bool isCrime = reff->IsCrimeToActivate();
									int tmpInvCount = 0;
									auto inv = reff->GetInventory(CanDisplay);

									// 自动拾取条件1
									bool auto1 = false;
									if (show_items_window_auto_cont) {
										if (autoContFormIds.find(baseObj->GetFormID()) != autoContFormIds.end()) {
											auto1 = true;
										}
									}

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

															} else if (obj->IsLockpick()) {
																if (show_items_window_auto_cont_lock) {
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
															} else {
																auto misc = obj->As<RE::TESObjectMISC>();
																if (misc) {
																	if (HasKeyword(misc, VendorItemGem)) {
																		if (show_items_window_auto_cont_ston) {
																			if (autoTakeForCONT(reff, obj, count, player)) {
																				continue;
																			}
																		}

																	} else if (HasKeyword(misc, VendorItemOreIngot)) {
																		if (show_items_window_auto_cont_anvi) {
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

												tmpInvCount++;
												if (tmpInvCount == 200) {
													break;
												}
											}
										}
									}
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

									// 自动拾取判断
									if (autoHarvest(reff, player, show_items_window_auto_flor, distance, flora)) {
										continue;
									}

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

									// 自动拾取判断
									if (autoHarvest(reff, player, show_items_window_auto_tree, distance, tree)) {
										continue;
									}
									tmpCountTREE++;

									break;
								}
							case RE::FormType::KeyMaster:
								{
									tmpCountKEYM++;
									break;
								}
							case RE::FormType::Activator:
								{
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
									continue;
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
