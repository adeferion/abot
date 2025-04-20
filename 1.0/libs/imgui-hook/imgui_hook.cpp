#pragma warning( disable : 4003)
#pragma warning( disable : 4805)
#include <Windows.h>
#include <functional> 
#include <GL/gl.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include "../../src/data.h"

void ApplyColor() {
	ImVec4* colors = ImGui::GetStyle().Colors;
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

void ApplyStyle() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25.00f;
	style.ScrollbarSize = 15.00f;
	style.GrabMinSize = 10.00f;
	style.WindowBorderSize = 1.00f;
	style.ChildBorderSize = 1.00f;
	style.PopupBorderSize = 1.00f;
	style.FrameBorderSize = 1.00f;
	style.TabBorderSize = 1.00f;
    style.WindowRounding = 7.0f;
	style.ChildRounding = 4.00f;
	style.FrameRounding = 3.00f;
	style.PopupRounding = 4.00f;
	style.ScrollbarRounding = 9.00f;
	style.GrabRounding = 3.00f;
	style.LogSliderDeadzone = 4.00f;
	style.TabRounding = 4.00f;
	style.WindowMenuButtonPosition = 0;
	style.ColorButtonPosition = 1;
	style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
	style.SelectableTextAlign = ImVec2(0.00f, 0.00f);
	style.DisplaySafeAreaPadding = ImVec2(3.00f, 3.00f);
}

#define _CAST(t,v)	reinterpret_cast<t>(v)
#define _VOID_1(v)	std::function<void(v)>
#define _VOID_2(v)	_VOID_1(_VOID_1(v))

typedef BOOL(__stdcall* wglSwapBuffers_t) (
	HDC hDc
);

typedef LRESULT(CALLBACK* WNDPROC) (
	IN  HWND   hwnd,
	IN  UINT   uMsg,
	IN  WPARAM wParam,
	IN  LPARAM lParam
);

extern LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam
);

extern void RenderMain();

namespace ImGuiHook 
{
	// Original functions variable
	static WNDPROC			o_WndProc;
	static wglSwapBuffers_t o_wglSwapBuffers;

	// Global variable
	static HGLRC      g_WglContext;
	static bool	      initImGui = false;
	static _VOID_1()  RenderMain;

	// WndProc callback ImGui handler
	LRESULT CALLBACK h_WndProc(
		const HWND	hWnd, 
		UINT		uMsg, 
		WPARAM		wParam, 
		LPARAM		lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;

		return CallWindowProc(o_WndProc, hWnd, uMsg, wParam, lParam);
	}

	// Helper function
	void ExitStatus(bool* status, bool value)
	{
		if (status) *status = value;
	}

	// Initialisation for ImGui
	void InitOpenGL2(
		IN  HDC	  hDc, 
		OUT bool* init,
		OUT bool* status)
	{
		if (*init) return;
		auto tStatus = true;

		auto hWnd = WindowFromDC(hDc);
		auto wLPTR = SetWindowLongPtr(hWnd, GWLP_WNDPROC, _CAST(LONG_PTR, h_WndProc));
		if (!wLPTR) return ExitStatus(status, false);

		o_WndProc = _CAST(WNDPROC, wLPTR);
		g_WglContext = wglCreateContext(hDc);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromMemoryTTF(fontData, sizeof(fontData), 14.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		io.IniFilename = NULL;
		ApplyColor();
		ApplyStyle();
		tStatus &= ImGui_ImplWin32_Init(hWnd);
		tStatus &= ImGui_ImplOpenGL2_Init();

		*init = true;
		return ExitStatus(status, tStatus);
	}

	// Generic ImGui renderer for Win32 backend
	void RenderWin32(
		IN  std::function<void()> render)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		render();

		ImGui::EndFrame();
		ImGui::Render();
	}

	// Generic ImGui renderer for OpenGL2 backend
	void RenderOpenGL2(
		IN  HGLRC 	  WglContext,
		IN  HDC		  hDc,
		IN  _VOID_2() render,
		IN  _VOID_1() render_inner,
		OUT bool*	  status)
	{
		auto tStatus = true;

		auto o_WglContext = wglGetCurrentContext();
		tStatus &= wglMakeCurrent(hDc, WglContext);

		ImGui_ImplOpenGL2_NewFrame();
		render(render_inner);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		tStatus &= wglMakeCurrent(hDc, o_WglContext);

		return ExitStatus(status, tStatus);
	}

	// Hooked wglSwapBuffers function
	BOOL __stdcall h_wglSwapBuffers(
		IN  HDC hDc)
	{
		InitOpenGL2(hDc, &initImGui, nullptr);
		RenderOpenGL2(g_WglContext, hDc, RenderWin32, RenderMain, nullptr);

		return o_wglSwapBuffers(hDc);
	}

	// Function to get the pointer of wglSwapBuffers
	wglSwapBuffers_t* get_wglSwapBuffers()
	{
		auto hMod = GetModuleHandleA("OPENGL32.dll");
		if (!hMod) return nullptr;

		return (wglSwapBuffers_t*)GetProcAddress(hMod, "wglSwapBuffers");
	}

	// Initialise hook
	bool InitHook()
	{
		if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
			return kiero::bind(get_wglSwapBuffers(), (void**)&o_wglSwapBuffers, h_wglSwapBuffers) == kiero::Status::Success;

		return false;
	}

	// Main load function
	bool Load(
		IN  _VOID_1() render)
	{
		RenderMain = render;
		return InitHook();
	}

	// Main unload function
	void Unload()
	{
		kiero::shutdown();
	}
}


