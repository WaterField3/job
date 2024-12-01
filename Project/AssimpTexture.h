#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>							// ComPtrの定義を含むヘッダファイル
#include <filesystem>

#include "direct3d.h"

namespace TMF
{
	class AssimpTexture
	{
		std::string m_textureName{};						// ファイル名
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv{};		// シェーダーリソースビュー

		int m_width;									// 幅
		int m_height;									// 高さ
		int m_bpp;										// BPP
	public:
		AssimpTexture();
		~AssimpTexture();
		bool Load(const std::string& filename);
		bool LoadFromFemory(const unsigned char* data, int len);

		void SetGPU();
	};
}
