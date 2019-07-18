#include "PixelShader.h"

PixelShader::PixelShader()
{
}

PixelShader::PixelShader(LPCTSTR fileName)
	: Shader(fileName)
{
	profile = "ps_5_0";
}

PixelShader::PixelShader(LPCTSTR fileName, LPCSTR entry, LPCSTR profile)
	: Shader(fileName, entry, profile)
{
}

PixelShader::~PixelShader()
{
}

bool PixelShader::CompileShader(ID3D11Device * device)
{
	// ���ڿ� ���� -> ���� ���̴� ���� ������ ����.
	TCHAR buffer[128];
	wcscpy_s(
		buffer, _countof(buffer),
		ConcatString(fileName, TEXT("PS.fx")).c_str()
	);
	this->fileName = (LPCTSTR)buffer;

	// �ȼ� ���̴� ������ -> ����Ʈ �ڵ�.
	HRESULT result = D3DCompileFromFile(fileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile, NULL, NULL, &shaderBuffer, NULL
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("�ȼ� ���̴� ������ ����")))
		return false;

	return true;
}

bool PixelShader::CreateShader(ID3D11Device * device, bool loadPreCompiled)
{
	// ���� �����ϵ� ���̴� ������ �ε��ϴ� ���.
	if (loadPreCompiled)
	{
		D3DReadFileToBlob(fileName, &shaderBuffer);
	}

	// �ȼ� ���̴� ��ü ����.
	HRESULT result = device->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&pixelShader
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("�ȼ� ���̴� ��ü ���� ����")))
		return false;

	return true;
}

void PixelShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	// �ȼ� ���̴� ����(���ε�).
	deviceContext->PSSetShader(pixelShader, NULL, NULL);
}

void PixelShader::Release()
{
	// �ڿ� ����.
	Shader::Release();
	Memory::SafeRelease(pixelShader);
	Memory::SafeRelease(samplerState);
	
	for (int ix = 0; ix < textures.size(); ++ix)
	{
		textures[ix].Release();
	}
}

bool PixelShader::CreateSamplerState(ID3D11Device * device)
{
	// ���÷� ������Ʈ ������.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		// �⺻ ����.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// ���÷� ������Ʈ ����.
	HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (IsError(result, TEXT("���÷� ������Ʈ ���� ����")))
		return false;

	return true;
}

void PixelShader::BindSamplerState(ID3D11DeviceContext * deviceContext)
{
	// ���÷� ������Ʈ ���ε�.
	deviceContext->PSSetSamplers(0, 1, &samplerState);
}

void PixelShader::AddTexture(LPCTSTR fileName)
{
	//texture.fileName = fileName;
	Texture newTexture;
	newTexture.fileName = fileName;
	textures.push_back(newTexture);
}

bool PixelShader::LoadTextures(ID3D11Device * device)
{
	// �ؽ�ó �ε�.
	//texture.fileName = fileName;
	//if (texture.LoadTextureFromFile(device) == false)
	//	return false;
	for (int ix = 0; ix < textures.size(); ++ix)
	{
		if (textures[ix].LoadTextureFromFile(device) == false)
			return false;
	}

	//for (auto iterator = textures.begin(); 
	//	iterator < textures.end(); 
	//	++iterator)
	//{
	//	if (iterator->LoadTextureFromFile(device) == false)
	//		return false;
	//}

	return true;
}

void PixelShader::BindTextures(ID3D11DeviceContext * deviceContext)
{
	for (int ix = 0; ix < textures.size(); ++ix)
	{
		deviceContext->PSSetShaderResources(
			ix, 
			1, 
			&textures[ix].textureResourceView
		);
	}
}

void PixelShader::BindTexture(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * shaderResourceView)
{
	deviceContext->PSSetShaderResources(
		0, 1, &shaderResourceView
	);
}

	
