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
	// ���ڿ� ���� -> ���� ���̴� ���� ������ ����.
	TCHAR buffer[128];
	wcscpy_s(
		buffer, _countof(buffer), 
		ConcatString(fileName, TEXT("VS.fx")).c_str()
	);
	this->fileName = (LPCTSTR)buffer;

	// ���� ���̴� ������ -> ����Ʈ �ڵ�.
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL, entryPoint, profile, NULL, NULL, &shaderBuffer, NULL);

	// ���� Ȯ��.
	if (IsError(result, TEXT("���� ���̴� ������ ����")))
		return false;

	return true;
}

bool VertexShader::CreateShader(ID3D11Device * device, bool loadPreCompiled)
{
	// ���� �����ϵ� ���̴� ������ �ε��ϴ� ���.
	if (loadPreCompiled)
	{
		D3DReadFileToBlob(fileName, &shaderBuffer);
	}

	// ���� ���̴� ��ü ����.
	HRESULT result = device->CreateVertexShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		&vertexShader
	);

	// ���� Ȯ��.
	if (IsError(result, TEXT("���� ���̴� ��ü ���� ����")))
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