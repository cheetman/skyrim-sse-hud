#pragma once
#include <PCH.h>

//int lastAmmoId = 0;
RE::TESAmmo* lastAmmo = nullptr;

class EquipEvent : public RE::BSTEventSink<RE::TESEquipEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(RE::TESEquipEvent const* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_eventSource)
	{
		if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (menu::auto_remove_ammo) {
			const auto form = RE::TESForm::LookupByID(a_event->baseObject);
			if (form->IsWeapon()) {
				auto weapon = form->As<RE::TESObjectWEAP>();
				if (weapon->IsBow() || weapon->IsCrossbow()) {
					// 卸载时
					if (!a_event->equipped) {
						RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
						if (player) {
							auto ammo = player->GetCurrentAmmo();
							if (ammo) {
								auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
								actorEquipManager->UnequipObject(player, ammo);
								//lastAmmoId = ammo->GetFormID();
								if (menu::remember_last_ammo) {
									lastAmmo = ammo;
								}
							}
						}
					} else {
						if (menu::remember_last_ammo && lastAmmo) {
							RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
							if (player) {
								const auto inv = player->GetInventory([](RE::TESBoundObject& a_object) -> bool {
									return a_object.GetFormID() == lastAmmo->GetFormID();
								});
								
								if (inv.size() == 1) {
									auto actorEquipManager = RE::ActorEquipManager::GetSingleton();
									actorEquipManager->EquipObject(player, lastAmmo);
								}
							}
						}
					}
				}
			}
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	static bool Register()
	{
		static EquipEvent singleton;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
		if (!ScriptEventSource) {
			SKSE::log::warn("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&singleton);
		SKSE::log::info("Register {}", typeid(singleton).name());

		return true;
	}

private:
	EquipEvent() = default;

	~EquipEvent() = default;

	EquipEvent(const EquipEvent&) = delete;

	EquipEvent(EquipEvent&&) = delete;

	EquipEvent& operator=(const EquipEvent&) = delete;

	EquipEvent& operator=(EquipEvent&&) = delete;
};
