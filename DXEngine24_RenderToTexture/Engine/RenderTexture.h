#pragma once

#include "DXUtil.h"
#include "Camera.h"

using namespace DirectX;

class RenderTexture : public AlignedAllocation<16>
{
public:
	RenderTexture();
	~RenderTexture();

	bool Initialize(ID3D11Device* device, Camera* camera, int width, int height);

	void SetRenderTarget(
		ID3D11DeviceContext* deviceContext,
		ID3D11DepthStencilView* depthStencilView
	);

	void ClearRenderTarget(
		ID3D11DeviceContext* deviceContext,
		ID3D11DepthStencilView* depthStencilView,
		float color[]
	);

	ID3D11ShaderResourceView* GetShaderResourceView() const {
		return shaderResourceView;
	}

	XMMATRIX GetProjectionMatrix() const {
		return projection;
	}

	void Release();

private:

	ID3D11Texture2D* renderTargetTexture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;

	XMMATRIX view;
	XMMATRIX projection;
};