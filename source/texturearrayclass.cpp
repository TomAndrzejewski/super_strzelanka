////////////////////////////////////////////////////////////////////////////////
// Filename: texturearrayclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texturearrayclass.h"
#include <cassert>
#include <memory>



TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
}


TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}


TextureArrayClass::~TextureArrayClass()
{
}


bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	HRESULT result;

	TexMetadata info;
	auto image = std::make_unique<ScratchImage>();
	auto hr = LoadFromDDSFile(filename1, DDS_FLAGS_NONE, &info, *image);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	const Image* img = image->GetImages();
	assert(img);
	size_t nimg = image->GetImageCount();
	assert(nimg > 0);
	hr = SaveToDDSFile(img, nimg, image->GetMetadata(), DDS_FLAGS_NONE, filename1);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	// Creating texture2D
	hr = CreateDDSTextureFromFile(device, filename1, NULL, &m_textures[0]);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	TexMetadata info1;
	auto image1 = std::make_unique<ScratchImage>();
	hr = LoadFromDDSFile(filename2, DDS_FLAGS_NONE, &info1, *image1);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	const Image* img1 = image1->GetImages();
	assert(img1);
	size_t nimg1 = image1->GetImageCount();
	assert(nimg1 > 0);
	hr = SaveToDDSFile(img1, nimg1, image1->GetMetadata(), DDS_FLAGS_NONE, filename2);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateDDSTextureFromFile(device, filename2, NULL, &m_textures[1]);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


void TextureArrayClass::Shutdown()
{
	// Release the texture resources.
	if(m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if(m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	return;
}


ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}