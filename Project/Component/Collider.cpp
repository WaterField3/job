#include "Collider.h"

#include <Imgui/imgui.h>

#include "Component/Transform.h"

namespace TMF
{
	REGISTER_COMPONENT(Collider);

	void Collider::OnInitialize()
	{
		MakeCollision();
	}
	void Collider::OnFinalize()
	{

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
		if (ImGui::BeginListBox("ColliderType"))
		{
			//for (int i = 0; i < (int)Collider_Type::MAX; i++)
			//{
			//	auto isSelected = i == (int)m_collidrType;
			//	auto enumName = 
			//	if (ImGui::Selectable(""))
			//}
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
}