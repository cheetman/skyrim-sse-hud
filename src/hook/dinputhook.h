#pragma once

namespace dinputhook
{

	
	void DispatchInputEvent(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent** a_evns);
	inline REL::Relocation<decltype(DispatchInputEvent)> _DispatchInputEvent;
	void Install();

	extern std::unordered_map<uint32_t, std::string> keyNameMap;
	std::string __fastcall getKeyName(int scancode, int keyModifier, bool isWait);
	bool setKeyAndModifier(int& hotkey, int& keyModifier, int scancode);
	bool __fastcall checkModifier(int keyModifier);
}
