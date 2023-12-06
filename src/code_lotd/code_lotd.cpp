#include <event/BSTMenuEvent.h>
#include <fonts/IconsMaterialDesignIcons.h>
#include <hook/dinputhook.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/stat.h>
#include <menu/menu.h>
#include <menu/menu_track.h>
#include <menu/theme.h>
#include <setting/i18n.h>
#include <setting/setting.h>

namespace lotdcode
{
	void __cdecl timerRefresh(void*)
	{
		while (true) {
			Sleep(2000);

			if (!startflag) {
				Sleep(1000);
				continue;
			}

			if (isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu) {
				Sleep(1000);
				continue;
			}

			if (activeItems) {
				continue;
			}

			// 暂时挂在这里
			if (lotd::isLoad) {
				lotd::refreshCount();
				lotd::refreshDisplayCount();
				lotd::refreshAutoTrackItem();
			}
		}
	}

	void __fastcall render()
	{
		//第一次加载游戏后
		if (!startflag || isGameLoading || isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu) {
			return;
		}

		// 追踪
		menu::renderTrack();

		ImGuiWindowFlags window_flags = 0;
		if (menu::no_titlebar)
			window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (menu::no_resize)
			window_flags |= ImGuiWindowFlags_NoResize;
		if (menu::no_collapse)
			window_flags |= ImGuiWindowFlags_NoCollapse;
		if (menu::no_background)
			window_flags |= ImGuiWindowFlags_NoBackground;
		//if (auto_resize)
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		if (lotd::showlocationItemCount || stats::showlocationExCount) {
			ImGui::Begin("附近艺术馆藏品数量", nullptr, window_flags);

			if (lotd::showlocationItemCount) {
				ImGui::Text(ICON_MDI_TREASURE_CHEST_OUTLINE " 藏品数：");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (lotd::locationItemCount > 0) {
					menu::myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", lotd::locationItemCount);
				} else {
					menu::myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			if (stats::showlocationExCount) {
				ImGui::Text(ICON_MDI_SPADE " 考古点：");
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				if (stats::locationExCount > 0) {
					menu::myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%d", stats::locationExCount);
					if (activeItems) {
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
						if (ImGui::SmallButton(ICON_MDI_MAP_MARKER_RADIUS "##ex")) {
							for (auto item : stats::locationExIds) {
								TrackItem trackItem;
								trackItem.name = item->GetDisplayFullName();
								trackPtrs2.insert(std::make_pair(item, trackItem));
								//menu::isTrack = true;
							}
						}
					}
				} else {
					menu::myTextColored(ImVec4(1, 0, 0.0f, 1.0f), "0");
				}
			}

			ImGui::End();
		}

		if (lotd::showDisplayItemCount) {
			ImGui::Begin("艺术馆藏品数量", nullptr, window_flags);

			ImGui::Text(ICON_MDI_TREASURE_CHEST_OUTLINE " 已收集藏品数：");
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			menu::myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), "%0.0f", lotd::displayCount);

			ImGui::End();
		}

		if (active) {
			static int selected = 0;
			ImGui::SetNextWindowPos(ImVec2(110, 35), ImGuiCond_FirstUseEver);

			ImGui::Begin("艺术馆藏品追踪", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

			{
				ImGui::BeginGroup();
				ImVec2 size(ImGui::GetFontSize() * 6, 0);
				if (ImGui::Selectable(I18Ni(ICON_MDI_TOOLS, "common.setting.tab-common"), selected == 0, 0, size)) {
					selected = 0;
				}
				if (ImGui::Selectable(I18Ni(ICON_MDI_GREENHOUSE, "common.setting.tab-lotd"), selected == 1, 0, size)) {
					selected = 1;
				}

				if (ImGui::BeginPopupModal("关于", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
					ImGui::Text("版本：v1.0.0");
					ImGui::Text("作者：Cheatman、火锅");

					if (ImGui::Button(I18Nc("common.setting.btn-ok"), ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::Selectable(I18Ni(ICON_MDI_INFORMATION_OUTLINE, "common.setting.tab-about"), false, 0, size)) {
					ImGui::OpenPopup("关于");
				}

				ImGui::EndGroup();
			}

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				if (selected == 0) {
					{
						ImGui::Text(I18Ni(ICON_MDI_COG, "common.setting.label-normal"));
						ImGui::Indent();

						ImGui::AlignTextToFramePadding();
						if (ImGui::BeginPopupContextItem("PopupFonts")) {
							for (auto& item : menu::font_items) {
								if (ImGui::Selectable(item.second.c_str())) {
									ImGuiIO& io = ImGui::GetIO();
									menu::imgui_font_index = item.first;
									io.FontDefault = menu::fonts[menu::imgui_font_index];
								}
							}
							ImGui::EndPopup();
						}
						ImGui::Text(I18Nc("common.setting.label-font"));
						ImGui::SameLine();
						if (ImGui::Button(menu::font_items.at(menu::imgui_font_index).c_str(), ImVec2(ImGui::GetFontSize() * 6, 0))) {
							ImGui::OpenPopup("PopupFonts");
						}

						ImGui::SameLine();
						ImGui::Text(I18Nc("common.setting.label-fontScale"));
						ImGui::SameLine();
						ImGui::PushItemWidth(ImGui::GetFontSize() * 4);
						ImGui::DragFloat("##UI缩放", &ImGui::GetIO().FontGlobalScale, 0.005f, 0.5f, 1.8f, "%.2f", ImGuiSliderFlags_NoInput);
						ImGui::PopItemWidth();
						ImGui::Unindent();
					}

					ImGui::Spacing();
					{
						ImGui::Text(I18Ni(ICON_MDI_TABLE_OF_CONTENTS, "common.setting.label-autoMarker"));
						ImGui::Indent();

						ImGui::Checkbox(I18Nc("finder.setting.checkbox-autoMarkLotdItems"), &lotd::isAutoTrackLotdItems);
						if (lotd::isAutoTrackLotdItems) {
							ImGui::Indent();

							{
								ImGui::BeginGroup();

								ImGui::Checkbox(I18Nc("finder.setting.checkbox-markerNameTag"), &menu::show_item_window_track_icon_name);
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-markerHighLight"), &menu::show_item_window_track_highlight);

								ImGui::ColorEdit4(I18Nc("finder.setting.color-markerTagColorLotd"), &menu::ColorTrackLotd.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

								ImGui::EndGroup();
							}

							ImGui::SameLine();
							{
								ImGui::BeginGroup();

								if (ImGui::Checkbox(I18Nc("finder.setting.checkbox-privateItemHidden"), &lotd::isAutoTrackLotdItemsCrimeIgnore)) {
									lotd::isCrimeIgnore = lotd::isAutoTrackLotdItemsCrimeIgnore;
								}
								ImGui::Checkbox(I18Nc("finder.setting.checkbox-excludeArmoryItems"), &lotd::isArmoryIgnore);

								ImGui::EndGroup();
							}

							ImGui::AlignTextToFramePadding();
							ImGui::Text(I18Nc("finder.setting.slider-markerZoom"));
							ImGui::SameLine();
							ImGui::PushItemWidth(ImGui::GetFontSize() * 5);
							ImGui::SliderFloat("##markerZoom", &show_item_window_track_icon_scale, 0, 10, "+%d");
							ImGui::PopItemWidth();

							ImGui::AlignTextToFramePadding();
							if (ImGui::BeginPopupContextItem("PopupDisplayType")) {
								for (auto& item : menu::displayType) {
									if (ImGui::Selectable(item.second.c_str())) {
										menu::show_item_window_track_displayType = item.first;
									}
								}
								ImGui::EndPopup();
							}
							ImGui::Text(I18Nc("finder.setting.btn-markerDisplayType"));
							ImGui::SameLine();
							if (ImGui::Button(menu::displayType.at(menu::show_item_window_track_displayType).c_str(), ImVec2(ImGui::GetFontSize() * 5, 0))) {
								ImGui::OpenPopup("PopupDisplayType");
							}
							ImGui::Unindent();
						}
						ImGui::Unindent();
					}

					ImGui::Spacing();

					{
						ImGui::Text(I18Ni(ICON_MDI_MAP_SEARCH_OUTLINE, "common.setting.label-itemStatistics"));
						ImGui::Indent();

						ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyLotdItemsQuantity"), &lotd::showlocationItemCount);
						ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayNearbyLotdExcavationQuantity"), &stats::showlocationExCount);
						ImGui::Checkbox(I18Nc("finder.setting.checkbox-displayLotdItemsQuantity"), &lotd::showDisplayItemCount);
						ImGui::Unindent();
					}

					ImGui::Spacing();

					{
						ImGui::Text(I18Ni(ICON_MDI_KEYBOARD_VARIANT, "common.setting.label-hotkey"));
						ImGui::Indent();

						ImGui::AlignTextToFramePadding();
						ImGui::Text(I18Nc("common.setting.label-hotkeySetting"));
						ImGui::SameLine();
						if (ImGui::Button(dinputhook::getKeyName(menu::hotkeySetting, menu::hotkeySettingModifier, menu::isWaitHotkeySetting).c_str(), ImVec2(ImGui::GetFontSize() * 7, 0))) {
							menu::isWaitHotkeySetting = true;
						}

						ImGui::AlignTextToFramePadding();
						ImGui::Text(I18Nc("common.setting.label-hotkeyFinderTrack"));
						ImGui::SameLine();
						if (ImGui::Button(dinputhook::getKeyName(menu::hotkeyTrack, menu::hotkeyTrackModifier, menu::isWaitTrack).c_str(), ImVec2(ImGui::GetFontSize() * 7, 0))) {
							menu::isWaitTrack = true;
						}

						ImGui::Unindent();
					}
				} else if (selected == 1) {
					ImGui::Text(I18Nc("lotd.setting.label-totalItemsIdentified"), lotd::formIds.size());

					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();

					ImGui::Text(I18Nc("lotd.setting.label-ModStatistics"));
					if (ImGui::BeginTable("splitLotdMod", 8)) {
						for (const auto& pair : lotd::formIdsM) {
							ImGui::TableNextColumn();
							ImGui::Text("%s", setting::getLotdItemListModName(pair.first).c_str());
							ImGui::TableNextColumn();

							if (pair.second.size() == 0 && lotd::loadCountsM[pair.first] == 0) {
								menu::myTextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), I18Nc("lotd.setting.label-notInstalled"));
							} else {
								menu::myTextColored(ImVec4(0.0f, 1, 0.0f, 1.0f), I18Nc("lotd.setting.label-installed"), pair.second.size());
							}
						}
						ImGui::EndTable();
					}
				}
			}

			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::BeginPopupModal(I18Nc("common.setting.popup-info"), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text(I18Nc("common.setting.label-configSaved"));
				if (ImGui::Button(I18Nc("common.setting.btn-ok"), ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::Button(I18Ni(ICON_MDI_CONTENT_SAVE, "common.setting.btn-saveConfig"))) {
				setting::save_settings();
				ImGui::OpenPopup(I18Nc("common.setting.popup-info"));
			}

			ImGui::EndGroup();
			ImGui::End();
		}
	}

}
