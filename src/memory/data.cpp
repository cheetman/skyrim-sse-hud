#include "data.h"
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/npc.h>
#include <utils/utils.h>
#include <memory/player.h>



	// 物品排除
std::unordered_set<int> excludeFormIds;
std::vector<ExcludeForm> excludeForms;

// 商贩容器
std::unordered_set<RE::FormID> merchantContFormIds;
bool merchantContIgnore = true;

// 龙爪
std::unordered_set<RE::FormID> clawFormIds;

// 矿脉
std::unordered_set<RE::FormID> oreFormIds;

// 天气
std::vector<WeatherForm> weatherForms;
RE::FormID currentWeather = 0;


namespace data
{
	REL::Version gameVersion;

	


	void init()
	{
		for (auto id : excludeFormIds) {
			auto form = RE::TESForm::LookupByID(id);
			if (form) {
				excludeForms.push_back({ form->GetFormID(), form->GetName(), "" });
			}
		}

		for (auto id : excludeNpcFormIds) {
			auto form = RE::TESForm::LookupByID(id);
			if (form) {
				excludeNpcForms.push_back({ form->GetFormID(), form->GetName(), "" });
			}
		}

		for (auto id : autoContFormIds) {
			auto form = RE::TESForm::LookupByID(id);
			if (form) {
				autoContForms.push_back({ form->GetFormID(), form->GetName(), "" });
			}
		}

		for (auto id : excludeLocationFormIds) {
			if (id == 0) {
				excludeLocationForms.push_back({ 0, "天际", "" });
				continue;
			}
			auto form = RE::TESForm::LookupByID(id);
			if (form) {
				excludeLocationForms.push_back({ form->GetFormID(), form->GetName(), "" });
			}
		}

		// 查询商贩的箱子
		const auto handler = RE::TESDataHandler::GetSingleton();
		auto& facts = handler->GetFormArray<RE::TESFaction>();
		for (RE::TESFaction*& item : facts) {
			if (item->IsVendor()) {
				auto merchantContainer = item->vendorData.merchantContainer;
				if (merchantContainer) {
					merchantContFormIds.insert(merchantContainer->GetFormID());
				}
			}
		}

		// 龙爪
		clawFormIds.insert(0x04B56C);
		clawFormIds.insert(0x0AB7BB);
		clawFormIds.insert(0x07C260);
		clawFormIds.insert(0x05AF48);
		clawFormIds.insert(0x0ED417);
		clawFormIds.insert(0x0AB375);
		clawFormIds.insert(0x08CDFA);
		clawFormIds.insert(0x0B634C);
		clawFormIds.insert(0x0999E7);
		clawFormIds.insert(0x0663D7);
		clawFormIds.insert(0x039647);

		// 矿脉
		oreFormIds.insert(0x000613A6);
		oreFormIds.insert(0x000613A7);
		oreFormIds.insert(0x000E2BC7);

		// 天气
		for (auto form : handler->GetFormArray<RE::TESWeather>()) {
			if (!form) {
				continue;
			}
			weatherForms.push_back({ form->GetFormID(), form->GetFormEditorID(), form->data.flags.get() });
		}

		
		for (auto ids : excludeEffectFormIds) {
			auto form = RE::TESForm::LookupByID(ids.effectId);
			auto formSpell = RE::TESForm::LookupByID(ids.spellId);
			if (form && formSpell) {
				excludeEffectForms.push_back({ 
					form->GetFormID(),
					formSpell->GetFormID(),
					form->GetName(),
					formSpell->GetName()
				});
			}
		}

	


	}
}
