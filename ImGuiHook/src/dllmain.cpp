#include <Windows.h>
#include "ImGuiHook/ImGuiHook.hpp"

bool __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ImGuiHook::InitHook), nullptr, 0, nullptr);
    return TRUE;
}

