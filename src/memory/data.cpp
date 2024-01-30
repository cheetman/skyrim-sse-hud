#include "data.h"
#include <hook/BSRenderManager.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/npc.h>
#include <memory/player.h>
#include <utils/utils.h>

// 物品排除
std::unordered_set<RE::FormID> excludeFormIds;
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

	std::unordered_set<RE::FormID> autoTrackFormIds;
	std::vector<ExcludeForm> autoTrackForms;

	std::vector<PositionData> moveToPositions;
	std::vector<PositionData> tmpMoveToPositions;

	std::vector<ImageData> images;

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
				excludeEffectForms.push_back({ form->GetFormID(),
					formSpell->GetFormID(),
					form->GetName(),
					formSpell->GetName() });
			}
		}

		for (auto id : autoTrackFormIds) {
			auto form = RE::TESForm::LookupByID(id);
			if (form) {
				autoTrackForms.push_back({ form->GetFormID(), form->GetName(), "" });
			}
		}

		// 初始化任务最大值
		auto quests = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESQuest>();
		auto items = getItem2Info();
		items->quests.resize(quests.size());
		items[1].quests.resize(quests.size());

		// 初始化位置配置
		for (auto item : tmpMoveToPositions) {
			auto form = RE::TESForm::LookupByID<RE::TESObjectCELL>(item.cellId);
			if (!form) {
				continue;
			}
			item.cellPtr = form;
			if (item.worldSpaceId) {
				auto form2 = RE::TESForm::LookupByID<RE::TESWorldSpace>(item.worldSpaceId);
				if (!form2) {
					continue;
				}
				item.worldSpacePtr = form2;
			}
			moveToPositions.push_back(item);
		}

		// 拾取黑白名单

		for (auto forms : whiteListFormIds) {
			for (auto ids : whiteListFormIds[forms.first]) {
				auto form = RE::TESForm::LookupByID(ids);
				if (form) {
					whiteListForms[forms.first].push_back({ form->GetFormID(), form->GetName(), "" });
				}
			}
		}
		for (auto forms : blackListFormIds) {
			for (auto ids : blackListFormIds[forms.first]) {
				auto form = RE::TESForm::LookupByID(ids);
				if (form) {
					blackListForms[forms.first].push_back({ form->GetFormID(), form->GetName(), "" });
				}
			}
		}

		// 图片载入

		//LoadFromWICFile
		//ImGui::Image();
		//HRESULT hr = DirectX::LoadFromWICFile(L"your_image.png", DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
		//WCHAR strFile[40];
		//wsprintfW(strFile, L"E:\\test.png");

		
		for (const auto& entry : std::filesystem::directory_iterator(L"C:\\Users\\汪意超\\Downloads")) {
			if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
				// 创建纹理和纹理视图
				ID3D11ShaderResourceView* textureView;
				ID3D11Resource* texture;
				auto manager = RE::BSRenderManager::GetSingleton();
				auto device = reinterpret_cast<ID3D11Device*>(manager->forwarder);
				HRESULT hr = DirectX::CreateWICTextureFromFile(device, entry.path().c_str(), &texture, &textureView);
				if (SUCCEEDED(hr)) {

					ID3D11Texture2D* pTexture = reinterpret_cast<ID3D11Texture2D*>(texture);
					if (pTexture) {
						D3D11_TEXTURE2D_DESC desc;
						pTexture->GetDesc(&desc);
						UINT width = desc.Width;
						UINT height = desc.Height;

						ImageData item;
						item.name = entry.path().filename().string();
						item.textureView = textureView;
						item.texture = texture;
						item.width = desc.Width;
						item.height = desc.Height;
						images.push_back(item);
					}
				}


			}
		}

	
	}
}
