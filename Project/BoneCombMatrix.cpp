#include "BoneCombMatrix.h"

#include "dx11helper.h"

namespace TMF
{
	bool BoneCombMatrix::Create()
	{
		auto device = D3D::Get()->GetDevice();

		// コンスタントバッファ作成
		auto sts = CreateConstantBufferWrite(
			device,								// デバイス
			sizeof(D3D::CBBoneCombMatrix),			// サイズ
			ConstantBuffer.GetAddressOf());		// コンスタントバッファ
		if (!sts) {
			MessageBox(NULL, L"CreateBuffer(constant buffer BoneMatrices) error", L"Error", MB_OK);
			return false;
		}

		// 行列群を定数バッファへセット
		Update();

		return true;
	}
	void BoneCombMatrix::Update()
	{
		auto devcontext = D3D::Get()->GetContext();

		//定数バッファ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;

		auto hr = devcontext->Map(
			ConstantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (SUCCEEDED(hr))
		{
			memcpy(msr.pData, &ConstantBufferMemory, sizeof(D3D::CBBoneCombMatrix));
			D3D::Get()->GetContext()->Unmap(ConstantBuffer.Get(), 0);
		}
	}
	void BoneCombMatrix::SetGPU()
	{
		auto devcontext = D3D::Get()->GetContext();

		// コンスタントバッファをｂ５レジスタへセット（頂点シェーダー用）
		devcontext->VSSetConstantBuffers(5, 1, ConstantBuffer.GetAddressOf());
	}
}
