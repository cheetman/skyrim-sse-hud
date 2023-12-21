#include <event/BSTCrosshairRefEvent.h>
#include <event/BSTMenuEvent.h>
#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <memory/autotake.h>
#include <memory/lotd.h>
#include <memory/memory.h>
#include <memory/player.h>
#include <memory/stat.h>
#include <menu/lotd.h>
#include <menu/menu.h>
#include <menu/menu_npc.h>
#include <menu/menu_stat.h>
#include <menu/theme.h>
#include <nlohmann/json.hpp>
#include <setting/setting.h>
#include <utils/GeneralUtil.h>
#include <utils/NameUtil.h>

std::unordered_map<RE::TESObjectREFR*, TrackItem> trackPtrs2;
std::unordered_map<RE::Actor*, TrackItem> trackActorPtrs2;
std::mutex mtxTrack;

namespace menu
{
	int trackX1 = 0;
	int trackY1 = 0;
	int trackX2 = 0;
	int trackY2 = 0;
	bool show_item_window_track_icon_name = true;
	bool show_item_window_track_highlight = true;
	bool show_item_window_track_auto_tag = false;
	bool show_item_window_track_auto_tag_OutOfRangeIcon = false;
	float show_item_window_track_icon_scale = 0;
	float show_item_window_track_icon_scale_max = 1.5f;
	float show_item_window_track_icon_scale_min = 0.5f;
	ImVec4 colorTrack(0.0f, 1.0f, 0.1f, 0.646f);
	bool isTrack = false;

	int show_item_window_track_displayType = 0;
	ImVec4 ColorTrackLotd(1.0f, 0.843f, 0.0f, 0.95f);

	RE::NiPointer<RE::NiCamera> getCamera()
	{
		RE::PlayerCamera* camera = RE::PlayerCamera::GetSingleton();
		if (camera->cameraRoot->children.size() == 0)
			return nullptr;
		for (auto& entry : camera->cameraRoot->children) {
			auto asCamera = skyrim_cast<RE::NiCamera*>(entry.get());
			if (asCamera)
				return RE::NiPointer<RE::NiCamera>(asCamera);
		}
		return nullptr;
	};

	float calculateScale(float distance)
	{
		if (distance >= 20) {
			return show_item_window_track_icon_scale_min;
		}
		//float minValue = 0.1;
		float result = show_item_window_track_icon_scale_max - (distance / 20) * (show_item_window_track_icon_scale_max - show_item_window_track_icon_scale_min);
		return result;
	}

