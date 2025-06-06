#define IMGUI_DEFINE_MATH_OPERATORS
#include "gui.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-hook.hpp>
#include <shellapi.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <chrono>
#include "replayEngine.h"
#include "hooks.h"
#include "recorder.hpp"
#include "hwid.h"
#include "globals.h"

// Globals: std::string version only (remove char arrays)
std::string username = "";
std::string password = "";
std::string userStatus = "Unknown";

bool meta = false;
int items_index = 0;
const char* items[] = {"General", "Assist", "Video", "Settings", "Theme", "Data", "About"};
bool down = false;
bool alwaysTrue = true;

bool gui::show = false;
bool gui::inited = false;

bool isLoggedIn = false;
bool showLoginWindow = true;
char loginError[256] = "";

extern std::string userStatus;

static bool loaded = false;
static bool showThemeWindow = false;
static const char* themes[] = { "Classic", "DarkV2", "DarkV3", "Maroon", "Cappuchino", "ImGui Dark", "ImGui Light", "ImGui Classic" };
static int currentTheme = 0;

void StyleColorsClassic()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.41f, 0.41f, 0.41f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.41f, 0.41f, 0.41f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.41f, 0.41f, 0.41f, 0.40f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.41f, 0.41f, 0.41f, 0.40f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.35f, 0.35f, 0.35f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.35f, 0.35f, 0.35f, 0.35f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

}

void StyleColorsDarkV2()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
}

void StyleColorsDarkV3()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.15f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.48f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.37f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.21f, 0.16f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.45f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.41f, 0.00f, 1.00f, 0.40f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.48f, 0.26f, 0.98f, 0.52f);
	colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.34f, 0.06f, 0.98f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.13f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.40f, 0.26f, 0.98f, 0.50f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.20f, 0.58f, 0.73f);
	//colors[ImGuiCol_TabSelected]            = ImVec4(0.29f, 0.20f, 0.68f, 1.00f);
	//colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	//colors[ImGuiCol_TabDimmed]              = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	//colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	//colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	//colors[ImGuiCol_TextLink]               = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	//colors[ImGuiCol_NavCursor]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void StyleColorsCappuchino()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] =                    ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] =            ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] =                ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_PopupBg] =                 ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] =                  ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
    colors[ImGuiCol_BorderShadow] =            ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] =                 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] =          ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] =           ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] =                 ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive] =           ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] =        ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] =               ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] =             ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] =           ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] =    ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] =     ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] =               ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] =              ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] =        ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] =                  ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] =           ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] =            ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Header] =                  ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] =           ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] =            ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] =               ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] =        ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SeparatorActive] =         ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip] =              ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] =       ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] =        ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] =                     ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] =              ImVec4(0.28f, 0.56f, 1.00f, 0.80f);
    colors[ImGuiCol_TabActive] =               ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] =            ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] =      ImVec4(0.14f, 0.22f, 0.36f, 1.00f);
    //colors[ImGuiCol_DockingPreview] =          ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    //colors[ImGuiCol_DockingEmptyBg] =          ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] =               ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] =        ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] =           ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] =    ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] =          ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] =          ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] =            ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] =   ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] =       ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] =        ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void StyleColorsMaroon()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.07f, 0.02f, 0.02f, 0.85f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.14f, 0.11f, 0.11f, 0.92f);
	colors[ImGuiCol_Border]                 = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.70f, 0.41f, 0.41f, 0.40f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.75f, 0.48f, 0.48f, 0.69f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.48f, 0.18f, 0.18f, 0.65f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.52f, 0.12f, 0.12f, 0.87f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.80f, 0.40f, 0.40f, 0.20f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.30f, 0.20f, 0.20f, 0.60f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.96f, 0.17f, 0.17f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.00f, 0.07f, 0.07f, 0.40f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.00f, 0.36f, 0.36f, 0.60f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.80f, 0.39f, 0.39f, 0.60f);
	colors[ImGuiCol_Button]                 = ImVec4(0.71f, 0.18f, 0.18f, 0.62f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.71f, 0.27f, 0.27f, 0.79f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.80f, 0.46f, 0.46f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.56f, 0.16f, 0.16f, 0.45f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.53f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.87f, 0.53f, 0.53f, 0.80f);
	colors[ImGuiCol_Separator]              = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.68f, 0.21f, 0.21f, 0.80f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.47f, 0.12f, 0.12f, 0.79f);
	//colors[ImGuiCol_TabSelected]            = ImVec4(0.68f, 0.21f, 0.21f, 1.00f);
	//colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.95f, 0.84f, 0.84f, 0.40f);
	//colors[ImGuiCol_TabDimmed]              = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	//colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	//colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.55f, 0.23f, 0.23f, 1.00f);
	//colors[ImGuiCol_DockingPreview]         = ImVec4(0.90f, 0.40f, 0.40f, 0.31f);
	//colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.56f, 0.16f, 0.16f, 0.45f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.68f, 0.21f, 0.21f, 0.80f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void ApplyTheme(int theme)
{
    switch (theme)
    {
        case 0: StyleColorsClassic(); break;
        case 1: StyleColorsDarkV2(); break;
        case 2: StyleColorsDarkV3(); break;
        case 3: StyleColorsMaroon(); break;
        case 4: StyleColorsCappuchino(); break;
        case 5: ImGui::StyleColorsDark(); break;
        case 6: ImGui::StyleColorsLight(); break;
        case 7: ImGui::StyleColorsClassic(); break;
    }
}

