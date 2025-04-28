#include <Windows.h>
#include <functional>
#include "../Inludes/ImGuiHook/ImGuiHook.hpp"
#include "../Inludes/imgui/imgui.h"
//在您的项目中 创建一个线程并且调用ImGuiHook::InitHook传入您的绘制函数

static void Render()
{
	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
	ImGui::Begin("Default Menu");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();
}
static void Initialize() {
	ImGuiHook::InitHook(Render);
}
bool __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialize),nullptr, 0, nullptr);
    return TRUE;
}

