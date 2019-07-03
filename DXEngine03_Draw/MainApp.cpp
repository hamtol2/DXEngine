#pragma once

#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	Engine engine(hInstance);

	if (engine.Init() == false)
		return 0;

	return engine.Run();
}