#include "Model.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObjectManager.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "direct3d.h"

namespace TMF
{
	REGISTER_COMPONENT(Model);

	Model::Model()
	{

	}

	Model::~Model()
	{

	}

	void Model::OnInitialize()
	{
		auto wideFileName = std::wstring(m_loadFileName.begin(), m_loadFileName.end());
		m_model = D3D::Get()->LoadObjModel(wideFileName.c_str());
	}

	void Model::OnFinalize()
	{

	}

	void Model::OnUpdate()
	{

	}

	void Model::OnLateUpdate()
	{

	}

	void Model::OnDraw()
	{

		if (m_isDraw)
		{
			ModelDraw();
		}
	}

	void Model::OnDrawImGui()
	{
		ImGui::Checkbox("active", &m_isDraw);
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_loadFileName.c_str());
		if (ImGui::InputText("fileName", buf, 256))
		{
			m_loadFileName = buf;
		}
		if (ImGui::Button("LoadModel"))
		{
			auto wideFileName = std::wstring(m_loadFileName.begin(), m_loadFileName.end());
			m_model = D3D::Get()->LoadObjModel(wideFileName.c_str());
		}
	}

	void Model::ModelDraw()
	{
		ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

		D3D::ConstBuffer cb;
		DirectX::SimpleMath::Matrix matrixWorld;
		
		auto view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 5.0f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
		auto proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(1024) / float(576), 0.1f, 10.f);
		for (auto pGameObject : GameObjectManager::Instance().GetGameObjects())
		{
			if (pGameObject->GetName() == "CameraObject")
			{
				auto cameraComponent = pGameObject->GetComponent<Camera>();
				if (auto camera = cameraComponent.lock())
				{
					view = camera->GetViewMatrix();
					proj = camera->GetProjectionMatrix();
				}

			}
		}
		

		// XYZ�̎O���̉�]�p�x���w�肵�ĉ�]��������@�@���@�I�C���[�p
		auto matrixRotateX = DirectX::SimpleMath::Matrix::CreateRotationX(0);
		auto matrixRotateY = DirectX::SimpleMath::Matrix::CreateRotationY(0);
		auto matrixRotateZ = DirectX::SimpleMath::Matrix::CreateRotationZ(0);

		// ��]�s��
		auto matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

		if (auto gameObject = m_pOwner.lock())
		{
			auto Component = gameObject->GetComponent<Transform>();
			if (auto pComponent = Component.lock())
			{
				matrixWorld = pComponent->GetMatrixLocal();
				matrixRotate = pComponent->GetMatrixRotation();
			}
		}

		// �@���x�N�g����]�p�s��
		//cb.matrixWorldNormal = matrixRotate.Transpose();

		cb.matrixWorld = matrixWorld * view * proj;
		// ���������s��̓]�u�s����쐬���� ���V�F�[�_�[��C++�Ń������̕��т��قȂ邽��
		cb.matrixWorld = cb.matrixWorld.Transpose();

		// UV�A�j���[�V�����s��
		cb.matrixUV = DirectX::XMMatrixIdentity();
		cb.matrixUV = XMMatrixTranspose(cb.matrixUV);

		cb.materialDiffuse = { 1,1,1,1 };


		// �s����V�F�[�_�[�ɓn��
		d3dContext->UpdateSubresource(D3D::Get()->GetConstantBuffer(), 0, NULL,
			&cb, 0, 0);

		D3D::Model& mdl = m_model;

		// ������Draw���钸�_�o�b�t�@�i���f���j���w�肷��
		UINT strides = D3D::Get()->GetVertexStride();
		UINT offsets = 0;
		d3dContext->IASetVertexBuffers(0, 1, &mdl.vertexBuffer,
			&strides, &offsets);

		// �`��Ɏg���C���f�b�N�X�o�b�t�@���w�肷��
		d3dContext->IASetIndexBuffer(mdl.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �s�N�Z���V�F�[�_�[�Ƀe�N�X�`����n��
		d3dContext->PSSetShaderResources(0, 1, &mdl.texture);

		D3D::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// ��P�����@���@�`�悷�钸�_��
		d3dContext->DrawIndexed(mdl.numIndex, 0, 0); // �`�施��
	}
}
