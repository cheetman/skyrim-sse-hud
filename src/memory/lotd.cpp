#include "lotd.h"
#include <event/BSTContainerChangedEvent.h>
#include <filesystem>
#include <fonts/IconsMaterialDesignIcons.h>
#include <memory/memory.h>
#include <setting/setting.h>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>
#include <utils/utils.h>
#include <windows.h>

namespace lotd
{
	std::vector<List> lists;
	std::map<std::string, std::vector<Form>> listsR;
	std::map<std::string, std::unordered_set<RE::FormID>> formIdsR;
	std::map<std::string, std::unordered_set<RE::FormID>> formIdsM;
	std::unordered_set<RE::FormID> formIds;

	std::map<std::string, std::string> roomNames;
	//std::map<std::string, std::unordered_set<RE::FormID>> displayIdsC;
	std::map<std::string, std::unordered_set<RE::FormID>> displayIdsR;

	// 艺术馆陈列品
	std::unordered_set<RE::FormID> displayIds;
	std::map<std::uint32_t, std::unordered_set<RE::FormID>> displayIdsC;

	// 身上物品
	std::unordered_set<RE::FormID> playerInvIds;

	// 艺术馆地点
	std::unordered_set<RE::FormID> locationIds;

	Lotd2Info* lotdItems = new Lotd2Info[2];
	Lotd2InfoAttached* lotdItemsAttached = new Lotd2InfoAttached[2];

	bool isLoad = false;
	bool isShow = false;
	bool isShowAttached = false;
	bool isInvIgnore = true;
	bool isCrimeIgnore = false;
	bool isArmoryIgnore = true;

	std::uint8_t lotdCompileIndex;
	std::uint8_t lotdSmallFileCompileIndex;
	int nowItemIndex = 0;

	std::unordered_set<RE::FormID> locationItemIds;
	int locationItemCount = 0;

	bool showlocationItemCount = false;

	bool compareForLotdItem(const LotdInfo& info1, const LotdInfo& info2)
	{
		if (info1.gold != info2.gold) {
			return info1.gold > info2.gold;
		} else {
			return info1.baseFormId < info2.baseFormId;
		}
	}

	bool compareForLotdItemAttached(const LotdInfo& info1, const LotdInfo& info2)
	{
		if (info1.roomName != info2.roomName) {
			return info1.roomName > info2.roomName;
		} else {
			return info1.baseFormId < info2.baseFormId;
		}
	}

	std::vector<LotdInfo>& getItems(std::string roomName)
	{
		return lotdItems[!nowItemIndex].lists[roomName];
	}

	int getCount(std::string roomName)
	{
		return lotdItems[!nowItemIndex].counts[roomName];
	}

	std::vector<LotdInfo>& getItemsAttached()
	{
		return lotdItemsAttached[!nowItemIndex].list;
	}

	int getCountAttached()
	{
		return lotdItemsAttached[!nowItemIndex].count;
	}

