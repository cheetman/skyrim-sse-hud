#include "track.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <menu/menu_track.h>
#include <utils/utils.h>

namespace track
{
	bool isAutoTrackItems = false;
	bool isAutoTrackItemsFlag = false;

	std::string getTrackerName(RE::TESBoundObject* obj, std::string name)
	{
		switch (obj->GetFormType()) {
		case RE::FormType::Scroll:
			return ICON_MDI_SWORD " " + name;
			break;
		case RE::FormType::Weapon:
			return ICON_MDI_SWORD " " + name;
			break;
		case RE::FormType::Armor:
			return ICON_MDI_SHIELD " " + name;
			break;
		case RE::FormType::Ammo:
			return ICON_MDI_ARROW_PROJECTILE " " + name;
			break;
		case RE::FormType::Book:
			return ICON_MDI_BOOK_OPEN_VARIANT " " + name;
			break;
		case RE::FormType::AlchemyItem:
			{
				auto alchemyItem = obj->As<RE::AlchemyItem>();
				if (alchemyItem->IsFood()) {
					return ICON_MDI_FOOD_DRUMSTICK " " + name;
				} else {
					return ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + name;
				}
				break;
			}
		case RE::FormType::Ingredient:
			return ICON_MDI_SOURCE_BRANCH " " + name;
			break;
		case RE::FormType::Misc:
			{
				auto misc = obj->As<RE::TESObjectMISC>();
				if (misc) {
					// 宝石
					if (FormUtil::HasKeyword(misc, VendorItemGem)) {
						return ICON_MDI_DIAMOND_STONE " " + name;
					} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
						return ICON_MDI_ANVIL " " + name;
					} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
						return ICON_MDI_BOX_CUTTER " " + name;
					} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
						return ICON_MDI_RABBIT " " + name;
					} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
						return ICON_MDI_TOOLS " " + name;
					} else {
						return ICON_MDI_PACKAGE_VARIANT_CLOSED " " + name;
					}
				}
				break;
			}
		case RE::FormType::KeyMaster:
			return ICON_MDI_KEY " " + name;
			break;
		case RE::FormType::Note:
			return ICON_MDI_BOOK_OPEN_VARIANT " " + name;
			break;
		case RE::FormType::SoulGem:
			return ICON_MDI_CARDS_DIAMOND " " + name;
			break;
		default:
			return name;
			break;
		}

		return name;
	}

	void refreshAutoTrackItem()
	{
		if (lotd::isAutoTrackLotdItemsFlag || lotd::isAutoTrackLotdExcavationFlag || lotd::isAutoTrackLotdCardsFlag || track::isAutoTrackItemsFlag) {
			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (!player) {
				return;
			}

			if (lotd::isAutoTrackLotdItems) {
				// 缓存身上物品
				lotd::refreshInvItemsCache();

				// 缓存陈列品
				lotd::refreshDisplayItemsCache();
			}

			Sleep(100);

			// 数量统计(int)
			int tmpCount = 0;

			int currentLocationFormId = 0;
			auto currentLocation = player->currentLocation;
			if (currentLocation) {
				currentLocationFormId = currentLocation->GetFormID();
			}

			// 艺术馆地点排除
			if (lotd::locationIds.find(currentLocationFormId) == lotd::locationIds.end()) {
				const auto& [map, lock] = RE::TESForm::GetAllForms();
				const RE::BSReadWriteLock locker{ lock };
				if (!map) {
					return;
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
									if (!actor->Is3DLoaded()) {
										continue;
									}

									if (actor->IsMarkedForDeletion() || actor->IsIgnored()) {
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
									auto name = actor->GetDisplayFullName();
									if (strlen(name) == 0) {
										continue;
									}

									bool isCrime = actor->IsCrimeToActivate();
									if (!isCrime) {
										auto inv = actor->GetInventory(FormUtil::CanDisplay);
										for (auto& [obj, data] : inv) {
											auto& [count, entry] = data;
											if (count > 0 && entry) {
												// 放到对应的数组里

												if (lotd::isAutoTrackLotdItemsFlag) {
													bool success = lotd::checkItem(obj->GetFormID());
													if (success) {
														if (trackPtrs2.find(actor) == trackPtrs2.end()) {
															TrackItem trackItem;
															trackItem.name = getTrackerName(obj, obj->GetName());
															trackItem.isLotd = true;
															trackItem.itemBaseFormId = obj->GetFormID();
															trackItem.isCont = true;
															trackPtrs2.insert(std::make_pair(actor, trackItem));
															//menu::tintTrack(actor);
															break;  // 后面修改支持多个
														}
													}
												}

												if (track::isAutoTrackItemsFlag) {
													if (data::autoTrackFormIds.find(obj->GetFormID()) != data::autoTrackFormIds.end()) {
														if (trackPtrs2.find(actor) == trackPtrs2.end()) {
															TrackItem trackItem;
															trackItem.name = getTrackerName(obj, obj->GetName());
															trackItem.isAuto = true;
															trackItem.itemBaseFormId = obj->GetFormID();
															trackItem.isCont = true;
															trackPtrs2.insert(std::make_pair(actor, trackItem));
															//menu::tintTrack(actor);
															break;  // 后面修改支持多个
														}
													}
												}
											}
										}
									}
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
										case RE::FormType::Scroll:
										case RE::FormType::Weapon:
										case RE::FormType::Armor:
										case RE::FormType::Ammo:
										case RE::FormType::Book:
										case RE::FormType::AlchemyItem:
										case RE::FormType::Ingredient:
										case RE::FormType::Misc:
										case RE::FormType::KeyMaster:
										case RE::FormType::Note:
										case RE::FormType::SoulGem:
										case RE::FormType::Tree:
										case RE::FormType::Flora:
											{
												if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
													continue;
												}

												if (!reff->Is3DLoaded()) {
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

												bool isCrime = reff->IsCrimeToActivate();
												if (isCrime && lotd::isAutoTrackLotdItemsCrimeIgnore) {
													continue;
												}

												switch (baseObj->GetFormType()) {
												case RE::FormType::Tree:
													{
														auto tree = baseObj->As<RE::TESObjectTREE>();
														if (tree) {
															if (!tree->produceItem) {
																continue;
															}
														}
														if (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested) {
															continue;
														}
														break;
													}
												case RE::FormType::Flora:
													{
														auto flora = baseObj->As<RE::TESFlora>();
														if (flora) {
															if (!flora->produceItem) {
																continue;
															}
														}
														if (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested) {
															continue;
														}
														break;
													}
												default:
													break;
												}

												if (lotd::isAutoTrackLotdItemsFlag) {
													bool success = lotd::checkItem(baseObj->GetFormID());
													if (success) {
														if (trackPtrs2.find(reff) == trackPtrs2.end()) {
															TrackItem trackItem;
															trackItem.name = getTrackerName(baseObj, name);
															trackItem.isLotd = true;
															trackItem.itemBaseFormId = baseObj->GetFormID();
															trackPtrs2.insert(std::make_pair(reff, trackItem));
															menu::tintTrack(reff);
															continue;
														}
													}
												}
												if (lotd::isAutoTrackLotdCardsFlag) {
													if (lotd::cardIds.find(baseObj->GetFormID()) != lotd::cardIds.end()) {
														if (trackPtrs2.find(reff) == trackPtrs2.end()) {
															TrackItem trackItem;
															std::string name = reff->GetDisplayFullName();
															trackItem.name = ICON_MDI_CARDS_PLAYING_CLUB_OUTLINE " " + name;
															trackItem.isLotd = true;
															trackItem.itemBaseFormId = baseObj->GetFormID();
															trackPtrs2.insert(std::make_pair(reff, trackItem));
															menu::tintTrack(reff);
															continue;
														}
													}
												}

												if (track::isAutoTrackItemsFlag) {
													if (data::autoTrackFormIds.find(baseObj->GetFormID()) != data::autoTrackFormIds.end()) {
														if (trackPtrs2.find(reff) == trackPtrs2.end()) {
															TrackItem trackItem;
															switch (baseObj->GetFormType()) {
															case RE::FormType::Tree:
															case RE::FormType::Flora:
																{
																	auto isHarvested = (reff->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested);
																	if (isHarvested) {
																		continue;
																	}
																	trackItem.isTree = true;
																}
															default:
																break;
															}
															trackItem.name = getTrackerName(baseObj, name);
															trackItem.isAuto = true;
															trackItem.itemBaseFormId = baseObj->GetFormID();
															trackPtrs2.insert(std::make_pair(reff, trackItem));
															menu::tintTrack(reff);
															continue;
														}
													}
												}

												break;
											}

										case RE::FormType::Container:
											{
												if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
													continue;
												}

												if (!reff->Is3DLoaded()) {
													continue;
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

												auto name = reff->GetDisplayFullName();
												if (strlen(name) == 0) {
													continue;
												}

												int direction = 0;
												if (show_items_window_direction) {
													direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
												}

												bool isCrime = reff->IsCrimeToActivate();

												if (!isCrime || !lotd::isAutoTrackLotdItemsCrimeIgnore) {
													auto inv = reff->GetInventory(FormUtil::CanDisplay);
													bool stealing = player->WouldBeStealing(reff);

													for (auto& [obj, data] : inv) {
														auto& [count, entry] = data;
														if (count > 0 && entry) {
															// 放到对应的数组里

															if (lotd::isAutoTrackLotdItemsFlag) {
																bool success = lotd::checkItem(obj->GetFormID());
																if (success) {
																	bool isCrime2 = !entry.get()->IsOwnedBy(player, !stealing);
																	if (isCrimeIgnore) {
																		if (isCrime2 && lotd::isAutoTrackLotdItemsCrimeIgnore) {
																			continue;
																		}
																	}
																	if (trackPtrs2.find(reff) == trackPtrs2.end()) {
																		TrackItem trackItem;
																		trackItem.name = getTrackerName(obj, obj->GetName());
																		trackItem.isLotd = true;
																		trackItem.itemBaseFormId = obj->GetFormID();
																		trackItem.isCont = true;
																		trackPtrs2.insert(std::make_pair(reff, trackItem));
																		menu::tintTrack(reff);
																		break;  // 后面修改支持多个
																	}
																}
															}

															if (lotd::isAutoTrackLotdCardsFlag) {
																if (lotd::cardIds.find(obj->GetFormID()) != lotd::cardIds.end()) {
																	if (trackPtrs2.find(reff) == trackPtrs2.end()) {
																		TrackItem trackItem;
																		std::string name = obj->GetName();
																		trackItem.name = ICON_MDI_CARDS_PLAYING_CLUB_OUTLINE " " + name;

																		trackItem.isLotd = true;
																		trackItem.itemBaseFormId = obj->GetFormID();
																		trackItem.isCont = true;
																		trackPtrs2.insert(std::make_pair(reff, trackItem));
																		menu::tintTrack(reff);
																		break;  // 后面修改支持多个
																	}
																}
															}

															if (track::isAutoTrackItemsFlag) {
																if (data::autoTrackFormIds.find(obj->GetFormID()) != data::autoTrackFormIds.end()) {
																	if (trackPtrs2.find(reff) == trackPtrs2.end()) {
																		TrackItem trackItem;
																		trackItem.name = getTrackerName(obj, obj->GetName());
																		trackItem.isAuto = true;
																		trackItem.itemBaseFormId = obj->GetFormID();
																		trackItem.isCont = true;
																		trackPtrs2.insert(std::make_pair(reff, trackItem));
																		menu::tintTrack(reff);
																		continue;
																	}
																}
															}
														}
													}
												}

												break;
											}

										case RE::FormType::Activator:
											{
												if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
													continue;
												}

												if (!reff->Is3DLoaded()) {
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

												// 挖掘点 自动标记

												if (lotd::isAutoTrackLotdExcavationFlag) {
													if (lotd::excavationIds.find(baseObj->GetFormID()) != lotd::excavationIds.end()) {
														TrackItem trackItem;
														trackItem.name = ICON_MDI_SPADE " " + std::string(name);
														trackItem.isLotd = true;
														trackItem.itemBaseFormId = baseObj->GetFormID();
														trackPtrs2.insert(std::make_pair(reff, trackItem));
														menu::tintTrack(reff);
														continue;
													}
												}

												if (reff->formFlags & RE::TESForm::RecordFlags::kDestroyed) {
													continue;
												}

												if (track::isAutoTrackItemsFlag) {
													if (data::autoTrackFormIds.find(baseObj->GetFormID()) != data::autoTrackFormIds.end()) {
														if (trackPtrs2.find(reff) == trackPtrs2.end()) {
															TrackItem trackItem;
															trackItem.name = getTrackerName(baseObj, name);
															trackItem.isAuto = true;
															trackItem.itemBaseFormId = baseObj->GetFormID();
															trackPtrs2.insert(std::make_pair(reff, trackItem));
															menu::tintTrack(reff);
															continue;
														}
													}
												}

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
										default:
											{
												continue;
											}
										}
									}
								}
							}
						}
					}
				}
			}

			lotd::isAutoTrackLotdItemsFlag = false;
			lotd::isAutoTrackLotdExcavationFlag = false;
			lotd::isAutoTrackLotdCardsFlag = false;
			track::isAutoTrackItemsFlag = false;
		}
	}
}
