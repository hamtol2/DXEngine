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

	// ���÷� ������Ʈ ����.
	bool CreateSamplerState(ID3D11Device* device);
	void BindSamplerState(ID3D11DeviceContext* deviceContext);

	// �ؽ�ó ����.
	void AddTexture(LPCTSTR fileName);
	//bool LoadTexture(ID3D11Device* device, LPCTSTR fileName);
	bool LoadTextures(ID3D11Device* device);
	void BindTextures(ID3D11DeviceContext* deviceContext);

	void BindTexture(ID3D11DeviceContext* deviceContext,
		ID3D11ShaderResourceView* shaderResourceView
	);

	// Getter.
	ID3D11PixelShader* GetPixelShader() const { return pixelShader; }

	//Texture GetTexture() const { return texture; }
	std::vector<Texture> GetTextureArray() const { return textures; }
	ID3D11SamplerState* GetSamplerState() const { return samplerState; }

private:

	ID3D11PixelShader* pixelShader;

	//Texture texture;
	std::vector<Texture> textures;
	ID3D11SamplerState* samplerState;
};