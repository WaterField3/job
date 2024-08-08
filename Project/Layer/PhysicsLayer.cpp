#include "PhysicsLayer.h"
#include "GameObject/GameObjectManager.h"
#include "PhysicsManager.h"

namespace TMF
{
	void PhysicsLayer::OnInitialize()
	{
	}

	void PhysicsLayer::OnFinalize()
	{
		PhysicsManager::Instance().AllRemoveRigidBody();
	}

	void PhysicsLayer::OnUpdate()
	{
		PhysicsManager::Instance().Update();
	}

	void PhysicsLayer::OnDraw()
	{
		PhysicsManager::Instance().Draw();
	}
}
