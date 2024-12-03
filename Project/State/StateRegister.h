#pragma once

#include <string>

#include "StateManager.h"

namespace TMF
{
	template <typename TState>
	class StateRegister
	{
	public:
		StateRegister(std::string name)
		{
			StateManager::Instance().RgisterState<TState>(name);
		}
		virtual ~StateRegister() = default;
	};
}

// CreateState���V�X�e���ɓo�^(cpp��namespace�̊O�ɏ����Ă�������)
#define REGISTER_STATE(stateName, addStateName) \
namespace \
{ \
	inline TMF::StateRegister<stateName> g_stateNameRegister(addStateName); \
}