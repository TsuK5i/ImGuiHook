#pragma comment(lib, "opengl32.lib")
#include "ImGuiHook.hpp"
#include "../../Inludes/kiero/kiero.h"
#include "../../Inludes/minhook/include/MinHook.h"
#include "../../Inludes/imgui/imgui_impl_win32.h"
#include "../../Inludes/imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <libloaderapi.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);                // Use ImGui::GetCurrentContext()
#define _CAST(t,v)	reinterpret_cast<t>(v)

void ImGuiHook::Render() {
	//在这个函数内编写您的代码
	auto DrawList = ImGui::GetBackgroundDrawList();
	DrawList->AddRectFilled(ImVec2(0, 0), ImVec2(100, 100), ImColor(255,255,255));
	//如果一切都没有问题的话，您使用这段代码应该会看见左上角有一个白色的方块
}

LRESULT CALLBACK ImGuiHook::h_WndProc(const HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	WParam = wParam;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;
	return CallWindowProc(o_WndProc, hWnd, uMsg, wParam, lParam);
}
void ImGuiHook::ExitStatus(bool* status, bool value)
{
	if (status) *status = value;
}
void ImGuiHook::InitOpenGL3(IN  HDC hDc, OUT bool* init, OUT bool* status) {
	if (WindowFromDC(hDc) == hWnd && *init) return;
	auto tStatus = true;
	hWnd = WindowFromDC(hDc);
	auto wLPTR = SetWindowLongPtr(hWnd, GWLP_WNDPROC, _CAST(LONG_PTR, h_WndProc));
	if (*init)
	{
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplOpenGL3_Init();
		return;
	}
	if (!wLPTR) return ExitStatus(status, false);
	o_WndProc = _CAST(WNDPROC, wLPTR);
	g_WglContext = wglCreateContext(hDc);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	tStatus &= ImGui_ImplWin32_Init(hWnd);
	tStatus &= ImGui_ImplOpenGL3_Init();
	*init = true;
	return ExitStatus(status, tStatus);
}
void ImGuiHook::RenderWin32(IN  std::function<void()> render) {
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	render();
	ImGui::EndFrame();
	ImGui::Render();
}
void ImGuiHook::RenderOpenGL3(IN  HGLRC 	  WglContext,IN  HDC hDc,OUT bool* status)
{
	auto tStatus = true;
	auto o_WglContext = wglGetCurrentContext();
	tStatus &= wglMakeCurrent(hDc, WglContext);
	ImGui_ImplOpenGL3_NewFrame();
	RenderWin32(Render);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	tStatus &= wglMakeCurrent(hDc, o_WglContext);
	return ExitStatus(status, tStatus);
}
bool __stdcall ImGuiHook::wglSwapBuffers(IN  HDC hDc)
{
	InitOpenGL3(hDc, &initImGui, nullptr);
	RenderOpenGL3(g_WglContext, hDc, nullptr);
	return OriginalwglSwapBuffers(hDc);
}

void ImGuiHook::InitHook() {
	//FreeConsole();
	//AllocConsole();
	//FILE* f;
	//freopen_s(&f, "CONOUT$", "w", stderr);
	//freopen_s(&f, "CONOUT$", "w", stdout);
	//freopen_s(&f, "CONIN$", "r", stdin);
	MH_Initialize();
	if (kiero::init(kiero::RenderType::OpenGL) == kiero::Status::Success) {
		auto hMod = GetModuleHandleA("OPENGL32.dll");
		if (!hMod) {
			std::cerr << "Failed to load OpenGL library (OPENGL32.dll)" << std::endl;
			return;
		}
		auto wglSwapBuffersAddress = GetProcAddress(hMod, "wglSwapBuffers");
		std::cerr << "wglSwapBuffersAddress:" << wglSwapBuffersAddress << std::endl;
		if (!wglSwapBuffersAddress)
			return;
		if (MH_CreateHook(wglSwapBuffersAddress, ImGuiHook::wglSwapBuffers, reinterpret_cast<LPVOID*>(&OriginalwglSwapBuffers)) != MH_OK || MH_EnableHook(wglSwapBuffersAddress) != MH_OK)
		{
			std::cerr << "Failed to enable hook! Target: " << wglSwapBuffersAddress << std::endl;
			return;
		}
	}
}