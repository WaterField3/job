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
		auto isPlay = ApplicationState::Instance().GetIsPlay();
		auto isPause = ApplicationState::Instance().GetIsPause();
		auto isNextFrame = ApplicationState::Instance().GetIsNextFrame();
		if ((isPlay && !isPause) || (isPlay && isNextFrame))
		{
			GameObjectManager::Instance().Update();
		}
	}

	void GameLayer::OnLateUpdate()
	{
		auto isPlay = ApplicationState::Instance().GetIsPlay();
		auto isPause = ApplicationState::Instance().GetIsPause();
		auto isNextFrame = ApplicationState::Instance().GetIsNextFrame();
		if ((isPlay && !isPause) || (isPlay && isNextFrame))
		{
			GameObjectManager::Instance().LateUpdate();
		}
	}

	void GameLayer::OnDraw()
	{
		GameObjectManager::Instance().Draw();
	}
}
