#include "ComponentManager.h"

#include "GameObject/GameObject.h"

namespace TMF
{
	void ComponentManager::AddComponent(std::string name, std::weak_ptr<GameObject> pGameObject)
	{
		auto it = m_addComponentMap.find(name);
		if (it != m_addComponentMap.end())
		{
			it->second(pGameObject);
		}
	}
}
