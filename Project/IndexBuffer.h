#pragma once

#include <vector>
#include <wrl/client.h>
#include "dx11helper.h"

#include "direct3d.h"

namespace TMF
{
	class IndexBuffer
	{

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	public:
		void Create(const std::vector<unsigned int>& indices) {

			// �f�o�C�X�擾

			auto device = D3D::Get()->GetDevice();

			assert(device);

			// �C���f�b�N�X�o�b�t�@�쐬
			bool sts = CreateIndexBuffer(
				device,										// �f�o�C�X
				(unsigned int)(indices.size()),				// �C���f�b�N�X��
				(void*)indices.data(),						// �C���f�b�N�X�f�[�^�擪�A�h���X
				&m_IndexBuffer);							// �C���f�b�N�X�o�b�t�@

			assert(sts == true);
		}

		void SetGPU() {
			// �f�o�C�X�R���e�L�X�g�擾
			auto devicecontext = D3D::Get()->GetContext();

			// �C���f�b�N�X�o�b�t�@���Z�b�g
			devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		}
	};
}