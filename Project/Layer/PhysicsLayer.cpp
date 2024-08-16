#include "PhysicsLayer.h"
#include "GameObject/GameObjectManager.h"
#include "PhysicsManager.h"

#include "ApplicationState.h"
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
		PhysicsManager::Instance().Update();
	}

	void PhysicsLayer::OnDraw()
	{
		PhysicsManager::Instance().Draw();
	}
	bool PhysicsLayer::GetIsUpdate()
	{
		return ApplicationState::Instance().GetIsUpdate();
	}
}
