#include "Rigidbody.h"

#include <Imgui/imgui.h>

#include "Collider.h"
#include "Transform.h"
#include "PhysicsManager.h"
#include "GhostObject.h"

namespace TMF
{
	REGISTER_COMPONENT(Rigidbody);

	void Rigidbody::OnInitialize()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto transform = owner->GetComponent<Transform>();
			auto pos = DirectX::SimpleMath::Vector3::Zero;
			auto qua = DirectX::SimpleMath::Quaternion::Identity;
			if (auto trans = transform.lock())
			{
				pos = trans->GetPosition();
				qua = trans->GetRotation();
			}
			auto collider = owner->GetComponent<Collider>();
			if (auto col = collider.lock())
			{
				auto inertia = btVector3(pos.x, pos.y, pos.z);
				auto colShape = col->GetCollisionShape();
				if (auto usefullColShape = colShape.lock())
				{
					usefullColShape->calculateLocalInertia(m_mass, inertia);
					m_pMotionState = std::make_unique<btDefaultMotionState>(MakebtTransform(pos, qua));
					btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_pMotionState.get(), usefullColShape.get(), inertia);
					m_pRigidBody = std::make_shared<btRigidBody>(rigidBodyCI);
					if (auto owner = m_pOwner.lock())
					{
						m_pRigidBody->setUserPointer(owner.get());
					}
					PhysicsManager::Instance().AddRigidBody(m_pRigidBody);
				}
			}
		}
	}

	void Rigidbody::OnFinalize()
	{
		RemoveRigidBody();
	}

	void Rigidbody::OnUpdate()
	{

	}

	void Rigidbody::OnLateUpdate()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto transformComponent = owner->GetComponent<Transform>();
			auto pos = DirectX::SimpleMath::Vector3::Zero;
			auto rotate = DirectX::SimpleMath::Quaternion::Identity;
			if (auto transform = transformComponent.lock())
			{
				btTransform trans;
				m_pRigidBody->getMotionState()->getWorldTransform(trans);
				pos = DirectX::SimpleMath::Vector3{ trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ() };
				transform->SetPosition(pos);
				rotate = transform->GetRotation();
			}
			auto ghostComponent = owner->GetComponent<GhostObject>();
			if (auto ghost = ghostComponent.lock())
			{
				ghost->SetGhostObjectTransform(pos, rotate);
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

	boost::uuids::uuid Rigidbody::OnGetUUID()
	{
		return m_uuID;
	}

	void Rigidbody::SetRigidBodyTranform(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua)
	{
		auto trans = btTransform(MakebtQuaternion(qua), MakebtVector3(pos));
		m_pRigidBody->setWorldTransform(trans);
	}

	void Rigidbody::RemoveRigidBody()
	{
		if (m_pRigidBody)
		{
			PhysicsManager::Instance().RemoveRigidBody(m_pRigidBody);
		}
	}



	void Rigidbody::AddRigidBody(std::weak_ptr<btCollisionShape> col, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua)
	{
		if (auto colShape = col.lock())
		{
			auto inertia = btVector3(pos.x, pos.y, pos.z);
			colShape->calculateLocalInertia(m_mass, inertia);
			m_pMotionState = std::make_unique<btDefaultMotionState>(MakebtTransform(pos, qua));
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_pMotionState.get(), colShape.get(), inertia);
			m_pRigidBody = std::make_shared<btRigidBody>(rigidBodyCI);
			if (auto owner = m_pOwner.lock())
			{
				m_pRigidBody->setUserPointer(static_cast<void*>(owner.get()));
			}
			PhysicsManager::Instance().AddRigidBody(m_pRigidBody);
		}
	}

	btVector3 Rigidbody::MakebtVector3(DirectX::SimpleMath::Vector3 vec)
	{
		auto btVec = btVector3(vec.x, vec.y, vec.z);
		return btVec;
	}

	btQuaternion Rigidbody::MakebtQuaternion(DirectX::SimpleMath::Quaternion qua)
	{
		auto btQua = btQuaternion(qua.x, qua.y, qua.z, qua.w);
		return btQua;
	}
	btTransform Rigidbody::MakebtTransform(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Quaternion qua)
	{
		return btTransform(MakebtQuaternion(qua), MakebtVector3(vec));
	}
	std::string Rigidbody::LabelChange(const char* labelName)
	{
		auto label = std::string(labelName);
		label += "## " + boost::uuids::to_string(m_uuID);
		return label;
	}
}
