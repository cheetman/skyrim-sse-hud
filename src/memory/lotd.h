#pragma once
#include <unordered_set>

namespace lotd
{
	struct Form
	{
		RE::FormID formId;
		RE::FormType formType;
		std::string name;
		std::string formTypeName;
		std::string roomName;
	};

	// 以FLST区分
	struct List
	{
		std::string name;
		std::string roomName;
		std::uint32_t size = 0;
		std::uint32_t sizeACTI = 0;
		std::uint32_t sizeFLST = 0;
		std::vector<Form> forms;

	public:
		List(std::string& name, std::string& roomName)
		{
			this->name = name;
			this->roomName = roomName;
		}
	};


	
	struct LotdInfo
	{
		RE::FormID formId;
		RE::FormType formType;
		std::string name;
		std::string formTypeName;
		std::string roomName;

		int gold = 0;
		float weight = 0;
		std::string formIdStr = "";
		RE::FormID baseFormId = 0;
		std::string baseFormIdStr = "";
		std::string formTypeStr = "";
		std::string filename = "";

		bool isCrime = false;
		RE::TESObjectREFR* ptr = nullptr;
		int count = 0;
		std::string contname;
		RE::TESObjectREFR* contptr = nullptr;
		RE::LOCK_LEVEL lockLevel;
		bool isDeleted = false;
		bool isHarvested = false;
		bool isEnchanted = false;

		float distance = 0.0f;
		int direction = 0;
		bool isAuto = false;
	};

	struct Lotd2Info
	{
		std::map<std::string, int> counts;
		std::map<std::string, std::vector<LotdInfo>> lists;
	};

	extern bool isLoad;
	extern bool isShow;
	extern int nowItemIndex;

	// 集合 按照他的FLST区分
	extern std::vector<List> lists;
	// 集合 按照房间区分
	extern std::map<std::string, std::vector<Form>> listsR;
	extern std::map<std::string, std::unordered_set<RE::FormID>> formIdsR;

	void init();
	void refreshItemInfo();
	std::vector<LotdInfo>& getItems(std::string roomName);
	int getCount(std::string roomName);

}
