#pragma once
#include <unordered_set>
#include <utils/WICTextureLoader/WICTextureLoader11.h>

struct ExcludeForm
{
	RE::FormID formId = 0;
	std::string name = "";
	std::string formTypeStr = "";

	bool operator<(const ExcludeForm& other) const
	{
		return formId < other.formId;
	}
};

struct WeatherForm
{
	RE::FormID formId = 0;
	std::string editorId = "";
	RE::TESWeather::WeatherDataFlag flag = RE::TESWeather::WeatherDataFlag::kNone;

	bool operator<(const WeatherForm& other) const
	{
		return formId < other.formId;
	}
};

/// <summary>
/// 排除物品
/// </summary>
extern std::unordered_set<RE::FormID> excludeFormIds;
extern std::vector<ExcludeForm> excludeForms;

/// <summary>
/// 商贩物品
/// </summary>
extern std::unordered_set<RE::FormID> merchantContFormIds;
extern bool merchantContIgnore;

extern std::unordered_set<RE::FormID> clawFormIds;

extern std::unordered_set<RE::FormID> oreFormIds;

extern std::vector<WeatherForm> weatherForms;
extern RE::FormID currentWeather;


namespace data
{
	void init();
	extern REL::Version gameVersion;

	extern std::unordered_set<RE::FormID> autoTrackFormIds;
	extern std::vector<ExcludeForm> autoTrackForms;


	
	struct PositionData
	{
		std::string name;
		RE::TESObjectCELL* cellPtr = nullptr;
		RE::FormID cellId = 0;
		RE::TESWorldSpace* worldSpacePtr = nullptr;
		RE::FormID worldSpaceId = 0;
		RE::NiPoint3 position;
		RE::NiPoint3 angle;
	};

	
	struct ImageData
	{
		std::string name;
		ID3D11ShaderResourceView* textureView;
		ID3D11Resource* texture;
		float width;
		float height;
	};

	extern std::vector<PositionData> moveToPositions;
	extern std::vector<PositionData> tmpMoveToPositions;
	extern std::vector<ImageData> images;
}
