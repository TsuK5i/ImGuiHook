#pragma once
#include <Windows.h>
#include <functional>


class ImGuiHook {
private:
	static inline constexpr auto null = 0;//我不喜欢使用NULL
	static inline BOOL(__stdcall* OriginalwglSwapBuffers)(HDC hDc)  = nullptr;
	static inline HWND hWnd = nullptr;
	static inline bool initImGui = false;
	static inline WNDPROC o_WndProc = nullptr;
	static inline WPARAM WParam = null;
	static inline HGLRC g_WglContext = nullptr;
	static inline std::function<void()> Render = std::function<void()>();
	static LRESULT __stdcall h_WndProc(const HWND	 hWnd, UINT uMsg, WPARAM wParam, LPARAM	lParam);
	static bool __stdcall wglSwapBuffers(IN  HDC hDc);
	static void InitOpenGL3(IN  HDC hDc,OUT bool* init,OUT bool* status);
	static void RenderOpenGL3(IN  HGLRC  WglContext, IN  HDC hDc, OUT bool* status);
	static void RenderWin32(IN  std::function<void()> render);
	static void ExitStatus(bool* status, bool value);
public:
	static void InitHook(std::function<void()>);
};