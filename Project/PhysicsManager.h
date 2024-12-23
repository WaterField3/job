#pragma once

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <map>

#include "Utility/BulletDebugDrawer.h"
#include "MyBulletCollisionDispatcher.h"

class btGhostObject;
namespace TMF
{
	class PhysicsManager
	{
	public:
		void Initialize();
		void Finalize();
		void Update();
		void Draw();
		void AddRigidBody(std::weak_ptr<btRigidBody> pRigidBody);
		void RemoveRigidBody(std::weak_ptr<btRigidBody> pRigidBody);
		void AddGhostObject(std::weak_ptr<btGhostObject> pGhostObject);
		void RemoveGhostObject(std::weak_ptr<btGhostObject> pGhostObject);
		void Reset();
		bool RayCastHit(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end);

		static PhysicsManager& Instance()
		{
			static PhysicsManager instance;
			return instance;
		}
	private:
		std::unique_ptr<btBroadphaseInterface> m_pBroadphaseInterface;
		std::unique_ptr<btDefaultCollisionConfiguration> m_pCollisionConfig;
		std::unique_ptr<MyBulletCollisionDispatcher> m_pCollisionDispacher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_pConstrainSolver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_pDynamicsWorld;
		std::unique_ptr<BulletDebugDrawer> m_pBulletDebugDrawer;
		std::map<std::pair<const btCollisionObject*, const btCollisionObject*>, bool> m_contactPairsMap;
	};
}
