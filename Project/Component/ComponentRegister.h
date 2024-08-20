#pragma once

#include <string>
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "ComponentManager.h"

namespace TMF
{
	template <typename TComponent>
	class ComponentRegister
	{
	public:
		ComponentRegister(std::string name)
		{
			ComponentManager::Instance().RegisterComponent<TComponent>(name);
		}
		virtual ~ComponentRegister() = default;
	};
}

// AddComponent���V�X�e���ɓo�^(cpp��namespace�̊O�ɏ����Ă�������)
#define REGISTER_COMPONENT(componentName, addComponentName) \
namespace \
{ \
	inline TMF::ComponentRegister<componentName> g_componentNameRegister(addComponentName); \
} \
CEREAL_REGISTER_TYPE(componentName); \
CEREAL_REGISTER_POLYMORPHIC_RELATION(TMF::Component, componentName);