#pragma once
#include <Windows.h>
#include <functional>


class ImGuiHook {
public:
	static inline BOOL(__stdcall* OriginalwglSwapBuffers)(HDC hDc)  = nullptr;
	static inline HWND hWnd = nullptr;
	static inline bool initImGui = false;
	static inline WNDPROC o_WndProc;
	static inline WPARAM WParam;
	static inline HGLRC g_WglContext;
	static LRESULT CALLBACK h_WndProc(const HWND	 hWnd, UINT uMsg, WPARAM wParam, LPARAM	lParam);
	static bool __stdcall wglSwapBuffers(IN  HDC hDc);
	static void InitOpenGL3(IN  HDC hDc,OUT bool* init,OUT bool* status);
	static void RenderOpenGL3(IN  HGLRC  WglContext, IN  HDC hDc, OUT bool* status);
	static void RenderWin32(IN  std::function<void()> render);
	static void ExitStatus(bool* status, bool value);
	static void Render();
	static void InitHook();
};