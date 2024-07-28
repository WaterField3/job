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

// AddComponent���V�X�e���ɓo�^(cpp�ɏ����Ă�������)
#define REGISTER_COMPONENT(componentName) \
namespace \
{ \
	inline TMF::ComponentRegister<componentName> g_##componentName##Register(#componentName); \
}

// Cereal�ɃR���|�[�l���g��o�^(h�ɏ����Ă�������)
#define REGISTER_CEREAL_TYPE(componentName) \
CEREAL_REGISTER_TYPE(componentName); \
CEREAL_REGISTER_POLYMORPHIC_RELATION(TMF::Component, componentName);