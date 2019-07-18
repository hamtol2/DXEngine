#pragma once

#include "DXUtil.h"
#include "Vertex.h"
#include "Mesh.h"

class RTRenderer : public AlignedAllocation<16>
{
public:
	RTRenderer();
	~RTRenderer();

	bool Initialize(
		ID3D11Device* device, 
		ID3DBlob* vertexShaderBuffer,
		int screenWidth,
		int screenHeight,
		int bitmapWidth,
		int bitmapHeight
	);

	void UpdateBuffers(
		ID3D11DeviceContext* deviceContext,
		int positionX, int positionY
	);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	void Release();

private:

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11InputLayout* inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[5] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int vertexCount;
	int indexCount;
	
	int screenWidth;						// 그릴 위치 계산용(화면 가로 크기).
	int screenHeight;						// 그릴 위치 계산용(화면 세로 크기).
	
	int bitmapWidth;						// 2D 이미지 그릴 화면 가로 크기.
	int bitmapHeight;						// 2D 이미지 그릴 화면 세로 크기.

	Vertex vertices[4];						// 정점 배열.
	
	ID3D11Buffer* constantBuffer;			// 월드 행렬 전달할 상수 버퍼.
	Mesh::PerObjectBuffer matrixData;		// 월드 행렬.
};