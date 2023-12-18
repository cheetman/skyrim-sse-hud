#pragma once
#include <imgui/imgui.h>

struct QuestInfo;

namespace menu
{

	void buildQuestItem(int count, std::vector<QuestInfo>& items);

	extern ImVec4 colorQuestTableHeaderBg;
	extern ImVec4 colorQuestTableBorderStrong;
}
