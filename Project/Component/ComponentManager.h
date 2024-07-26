#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>

#include "GameObject/GameObject.h"

namespace TMF
{
	class ComponentManager
	{
	public:
		template <typename TComponent>
		void RegisterComponent(std::string name)
		{
			m_componentName.push_back(name);
			m_addComponentMap.emplace(name, [&](std::weak_ptr<GameObject> pGameObject) {AddComponent<TComponent>(pGameObject); });
		}

		static ComponentManager& Instance()
		{
			static ComponentManager instance;
			return instance;
		}

		void AddComponent(std::string name, std::weak_ptr<GameObject> pGameObject);
		inline std::vector<std::string> GetComponentNames() const { return m_componentName; }

	private:
		template <typename TComponent>
		void AddComponent(std::weak_ptr<GameObject> pGameObject)
		{
			if (auto pObject = pGameObject.lock())
			{
				pObject->AddComponent<TComponent>();
			}
		}
		
	private:
		std::vector<std::string> m_componentName;
		std::map<std::string, std::function<void(std::weak_ptr<GameObject>)>> m_addComponentMap;
	};
}
