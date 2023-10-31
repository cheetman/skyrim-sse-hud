#pragma once
#include <filesystem>

namespace setting
{
	struct LotdItemList
	{
		std::string listEditorId;
		std::string roomName;
		std::string listFormId;
		std::string modName;
	};
	struct GalleryData
	{
		std::string filename;
		std::string name;
		std::vector<int> formids;
	};

	struct GalleryDataGen
	{
		std::string filename;
		std::string name;
		std::vector<std::string> formids;
	};

	extern std::filesystem::path settings_path;
	extern std::filesystem::path settings_path_gallery;
	extern std::filesystem::path settings_path_lotd_item_list;
	extern std::vector<LotdItemList> lotdItemLists;
	extern std::vector<std::uint32_t> displayConts;
	//extern std::vector<GalleryData> galleryList;
	bool save_settings();
	bool load_settings();
	bool load_settings_lotd();
	 

	


}
