#pragma once

#include <Windows.h>
#include "AppWindow.h"
#include "DXUtil.h"

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void StartTimer();
	double GetTime() const;
	float GetDeltaTime();

	void UpdateTimer(AppWindow* window);
	void Reset(AppWindow* window);
	int GetFPS() const;

private:

	double countsPerSecond = 0.0;
	__int64 countStart = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 previousFrameTime = 0;
};