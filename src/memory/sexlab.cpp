#include "sexlab.h"
#include <utils/utils.h>

namespace sexlab
{
	float arousal = 0.0f;
	float arousalBase = 0.0f;
	/*float OSLArousal = 0.0f;
	float OSLLibido = 0.0f;
	float OSLArousalBaseline = 0.0f;
	float SexlabArousal = 0.0f;
	float SexlabArousalBase = 100.0f;*/

	bool isLoadOSLAroused = false;
	bool isLoadSexlabAroused = false;

	bool isShowPlayOSLArousal = false;
	bool isShowPlaySexlabAroused = false;

	const RE::BSFixedString OSLArousalPapyrusClass = "OSLArousedNative";
	const RE::BSFixedString SexlabArousalPapyrusClass = "slaInternalModules";
	using InternalVM = RE::BSScript::Internal::VirtualMachine;

	using ObjectPtr = RE::BSTSmartPointer<RE::BSScript::Object>;
	using InternalVM = RE::BSScript::Internal::VirtualMachine;

	inline RE::VMHandle GetHandle(RE::TESForm* a_form)
	{
		auto vm = InternalVM::GetSingleton();
		auto policy = vm->GetObjectHandlePolicy();
		return policy->GetHandleForObject(a_form->GetFormType(), a_form);
	}

	inline ObjectPtr GetObject(RE::TESForm* a_form, const char* a_class, bool a_create = false)
	{
		auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		auto handle = GetHandle(a_form);

		ObjectPtr object = nullptr;
		bool found = vm->FindBoundObject(handle, a_class, object);
		if (!found && a_create) {
			vm->CreateObject2(a_class, object);
			vm->BindObject(object, handle, false);
		}

		return object;
	}

	ObjectPtr qusetObjPtr = 0;
	void init()
	{
		const auto handler = RE::TESDataHandler::GetSingleton();

		auto fileLotd = handler->LookupModByName("OSLAroused.esp");
		if (!fileLotd || fileLotd->compileIndex == 0xFF) {
			//return;
		} else {
			isLoadOSLAroused = true;
		}

		// 判断sexlab Aroused

		auto quest = RE::TESForm::LookupByEditorID<RE::TESQuest>("sla_Framework");
		if (quest) {
			isLoadSexlabAroused = true;

			qusetObjPtr = GetObject(quest, "slaFrameworkScr", true);
		}

		
	}

	void refreshInfo()
	{
		auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (isLoadOSLAroused && isShowPlayOSLArousal) {
			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new ScriptUtil::VMStackCallbackFunctor(
					[](float value) {
						arousal = value;
					}));

				auto args = RE::MakeFunctionArguments(std::move(player));
				bool result = vm->DispatchStaticCall(OSLArousalPapyrusClass, "GetArousal", args, callback);

				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback2(new ScriptUtil::VMStackCallbackFunctor(
					[](float value) {
						arousalBase = value;
					}));

				auto args2 = RE::MakeFunctionArguments(std::move(player));
				result = vm->DispatchStaticCall(OSLArousalPapyrusClass, "GetArousalBaseline", args2, callback2);
			}
			/*RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new ScriptUtil::VMStackCallbackFunctor(
				[](float value) {
					OSLLibido = value;
				}));

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			auto args = RE::MakeFunctionArguments(std::move(player));
			bool result = RE::BSScript::Internal::VirtualMachine::GetSingleton()->DispatchStaticCall("OSLArousedNative", "GetLibido", args, callback);
		*/
		}

		if (isLoadSexlabAroused && isShowPlaySexlabAroused) {
			arousalBase = 100;
			auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			auto result = vm->TypeIsValid("slaInternalModules");
			result = vm->TypeIsValid("slaInternalModules");
			result = vm->TypeIsValid("slaInternalModules");
			result = vm->TypeIsValid("slaFrameworkScr");
			result = vm->TypeIsValid("slaFrameworkScr");
			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new ScriptUtil::VMStackCallbackFunctor(
					[](float value) {
						arousal = value;
					}));

				auto args = RE::MakeFunctionArguments(std::move(player));
				//bool result = vm->DispatchStaticCall("slaInternalModules", "GetArousal", args, callback);
				//bool result = vm->DispatchStaticCall("slaFrameworkScr", "GetActorArousal", args, callback);
				RE::BSTSmartPointer<RE::BSScript::Object> objectPtr;
				vm->FindBoundObject(0, "slaFrameworkScr", objectPtr);

				auto obj = GetObject(player, "slaFrameworkScr", true);

				for (auto& [thisClassName, classInfo] : vm->objectTypeMap) {
					if (thisClassName == "slaFrameworkScr") {
						for (uint32_t i = 0; i < classInfo->GetNumGlobalFuncs(); ++i) {
							auto& f = classInfo->GetGlobalFuncIter()[i].func;
							auto name = f.get()->GetName().data();
							int dd = 1;
						}

						for (uint32_t i = 0; i < classInfo->GetNumMemberFuncs(); ++i) {
							auto& f = classInfo->GetMemberFuncIter()[i].func;
							auto name = f.get()->GetName().data();
							int dd = 1;
						}
						break;
					}
					/*if (thisClassName == "slaUtilScr") {
						for (uint32_t i = 0; i < classInfo->GetNumGlobalFuncs(); ++i) {
							auto& f = classInfo->GetGlobalFuncIter()[i].func;
							auto name = f.get()->GetName().data();
							int dd = 1;
						}
						for (uint32_t i = 0; i < classInfo->GetNumMemberFuncs(); ++i) {
							auto& f = classInfo->GetMemberFuncIter()[i].func;
							auto name = f.get()->GetName().data();
							int dd = 1;
						}
						break;
					}*/
				}

				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback4(new ScriptUtil::VMIntStackCallbackFunctor(
					[](int value) {
						arousal = value;
					}));

					//auto args = RE::MakeFunctionArguments(std::forward<RE::BSScript::IFunctionArguments>(player));

					 //auto args = RE::FunctionArguments<RE::Actor*>(player);

				//RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback3{};

				auto obj2 = GetObject(player, "slaFrameworkScr", true);

				

				/*auto args2 = RE::MakeFunctionArguments(std::move(player), std::move(player));
				bool result = vm->DispatchMethodCall1(obj2, "GetActorArousal", args2, callback4);*/

				
				auto args6 = RE::MakeFunctionArguments(std::move(player));
				bool result6 = vm->DispatchMethodCall1(qusetObjPtr, "GetActorArousal", args6, callback4);
				int i = 0;
				/*	RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback2{};
				bool result = vm->DispatchStaticCall("slaFrameworkScr", "GetActorArousal", args, callback2);*/
			}
		}
	}
}
