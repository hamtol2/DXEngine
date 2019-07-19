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
	// 문자열 조합 -> 정점 셰이더 파일 설정을 위해.
	TCHAR buffer[128];
	wcscpy_s(
		buffer, _countof(buffer),
		ConcatString(fileName, TEXT("PS.fx")).c_str()
	);
	this->fileName = (LPCTSTR)buffer;

	// 픽셀 셰이더 컴파일 -> 바이트 코드.
	HRESULT result = D3DCompileFromFile(fileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile, NULL, NULL, &shaderBuffer, NULL
	);

	// 오류 확인.
	if (IsError(result, TEXT("픽셀 셰이더 컴파일 실패")))
		return false;

	return true;
}

bool PixelShader::CreateShader(ID3D11Device * device, bool loadPreCompiled)
{
	// 사전 컴파일된 셰이더 파일을 로드하는 경우.
	if (loadPreCompiled)
	{
		D3DReadFileToBlob(fileName, &shaderBuffer);
	}

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
	Memory::SafeRelease(samplerState);
	
	for (int ix = 0; ix < textures.size(); ++ix)
	{
		textures[ix].Release();
	}
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

void PixelShader::AddTexture(LPCTSTR fileName)
{
	//texture.fileName = fileName;
	Texture newTexture;
	newTexture.fileName = fileName;
	textures.push_back(newTexture);
}

void PixelShader::AddTexture(Texture & texture)
{
	textures.push_back(texture);
}

bool PixelShader::LoadTextures(ID3D11Device * device)
{
	// 텍스처 로드.
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