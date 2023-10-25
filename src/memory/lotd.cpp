#include "lotd.h"
#include <filesystem>
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
	Lotd2Info* lotdItems = new Lotd2Info[2];

	bool isLoad = false;
	bool isShow = false;
	int nowItemIndex = 0;

	std::vector<Form>& getItems(std::string& roomName)
	{
		return lotdItems[!nowItemIndex].lists[roomName];
	}

	int getCount(std::string& roomName)
	{
		return lotdItems[!nowItemIndex].counts[roomName];
	}

	void init()
	{
		// 读取配置文件
		if (!setting::load_settings_lotd()) {
			return;
		}

		//初始化艺术馆
		const auto handler = RE::TESDataHandler::GetSingleton();

		auto file = handler->LookupModByName("LegacyoftheDragonborn.esm");
		if (!file || file->compileIndex == 0xFF) {
			return;
		}
		isLoad = true;

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

			RE::BGSListForm* listform = RE::TESForm::LookupByEditorID<RE::BGSListForm>(listItem.listEditorId);
			if (listform) {
				for (auto form : listform->forms) {
					if (form->Is(RE::FormType::Activator)) {
						list.sizeACTI++;
					} else if (form->Is(RE::FormType::FormList)) {
						list.sizeFLST++;
					} else {
						list.size++;
						// 集合 按照他的FLST区分
						list.forms.push_back({ form->formID, form->formType.get(), form->GetName(), GetFormTypeName(form->formType.underlying()), listItem.roomName });
						// 集合 按照房间区分
						listsR[listItem.roomName].push_back({ form->formID, form->formType.get(), form->GetName(), GetFormTypeName(form->formType.underlying()), listItem.roomName });
						formIdsR[listItem.roomName].insert({ form->formID });
					}
				}
			}
			lists.push_back(list);
		}
	}

	Form* findItem(RE::FormID baseFormId, std::map<std::string, int>& tmpCountMap)
	{
		auto& nowMap = lotdItems[nowItemIndex].lists;
		for (const auto& pair : formIdsR) {
			if (pair.second.find(baseFormId) != pair.second.end()) {
				auto& nowItemInfo = nowMap[pair.first];
				int tmpCount = tmpCountMap[pair.first];
				// 检查一下长度
				if (tmpCountMap[pair.first] + 1 > nowItemInfo.size()) {
					nowItemInfo.resize(nowItemInfo.size() + 20);
				}
				tmpCountMap[pair.first]++;
				return &nowItemInfo[tmpCount];
			}
		}
		return nullptr;
	}

	void refreshItemInfo()
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		nowItemIndex = !nowItemIndex;
		Sleep(100);

		// 数量统计(int)
		std::map<std::string, int> tmpCountMap;

		auto currentLocation = player->currentLocation;

		{
			const auto& [map, lock] = RE::TESForm::GetAllForms();
			const RE::BSReadLockGuard locker{ lock };
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

								if (!show_items_window_3D) {
									if (!actor->Is3DLoaded()) {
										continue;
									}
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

								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].ptr = actor;
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].baseFormId = actor->GetFormID();
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].baseFormIdStr = FormIDToString(actor->GetFormID());
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].formId = actor->GetFormID();
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].formIdStr = FormIDToString(actor->GetFormID());
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].formTypeStr = GetFormTypeName(actor->formType.underlying());
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].name = name;
								////items[nowItemIndex].itemInfoACHR[tmpCountACHR].weight = actor->GetWeight();
								////items[nowItemIndex].itemInfoACHR[tmpCountACHR].gold = actor->GetGoldValue();
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].isCrime = actor->IsCrimeToActivate();
								////items[nowItemIndex].itemInfoACHR[tmpCountACHR].isEnchanted = actor->IsEnchanted();

								//if (show_items_window_direction) {
								//	items[nowItemIndex].itemInfoACHR[tmpCountACHR].distance = distance;
								//	items[nowItemIndex].itemInfoACHR[tmpCountACHR].direction = ValueUtil::calculateDirection(actor->GetPosition(), player->GetPosition(), player->GetAngle());
								//}

								int tmpInvCount = 0;
								auto inv = actor->GetInventory(FormUtil::CanDisplay);
								for (auto& [obj, data] : inv) {
									auto& [count, entry] = data;
									if (count > 0 && entry) {
#ifndef NDEBUG
										if (obj->IsIgnored()) {
											MessageBox(nullptr, obj->GetName(), nullptr, MB_OK);
										}
#endif

										/*items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].ptr = obj;
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].name = obj->GetName();
										bool stealing = player->WouldBeStealing(actor);
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].isCrime = !entry.get()->IsOwnedBy(player, !stealing);
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount].isEnchanted = entry.get()->IsEnchanted();
										items[nowItemIndex].itemInfoACHR[tmpCountACHR].invs[tmpInvCount++].count = count;*/
										if (tmpInvCount == 200) {
											break;
										}
									}
								}
								//items[nowItemIndex].itemInfoACHR[tmpCountACHR].invCount = tmpInvCount;

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
									case RE::FormType::Armor:
									case RE::FormType::Ammo:
									case RE::FormType::Book:
									case RE::FormType::AlchemyItem:
									case RE::FormType::Ingredient:
									case RE::FormType::Misc:
									case RE::FormType::Tree:
									case RE::FormType::KeyMaster:
									case RE::FormType::SoulGem:
										{
											if (reff->IsMarkedForDeletion() || reff->IsIgnored()) {
												continue;
											}

											if (!reff->Is3DLoaded()) {
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

											// 放到对应的数组里
											Form* itemptr = findItem(baseObj->GetFormID(), tmpCountMap);
											if (itemptr) {
												itemptr->formId = reff->GetFormID();
												itemptr->name = name;
											}

											/*items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].ptr = reff;
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
												items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoWEAP[tmpCountWEAP].filename = baseObj->GetFile(0)->fileName;
											}*/

											break;
										}

									case RE::FormType::Container:
										{
											if (reff->IsMarkedForDeletion()) {
												continue;
											}

#ifndef NDEBUG
											if (reff->IsIgnored()) {
												MessageBox(nullptr, reff->GetDisplayFullName(), nullptr, MB_OK);
											}
#endif

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

											/*items[nowItemIndex].itemInfoCONT[tmpCountCONT].ptr = reff;
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
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].direction = ValueUtil::calculateDirection(reff->GetPosition(), player->GetPosition(), player->GetAngle());
											}
											if (show_items_window_file) {
												items[nowItemIndex].itemInfoCONT[tmpCountCONT].filename = baseObj->GetFile(0)->fileName;
											}*/

											int tmpInvCount = 0;
											auto inv = reff->GetInventory(FormUtil::CanDisplay);

											// 自动拾取条件1
											/*bool auto1 = false;
											if (show_items_window_auto_cont) {
												if (autoContFormIds.find(baseObj->GetFormID()) != autoContFormIds.end()) {
													auto1 = true;
												}
											}
											items[nowItemIndex].itemInfoCONT[tmpCountCONT].isAuto = auto1;*/

											for (auto& [obj, data] : inv) {
												auto& [count, entry] = data;
												if (count > 0 && entry) {
#ifndef NDEBUG
													if (obj->IsIgnored()) {
														MessageBox(nullptr, obj->GetName(), nullptr, MB_OK);
													}
#endif

													/*items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].ptr = obj;
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].name = obj->GetName();
													bool stealing = player->WouldBeStealing(reff);
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].isCrime = !entry.get()->IsOwnedBy(player, !stealing);
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount].isEnchanted = entry.get()->IsEnchanted();
													items[nowItemIndex].itemInfoCONT[tmpCountCONT].invs[tmpInvCount++].count = count;*/
													if (tmpInvCount == 200) {
														break;
													}
												}
											}

											//items[nowItemIndex].itemInfoCONT[tmpCountCONT].invCount = tmpInvCount;


											break;
										}

									case RE::FormType::Activator: // 应该不需要
									case RE::FormType::Flora: // 应该不需要
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
	}
}
