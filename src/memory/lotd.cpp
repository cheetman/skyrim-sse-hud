#include "lotd.h"
#include <filesystem>
#include <setting/setting.h>
#include <utils/NameUtil.h>
#include <windows.h>

namespace lotd
{
	std::vector<List> lists;
	bool isLoad = false;

	void init()
	{
		// 读取配置文件
		if (!setting::load_settings_lotd()) {
			return;
		}

		//初始化艺术馆
		const auto handler = RE::TESDataHandler::GetSingleton();

		auto file = handler->LookupModByName("LegacyoftheDragonborn.esm");
		if (!file || file->compileIndex == 0xFF) {
			return;
		}
		isLoad = true;

		// 获取物品列表
		for (auto& listItem : setting::lotdItemLists) {
			List list(listItem.listEditorId, listItem.roomName);
			RE::BGSListForm* listform = RE::TESForm::LookupByEditorID<RE::BGSListForm>(listItem.listEditorId);
			if (listform) {
				for (auto form : listform->forms) {
					if (form->Is(RE::FormType::Activator)) {
						list.sizeACTI++;
					} else if (form->Is(RE::FormType::FormList)) {
						list.sizeFLST++;
					} else {
						list.size++;
						list.forms.push_back({ form->formID, form->formType.get(), form->GetName(), GetFormTypeName(form->formType.underlying()), listItem.roomName });
					}
				}
			}
			lists.push_back(list);
		}
	}
}
