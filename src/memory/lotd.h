#pragma once

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

	extern bool isLoad;
	void init();

	extern std::vector<List> lists;

}
