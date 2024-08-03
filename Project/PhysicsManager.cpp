#include "PhysicsManager.h"

namespace TMF
{
	void PhysicsManager::Initialize()
	{

	}
	void PhysicsManager::Finalize()
	{
		for (auto& rigidBody : m_pRigidBodys)
		{
			if (auto rb = rigidBody.lock())
			{
				m_pDynamicsWorld->removeRigidBody(rb.get());
			}
		}
	}
	void PhysicsManager::Update()
	{

	}
	void PhysicsManager::LateUpdate()
	{

	}
	void PhysicsManager::Draw()
	{

	}
	void PhysicsManager::AddRigidBody(std::weak_ptr<btRigidBody> rigidBody)
	{
		if (auto rb = rigidBody.lock())
		{
			m_pRigidBodys.push_back(rigidBody);
			m_pDynamicsWorld->addRigidBody(rb.get());
		}
	}
}
