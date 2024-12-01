#include "BoneCombMatrix.h"

#include "dx11helper.h"

namespace TMF
{
	bool BoneCombMatrix::Create()
	{
		auto device = D3D::Get()->GetDevice();

		// �R���X�^���g�o�b�t�@�쐬
		auto sts = CreateConstantBufferWrite(
			device,								// �f�o�C�X
			sizeof(D3D::CBBoneCombMatrix),			// �T�C�Y
			ConstantBuffer.GetAddressOf());		// �R���X�^���g�o�b�t�@
		if (!sts) {
			MessageBox(NULL, L"CreateBuffer(constant buffer BoneMatrices) error", L"Error", MB_OK);
			return false;
		}

		// �s��Q��萔�o�b�t�@�փZ�b�g
		Update();

		return true;
	}
	void BoneCombMatrix::Update()
	{
		auto devcontext = D3D::Get()->GetContext();

		//�萔�o�b�t�@��������
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

		// �R���X�^���g�o�b�t�@�����T���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
		devcontext->VSSetConstantBuffers(5, 1, ConstantBuffer.GetAddressOf());
	}
}
