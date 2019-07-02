#pragma once

#include "DXUtil.h"

using namespace DirectX;

class Mesh
{
private:
	struct Vertex
	{
		XMFLOAT3 position;

		Vertex(float x, float y, float z) :
			position(x, y, z) { }
	};

public:
	Mesh();
	~Mesh();

	bool InitializeBuffers(
		ID3D11Device* device, ID3DBlob* vertexShaderBuffer
	);

	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:

	int nVertices;				// 정점 개수.

	ID3D11Buffer* vertexBuffer;			// 정점 정보 저장할 버퍼.
	ID3D11InputLayout* inputLayout;		// 입력 레이아웃.
};