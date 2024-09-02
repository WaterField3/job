#pragma once

#include <vector>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <cereal/cereal.hpp>
#include <optional>

#include "Component/Component.h"
#include "Utility/CerealExtention.h"
#include "Utility/CerealHelper.h"

namespace TMF
{
	class Transform;
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		~GameObject() = default;
		template <typename TComponent>
		std::weak_ptr<TComponent> AddComponent()
		{
			auto pComponent = std::make_shared<TComponent>();
			pComponent->Initialize(shared_from_this());

			m_pComponents.push_back(pComponent);

			return pComponent;
		}

		template <typename TComponent>
		void RemoveComponent(int index)
		{
			auto count = 0;
			std::erase_if(m_pComponents, [&count,index](std::shared_ptr<Component> pComponent)
				{
					count++;
					if (pComponent->IsRemovable() == false && count == index)
					{
						return false;
					}
					if (typeid(*pComponent) == typeid(TComponent) && index == count)
					{
						pComponent->Finalize();
						return true;
					}
					return false;
				});
		}
		template <typename TComponent>
		std::weak_ptr<TComponent> GetComponent()
		{
			for (auto& pComponent : m_pComponents)
			{
				if (auto result = std::dynamic_pointer_cast<TComponent>(pComponent))
				{
					return result;
				}
			}
			std::weak_ptr<TComponent> nonPtr;
			return  nonPtr;
		}

		void Initialize();
		void Finalize();
		void Update();
		void LateUpdate();
		void Draw();
		void DrawImGui();
		void CollisionEnter(GameObject* pGameObject);
		void CollisionStay(GameObject* pGameObject);
		void CollisionExit(GameObject* pGameObject);
		void TrigerEnter(GameObject* pGameObject);
		void TrigerStay(GameObject* pGameObject);
		void TrigerExit(GameObject* pGameObject);

		inline void SetName(std::string name) { m_name = name; }
		inline std::string GetName() const { return m_name; }
		inline std::string GetStrUUID() const { return boost::uuids::to_string(m_uuID); }

	private:
		std::vector<std::shared_ptr<Component>> m_pComponents;
		std::string m_name = "NewGameObject";
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
		std::string m_selectComponentName = "";
		std::weak_ptr<Transform> m_pTransform;
		int m_selectIndex = 0;
		bool m_isActive = true;

		SERIALIZE(m_name, m_uuID, m_pComponents);
	};
}
