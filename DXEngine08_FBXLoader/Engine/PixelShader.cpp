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
	// 픽셀 셰이더 컴파일 -> 바이트 코드.
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL,
		entryPoint, profile, NULL, NULL, &shaderBuffer, NULL
	);

	// 오류 확인.
	if (IsError(result, TEXT("픽셀 셰이더 컴파일 실패")))
		return false;

	return true;
}

bool PixelShader::CreateShader(ID3D11Device * device)
{
	// 픽셀 셰이더 객체 생성.
	HRESULT result = device->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&pixelShader
	);

	// 오류 확인.
	if (IsError(result, TEXT("픽셀 셰이더 객체 생성 실패")))
		return false;

	return true;
}

void PixelShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	// 픽셀 셰이더 연결(바인딩).
	deviceContext->PSSetShader(pixelShader, NULL, NULL);
}

void PixelShader::Release()
{
	// 자원 해제.
	Shader::Release();
	Memory::SafeRelease(pixelShader);
}

bool PixelShader::CreateSamplerState(ID3D11Device * device)
{
	// 샘플러 스테이트 서술자.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		// 기본 필터.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 샘플러 스테이트 생성.
	HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (IsError(result, TEXT("샘플러 스테이트 생성 실패")))
		return false;

	return true;
}

void PixelShader::BindSamplerState(ID3D11DeviceContext * deviceContext)
{
	// 샘플러 스테이트 바인딩.
	deviceContext->PSSetSamplers(0, 1, &samplerState);
}

bool PixelShader::LoadTexture(ID3D11Device * device, LPCTSTR fileName)
{
	// 텍스처 로드.
	texture.fileName = fileName;
	if (texture.LoadTextureFromFile(device) == false)
		return false;

	return true;
}

void PixelShader::BindTexture(ID3D11DeviceContext * deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, &texture.textureResourceView);
}
