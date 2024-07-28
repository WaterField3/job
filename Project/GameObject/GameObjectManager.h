#pragma once

#include <vector>
#include <memory>
#include <cereal/cereal.hpp>

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

		std::vector<std::shared_ptr<GameObject>> GetGameObjects();

		static GameObjectManager& Instance() 
		{
			static GameObjectManager instance;
			return instance;
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(m_pGameObjects));
		}

	private:
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
	};
}
