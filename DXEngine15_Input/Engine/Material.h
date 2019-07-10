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
	bool LoadTexture(ID3D11Device* device);
	void BindTexture(ID3D11DeviceContext* deviceContext);

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