void SaveThemeSetting(int theme)
{
    std::ofstream outFile("abot_imgui.txt");
    if (outFile.is_open())
    {
        outFile << theme;
        outFile.close();
    }
}

void LoadThemeSetting()
{
    std::ifstream inFile("abot_imgui.txt");
    if (inFile.is_open())
    {
        int savedTheme = 0;
        inFile >> savedTheme;
        inFile.close();

        if (savedTheme >= 0 && savedTheme < IM_ARRAYSIZE(themes)) // Validate
        {
            currentTheme = savedTheme;
            ApplyTheme(currentTheme);
        }
    }
}

void Logout() {
    isLoggedIn = false;
    showLoginWindow = true;
    username.clear();
    password.clear();
    memset(loginError, 0, sizeof(loginError));
    std::remove("logindata.txt"); // Delete the saved file
}

void SaveLoginData(const std::string& u, const std::string& p) {
    std::ofstream file("logindata.txt");
    if (file.is_open()) {
        file << u << "\n" << p << "\n";
        file.close();
    }
}

bool LoadLoginData(std::string& outUsername, std::string& outPassword) {
    std::ifstream file("logindata.txt");
    if (file.is_open()) {
        std::getline(file, outUsername);
        std::getline(file, outPassword);
        file.close();
        return true;
    }
    return false;
}

void RenderLogin() {
    static bool triedAutoLogin = false;

    ImVec2 windowSize = ImVec2(250, 120);
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 centerPos = ImVec2(
        viewport->Pos.x + (viewport->Size.x - windowSize.x) * 0.5f,
        viewport->Pos.y + (viewport->Size.y - windowSize.y) * 0.1f
    );

    if (!loaded)
    {
        LoadThemeSetting();
        loaded = true;
    }

    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);
    ImGui::SetNextWindowFocus();

    ImGui::Begin("aBot Login", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar
    );

    // On first call, try loading saved login data once
    static char usernameBuf[128] = {};
    static char passwordBuf[128] = {};
    static bool initialized = false;

    if (!triedAutoLogin) {
        triedAutoLogin = true;
        if (LoadLoginData(username, password)) {
            // Optionally attempt auto-login here or just fill inputs
        }
    }

    // Initialize buffers only once
    if (!initialized) {
        strncpy(usernameBuf, username.c_str(), sizeof(usernameBuf) - 1);
        strncpy(passwordBuf, password.c_str(), sizeof(passwordBuf) - 1);
        initialized = true;
    }

    // Input fields
    ImGui::InputText("Username", usernameBuf, IM_ARRAYSIZE(usernameBuf));
    ImGui::InputText("Password", passwordBuf, IM_ARRAYSIZE(passwordBuf), ImGuiInputTextFlags_Password);
    
    if (ImGui::Button("Login")) {
        // Update std::string with buffer content
        username = std::string(usernameBuf);
        password = std::string(passwordBuf);

        std::string hwid = GetHWID();
        if (!CheckCredentialsOnline(username, password, hwid)) {
            strcpy_s(loginError, "Invalid HWID or Credentials!");
        } else {
            isLoggedIn = true;
            showLoginWindow = false;
            strcpy_s(loginError, "");  // Clear error

            SaveLoginData(username, password);  // Save successful login
        }
    }

    if (strlen(loginError) > 0) {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), loginError);
    }

    ImGui::End();
}

