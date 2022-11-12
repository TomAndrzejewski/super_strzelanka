////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"
#include <cassert>
#include <memory>


TextureClass::TextureClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	// Has to do all this shit before calling CreateDDSTextureFromFile() because those textures dont wanna work without it
	// I think LoadFromDDSFile() and SaveToDDSFile() somehow set texture's metadata correctly, because that's what's been failing on in CreateDDSTextureFromFile()
	TexMetadata info;
	auto image = std::make_unique<ScratchImage>();
	auto hr = LoadFromDDSFile(filename, DDS_FLAGS_NONE, &info, *image);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	const Image* img = image->GetImages();
	assert(img);
	size_t nimg = image->GetImageCount();
	assert(nimg > 0);
	hr = SaveToDDSFile(img, nimg, image->GetMetadata(), DDS_FLAGS_NONE, filename);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}

	// Creating texture2D
	hr = CreateDDSTextureFromFile(device, filename, (ID3D11Resource**)&m_texture, &m_textureView);
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}
}


void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data.
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}