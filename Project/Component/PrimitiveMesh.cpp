#include "PrimitiveMesh.h"

#include <Imgui/imgui.h>
#include <WICTextureLoader.h>

#include "direct3d.h"
#include "ComponentRegister.h"
#include "Transform.h"
#include "Camera.h"
#include "Collider.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Utility/Log.h"
#include "FreeCamera.h"

REGISTER_COMPONENT(TMF::PrimitiveMesh, "PrimitiveMesh");

namespace TMF
{
	void PrimitiveMesh::OnInitialize()
	{
		MakeMesh();
		LoadTexture();
	}
	void PrimitiveMesh::MakeMesh()
	{
		DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(m_scale.x, m_scale.y, m_scale.z);

		auto context = D3D::Get()->GetContext();
		DirectX::GeometricPrimitive::SetDepthBufferMode(m_isSetDepthBufferMode);
		// �`��𐶐�
		switch (m_shapeType)
		{
		case TMF::PrimitiveMesh::CUBE:
			m_pShape = DirectX::GeometricPrimitive::CreateBox(context, size, m_isRhcoords, m_isInvertn);
			break;
		case TMF::PrimitiveMesh::SPHER:
			m_pShape = DirectX::GeometricPrimitive::CreateSphere(context, size.x, size_t(16), m_isRhcoords, m_isInvertn);
			break;
		case TMF::PrimitiveMesh::CYLINDER:
			m_pShape = DirectX::GeometricPrimitive::CreateCylinder(context, size.y, size.x, size_t(16), m_isRhcoords);
			break;
		case TMF::PrimitiveMesh::CONE:
			m_pShape = DirectX::GeometricPrimitive::CreateCone(context, size.y, size.x, size_t(16), m_isRhcoords);
			break;
		case TMF::PrimitiveMesh::TORUS:
			m_pShape = DirectX::GeometricPrimitive::CreateTorus(context, size.y, size.x, size_t(16), m_isRhcoords);
			break;
		default:
			break;
		}
	}
	void PrimitiveMesh::OnFinalize()
	{

	}
	void PrimitiveMesh::OnUpdate()
	{
	}
	void PrimitiveMesh::OnLateUpdate()
	{

	}
	void PrimitiveMesh::OnDraw()
	{
		if (m_pShape)
		{
			auto pOwner = m_pOwner.lock();
			auto world = DirectX::SimpleMath::Matrix::Identity;
			auto pTransform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				world = pLockTransform->GetWorldMatrix();
			}

			auto pCamera = GameObjectManager::Instance().GetComponent<Camera>();
			//auto pCamera = GameObjectManager::Instance().GetComponent<FreeCamera>();
			if (auto pLockCamera = pCamera.lock())
			{
				auto view = pLockCamera->GetViewMatrix();
				auto proj = pLockCamera->GetProjectionMatrix();
				m_pShape->Draw(world, view, proj, m_color, m_pTexture.Get());
			}
		}
	}
	void PrimitiveMesh::OnDrawImGui()
	{
		const char* types[] = { "Box","Sphere","Cylinder","Cone" ,"Torus" };
		int selectIndex = (int)m_shapeType;
		auto shapeLabel = StringHelper::CreateLabel("ColliderType", m_uuID);
		if (ImGui::BeginCombo(shapeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_shapeType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_shapeType = ShapeType(i);
					selectIndex = i;
					MakeMesh();
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		auto scaleLabel = StringHelper::CreateLabel("Scale", m_uuID);
		if (ImGui::DragFloat3(scaleLabel.c_str(), &m_scale.x))
		{
			MakeMesh();
		}

		auto colorLabel = StringHelper::CreateLabel("Color", m_uuID);
		if (ImGui::DragFloat4(colorLabel.c_str(), &m_color.x, 0.1f, 0.0f, 1.0f))
		{

		}
		auto invertnLabel = StringHelper::CreateLabel("Inverth", m_uuID);
		if (ImGui::Checkbox(invertnLabel.c_str(), &m_isInvertn))
		{

		}
		auto rhcoordsLabel = StringHelper::CreateLabel("Rhcoord", m_uuID);
		if (ImGui::Checkbox(rhcoordsLabel.c_str(), &m_isRhcoords))
		{

		}
		auto depthBufferModeLabel = StringHelper::CreateLabel("DepthBufferMode", m_uuID);
		if (ImGui::Checkbox(depthBufferModeLabel.c_str(), &m_isSetDepthBufferMode))
		{

		}
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_textureName.c_str());
		auto label = StringHelper::CreateLabel("FileName", m_uuID);
		if (ImGui::InputText(label.c_str(), buf, 256))
		{
			m_textureName = buf;
		}

		auto loadLabel = StringHelper::CreateLabel("LoadTexture", m_uuID);
		if (ImGui::Button(loadLabel.c_str()))
		{
			LoadTexture();
		}

		auto updateLabel = StringHelper::CreateLabel("MeshUpdate", m_uuID);
		if (ImGui::Button(updateLabel.c_str()))
		{
			MakeMesh();
		}
	}
	std::shared_ptr<Component> PrimitiveMesh::OnClone() const
	{
		auto pClone = std::make_shared<PrimitiveMesh>();
		pClone->m_isInvertn = this->m_isInvertn;
		pClone->m_isRhcoords = this->m_isRhcoords;
		pClone->m_shapeType = this->m_shapeType;
		pClone->m_color = this->m_color;
		pClone->m_scale = this->m_scale;
		pClone->m_textureName = this->m_textureName;
		return move(pClone);
	}
	void PrimitiveMesh::SetColor(DirectX::SimpleMath::Color color)
	{
		m_color = color;
	}
	void PrimitiveMesh::LoadTexture()
	{
		// �p�X���ݒ肳��Ă��Ȃ��Ƃ����s���Ȃ�
		if (m_textureName == "")
		{
			return;
		}

		// �e�N�X�`���ǂݍ���
		try
		{
			auto wideFileName = std::wstring(m_textureName.begin(), m_textureName.end());
			auto context = D3D::Get()->GetContext();
			DirectX::CreateWICTextureFromFile(D3D::Get()->GetDevice(), nullptr, wideFileName.c_str(), nullptr, m_pTexture.ReleaseAndGetAddressOf());
			context->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
	}
}