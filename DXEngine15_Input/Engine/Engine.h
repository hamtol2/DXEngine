#pragma once

#include "DXApp.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "InputClass.h"

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
	void Update() override;
	void Render() override;

	// 입력 처리 함수.
	void ProcessInput();

private:

	// 장면(Scene) 초기화.
	bool InitializeScene();
	bool InitializeTransformation();

	// 입력 초기화.
	void InitializeInput();

private:

	Material* material;
	Material* material2;

	ID3D11Buffer* constantBuffer;			// 뷰, 투영행렬 버퍼.

	// 카메라 정보.
	Camera* camera;					// 카메라.

	Mesh* mesh;
	Mesh* mesh2;

	// 입력 클래스.
	InputClass* input;
};