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

	// 행렬 초기화.
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update()
{
	// 월드 행렬 바인딩.
	mesh->Update(deviceContext);

	// 뷰/투영 행렬 바인딩.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
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
	mesh = new Mesh(2.0f, 1.0f, 0.0f);
	// 초기화.
	if (mesh->InitializeBuffers(device, vertexShader->GetShaderBuffer()) 
		== false)
		return false;

	return true;
}

bool Engine::InitializeTransformation()
{
	// 카메라 정보 설정.
	cameraPosition = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f);
	cameraView = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camerUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 뷰 행렬.
	XMMATRIX view = XMMatrixLookAtLH(cameraPosition, cameraView, camerUpVector);

	// 투영 행렬.
	// 시야각 / 종횡비 설정.
	float fovY = XMConvertToRadians(70.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	XMMATRIX projection = XMMatrixPerspectiveFovLH(fovY, aspectRatio, 1.0f, 100.0f);

	// 버퍼에 담을 구조체 변수 설정.
	PerSceneBuffer matrixData;
	matrixData.view = XMMatrixTranspose(view);
	matrixData.projection = XMMatrixTranspose(projection);

	// 버퍼 생성.
	// 버퍼 서술자.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerSceneBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 배열 정보 넣어줄 구조체.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// 상수 버퍼 생성.
	HRESULT result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("씬 상수 버퍼 생성 실패")))
	{
		return false;
	}

	return true;
}
