#include <fonts/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <menu/menu.h>

namespace menu
{
	std::string fontFilePath = "data\\skse\\plugins\\xyght3.0-62354202.ttf";
	int imgui_style_index = 6;
	int imgui_font_index = 3;
	std::vector<ImFont*> fonts;

	void initFonts()
	{
		if (std::filesystem::exists(fontFilePath)) {
			ImGuiIO& io = ImGui::GetIO();

			float baseFontSize = 18.0f;
			float iconFontSize = baseFontSize * 2.0f / 3.0f;
			ImFont* font = io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\xyght3.0-62354202.ttf", baseFontSize, 0, io.Fonts->GetGlyphRangesChineseFull());

			ImFontConfig config;
			config.MergeMode = true;
			config.GlyphMinAdvanceX = iconFontSize;
			static const ImWchar icon_ranges[] = { 0xf000, 0xf3ff, 0 };
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\fontawesome-webfont.ttf", iconFontSize, &config, icon_ranges);
			SKSE::log::info("AddFontFromFileTTF");

			ImFontConfig config2;
			config2.MergeMode = true;
			config2.PixelSnapH = true;
			config2.GlyphMinAdvanceX = iconFontSize;
			static const ImWchar icon_ranges2[] = { static_cast<ImWchar>(ICON_MIN_MDI), static_cast<ImWchar>(ICON_MAX_MDI), static_cast<ImWchar>(0) };
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\materialdesignicons-webfont.ttf", iconFontSize, &config2, icon_ranges2);

			menu::fonts.push_back(font);

			font = io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\bt-zh-cn.ttf", baseFontSize, 0, io.Fonts->GetGlyphRangesChineseFull());
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\fontawesome-webfont.ttf", iconFontSize, &config, icon_ranges);
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\materialdesignicons-webfont.ttf", iconFontSize, &config2, icon_ranges2);
			menu::fonts.push_back(font);

			font = io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\ys-zh-cn.ttf", baseFontSize, 0, io.Fonts->GetGlyphRangesChineseFull());
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\fontawesome-webfont.ttf", iconFontSize, &config, icon_ranges);
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\materialdesignicons-webfont.ttf", iconFontSize, &config2, icon_ranges2);
			menu::fonts.push_back(font);

			font = io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\b3-zh-cn.ttf", baseFontSize, 0, io.Fonts->GetGlyphRangesChineseFull());
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\fontawesome-webfont.ttf", iconFontSize, &config, icon_ranges);
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\materialdesignicons-webfont.ttf", iconFontSize, &config2, icon_ranges2);
			menu::fonts.push_back(font);

			/*font = io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\SourceHanSansSC-Medium.ttf", baseFontSize, 0, io.Fonts->GetGlyphRangesChineseFull());
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\fontawesome-webfont.ttf", iconFontSize, &config, icon_ranges);
			io.Fonts->AddFontFromFileTTF("data\\skse\\plugins\\materialdesignicons-webfont.ttf", iconFontSize, &config2, icon_ranges2);
			menu::fonts.push_back(font);*/

			if (menu::fonts.size() > imgui_font_index) {
				io.FontDefault = menu::fonts[imgui_font_index];
			}
		}
	}

