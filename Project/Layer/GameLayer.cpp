#include "GameLayer.h"

#include "GameObject/GameObjectManager.h"
#include "ApplicationState.h"
#include "EffectManager.h"
#include "AudioManager.h"

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
		EffectManager::Instance().Update();
		AudioManager::Instance().Update();
	}

	void GameLayer::OnLateUpdate()
	{
		
	}

	void GameLayer::OnDraw()
	{
		GameObjectManager::Instance().Draw();
		EffectManager::Instance().Draw();
	}
	bool GameLayer::GetIsUpdate()
	{
		return ApplicationState::Instance().GetIsUpdate();
		//return true;
	}
}
