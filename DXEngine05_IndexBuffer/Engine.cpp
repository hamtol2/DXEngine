#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{
	Memory::SafeDelete(mesh);

	vertexShader->Release();
	Memory::SafeDelete(vertexShader);

	pixelShader->Release();
	Memory::SafeDelete(pixelShader);
}

int Engine::Run()
{
	return window->Run(this);
}

bool Engine::Init()
{
	if (DXApp::Init() == false)
		return false;
	
	// 장면 초기화.
	if (InitializeScene() == false)
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

	// 셰이더 바인딩.
	vertexShader->BindShader(deviceContext);
	pixelShader->BindShader(deviceContext);

	// 메시 버퍼 그리기.
	mesh->RenderBuffers(deviceContext);

	// 백버퍼 <-> 프론트 버퍼 교환.
	swapChain->Present(1, 0);
}

bool Engine::InitializeScene()
{
	// 정점 셰이더 생성.
	vertexShader = new VertexShader(TEXT("Shader//VS.fx"));
	// 정점 셰이더 컴파일.
	if (vertexShader->CompileShader(device) == false)
		return false;

	// 정점 셰이더 객체 생성.
	if (vertexShader->CreateShader(device) == false)
		return false;

	// 픽셀 셰이더 생성.
	pixelShader = new PixelShader(TEXT("Shader//PS.fx"));
	// 픽셀 셰이더 컴파일.
	if (pixelShader->CompileShader(device) == false)
		return false;
	// 픽셀 셰이더 객체 생성.
	if (pixelShader->CreateShader(device) == false)
		return false;

	// 메쉬 생성.
	mesh = new Mesh();
	// 초기화.
	if (mesh->InitializeBuffers(device, vertexShader->GetShaderBuffer()) 
		== false)
		return false;

	return true;
}
