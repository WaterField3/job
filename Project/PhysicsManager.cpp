#include "PhysicsManager.h"

#include "direct3d.h"

namespace TMF
{
	void PhysicsManager::Initialize()
	{
		m_pBroadphaseInterface = std::make_unique<btDbvtBroadphase>();
		m_pCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		m_pCollisionDispacher = std::make_unique<btCollisionDispatcher>(m_pCollisionConfig.get());
		m_pConstrainSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_pCollisionDispacher.get(), m_pBroadphaseInterface.get(), m_pConstrainSolver.get(), m_pCollisionConfig.get());
		m_pBulletDebugDrawer = std::make_unique<BulletDebugDrawer>(D3D::Get()->GetDevice(), D3D::Get()->GetContext(), D3D::Get()->GetInputLayout());
		m_pDynamicsWorld->setDebugDrawer(m_pBulletDebugDrawer.get());
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
		m_pDynamicsWorld->stepSimulation(1 / 60.0f);

	}
	void PhysicsManager::Draw()
	{
		m_pDynamicsWorld->debugDrawWorld();
		m_pBulletDebugDrawer->Render(D3D::Get()->GetDevice(), D3D::Get()->GetContext(), D3D::Get()->GetInputLayout());
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
