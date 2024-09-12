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
		GameObjectManager::Instance().LateUpdate();
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
