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

			// デバイス取得

			auto device = D3D::Get()->GetDevice();

			assert(device);

			// インデックスバッファ作成
			bool sts = CreateIndexBuffer(
				device,										// デバイス
				(unsigned int)(indices.size()),				// インデックス数
				(void*)indices.data(),						// インデックスデータ先頭アドレス
				&m_IndexBuffer);							// インデックスバッファ

			assert(sts == true);
		}

		void SetGPU() {
			// デバイスコンテキスト取得
			auto devicecontext = D3D::Get()->GetContext();

			// インデックスバッファをセット
			devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		}
	};
}