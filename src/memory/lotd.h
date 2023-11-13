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
		std::uint32_t size2 = 0;
		std::uint32_t sizeACTI2 = 0;
		std::uint32_t sizeFLST2 = 0;
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
		RE::FormID baseFormId = 0;
		RE::TESBoundObject* baseObjPtr = nullptr;
		std::string formTypeStr = "";
		std::string filename = "";

		bool isCrime = false;
		RE::TESObjectREFR* ptr = nullptr;
		int count = 0;
		std::string contname;
		RE::TESObjectREFR* contptr = nullptr;
		RE::FormID contformId;
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

	// 给前面的用
	struct Lotd2InfoAttached
	{
		int count = 0;
		std::vector<LotdInfo> list;
	};

	extern bool isLoad;
	extern bool isShow;
	extern bool isShowAttached;
	extern bool isInvIgnore;
	extern bool isCrimeIgnore;
	extern bool isArmoryIgnore;
	
	extern std::uint8_t lotdCompileIndex;
	extern std::uint8_t lotdSmallFileCompileIndex;
	extern int nowItemIndex;

	// 集合 按照他的FLST区分
	extern std::vector<List> lists;
	// 集合 按照房间区分
	extern std::map<std::string, std::vector<Form>> listsR;
	extern std::map<std::string, std::unordered_set<RE::FormID>> formIdsR;
	extern std::unordered_set<RE::FormID> formIds;
	// 集合 按照mod区分
	extern std::map<std::string, std::unordered_set<RE::FormID>> formIdsM;
	extern std::map<std::string, int> loadCountsM;

	// 多语言
	extern std::map<std::string, std::string> roomNames;

	// 已展示
	extern std::map<std::uint32_t, std::unordered_set<RE::FormID>> displayIdsC;
	extern std::map<std::string, std::unordered_set<RE::FormID>> displayIdsR;
	extern std::unordered_set<RE::FormID> displayIds;

	// 身上物品
	extern std::unordered_set<RE::FormID> playerInvIds;



	void init();
	void refreshItemInfo();
	std::vector<LotdInfo>& getItems(std::string roomName);
	int getCount(std::string roomName);

	void refreshItemInfoAttached();
	std::vector<LotdInfo>& getItemsAttached();
	int getCountAttached();


	// 艺术馆地点
	extern std::unordered_set<RE::FormID> locationIds;

	// 挖掘点
	extern std::unordered_set<RE::FormID> excavationIds;

	// 附近藏品数量
	extern bool showlocationItemCount;
	extern std::unordered_set<RE::FormID> locationItemIds;
	extern int locationItemCount;

	/// <summary>
	/// 统计区域内的数量
	/// </summary>
	void refreshCount();

	/// <summary>
	/// 刷新身上物品
	/// </summary>
	void refreshInvItemsCache();
	/// <summary>
	/// 刷新陈列品物品
	/// </summary>
	void refreshDisplayItemsCache();

	// 身上物品
	extern std::unordered_set<RE::FormID> playerInvIds;

	void trackAllItem(int count, std::vector<LotdInfo>& items);
}

