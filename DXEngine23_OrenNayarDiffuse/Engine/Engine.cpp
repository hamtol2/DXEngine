#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) : DXApp(hinstance)
{

}

Engine::~Engine()
{
	//Memory::SafeDelete(mesh);
	//Memory::SafeDelete(mesh2);
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		Memory::SafeDelete((*mesh));
	}

	//material->Release();
	//Memory::SafeDelete(material);
	//
	//material2->Release();
	//Memory::SafeDelete(material2);

	Memory::SafeDelete(camera);
	Memory::SafeDelete(input);

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

	// 타이머 초기화.
	InitializeTimer();

	// 입력 초기화.
	InitializeInput();

	// 장면 초기화.
	if (InitializeScene() == false)
		return false;

	// 행렬 초기화.
	if (InitializeTransformation() == false)
		return false;

	return true;
}

void Engine::Update(float deltaTime)
{
	// 월드 행렬 바인딩.
	//mesh->Update(deviceContext);
	camera->UpdateCamera();

	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(
		camera->GetViewMatrix() * camera->GetProjectionMatrix()
	);
	matrixData.worldLightPosition = XMFLOAT3(5000.0f, 5000.0f, -5000.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

	OutputDebugString(TEXT("X: "));
	OutputDebugString(std::to_wstring(camera->GetPosition().x).c_str());
	OutputDebugString(TEXT("Y: "));
	OutputDebugString(std::to_wstring(camera->GetPosition().y).c_str());
	OutputDebugString(TEXT("Z: "));
	OutputDebugString(std::to_wstring(camera->GetPosition().z).c_str());
	OutputDebugString(TEXT("\n"));

	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &matrixData, 0, 0);

	// 뷰/투영 행렬 바인딩.
	deviceContext->VSSetConstantBuffers(1, 1, &constantBuffer);
}

void Engine::Render(float deltaTime)
{
	float color[] = { 0.0f, 0.7f, 0.7f, 1.0f };

	// 렌더 타겟을 설정한 색상으로 칠하기.
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// 뎁스/스텐실 뷰 지우기.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		// 월드 행렬 바인딩.
		(*mesh)->Update(deviceContext);

		// 셰이더 바인딩.
		(*mesh)->BindShaders(deviceContext);

		// 텍스처/샘플러 스테이트 바인딩.
		(*mesh)->BindTextures(deviceContext);
		(*mesh)->BindSamplerState(deviceContext);

		// 래스터라이저 스테이트 바인딩.
		(*mesh)->BindRasterizerState(deviceContext);

		// 메시 버퍼 그리기.
		(*mesh)->RenderBuffers(deviceContext);
	}

	// 백버퍼 <-> 프론트 버퍼 교환.
	swapChain->Present(1, 0);
}

