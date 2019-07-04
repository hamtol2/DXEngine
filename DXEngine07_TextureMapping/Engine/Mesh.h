#pragma once

#include "DXUtil.h"

using namespace DirectX;

class Mesh
{
private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;

		Vertex(float x, float y, float z) :
			position(x, y, z) { }
		Vertex(XMFLOAT3 position) : position(position) { }
		Vertex(XMFLOAT3 position, XMFLOAT4 color) 
			: position(position), color(color) { }
	};

	struct PerObjectBuffer
	{
		PerObjectBuffer() { ZeroMemory(this, sizeof(this)); }

		XMMATRIX world;
	};

public:
	Mesh();
	Mesh(float x, float y, float z);
	~Mesh();

	bool InitializeBuffers(
		ID3D11Device* device, ID3DBlob* vertexShaderBuffer
	);

	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	void Update(ID3D11DeviceContext* deviceContext);

	// Getter.
	XMFLOAT3 GetPosition() const { return position; }
	XMMATRIX GetWorldMatrix() const;

	// Setter.
	void SetPosition(float x, float y, float z)
	{
		position = XMFLOAT3(x, y, z);
	}

private:

	int nVertices;				// 정점 개수.
	int nIndices;				// 인덱스 개수.

	ID3D11Buffer* vertexBuffer;			// 정점 정보 저장할 버퍼.
	ID3D11Buffer* indexBuffer;			// 인덱스 저장할 버퍼.
	ID3D11InputLayout* inputLayout;		// 입력 레이아웃.

	ID3D11Buffer* constantBuffer;		// 상수 버퍼 (월드 행렬).

	XMFLOAT3 position;					// 위치 정보.
};