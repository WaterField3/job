#include "AssimpMaterial.h"

namespace TMF
{
	AssimpMaterial::AssimpMaterial(D3D::MATERIAL mtrl)
	{
		Create(mtrl);
	}

	AssimpMaterial::~AssimpMaterial()
	{
		Finalize();
	}

	bool AssimpMaterial::Create(D3D::MATERIAL mtrl)
	{
		auto device = D3D::Get()->GetDevice();

		// コンスタントバッファ作成
		auto sts = CreateConstantBuffer(
			device,								// デバイス
			sizeof(ConstantBufferMaterial),		// サイズ
			m_pConstantBufferMaterial.GetAddressOf());		// コンスタントバッファ４
		if (!sts) {
			MessageBox(NULL, L"CreateBuffer(constant buffer Material) error", L"Error", MB_OK);
			return false;
		}

		m_Material.Ambient = mtrl.Ambient;
		m_Material.Diffuse = mtrl.Diffuse;
		m_Material.Specular = mtrl.Specular;
		m_Material.Emission = mtrl.Emission;
		m_Material.Shiness = mtrl.Shiness;
		m_Material.TextureEnable = mtrl.TextureEnable;

		Update();

		return true;
	}
	void AssimpMaterial::Update()
	{
		ConstantBufferMaterial		cb{};

		cb.Material = m_Material;

		auto devcontext = D3D::Get()->GetContext();

		devcontext->UpdateSubresource(
			m_pConstantBufferMaterial.Get(),
			0,
			nullptr,
			&cb,
			0, 0);

		// コンスタントバッファ4をｂ3レジスタへセット（頂点シェーダー用）
		devcontext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

		// コンスタントバッファ4をｂ3レジスタへセット(ピクセルシェーダー用)
		devcontext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

	}
	void AssimpMaterial::SetGPU()
	{

		auto devcontext = D3D::Get()->GetContext();

		// コンスタントバッファ4をｂ3レジスタへセット（頂点シェーダー用）
		devcontext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

		// コンスタントバッファ4をｂ3レジスタへセット(ピクセルシェーダー用)
		devcontext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
	}
	void AssimpMaterial::SetMaterial(const D3D::MATERIAL& mtrl)
	{
		ConstantBufferMaterial		cb{};

		cb.Material = mtrl;

		auto devcontext = D3D::Get()->GetContext();

		devcontext->UpdateSubresource(
			m_pConstantBufferMaterial.Get(),
			0,
			nullptr,
			&cb,
			0, 0);

		// コンスタントバッファ4をｂ3レジスタへセット（頂点シェーダー用）
		devcontext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

		// コンスタントバッファ4をｂ3レジスタへセット(ピクセルシェーダー用)
		devcontext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
	}
	void AssimpMaterial::Finalize()
	{

	}
}