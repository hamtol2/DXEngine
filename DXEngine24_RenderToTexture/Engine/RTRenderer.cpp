#include "RTRenderer.h"


RTRenderer::RTRenderer()
{
	vertexBuffer = NULL;
	indexBuffer = NULL;

	inputLayout = NULL;
	constantBuffer = NULL;
}


RTRenderer::~RTRenderer()
{
}

bool RTRenderer::Initialize(ID3D11Device * device, ID3DBlob * vertexShaderBuffer, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->bitmapWidth = bitmapWidth;
	this->bitmapHeight = bitmapHeight;

	vertexCount = ARRAYSIZE(vertices);
	//vertexCount = sizeof(vertices) / sizeof(vertices[0]);

	// ������.
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.ByteWidth = sizeof(Vertex) * vertexCount;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = &vertices;

	// ���� ����.
	HRESULT result = device->CreateBuffer(
		&vbDesc, &vbData, &vertexBuffer
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT ���� ���� ���� ����")))
		return false;

	// �ε��� �迭.
	DWORD indices[]
	{
		0, 1, 2,
		0, 2, 3
	};

	indexCount = sizeof(indices) / sizeof(indices[0]);

	// ������.
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));

	ibDesc.ByteWidth = sizeof(DWORD) * indexCount;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = &indices;

	// ���� ����.
	result = device->CreateBuffer(
		&ibDesc, &ibData, &indexBuffer
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT �ε��� ���� ���� ����")))
		return false;

	// �Է� ���̾ƿ�.
	result = device->CreateInputLayout(
		inputLayoutDesc,
		ARRAYSIZE(inputLayoutDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&inputLayout
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT �Է� ���̾ƿ� ���� ����")))
		return false;

	// ��� ����(���� ��� ���޿�).
		// ������.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(Mesh::PerObjectBuffer);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ��� �����.
	matrixData.world = XMMatrixTranspose(XMMatrixIdentity());

	// ���� �迭 ���� �־��� ����ü.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &matrixData;

	// ���� ����.
	result = device->CreateBuffer(
		&cbDesc, &cbData, &constantBuffer
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT ��� ���� ���� ����")))
		return false;

	return true;
}

void RTRenderer::UpdateBuffers(ID3D11DeviceContext * deviceContext, int positionX, int positionY)
{
	float left = static_cast<float>((screenWidth / 2) * -1)
		+ static_cast<float>(positionX);
	float right = left + static_cast<float>(bitmapWidth);
	float top = static_cast<float>((screenHeight / 2))
		- static_cast<float>(positionY);
	float bottom = top - static_cast<float>(bitmapHeight);

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].textureCoord = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, top, 0.0f);
	vertices[1].textureCoord = XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[2].textureCoord = XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[3].textureCoord = XMFLOAT2(0.0f, 1.0f);

	deviceContext->UpdateSubresource(
		vertexBuffer, 
		0, 0, 
		&vertices, 
		0, 0
	);
}

void RTRenderer::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// ��� ���� ���ε�.
	deviceContext->VSSetConstantBuffers(
		0, 1, &constantBuffer
	);

	// ���� ���� ����.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �ε��� ���� ���ε�.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �Է� ���̾ƿ� ����.
	deviceContext->IASetInputLayout(inputLayout);

	// ���� ����.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �׸���.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void RTRenderer::Release()
{
	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(indexBuffer);
	Memory::SafeRelease(inputLayout);
	Memory::SafeRelease(constantBuffer);
}