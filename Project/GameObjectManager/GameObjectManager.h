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
		template <typename TGameObject>
		std::weak_ptr<TGameObject> AddGameObject()
		{
			auto pGameObject = std::make_shared<TGameObject>();

			m_pGameObjects.push_back(pGameObject);

			return pGameObject;
		}

		template <typename TGameObject>
		void RemoveGameObject()
		{
			int iterator = 0;

			for (auto& gameObject : m_pGameObjects)
			{
				if (typeid(*gameObject) == typeid(TGameObject))
				{
					m_pGameObjects.erase(m_pGameObjects.begin() + iterator);
				}
				else
				{
					iterator++;
				}
			}
		}

	private:
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
	};
}

