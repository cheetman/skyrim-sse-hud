#pragma once
#include <PCH.h>

static bool isOpenCursorMenu = false;

class MenuOpenCloseEvent : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	virtual RE::BSEventNotifyControl ProcessEvent(RE::MenuOpenCloseEvent const* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource) {
			SKSE::log::info("Event Source Not Found!");
			return RE::BSEventNotifyControl::kContinue;
		}

		/*if (a_event->opening && a_event->menuName == RE::InterfaceStrings::GetSingleton()->console) {
		}*/

		if (a_event->menuName == RE::InterfaceStrings::GetSingleton()->cursorMenu) {
			isOpenCursorMenu = a_event->opening;
		}

		//SKSE::log::info("{} {} !", a_event->menuName, a_event->opening ? "Open" : "Close");

		return RE::BSEventNotifyControl::kContinue;
	}

	static bool Register()
	{
		static MenuOpenCloseEvent singleton;

		auto ui = RE::UI::GetSingleton();
		if (!ui)
			return false;

		ui->AddEventSink(&singleton);
		SKSE::log::info("Register {}", typeid(singleton).name());

		return true;
	}

private:
	MenuOpenCloseEvent() = default;

	~MenuOpenCloseEvent() = default;

	MenuOpenCloseEvent(const MenuOpenCloseEvent&) = delete;

	MenuOpenCloseEvent(MenuOpenCloseEvent&&) = delete;

	MenuOpenCloseEvent& operator=(const MenuOpenCloseEvent&) = delete;

	MenuOpenCloseEvent& operator=(MenuOpenCloseEvent&&) = delete;
};
