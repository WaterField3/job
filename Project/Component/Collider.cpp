#include "Collider.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "GhostObject.h"
#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::Collider, "Collider");

namespace TMF
{

	void Collider::OnInitialize()
	{
		//AddRigidBody();
	}
	void Collider::OnFinalize()
	{
		auto owner = m_pOwner.lock();
		auto rigidBody = owner->GetComponent<Rigidbody>();
		if (auto rb = rigidBody.lock())
		{
			rb->RemoveRigidBody();
		}
		auto ghostObject = owner->GetComponent<GhostObject>();
		if (auto ghost = ghostObject.lock())
		{
			ghost->RemoveGhostObject();
		}
	}
	void Collider::OnUpdate()
	{

	}
	void Collider::OnLateUpdate()
	{

	}
	void Collider::OnDraw()
	{

	}
	void Collider::OnDrawImGui()
	{
		const char* types[] = { "Box","Capsule","Sphere","Cylinder","Cone" };
		static int selectIndex = (int)m_collidrType;
		if (ImGui::BeginCombo("ColliderType", types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_collidrType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_collidrType = Collider_Type(i);
					selectIndex = i;
					UpdateShapeInfo();

				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
			ImGui::SameLine();
		}

		auto sizeLabel = StringHelper::CreateLabel("Size", m_uuID);
		if (ImGui::DragFloat3(sizeLabel.c_str(), &m_size.x, 0.1f))
		{
			UpdateShapeInfo();
		}
		auto centerLabel = StringHelper::CreateLabel("Center", m_uuID);
		if (ImGui::DragFloat3(centerLabel.c_str(), &m_center.x, 0.1f))
		{
			UpdateShapeInfo();
		}

		if (ImGui::Button("Update"))
		{
			UpdateShapeInfo();
		}
	}

	void Collider::MakeCollision()
	{
		auto btscale = btVector3(1.0f, 1.0f, 1.0f);
		if (auto owner = m_pOwner.lock())
		{
			auto transformComponent = owner->GetComponent<Transform>();
			if (auto transform = transformComponent.lock())
			{
				auto scale = transform->GetScale();
				scale.x = std::abs(scale.x);
				scale.y = std::abs(scale.y);
				scale.z = std::abs(scale.z);
				scale *= m_size;
				btscale.setValue(scale.x, scale.y, scale.z);
			}
		}
		btScalar radius = btscale.getX();
		btScalar height = btscale.getY();
		switch (m_collidrType)
		{
		case TMF::Collider::Collider_Type::BOX:
			m_pCollisionShape = std::make_shared<btBoxShape>(btscale);
			break;
		case TMF::Collider::Collider_Type::CAPSULE:
			m_pCollisionShape = std::make_shared<btCapsuleShape>(radius, height);
			break;
		case TMF::Collider::Collider_Type::SPHERE:
			m_pCollisionShape = std::make_shared<btSphereShape>(radius);
			break;
		case TMF::Collider::Collider_Type::CYLINDER:
			m_pCollisionShape = std::make_shared<btCylinderShape>(btscale);
			break;
		case TMF::Collider::Collider_Type::CONE:
			m_pCollisionShape = std::make_shared<btConeShape>(radius, height);
			break;
		default:
			break;
		}
	}
	std::weak_ptr<btCollisionShape> Collider::GetCollisionShape()
	{
		if (m_pCollisionShape.get() == nullptr)
		{
			MakeCollision();
		}
		return m_pCollisionShape;

	}



	void Collider::UpdateShapeInfo()
	{
		AddRigidBody();
		AddGhostObject();
	}
	void Collider::AddRigidBody()
	{
		auto owner = m_pOwner.lock();
		auto transformComponent = owner->GetComponent<Transform>();
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		auto rotate = DirectX::SimpleMath::Quaternion::Identity;
		if (auto transform = transformComponent.lock())
		{
			pos = transform->GetPosition();
			pos += m_center;
			rotate = transform->GetRotation();
		}
		auto rigidBody = owner->GetComponent<Rigidbody>();
		if (auto rb = rigidBody.lock())
		{
			rb->RemoveRigidBody();
			MakeCollision();
			rb->AddRigidBody(m_pCollisionShape, pos, rotate);
		}
	}
	void Collider::AddGhostObject()
	{
		auto owner = m_pOwner.lock();
		auto transformComponent = owner->GetComponent<Transform>();
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		auto rotate = DirectX::SimpleMath::Quaternion::Identity;
		if (auto transform = transformComponent.lock())
		{
			pos = transform->GetPosition();
			pos += m_center;
			rotate = transform->GetRotation();
		}
		auto ghostObject = owner->GetComponent<GhostObject>();
		if (auto ghost = ghostObject.lock())
		{
			ghost->RemoveGhostObject();
			MakeCollision();
			ghost->AddGhostObject(m_pCollisionShape, pos, rotate);
		}
	}
}