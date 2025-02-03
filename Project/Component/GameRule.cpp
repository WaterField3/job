#include "GameRule.h"

#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "PlayerStatus.h"
#include "SceneLoad.h"
#include "Fade.h"

REGISTER_COMPONENT(TMF::GameRule, "GameRule");

namespace TMF
{
	void GameRule::OnInitialize()
	{
		auto playerObjects = GameObjectManager::Instance().GetGameObjects(static_cast<int>(GameObject::Tag::Player));
		auto size = playerObjects.size();
		for (auto i = 0; i < size; i++)
		{
			if (auto pLockPlayerObject = playerObjects[i].lock())
			{
				auto pPlayerStatus = pLockPlayerObject->GetComponent<PlayerStatus>();
				if (auto pLockPlayerStatus = pPlayerStatus.lock())
				{
					m_pPlayerStatuses.push_back(pLockPlayerStatus);
				}
			}
		}
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pSceneLoad = pLockOwner->GetComponent<SceneLoad>();
		}
		m_pFade = GameObjectManager::Instance().GetComponent<Fade>();
	}
	void GameRule::OnFinalize()
	{
	}
	void GameRule::OnUpdate()
	{
		auto size = m_pPlayerStatuses.size();
		if (size == 0)
		{
			return;
		}
		for (auto i = 0; i < size; i++)
		{
			if (auto pLockPlayerStatus = m_pPlayerStatuses[i].lock())
			{
				if (pLockPlayerStatus->GetIsDead() == true)
				{
					if (auto pLockSceneLoad = m_pSceneLoad.lock())
					{
						//pLockSceneLoad->Load();
						return;
					}
					if (auto pLockFade = m_pFade.lock())
					{
						pLockFade->FadeStart();
						return;
					}
				}
			}
		}
	}
	void GameRule::OnLateUpdate()
	{
	}
	void GameRule::OnDraw()
	{
	}
	void GameRule::OnDrawImGui()
	{
	}
	std::shared_ptr<Component> GameRule::OnClone() const
	{
		auto pClone = std::make_shared<GameRule>();
		return move(pClone);
	}
}