	void init()
	{
		// 读取配置文件
		if (!setting::load_settings_lotd()) {
			return;
		}
		// 写死中文
		roomNames.insert({ "Armory", "军械库" });
		roomNames.insert({ "DaedricGallery", "魔族大厅" });
		roomNames.insert({ "DragonbornHall", "龙裔大厅" });
		roomNames.insert({ "HallOfHeroes", "英雄大厅" });
		roomNames.insert({ "HallOfLostEmpires", "失落帝国大厅" });
		roomNames.insert({ "HallOfOddities", "奇物大厅" });
		roomNames.insert({ "HallOfSecrets", "秘密大厅" });
		roomNames.insert({ "GalleryLibrary", "艺术馆图书室" });
		roomNames.insert({ "NaturalScience", "自然科学" });
		roomNames.insert({ "MuseumStoreroom", "博物馆储藏室" });
		roomNames.insert({ "Guildhouse", "探险家协会" });

		/*displayIdsC["00126087"];
		displayIdsC["002ACDD9"];
		displayIdsC["006C22C2"];
		displayIdsC["002F8E89"];*/

		//初始化艺术馆
		const auto handler = RE::TESDataHandler::GetSingleton();

		auto fileLotd = handler->LookupModByName("LegacyoftheDragonborn.esm");
		if (!fileLotd || fileLotd->compileIndex == 0xFF) {
			return;
		}

		isLoad = true;
		lotdCompileIndex = fileLotd->compileIndex;
		lotdSmallFileCompileIndex = fileLotd->smallFileCompileIndex;

		// 地点排除
		locationIds.insert(FormUtil::GetFormId(0x139074, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x1432E2, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D2D82, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D2D83, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D2D87, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D2D88, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D7E90, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D7E91, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D7E92, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D7E94, lotdCompileIndex, lotdSmallFileCompileIndex));
		locationIds.insert(FormUtil::GetFormId(0x5D7E95, lotdCompileIndex, lotdSmallFileCompileIndex));

		// 陈列品箱子
		for (auto& contId : setting::displayConts) {
			displayIdsC[FormUtil::GetFormId(contId, lotdCompileIndex, lotdSmallFileCompileIndex)];
		}

		// 获取物品列表
		for (auto& listItem : setting::lotdItemLists) {
			List list(listItem.listEditorId, listItem.roomName);
			// 不存在初始化
			if (listsR.find(listItem.roomName) == listsR.end()) {
				std::vector<Form> forms;
				listsR.insert({ listItem.roomName, forms });
			}
			if (formIdsR.find(listItem.roomName) == formIdsR.end()) {
				std::unordered_set<RE::FormID> formids;
				formIdsR.insert({ listItem.roomName, formids });
			}
			if (formIdsM.find(listItem.modName) == formIdsM.end()) {
				std::unordered_set<RE::FormID> formids;
				formIdsM.insert({ listItem.modName, formids });
			}

			RE::BGSListForm* listform = RE::TESForm::LookupByEditorID<RE::BGSListForm>(listItem.listEditorId);
			if (listform) {
				for (auto form : listform->forms) {
					if (form->Is(RE::FormType::Activator)) {
						list.sizeACTI++;
					} else if (form->Is(RE::FormType::FormList)) {
						list.sizeFLST++;
						auto listform2 = form->As<RE::BGSListForm>();
						if (listform2) {
							for (auto form2 : listform2->forms) {
								if (form2->Is(RE::FormType::Activator)) {
									list.sizeACTI2++;
								} else if (form2->Is(RE::FormType::FormList)) {
									list.sizeFLST2++;
								} else {
									list.size2++;
									// 集合 按照他的FLST区分
									list.forms.push_back({ form2->formID, form2->formType.get(), form2->GetName(), GetFormTypeName(form2->formType.underlying()), listItem.roomName });
									// 集合 按照房间区分
									listsR[listItem.roomName].push_back({ form2->formID, form2->formType.get(), form2->GetName(), GetFormTypeName(form2->formType.underlying()), listItem.roomName });
									formIdsR[listItem.roomName].insert(form2->formID);
									formIds.insert(form2->formID);
									// 集合 按照mod区分
									formIdsM[listItem.modName].insert(form2->formID);
								}
							}
						}

					} else {
						list.size++;
						// 集合 按照他的FLST区分
						list.forms.push_back({ form->formID, form->formType.get(), form->GetName(), GetFormTypeName(form->formType.underlying()), listItem.roomName });
						// 集合 按照房间区分
						listsR[listItem.roomName].push_back({ form->formID, form->formType.get(), form->GetName(), GetFormTypeName(form->formType.underlying()), listItem.roomName });
						formIdsR[listItem.roomName].insert(form->formID);
						formIds.insert(form->formID);
						// 集合 按照mod区分
						formIdsM[listItem.modName].insert(form->formID);
					}
				}
			}
			lists.push_back(list);
		}
	}

	LotdInfo* findItem(RE::FormID baseFormId, std::map<std::string, int>& tmpCountMap)
	{
		// 先判断是否陈列
		if (displayIds.find(baseFormId) != displayIds.end()) {
			return nullptr;
		}

		auto& nowMap = lotdItems[nowItemIndex].lists;
		for (const auto& pair : formIdsR) {
			// 忽略军械库
			if (isArmoryIgnore) {
				if (pair.first == "Armory") {
					continue;
				}
			}
			if (pair.second.find(baseFormId) != pair.second.end()) {
				auto& nowItemInfo = nowMap[pair.first];
				// 取现在的索引
				int tmpCount = tmpCountMap[pair.first];
				// 检查一下长度
				if (tmpCount + 1 > nowItemInfo.size()) {
					nowItemInfo.resize(nowItemInfo.size() + 20);
				}

				if (isInvIgnore) {
					if (playerInvIds.find(baseFormId) != playerInvIds.end()) {
						return nullptr;
					}
				}
				//tmpCountMap[pair.first]++;
				nowItemInfo[tmpCount].roomName = pair.first;
				return &nowItemInfo[tmpCount];
			}
		}
		return nullptr;
	}

