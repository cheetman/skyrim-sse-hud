#pragma once
#include <unordered_set>

struct Item2Info;

struct QuestAliasInfo
{
	std::uint32_t aliasID = 0;
	std::string aliasName = "";
	std::string targetName = "";
	RE::FormID targetFormId = 0;
	std::string type = "";
	std::string fillType = "";
	RE::TESObjectREFR* refPtr = nullptr;
	RE::TESNPC* npcPtr = nullptr;
	RE::TESBoundObject* objPtr = nullptr;

	bool isLink = false;
};

struct QuestInfo
{
	RE::TESQuest* ptr = nullptr;
	RE::FormID formId = 0;
	std::uint16_t currentStage = 0;
	std::uint32_t currentInstanceID = 0;
	std::string formIdStr = "";
	RE::FormID baseFormId = 0;
	std::string editorId = "";
	std::string formTypeStr = "";
	std::string filename = "";
	std::uint16_t flag = 0;

	std::string name = "";
	bool isActive = false;
	RE::LOCK_LEVEL lockLevel;
	bool isDeleted = false;
	bool isHarvested = false;
	std::string questTypeName = "";

	int completedStage = 0;
	int allStage = 0;
	float progressStage = 0;
	float distance = 0.0f;
	bool isAuto = false;
	std::vector<QuestAliasInfo> aliases;
	int aliasCount = 0;
};

namespace quest
{
	extern std::unordered_set<RE::FormID> excludeQuestFormIds;
	void refreshQuestInfo(Item2Info& item);

}
