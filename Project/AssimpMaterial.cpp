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

		// �R���X�^���g�o�b�t�@�쐬
		auto sts = CreateConstantBuffer(
			device,								// �f�o�C�X
			sizeof(ConstantBufferMaterial),		// �T�C�Y
			m_pConstantBufferMaterial.GetAddressOf());		// �R���X�^���g�o�b�t�@�S
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

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
		devcontext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
		devcontext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

	}
	void AssimpMaterial::SetGPU()
	{

		auto devcontext = D3D::Get()->GetContext();

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
		devcontext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
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

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
		devcontext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
		devcontext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
	}
	void AssimpMaterial::Finalize()
	{

	}
}