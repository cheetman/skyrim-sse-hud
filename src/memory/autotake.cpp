#include "autotake.h"
#include "lotd.h"
#include "memory.h"
#include <event/BSTMenuEvent.h>
#include <memory/sexlab.h>
#include <memory/stat.h>
#include <memory/track.h>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
#include <utils/PlayerDataProvider.h>

bool show_items_window_auto_ignore = true;
bool show_items_window_auto_notification = true;
bool show_items_window_auto_ammo = false;
bool show_items_window_auto_keym = false;
bool show_items_window_auto_weap = false;
bool show_items_window_auto_armo = false;
bool show_items_window_auto_flor = false;
bool show_items_window_auto_tree = false;
//bool show_items_window_auto_acti = false;
bool show_items_window_auto_food = false;
bool show_items_window_auto_ingr = false;
bool show_items_window_auto_alch = false;
bool show_items_window_auto_book = false;
bool show_items_window_auto_misc = false;
bool show_items_window_auto_sgem = false;
bool show_items_window_auto_achr = false;
bool show_items_window_auto_cont = false;
bool show_items_window_auto_conttype = false;
bool show_items_window_auto_ston = false;
bool show_items_window_auto_anvi = false;
bool show_items_window_auto_anhd = false;
bool show_items_window_auto_anpa = false;
bool show_items_window_auto_tool = false;
bool show_items_window_auto_achr_ingr = false;
bool show_items_window_auto_achr_food = false;
bool show_items_window_auto_achr_alch = false;
bool show_items_window_auto_achr_sgem = false;
bool show_items_window_auto_achr_ammo = false;
bool show_items_window_auto_achr_scrl = false;
bool show_items_window_auto_achr_keym = false;
bool show_items_window_auto_achr_book = false;
bool show_items_window_auto_achr_misc = false;
bool show_items_window_auto_achr_gold = false;
bool show_items_window_auto_achr_weap = false;
bool show_items_window_auto_achr_armo = false;
bool show_items_window_auto_achr_ston = false;
bool show_items_window_auto_achr_anvi = false;
bool show_items_window_auto_achr_lock = false;
bool show_items_window_auto_achr_anhd = false;
bool show_items_window_auto_achr_anpa = false;
bool show_items_window_auto_achr_tool = false;
bool show_items_window_auto_cont_ingr = false;
bool show_items_window_auto_cont_food = false;
bool show_items_window_auto_cont_alch = false;
bool show_items_window_auto_cont_sgem = false;
bool show_items_window_auto_cont_ammo = false;
bool show_items_window_auto_cont_scrl = false;
bool show_items_window_auto_cont_keym = false;
bool show_items_window_auto_cont_book = false;
bool show_items_window_auto_cont_misc = false;
bool show_items_window_auto_cont_gold = false;
bool show_items_window_auto_cont_weap = false;
bool show_items_window_auto_cont_armo = false;
bool show_items_window_auto_cont_ston = false;
bool show_items_window_auto_cont_anvi = false;
bool show_items_window_auto_cont_lock = false;
bool show_items_window_auto_cont_anhd = false;
bool show_items_window_auto_cont_anpa = false;
bool show_items_window_auto_cont_tool = false;

bool show_items_window_auto_weap_enchant = false;
bool show_items_window_auto_weap_price = false;
bool show_items_window_auto_weap_priceweight = false;
bool show_items_window_auto_armo_enchant = false;
bool show_items_window_auto_armo_price = false;
bool show_items_window_auto_armo_priceweight = false;
int show_items_window_auto_weap_price_value = 500;
int show_items_window_auto_armo_price_value = 500;
int show_items_window_auto_weap_priceweight_value = 20;
int show_items_window_auto_armo_priceweight_value = 20;

int show_items_window_auto_dis = 2;

int refresh_time_auto = 1;
int show_items_window_auto_every_max = 10;
int show_items_window_auto_every = 0;

bool show_items_window_auto_enable = true;

// 地点排除
std::unordered_set<RE::FormID> excludeLocationFormIds;
std::vector<ExcludeForm> excludeLocationForms;

// 容器类型
std::unordered_set<RE::FormID> autoContFormIds;
std::vector<IncludeForm> autoContForms;

std::unordered_map<std::string, std::unordered_set<RE::FormID>> whiteListFormIds;
std::unordered_map<std::string, std::vector<ExcludeForm>> whiteListForms;
std::unordered_map<std::string, std::unordered_set<RE::FormID>> blackListFormIds;
std::unordered_map<std::string, std::vector<ExcludeForm>> blackListForms;

