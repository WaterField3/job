#include "AssimpTexture.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include	<iostream>
#include	<stb_image.h>

namespace TMF
{

	AssimpTexture::AssimpTexture()
	{

	}

	AssimpTexture::~AssimpTexture()
	{

	}

	bool AssimpTexture::Load(const std::string& filename)
	{
		bool sts = true;
		unsigned char* pixels;

		// 画像読み込み
		pixels = stbi_load(filename.c_str(), &m_width, &m_height, &m_bpp, 4);
		if (pixels == nullptr) {
			std::cout << filename.c_str() << " Load error " << std::endl;
			return false;
		}

		// テクスチャ2Dリソース生成
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = m_width;
		desc.Height = m_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource{};
		subResource.pSysMem = pixels;
		subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
		subResource.SysMemSlicePitch = 0;

		auto device = D3D::Get()->GetDevice();

		auto hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
		if (FAILED(hr)) {
			stbi_image_free(pixels);
			return false;
		}

		// SRV生成
		hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_srv.GetAddressOf());
		if (FAILED(hr)) {
			stbi_image_free(pixels);
			return false;
		}

		// ピクセルイメージ解放
		stbi_image_free(pixels);

		return true;
	}

	bool AssimpTexture::LoadFromFemory(const unsigned char* data, int len)
	{
		bool sts = true;
		unsigned char* pixels;

		// 画像読み込み
		pixels = stbi_load_from_memory(data,
			len,
			&m_width,
			&m_height,
			&m_bpp,
			STBI_rgb_alpha);

		// テクスチャ2Dリソース生成
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = m_width;
		desc.Height = m_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource{};
		subResource.pSysMem = pixels;
		subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
		subResource.SysMemSlicePitch = 0;

		auto device = D3D::Get()->GetDevice();

		auto hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
		if (FAILED(hr)) {
			stbi_image_free(pixels);
			return false;
		}

		// SRV生成
		hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_srv.GetAddressOf());
		if (FAILED(hr)) {
			stbi_image_free(pixels);
			return false;
		}

		// ピクセルイメージ解放
		stbi_image_free(pixels);

		return true;
	}

	void AssimpTexture::SetGPU()
	{
		auto devicecontext = D3D::Get()->GetContext();
		devicecontext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
	}
}