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
