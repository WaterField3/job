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

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(m_name));
			try
			{
				archive(CEREAL_NVP(m_uuID));
			}
			catch (const std::exception&)
			{

			}
		}

		void Initialize();
		void Finalize();
		void Update();
		void LateUpdate();
		void Draw();
		void DrawImGui();

		inline void SetName(std::string name) { m_name = name; }
		inline std::string GetName() const { return m_name; }
		inline std::string GetStrUUID() const { return boost::uuids::to_string(m_uuID); }

	private:
		std::vector<std::shared_ptr<Component>> m_pComponents;
		std::string m_name = "NewGameObject";
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
	};
}