#include "Texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

bool Texture::LoadTextureFromFile(ID3D11Device * device)
{
	// �̹��� ���� �б� -> �ؽ�ó ���ҽ� �����.
	// -> �ؽ�ó ���ҽ��� �̹��� ������ ����.
	// -> �ؽ�ó ���ҽ��� �����ϴ� ���ҽ� �� �����.
	ScratchImage image;
	// �̹��� ���Ϸκ��� ��ũ��ġ�̹��� �����.
	HRESULT result = GetScratchImage(fileName, &image);
	// ���� Ȯ��.
	if (IsError(result, TEXT("�̹��� ���� �ε� ����")))
		return false;

	// �ؽ�ó ���ҽ� ����.
	ID3D11Texture2D* texture;
	result = CreateTexture(device, 
		image.GetImages(), 
		image.GetImageCount(),
		image.GetMetadata(),
		(ID3D11Resource**)&texture
	);
	// ���� Ȯ��.
	if (IsError(result, TEXT("�ؽ�ó ���ҽ� ���� ����")))
		return false;

	// ���̴� ���ҽ� �� ����.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	// dds ���˿� ���� �б�.
	if (IsDDS(fileName))
	{
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		viewDesc.TextureCube.MostDetailedMip = 0;
		viewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	}
	else
	{
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = 1;
	}

	// ���̴� ���ҽ� �� �����.
	result = device->CreateShaderResourceView(
		texture, 
		&viewDesc, 
		&textureResourceView
	);
	// ���� Ȯ��.
	if (IsError(result, TEXT("���̴� ���ҽ� �� ���� ����")))
		return false;

	// �ؽ�ó ���ҽ� ����.
	Memory::SafeRelease(texture);

	return true;
}

void Texture::Release()
{
	Memory::SafeRelease(textureResourceView);
}

HRESULT Texture::GetScratchImage(LPCTSTR fileName, ScratchImage * image)
{
	TCHAR fileExtension[10];
	_wsplitpath_s(
		fileName,
		NULL, NULL, NULL, NULL, NULL, NULL,
		fileExtension, 
		sizeof(fileExtension) / sizeof(LPCTSTR)
	);

	// �̹��� ���� Ȯ���� Ȯ��.
	if (wcscmp(fileExtension, TEXT(".tga")) == 0
		|| wcscmp(fileExtension, TEXT(".TGA")) == 0)
		return LoadFromTGAFile(fileName, NULL, *image);

	else if (wcscmp(fileExtension, TEXT(".dds")) == 0
		|| wcscmp(fileExtension, TEXT(".DDS")) == 0)
		return LoadFromDDSFile(fileName, DDS_FLAGS_NONE, NULL, *image);

	else
		return LoadFromWICFile(fileName, WIC_FLAGS_NONE, NULL, *image);
}

bool Texture::IsDDS(LPCTSTR fileName)
{
	TCHAR fileExtension[10];
	_wsplitpath_s(
		fileName,
		NULL, NULL, NULL, NULL, NULL, NULL,
		fileExtension,
		sizeof(fileExtension) / sizeof(LPCTSTR)
	);

	return (wcscmp(fileExtension, TEXT(".dds")) == 0
		|| wcscmp(fileExtension, TEXT(".DDS")) == 0);
}