	void buildStyle()
	{
		switch (menu::imgui_style_index) {
		case 0:
			ImGui::StyleColorsDark(&ImGui::GetStyle());
			break;
		case 1:
			ImGui::StyleColorsLight(&ImGui::GetStyle());
			break;
		case 2:
			ImGui::StyleColorsClassic(&ImGui::GetStyle());
			break;
		case 3:
			{
				ImGuiStyle& style = ImGui::GetStyle();
				ImGui::StyleColorsDark(&style);
				style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.85f);
				style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 0.63f);
				style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 0.8f);
				break;
			}
		case 4:
			{
				ImGuiStyle& style = ImGui::GetStyle();
				style.Alpha = 1.0;
				/*style.WindowFillAlphaDefault = 0.83;
			style.ChildWindowRounding = 3;*/
				style.WindowRounding = 3;
				style.GrabRounding = 1;
				style.GrabMinSize = 20;
				style.FrameRounding = 3;

				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
				style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
				style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
				style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
				style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
				style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
				style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
				style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
				style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
				style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
				style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
				style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
				style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
				style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
				style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
				style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
				style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
				style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
				style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
				style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
				style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
				/*style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);*/
				style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
				style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
				style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				/*style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
			style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);*/
				style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
				style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
				/*style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);*/

				// 覆盖
				style.Colors[ImGuiCol_Tab] = ImLerp(style.Colors[ImGuiCol_Header], style.Colors[ImGuiCol_TitleBgActive], 0.80f);
				style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_HeaderHovered];
				style.Colors[ImGuiCol_TabActive] = ImLerp(style.Colors[ImGuiCol_HeaderActive], style.Colors[ImGuiCol_TitleBgActive], 0.60f);
				style.Colors[ImGuiCol_TabUnfocused] = ImLerp(style.Colors[ImGuiCol_Tab], style.Colors[ImGuiCol_TitleBg], 0.80f);
				style.Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(style.Colors[ImGuiCol_TabActive], style.Colors[ImGuiCol_TitleBg], 0.40f);
				style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
				style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);  // Prefer using Alpha=1.0 here
				style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
				style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
				break;
			}
		case 5:
			{
				ImGuiStyle& mStyle = ImGui::GetStyle();
				//mStyle.WindowMinSize = ImVec2(160, 20);
				mStyle.FramePadding = ImVec2(4, 2);
				mStyle.ItemSpacing = ImVec2(6, 2);
				mStyle.ItemInnerSpacing = ImVec2(6, 4);
				mStyle.Alpha = 0.95f;
				mStyle.WindowRounding = 4.0f;
				mStyle.FrameRounding = 2.0f;
				mStyle.IndentSpacing = 6.0f;
				mStyle.ItemInnerSpacing = ImVec2(2, 4);
				mStyle.ColumnsMinSpacing = 50.0f;
				mStyle.GrabMinSize = 14.0f;
				mStyle.GrabRounding = 16.0f;
				mStyle.ScrollbarSize = 12.0f;
				mStyle.ScrollbarRounding = 16.0f;

				ImGuiStyle& style = mStyle;
				style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
				style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
				style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
				style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
				style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
				style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
				style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
				style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
				style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
				style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
				style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
				style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
				style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
				style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
				style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
				style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
				style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
				style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
				style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
				style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
				style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
				style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
				style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
				style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
				style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
				style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
				//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);

				// 覆盖
				style.Colors[ImGuiCol_Tab] = ImLerp(style.Colors[ImGuiCol_Header], style.Colors[ImGuiCol_TitleBgActive], 0.80f);
				style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_HeaderHovered];
				style.Colors[ImGuiCol_TabActive] = ImLerp(style.Colors[ImGuiCol_HeaderActive], style.Colors[ImGuiCol_TitleBgActive], 0.60f);
				style.Colors[ImGuiCol_TabUnfocused] = ImLerp(style.Colors[ImGuiCol_Tab], style.Colors[ImGuiCol_TitleBg], 0.80f);
				style.Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(style.Colors[ImGuiCol_TabActive], style.Colors[ImGuiCol_TitleBg], 0.40f);
				style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
				style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);  // Prefer using Alpha=1.0 here
				style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
				style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
				break;
			}
		case 6:
			{
				// Theme from https://github.com/ArranzCNL/ImprovedCameraSE-NG
				ImGuiStyle* style = &ImGui::GetStyle();
				ImVec4* colors = ImGui::GetStyle().Colors;

				ImGui::StyleColorsClassic(style);

				// 影响大小
				/*style->WindowTitleAlign = ImVec2(0.5, 0.5);
				style->FramePadding = ImVec2(4, 4);*/

				// Rounded slider grabber
				style->GrabRounding = 12.0f;

				// Window
				colors[ImGuiCol_WindowBg] = ImVec4{ 0.118f, 0.118f, 0.118f, 0.784f };
				colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.5f };
				colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.75f };
				colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

				// Header
				colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
				colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
				colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

				// Title
				colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
				colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
				colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

				// Frame Background
				colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
				colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
				colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

				// Button
				colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
				colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
				colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

				// Tab
				colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
				colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
				colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
				colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
				colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

				// Seperator - Inner borders from Visual Studio are this colour when grabbing.
				//colors[ImGuiCol_SeparatorActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);

				// 覆盖
				colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
				colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
				colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
				colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

				// 覆盖2

				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
				//colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
				colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
				colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

				// 覆盖3
				colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 0.63f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 0.8f);

				break;
			}
		}
	}
}
