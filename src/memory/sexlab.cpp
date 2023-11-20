#include "sexlab.h"
#include <utils/utils.h>

namespace sexlab
{
	bool isShowPlayOSLArousal = false;
	float OSLArousal = 0.0f;
	float OSLLibido = 0.0f;
	float OSLArousalBaseline = 0.0f;
	bool isLoadOSLAroused = false;

	void init()
	{
		const auto handler = RE::TESDataHandler::GetSingleton();

		auto fileLotd = handler->LookupModByName("OSLAroused.esp");
		if (!fileLotd || fileLotd->compileIndex == 0xFF) {
			return;
		}

		isLoadOSLAroused = true;

	}

	void refreshInfo()
	{
		if (isLoadOSLAroused && isShowPlayOSLArousal) {
			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (player) {
				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new ScriptUtil::VMStackCallbackFunctor(
					[](float value) {
						OSLArousal = value;
					}));

				auto args = RE::MakeFunctionArguments(std::move(player));
				bool result = RE::BSScript::Internal::VirtualMachine::GetSingleton()->DispatchStaticCall("OSLArousedNative", "GetArousal", args, callback);

				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback2(new ScriptUtil::VMStackCallbackFunctor(
					[](float value) {
						OSLArousalBaseline = value;
					}));

				auto args2 = RE::MakeFunctionArguments(std::move(player));
				result = RE::BSScript::Internal::VirtualMachine::GetSingleton()->DispatchStaticCall("OSLArousedNative", "GetArousalBaseline", args2, callback2);
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
	}
}
