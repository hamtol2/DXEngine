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
	// ÇÈ¼¿ ¼ÎÀÌ´õ ÄÄÆÄÀÏ -> ¹ÙÀÌÆ® ÄÚµå.
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL,
		entryPoint, profile, NULL, NULL, &shaderBuffer, NULL
	);

	// ¿À·ù È®ÀÎ.
	if (IsError(result, TEXT("ÇÈ¼¿ ¼ÎÀÌ´õ ÄÄÆÄÀÏ ½ÇÆÐ")))
		return false;

	return true;
}

bool PixelShader::CreateShader(ID3D11Device * device)
{
	// ÇÈ¼¿ ¼ÎÀÌ´õ °´Ã¼ »ý¼º.
	HRESULT result = device->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&pixelShader
	);

	// ¿À·ù È®ÀÎ.
	if (IsError(result, TEXT("ÇÈ¼¿ ¼ÎÀÌ´õ °´Ã¼ »ý¼º ½ÇÆÐ")))
		return false;

	return true;
}

void PixelShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	// ÇÈ¼¿ ¼ÎÀÌ´õ ¿¬°á(¹ÙÀÎµù).
	deviceContext->PSSetShader(pixelShader, NULL, NULL);
}

void PixelShader::Release()
{
	// ÀÚ¿ø ÇØÁ¦.
	Shader::Release();
	Memory::SafeRelease(pixelShader);
}
