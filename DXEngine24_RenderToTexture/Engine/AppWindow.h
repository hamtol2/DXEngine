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

	HWND hwnd;				// ������ �ڵ�.
	HINSTANCE hinstance;		// â �ν��Ͻ�.
	UINT screenWidth;		// ȭ�� ����ũ��.
	UINT screenHeight;		// ����ũ��.
	LPCTSTR applicationName;// â �̸�.
	DWORD windowStyle;		// â ���.
};