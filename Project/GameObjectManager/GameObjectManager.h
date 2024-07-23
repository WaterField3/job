#pragma once

#include <vector>
#include <memory>

#include "typeinfo"

namespace TMF
{
	class GameObject;

	class GameObjectManager
	{
	public:

		std::weak_ptr<GameObject> CreateGameObject();
		void DestroyGameObject(GameObject* pGameObject);
		void Update();
		void LateUpdate();
		void Draw();

		static GameObjectManager& Instance() 
		{
			static GameObjectManager instance;
			return instance;
		}

	private:
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
	};
}