bool auto_whitelist_misc = false;
bool auto_whitelist_ammo = false;
bool auto_whitelist_food = false;
bool auto_whitelist_flor = false;
bool auto_whitelist_ingr = false;
bool auto_whitelist_alch = false;
bool auto_whitelist_sgem = false;
bool auto_whitelist_tree = false;
bool auto_whitelist_weap = false;
bool auto_whitelist_ston = false;
bool auto_whitelist_anvi = false;
bool auto_whitelist_anhd = false;
bool auto_whitelist_anpa = false;
bool auto_whitelist_armo = false;
bool auto_whitelist_tool = false;
bool only_pickup_lotd_items = false;

bool checkTakeCount()
{
	// 限制一下每次拾取
	if (show_items_window_auto_every > show_items_window_auto_every_max) {
		return true;
	}
	show_items_window_auto_every++;
	return false;
}

bool checkWhiteBlackList(RE::FormID formId, bool isWhiteMode, std::string type)
{
	if (isWhiteMode) {
		if (whiteListFormIds[type].find(formId) != whiteListFormIds[type].end()) {
			return false;
		}
		return true;
	} else {
		if (blackListFormIds[type].find(formId) != blackListFormIds[type].end()) {
			return true;
		}
		return false;
	}
}

bool __fastcall autoTakeArmo(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance)
{
	if (only_pickup_lotd_items) {
		if (!reff->IsCrimeToActivate()) {
			if (!reff->IsMarkedForDeletion()) {
				//reff->extraList.GetCount();
				if (FormUtil::IsQuestItem(&reff->extraList)) {
					return false;
				}

				if (!lotd::checkItem(reff->GetBaseObject()->GetFormID())) {
					return false;
				}
				// 限制数量
				if (checkTakeCount()) {
					return true;
				}
				addItemAuto(nullptr, reff, 1);
				return true;
			}
		}

	} else if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!reff->IsMarkedForDeletion()) {
					if (FormUtil::IsQuestItem(&reff->extraList)) {
						return false;
					}

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

					if (show_items_window_auto_armo_priceweight) {
						float weight = reff->GetObjectReference()->GetWeight();
						if (weight == 0) {
							return false;
						}
						if ((reff->GetObjectReference()->GetGoldValue() / weight) < show_items_window_auto_armo_priceweight_value) {
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
							// 限制数量
							if (checkTakeCount()) {
								return true;
							}
							addItemAuto(nullptr, reff, 1);
							return true;
						}
					} else {
						// 限制数量
						if (checkTakeCount()) {
							return true;
						}
						addItemAuto(nullptr, reff, 1);
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoTakeWeap(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance)
{
	// 自动拾取
	if (only_pickup_lotd_items) {
		if (!reff->IsCrimeToActivate()) {
			if (!reff->IsMarkedForDeletion()) {
				//reff->extraList.GetCount();
				if (FormUtil::IsQuestItem(&reff->extraList)) {
					return false;
				}

				if (!lotd::checkItem(reff->GetBaseObject()->GetFormID())) {
					return false;
				}
				// 限制数量
				if (checkTakeCount()) {
					return true;
				}
				addItemAuto(nullptr, reff, 1);
				return true;
			}
		}

	} else if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!reff->IsMarkedForDeletion()) {
					//reff->extraList.GetCount();
					if (FormUtil::IsQuestItem(&reff->extraList)) {
						return false;
					}

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

					if (show_items_window_auto_weap_priceweight) {
						float weight = reff->GetObjectReference()->GetWeight();
						if (weight == 0) {
							return false;
						}
						if ((reff->GetObjectReference()->GetGoldValue() / weight) < show_items_window_auto_weap_priceweight_value) {
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
							// 限制数量
							if (checkTakeCount()) {
								return true;
							}
							addItemAuto(nullptr, reff, 1);
							return true;
						}
					} else {
						// 限制数量
						if (checkTakeCount()) {
							return true;
						}
						addItemAuto(nullptr, reff, 1);
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool __fastcall autoTake(RE::TESObjectREFR* reff, RE::PlayerCharacter* player, bool autoFlag, int distance)
{
	if (only_pickup_lotd_items) {
		if (!reff->IsCrimeToActivate()) {
			if (!reff->IsMarkedForDeletion()) {
				if (FormUtil::IsQuestItem(&reff->extraList)) {
					return false;
				}

				if (!lotd::checkItem(reff->GetBaseObject()->GetFormID())) {
					return false;
				}
				// 限制数量
				if (checkTakeCount()) {
					return true;
				}
				addItemAuto(nullptr, reff, 1);
				return true;
			}
		}

	} else if (autoFlag) {
		if (distance < show_items_window_auto_dis) {
			if (!reff->IsCrimeToActivate()) {
				if (!reff->IsMarkedForDeletion()) {
					if (FormUtil::IsQuestItem(&reff->extraList)) {
						return false;
					}
					if (show_items_window_auto_ignore) {
						// 判断地点忽略
						int formID = 0;
						if (player->currentLocation) {
							formID = player->currentLocation->GetFormID();
						}
						if (excludeLocationFormIds.find(formID) == excludeLocationFormIds.end()) {
							// 限制数量
							if (checkTakeCount()) {
								return true;
							}
							//deleteREFRs.insert(reff);
							addItemAuto(nullptr, reff, 1);
							/*reff->Get3D()->AsNode();
								reff->Get3D()->CreateClone();*/
							//reff->GetSequence

							return true;
						}
					} else {
						// 限制数量
						if (checkTakeCount()) {
							return true;
						}
						//deleteREFRs.insert(reff);
						addItemAuto(nullptr, reff, 1);

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
								// 限制数量
								if (checkTakeCount()) {
									return true;
								}
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
							// 限制数量
							if (checkTakeCount()) {
								return true;
							}
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
								// 限制数量
								if (checkTakeCount()) {
									return true;
								}
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

	// 限制数量
	if (checkTakeCount()) {
		return true;
	}
	RemoveItemACHR(nullptr, actor, obj, count, true);
	return true;
}

bool __fastcall autoTakeForCONT(RE::TESObjectREFR* reff, RE::TESBoundObject* obj, int count, RE::PlayerCharacter* player)

{
	if (show_items_window_ignore) {
		if (excludeFormIds.find(obj->GetFormID()) != excludeFormIds.end()) {
			return false;
		}
	}

	// 限制数量
	if (checkTakeCount()) {
		return true;
	}

	RemoveItemCONT(nullptr, reff, obj, count, true);
	return true;
}

void RemoveItemACHR(RE::PlayerCharacter* contReff, RE::TESObjectREFR* actor, RE::TESBoundObject* obj, int count, bool autoTake)
{
	if (!contReff) {
		contReff = RE::PlayerCharacter::GetSingleton();
		if (!contReff) {
			return;
		}
	}

	contReff->PlayPickUpSound(obj, true, false);
	char buf[100];
	if (autoTake) {
		if (show_items_window_auto_notification) {
			if (count > 1) {
				snprintf(buf, 100, "%s(%d) 从%s尸体中自动拾取", obj->GetName(), count, actor->GetDisplayFullName());
			} else {
				snprintf(buf, 100, "%s 从%s尸体中自动拾取", obj->GetName(), actor->GetDisplayFullName());
			}
			RE::DebugNotification(buf, NULL, false);
		}
	} else {
		if (count > 1) {
			snprintf(buf, 100, "%s(%d) 从%s尸体中拾取", obj->GetName(), count, actor->GetDisplayFullName());
		} else {
			snprintf(buf, 100, "%s 从%s尸体中拾取", obj->GetName(), actor->GetDisplayFullName());
		}
		RE::DebugNotification(buf, NULL, false);
	}
	ScriptUtil::RemoveItem(nullptr, 0, actor, obj, count, true, contReff);
}

void RemoveItemCONT(RE::PlayerCharacter* contReff, RE::TESObjectREFR* actor, RE::TESBoundObject* obj, int count, bool autoTake)
{
	if (!contReff) {
		contReff = RE::PlayerCharacter::GetSingleton();
		if (!contReff) {
			return;
		}
	}

	contReff->PlayPickUpSound(obj, true, false);
	char buf[100];
	if (autoTake) {
		if (show_items_window_auto_notification) {
			if (count > 1) {
				snprintf(buf, 100, "%s(%d) 从%s中自动拾取", obj->GetName(), count, actor->GetDisplayFullName());
			} else {
				snprintf(buf, 100, "%s 从%s中自动拾取", obj->GetName(), actor->GetDisplayFullName());
			}
			RE::DebugNotification(buf, NULL, false);
		}
	} else {
		if (count > 1) {
			snprintf(buf, 100, "%s(%d) 从%s中拾取", obj->GetName(), count, actor->GetDisplayFullName());
		} else {
			snprintf(buf, 100, "%s 从%s中拾取", obj->GetName(), actor->GetDisplayFullName());
		}
		RE::DebugNotification(buf, NULL, false);
	}
	ScriptUtil::RemoveItem(nullptr, 0, actor, obj, count, true, contReff);
}

void addItem(RE::PlayerCharacter* contReff, RE::TESObjectREFR* reff, int count)
{
	//player->PickUpObject(reff, 1);
	if (!contReff) {
		contReff = RE::PlayerCharacter::GetSingleton();
		if (!contReff) {
			return;
		}
	}
	contReff->PlayPickUpSound(reff->GetBaseObject(), true, false);
	char buf[80];
	snprintf(buf, 80, "%s 已拾取", reff->GetDisplayFullName());
	RE::DebugNotification(buf, NULL, false);
	ScriptUtil::AddItem(nullptr, 0, contReff, reff, count, true);
}

void addItemAuto(RE::PlayerCharacter* contReff, RE::TESObjectREFR* reff, int count)
{
	//player->PickUpObject(reff, 1);
	if (!contReff) {
		contReff = RE::PlayerCharacter::GetSingleton();
		if (!contReff) {
			return;
		}
	}
	contReff->PlayPickUpSound(reff->GetBaseObject(), true, false);
	if (show_items_window_auto_notification) {
		char buf[80];
		snprintf(buf, 80, "%s 已自动拾取", reff->GetDisplayFullName());
		RE::DebugNotification(buf, NULL, false);
	}
	ScriptUtil::AddItem(nullptr, 0, contReff, reff, count, true);
}

void __cdecl TimerAutoPick(void*)
{
	while (true) {
		if (refresh_time_auto < 1) {
			refresh_time_auto = 1;
		}
		if (refresh_time_auto > 10) {
			refresh_time_auto = 10;
		}
		Sleep(refresh_time_auto * 1000);

		if (!startflag) {
			Sleep(1000);
			continue;
		}

		if (isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu) {
			Sleep(1000);
			continue;
		}

		if (activeItems) {
			continue;
		}

		if (!show_items_window_auto_enable || (!only_pickup_lotd_items && !show_items_window && !show_items_window_auto_ammo && !show_items_window_auto_flor && !show_items_window_auto_food && !show_items_window_auto_ingr && !show_items_window_auto_alch && !show_items_window_auto_misc && !show_items_window_auto_tree && !show_items_window_auto_sgem && !show_items_window_auto_achr && !show_items_window_auto_cont && !show_items_window_auto_ston && !show_items_window_auto_anvi && !show_items_window_auto_anhd && !show_items_window_auto_anpa && !show_items_window_auto_tool && !show_items_window_auto_book)) {
			Sleep(1000);
			// 暂时挂在这里
			if (lotd::isLoad) {
				lotd::refreshCount();
				lotd::refreshDisplayCount();
			}
			track::refreshAutoTrackItem();
			sexlab::refreshInfo();
			stats::refreshCount();
			continue;
		} else {
			// 暂时挂在这里
			if (lotd::isLoad) {
				lotd::refreshCount();
				lotd::refreshDisplayCount();
			}
			track::refreshAutoTrackItem();
			sexlab::refreshInfo();
			stats::refreshCount();
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			Sleep(1000);
			continue;
		}

		// 限制拾取数量
		show_items_window_auto_every = 1;

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

		int currentLocationFormId = 0;
		// 地点忽略结果
		if (show_items_window_auto_ignore) {
			// 判断地点忽略
			if (currentLocation) {
				currentLocationFormId = currentLocation->GetFormID();
			}
			// 不存在
			if (excludeLocationFormIds.find(currentLocationFormId) != excludeLocationFormIds.end()) {
				continue;
			}
		}

		// 艺术馆地点排除
		if (lotd::isLoad) {
			if (lotd::locationIds.find(currentLocationFormId) != lotd::locationIds.end()) {
				continue;
			}
		}

		if (player->currentLocation) {
			if (FormUtil::HasKeyword(player->currentLocation, LocTypePlayerHouse)) {
				continue;
			}
		}

		{
			const auto& [map, lock] = RE::TESForm::GetAllForms();
			const RE::BSReadWriteLock locker{ lock };
			//const RE::BSReadLockGuard locker{ lock };

			if (!map) {
				continue;
			}

			for (auto& [id, form] : *map) {
				if (form) {
					if (form->Is(RE::FormType::ActorCharacter)) {
						if (!show_items_window_auto_achr && !only_pickup_lotd_items) {
							continue;
						}
						auto actor = form->As<RE::Actor>();
						if (actor && actor->IsDead() && !actor->IsSummoned()) {
							if (actor->GetCurrentLocation() == currentLocation) {
								// 排除自己
								if (actor->IsPlayerRef()) {
									continue;
								}

								if (!actor->Is3DLoaded()) {
									continue;
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

								int tmpInvCount = 0;
								auto inv = actor->GetInventory(FormUtil::CanDisplay);
								for (auto& [obj, data] : inv) {
									auto& [count, entry] = data;
									if (count > 0 && entry) {
										// 自动拾取
										if (distance < show_items_window_auto_dis) {
											/*if (FormUtil::IsQuestItem(&entry.get()->extraLists) {
												return false;
											}*/

											if (entry.get()->IsQuestObject()) {
												continue;
											}
											switch (obj->GetFormType()) {
											case RE::FormType::Weapon:
												{
													if (only_pickup_lotd_items) {
														if (!lotd::checkItem(obj->GetFormID())) {
															continue;
														}
														if (autoTakeForACHR(actor, obj, count, player)) {
															continue;
														}
													} else if (show_items_window_auto_achr_weap) {
														if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_weap, "weap")) {
															continue;
														}

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

														if (show_items_window_auto_weap_priceweight) {
															float weight = obj->GetWeight();
															if (weight == 0) {
																break;
															}
															if ((obj->GetGoldValue() / weight) < show_items_window_auto_weap_priceweight_value) {
																break;
															}
														}

														if (autoTakeForACHR(actor, obj, count, player)) {
															continue;
														}
													}
													break;
												}
											case RE::FormType::Armor:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_armo) {
													if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_armo, "armo")) {
														continue;
													}

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

													if (show_items_window_auto_armo_priceweight) {
														float weight = obj->GetWeight();
														if (weight == 0) {
															break;
														}
														if ((obj->GetGoldValue() / weight) < show_items_window_auto_armo_priceweight_value) {
															break;
														}
													}

													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
												break;
											case RE::FormType::Ammo:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_ammo) {
													if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_ammo, "ammo")) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
												break;
											case RE::FormType::Scroll:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_scrl) {
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
												break;
											case RE::FormType::Misc:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else {
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
															if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																if (show_items_window_auto_achr_ston) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_ston, "ston")) {
																		continue;
																	}
																	if (autoTakeForACHR(actor, obj, count, player)) {
																		continue;
																	}
																}

															} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																if (show_items_window_auto_achr_anvi) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_anvi, "anvi")) {
																		continue;
																	}
																	if (autoTakeForACHR(actor, obj, count, player)) {
																		continue;
																	}
																}
															} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																if (show_items_window_auto_achr_anhd) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_anhd, "anhd")) {
																		continue;
																	}
																	if (autoTakeForACHR(actor, obj, count, player)) {
																		continue;
																	}
																}
															} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																if (show_items_window_auto_achr_anpa) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_anpa, "anpa")) {
																		continue;
																	}
																	if (autoTakeForACHR(actor, obj, count, player)) {
																		continue;
																	}
																}
															} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																if (show_items_window_auto_achr_tool) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_tool, "tool")) {
																		continue;
																	}
																	if (autoTakeForACHR(actor, obj, count, player)) {
																		continue;
																	}
																}
															} else {
																if (show_items_window_auto_achr_misc) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_misc, "misc")) {
																		continue;
																	}
																	if (autoTakeForACHR(actor, obj, count, player)) {
																		continue;
																	}
																}
															}
														}
													}
												}
												break;

											case RE::FormType::KeyMaster:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_keym) {
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
												break;
											case RE::FormType::AlchemyItem:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else {
													auto alchemyItem = obj->As<RE::AlchemyItem>();
													if (alchemyItem->IsFood()) {
														if (show_items_window_auto_achr_food) {
															if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_food, "food")) {
																continue;
															}
															if (autoTakeForACHR(actor, obj, count, player)) {
																continue;
															}
														}

													} else {
														if (show_items_window_auto_achr_alch) {
															if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_alch, "alch")) {
																continue;
															}
															if (autoTakeForACHR(actor, obj, count, player)) {
																continue;
															}
														}
													}
												}
												break;
											case RE::FormType::SoulGem:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_sgem) {
													if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_sgem, "sgem")) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
												break;
											case RE::FormType::Ingredient:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_ingr) {
													if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_ingr, "ingr")) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												}
												break;
											case RE::FormType::Book:
												if (only_pickup_lotd_items) {
													if (!lotd::checkItem(obj->GetFormID())) {
														continue;
													}
													if (autoTakeForACHR(actor, obj, count, player)) {
														continue;
													}
												} else if (show_items_window_auto_achr_book) {
													auto book = obj->As<RE::TESObjectBOOK>();
													if (book) {
														if (book->IsRead()) {
															continue;
														}
													}
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
								if (!reff->Is3DLoaded()) {
									continue;
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

											if (!only_pickup_lotd_items) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_weap, "weap")) {
													continue;
												}
											}

											// 自动拾取判断
											if (autoTakeWeap(reff, player, show_items_window_auto_weap, distance)) {
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

											if (!only_pickup_lotd_items) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_armo, "armo")) {
													continue;
												}
											}

											// 自动拾取判断
											if (autoTakeArmo(reff, player, show_items_window_auto_armo, distance)) {
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

											if (!only_pickup_lotd_items) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_ammo, "ammo")) {
													continue;
												}
											}

											// 自动拾取判断
											if (autoTake(reff, player, show_items_window_auto_ammo, distance)) {
												continue;
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

											if (!only_pickup_lotd_items) {
												auto book = baseObj->As<RE::TESObjectBOOK>();
												if (book) {
													if (book->IsRead()) {
														continue;
													}
												}
											}

											// 自动拾取判断
											if (autoTake(reff, player, show_items_window_auto_book, distance)) {
												continue;
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
												if (!only_pickup_lotd_items && show_items_window_auto_food) {
													if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_food, "food")) {
														continue;
													}
												}

												// 自动拾取判断
												if (autoTake(reff, player, show_items_window_auto_food, distance)) {
													continue;
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

												if (!only_pickup_lotd_items && show_items_window_auto_alch) {
													if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_alch, "alch")) {
														continue;
													}
												}

												// 自动拾取判断
												if (autoTake(reff, player, show_items_window_auto_alch, distance)) {
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
											if (!only_pickup_lotd_items && show_items_window_auto_ingr) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_ingr, "ingr")) {
													continue;
												}
											}

											if (autoTake(reff, player, show_items_window_auto_ingr, distance)) {
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

											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											if (!reff->GetPlayable()) {
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

											auto misc = baseObj->As<RE::TESObjectMISC>();
											if (misc) {
												if (FormUtil::HasKeyword(misc, VendorItemGem)) {
													if (!only_pickup_lotd_items && show_items_window_auto_ston) {
														if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_ston, "ston")) {
															continue;
														}
													}
													// 自动拾取判断
													if (autoTake(reff, player, show_items_window_auto_ston, distance)) {
														continue;
													}

												} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
													if (!only_pickup_lotd_items && show_items_window_auto_anvi) {
														if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_anvi, "anvi")) {
															continue;
														}
													}
													// 自动拾取判断
													if (autoTake(reff, player, show_items_window_auto_anvi, distance)) {
														continue;
													}

												} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
													if (!only_pickup_lotd_items && show_items_window_auto_anhd) {
														if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_anhd, "anhd")) {
															continue;
														}
													}
													// 自动拾取判断
													if (autoTake(reff, player, show_items_window_auto_anhd, distance)) {
														continue;
													}
												} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
													if (!only_pickup_lotd_items && show_items_window_auto_anpa) {
														if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_anpa, "anpa")) {
															continue;
														}
													}
													// 自动拾取判断
													if (autoTake(reff, player, show_items_window_auto_anpa, distance)) {
														continue;
													}
												} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
													if (!only_pickup_lotd_items && show_items_window_auto_tool) {
														if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_tool, "tool")) {
															continue;
														}
													}
													// 自动拾取判断
													if (autoTake(reff, player, show_items_window_auto_tool, distance)) {
														continue;
													}
												} else {
													if (show_items_window_auto_misc) {
														if (clawFormIds.find(baseObj->GetFormID()) != clawFormIds.end()) {
															continue;
														}
														if (!only_pickup_lotd_items && show_items_window_auto_misc) {
															if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_misc, "misc")) {
																continue;
															}
														}
														// 自动拾取判断
														if (autoTake(reff, player, show_items_window_auto_misc, distance)) {
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
											if (!show_items_window_auto_cont && !only_pickup_lotd_items) {
												continue;
											}

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

											if (merchantContFormIds.find(reff->GetFormID()) != merchantContFormIds.end()) {
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
											int tmpInvCount = 0;
											auto inv = reff->GetInventory(FormUtil::CanDisplay);

											// 自动拾取条件1
											bool auto1 = false;
											if (show_items_window_auto_conttype) {
												if (autoContFormIds.find(baseObj->GetFormID()) != autoContFormIds.end()) {
													auto1 = true;
												}
											} else {
												auto1 = true;
											}

											RE::LOCK_LEVEL lockLevel = reff->GetLockLevel();

											// 满足自动拾取(加上距离条件和地点条件)
											if (auto1 && distance < show_items_window_auto_dis && !isCrime && lockLevel == RE::LOCK_LEVEL::kUnlocked) {
												for (auto& [obj, data] : inv) {
													auto& [count, entry] = data;
													if (count > 0 && entry) {
														if (entry.get()->IsQuestObject()) {
															continue;
														}
														bool stealing = player->WouldBeStealing(reff);
														bool isCrimeInv = !entry.get()->IsOwnedBy(player, !stealing);
														// 并且不犯罪 判断是否拾取
														if (!isCrimeInv) {
															switch (obj->GetFormType()) {
															case RE::FormType::Weapon:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_weap) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_weap, "weap")) {
																		continue;
																	}

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
																	if (show_items_window_auto_weap_priceweight) {
																		float weight = obj->GetWeight();
																		if (weight == 0) {
																			break;
																		}
																		if ((obj->GetGoldValue() / weight) < show_items_window_auto_weap_priceweight_value) {
																			break;
																		}
																	}

																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Armor:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_armo) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_armo, "armo")) {
																		continue;
																	}

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

																	if (show_items_window_auto_armo_priceweight) {
																		float weight = obj->GetWeight();
																		if (weight == 0) {
																			break;
																		}
																		if ((obj->GetGoldValue() / weight) < show_items_window_auto_armo_priceweight_value) {
																			break;
																		}
																	}

																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Ammo:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_ammo) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_ammo, "ammo")) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Scroll:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_scrl) {
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Misc:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else {
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
																			if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																				if (show_items_window_auto_cont_ston) {
																					if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_ston, "ston")) {
																						continue;
																					}
																					if (autoTakeForCONT(reff, obj, count, player)) {
																						continue;
																					}
																				}

																			} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																				if (show_items_window_auto_cont_anvi) {
																					if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_anvi, "anvi")) {
																						continue;
																					}
																					if (autoTakeForCONT(reff, obj, count, player)) {
																						continue;
																					}
																				}
																			} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																				if (show_items_window_auto_cont_anhd) {
																					if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_anhd, "anhd")) {
																						continue;
																					}
																					if (autoTakeForCONT(reff, obj, count, player)) {
																						continue;
																					}
																				}
																			} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																				if (show_items_window_auto_cont_anpa) {
																					if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_anpa, "anpa")) {
																						continue;
																					}
																					if (autoTakeForCONT(reff, obj, count, player)) {
																						continue;
																					}
																				}
																			} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																				if (show_items_window_auto_cont_tool) {
																					if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_tool, "tool")) {
																						continue;
																					}
																					if (autoTakeForCONT(reff, obj, count, player)) {
																						continue;
																					}
																				}
																			} else {
																				if (show_items_window_auto_cont_misc) {
																					if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_misc, "misc")) {
																						continue;
																					}
																					if (autoTakeForCONT(reff, obj, count, player)) {
																						continue;
																					}
																				}
																			}
																		}
																	}
																}

																break;
															case RE::FormType::KeyMaster:
																if (show_items_window_auto_cont_keym) {
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::AlchemyItem:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else {
																	auto alchemyItem = obj->As<RE::AlchemyItem>();
																	if (alchemyItem->IsFood()) {
																		if (show_items_window_auto_cont_food) {
																			if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_food, "food")) {
																				continue;
																			}
																			if (autoTakeForCONT(reff, obj, count, player)) {
																				continue;
																			}
																		}

																	} else {
																		if (show_items_window_auto_cont_alch) {
																			if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_alch, "alch")) {
																				continue;
																			}
																			if (autoTakeForCONT(reff, obj, count, player)) {
																				continue;
																			}
																		}
																	}
																}
																break;
															case RE::FormType::SoulGem:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_sgem) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_sgem, "sgem")) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Ingredient:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_ingr) {
																	if (checkWhiteBlackList(obj->GetFormID(), auto_whitelist_ingr, "ingr")) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																}
																break;

															case RE::FormType::Book:
																if (only_pickup_lotd_items) {
																	if (!lotd::checkItem(obj->GetFormID())) {
																		continue;
																	}
																	if (autoTakeForCONT(reff, obj, count, player)) {
																		continue;
																	}
																} else if (show_items_window_auto_cont_book) {
																	auto book = obj->As<RE::TESObjectBOOK>();
																	if (book) {
																		if (book->IsRead()) {
																			continue;
																		}
																	}
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

											if (!only_pickup_lotd_items && show_items_window_auto_flor) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_flor, "flor")) {
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

											if (!only_pickup_lotd_items && show_items_window_auto_tree) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_tree, "tree")) {
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

											// 自动拾取判断
											if (autoTake(reff, player, show_items_window_auto_keym, distance)) {
												continue;
											}

											tmpCountKEYM++;
											break;
										}
									case RE::FormType::Activator:
										{
											// 灰烬
											auto ashPile = reff->extraList.GetAshPileRef();
											if (ashPile && ashPile.get()) {
												auto actor = ashPile.get().get();

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

												int tmpInvCount = 0;
												auto inv = actor->GetInventory(FormUtil::CanDisplay);
												for (auto& [obj, data] : inv) {
													auto& [count, entry] = data;
													if (count > 0 && entry) {
														// 自动拾取

														if (show_items_window_auto_achr && distance < show_items_window_auto_dis) {
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

																	if (show_items_window_auto_weap_priceweight) {
																		float weight = obj->GetWeight();
																		if (weight == 0) {
																			break;
																		}
																		if ((obj->GetGoldValue() / weight) < show_items_window_auto_weap_priceweight_value) {
																			break;
																		}
																	}

																	if (autoTakeForCONT(actor, obj, count, player)) {
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

																	if (show_items_window_auto_armo_priceweight) {
																		float weight = obj->GetWeight();
																		if (weight == 0) {
																			break;
																		}
																		if ((obj->GetGoldValue() / weight) < show_items_window_auto_armo_priceweight_value) {
																			break;
																		}
																	}

																	if (autoTakeForCONT(actor, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Ammo:
																if (show_items_window_auto_achr_ammo) {
																	if (autoTakeForCONT(actor, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Scroll:
																if (show_items_window_auto_achr_scrl) {
																	if (autoTakeForCONT(actor, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Misc:
																{
																	if (obj->IsGold()) {
																		if (show_items_window_auto_achr_gold) {
																			if (autoTakeForCONT(actor, obj, count, player)) {
																				continue;
																			}
																		}

																	} else if (obj->IsLockpick()) {
																		if (show_items_window_auto_achr_lock) {
																			if (autoTakeForCONT(actor, obj, count, player)) {
																				continue;
																			}
																		}
																	} else {
																		auto misc = obj->As<RE::TESObjectMISC>();
																		if (misc) {
																			if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																				if (show_items_window_auto_achr_ston) {
																					if (autoTakeForCONT(actor, obj, count, player)) {
																						continue;
																					}
																				}

																			} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																				if (show_items_window_auto_achr_anvi) {
																					if (autoTakeForCONT(actor, obj, count, player)) {
																						continue;
																					}
																				}
																			} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																				if (show_items_window_auto_achr_anhd) {
																					if (autoTakeForCONT(actor, obj, count, player)) {
																						continue;
																					}
																				}
																			} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																				if (show_items_window_auto_achr_anpa) {
																					if (autoTakeForCONT(actor, obj, count, player)) {
																						continue;
																					}
																				}
																			} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																				if (show_items_window_auto_achr_tool) {
																					if (autoTakeForCONT(actor, obj, count, player)) {
																						continue;
																					}
																				}
																			} else {
																				if (show_items_window_auto_achr_misc) {
																					if (autoTakeForCONT(actor, obj, count, player)) {
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
																	if (autoTakeForCONT(actor, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::AlchemyItem:
																{
																	auto alchemyItem = obj->As<RE::AlchemyItem>();
																	if (alchemyItem->IsFood()) {
																		if (show_items_window_auto_achr_food) {
																			if (autoTakeForCONT(actor, obj, count, player)) {
																				continue;
																			}
																		}

																	} else {
																		if (show_items_window_auto_achr_alch) {
																			if (autoTakeForCONT(actor, obj, count, player)) {
																				continue;
																			}
																		}
																	}
																	break;
																}
															case RE::FormType::SoulGem:
																if (show_items_window_auto_achr_sgem) {
																	if (autoTakeForCONT(actor, obj, count, player)) {
																		continue;
																	}
																}
																break;
															case RE::FormType::Ingredient:
																if (show_items_window_auto_achr_ingr) {
																	if (autoTakeForCONT(actor, obj, count, player)) {
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

												tmpCountACTI++;
											}
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

											if (!only_pickup_lotd_items && show_items_window_auto_sgem) {
												if (checkWhiteBlackList(baseObj->GetFormID(), auto_whitelist_sgem, "sgem")) {
													continue;
												}
											}
											// 自动拾取判断
											if (autoTake(reff, player, show_items_window_auto_sgem, distance)) {
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
	}
}
