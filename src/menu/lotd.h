#pragma once
#include <imgui/imgui.h>

namespace lotd
{
	struct LotdInfo;
	void buildItemInfoAttached(int count, std::vector<LotdInfo>& items);

	extern ImVec4 colorTableHeaderBg;
	extern ImVec4 colorTableBorderStrong;
}
