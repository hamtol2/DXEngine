#pragma once

#include "DXApp.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

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

private:

	// 장면(Scene) 초기화.
	bool InitializeScene();
	bool InitializeTransformation();

private:

	Material* material;
	Material* material2;

	ID3D11Buffer* constantBuffer;			// 뷰, 투영행렬 버퍼.

	// 카메라 정보.
	//XMVECTOR cameraPosition;		// 카메라 위치.
	//XMVECTOR cameraView;			// 카메라 방향.
	//XMVECTOR camerUpVector;			// 카메라 위 방향.
	Camera* camera;					// 카메라.

	Mesh* mesh;
	Mesh* mesh2;
};