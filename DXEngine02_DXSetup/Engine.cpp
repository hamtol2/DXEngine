#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{

}

int Engine::Run()
{
	return window->Run(this);
}

bool Engine::Init()
{
	if (DXApp::Init() == false)
		return false;

	return true;
}

void Engine::Update()
{

}

void Engine::Render()
{
	float color[] = { 0.0f, 0.5f, 0.5f, 1.0f };

	// 렌더 타겟을 설정한 색상으로 칠하기.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// 백버퍼 <-> 프론트 버퍼 교환.
	swapChain->Present(1, 0);
}