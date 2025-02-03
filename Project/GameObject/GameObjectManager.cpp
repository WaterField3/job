#include "GameObjectManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <fstream>
#include <filesystem>

#include "GameObject/GameObject.h"
#include "Utility/CerealExtention.h"
#include "Component/Transform.h"
#include "Utility/Log.h"

namespace TMF
{
	std::weak_ptr<GameObject> GameObjectManager::CreateGameObject()
	{
		auto pGameObject = std::make_shared<GameObject>();
		pGameObject->AddComponent<Transform>();
		pGameObject->Initialize();

		m_pGameObjects.push_back(pGameObject);

		return pGameObject;
	}
	// jsonからオブジェクトを生成
	void GameObjectManager::CreateGameObject(std::string fileName, std::weak_ptr<GameObject> obj)
	{
		fileName = fileName + ".json";

		if (!std::filesystem::is_regular_file(fileName))
		{
			return;
		}
		auto pGameObject = std::make_shared<GameObject>();

		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			try
			{
				inArchive(pGameObject);
			}
			catch (const std::exception& e)
			{
				Log::Info(e.what());
			}
		}

		pGameObject->AddComponent<Transform>();
		pGameObject->Initialize();

		m_pGameObjects.push_back(pGameObject);
	}

	void GameObjectManager::CreateGameObject(std::weak_ptr<GameObject> obj)
	{
		if (auto pLockObj = obj.lock())
		{
			auto pGameObject = std::make_shared<GameObject>(pLockObj);
			pGameObject->Initialize();

			m_pGameObjects.push_back(pGameObject);
		}
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
		for (auto index = 0; index < m_pGameObjects.size(); index++)
		{
			m_pGameObjects[index]->Update();
		}
		//for (auto& pGameObject : m_pGameObjects)
		//{
		//	if (m_isLoaded == true)
		//	{
		//		return;
		//	}
		//	pGameObject->Update();
		//}

	}

	void GameObjectManager::LateUpdate()
	{

		for (auto& pGameObject : m_pGameObjects)
		{
			if (m_isLoaded == true)
			{
				return;
			}
			if (pGameObject->GetActive())
			{

				pGameObject->LateUpdate();
			}
		}

	}

	void GameObjectManager::Draw()
	{
		if (m_isLoaded == true)
		{
			m_isLoaded = false;
		}
		else
		{
			for (auto& pGameObject : m_pGameObjects)
			{
				if (pGameObject->GetActive())
				{
					pGameObject->Draw();
				}
			}
		}
	}

	void GameObjectManager::Save()
	{
		auto fileName = m_nowSceneName + ".json";
		std::ofstream ss(fileName.c_str(), std::ios::out);
		{
			cereal::JSONOutputArchive oArchive(ss);
			oArchive(Instance());
		}
	}

	void GameObjectManager::Load()
	{
		auto fileName = m_nowSceneName + ".json";
		if (!std::filesystem::is_regular_file(fileName))
		{
			return;
		}
		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			inArchive(GameObjectManager::Instance());
			m_isLoaded = true;
		}
	}

	void GameObjectManager::SaveTest()
	{
		auto fileName = m_nowSceneName + "Test.json";
		std::ofstream ss(fileName.c_str(), std::ios::out);
		{
			cereal::JSONOutputArchive oArchive(ss);
			oArchive(Instance());
		}
	}

	void GameObjectManager::LoadTest()
	{
		auto fileName = m_nowSceneName + "Test.json";
		if (!std::filesystem::is_regular_file(fileName))
		{
			return;
		}
		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			inArchive(GameObjectManager::Instance());
			//m_isLoaded = true;
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
		m_nowSceneName = fileName;
		fileName = fileName + ".json";
		if (!std::filesystem::is_regular_file(fileName))
		{
			return;
		}
		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			inArchive(GameObjectManager::Instance());
			//m_isLoaded = true;
		}
	}

	void GameObjectManager::SaveObject(std::string fileName, std::weak_ptr<GameObject> obj)
	{
		fileName = fileName + ".json";
		std::ofstream ss(fileName.c_str(), std::ios::out);
		{
			cereal::JSONOutputArchive oArchive(ss);
			oArchive(obj);
		}
	}

	void GameObjectManager::SaveObjects(std::string fileName)
	{
		fileName = fileName + ".json";
		std::ofstream ss(fileName.c_str(), std::ios::out);
		{
			cereal::JSONOutputArchive oArchive(ss);
			oArchive(m_pGameObjects);
		}
	}

	void GameObjectManager::LoadObjects(std::string fileName)
	{
		fileName = fileName + ".json";
		if (!std::filesystem::is_regular_file(fileName))
		{
			return;
		}
		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			inArchive(m_pGameObjects);
		}
	}

	std::weak_ptr<GameObject> GameObjectManager::LoadObject(std::string fileName)
	{
		fileName = fileName + ".json";
		auto pGameObject = std::make_shared<GameObject>();

		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			try
			{
				inArchive(pGameObject);
				m_isLoaded = true;
			}
			catch (const std::exception& e)
			{
				Log::Info(e.what());
			}
		}

		pGameObject->Initialize();

		m_pGameObjects.push_back(pGameObject);

		return pGameObject;
	}


	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjects()
	{
		return m_pGameObjects;
	}
	std::vector<std::weak_ptr<GameObject>> GameObjectManager::GetGameObjects(int tag)
	{
		auto pGameObjects = std::vector<std::weak_ptr<GameObject>>();
		if (tag < int(GameObject::Tag::Max) && 0 <= tag)
		{
			for (auto& pGameObject : m_pGameObjects)
			{
				if (pGameObject->GetTag() == GameObject::Tag(tag))
				{
					pGameObjects.push_back(pGameObject);
				}
			}
		}
		return pGameObjects;
	}
	std::weak_ptr<GameObject> GameObjectManager::GetGameObject(std::string name)
	{
		for (auto& pGameObject : m_pGameObjects)
		{
			if (pGameObject->GetName() == name)
			{
				return pGameObject;
			}
		}
		return std::weak_ptr<GameObject>();
	}
}
