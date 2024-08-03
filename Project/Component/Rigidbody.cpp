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
			auto ColliderComponent = owner->GetComponent<Collider>();
			auto transformComponent = owner->GetComponent<Transform>();
			auto btPos = btVector3(0.0f, 0.0f, 0.0f);
			if (auto transform = transformComponent.lock())
			{
				auto pos = transform->GetPosition();
				btPos.setValue(pos.x, pos.y, pos.z);
			}

			if (auto collider = ColliderComponent.lock())
			{
				auto collisionShape = collider->GetCollisionShape();
				if (auto colShape = collisionShape.lock())
				{
					btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_pMotionState.get(), colShape.get(), btPos);
					m_pRigidBody = std::make_shared<btRigidBody>(rigidBodyCI);
					//PhysicsManager::Instance().AddRigidBody(m_pRigidBody);
				}
			}
		}
	}

	void Rigidbody::OnFinalize()
	{

	}

	void Rigidbody::OnUpdate()
	{

	}

	void Rigidbody::OnLateUpdate()
	{

	}

	void Rigidbody::OnDraw()
	{

	}

	void Rigidbody::OnDrawImGui()
	{
		auto label = LabelTransform("Mass");
		if (ImGui::DragFloat(label.c_str(), &m_mass, 0.1f))
		{

		}
		label = LabelTransform("Drag");
		if (ImGui::DragFloat(label.c_str(), &m_drag, 0.1f))
		{

		}
		label = LabelTransform("AngularDrag");
		if (ImGui::DragFloat(label.c_str(), &m_angularDrag, 0.1f))
		{

		}
	}
	std::string Rigidbody::LabelTransform(const char* labelName)
	{
		auto label = std::string(labelName);
		label += "## " + boost::uuids::to_string(m_uuID);
		return label;
	}
}
