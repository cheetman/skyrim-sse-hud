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

	struct Lotd2Info
	{
		std::map<std::string, int> counts;
		std::map<std::string, std::vector<Form>> lists;
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
	std::vector<Form>& getItems(std::string& roomName);
	int getCount(std::string& roomName);

}
