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

	// ���(Scene) �ʱ�ȭ.
	bool InitializeScene();
	bool InitializeTransformation();

private:

	Material* material;
	Material* material2;

	ID3D11Buffer* constantBuffer;			// ��, ������� ����.

	// ī�޶� ����.
	//XMVECTOR cameraPosition;		// ī�޶� ��ġ.
	//XMVECTOR cameraView;			// ī�޶� ����.
	//XMVECTOR camerUpVector;			// ī�޶� �� ����.
	Camera* camera;					// ī�޶�.

	Mesh* mesh;
	Mesh* mesh2;
};