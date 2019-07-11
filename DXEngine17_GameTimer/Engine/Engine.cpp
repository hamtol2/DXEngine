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

	// Ÿ�̸� �ʱ�ȭ.
	InitializeTimer();

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

void Engine::Update(float deltaTime)
{
	// ���� ��� ���ε�.
	//mesh->Update(deviceContext);
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(
		camera->GetViewMatrix() * camera->GetProjectionMatrix()
	);
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);

	// ��/���� ��� ���ε�.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render(float deltaTime)
{
	float color[] = { 0.0f, 0.25f, 0.25f, 1.0f };

	// ���� Ÿ���� ������ �������� ĥ�ϱ�.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// ����/���ٽ� �� �����.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ------- 1�� �޽� �׸��� ------- //

	// �޽� ȸ��.
	//if (input->IsKeyDown(Keyboard::Keys::A)
	//	|| input->IsKeyDown(Keyboard::Keys::Left))
	//{
	//	XMFLOAT3 rot = mesh->GetRotation();
	//	rot.y += 3.0f;
	//	mesh->SetRotation(rot.x, rot.y, rot.z);
	//	//XMFLOAT3 pos = mesh->GetPosition();
	//	//pos.x -= 3.0f;
	//	//mesh->SetPosition(pos.x, pos.y, pos.z);
	//}

	//if (input->IsKeyDown(Keyboard::Keys::D)
	//	|| input->IsKeyDown(Keyboard::Keys::Right))
	//{
	//	XMFLOAT3 rot = mesh->GetRotation();
	//	rot.y -= 3.0f;
	//	mesh->SetRotation(rot.x, rot.y, rot.z);
	//	//XMFLOAT3 pos = mesh->GetPosition();
	//	//pos.x += 3.0f;
	//	//mesh->SetPosition(pos.x, pos.y, pos.z);
	//}

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

void Engine::ProcessInput(float deltaTime)
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

	float moveSpeed = 200.0f;
	// ī�޶� �̵�/ȸ��.
	if (input->IsKeyDown(Keyboard::Keys::A) || input->IsKeyDown(Keyboard::Keys::Left))
	{
		camera->MoveRight(-1.0f * moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(Keyboard::Keys::D) || input->IsKeyDown(Keyboard::Keys::Right))
	{
		camera->MoveRight(1.0f * moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(Keyboard::Keys::W) || input->IsKeyDown(Keyboard::Keys::Up))
	{
		camera->MoveForward(1.0f * moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(Keyboard::Keys::S) || input->IsKeyDown(Keyboard::Keys::Down))
	{
		camera->MoveForward(-1.0f * moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(Keyboard::Keys::Q))
	{
		camera->MoveUp(1.0f * moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(Keyboard::Keys::E))
	{
		camera->MoveUp(-1.0f * moveSpeed * deltaTime);
	}

	float rotationSpeed = 5.0f;
	// ���콺 ���� ��ư ���� ���� �̵�.
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{	
		camera->Yaw(state.x * rotationSpeed * deltaTime);
		camera->Pitch(state.y * rotationSpeed * deltaTime);
	}
}

bool Engine::InitializeScene()
{
	// ��Ƽ���� ��ü ����.
	material = new Material(TEXT("Shader/WarpDiffuse"));
	material2 = new Material(TEXT("Shader/Specular"));

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
	material->AddTexture(TEXT("Resources/Textures/Warp.png"));
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

void Engine::InitializeTimer()
{
	gameTimer = new GameTimer();
	gameTimer->StartTimer();
}
