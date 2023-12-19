#pragma once
#include <unordered_set>

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
extern std::unordered_set<int> excludeFormIds;
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
}
