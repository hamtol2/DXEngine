#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{
	Memory::SafeDelete(mesh);
	Memory::SafeDelete(mesh2);

	material->Release();
	Memory::SafeDelete(material);
	
	material2->Release();
	Memory::SafeDelete(material2);

	Memory::SafeDelete(camera);

	Memory::SafeRelease(constantBuffer);
}

int Engine::Run()
{
	return window->Run(this);
}

bool Engine::Init()
{
	if (DXApp::Init() == false)
		return false;

	// �Է� �ʱ�ȭ.
	InitializeInput();
	
	// ��� �ʱ�ȭ.
	if (InitializeScene() == false)
		return false;

	// ��� �ʱ�ȭ.
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update()
{
	// ���� ��� ���ε�.
	//mesh->Update(deviceContext);

	// ��/���� ��� ���ε�.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render()
{
	float color[] = { 0.0f, 0.25f, 0.25f, 1.0f };

	// ���� Ÿ���� ������ �������� ĥ�ϱ�.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// ����/���ٽ� �� �����.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ------- 1�� �޽� �׸��� ------- //
	// ���� ��� ���ε�.
	mesh->Update(deviceContext);

	// ���̴� ���ε�.
	material->BindShaders(deviceContext);

	// �ؽ�ó/���÷� ������Ʈ ���ε�.
	material->BindTexture(deviceContext);
	material->BindSamplerState(deviceContext);

	// �޽� ���� �׸���.
	mesh->RenderBuffers(deviceContext);
	// ------- 1�� �޽� �׸��� ------- //

	// ------- 2�� �޽� �׸��� ------- //
	// ���� ��� ���ε�.
	mesh2->Update(deviceContext);

	// ���̴� ���ε�.
	material2->BindShaders(deviceContext);

	// �ؽ�ó/���÷� ������Ʈ ���ε�.
	material2->BindTexture(deviceContext);
	material2->BindSamplerState(deviceContext);

	// �޽� ���� �׸���.
	mesh2->RenderBuffers(deviceContext);
	// ------- 2�� �޽� �׸��� ------- //

	// ����� <-> ����Ʈ ���� ��ȯ.
	swapChain->Present(1, 0);
}

void Engine::ProcessInput()
{
	// ESC ���� ó��.
	if (input->IsKeyDown(Keyboard::Keys::Escape))
	{
		if (MessageBox(NULL, 
			TEXT("�����Ͻðڽ��ϱ�?"), 
			TEXT("����"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DestroyWindow(window->GetWindowHandle());
		}
	}
}

bool Engine::InitializeScene()
{
	// ��Ƽ���� ��ü ����.
	material = new Material(TEXT("Shader/Specular"));
	material2 = new Material(TEXT("Shader/Diffuse"));

	// ��Ƽ���� ������.
	if (material->CompileShaders(device) == false)
		return false;
	if (material2->CompileShaders(device) == false)
		return false;

	// �� ���̴� ��ü ����.
	if (material->CreateShaders(device) == false)
		return false;
	if (material2->CreateShaders(device) == false)
		return false;

	// �ؽ�ó ���� ó��.
	// �ؽ�ó �߰�.
	material->AddTexture(TEXT("Resources/Textures/T_Chr_FPS_D.png"));
	material2->AddTexture(TEXT("Resources/Textures/T_Chr_FPS_D.png"));

	// �ؽ�ó �ε�.
	if (material->LoadTexture(device) == false)
		return false;
	if (material2->LoadTexture(device) == false)
		return false;

	// ���÷� ������Ʈ ����.
	if (material->CreateSamplerState(device) == false)
		return false;
	if (material2->CreateSamplerState(device) == false)
		return false;

	// �޽� ����.
	//mesh = new Mesh(0.0f, 0.0f, 0.0f);
	mesh = new Mesh("Resources/Models/HeroTPP.FBX");
	mesh->SetPosition(-70.0f, -90.0f, 0.0f);
	mesh->SetRotation(-90.0f, 180.0f, 0.0f);

	mesh2 = new Mesh("Resources/Models/HeroTPP.FBX");
	mesh2->SetPosition(70.0f, -90.0f, 0.0f);
	mesh2->SetRotation(-90.0f, 180.0f, 0.0f);

	// �ʱ�ȭ.
	//if (mesh->InitializeBuffers(device, vertexShader->GetShaderBuffer()) 
	//	== false)
	//	return false;
	if (mesh->InitializeBuffers(device, material) == false)
		return false;
	if (mesh2->InitializeBuffers(device, material) == false)
		return false;

	return true;
}

bool Engine::InitializeTransformation()
{
	// �þ߰� / ��Ⱦ�� ����.
	float fovY = XMConvertToRadians(60.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	// ī�޶� ��ü ����.
	camera = new Camera(fovY, aspectRatio, 1.0f, 10000.0f);

	// ���ۿ� ���� ����ü ���� ����.
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(
		camera->GetViewMatrix() * camera->GetProjectionMatrix()
	);
	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	// ���� ����.
	// ���� ������.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerSceneBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// �ε��� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// ��� ���� ����.
	HRESULT result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("�� ��� ���� ���� ����")))
	{
		return false;
	}

	return true;
}

void Engine::InitializeInput()
{
	input = new InputClass();
	input->InitializeInput(window->GetWindowHandle());
}