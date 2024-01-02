#include "list_quest.h"
#include <imgui/imgui.h>

namespace list
{
	bool isShowQuest = false;

	void renderQuest()
	{
		if (isShowQuest) {
			ImGui::SetNextWindowPos(ImVec2(400, 35), ImGuiCond_FirstUseEver);
			ImGui::Begin("任务列表", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);




			ImGui::End();
		}
	}
}
