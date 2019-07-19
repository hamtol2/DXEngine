#pragma once

#include "DXUtil.h"

#define BUFFERCOUNT 2

class DeferredBuffer
{
public:
	DeferredBuffer();
	~DeferredBuffer();

	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight);
	void SetRenderTargets(
		ID3D11DeviceContext* deviceContext,
		ID3D11DepthStencilView* depthStencilView
	);
	void ClearRenderTargets(
		ID3D11DeviceContext* deviceContext,
		ID3D11DepthStencilView* depthStencilView,
		float color[]
	);

	void Release();

	// Getter.

private:

	// ·»´õ Å¸°Ù Å©±â.
	int textureWidth = 0;
	int textureHeight = 0;

	ID3D11Texture2D* renderTargetTextureArray[BUFFERCOUNT];
	ID3D11RenderTargetView* renderTargetViewArray[BUFFERCOUNT];
	ID3D11ShaderResourceView* shaderResourceViewArray[BUFFERCOUNT];
};