#pragma once

#include <vector>
#include <memory>
#include <typeinfo>
#include <iostream>

#include "Component/Component.h"

namespace TMF
{
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
	public:
		template <typename TComponent>
		std::weak_ptr<TComponent> AddComponent()
		{
			auto pComponent = std::make_shared<TComponent>();
			pComponent->Initialize(shared_from_this());

			m_pComponents.push_back(pComponent);

			return pComponent;
		}

		template <typename TComponent>
		void RemoveComponent()
		{
			int iterator = 0;

			for (auto& component : m_pComponents)
			{
				if (typeid(*component) == typeid(TComponent))
				{
					m_pComponents.erase(m_pComponents.begin() + iterator);
				}
				else
				{
					iterator++;
				}
			}
		}
		template <typename TComponent>
		std::weak_ptr<TComponent> GetComponent()
		{
			for (auto& component : m_pComponents)
			{
				if (typeid(*component) == typeid(TComponent))
				{
					return std::dynamic_pointer_cast<TComponent>(component);
				}
			}
		}

		void Initialize();
		void Finalize();
		void Update();
		void Draw();
		void LateUpdate();

	private:
		std::vector<std::shared_ptr<Component>> m_pComponents;
	};
}