#include "Material.h"

Material::Material()
{
}

Material::Material(LPCTSTR shaderName)
{
	this->shaderName = shaderName;
	vertexShader = new VertexShader(shaderName);
	pixelShader = new PixelShader(shaderName);
}

Material::~Material()
{
}

bool Material::CompileShaders(ID3D11Device * device)
{
	// ���� ���̴� ������.
	if (vertexShader->CompileShader(device) == false)
		return false;

	// �ȼ� ���̴� ������.
	if (pixelShader->CompileShader(device) == false)
		return false;

	return true;
}

bool Material::CreateShaders(ID3D11Device * device)
{
	// ���� ���̴� ��ü ����.
	if (vertexShader->CreateShader(device) == false)
		return false;

	// �ȼ� ���̴� ��ü ����.
	if (pixelShader->CreateShader(device) == false)
		return false;

	return true;
}

void Material::BindShaders(ID3D11DeviceContext * deviceContext)
{
	vertexShader->BindShader(deviceContext);
	pixelShader->BindShader(deviceContext);
}

void Material::AddTexture(LPCTSTR fileName)
{
	pixelShader->AddTexture(fileName);
}

bool Material::LoadTexture(ID3D11Device * device)
{
	if (pixelShader->LoadTexture(device) == false)
		return false;

	return true;
}

void Material::BindTexture(ID3D11DeviceContext * deviceContext)
{
	pixelShader->BindTexture(deviceContext);
}

bool Material::CreateSamplerState(ID3D11Device * device)
{
	if (pixelShader->CreateSamplerState(device) == false)
		return false;

	return true;
}

void Material::BindSamplerState(ID3D11DeviceContext * deviceContext)
{
	pixelShader->BindSamplerState(deviceContext);
}

void Material::Release()
{
	vertexShader->Release();
	pixelShader->Release();
}
