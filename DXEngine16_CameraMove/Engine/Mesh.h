#pragma once

#include "DXUtil.h"
#include "Vertex.h"
#include "Material.h"
#include <vector>

using namespace DirectX;

class Mesh : public AlignedAllocation<16>
{
private:
	struct PerObjectBuffer
	{
		PerObjectBuffer() { ZeroMemory(this, sizeof(this)); }
		XMMATRIX world;
	};

public:
	Mesh();
	Mesh(float x, float y, float z);
	Mesh(LPCSTR fbxName);
	~Mesh();

	// ����: SIMD �����ؿ���.
	// 16����Ʈ ������ ���� ������ �����ε�.
	//void* operator new(size_t byteSize)
	//{
	//	return _aligned_malloc(byteSize, 16);
	//}

	//void operator delete(void* pointer)
	//{
	//	_aligned_free(pointer);
	//}

	bool InitializeBuffers(ID3D11Device* device, Material* material);
	//bool InitializeBuffers(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	void Update(ID3D11DeviceContext* deviceContext);

	// Getter.
	XMFLOAT3 GetPosition() const { return position; }
	XMFLOAT3 GetRotation() const { return rotation; }
	XMFLOAT3 GetScale() const { return scale; }

	XMMATRIX GetWorldMatrix();

	XMMATRIX GetTranslationMatrix();
	XMMATRIX GetRotationMatrix();
	XMMATRIX GetScaleMatrix();

	LPCSTR GetFBXName() const { return fileName; }
	std::vector<Vertex>* GetVertexArray() { return &vertices; }
	std::vector<DWORD>* GetIndexArray() { return &indices; }

	int GetVertexCount() const { return vertices.size(); }
	int GetIndexCount() const { return indices.size(); }

	D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() { return inputLayoutDesc; }

	// Setter.
	void SetPosition(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	void SetRotation(float x, float y, float z) { rotation = XMFLOAT3(x, y, z); }
	void SetScale(float x, float y, float z) { scale = XMFLOAT3(x, y, z); }

private:

	bool InitializeBuffers(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);

private:

	LPCSTR fileName;			// FBX ���� �̸�.
	std::vector<Vertex> vertices;		// ���� �迭.
	std::vector<DWORD> indices;			// �ε��� �迭.
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int nVertices;				// ���� ����.
	int nIndices;				// �ε��� ����.

	ID3D11Buffer* vertexBuffer;			// ���� ���� ������ ����.
	ID3D11Buffer* indexBuffer;			// �ε��� ������ ����.
	ID3D11InputLayout* inputLayout;		// �Է� ���̾ƿ�.
	ID3D11Buffer* constantBuffer;		// ��� ���� (���� ���).

	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ��ġ ����.
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ȸ�� ����.
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);			// ������ ����.

	PerObjectBuffer matrixData;
};