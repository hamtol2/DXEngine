#pragma once

#include "Shader.h"
#include "Texture.h"

class PixelShader :	public Shader
{
public:
	PixelShader();
	PixelShader(LPCTSTR fileName);
	PixelShader(LPCTSTR fileName, LPCSTR entry, LPCSTR profile);
	~PixelShader();

	virtual bool CompileShader(ID3D11Device* device) override;
	virtual bool CreateShader(ID3D11Device* device, bool loadPreCompiled = false) override;
	virtual void BindShader(ID3D11DeviceContext* deviceContext) override;
	virtual void Release() override;

	// 샘플러 스테이트 관련.
	bool CreateSamplerState(ID3D11Device* device);
	void BindSamplerState(ID3D11DeviceContext* deviceContext);

	// 텍스처 관련.
	void AddTexture(LPCTSTR fileName);
	//bool LoadTexture(ID3D11Device* device, LPCTSTR fileName);
	bool LoadTexture(ID3D11Device* device);
	void BindTexture(ID3D11DeviceContext* deviceContext);

	// Getter.
	ID3D11PixelShader* GetPixelShader() const { return pixelShader; }

	Texture GetTexture() const { return texture; }
	ID3D11SamplerState* GetSamplerState() const { return samplerState; }

private:

	ID3D11PixelShader* pixelShader;

	Texture texture;
	ID3D11SamplerState* samplerState;
};