#pragma once

#include "DXUtil.h"
#include "Camera.h"

class DeferredRenderer : public AlignedAllocation<16>
{
public:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 textureCoord;

		VertexType() { ZeroMemory(this, sizeof(this)); }
	};

public:
	DeferredRenderer();
	~DeferredRenderer();

	bool Initialize(
		ID3D11Device* device,
		ID3DBlob* vertexShaderBuffer,
		Camera* camera, 
		UINT screenWidth, UINT screenHeight
	);

	void Render(ID3D11DeviceContext* deviceConext);
	void Update(ID3D11DeviceContext* deviceConext);
	
	void Release();

	// Getter.
	const XMMATRIX GetViewMatrix() const { return view; }
	const XMMATRIX GetProjectionMatrix() const { return projection; }

private:

	// 입력 레이아웃.
	ID3D11InputLayout* inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int vertexCount = 0;
	int indexCount = 0;

	ID3D11Buffer* vertexBuffer = NULL;
	ID3D11Buffer* indexBuffer = NULL;
	ID3D11Buffer* constantBuffer = NULL;

	XMMATRIX view = DirectX::XMMatrixIdentity();
	XMMATRIX projection = DirectX::XMMatrixIdentity();
};