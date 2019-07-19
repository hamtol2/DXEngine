#include "Texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

bool Texture::LoadTextureFromFile(ID3D11Device * device)
{
	// 이미지 파일 읽기 -> 텍스처 리소스 만들기.
	// -> 텍스처 리소스에 이미지 데이터 쓰기.
	// -> 텍스처 리소스에 접근하는 리소스 뷰 만들기.
	ScratchImage image;
	// 이미지 파일로부터 스크래치이미지 만들기.
	HRESULT result = GetScratchImage(fileName, &image);
	// 오류 확인.
	if (IsError(result, TEXT("이미지 파일 로드 실패")))
		return false;

	// 텍스처 리소스 생성.
	ID3D11Texture2D* texture;
	result = CreateTexture(device, 
		image.GetImages(), 
		image.GetImageCount(),
		image.GetMetadata(),
		(ID3D11Resource**)&texture
	);
	// 오류 확인.
	if (IsError(result, TEXT("텍스처 리소스 생성 실패")))
		return false;

	// 셰이더 리소스 뷰 생성.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	// dds 포맷에 따른 분기.
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

	// 셰이더 리소스 뷰 만들기.
	result = device->CreateShaderResourceView(
		texture, 
		&viewDesc, 
		&textureResourceView
	);
	// 오류 확인.
	if (IsError(result, TEXT("셰이더 리소스 뷰 생성 실패")))
		return false;

	// 텍스처 리소스 해제.
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

	// 이미지 파일 확장자 확인.
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
