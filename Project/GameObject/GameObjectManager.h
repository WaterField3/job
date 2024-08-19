#pragma once

#include <vector>
#include <memory>
#include <cereal/cereal.hpp>
#include <typeinfo>
#include <string>

#include "Utility/CerealHelper.h"

namespace TMF
{
	class GameObject;
	class GameObjectManager
	{
	public:

		std::weak_ptr<GameObject> CreateGameObject();
		void DestroyGameObject(GameObject* pGameObject);
		void Initialize();
		void Finalize();
		void Update();
		void LateUpdate();
		void Draw();
		void Save(std::string fileName);
		void Load(std::string fileName);

		std::vector<std::shared_ptr<GameObject>> GetGameObjects();
		std::weak_ptr<GameObject> GetGameObject(std::string name);

		template < typename TComponent>
		std::weak_ptr<TComponent> GetComponent(std::string name)
		{
			for (auto gameobject : m_pGameObjects)
			{
				if (gameobject->GetName() == name)
				{
					return gameobject->GetComponent<TComponent>();
				}
			}
			std::weak_ptr<TComponent> nonPtr;
			return  nonPtr;
		}

		static GameObjectManager& Instance()
		{
			static GameObjectManager instance;
			return instance;
		}

		SERIALIZE(m_pGameObjects);

	private:
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
	};
}