void MetaRender()
{
    if (meta)
    {
        auto pl = gd::GameManager::sharedState()->getPlayLayer();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1);
        ImGui::Begin("Meta", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
        if (pl)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255, 200).Value);
            auto dual = pl->m_isDualMode;
            if (!dual)
            {
                ImGui::Text("aBot 2.0b7\nFrame: %i\nPosition X: %f\nPosition Y: %f\nRotation %f\nY Accel: %f",
                            replay.get_frame(), pl->m_player1->m_position.x, pl->m_player1->m_position.y, pl->m_player1->getRotation(),
                            pl->m_player1->m_yAccel);
            }
            else
            {
                ImGui::Text("aBot 2.0b7\nFrame: %i\nPosition X: %f, %f\nPosition Y: %f, %f\nRotation %f, %f\nY Accel: %f, %f",
                            replay.get_frame(), pl->m_player1->m_position.x, pl->m_player2->m_position.x,
                            pl->m_player1->m_position.y, pl->m_player2->m_position.y, pl->m_player1->getRotation(), pl->m_player2->getRotation(),
                            pl->m_player1->m_yAccel, pl->m_player2->m_yAccel);
            }
            ImGui::PopStyleColor();
        }
        ImGuiHook::blockMetaInput = !ImGui::IsWindowHovered(ImGuiHoveredFlags_None);
        ImGui::End();
        ImGui::PopStyleVar();
    }
}

bool opennedSP = false;
vector<string> replay_list;

