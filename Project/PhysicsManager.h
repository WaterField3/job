#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

#include "Utility/BulletDebugDrawer.h"

namespace TMF
{
	class PhysicsManager
	{
	public:
		void Initialize();
		void Finalize();
		void Update();
		void LateUpdate();
		void Draw();
		void AddRigidBody(std::weak_ptr<btRigidBody> rigidBody);
		void RemoveRigidBody(std::weak_ptr<btRigidBody> rigidBody);
		void AllRemoveRigidBody();

		static PhysicsManager& Instance()
		{
			static PhysicsManager instance;
			return instance;
		}
	private:
		std::unique_ptr<btBroadphaseInterface> m_pBroadphaseInterface;
		std::unique_ptr<btDefaultCollisionConfiguration> m_pCollisionConfig;
		std::unique_ptr<btCollisionDispatcher> m_pCollisionDispacher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_pConstrainSolver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_pDynamicsWorld;
		std::unique_ptr<BulletDebugDrawer> m_pBulletDebugDrawer;
	};
}
