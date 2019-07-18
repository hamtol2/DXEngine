#pragma once

#include "DXApp.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "InputClass.h"
#include "GameTimer.h"

#include "RenderTexture.h"
#include "RTRenderer.h"

class Engine : public DXApp
{
private:
	struct PerSceneBuffer
	{
		PerSceneBuffer() { ZeroMemory(this, sizeof(this)); }

		XMMATRIX viewProjection;
		XMFLOAT3 worldLightPosition;
		XMFLOAT3 worldCameraPosition;
	};

public:
	Engine(HINSTANCE hinstance);
	~Engine();

	int Run();
	bool Init() override;
	void Update(float deltaTime) override;

	void Render(float deltaTime) override;

	// �Է� ó�� �Լ�.
	void ProcessInput(float deltaTime);

	// Ÿ�̸� ���� �Լ�.
	void UpdateTimer() 
	{ 
		gameTimer->UpdateTimer(window); 
	}
	float GetDeltaTime() const 
	{ 
		return gameTimer->GetDeltaTime();
	}
	int GetFPS() const
	{
		return gameTimer->GetFPS();
	}

private:

	// ���(Scene) �ʱ�ȭ.
	bool InitializeScene();
	bool InitializeTransformation();

	// �Է� �ʱ�ȭ.
	void InitializeInput();

	// Ÿ�̸� �ʱ�ȭ.
	void InitializeTimer();

	// �޽� �ʱ�ȭ.
	bool InitializeMeshes();

	// ���� �Լ�.
	void RenderToTexture();
	void RenderScene();
	void BeginScene(float color[]);

	// ������� ������Ʈ �Լ�.
	void UpdatePerspectiveCamera();
	void UpdateOrthographicCamera();

private:

	//Material* material;
	//Material* material2;

	ID3D11Buffer* constantBuffer;			// ��, ������� ����.

	// ī�޶� ����.
	Camera* camera;					// ī�޶�.

	//Mesh* mesh;
	//Mesh* mesh2;

	// �޽� �迭.
	std::vector<Mesh*> meshes;

	// �Է� Ŭ����.
	InputClass* input;

	// ���� Ÿ�̸�.
	GameTimer* gameTimer;

	// ���� �ؽ�ó ��.
	RenderTexture* renderTexture;
	RTRenderer* rtRenderer;
	Material* rtMaterial;
};