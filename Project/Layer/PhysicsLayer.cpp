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
		GameObjectManager::Instance().LateUpdate();
		PhysicsManager::Instance().Update();
	}

	void PhysicsLayer::OnDraw()
	{
		PhysicsManager::Instance().Draw();
	}
	bool PhysicsLayer::GetIsUpdate()
	{
		return ApplicationState::Instance().GetIsUpdate();
		//return true;
	}
}
