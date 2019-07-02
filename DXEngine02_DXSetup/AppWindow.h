#pragma once

#include <Windows.h>

class Engine;

class AppWindow
{
public:
	AppWindow(HINSTANCE hinstance);
	~AppWindow();

	int Run(Engine* engine);
	bool InitializeWindow();
	virtual LRESULT MessageProcessor(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	);

	// Getter.
	HWND GetWindowHandle() const { return hwnd; }
	UINT GetScreenWidth() const { return screenWidth; }
	UINT GetScreenHeight() const { return screenHeight; }
	LPCTSTR GetApplicationName() const { return applicationName; }

protected:

	HWND hwnd;				// 윈도우 핸들.
	HINSTANCE hinstance;		// 창 인스턴스.
	UINT screenWidth;		// 화면 가로크기.
	UINT screenHeight;		// 세로크기.
	LPCTSTR applicationName;// 창 이름.
	DWORD windowStyle;		// 창 모양.
};