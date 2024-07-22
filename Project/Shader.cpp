#include "Shader.h"
#include "dx11helper.h"
#include"GraphicManager/GraphicManager.h"

void Shader::Create(std::string vs, std::string ps)
{
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device;
	device = D3D::Get()->GetDevice();

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	bool sts = CreateVertexShader(device,
		vs.c_str(),
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, L"CreateVertexShader error", L"error", MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		device,							// �f�o�C�X�I�u�W�F�N�g
		ps.c_str(),
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, L"CreatePixelShader error", L"error", MB_OK);
		return;
	}

	return;

}

void Shader::SetGPU()
{
}
