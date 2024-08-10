#include "Collider.h"

#include <Imgui/imgui.h>

#include "Component/Transform.h"
#include "Component/Rigidbody.h"

namespace TMF
{
	REGISTER_COMPONENT(Collider);

	void Collider::OnInitialize()
	{
		AddRigidBody();
	}
	void Collider::OnFinalize()
	{
		auto owner = m_pOwner.lock();
		auto rigidBody = owner->GetComponent<Rigidbody>();
		if (auto rb = rigidBody.lock())
		{
			rb->RemoveRigidBody();
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

		if (ImGui::Button("Update"))
		{
			UpdateShapeInfo();

		}
	}
	boost::uuids::uuid Collider::OnGetUUID()
	{
		return m_uuID;
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
}