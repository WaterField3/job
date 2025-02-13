#include "Rigidbody.h"

#include <Imgui/imgui.h>

#include "Collider.h"
#include "Transform.h"
#include "PhysicsManager.h"
#include "GhostObject.h"
#include "Input.h"
#include "Utility/Log.h"

REGISTER_COMPONENT(TMF::Rigidbody, "Rigidbody");

using namespace DirectX::SimpleMath;

namespace TMF
{
	void Rigidbody::OnInitialize()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto transform = owner->GetComponent<Transform>();
			auto pos = DirectX::SimpleMath::Vector3::Zero;
			auto qua = Quaternion::Identity;
			if (auto trans = transform.lock())
			{
				pos = trans->GetWorldPosition();
				qua = trans->GetRotation();
			}
			auto collider = owner->GetComponent<Collider>();
			if (auto col = collider.lock())
			{

				auto colShape = col->GetCollisionShape();
				auto inertia = btVector3(0, 0, 0);

				if (auto usefullColShape = colShape.lock())
				{
					usefullColShape->calculateLocalInertia(m_mass, inertia);
					m_pMotionState = std::make_unique<btDefaultMotionState>(MakebtTransform(pos, qua));
					auto  rigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(m_mass, m_pMotionState.get(), usefullColShape.get(), inertia);
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
		RemoveRigidbody();
	}

	void Rigidbody::OnUpdate()
	{

	}

	void Rigidbody::OnLateUpdate()
	{
		if (m_pRigidBody)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pTransform = pLockOwner->GetComponent<Transform>();
				auto pos = Vector3::Zero;
				auto rotate = Quaternion::Identity;
				auto pColl = pLockOwner->GetComponent<Collider>();

				if (auto pLockTransform = pTransform.lock())
				{
					btTransform trans;
					m_pRigidBody->getMotionState()->getWorldTransform(trans);
					//m_pRigidBody->setCenterOfMassTransform(trans);
					pos = Vector3{ trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ() };
					pLockTransform->SetPosition(pos);
					rotate = Quaternion(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW());
					pLockTransform->SetRotation(rotate);

				}
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
		ImGui::LabelText(label.c_str(), "%s", "AngularFactor");
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

	std::shared_ptr<Component> Rigidbody::OnClone() const
	{
		auto pClone = std::make_shared<Rigidbody>();
		pClone->m_mass = this->m_mass;
		pClone->m_drag = this->m_drag;
		pClone->m_angularDrag = this->m_angularDrag;
		pClone->m_angulerSleepingThresholds = this->m_angulerSleepingThresholds;
		pClone->m_linearSleepingThresholds = this->m_linearSleepingThresholds;
		pClone->m_isAngularFactorX = this->m_isAngularFactorX;
		pClone->m_isAngularFactorY = this->m_isAngularFactorY;
		pClone->m_isAngularFactorZ = this->m_isAngularFactorZ;
		return move(pClone);
	}

	void Rigidbody::SetRigidBodyTranform(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua)
	{
		auto trans = btTransform(MakebtQuaternion(qua), MakebtVector3(pos));
		m_pRigidBody->setWorldTransform(trans);
	}

	void Rigidbody::RemoveRigidbody()
	{
		if (m_pRigidBody)
		{
			PhysicsManager::Instance().RemoveRigidBody(m_pRigidBody);
		}
	}



	void Rigidbody::AddRigidBody(std::weak_ptr<btCollisionShape> col, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua)
	{
		if (auto pLockColliderShape = col.lock())
		{
			auto inertia = btVector3(pos.x, pos.y, pos.z);
			pLockColliderShape->calculateLocalInertia(m_mass, inertia);
			m_pMotionState = std::make_unique<btDefaultMotionState>(MakebtTransform(pos, qua));
			auto rigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(m_mass, m_pMotionState.get(), pLockColliderShape.get(), inertia);
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

	void Rigidbody::SetLinearVelocity(btVector3 velocity)
	{
		m_pRigidBody->setLinearVelocity(velocity);
	}

	DirectX::SimpleMath::Vector3 Rigidbody::GetLinearVelocity()
	{
		auto velocity = m_pRigidBody->getLinearVelocity();
		return DirectX::SimpleMath::Vector3(velocity.getX(), velocity.getY(), velocity.getZ());
	}

	void Rigidbody::ApplyImpulse(DirectX::SimpleMath::Vector3 impulse, DirectX::SimpleMath::Vector3 relPos)
	{
		m_pRigidBody->applyImpulse(MakebtVector3(impulse), MakebtVector3(relPos));
	}

	void Rigidbody::ApplyForce(DirectX::SimpleMath::Vector3 force, DirectX::SimpleMath::Vector3 relPos)
	{
		m_pRigidBody->applyForce(MakebtVector3(force), MakebtVector3(relPos));
	}

	void Rigidbody::ApplyTorque(DirectX::SimpleMath::Vector3 torque)
	{
		m_pRigidBody->applyTorque(MakebtVector3(torque));
	}

	void Rigidbody::SetAngularVelocity(DirectX::SimpleMath::Vector3 velocity)
	{
		m_pRigidBody->setAngularVelocity(MakebtVector3(velocity));
	}

	void Rigidbody::SetAngularVelocity(btVector3 velocity)
	{
		m_pRigidBody->setAngularVelocity(velocity);
	}

	void Rigidbody::SetRotation(DirectX::SimpleMath::Vector3 rotation)
	{
		auto pLockOwner = m_pOwner.lock();
		auto pTransform = pLockOwner->GetComponent<Transform>();
		btTransform btTrans;
		if (auto pLockTransform = pTransform.lock())
		{
			m_pRigidBody->getMotionState()->getWorldTransform(btTrans);
			auto btQua = btQuaternion(MakebtVector3(rotation), SIMD_PI / 2);
			btTrans.setRotation(btQua);
			m_pRigidBody->getMotionState()->setWorldTransform(btTrans);
		}
	}

	void Rigidbody::SetRotation(DirectX::SimpleMath::Quaternion rotation)
	{
		auto pLockOwner = m_pOwner.lock();
		auto pTransform = pLockOwner->GetComponent<Transform>();
		btTransform btTrans;
		if (auto pLockTransform = pTransform.lock())
		{
			m_pRigidBody->getMotionState()->getWorldTransform(btTrans);
			auto btQua = MakebtQuaternion(rotation);
			btTrans.setRotation(btQua);
			m_pRigidBody->getMotionState()->setWorldTransform(btTrans);
		}
	}

	void Rigidbody::SetRotation(btQuaternion rotation)
	{
		auto pLockOwner = m_pOwner.lock();
		auto pTransform = pLockOwner->GetComponent<Transform>();
		btTransform btTrans;
		if (auto pLockTransform = pTransform.lock())
		{
			m_pRigidBody->getMotionState()->getWorldTransform(btTrans);
			btTrans.setRotation(rotation);
			m_pRigidBody->getMotionState()->setWorldTransform(btTrans);
		}
	}

	void Rigidbody::GetTotalTorque()
	{
		auto torque = m_pRigidBody->getTotalTorque();
		Log::Info("%d", torque.getX());
		Log::Info("%d", torque.getY());
		Log::Info("%d", torque.getZ());
	}

	void Rigidbody::ClearForces()
	{
		//m_pRigidBody->clearForces();
		m_pRigidBody->setDamping(0.0f, 1.0f);
	}

	void Rigidbody::GetAngularFactor()
	{
		auto factor = m_pRigidBody->getAngularFactor();
		Log::Info("X : %d", factor.getX());
		Log::Info("Y : %d", factor.getY());
		Log::Info("Z : %d", factor.getZ());
	}

	void Rigidbody::SetAngularFactor(DirectX::SimpleMath::Vector3 angular)
	{
		m_pRigidBody->setAngularFactor(MakebtVector3(angular));
		m_isAngularFactorX = angular.x;
		m_isAngularFactorY = angular.y;
		m_isAngularFactorZ = angular.z;
	}

	void Rigidbody::GetNowbtTransform()
	{
		btTransform btTrans;
		m_pRigidBody->getMotionState()->getWorldTransform(btTrans);
	}

	void Rigidbody::SetBtTransform(btTransform setBtTransform)
	{
		m_pRigidBody->getMotionState()->setWorldTransform(setBtTransform);
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto rotation = Quaternion(setBtTransform.getRotation().getX(), setBtTransform.getRotation().getY(), setBtTransform.getRotation().getZ(), setBtTransform.getRotation().getW());
				pLockTransform->SetRotation(rotation);

			}
		}
	}

	btTransform Rigidbody::GetBtTransform()
	{
		btTransform btTrans;
		m_pRigidBody->getMotionState()->getWorldTransform(btTrans);
		return btTrans;
	}

	void Rigidbody::SetCenterOfMassTransform(btTransform btTransform)
	{
		m_pRigidBody->setCenterOfMassTransform(btTransform);
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
