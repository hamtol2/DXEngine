#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

class Material
{
public:
	Material();
	Material(LPCTSTR shaderName);
	~Material();

	bool CompileShaders(ID3D11Device* device);
	bool CreateShaders(ID3D11Device* device);
	void BindShaders(ID3D11DeviceContext* deviceContext);

	void AddTexture(LPCTSTR fileName);
	bool LoadTextures(ID3D11Device* device);
	void BindTextures(ID3D11DeviceContext* deviceContext);

	void BindTexture(ID3D11DeviceContext* deviceContext,
		ID3D11ShaderResourceView* shaderResourceView
	);

	bool CreateSamplerState(ID3D11Device* device);
	void BindSamplerState(ID3D11DeviceContext* deviceContext);

	void Release();

	// Getter.
	VertexShader* GetVertexShader() const { return vertexShader; }
	PixelShader* GetPixelShader() const { return pixelShader; }

private:

	LPCTSTR shaderName;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
};