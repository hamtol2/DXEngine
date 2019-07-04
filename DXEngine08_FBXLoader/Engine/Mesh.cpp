#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(float x, float y, float z)
{
	SetPosition(x, y, z);
}

Mesh::~Mesh()
{
	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(inputLayout);
}

bool Mesh::InitializeBuffers(
	ID3D11Device * device, ID3DBlob * vertexShaderBuffer)
{
	// 정점 배열.
	Vertex vertices[] = {
		Vertex(
			XMFLOAT3(-2.5f, 1.5f, 0.5f), 
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT2(0.0f, 0.0f)
		),
		Vertex(
			XMFLOAT3(2.5f, 1.5f, 0.5f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT2(1.0f, 0.0f)
		),
		Vertex(
			XMFLOAT3(2.5f, -1.5f, 0.5f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT2(1.0f, 1.0f)
		),
		Vertex(
			XMFLOAT3(-2.5f, -1.5f, 0.5f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT2(0.0f, 1.0f)
		)
	};

	// 배열 크기 저장.
	//int size = sizeof(vertices) / sizeof(vertices[0]);
	nVertices = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 정점 배열 정보 넣어줄 구조체.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

	// 정점 버퍼 생성.
	HRESULT result = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (IsError(result, TEXT("정점 버퍼 생성 실패")))
	{
		return false;
	}

	// 인덱스 배열.
	DWORD indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	nIndices = ARRAYSIZE(indices);

	// 버퍼 서술자.
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));

	ibDesc.ByteWidth = sizeof(DWORD) * nIndices;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 배열 정보 넣어줄 구조체.
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = indices;

	// 인덱스 버퍼 생성.
	result = device->CreateBuffer(&ibDesc, &ibData, &indexBuffer);
	if (IsError(result, TEXT("인덱스 버퍼 생성 실패")))
	{
		return false;
	}

	// 입력 레이아웃 서술자 생성.
	//LPCSTR SemanticName;
	//UINT SemanticIndex;
	//DXGI_FORMAT Format;
	//UINT InputSlot;
	//UINT AlignedByteOffset;
	//D3D11_INPUT_CLASSIFICATION InputSlotClass;
	//UINT InstanceDataStepRate;
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 입력 레이아웃 생성.
	result = device->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&inputLayout
	);

	if (IsError(result, TEXT("입력 레이아웃 생성 실패")))
	{
		return false;
	}

	// 상수 버퍼 생성.
	// DX 행우선 / HLSL 열우선.
	PerObjectBuffer matrixData;
	matrixData.world = XMMatrixTranspose(GetWorldMatrix());		// 월드 행렬 설정.

	// 버퍼 서술자.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.ByteWidth = sizeof(PerObjectBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 배열 정보 넣어줄 구조체.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// 버퍼 생성.
	result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("오브젝트 상수 버퍼 생성 실패")))
	{
		return false;
	}

	return true;
}

void Mesh::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 정점 버퍼 전달.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// 인덱스 버퍼 바인딩.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 입력 레이아웃 전달.
	deviceContext->IASetInputLayout(inputLayout);

	// 위상 설정.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 그리기.
	//deviceContext->Draw(nVertices, 0);
	deviceContext->DrawIndexed(nIndices, 0, 0);
}

void Mesh::Update(ID3D11DeviceContext * deviceContext)
{
	// 위치(회전, 스케일) 정보 업데이트.

	// 월드 행렬 버퍼 바인딩.
	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
}

XMMATRIX Mesh::GetWorldMatrix() const
{
	return XMMatrixTranslation(position.x, position.y, position.z);
}