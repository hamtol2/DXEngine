#include "DeferredBuffer.h"

DeferredBuffer::DeferredBuffer()
{
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		renderTargetTextureArray[ix] = NULL;
		renderTargetViewArray[ix] = NULL;
		shaderResourceViewArray[ix] = NULL;
	}
}

DeferredBuffer::~DeferredBuffer()
{

}

bool DeferredBuffer::Initialize(ID3D11Device * device, int textureWidth, int textureHeight)
{
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;

	// ������.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// ���� Ÿ�� �ؽ�ó.
	HRESULT result;
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		result = device->CreateTexture2D(
			&textureDesc, NULL, &renderTargetTextureArray[ix]
		);

		// ���� Ȯ��.
		if (IsError(result, TEXT("RT ���� Ÿ�� �ؽ�ó ���� ����")))
			return false;
	}	

	// ������.
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		// ���� Ÿ�� �� ����.
		result = device->CreateRenderTargetView(
			renderTargetTextureArray[ix], &rtvDesc, &renderTargetViewArray[ix]
		);

		// ���� Ȯ��.
		if (IsError(result, TEXT("RT ���� Ÿ�� �� ���� ����")))
			return false;
	}
	

	// ���̴� ���ҽ� �� ������.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		// SRV ����.
		result = device->CreateShaderResourceView(
			renderTargetTextureArray[ix], &srvDesc, &shaderResourceViewArray[ix]
		);

		// ���� Ȯ��.
		if (IsError(result, TEXT("RT ���̴� ���ҽ� �� ���� ����")))
			return false;
	}

	return true;
}

void DeferredBuffer::SetRenderTargets(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView)
{
	// ���� Ÿ�� ����. �迭�� ����.
	deviceContext->OMSetRenderTargets(
		BUFFERCOUNT,
		renderTargetViewArray,
		depthStencilView
	);
}

void DeferredBuffer::ClearRenderTargets(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView * depthStencilView, float color[])
{
	// ���� Ÿ�� �����.
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		deviceContext->ClearRenderTargetView(renderTargetViewArray[ix], color);
	}

	// ���� ���ٽ� �� �����.
	deviceContext->ClearDepthStencilView(
		depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0
	);
}

void DeferredBuffer::Release()
{
	// ���ҽ� ����.
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		Memory::SafeRelease(renderTargetTextureArray[ix]);
		Memory::SafeRelease(renderTargetViewArray[ix]);
		Memory::SafeRelease(shaderResourceViewArray[ix]);
	}
}