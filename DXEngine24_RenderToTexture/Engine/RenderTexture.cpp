#include "RenderTexture.h"



RenderTexture::RenderTexture()
{
	renderTargetTexture = NULL;
	renderTargetView = NULL;
	shaderResourceView = NULL;
}


RenderTexture::~RenderTexture()
{
}

bool RenderTexture::Initialize(
	ID3D11Device * device, 
	Camera * camera, 
	int width, int height)
{
	// ������.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// ���� Ÿ�� �ؽ�ó.
	HRESULT result = device->CreateTexture2D(
		&textureDesc, NULL, &renderTargetTexture
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT ���� Ÿ�� �ؽ�ó ���� ����")))
		return false;

	// ������.
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// ���� Ÿ�� �� ����.
	result = device->CreateRenderTargetView(
		renderTargetTexture, &rtvDesc, &renderTargetView
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT ���� Ÿ�� �� ���� ����")))
		return false;

	// ���̴� ���ҽ� �� ������.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// SRV ����.
	result = device->CreateShaderResourceView(
		renderTargetTexture, &srvDesc, &shaderResourceView
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("RT ���̴� ���ҽ� �� ���� ����")))
		return false;

	// ��/���� ��� �����.
	view = XMMatrixLookAtLH(
		camera->GetPositionVector(),
		camera->GetCameraLook(),
		camera->GetCameraUp()
	);

	projection = XMMatrixOrthographicLH(
		static_cast<float>(width),
		static_cast<float>(height),
		1.0f, 
		100000.0f
	);

	return true;
}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView)
{
	deviceContext->OMSetRenderTargets(
		1, &renderTargetView, depthStencilView
	);
}

void RenderTexture::ClearRenderTarget(
	ID3D11DeviceContext * deviceContext, 
	ID3D11DepthStencilView * depthStencilView,
	float color[])
{
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderTexture::Release()
{
	Memory::SafeRelease(renderTargetTexture);
	Memory::SafeRelease(renderTargetView);
	Memory::SafeRelease(shaderResourceView);
}