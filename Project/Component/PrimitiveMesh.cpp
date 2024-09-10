#include "PrimitiveMesh.h"

#include "Imgui/imgui.h"

#include "direct3d.h"
#include "ComponentRegister.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::PrimitiveMesh, "PrimitiveMesh");

namespace TMF
{
	void PrimitiveMesh::OnInitialize()
	{
		MakeMesh();
	}
	void PrimitiveMesh::MakeMesh()
	{
		DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		if (auto pOwner = m_pOwner.lock())
		{
			auto pTransform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto transformSize = pLockTransform->GetScale();
				size = DirectX::XMFLOAT3(transformSize.x, transformSize.y, transformSize.z);
			}
		}
		auto context = D3D::Get()->GetContext();
		switch (m_shapeType)
		{
		case TMF::PrimitiveMesh::CUBE:
			m_pShape = DirectX::GeometricPrimitive::CreateCube(context, size.x);
			break;
		case TMF::PrimitiveMesh::SPHER:
			m_pShape = DirectX::GeometricPrimitive::CreateSphere(context, size.x, size_t(16));
			break;
		case TMF::PrimitiveMesh::CYLINDER:
			m_pShape = DirectX::GeometricPrimitive::CreateCylinder(context, size.y, size.x);
			break;
		case TMF::PrimitiveMesh::CONE:
			m_pShape = DirectX::GeometricPrimitive::CreateCone(context, size.y, size.x);
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
			if (auto pLockCamera = pCamera.lock())
			{
				auto view = pLockCamera->GetViewMatrix();
				auto proj = pLockCamera->GetProjectionMatrix();
				m_pShape->Draw(world, view, proj, m_color);
			}
		}
	}
	void PrimitiveMesh::OnDrawImGui()
	{
		const char* types[] = { "Box","Sphere","Cylinder","Cone"};
		static int selectIndex = (int)m_shapeType;
		if (ImGui::BeginCombo("ColliderType", types[selectIndex]))
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
		
		auto colorLabel = StringHelper::CreateLabel("Color", m_uuID);
		if (ImGui::DragFloat4(colorLabel.c_str(), &m_color.x, 0.1f, 0.0f, 1.0f))
		{

		}
	}
}