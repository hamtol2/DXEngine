#pragma once

#include "DXApp.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"

class Engine : public DXApp
{
private:
	struct PerSceneBuffer
	{
		PerSceneBuffer() { ZeroMemory(this, sizeof(this)); }

		XMMATRIX viewProjection;
		//XMMATRIX projection;
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

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	ID3D11Buffer* constantBuffer;			// 뷰, 투영행렬 버퍼.

	// 카메라 정보.
	XMVECTOR cameraPosition;		// 카메라 위치.
	XMVECTOR cameraView;			// 카메라 방향.
	XMVECTOR camerUpVector;			// 카메라 위 방향.

	Mesh* mesh;
};