void SelectReplay()
{
    auto itemRectMin = ImGui::GetItemRectMin();
    auto itemRectMax = ImGui::GetItemRectMax();
    auto itemRectSize = ImGui::GetItemRectSize();

    ImVec2 inputTextPos = itemRectMax;
    ImGui::GetWindowDrawList()->AddLine(ImVec2(inputTextPos.x, inputTextPos.y - itemRectSize.y - 1),
                                        ImVec2(inputTextPos.x, inputTextPos.y - 1),
                                        ImColor(255, 255, 255, int(ImGui::GetStyle().Alpha * 255)));

    ImGui::SameLine(0, 0);

    if (ImGui::ArrowButton("##comboopen", opennedSP ? ImGuiDir_Up : ImGuiDir_Down))
    {
        opennedSP = !opennedSP;
        if (opennedSP)
        {
            replay_list.clear();
            for (const auto &entry : filesystem::directory_iterator("aBot/Replays"))
            {
                string replay = entry.path().filename().string();
                if (replay.substr(replay.size() - 3, replay.size()) == ".ab2")
                {
                    replay_list.push_back(entry.path().filename().string().erase(replay.size() - 3, replay.size()));
                }
            }
        }
    }

    if (opennedSP)
    {
        ImGui::SetNextWindowPos(ImVec2(itemRectMin.x, itemRectMax.y + 4));
        ImGui::SetNextWindowSize(ImVec2(itemRectSize.x + ImGui::GetItemRectSize().x, NULL));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
        ImGui::Begin("##replaylist", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        for (int i = 0; i < (int)replay_list.size(); i++)
        {
            if (ImGui::MenuItem(replay_list[i].c_str()))
            {
                strcpy_s(replay.replay_name, replay_list[i].c_str());
                opennedSP = false;
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}

void gui::BeginWindow(const char *title)
{
    ImGui::Begin(title, nullptr, 
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoResize
    );

    ImGui::Spacing();
}

void gui::Render()
{
    MetaRender();

    gui::Toggle();

    if (gui::show)
    {
        if (!isLoggedIn) {
            if (showLoginWindow) {
                RenderLogin();
                return;
            }
        }
        
        if (!loaded)
        {
            LoadThemeSetting();
            loaded = true;
        }

        if (!gui::inited)
        {
            gui::inited = true;
            ImGui::SetNextWindowSize(ImVec2(450, 350));
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }

        BeginWindow("aBot");

        ImVec2 windowSize = ImGui::GetWindowSize();
        if (windowSize.x != 450 || windowSize.y != 350) {
            gui::inited = false;
        }

        if (ImGui::BeginTabBar("##tabs"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(items); i++)
            {
                if (ImGui::BeginTabItem(items[i]))
                {
                    items_index = i;

                    if (items_index == 0)
                    {
                        int mode = (int)replay.mode;

                        const char* modeItems[] = { "Disabled", "Record", "Playback" };
                        const char* currentMode = modeItems[mode];

                        float comboBoxWidth = windowSize.x - 10;

                        ImGui::PushItemWidth(comboBoxWidth);

                        if (ImGui::BeginCombo("##Mode", currentMode))
                        {
                            for (int i = 0; i < IM_ARRAYSIZE(modeItems); i++)
                            {
                                bool isSelected = (mode == i);
                                if (ImGui::Selectable(modeItems[i], isSelected))
                                {
                                    mode = i;
                                    if (mode == 1)
                                    {
                                        if (gd::GameManager::sharedState()->getGameVariable("0027"))
                                        {
                                            gd::GameManager::sharedState()->setGameVariable("0027", false);
                                        }
                                        replay.clear();
                                    }
                                }
                                if (isSelected) 
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }

                        ImGui::PopItemWidth();

                        replay.mode = (state)mode;
                        ImGui::Separator();

                        ImGui::PushItemWidth(comboBoxWidth);
                        ImGui::InputText("##ReplayName", replay.replay_name, IM_ARRAYSIZE(replay.replay_name));
                        ImGui::PopItemWidth();

                        float buttonWidth = (windowSize.x - 15) / 2;

                        ImGui::Columns(2, "buttons", false);

                        if (ImGui::Button("Save", ImVec2(buttonWidth, 0)))
                        {
                            if ((replay.replay_name != NULL) && (replay.replay_name[0] == '\0'))
                            {
                            }
                            else
                            {
                                replay.save((string)replay.replay_name);
                            }
                        }

                        ImGui::NextColumn();

                        if (ImGui::Button("Load", ImVec2(buttonWidth, 0)))
                        {
                            if ((replay.replay_name != NULL) && (replay.replay_name[0] == '\0'))
                            {
                            }
                            else
                            {
                                replay.load((string)replay.replay_name);
                            }
                        }

                        ImGui::Columns(1);

                        if (ImGui::Button("Clear", ImVec2(windowSize.x - 10, 0)))
                        {
                            replay.clear();
                        }

                        ImGui::Separator();

                        ImGui::PushItemWidth(135.f);
                        ImGui::InputFloat("##FPS", &replay.fps_value, 1.f, 1.f, "%.2f");
                        ImGui::SameLine();
                        ImGui::PushItemWidth(137.f);
                        ImGui::InputFloat("##Speed", &replay.speed_value, 0.01f, 0.f, "%.2f");
                        ImGui::PopItemWidth();

                        ImGui::Separator();

                        if (false)
                        {
                            ImGui::Columns(3, "game_buttons", false);
                            if (ImGui::Button("Show Cursor Fix"))
                            {
                                gd::GameManager::sharedState()->setGameVariable("0024", true);
                                cocos2d::CCEGLView::sharedOpenGLView()->showCursor(true);
                            }
                            ImGui::NextColumn();
                            if (ImGui::Button("Show Restart Button"))
                            {
                                gd::GameManager::sharedState()->setGameVariable("0074", true);
                            }
                            ImGui::NextColumn();
                            if (ImGui::Button("Hide Practice Buttons"))
                            {
                                gd::GameManager::sharedState()->setGameVariable("0071", true);
                            }
                            ImGui::Columns(1);
                        }

                        ImGui::Text("Frame: %i\nReplay Size: %i", replay.get_frame(), replay.replay2.size());
                    }
                    else if (items_index == 1)
                    {
                        ImGui::Checkbox("Frame Advance", &frameAdvance.enabled);

                        ImGui::Separator();

                        if (ImGui::Checkbox("Spambot", &spamBot.enabled))
                        {
                            spamBot.reset_temp();
                            auto pl = gd::GameManager::sharedState()->getPlayLayer();
                            if (pl)
                            {
                                if (!spamBot.enabled)
                                {
                                    hooks::playLayer_releaseButtonHook(pl, 0, 0, true);
                                    hooks::playLayer_releaseButtonHook(pl, 0, 0, false);
                                }
                            }
                        }

                        ImGui::Checkbox("Dual Clicks", &replay.dual_clicks);

                        ImGui::Separator();

                        if (ImGui::Checkbox("Straight Fly", &straightFly.enabled))
                        {
                            auto pl = gd::GameManager::sharedState()->getPlayLayer();
                            straightFly.start(pl);
                            if (pl)
                            {
                                if (!straightFly.enabled)
                                {
                                    hooks::playLayer_releaseButtonHook(pl, 0, 0, true);
                                    hooks::playLayer_releaseButtonHook(pl, 0, 0, false);
                                }
                            }
                        }

                        ImGui::SameLine();
                        ImGui::DragInt("##StraightFlyAcc", &straightFly.accuracy, 1, 0, 100, "Accuracy: %i");
                        ImGui::Text("Note: straight fly only works with p1");
                    }
                    else if (items_index == 2)
                    {
                        auto pl = gd::GameManager::sharedState()->getPlayLayer();
                        if (!pl) {
                            ImGui::Text("Enter a level to see the renderer tab.");
                        }
                        else {
                            static bool isRecording = false;
                            if (ImGui::Checkbox("Record", &isRecording)) {
                                if (isRecording) {
                                    recorder.start("aBot/Videos/" + string(recorder.video_name));
                                }
                                else {
                                    recorder.stop();
                                }

                            }

                            ImGui::SameLine();

                            ImGui::Checkbox("Render Until the End", &recorder.m_until_end);
                            ImGui::SameLine();

                            ImGui::Checkbox("Include Audio", &recorder.m_include_audio);     
                            ImGui::PushItemWidth(260);     
                            ImGui::InputText("##replay_name", recorder.video_name, IM_ARRAYSIZE(recorder.video_name));

                            ImGui::Spacing();

                            ImGui::Text("Resolution:");
                            ImGui::Separator(); 

                            ImGui::PushItemWidth(40);
                            ImGui::InputInt("##width", &recorder.m_width, 0);
                            ImGui::SameLine(0, 5);

                            ImGui::Text("x");
                            ImGui::SameLine(0, 5);

                            ImGui::PushItemWidth(40);
                            ImGui::InputInt("##height", &recorder.m_height, 0);
                            ImGui::SameLine(0, 5);

                            ImGui::Text("@");
                            ImGui::SameLine(0, 5);

                            ImGui::PushItemWidth(30);
                            ImGui::InputInt("##fps", &recorder.m_fps, 0);

                            ImGui::Spacing();

                            ImGui::Text("Encoding Settings");
                            ImGui::Separator(); 

                            char bitrate[128];
                            strcpy_s(bitrate, recorder.m_bitrate.c_str());
                            ImGui::PushItemWidth(50);
                            if (ImGui::InputText("Bitrate", bitrate, sizeof(bitrate))) {
                                recorder.m_bitrate = string(bitrate);
                            }

                            ImGui::SameLine();

                            char codec[128];
                            strcpy_s(codec, recorder.m_codec.c_str());
                            ImGui::PushItemWidth(80);
                            if (ImGui::InputText("Codec", codec, sizeof(codec))) {
                                recorder.m_codec = string(codec);
                            }

                            char extra_args[512];
                            strcpy_s(extra_args, recorder.m_extra_args.c_str());
                            ImGui::PushItemWidth(260);
                            if (ImGui::InputText("Extra Arguments", extra_args, sizeof(extra_args))) {
                                recorder.m_extra_args = string(extra_args);
                            }

                            char extra_args_audio[512];
                            strcpy_s(extra_args_audio, recorder.m_extra_audio_args.c_str());
                            ImGui::PushItemWidth(260);
                            if (ImGui::InputText("Extra Arguments (Audio)", extra_args_audio, sizeof(extra_args_audio))) {
                                recorder.m_extra_audio_args = string(extra_args_audio);
                            }

                            ImGui::Spacing();

                            ImGui::Text("Level Settings");
                            ImGui::Separator(); 

                            ImGui::InputFloat("Second to Render After", &recorder.m_after_end_duration, 1);
                        }
                    }
                    else if (items_index == 3)
                    {
                        ImGui::Checkbox("Accuracy Fix", &replay.accuracy_fix);
                        ImGui::Checkbox("Checkpoint Fix", &alwaysTrue);
                        ImGui::Checkbox("Ignore Inputs on Playing", &replay.ignore_input);
                        ImGui::Checkbox("Real Time", &replay.real_time);
                        ImGui::Checkbox("Meta", &meta);
                        ImGui::Separator();
                        ImGui::SameLine();
                        ImGui::Text("SpamBot Settings");
                        ImGui::PushItemWidth(100.f);
                        ImGui::DragInt("##spamrelease", &spamBot.release, 1, 1, INT_MAX, "Release: %i");
                        ImGui::SameLine();
                        ImGui::PushItemWidth(100.f);
                        ImGui::DragInt("##spamhold", &spamBot.hold, 1, 1, INT_MAX, "Hold: %i");
                        ImGui::Checkbox("Player 1", &spamBot.player_p1);
                        ImGui::SameLine();
                        ImGui::Checkbox("Player 2", &spamBot.player_p2);

                        ImGui::Separator();
                    }
                        else if (items_index == 4)
                        {

                            if (ImGui::BeginCombo("Select Theme", themes[currentTheme]))
                            {
                                for (int n = 0; n < IM_ARRAYSIZE(themes); n++)
                                {
                                    bool isSelected = (currentTheme == n);
                                    if (ImGui::Selectable(themes[n], isSelected))
                                    {
                                        currentTheme = n;
                                        SaveThemeSetting(currentTheme);
                                        ApplyTheme(currentTheme);
                                    }
                                    if (isSelected)
                                        ImGui::SetItemDefaultFocus();
                                }
                                ImGui::EndCombo();
                            }

                            if (ImGui::Button("Reset"))
                            {
                                currentTheme = 0; // assuming 0 is classic or your default
                                ApplyTheme(currentTheme);
                                SaveThemeSetting(currentTheme);
                            }
                    }
                    else if (items_index == 5)
                    {
                        std::string hwid = GetHWID();
                        ImGui::Text("HWID: %s", hwid.c_str());
                        ImGui::Text("Logged in as: %s", username.c_str());
                        ImGui::Text("Status: %s", userStatus.c_str());
                        if (ImGui::Button("Logout")) {
                            Logout();
                        }
                    }
                    else if (items_index == 6)
                    {
                        ImGui::Text("About");
                        ImGui::Separator();
                        ImGui::Text("aBot Version: 2.0 Beta 7 - Hotfix 2");
                        ImGui::Text("Compile Date: 21/05/2025");
                        ImGui::Spacing();
                        ImGui::Text("Keybinds");
                        ImGui::Separator();
                        ImGui::Text("Alt - Toggle UI");
                        ImGui::Text("C - Enable Frame Advance + Next Frame");
                        ImGui::Text("F - Disable Frame Advance");
                        ImGui::Text("S - Toggle Spam Bot");
                        ImGui::Text("T - Toggle Straight Fly");
                    }

                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

void gui::Toggle()
{
    bool isPressed = (GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;  // Right Shift key check
    if (isPressed && !down)
    {
        gui::show = !gui::show;  // Toggle visibility
        opennedSP = false;       // Close replay selection if visible
        down = true;             // Mark the key as pressed
    }
    else if (!isPressed)
    {
        down = false;            // Reset when key is released
    }
}