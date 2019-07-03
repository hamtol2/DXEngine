#pragma once

#include "Shader.h"

class PixelShader : public Shader
{
public:
	PixelShader();
	PixelShader(LPCTSTR fileName);
	PixelShader(LPCTSTR fileName, LPCSTR entry, LPCSTR profile);
	~PixelShader();

	virtual bool CompileShader(ID3D11Device* device) override;
	virtual bool CreateShader(ID3D11Device* device) override;
	virtual void BindShader(ID3D11DeviceContext* deviceContext) override;
	virtual void Release() override;

	ID3D11PixelShader* GetPixelShader() const { return pixelShader; }

private:

	ID3D11PixelShader* pixelShader;
};