#include "DeferredRenderer.h"



DeferredRenderer::DeferredRenderer()
{
}


DeferredRenderer::~DeferredRenderer()
{
}

bool DeferredRenderer::Initialize(ID3D11Device * device, ID3DBlob * vertexShaderBuffer, Camera * camera, UINT screenWidth, UINT screenHeight)
{
	// ���� �����.
	float right = static_cast<float>(screenWidth / 2);
	float left = right * -1.0f;
	float top = static_cast<float>(screenHeight / 2);
	float bottom = top * -1.0f;

	// ���� �迭 �����.
	VertexType vertices[4];
	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].textureCoord = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, top, 0.0f);
	vertices[1].textureCoord = XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[2].textureCoord = XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[3].textureCoord = XMFLOAT2(0.0f, 1.0f);

	// ���� �� ����.
	vertexCount = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = &vertices;

	// ���� ���� ����.
	HRESULT result = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (IsError(result, TEXT("���� ���� ���� ����")))
	{
		return false;
	}

	// �ε��� ����.
	DWORD indices[] = 
	{
		0, 1, 2,
		0, 2, 3,
	};

	indexCount = ARRAYSIZE(indices);

	// ���� ������.
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(DWORD) * indexCount;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// �ε��� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = &indices;

	// �ε��� ���� ����.
	result = device->CreateBuffer(&ibDesc, &ibData, &indexBuffer);
	if (IsError(result, TEXT("�ε��� ���� ���� ����")))
	{
		return false;
	}

	// ��� ����(���� ���).
		// ��� ���� ����.
	// DX ��켱 / HLSL ���켱.
	// ���� ��� ����.
	XMMATRIX world  = XMMatrixTranspose(XMMatrixIdentity());		

	// ���� ������.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(XMMATRIX);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// �ε��� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &world;

	// ���� ����.
	result = device->CreateBuffer(&cbDesc, &cbData, &constantBuffer);
	if (IsError(result, TEXT("������Ʈ ��� ���� ���� ����")))
	{
		return false;
	}

	// �Է� ���̾ƿ�.
	result = device->CreateInputLayout(
		inputLayoutDesc,
		ARRAYSIZE(inputLayoutDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&inputLayout
	);

	if (IsError(result, TEXT("�Է� ���̾ƿ� ���� ����")))
		return false;

	// ��/���� ���.
	view = XMMatrixLookAtLH(
		camera->GetPositionVector(),
		camera->GetLookVector(),
		camera->GetUpVector()
	);

	projection = XMMatrixOrthographicLH(
		static_cast<float>(screenWidth), 
		static_cast<float>(screenHeight),
		camera->GetNearZ(),
		camera->GetFarZ()
	);

	return true;
}

void DeferredRenderer::Render(ID3D11DeviceContext * deviceConext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceConext->IASetInputLayout(inputLayout);
	deviceConext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceConext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceConext->DrawIndexed(indexCount, 0, 0);
}

void DeferredRenderer::Update(ID3D11DeviceContext * deviceConext)
{
	deviceConext->VSSetConstantBuffers(0, 1, &constantBuffer);
}

void DeferredRenderer::Release()
{
	Memory::SafeRelease(inputLayout);
	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(indexBuffer);
	Memory::SafeRelease(constantBuffer);
}
