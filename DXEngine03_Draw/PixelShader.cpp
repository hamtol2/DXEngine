#include "PixelShader.h"

PixelShader::PixelShader()
{
}

PixelShader::PixelShader(LPCTSTR fileName) : Shader(fileName)
{
	profile = "ps_4_0";
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
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL, entryPoint, profile, NULL, NULL, &shaderBuffer, NULL);
	if (IsError(result, TEXT("ÇÈ¼¿ ¼ÎÀÌ´õ ÄÄÆÄÀÏ ½ÇÆÐ")))
		return false;

	return true;
}

bool PixelShader::CreateShader(ID3D11Device * device)
{
	HRESULT result = device->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL, &pixelShader
	);

	if (IsError(result, TEXT("ÇÈ¼¿ ¼ÎÀÌ´õ »ý¼º ½ÇÆÐ")))
		return false;

	return true;
}

void PixelShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->PSSetShader(pixelShader, NULL, NULL);
}

void PixelShader::Release()
{
	Shader::Release();
	Memory::SafeRelease(pixelShader);
}