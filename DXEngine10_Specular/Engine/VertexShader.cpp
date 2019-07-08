#include "VertexShader.h"

VertexShader::VertexShader()
{
}

VertexShader::VertexShader(LPCTSTR fileName)
	: Shader(fileName)
{
	profile = "vs_5_0";
}

VertexShader::VertexShader(LPCTSTR fileName, LPCSTR entry, LPCSTR profile)
	: Shader(fileName, entry, profile)
{

}

VertexShader::~VertexShader()
{
}

bool VertexShader::CompileShader(ID3D11Device * device)
{
	ID3DBlob* errorBlob;

	// 정점 셰이더 컴파일 -> 바이트 코드.
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL, entryPoint, profile, NULL, NULL, &shaderBuffer, &errorBlob);

	// 오류 확인.
	if (errorBlob != NULL)
	{
		char* message = (char*)errorBlob->GetBufferPointer();
		OutputDebugStringA(message);
	}

	// 오류 확인.
	if (IsError(result, TEXT("정점 셰이더 컴파일 오류")))
		return false;

	return true;
}

bool VertexShader::CreateShader(ID3D11Device * device, bool loadPreCompiled)
{
	// 사전 컴파일된 셰이더 파일을 로드하는 경우.
	if (loadPreCompiled)
	{
		D3DReadFileToBlob(fileName, &shaderBuffer);
	}

	// 정점 셰이더 객체 생성.
	HRESULT result = device->CreateVertexShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&vertexShader
	);

	// 오류 확인.
	if (IsError(result, TEXT("정점 셰이더 객체 생성 실패")))
		return false;

	return true;
}

void VertexShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->VSSetShader(vertexShader, NULL, NULL);
}

void VertexShader::Release()
{
	Shader::Release();
	Memory::SafeRelease(vertexShader);
}