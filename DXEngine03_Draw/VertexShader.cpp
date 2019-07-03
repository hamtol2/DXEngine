#include "VertexShader.h"

VertexShader::VertexShader()
{

}

VertexShader::VertexShader(LPCTSTR fileName) : Shader(fileName)
{
	profile = "vs_4_0";
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
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL, entryPoint, profile, NULL, NULL, &shaderBuffer, NULL);
	if (IsError(result, TEXT("정점 셰이더 컴파일 실패")))
		return false;

	return true;
}

bool VertexShader::CreateShader(ID3D11Device * device)
{
	HRESULT result = device->CreateVertexShader(
		shaderBuffer->GetBufferPointer(), 
		shaderBuffer->GetBufferSize(), 
		NULL, 
		&vertexShader
	);

	if (IsError(result, TEXT("정점 셰이더 생성 실패")))
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