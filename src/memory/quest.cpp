#include "quest.h"
#include <fonts/IconsMaterialDesignIcons.h>
#include <memory/memory.h>

namespace quest
{
	// 任务忽略
	std::unordered_set<RE::FormID> excludeQuestFormIds;

	bool compareForQuest(const QuestInfo& info1, const QuestInfo& info2)
	{
		if (info1.isActive != info2.isActive) {
			return info1.isActive > info2.isActive;
		} else if (info1.progressStage != info2.progressStage) {
			return info1.progressStage > info2.progressStage;
		} else {
			return info1.formId > info2.formId;
		}
	}

	void refreshQuestInfo(Item2Info& item)
	{
		//auto testLocation = RE::TESForm::LookupByID<RE::BGSLocation>(0x192A9);
		int tmpQuestCount = 0;

		for (const auto quest : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESQuest>()) {
			/*if (quest->IsActive()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					tmpQuestCount++;
				}*/

			/*if (quest->IsStarting()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					tmpQuestCount++;
				}

				if (quest->IsCompleted()) {
					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = quest->GetCurrentStageID();
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					tmpQuestCount++;
				}*/

			if (excludeQuestFormIds.find(quest->GetFormID()) != excludeQuestFormIds.end()) {
				continue;
			}
			if (std::string(quest->GetFormEditorID()) == "BQ01") {
				// 看看状态
				int i = 0;
			}

			if (quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) && !quest->IsCompleted()) {
				if (quest->waitingStages) {
					auto currentStageID = quest->GetCurrentStageID();
					int completedStage = 0;
					int allStage = 0;
					std::uint16_t lastIndex = 0;
					for (auto item : *quest->waitingStages) {
						allStage++;
						if (item->data.flags.underlying() == 1) {
							//completedStage++;
							completedStage = allStage;
						}
						lastIndex = item->data.index;
					}

					// 排除已经到最后阶段的任务
					if (currentStageID == lastIndex) {
						continue;
					}

					item.quests[tmpQuestCount].name = quest->GetFullName();
					item.quests[tmpQuestCount].formId = quest->GetFormID();
					item.quests[tmpQuestCount].currentInstanceID = quest->currentInstanceID;
					item.quests[tmpQuestCount].currentStage = currentStageID;
					item.quests[tmpQuestCount].editorId = quest->GetFormEditorID();
					item.quests[tmpQuestCount].isActive = quest->IsActive();

					item.quests[tmpQuestCount].allStage = allStage;
					item.quests[tmpQuestCount].completedStage = completedStage;
					item.quests[tmpQuestCount].progressStage = (float)completedStage / (float)(allStage == 0 ? 1 : allStage);
					item.quests[tmpQuestCount].ptr = quest;
					std::string questTypeName = "";
					auto questType = quest->data.questType.get();
					switch (questType) {
					case RE::QUEST_DATA::Type::kNone:
						questTypeName = "-";
						break;
					case RE::QUEST_DATA::Type::kMainQuest:
						questTypeName = "主线任务";
						break;
					case RE::QUEST_DATA::Type::kMagesGuild:
						questTypeName = "法师工会";
						break;
					case RE::QUEST_DATA::Type::kThievesGuild:
						questTypeName = "盗贼工会";
						break;
					case RE::QUEST_DATA::Type::kDarkBrotherhood:
						questTypeName = "黑暗兄弟会";
						break;
					case RE::QUEST_DATA::Type::kDaedric:
						questTypeName = "魔神任务";
						break;
					case RE::QUEST_DATA::Type::kSideQuest:
						questTypeName = "支线任务";
						break;
					case RE::QUEST_DATA::Type::kCivilWar:
						questTypeName = "内战任务";
						break;
					case RE::QUEST_DATA::Type::kDLC01_Vampire:
						questTypeName = "DLC1黎明守卫";
						break;
					case RE::QUEST_DATA::Type::kDLC02_Dragonborn:
						questTypeName = "DLC2龙裔";
						break;
					default:
						break;
					}
					item.quests[tmpQuestCount].questTypeName = questTypeName;
					item.quests[tmpQuestCount].filename = quest->GetFile(0)->fileName;
					item.quests[tmpQuestCount].flag = quest->data.flags.underlying();
					// 目标
					int aliasCount = quest->aliases.size();
					if (item.quests[tmpQuestCount].aliases.size() < aliasCount) {
						item.quests[tmpQuestCount].aliases.resize(aliasCount);
					}
					aliasCount = 0;
					quest->aliasAccessLock.LockForRead();
					for (auto alias : quest->aliases) {
						item.quests[tmpQuestCount].aliases[aliasCount].aliasID = alias->aliasID;
						item.quests[tmpQuestCount].aliases[aliasCount].aliasName = alias->aliasName;
						item.quests[tmpQuestCount].aliases[aliasCount].targetName = "";
						item.quests[tmpQuestCount].aliases[aliasCount].targetFormId = 0;
						item.quests[tmpQuestCount].aliases[aliasCount].type = "";
						item.quests[tmpQuestCount].aliases[aliasCount].fillType = "";
						item.quests[tmpQuestCount].aliases[aliasCount].refPtr = nullptr;
						item.quests[tmpQuestCount].aliases[aliasCount].npcPtr = nullptr;
						item.quests[tmpQuestCount].aliases[aliasCount].isLink = false;

						if (alias->GetVMTypeID() == RE::BGSRefAlias::VMTYPEID) {
							auto reference = skyrim_cast<RE::BGSRefAlias*>(alias);
							if (reference) {
								item.quests[tmpQuestCount].aliases[aliasCount].type = "Ref";
								if (reference->fillType == RE::BGSBaseAlias::FILL_TYPE::kForced) {
									item.quests[tmpQuestCount].aliases[aliasCount].fillType = "Forced";

									auto ref = reference->fillData.forced.forcedRef;
									if (ref) {
										std::string name = ref.get().get()->GetDisplayFullName();
										if (name.empty()) {
											continue;
										}
										item.quests[tmpQuestCount].aliases[aliasCount].targetName = ICON_MDI_MAP_MARKER_RADIUS + std::string(" ") + name;
										item.quests[tmpQuestCount].aliases[aliasCount].targetFormId = ref.get().get()->GetFormID();
										item.quests[tmpQuestCount].aliases[aliasCount].refPtr = ref.get().get();
									}
								} else if (reference->fillType == RE::BGSBaseAlias::FILL_TYPE::kUniqueActor) {
									item.quests[tmpQuestCount].aliases[aliasCount].fillType = "UniqueActor";
									auto ref = reference->fillData.uniqueActor.uniqueActor;
									if (ref) {
										std::string name = ref->GetFullName();
										if (name.empty()) {
											continue;
										}
										item.quests[tmpQuestCount].aliases[aliasCount].targetName = ICON_MDI_ACCOUNT + std::string(" ") + name;
										item.quests[tmpQuestCount].aliases[aliasCount].targetFormId = ref->GetFormID();
										item.quests[tmpQuestCount].aliases[aliasCount].npcPtr = ref;
									}
								} else if (reference->fillType == RE::BGSBaseAlias::FILL_TYPE::kCreated) {
									item.quests[tmpQuestCount].aliases[aliasCount].fillType = "Created";
									auto obj = reference->fillData.created.object;
									if (obj) {
										if (reference->fillData.created.alias.create == RE::BGSRefAlias::CreatedFillData::Alias::Create::kIn) {
										}
										item.quests[tmpQuestCount].aliases[aliasCount].targetName = obj->GetName();
										item.quests[tmpQuestCount].aliases[aliasCount].targetFormId = obj->GetFormID();
										//item.quests[tmpQuestCount].aliases[aliasCount].objPtr = obj;
									}
								}
							}
						} else if (alias->GetVMTypeID() == RE::BGSLocAlias::VMTYPEID) {
							auto reference = skyrim_cast<RE::BGSLocAlias*>(alias);
							if (reference) {
								item.quests[tmpQuestCount].aliases[aliasCount].type = "Loc";
								if (reference->fillType == RE::BGSBaseAlias::FILL_TYPE::kForced) {
									item.quests[tmpQuestCount].aliases[aliasCount].fillType = "Forced";
									if (reference->unk28) {
										item.quests[tmpQuestCount].aliases[aliasCount].targetName = ((RE::BGSLocation*)reference->unk28)->GetFullName();
										item.quests[tmpQuestCount].aliases[aliasCount].targetFormId = ((RE::BGSLocation*)reference->unk28)->GetFormID();
									}
								}
							}
						}
						/*if (testLocation) {
							item.quests[tmpQuestCount].aliases[aliasCount].targetFormId9 = (std::uint64_t)testLocation;
						}*/

						aliasCount++;
					}
					quest->aliasAccessLock.UnlockForRead();
					item.quests[tmpQuestCount].aliasCount = aliasCount;

					tmpQuestCount++;
				}
			}
		}

		std::partial_sort(item.quests.begin(), item.quests.begin() + tmpQuestCount, item.quests.begin() + tmpQuestCount, compareForQuest);
		item.questCount = tmpQuestCount;
	}
}
