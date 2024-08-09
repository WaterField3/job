#include "GameLayer.h"

#include "GameObject/GameObjectManager.h"
#include "ApplicationState.h"

namespace TMF
{
	void GameLayer::OnInitialize()
	{
		GameObjectManager::Instance().Initialize();
	}

	void GameLayer::OnFinalize()
	{
		GameObjectManager::Instance().Finalize();
	}

	void GameLayer::OnUpdate()
	{
		GameObjectManager::Instance().Update();
	}

	void GameLayer::OnLateUpdate()
	{
		GameObjectManager::Instance().LateUpdate();
	}

	void GameLayer::OnDraw()
	{
		GameObjectManager::Instance().Draw();
	}
	bool GameLayer::GetIsUpdate()
	{
		return ApplicationState::Instance().GetIsUpdate();
	}
}
