#include "PhysicsManager.h"

#include "direct3d.h"
#include "GameObject/GameObjectManager.h"
#include "Component/Camera.h"

namespace TMF
{
	void PhysicsManager::Initialize()
	{
		m_pBroadphaseInterface = std::make_unique<btDbvtBroadphase>();
		m_pCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		m_pCollisionDispacher = std::make_unique<btCollisionDispatcher>(m_pCollisionConfig.get());
		m_pConstrainSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_pCollisionDispacher.get(), m_pBroadphaseInterface.get(), m_pConstrainSolver.get(), m_pCollisionConfig.get());
		m_pBulletDebugDrawer = std::make_unique<BulletDebugDrawer>( D3D::Get()->GetContext());
		m_pDynamicsWorld->setDebugDrawer(m_pBulletDebugDrawer.get());
	}
	void PhysicsManager::Finalize()
	{

	}
	void PhysicsManager::Update()
	{
		m_pDynamicsWorld->stepSimulation(1 / 60.0f,10);
		auto num = m_pCollisionDispacher.get()->getNumManifolds();
		for (auto i = 0; i < num; i++)
		{
			auto contactManifold = m_pCollisionDispacher.get()->getManifoldByIndexInternal(i);
			auto object1 = contactManifold->getBody0();
			auto gameObject1 = static_cast<GameObject*>(object1->getUserPointer());
			auto object2 = contactManifold->getBody1();
			auto gameObject2 = static_cast<GameObject*>(object1->getUserPointer());
			auto numContacts = contactManifold->getNumContacts();
			for (auto j = 0; j < numContacts; j++)
			{
				auto point = contactManifold->getContactPoint(j);
				if (point.getDistance() < 0.0f)
				{
					auto nomal = point.m_normalWorldOnB;
					auto impulse = point.getAppliedImpulse();
				}
			}

		}
	}

	void PhysicsManager::Draw()
	{
		auto compoent = GameObjectManager::Instance().GetComponent<Camera>("CameraObject");
		if (auto camera = compoent.lock())
		{
			D3D::Get()->SettingEffect(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		}
		auto CollisionObjects = m_pDynamicsWorld->getCollisionObjectArray();
		if (CollisionObjects.size() > 0)
		{
			m_pDynamicsWorld->debugDrawWorld();
			m_pBulletDebugDrawer->Render();
		}
	}
	void PhysicsManager::AddRigidBody(std::weak_ptr<btRigidBody> rigidBody)
	{
		if (auto rb = rigidBody.lock())
		{
			m_pDynamicsWorld->addRigidBody(rb.get());
		}
	}
	void PhysicsManager::RemoveRigidBody(std::weak_ptr<btRigidBody> rigidBody)
	{
		if (auto rb = rigidBody.lock())
		{
			m_pDynamicsWorld->removeRigidBody(rb.get());
		}
	}
	void PhysicsManager::AllRemoveRigidBody()
	{
		auto collisionObjects = m_pDynamicsWorld->getCollisionObjectArray();
		auto size = collisionObjects.size();
		for (int i = 0; i < size; i++)
		{
			auto collisionObject = collisionObjects[i];
			auto rigidBody = btRigidBody::upcast(collisionObject);
			m_pDynamicsWorld->removeRigidBody(rigidBody);
		}
	}
}
