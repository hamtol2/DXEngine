#include "GameTimer.h"



GameTimer::GameTimer()
{
}


GameTimer::~GameTimer()
{
}

void GameTimer::StartTimer()
{
	// ���� ���� ���� ���� ����.
	LARGE_INTEGER frequencyCount;

	// �ϵ����(CPU) ������ ���� (�ʴ���).
	QueryPerformanceFrequency(&frequencyCount);

	// ���� Ÿ������ ��ȯ�ؼ� ����.
	countsPerSecond = static_cast<double>(frequencyCount.QuadPart);

	// ƽ(Tick - Frame - Update).
	QueryPerformanceCounter(&frequencyCount);
	countStart = frequencyCount.QuadPart;
}

// ���� ���� ���ķ� ���� �ð� ��ȯ.
double GameTimer::GetTime() const
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	double time = static_cast<double>(
		(currentTime.QuadPart - countStart) / countsPerSecond
	);

	return time;
}

float GameTimer::GetDeltaTime()
{
	// ƽ ���� ���� �ð� ���ϱ�.
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	// ƽ ���� ���� ���.
	__int64 tickCount = currentTime.QuadPart - previousFrameTime;

	// ���� ������ �ð� ����.
	previousFrameTime = currentTime.QuadPart;

	// ���� ó��.
	if (tickCount < 0)
		tickCount = 0;

	// ������ �ð��� �ʴ����� ��ȯ.
	float deltaTime = static_cast<float>(tickCount / countsPerSecond);

	// ���� ó��.
	if (deltaTime < 0.0f)
		deltaTime = 0.0f;

	return deltaTime;
}

void GameTimer::UpdateTimer(AppWindow * window)
{
	// ƽ ���� ������ �� ������Ű��.
	++frameCount;

	// 1�� ������ Ÿ�̸� �ʱ�ȭ.
	if (GetTime() > 1.0f)
		Reset(window);
}

void GameTimer::Reset(AppWindow * window)
{
	// FPS ���.
	fps = frameCount;
	frameCount = 0;
	StartTimer();

	// â Ÿ��Ʋ �ٿ� FPS ���ڿ� �߰�.
	TCHAR buffer[256];
	_itow_s(fps, buffer, 10);

	// �ܼ�(Visual Studio Console)�� ���.
	OutputDebugString(TEXT("FPS: "));
	OutputDebugString(buffer);
	OutputDebugString(TEXT("\n"));

	// ���ڿ� �����ؼ� â ���� �ٲٱ�.
	std::wstring newTitle = std::wstring(window->GetApplicationName());
	newTitle = newTitle + TEXT(" FPS: ") + buffer;
	SetWindowText(window->GetWindowHandle(), newTitle.c_str());
}

int GameTimer::GetFPS() const
{
	return fps;
}
