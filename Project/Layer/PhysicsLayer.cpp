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

	}

	void PhysicsLayer::OnUpdate()
	{
		PhysicsManager::Instance().LateUpdate();
	}

	void PhysicsLayer::OnDraw()
	{
		PhysicsManager::Instance().Draw();
	}
}
