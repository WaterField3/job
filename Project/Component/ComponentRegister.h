#pragma once

#include <string>

#include "Component/ComponentManager.h"

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

#define REGISTER_COMPONENT(componentName) \
namespace \
{ \
	TMF::ComponentRegister<componentName> g_##componentName##Register(#componentName); \
}