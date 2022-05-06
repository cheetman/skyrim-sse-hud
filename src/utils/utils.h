#pragma once
#include <PCH.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <stdlib.h>

namespace StringUtil
{
	static std::string cutString(std::string a_value)
	{
		std::string text;
		if (a_value.find(".") != std::string::npos) {
			auto s = a_value.substr(a_value.find(".") + 1, 2);
			if (count(s.begin(), s.end(), '0') == 2) {
				text = a_value.substr(0, a_value.find("."));
			} else {
				text = a_value.substr(0, a_value.find(".") + 3);
			}
		} else {
			text = a_value;
		}
		return text;
	}

	static std::string getStringValueFromFloat(float a_x) { return cutString(fmt::format(FMT_STRING("{:.2f}"), a_x)); }

	static float calculateValue(float a_rm, float a_r) { return (a_rm * a_r) / 100; }

	template <typename T>
	static std::string intToHex(T a_i)
	{
		std::stringstream stream;
		stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << a_i;

		return stream.str();
	}

	template <typename T>
	static std::string delimitTwoValues(float a_v1, float a_v2, T v_d)
	{
		return fmt::format(FMT_STRING("{}{}{}"), getStringValueFromFloat(a_v1), v_d, getStringValueFromFloat(a_v2));
	}

	
	static std::string Utf8ToGbk(const char* src_str)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
		wchar_t* wszGBK = new wchar_t[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char* szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
		std::string strTemp(szGBK);
		if (wszGBK)
			delete[] wszGBK;
		if (szGBK)
			delete[] szGBK;
		return strTemp;
	}

}


namespace ValueUtil
{
	static float calculateArmorDamageRes(float a_armor_rating, int32_t a_pieces_worn)
	{
		return (float(a_armor_rating * 0.12) + float(3 * a_pieces_worn));
	}

	static std::string getValueWithCapIfNeeded(float a_res, float a_cap, std::string a_ending)
	{
		auto value = StringUtil::getStringValueFromFloat(a_res);

		if (a_res > a_cap) {
			value = fmt::format(FMT_STRING("{}{} ({})"),
				StringUtil::getStringValueFromFloat(a_cap),
				a_ending,
				StringUtil::getStringValueFromFloat(a_res));
		}
		return value;
	}

	static std::string getValueWithPermAV(float a_av, float a_perm_av)
	{
		if (a_av != a_perm_av) {
			return StringUtil::delimitTwoValues(a_av, a_perm_av, "/"sv);
		}
		return StringUtil::getStringValueFromFloat(a_av);
	}
}

namespace VectorUtil
{
	static std::vector<uint16_t> getIntersect(std::vector<uint16_t>& a_vec_first, std::vector<uint16_t>& a_vec_second)
	{
		std::vector<uint16_t> intersectionVector;

		std::sort(a_vec_first.begin(), a_vec_first.end());
		std::sort(a_vec_second.begin(), a_vec_second.end());

		std::set_intersection(a_vec_first.begin(),
			a_vec_first.end(),
			a_vec_second.begin(),
			a_vec_second.end(),
			back_inserter(intersectionVector));
		return intersectionVector;
	}

	template <typename T>
	static std::string getDelimitedString(std::vector<T>& a_vec)
	{
		auto deliString =
			a_vec.empty() ?
                "" :
                std::accumulate(std::begin(a_vec), std::end(a_vec), std::string{}, [](std::string r, int p) {
					return std::move(r) + std::to_string(p) + ",";
				});

		if (!deliString.empty()) {
			deliString.pop_back();
		}
		return deliString;
	}
}

namespace QuestUtil
{
	static std::vector<uint16_t> getStagesIfComplete(RE::TESQuest* a_quest)
	{
		std::vector<uint16_t> finStages;

		//all favour quests finish at 25 execpt rift = 20
		auto hex = StringUtil::intToHex(a_quest->GetFormID());
		logger::debug("Questname ({}), formid {}, prio {}"sv, a_quest->GetName(), hex, a_quest->data.priority);

		//todo make util function if for checking stages
		logger::trace("executed Stages for quest {}"sv, hex);
		auto exec = a_quest->executedStages;
		for (auto it = exec->begin(); it != exec->end(); ++it) {
			auto i = *it;
			auto qstdata = i.data;
			logger::trace("index {}, flag {}"sv, 1);
			if (qstdata.flags.underlying() == 1) {
				finStages.push_back(qstdata.index);
			}
		}

		logger::trace("waiting Stages for quest {}"sv, hex);
		auto waiting = a_quest->waitingStages;
		for (auto it = waiting->begin(); it != waiting->end(); ++it) {
			auto i = *it;
			auto qstdata = i->data;
			logger::trace("index {}, flag {}"sv, qstdata.index, qstdata.flags.get());
			if (qstdata.flags.underlying() == 1) {
				finStages.push_back(qstdata.index);
			}
		}
		return finStages;
	}

	static bool isOneQuestStageComplete(RE::TESQuest* a_quest, std::vector<uint16_t> a_stages)
	{
		auto finStages = getStagesIfComplete(a_quest);
		auto intersect = VectorUtil::getIntersect(finStages, a_stages);
		auto hex = StringUtil::intToHex(a_quest->GetFormID());

		logger::debug("For Quest {}, got Stages (Values)/size completed ({})/{}, given ({})/{}, intersect ({})/{}"sv,
			hex,
			VectorUtil::getDelimitedString(finStages),
			finStages.size(),
			VectorUtil::getDelimitedString(a_stages),
			a_stages.size(),
			VectorUtil::getDelimitedString(intersect),
			intersect.size());

		return intersect.size() > 0 ? true : false;
	}

	static bool isQuestStageComplete(RE::TESQuest* a_quest, uint16_t a_stage)
	{
		return isOneQuestStageComplete(a_quest, std::vector{ a_stage });
	}

	static bool isQuestStageComplete(RE::FormID a_form_id, uint16_t a_stage)
	{
		auto qst = RE::TESForm::LookupByID(a_form_id)->As<RE::TESQuest>();
		return isQuestStageComplete(qst, a_stage);
	}

	static bool isOneQuestStageComplete(RE::FormID a_form_id, std::vector<uint16_t> a_stages)
	{
		auto qst = RE::TESForm::LookupByID(a_form_id)->As<RE::TESQuest>();
		return isOneQuestStageComplete(qst, a_stages);
	}

	static uint16_t getAs(int a_i) { return static_cast<uint16_t>(a_i); }
}
