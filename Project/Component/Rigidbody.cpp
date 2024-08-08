#include "Rigidbody.h"

#include <Imgui/imgui.h>

#include "Component/Collider.h"
#include "Component/Transform.h"
#include "PhysicsManager.h"

namespace TMF
{
	REGISTER_COMPONENT(Rigidbody);

	void Rigidbody::OnInitialize()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto colliderComponent = owner->GetComponent<Collider>();
			if (auto collider = colliderComponent.lock())
			{
				auto collisionShape = collider->GetCollisionShape();
				if (auto colShape = collisionShape.lock())
				{
					auto inertia = btVector3(0.0f, 0.0f, 0.0f);
					colShape->calculateLocalInertia(m_mass, inertia);
					m_pMotionState = std::make_unique<btDefaultMotionState>(TransfomPosToBtTransform());
					btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_pMotionState.get(), colShape.get(), inertia);
					m_pRigidBody = std::make_shared<btRigidBody>(rigidBodyCI);
					PhysicsManager::Instance().AddRigidBody(m_pRigidBody);
				}
			}
		}
	}

	void Rigidbody::OnFinalize()
	{
		PhysicsManager::Instance().RemoveRigidBody(m_pRigidBody);
	}

	void Rigidbody::OnUpdate()
	{

	}

	void Rigidbody::OnLateUpdate()
	{

		if (auto owner = m_pOwner.lock())
		{
			auto TransformComponent = owner->GetComponent<Transform>();
			if (auto transform = TransformComponent.lock())
			{
				btTransform trans;
				m_pRigidBody->getMotionState()->getWorldTransform(trans);
				auto Pos = DirectX::SimpleMath::Vector3{ trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ() };
				transform->SetPosition(Pos);
			}
		}
	}

	void Rigidbody::OnDraw()
	{

	}

	void Rigidbody::OnDrawImGui()
	{
		auto label = LabelChange("Mass");
		if (ImGui::DragFloat(label.c_str(), &m_mass, 0.1f))
		{

		}
		label = LabelChange("Drag");
		if (ImGui::DragFloat(label.c_str(), &m_drag, 0.1f))
		{

		}
		label = LabelChange("AngularDrag");
		if (ImGui::DragFloat(label.c_str(), &m_angularDrag, 0.1f))
		{

		}
	}

	btTransform Rigidbody::TransfomPosToBtTransform()
	{
		auto btPos = btVector3(0.0f, 0.0f, 0.0f);
		auto btQuotainon = btQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
		if (auto owner = m_pOwner.lock())
		{
			auto transformComponent = owner->GetComponent<Transform>();
			if (auto transform = transformComponent.lock())
			{
				auto pos = transform->GetPosition();
				btPos.setValue(pos.x, pos.y, pos.z);
				auto rotate = transform->GetRotation();
				btScalar x = rotate.x;
				btScalar y = rotate.y;
				btScalar z = rotate.z;
				btScalar w = rotate.w;
				btQuotainon.setValue(x, y, z, w);
			}
		}
		return btTransform(btQuotainon, btPos);
	}
	std::string Rigidbody::LabelChange(const char* labelName)
	{
		auto label = std::string(labelName);
		label += "## " + boost::uuids::to_string(m_uuID);
		return label;
	}
}
