#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

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

		static PhysicsManager& Instance()
		{
			static PhysicsManager instance;
			return instance;
		}
	private:
		std::unique_ptr<btDiscreteDynamicsWorld> m_pDynamicsWorld;
		std::vector<std::weak_ptr<btRigidBody>> m_pRigidBodys;
	};
}
