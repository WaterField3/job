#include "GhostObject.h"

#include "Collider.h"
#include "Transform.h"
#include "PhysicsManager.h"

REGISTER_COMPONENT(TMF::GhostObject, "GhostObject");

namespace TMF
{

	void GhostObject::OnInitialize()
	{
		m_pGhostObject = std::make_shared<btGhostObject>();
		if (auto owner = m_pOwner.lock())
		{
			auto transform = owner->GetComponent<Transform>();
			auto btPos = btVector3(0, 0, 0);
			auto btQua = btQuaternion(0, 0, 0, 1);
			if (auto trans = transform.lock())
			{
				auto pos = trans->GetPosition();
				btPos.setValue(pos.x, pos.y, pos.z);
				auto qua = trans->GetRotation();
				btQua.setValue(qua.x, qua.y, qua.z, qua.w);
			}
			btTransform btTrans(btQua, btPos);
			m_pGhostObject->setWorldTransform(btTrans);
			auto collider = owner->GetComponent<Collider>();
			if (auto col = collider.lock())
			{
				auto collisionShape = col->GetCollisionShape();
				if (auto shape = collisionShape.lock())
				{
					m_pGhostObject->setCollisionShape(shape.get());
					m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
					m_pGhostObject->setUserPointer(static_cast<void*>(owner.get()));
					PhysicsManager::Instance().AddGhostObject(m_pGhostObject);
				}
			}
		}
	}

	void GhostObject::OnFinalize()
	{
		RemoveGhostObject();
	}

	void GhostObject::OnUpdate()
	{

	}

	void GhostObject::OnLateUpdate()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto TransformComponent = owner->GetComponent<Transform>();
			if (auto transform = TransformComponent.lock())
			{
				auto btTrans = m_pGhostObject->getWorldTransform();
				auto Pos = DirectX::SimpleMath::Vector3{ btTrans.getOrigin().getX(),btTrans.getOrigin().getY(),btTrans.getOrigin().getZ() };
				transform->SetPosition(Pos);
			}
		}
	}

	void GhostObject::OnDraw()
	{
	}

	void GhostObject::OnDrawImGui()
	{

	}

	boost::uuids::uuid GhostObject::OnGetUUID()
	{
		return m_uuID;
	}

	void GhostObject::AddGhostObject(std::weak_ptr<btCollisionShape> col, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua)
	{
		auto btPos = btVector3(pos.x, pos.y, pos.z);
		auto btQua = btQuaternion(qua.x, qua.y, qua.z, qua.w);
		auto btTrans = btTransform(btQua, btPos);
		m_pGhostObject->setWorldTransform(btTrans);
		if (auto shape = col.lock())
		{
			m_pGhostObject->setCollisionShape(shape.get());
			m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			if (auto owner = m_pOwner.lock())
			{
				m_pGhostObject->setUserPointer(static_cast<void*>(owner.get()));
			}
			PhysicsManager::Instance().AddGhostObject(m_pGhostObject);
		}
	}

	void GhostObject::RemoveGhostObject()
	{
		if (m_pGhostObject)
		{
			PhysicsManager::Instance().RemoveGhostObject(m_pGhostObject);
		}
	}

	void GhostObject::SetGhostObjectTransform(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rotate)
	{
		auto btPos = btVector3(pos.x, pos.y, pos.z);
		auto btRot = btQuaternion(rotate.x, rotate.y, rotate.z, rotate.w);
		auto btTrans = btTransform(btRot, btPos);
		m_pGhostObject->setWorldTransform(btTrans);
	}

}