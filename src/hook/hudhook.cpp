#include "hudhook.h"

constexpr REL::ID EnemyUpdateHookBase(static_cast<std::uint64_t>(50776));
uintptr_t EnemyUpdateHook = (EnemyUpdateHookBase.address() + 0x44);
EnemyInfo enemyInfo[50];

auto IsSentient(RE::Actor* actor) -> uint32_t
{
	using func_t = decltype(&IsSentient);
	REL::Relocation<func_t> func{ REL::ID(36889) };
	return func(actor);
}

EnemyInfo* getEnemyData() {

	return &enemyInfo[0];
}

bool Hook_EnemyHealthLookupReferenceByHandle_impl(const RE::RefHandle& refHandle, RE::NiPointer<RE::TESObjectREFR>& refrOut)
{
	auto result = RE::LookupReferenceByHandle(refHandle, refrOut);
	if (!result) {
		return result;
	}
	RE::TESObjectREFR* reference = refrOut.get();
	if (!reference) {
		return result;
	}

	if (reference) {
		/*	logger::debug(reference->GetDisplayFullName());
		logger::debug(reference->GetName());*/
		logger::debug(reference->GetFormID());

		if (reference->GetBaseObject()->formType == RE::FormType::NPC ||
			reference->GetBaseObject()->formType == RE::FormType::ActorCharacter) {
			auto pNPC = DYNAMIC_CAST(reference, RE::TESObjectREFR, RE::Actor);
			if (pNPC) {
				auto formId = reference->GetFormID();
				int isSentient = IsSentient(pNPC);
				time_t now1 = time(NULL);
				for (int i = 0; i < 50; i++) {
					EnemyInfo& item = enemyInfo[i];
					if (item.formId == formId) {
						item.isSentient = isSentient;
						item.level = pNPC->GetLevel();
						item.kHealthBase = pNPC->GetPermanentActorValue(RE::ActorValue::kHealth);
						item.kHealth = pNPC->GetActorValue(RE::ActorValue::kHealth);
						item.kMagickaBase = pNPC->GetPermanentActorValue(RE::ActorValue::kMagicka);
						item.kMagicka = pNPC->GetActorValue(RE::ActorValue::kMagicka);
						item.kStaminaBase = pNPC->GetPermanentActorValue(RE::ActorValue::kStamina);
						item.kStamina = pNPC->GetActorValue(RE::ActorValue::kStamina);
						item.updateTime = now1;
						return true;
					}
				}

				for (int i = 0; i < 50; i++) {
					EnemyInfo& item = enemyInfo[i];
					if (item.kHealth <= 0) {
						item.formId = formId;
						item.isSentient = isSentient;
						item.name = pNPC->GetDisplayFullName();
						item.level = pNPC->GetLevel();
						item.kHealthBase = pNPC->GetPermanentActorValue(RE::ActorValue::kHealth);
						item.kHealth = pNPC->GetActorValue(RE::ActorValue::kHealth);
						item.kMagickaBase = pNPC->GetPermanentActorValue(RE::ActorValue::kMagicka);
						item.kMagicka = pNPC->GetActorValue(RE::ActorValue::kMagicka);
						item.kStaminaBase = pNPC->GetPermanentActorValue(RE::ActorValue::kStamina);
						item.kStamina = pNPC->GetActorValue(RE::ActorValue::kStamina);
						item.updateTime = now1;
						return true;
					}
				}

			}
		}
	}
}

void hookInstall()
{
	auto& trampoline = SKSE::GetTrampoline();
	//trampoline.write_call<5>(
	//	RE::unrestricted_cast<std::uintptr_t>(EnemyUpdateHook),
	//	RE::unrestricted_cast<std::uintptr_t>(Hook_EnemyHealthLookupReferenceByHandle_impl));
	trampoline.write_call<5>(EnemyUpdateHook, Hook_EnemyHealthLookupReferenceByHandle_impl);
}
