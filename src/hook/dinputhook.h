#pragma once

namespace dinputhook
{

	
	void DispatchInputEvent(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent** a_evns);
	inline REL::Relocation<decltype(DispatchInputEvent)> _DispatchInputEvent;

	void Install();


}
