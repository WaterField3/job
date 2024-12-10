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
		void CreateGameObject(std::string fileName, std::weak_ptr<GameObject> obj);
		void DestroyGameObject(GameObject* pGameObject);
		void Initialize();
		void Finalize();
		void Update();
		void LateUpdate();
		void Draw();
		void Save();
		void Load();
		void SaveTest();
		void LoadTest();
		void Save(std::string fileName);
		void Load(std::string fileName);
		void SaveObject(std::string fileName, std::weak_ptr<GameObject> obj);
		std::weak_ptr<GameObject> LoadObject(std::string fileName);

		std::vector<std::shared_ptr<GameObject>> GetGameObjects();
		std::vector<std::weak_ptr<GameObject>> GetGameObjects(int tag);
		std::weak_ptr<GameObject> GetGameObject(std::string name);

		template<typename TComponent>
		std::weak_ptr<TComponent> GetComponent()
		{
			for (auto gameobject : m_pGameObjects)
			{
				auto compoent = gameobject->GetComponent<TComponent>();
				if (!compoent.expired())
				{
					return compoent;
				}
			}
			std::weak_ptr<TComponent> nonPtr;
			return  nonPtr;
		}
		template<typename TComponent>
		std::vector<std::weak_ptr<TComponent>> GetComponents()
		{
			auto pComponents = std::vector<std::weak_ptr<TComponent>>();
			for (auto& pGameObject : m_pGameObjects)
			{
				auto pComponent = pGameObject->GetComponent<TComponent>();
				if (pComponent.expired() == false)
				{
					pComponents.push_back(pComponent);
				}
			}
			return pComponents;
		}

		static GameObjectManager& Instance()
		{
			static GameObjectManager instance;
			return instance;
		}

		SERIALIZE(m_pGameObjects);

	private:
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;

		std::string m_nowSceneName = "";
	};
}
