#include "Shader.h"

#include "dx11helper.h"
#include "direct3d.h"

namespace TMF
{
	void Shader::Create(std::string vs, std::string ps)
	{
		// �����X�L���Ή��@20231227�ǉ�
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_SINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);

		auto device = D3D::Get()->GetDevice();

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
		auto devicecontext = D3D::Get()->GetContext();

		devicecontext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
		devicecontext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g
		devicecontext->IASetInputLayout(m_pVertexLayout.Get());				// ���_���C�A�E�g�Z�b�g
	}
}