void Engine::ProcessInput(float deltaTime)
{
	// ESC 종료 처리.
	if (input->IsKeyDown(Keyboard::Keys::Escape))
	{
		if (MessageBox(NULL,
			TEXT("종료하시겠습니까?"),
			TEXT("종료"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DestroyWindow(window->GetWindowHandle());
		}
	}

	float moveSpeed = 200.0f;
	// 카메라 이동/회전.
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

	float rotationSpeed = 10.0f;
	// 마우스 왼쪽 버튼 눌릴 때만 이동.
	Mouse::State state = input->GetMouseState();
	if (state.leftButton)
	{
		camera->Yaw(state.x * rotationSpeed * deltaTime);
		camera->Pitch(state.y * rotationSpeed * deltaTime);
	}
}

bool Engine::InitializeScene()
{
	// FBX 이름 변수.
	LPCSTR fbxTPP = "Resources/Models/HeroTPP.FBX";
	LPCSTR fbxBox = "Resources/Models/SK_CharM_Cardboard.FBX";
	LPCSTR fbxWarrior = "Resources/Models/SK_CharM_Warrior.FBX";
	LPCSTR fbxBarbarous = "Resources/Models/SK_CharM_Barbarous.FBX";
	LPCSTR fbxBear = "Resources/Models/Enemy_Bear.FBX";
	LPCSTR fbxCube = "Resources/Models/cube.FBX";
	LPCSTR fbxSphere = "Resources/Models/sphere.FBX";


	// 텍스처 이름 변수.
	LPCTSTR tppDiffuseTexture = TEXT("Resources/Textures/T_Chr_FPS_D.png");
	LPCTSTR tppNormalTexture = TEXT("Resources/Textures/T_Chr_FPS_N.png");

	LPCTSTR warriorDiffuseTexture = TEXT("Resources/Textures/T_CharM_Warrior_D.TGA");
	LPCTSTR warriorNormalTexture = TEXT("Resources/Textures/T_CharM_Warrior_N.TGA");

	LPCTSTR barbarousDiffuseTexture = TEXT("Resources/Textures/T_Barbarous_D.TGA");
	LPCTSTR barbarousNormalTexture = TEXT("Resources/Textures/T_Barbarous_N.TGA");

	LPCTSTR boxDiffuseTexture = TEXT("Resources/Textures/Char_M_Cardboard_D.png");
	LPCTSTR boxNormalTexture = TEXT("Resources/Textures/Char_M_Cardboard_N.png");

	LPCTSTR bearDiffuseTexture = TEXT("Resources/Textures/T_Enemy_Bear_D.TGA");
	LPCTSTR bearNormalTexture = TEXT("Resources/Textures/T_Enemy_Bear_N.TGA");

	LPCTSTR lightStepTexture = TEXT("Resources/Textures/LightStep.png");
	LPCTSTR warpTexture = TEXT("Resources/Textures/Warp.png");

	LPCTSTR chapleTexture = TEXT("Resources/Textures/LancellottiChapel.dds");

	LPCTSTR coitTowerTexture = TEXT("Resources/Textures/CoitTower.dds");

	// 쉐이더 이름 변수.
	LPCTSTR diffuseShader = TEXT("Shader/Diffuse");
	LPCTSTR warpDiffuseShader = TEXT("Shader/WarpDiffuse");
	LPCTSTR normalMappingShader = TEXT("Shader/NormalMapping");
	LPCTSTR normalMappingShader2 = TEXT("Shader/NormalMapping2");

	LPCTSTR cubemappingShader = TEXT("Shader/CubeMapping");

	LPCTSTR rimShader = TEXT("Shader/Rim");
	LPCTSTR orenNayarShader = TEXT("Shader/OrenNayar");

	//Mesh* sphere = new Mesh(
	//	fbxSphere, cubemappingShader,
	//	D3D11_FILL_SOLID, D3D11_CULL_NONE
	//);
	//sphere->SetPosition(0.0f, 0.0f, 0.0f);
	//sphere->SetScale(200.0f, 200.0f, 200.0f);
	//sphere->AddTexture(coitTowerTexture);
	//meshes.push_back(sphere);

	// 메쉬 생성.
	Mesh* tppRim = new Mesh(fbxTPP, diffuseShader);
	tppRim->SetPosition(-70.0f, -90.0f, 0.0f);
	tppRim->SetRotation(-90.0f, 180.0f, 0.0f);

	tppRim->AddTexture(tppDiffuseTexture);

	// 배열에 추가.
	meshes.push_back(tppRim);

	// 메쉬 생성.
	Mesh* tppOrenNayar = new Mesh(fbxTPP, orenNayarShader);
	tppOrenNayar->SetPosition(70.0f, -90.0f, 0.0f);
	tppOrenNayar->SetRotation(-90.0f, 180.0f, 0.0f);

	tppRim->AddTexture(tppDiffuseTexture);

	// 배열에 추가.
	meshes.push_back(tppOrenNayar);

	//// 메쉬 생성.
	//Mesh* tppWarp = new Mesh(fbxTPP, warpDiffuseShader);
	//tppWarp->SetPosition(-70.0f, -90.0f, 0.0f);
	//tppWarp->SetRotation(-90.0f, 180.0f, 0.0f);

	//tppWarp->AddTexture(lightStepTexture);
	//tppWarp->AddTexture(tppDiffuseTexture);

	//// 배열에 추가.
	//meshes.push_back(tppWarp);

	//Mesh* tppNormal = new Mesh(
	//	fbxTPP, normalMappingShader2,
	//	D3D11_FILL_WIREFRAME,
	//	D3D11_CULL_NONE
	//);
	//tppNormal->SetPosition(70.0f, -90.0f, 0.0f);
	//tppNormal->SetRotation(-90.0f, 180.0f, 0.0f);

	//tppNormal->AddTexture(tppDiffuseTexture);
	//tppNormal->AddTexture(tppNormalTexture);

	//// 배열에 추가.
	//meshes.push_back(tppNormal);

	//Mesh* warriorNormal = new Mesh(fbxWarrior, normalMappingShader2);
	//warriorNormal->SetPosition(210.0f, -90.0f, 0.0f);
	//warriorNormal->SetRotation(-90.0f, 180.0f, 0.0f);

	//warriorNormal->AddTexture(warriorDiffuseTexture);
	//warriorNormal->AddTexture(warriorNormalTexture);

	//// 배열에 추가.
	//meshes.push_back(warriorNormal);

	//Mesh* barbarousNormal = new Mesh(fbxBarbarous, normalMappingShader);
	//barbarousNormal->SetPosition(350.0f, -90.0f, 0.0f);
	//barbarousNormal->SetRotation(-90.0f, 180.0f, 0.0f);

	//barbarousNormal->AddTexture(barbarousDiffuseTexture);
	//barbarousNormal->AddTexture(barbarousNormalTexture);

	//// 배열에 추가.
	//meshes.push_back(barbarousNormal);

	//Mesh* bearNormal = new Mesh(fbxBear, normalMappingShader);
	//bearNormal->SetPosition(530.0f, -90.0f, 0.0f);
	//bearNormal->SetRotation(-90.0f, 180.0f, 0.0f);

	//bearNormal->AddTexture(bearDiffuseTexture);
	//bearNormal->AddTexture(bearNormalTexture);

	//// 배열에 추가.
	//meshes.push_back(bearNormal);

	// 메시 초기화.
	if (InitializeMeshes() == false)
		return false;

	return true;
}

bool Engine::InitializeTransformation()
{
	// 시야각 / 종횡비 설정.
	float fovY = XMConvertToRadians(60.0f);
	float aspectRatio = static_cast<float>(window->GetScreenWidth()) / static_cast<float>(window->GetScreenHeight());

	// 카메라 객체 생성.
	camera = new Camera(fovY, aspectRatio, 1.0f, 1000000.0f);

	// 버퍼에 담을 구조체 변수 설정.
	PerSceneBuffer matrixData;
	matrixData.viewProjection = XMMatrixTranspose(
		camera->GetViewMatrix() * camera->GetProjectionMatrix()
	);
	matrixData.worldLightPosition = XMFLOAT3(500.0f, 500.0f, -500.0f);
	matrixData.worldCameraPosition = camera->GetPosition();

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

bool Engine::InitializeMeshes()
{
	for (auto mesh = meshes.begin(); mesh < meshes.end(); ++mesh)
	{
		// 머티리얼 컴파일.
		if ((*mesh)->CompileShaders(device) == false)
			return false;

		// 각 셰이더 객체 생성.
		if ((*mesh)->CreateShaders(device) == false)
			return false;

		// 텍스처 로드.
		if ((*mesh)->LoadTextures(device) == false)
			return false;

		// 샘플러 스테이트 생성.
		if ((*mesh)->CreateSamplerState(device) == false)
			return false;

		// 버퍼 초기화.
		if ((*mesh)->InitializeBuffers(device) == false)
			return false;

		// 래스터라이저 스테이트 생성.
		if ((*mesh)->CreateRasterizerState(device) == false)
			return false;
	}

	return true;
}