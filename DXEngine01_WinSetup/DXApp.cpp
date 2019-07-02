#include "DXApp.h"

DXApp::DXApp(HINSTANCE hinstance)
{
	// 扩档快 按眉 积己.
	window = new AppWindow(hinstance);
}

DXApp::~DXApp()
{
	delete window;
}

bool DXApp::Init()
{
	if (window->InitializeWindow() == false)
		return false;

	return true;
}