#include "Rigidbody.h"

#include <Imgui/imgui.h>

#include "Collider.h"
#include "Transform.h"
#include "PhysicsManager.h"
#include "GhostObject.h"

REGISTER_COMPONENT(TMF::Rigidbody, "Rigidbody");

using namespace DirectX::SimpleMath;

namespace TMF
{
	void Rigidbody::OnInitialize()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto transform = owner->GetComponent<Transform>();
			auto pos = Vector3::Zero;
			auto qua = Quaternion::Identity;
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
					m_pRigidBody->setSleepingThresholds(m_linearSleepingThresholds, m_angulerSleepingThresholds);
					//m_pRigidBody->setActivationState(DISABLE_DEACTIVATION);
					m_pRigidBody->setAngularFactor(MakebtVector3(Vector3(m_isAngularFactorX, m_isAngularFactorY, m_isAngularFactorZ)));
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
			auto pos = Vector3::Zero;
			auto rotate = Quaternion::Identity;
			if (auto transform = transformComponent.lock())
			{
				btTransform trans;
				m_pRigidBody->getMotionState()->getWorldTransform(trans);
				pos = Vector3{ trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ() };
				transform->SetPosition(pos);
				rotate = transform->GetRotation();
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
		label = LabelChange("LinearSleepingThresholds");
		if (ImGui::DragFloat(label.c_str(), &m_linearSleepingThresholds, 0.1f))
		{

		}
		label = LabelChange("AngulerSleepingThresholds");
		if (ImGui::DragFloat(label.c_str(), &m_angulerSleepingThresholds, 0.1f))
		{

		}
		label = LabelChange("");
		ImGui::LabelText(label.c_str(),"%s", "AngularFactor");
		label = LabelChange("X");
		if (ImGui::Checkbox(label.c_str(), &m_isAngularFactorX))
		{

		}
		label = LabelChange("Y");
		ImGui::SameLine(45);
		if (ImGui::Checkbox(label.c_str(), &m_isAngularFactorY))
		{

		}
		label = LabelChange("Z");
		ImGui::SameLine(80);
		if (ImGui::Checkbox(label.c_str(), &m_isAngularFactorZ))
		{

		}
		//if (ImGui::Button("a"))
		//{
		// //sleep‚©‚ç–ß‚ç‚È‚¢‚©‚çactive‚É
		////m_pRigidBody->activate();
		//	m_pRigidBody->applyImpulse(btVector3(0, 50, 0), btVector3(0, 0, 0));
		//}
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

	void Rigidbody::ApplyCentralForce(DirectX::SimpleMath::Vector3 force)
	{
		m_pRigidBody->applyCentralForce(MakebtVector3(force));
	}

	void Rigidbody::SetLinearVelocity(DirectX::SimpleMath::Vector3 velocity)
	{
		m_pRigidBody->setLinearVelocity(MakebtVector3(velocity));
	}

	void Rigidbody::ApplyImpulse(DirectX::SimpleMath::Vector3 impulse, DirectX::SimpleMath::Vector3 relPos)
	{
		m_pRigidBody->applyImpulse(MakebtVector3(impulse), MakebtVector3(relPos));
	}

	void Rigidbody::ApplyForce(DirectX::SimpleMath::Vector3 force, DirectX::SimpleMath::Vector3 relPos)
	{
		m_pRigidBody->applyForce(MakebtVector3(force), MakebtVector3(Vector3::Zero));
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
	btTransform Rigidbody::MakebtTransform(Vector3 vec, Quaternion qua)
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
