#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>							// ComPtr�̒�`���܂ރw�b�_�t�@�C��
#include <filesystem>

#include "direct3d.h"

namespace TMF
{
	class AssimpTexture
	{
		std::string m_textureName{};						// �t�@�C����
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv{};		// �V�F�[�_�[���\�[�X�r���[

		int m_width;									// ��
		int m_height;									// ����
		int m_bpp;										// BPP
	public:
		AssimpTexture();
		~AssimpTexture();
		bool Load(const std::string& filename);
		bool LoadFromFemory(const unsigned char* data, int len);

		void SetGPU();
	};
}