	void renderTrack()
	{
		// 追踪
		if (show_item_window_track_displayType != 0 && !isTrack) {
			return;
		}

		std::lock_guard<std::mutex> lock(mtxTrack);
		if (trackPtrs2.size() > 0 || trackActorPtrs2.size() > 0) {
			if (!(isOpenCursorMenu || isMainMenu || isLoadWaitSpinner || isFaderMenu)) {
				RE::NiPointer<RE::NiCamera> camera = getCamera();
				RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
				if (camera) {
					RE::NiCamera* ca = camera.get();
					if (ca) {
						for (const auto& item : trackPtrs2) {
							auto ptr = item.first;
							float x1;
							float y1;
							float z1;
							if (ptr->IsDeleted() || ptr->IsDestroyed() || ptr->IsIgnored() || !ptr->Is3DLoaded()) {
								trackPtrs2.erase(ptr);
								break;
							}

							if (item.second.isAuto && item.second.isTree) {
								if (ptr->formFlags & RE::TESObjectREFR::RecordFlags::kHarvested) {
									trackPtrs2.erase(ptr);
									break;
								}
							}

							auto p = ptr->GetPosition();
							bool result = ca->WorldPtToScreenPt3(ca->worldToCam, ca->port, p, x1, y1, z1, 1e-5f);
							if (!result) {
								trackPtrs2.erase(ptr);
								break;
							}
							if (x1 > 0 && y1 > 0 && z1 > 0) {
								trackX1 = x1 * screenWidth;
								trackY1 = screenHeight - y1 * screenHeight;
								if (trackX1 > 0 && trackX1 < screenWidth && trackY1 > 0 && trackY1 < screenHeight) {
									auto distance = ValueUtil::calculateDistance(ptr->GetPosition(), player->GetPosition()) / 100.0f;
									if (distance > show_items_window_auto_dis_local && distance > show_items_window_auto_dis_skyrim) {
										trackPtrs2.erase(ptr);
										break;
									}

									ImGui::SetNextWindowPos(ImVec2(trackX1 - 40, trackY1));

									char buf[32];
									snprintf(buf, 32, "%p", ptr);
									ImGui::Begin(buf, nullptr,
										ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

									if (show_item_window_track_auto_tag) {
										ImGui::SetWindowFontScale(calculateScale(distance));

									} else {
										ImGui::SetWindowFontScale(menu::font_scale + show_item_window_track_icon_scale);
									}

									if (item.second.isLotd || item.second.isAuto) {
										if (show_item_window_track_icon_name) {
											if (show_item_window_track_auto_tag_OutOfRangeIcon && distance > 30.0f) {
												myTextColored(ColorTrackLotd, ICON_MDI_MAP_MARKER_RADIUS " %0.0fm", distance);
											} else {
												myTextColored(ColorTrackLotd, "%s %0.1fm", item.second.name.c_str(), distance);
											}
										} else {
											myTextColored(ColorTrackLotd, ICON_MDI_MAP_MARKER_RADIUS " %0.1fm", distance);
										}
									} else {
										if (show_item_window_track_icon_name) {
											ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS "%s %0.1fm", item.second.name.c_str(), distance);
										} else {
											ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS " %0.1fm", distance);
										}
									}
									ImGui::SetWindowFontScale(menu::font_scale);

									ImGui::End();
								}
							}
						}
						for (const auto& item : trackActorPtrs2) {
							auto ptr = item.first;
							float x1;
							float y1;
							float z1;
							if (ptr->IsDeleted() || ptr->IsIgnored() || !ptr->Is3DLoaded()) {
								trackActorPtrs2.erase(ptr);
								break;
							}


							auto p = ptr->GetPosition();
							bool result = ca->WorldPtToScreenPt3(ca->worldToCam, ca->port, p, x1, y1, z1, 1e-5f);
							if (!result) {
								trackActorPtrs2.erase(ptr);
								break;
							}
							if (x1 > 0 && y1 > 0 && z1 > 0) {
								trackX1 = x1 * screenWidth;
								trackY1 = screenHeight - y1 * screenHeight;
								if (trackX1 > 0 && trackX1 < screenWidth && trackY1 > 0 && trackY1 < screenHeight) {
									auto distance = ValueUtil::calculateDistance(ptr->GetPosition(), player->GetPosition()) / 100.0f;
									if (distance > show_items_window_auto_dis_local && distance > show_items_window_auto_dis_skyrim) {
										trackActorPtrs2.erase(ptr);
										break;
									}

									ImGui::SetNextWindowPos(ImVec2(trackX1 - 40, trackY1));

									char buf[32];
									snprintf(buf, 32, "%p", ptr);
									ImGui::Begin(buf, nullptr,
										ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

									if (show_item_window_track_auto_tag) {
										ImGui::SetWindowFontScale(calculateScale(distance));

									} else {
										ImGui::SetWindowFontScale(menu::font_scale + show_item_window_track_icon_scale);
									}

									if (show_item_window_track_icon_name) {
										ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS " %s %0.1fm", item.second.name.c_str(), distance);
									} else {
										ImGui::Text(ICON_MDI_MAP_MARKER_RADIUS " %0.1fm", distance);
									}

									ImGui::SetWindowFontScale(menu::font_scale);

									ImGui::End();
								}
							}
						}
					}
				}
			}
		}
	}

	void tintTrack(RE::TESObjectREFR* reff)
	{
		/*if (show_item_window_track_highlight) {
			auto obj3D = item.ptr->Get3D();
			if (obj3D) {
				RE::NiColorA color{ colorTrack.x, colorTrack.y, colorTrack.z, colorTrack.w };
				auto task = SKSE::GetTaskInterface();
				task->AddTask([obj3D, color]() {
					obj3D->TintScenegraph(color);
				});
			}
		}*/

		if (show_item_window_track_highlight) {
			if (reff) {
				RE::TESEffectShader* shader = RE::TESForm::LookupByID<RE::TESEffectShader>(0x10F56B);
				if (shader) {
					auto ambientSound = shader->data.ambientSound;
					shader->data.ambientSound = nullptr;
					ScriptUtil::EffectShaderPlay(nullptr, 0, shader, reff, 999);
					shader->data.ambientSound = ambientSound;
				}
			}
		}
	}

	void tintTrackClose(RE::TESObjectREFR* reff)
	{
		if (reff) {
			RE::TESEffectShader* shader = RE::TESForm::LookupByID<RE::TESEffectShader>(0x10F56B);
			if (shader) {
				ScriptUtil::EffectShaderStop(nullptr, 0, shader, reff);
			}
		}
	}

}
