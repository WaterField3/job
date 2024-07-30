#include "GameObjectManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <fstream>

#include "GameObject/GameObject.h"
#include "Utility/CerealExtention.h"

namespace TMF
{
	std::weak_ptr<GameObject> GameObjectManager::CreateGameObject()
	{
		auto pGameObject = std::make_shared<GameObject>();
		pGameObject->Initialize();

		m_pGameObjects.push_back(pGameObject);

		return pGameObject;
	}

	void GameObjectManager::DestroyGameObject(GameObject* pGameObject)
	{
		int iterator = 0;

		for (auto& pObject : m_pGameObjects)
		{
			if (pObject.get() == pGameObject)
			{
				pObject->Finalize();
				m_pGameObjects.erase(m_pGameObjects.begin() + iterator);
			}
			else
			{
				iterator++;
			}
		}
	}

	void GameObjectManager::Initialize()
	{
		for (auto& pGameObject : m_pGameObjects)
		{
			pGameObject->Initialize();
		}
	}

	void GameObjectManager::Finalize()
	{
		for (auto& pGameObject : m_pGameObjects)
		{
			pGameObject->Finalize();
		}
	}

	void GameObjectManager::Update()
	{
		for (auto& pGameObject : m_pGameObjects)
		{
			pGameObject->Update();
		}
	}

	void GameObjectManager::LateUpdate()
	{
		for (auto& pGameObject : m_pGameObjects)
		{
			pGameObject->LateUpdate();
		}
	}

	void GameObjectManager::Draw()
	{
		for (auto& pGameObject : m_pGameObjects)
		{
			pGameObject->Draw();
		}
	}

	void GameObjectManager::Save(std::string fileName)
	{
		fileName = fileName + ".json";
		std::ofstream ss(fileName.c_str(), std::ios::out);
		{
			cereal::JSONOutputArchive oArchive(ss);
			oArchive(Instance());
		}
	}

	void GameObjectManager::Load(std::string fileName)
	{
		fileName = fileName + ".json";
		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			inArchive(GameObjectManager::Instance());
		}
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjects()
	{
		return m_pGameObjects;
	}
}
