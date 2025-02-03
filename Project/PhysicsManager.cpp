#include "PhysicsManager.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "direct3d.h"
#include "GameObject/GameObjectManager.h"
#include "Component/Camera.h"
#include "Component/FreeCamera.h"

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
		m_pDynamicsWorld->performDiscreteCollisionDetection();
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
		auto compoent = GameObjectManager::Instance().GetComponent<Camera>();
		if (auto camera = compoent.lock())
		{
			D3D::Get()->SettingEffect(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		}
		auto collisionObjects = m_pDynamicsWorld->getCollisionObjectArray();
		if (collisionObjects.size() > 0)
		{
#if _DEBUG
			m_pDynamicsWorld->debugDrawWorld();
			m_pBulletDebugDrawer->Render();
#endif // _DEBUG
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

	bool PhysicsManager::RayCastHit(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end)
	{
		auto btStart = btVector3(start.x, start.y, start.z);
		auto btEnd = btVector3(end.x, end.y, end.z);
		// RayResultCallback �I�u�W�F�N�g���쐬���āA���C�L���X�g���ʂ�ۑ�
		btCollisionWorld::ClosestRayResultCallback rayCallback(btStart, btEnd);

		// ���C�L���X�e�B���O�����s
		m_pDynamicsWorld->rayTest(btStart, btEnd, rayCallback);

		return rayCallback.hasHit();
	}

}