	bool checkItem(RE::FormID baseFormId)
	{
		// 先判断是否陈列
		if (displayIds.find(baseFormId) != displayIds.end()) {
			return false;
		}

		for (const auto& pair : formIdsR) {
			// 忽略军械库
			if (isArmoryIgnore) {
				if (pair.first == "Armory") {
					continue;
				}
			}
			if (pair.second.find(baseFormId) != pair.second.end()) {
				if (isInvIgnore) {
					if (playerInvIds.find(baseFormId) != playerInvIds.end()) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}

	LotdInfo* findItemAttached(RE::FormID baseFormId, int& tmpCount)
	{
		// 先判断是否陈列
		if (displayIds.find(baseFormId) != displayIds.end()) {
			return nullptr;
		}

		auto& nowItemInfo = lotdItemsAttached[nowItemIndex].list;
		for (const auto& pair : formIdsR) {
			// 忽略军械库
			if (isArmoryIgnore) {
				if (pair.first == "Armory") {
					continue;
				}
			}
			if (pair.second.find(baseFormId) != pair.second.end()) {
				// 检查一下长度
				if (tmpCount + 1 > nowItemInfo.size()) {
					nowItemInfo.resize(nowItemInfo.size() + 20);
				}
				nowItemInfo[tmpCount].roomName = pair.first;

				if (isInvIgnore) {
					if (playerInvIds.find(baseFormId) != playerInvIds.end()) {
						return nullptr;
					}
				}
				//tmpCount++;
				return &nowItemInfo[tmpCount];
			}
		}
		return nullptr;
	}

	void refreshInvItemsCache()
	{
		if (isInvIgnore) {
			if (isPlayerInvChanged) {
				RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
				if (!player) {
					return;
				}
				playerInvIds.clear();

				auto inv = player->GetInventory(FormUtil::CanDisplay);
				for (auto& [obj, data] : inv) {
					auto& [count, entry] = data;
					if (count > 0 && entry) {
						playerInvIds.insert(obj->GetFormID());
					}
				}
				isPlayerInvChanged = false;
			}
		}
	}

	void refreshDisplayItemsCache()
	{
		if (islotdContChanged) {

			// 1.清空
			displayIds.clear();
			// 删除之前的分类(后面改进)
			for (auto& pair : displayIdsR) {
				pair.second.clear();
			}


			for (auto& pair : displayIdsC) {
				auto form = RE::TESForm::LookupByID(pair.first);
				if (form) {
					auto reff = form->AsReference();
					if (reff) {
						if (reff->GetObjectReference()->Is(RE::FormType::Container)) {
							auto inv = reff->GetInventory(FormUtil::CanDisplay);
							for (auto& [obj, data] : inv) {
								auto& [count, entry] = data;
								if (count > 0 && entry) {
									// 无所谓暂时不区分
									//pair.second.insert(obj->GetFormID());
									displayIds.insert(obj->GetFormID());

									// 显示到对应的房间(后面改进)
									for (const auto& pair : formIdsR) {
										if (pair.second.find(obj->GetFormID()) != pair.second.end()) {
											displayIdsR[pair.first].insert(obj->GetFormID());
											break;
										}
									}
								}
							}
						}
					}
				}
			}

			islotdContChanged = false;
		}
	}

	void refreshItemInfo()
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		// 缓存身上物品
		refreshInvItemsCache();

		// 缓存陈列品
		refreshDisplayItemsCache();

		nowItemIndex = !nowItemIndex;
		Sleep(100);

		// 数量统计(int)
		std::map<std::string, int> tmpCountMap;

		int currentLocationFormId = 0;
		auto currentLocation = player->currentLocation;
		if (currentLocation) {
			currentLocationFormId = currentLocation->GetFormID();
		}

		// 艺术馆地点排除
		if (lotd::locationIds.find(currentLocationFormId) == lotd::locationIds.end()) {
			const auto& [map, lock] = RE::TESForm::GetAllForms();
			//const RE::BSReadLockGuard locker{ lock };
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

								int direction = 0;
								if (show_items_window_direction) {
									direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
								}

								bool isCrime = actor->IsCrimeToActivate();
								auto inv = actor->GetInventory(FormUtil::CanDisplay);
								for (auto& [obj, data] : inv) {
									auto& [count, entry] = data;
									if (count > 0 && entry) {
										// 放到对应的数组里
										LotdInfo* itemptr = findItem(obj->GetFormID(), tmpCountMap);
										if (itemptr) {
											// 外面加索引
											tmpCountMap[itemptr->roomName]++;

											itemptr->count = count;
											itemptr->contname = name;
											itemptr->contptr = actor;
											itemptr->contformId = actor->GetFormID();

											itemptr->ptr = nullptr;
											itemptr->formId = 0;
											itemptr->baseFormId = obj->GetFormID();
											itemptr->weight = obj->GetWeight();
											itemptr->gold = obj->GetGoldValue();
											itemptr->isCrime = isCrime;
											itemptr->isEnchanted = entry->IsEnchanted();  // 注意

											if (show_items_window_direction) {
												itemptr->distance = distance;
												itemptr->direction = direction;
											}
											if (show_items_window_file) {
												itemptr->filename = obj->GetFile(0)->fileName;
											}

											switch (obj->GetFormType()) {
											case RE::FormType::Scroll:
												itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
												break;
											case RE::FormType::Weapon:
												itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
												break;
											case RE::FormType::Armor:
												itemptr->name = ICON_MDI_SHIELD " " + std::string(obj->GetName());
												break;
											case RE::FormType::Ammo:
												itemptr->name = ICON_MDI_ARROW_PROJECTILE " " + std::string(obj->GetName());
												break;
											case RE::FormType::Book:
												itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
												break;
											case RE::FormType::AlchemyItem:
												{
													auto alchemyItem = obj->As<RE::AlchemyItem>();
													if (alchemyItem->IsFood()) {
														itemptr->name = ICON_MDI_FOOD_DRUMSTICK " " + std::string(obj->GetName());
													} else {
														itemptr->name = ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + std::string(obj->GetName());
													}
													break;
												}
											case RE::FormType::Ingredient:
												itemptr->name = ICON_MDI_SOURCE_BRANCH " " + std::string(obj->GetName());
												break;
											case RE::FormType::Misc:
												{
													auto misc = obj->As<RE::TESObjectMISC>();
													if (misc) {
														// 宝石
														if (FormUtil::HasKeyword(misc, VendorItemGem)) {
															itemptr->name = ICON_MDI_DIAMOND_STONE " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
															itemptr->name = ICON_MDI_ANVIL " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
															itemptr->name = ICON_MDI_BOX_CUTTER " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
															itemptr->name = ICON_MDI_RABBIT " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
															itemptr->name = ICON_MDI_TOOLS " " + std::string(obj->GetName());
														} else {
															itemptr->name = ICON_MDI_PACKAGE_VARIANT_CLOSED " " + std::string(obj->GetName());
														}
													}
													break;
												}
											case RE::FormType::KeyMaster:
												itemptr->name = ICON_MDI_KEY " " + std::string(obj->GetName());
												break;
											case RE::FormType::Note:
												itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
												break;
											case RE::FormType::SoulGem:
												itemptr->name = ICON_MDI_CARDS_DIAMOND " " + std::string(obj->GetName());
												break;
											default:
												break;
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

											// 放到对应的数组里
											LotdInfo* itemptr = findItem(baseObj->GetFormID(), tmpCountMap);
											if (itemptr) {
												bool isCrime = reff->IsCrimeToActivate();
												if (isCrimeIgnore) {
													if (isCrime) {
														continue;
													}
												}
												tmpCountMap[itemptr->roomName]++;

												itemptr->ptr = reff;
												itemptr->formId = reff->GetFormID();
												itemptr->baseFormId = baseObj->GetFormID();
												itemptr->weight = reff->GetWeight();
												itemptr->gold = baseObj->GetGoldValue();
												itemptr->isCrime = isCrime;
												itemptr->isEnchanted = reff->IsEnchanted();

												itemptr->contformId = 0;

												if (show_items_window_direction) {
													itemptr->distance = distance;
													itemptr->direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
												}
												if (show_items_window_file) {
													itemptr->filename = baseObj->GetFile(0)->fileName;
												}

												switch (baseObj->GetFormType()) {
												case RE::FormType::Scroll:
													itemptr->name = ICON_MDI_SWORD " " + std::string(name);
													break;
												case RE::FormType::Weapon:
													itemptr->name = ICON_MDI_SWORD " " + std::string(name);
													break;
												case RE::FormType::Armor:
													itemptr->name = ICON_MDI_SHIELD " " + std::string(name);
													break;
												case RE::FormType::Ammo:
													itemptr->name = ICON_MDI_ARROW_PROJECTILE " " + std::string(name);
													break;
												case RE::FormType::Book:
													itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(name);
													break;
												case RE::FormType::AlchemyItem:
													{
														auto alchemyItem = baseObj->As<RE::AlchemyItem>();
														if (alchemyItem->IsFood()) {
															itemptr->name = ICON_MDI_FOOD_DRUMSTICK " " + std::string(name);
														} else {
															itemptr->name = ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + std::string(name);
														}
														break;
													}
												case RE::FormType::Ingredient:
													itemptr->name = ICON_MDI_SOURCE_BRANCH " " + std::string(name);
													break;
												case RE::FormType::Misc:
													{
														auto misc = baseObj->As<RE::TESObjectMISC>();
														if (misc) {
															// 宝石
															if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																itemptr->name = ICON_MDI_DIAMOND_STONE " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																itemptr->name = ICON_MDI_ANVIL " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																itemptr->name = ICON_MDI_BOX_CUTTER " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																itemptr->name = ICON_MDI_RABBIT " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																itemptr->name = ICON_MDI_TOOLS " " + std::string(name);
															} else {
																itemptr->name = ICON_MDI_PACKAGE_VARIANT_CLOSED " " + std::string(name);
															}
														}
														break;
													}
												case RE::FormType::KeyMaster:
													itemptr->name = ICON_MDI_KEY " " + std::string(name);
													break;
												case RE::FormType::Note:
													itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(name);
													break;
												case RE::FormType::SoulGem:
													itemptr->name = ICON_MDI_CARDS_DIAMOND " " + std::string(name);
													break;
												default:
													break;
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

											auto inv = reff->GetInventory(FormUtil::CanDisplay);
											bool stealing = player->WouldBeStealing(reff);

											for (auto& [obj, data] : inv) {
												auto& [count, entry] = data;
												if (count > 0 && entry) {
													// 放到对应的数组里
													LotdInfo* itemptr = findItem(obj->GetFormID(), tmpCountMap);
													if (itemptr) {
														bool isCrime = !entry.get()->IsOwnedBy(player, !stealing);
														if (isCrimeIgnore) {
															if (isCrime) {
																continue;
															}
														}
														tmpCountMap[itemptr->roomName]++;

														itemptr->count = count;
														itemptr->contname = name;
														itemptr->contptr = reff;
														itemptr->contformId = reff->GetFormID();

														itemptr->ptr = nullptr;
														itemptr->formId = 0x0;
														//itemptr->name = obj->GetName();
														itemptr->baseFormId = obj->GetFormID();
														itemptr->weight = obj->GetWeight();
														itemptr->gold = obj->GetGoldValue();
														itemptr->isCrime = isCrime;
														itemptr->isEnchanted = entry->IsEnchanted();  // 注意

														if (show_items_window_direction) {
															itemptr->distance = distance;
															itemptr->direction = direction;
														}
														if (show_items_window_file) {
															itemptr->filename = obj->GetFile(0)->fileName;
														}

														switch (obj->GetFormType()) {
														case RE::FormType::Scroll:
															itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
															break;
														case RE::FormType::Weapon:
															itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
															break;
														case RE::FormType::Armor:
															itemptr->name = ICON_MDI_SHIELD " " + std::string(obj->GetName());
															break;
														case RE::FormType::Ammo:
															itemptr->name = ICON_MDI_ARROW_PROJECTILE " " + std::string(obj->GetName());
															break;
														case RE::FormType::Book:
															itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
															break;
														case RE::FormType::AlchemyItem:
															{
																auto alchemyItem = obj->As<RE::AlchemyItem>();
																if (alchemyItem->IsFood()) {
																	itemptr->name = ICON_MDI_FOOD_DRUMSTICK " " + std::string(obj->GetName());
																} else {
																	itemptr->name = ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + std::string(obj->GetName());
																}
																break;
															}
														case RE::FormType::Ingredient:
															itemptr->name = ICON_MDI_SOURCE_BRANCH " " + std::string(obj->GetName());
															break;
														case RE::FormType::Misc:
															{
																auto misc = obj->As<RE::TESObjectMISC>();
																if (misc) {
																	// 宝石
																	if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																		itemptr->name = ICON_MDI_DIAMOND_STONE " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																		itemptr->name = ICON_MDI_ANVIL " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																		itemptr->name = ICON_MDI_BOX_CUTTER " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																		itemptr->name = ICON_MDI_RABBIT " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																		itemptr->name = ICON_MDI_TOOLS " " + std::string(obj->GetName());
																	} else {
																		itemptr->name = ICON_MDI_PACKAGE_VARIANT_CLOSED " " + std::string(obj->GetName());
																	}
																}
																break;
															}
														case RE::FormType::KeyMaster:
															itemptr->name = ICON_MDI_KEY " " + std::string(obj->GetName());
															break;
														case RE::FormType::Note:
															itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
															break;
														case RE::FormType::SoulGem:
															itemptr->name = ICON_MDI_CARDS_DIAMOND " " + std::string(obj->GetName());
															break;
														default:
															break;
														}
													}
												}
											}
											break;
										}

									case RE::FormType::Tree:
									case RE::FormType::Activator:  // 应该不需要
									case RE::FormType::Flora:      // 应该不需要
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

		// 1.先清空
		for (auto& pair : lotdItems[nowItemIndex].counts) {
			pair.second = 0;
		}

		// 2.赋值
		for (const auto& pair : tmpCountMap) {
			lotdItems[nowItemIndex].counts[pair.first] = pair.second;
			// 3.排序
			auto& list = lotdItems[nowItemIndex].lists[pair.first];
			std::partial_sort(list.begin(), list.begin() + pair.second, list.begin() + pair.second, compareForLotdItem);
		}
	}

	void refreshItemInfoAttached()
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		// 缓存身上物品
		refreshInvItemsCache();

		// 缓存陈列品
		refreshDisplayItemsCache();

		nowItemIndex = !nowItemIndex;
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

								int direction = 0;
								if (show_items_window_direction) {
									direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
								}

								bool isCrime = actor->IsCrimeToActivate();
								auto inv = actor->GetInventory(FormUtil::CanDisplay);
								for (auto& [obj, data] : inv) {
									auto& [count, entry] = data;
									if (count > 0 && entry) {
										// 放到对应的数组里
										LotdInfo* itemptr = findItemAttached(obj->GetFormID(), tmpCount);
										if (itemptr) {
											tmpCount++;
											itemptr->count = count;
											itemptr->contname = name;
											itemptr->contptr = actor;
											itemptr->contformId = actor->GetFormID();

											itemptr->ptr = nullptr;
											itemptr->formId = 0;
											itemptr->baseFormId = obj->GetFormID();
											itemptr->weight = obj->GetWeight();
											itemptr->gold = obj->GetGoldValue();
											itemptr->isCrime = isCrime;
											itemptr->isEnchanted = entry->IsEnchanted();  // 注意

											if (show_items_window_direction) {
												itemptr->distance = distance;
												itemptr->direction = direction;
											}
											if (show_items_window_file) {
												itemptr->filename = obj->GetFile(0)->fileName;
											}

											switch (obj->GetFormType()) {
											case RE::FormType::Scroll:
												itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
												break;
											case RE::FormType::Weapon:
												itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
												break;
											case RE::FormType::Armor:
												itemptr->name = ICON_MDI_SHIELD " " + std::string(obj->GetName());
												break;
											case RE::FormType::Ammo:
												itemptr->name = ICON_MDI_ARROW_PROJECTILE " " + std::string(obj->GetName());
												break;
											case RE::FormType::Book:
												itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
												break;
											case RE::FormType::AlchemyItem:
												{
													auto alchemyItem = obj->As<RE::AlchemyItem>();
													if (alchemyItem->IsFood()) {
														itemptr->name = ICON_MDI_FOOD_DRUMSTICK " " + std::string(obj->GetName());
													} else {
														itemptr->name = ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + std::string(obj->GetName());
													}
													break;
												}
											case RE::FormType::Ingredient:
												itemptr->name = ICON_MDI_SOURCE_BRANCH " " + std::string(obj->GetName());
												break;
											case RE::FormType::Misc:
												{
													auto misc = obj->As<RE::TESObjectMISC>();
													if (misc) {
														// 宝石
														if (FormUtil::HasKeyword(misc, VendorItemGem)) {
															itemptr->name = ICON_MDI_DIAMOND_STONE " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
															itemptr->name = ICON_MDI_ANVIL " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
															itemptr->name = ICON_MDI_BOX_CUTTER " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
															itemptr->name = ICON_MDI_RABBIT " " + std::string(obj->GetName());
														} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
															itemptr->name = ICON_MDI_TOOLS " " + std::string(obj->GetName());
														} else {
															itemptr->name = ICON_MDI_PACKAGE_VARIANT_CLOSED " " + std::string(obj->GetName());
														}
													}
													break;
												}
											case RE::FormType::KeyMaster:
												itemptr->name = ICON_MDI_KEY " " + std::string(obj->GetName());
												break;
											case RE::FormType::Note:
												itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
												break;
											case RE::FormType::SoulGem:
												itemptr->name = ICON_MDI_CARDS_DIAMOND " " + std::string(obj->GetName());
												break;
											default:
												break;
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

											// 放到对应的数组里
											LotdInfo* itemptr = findItemAttached(baseObj->GetFormID(), tmpCount);
											if (itemptr) {
												bool isCrime = reff->IsCrimeToActivate();
												if (isCrimeIgnore) {
													if (isCrime) {
														continue;
													}
												}

												tmpCount++;
												itemptr->ptr = reff;
												itemptr->formId = reff->GetFormID();
												itemptr->baseFormId = baseObj->GetFormID();
												itemptr->weight = reff->GetWeight();
												itemptr->gold = baseObj->GetGoldValue();
												itemptr->isCrime = isCrime;
												itemptr->isEnchanted = reff->IsEnchanted();

												itemptr->contformId = 0;

												if (show_items_window_direction) {
													itemptr->distance = distance;
													itemptr->direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
												}
												if (show_items_window_file) {
													itemptr->filename = baseObj->GetFile(0)->fileName;
												}

												switch (baseObj->GetFormType()) {
												case RE::FormType::Scroll:
													itemptr->name = ICON_MDI_SWORD " " + std::string(name);
													break;
												case RE::FormType::Weapon:
													itemptr->name = ICON_MDI_SWORD " " + std::string(name);
													break;
												case RE::FormType::Armor:
													itemptr->name = ICON_MDI_SHIELD " " + std::string(name);
													break;
												case RE::FormType::Ammo:
													itemptr->name = ICON_MDI_ARROW_PROJECTILE " " + std::string(name);
													break;
												case RE::FormType::Book:
													itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(name);
													break;
												case RE::FormType::AlchemyItem:
													{
														auto alchemyItem = baseObj->As<RE::AlchemyItem>();
														if (alchemyItem->IsFood()) {
															itemptr->name = ICON_MDI_FOOD_DRUMSTICK " " + std::string(name);
														} else {
															itemptr->name = ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + std::string(name);
														}
														break;
													}
												case RE::FormType::Ingredient:
													itemptr->name = ICON_MDI_SOURCE_BRANCH " " + std::string(name);
													break;
												case RE::FormType::Misc:
													{
														auto misc = baseObj->As<RE::TESObjectMISC>();
														if (misc) {
															// 宝石
															if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																itemptr->name = ICON_MDI_DIAMOND_STONE " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																itemptr->name = ICON_MDI_ANVIL " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																itemptr->name = ICON_MDI_BOX_CUTTER " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																itemptr->name = ICON_MDI_RABBIT " " + std::string(name);
															} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																itemptr->name = ICON_MDI_TOOLS " " + std::string(name);
															} else {
																itemptr->name = ICON_MDI_PACKAGE_VARIANT_CLOSED " " + std::string(name);
															}
														}
														break;
													}
												case RE::FormType::KeyMaster:
													itemptr->name = ICON_MDI_KEY " " + std::string(name);
													break;
												case RE::FormType::Note:
													itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(name);
													break;
												case RE::FormType::SoulGem:
													itemptr->name = ICON_MDI_CARDS_DIAMOND " " + std::string(name);
													break;
												default:
													break;
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

											auto inv = reff->GetInventory(FormUtil::CanDisplay);
											bool stealing = player->WouldBeStealing(reff);

											for (auto& [obj, data] : inv) {
												auto& [count, entry] = data;
												if (count > 0 && entry) {
													// 放到对应的数组里
													LotdInfo* itemptr = findItemAttached(obj->GetFormID(), tmpCount);
													if (itemptr) {
														bool isCrime2 = !entry.get()->IsOwnedBy(player, !stealing);
														if (isCrimeIgnore) {
															if (isCrime2) {
																continue;
															}
														}
														tmpCount++;

														itemptr->count = count;
														itemptr->contname = name;
														itemptr->contptr = reff;
														itemptr->contformId = reff->GetFormID();

														itemptr->ptr = nullptr;
														itemptr->formId = 0x0;
														//itemptr->name = obj->GetName();
														itemptr->baseFormId = obj->GetFormID();
														itemptr->weight = obj->GetWeight();
														itemptr->gold = obj->GetGoldValue();
														itemptr->isCrime = isCrime2;
														itemptr->isEnchanted = entry->IsEnchanted();  // 注意

														if (show_items_window_direction) {
															itemptr->distance = distance;
															itemptr->direction = direction;
														}
														if (show_items_window_file) {
															itemptr->filename = obj->GetFile(0)->fileName;
														}

														switch (obj->GetFormType()) {
														case RE::FormType::Scroll:
															itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
															break;
														case RE::FormType::Weapon:
															itemptr->name = ICON_MDI_SWORD " " + std::string(obj->GetName());
															break;
														case RE::FormType::Armor:
															itemptr->name = ICON_MDI_SHIELD " " + std::string(obj->GetName());
															break;
														case RE::FormType::Ammo:
															itemptr->name = ICON_MDI_ARROW_PROJECTILE " " + std::string(obj->GetName());
															break;
														case RE::FormType::Book:
															itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
															break;
														case RE::FormType::AlchemyItem:
															{
																auto alchemyItem = obj->As<RE::AlchemyItem>();
																if (alchemyItem->IsFood()) {
																	itemptr->name = ICON_MDI_FOOD_DRUMSTICK " " + std::string(obj->GetName());
																} else {
																	itemptr->name = ICON_MDI_BOTTLE_TONIC_PLUS_OUTLINE " " + std::string(obj->GetName());
																}
																break;
															}
														case RE::FormType::Ingredient:
															itemptr->name = ICON_MDI_SOURCE_BRANCH " " + std::string(obj->GetName());
															break;
														case RE::FormType::Misc:
															{
																auto misc = obj->As<RE::TESObjectMISC>();
																if (misc) {
																	// 宝石
																	if (FormUtil::HasKeyword(misc, VendorItemGem)) {
																		itemptr->name = ICON_MDI_DIAMOND_STONE " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemOreIngot)) {
																		itemptr->name = ICON_MDI_ANVIL " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemAnimalHide)) {
																		itemptr->name = ICON_MDI_BOX_CUTTER " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemAnimalPart)) {
																		itemptr->name = ICON_MDI_RABBIT " " + std::string(obj->GetName());
																	} else if (FormUtil::HasKeyword(misc, VendorItemTool)) {
																		itemptr->name = ICON_MDI_TOOLS " " + std::string(obj->GetName());
																	} else {
																		itemptr->name = ICON_MDI_PACKAGE_VARIANT_CLOSED " " + std::string(obj->GetName());
																	}
																}
																break;
															}
														case RE::FormType::KeyMaster:
															itemptr->name = ICON_MDI_KEY " " + std::string(obj->GetName());
															break;
														case RE::FormType::Note:
															itemptr->name = ICON_MDI_BOOK_OPEN_VARIANT " " + std::string(obj->GetName());
															break;
														case RE::FormType::SoulGem:
															itemptr->name = ICON_MDI_CARDS_DIAMOND " " + std::string(obj->GetName());
															break;
														default:
															break;
														}
													}
												}
											}
											break;
										}

									case RE::FormType::Tree:
									case RE::FormType::Activator:  // 应该不需要
									case RE::FormType::Flora:      // 应该不需要
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

		auto& item = lotdItemsAttached[nowItemIndex];
		item.count = tmpCount;

		std::partial_sort(item.list.begin(), item.list.begin() + tmpCount, item.list.begin() + tmpCount, compareForLotdItemAttached);
	}

	void refreshCount()
	{
		if (showlocationItemCount) {

			// 缓存身上物品
			refreshInvItemsCache();

			// 缓存陈列品
			refreshDisplayItemsCache();

			locationItemIds.clear();

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (!player) {
				return;
			}

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

									auto inv = actor->GetInventory(FormUtil::CanDisplay);
									for (auto& [obj, data] : inv) {
										auto& [count, entry] = data;
										if (count > 0 && entry) {
											if (checkItem(obj->GetFormID())) {

												locationItemIds.insert(obj->GetFormID());
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

												if (checkItem(baseObj->GetFormID())) {
													if (isCrimeIgnore) {
														bool isCrime = reff->IsCrimeToActivate();
														if (isCrime) {
															continue;
														}
													}
													locationItemIds.insert(baseObj->GetFormID());
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

												bool stealing = player->WouldBeStealing(reff);
												auto inv = reff->GetInventory(FormUtil::CanDisplay);
												for (auto& [obj, data] : inv) {
													auto& [count, entry] = data;
													if (count > 0 && entry) {
														// 放到对应的数组里
														if (checkItem(obj->GetFormID())) {
															if (isCrimeIgnore) {
																bool isCrime = !entry.get()->IsOwnedBy(player, !stealing);
																if (isCrime) {
																	continue;
																}
															}
															locationItemIds.insert(obj->GetFormID());
														}
													}
												}
												break;
											}

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

			locationItemCount = locationItemIds.size();
		}
	}

}
