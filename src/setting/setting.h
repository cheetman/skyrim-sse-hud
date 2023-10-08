#pragma once
#include <filesystem>

namespace setting
{
	extern std::filesystem::path settings_path;
	extern std::filesystem::path settings_path_gallery;
	bool save_settings();
	bool load_settings();
	bool load_settings_gallery();

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
	extern std::vector<GalleryData> galleryList;
}
