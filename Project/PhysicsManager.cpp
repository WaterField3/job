#include "PhysicsManager.h"

#include "bullet/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "direct3d.h"
#include "GameObject/GameObjectManager.h"
#include "Component/Camera.h"

namespace TMF
{
	void PhysicsManager::Initialize()
	{
		m_pBroadphaseInterface = std::make_unique<btDbvtBroadphase>();
		m_pCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		m_pCollisionDispacher = std::make_unique<MyBulletCollisionDispatcher>(m_pCollisionConfig.get());
		m_pConstrainSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_pCollisionDispacher.get(), m_pBroadphaseInterface.get(), m_pConstrainSolver.get(), m_pCollisionConfig.get());
		m_pBulletDebugDrawer = std::make_unique<BulletDebugDrawer>(D3D::Get()->GetContext());
		m_pDynamicsWorld->setDebugDrawer(m_pBulletDebugDrawer.get());
	}
	void PhysicsManager::Finalize()
	{

	}
	void PhysicsManager::Update()
	{
		m_pDynamicsWorld->stepSimulation(1 / 60.0f, 10);
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
	void PhysicsManager::AddRigidBody(std::weak_ptr<btRigidBody> pRigidBody)
	{
		if (auto rb = pRigidBody.lock())
		{
			m_pDynamicsWorld->addRigidBody(rb.get());
		}
	}
	void PhysicsManager::RemoveRigidBody(std::weak_ptr<btRigidBody> pRigidBody)
	{
		if (auto rb = pRigidBody.lock())
		{
			m_pDynamicsWorld->removeRigidBody(rb.get());
		}
	}

	void PhysicsManager::AddGhostObject(std::weak_ptr<btGhostObject> pGhostObject)
	{
		if (auto ghost = pGhostObject.lock())
		{
			m_pDynamicsWorld->addCollisionObject(ghost.get());
		}
	}
	void PhysicsManager::RemoveGhostObject(std::weak_ptr<btGhostObject> pGhostObject)
	{
		if (auto ghost = pGhostObject.lock())
		{
			m_pDynamicsWorld->removeCollisionObject(ghost.get());
		}
	}

	void PhysicsManager::Reset()
	{
		m_pDynamicsWorld->getBroadphase()->resetPool(m_pDynamicsWorld->getDispatcher());
		m_pDynamicsWorld->getConstraintSolver()->reset();
		m_pCollisionDispacher->Reset();
	}

}
