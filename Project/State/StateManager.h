#pragma once

#include <string>
#include <map>
#include <functional>
#include <typeindex>
#include <memory>

#include "State.h"

namespace TMF
{
	class StateManager
	{
	public:
	template <typename TState>
		void RgisterState(std::string name)
		{
			m_createStateMap.emplace(name, [this]() { return CreateState<TState>(); });
		}

		static StateManager& Instance()
		{
			static StateManager instance;
			return instance;
		}

		std::unique_ptr<State> GetState(std::string name)
		{
			auto it = m_createStateMap.find(name);
			if (it != m_createStateMap.end())
			{
				auto state = it->second();
				return std::move(state);
			}
			return std::move(std::unique_ptr<State>());
		}

	private:
		template <typename TState>
		std::unique_ptr<TState> CreateState()
		{
			auto pState = std::make_unique<TState>();
			if (pState)
			{
				return std::move(pState);
			}

			return std::move(std::unique_ptr<TState>());
		}
		std::map<std::string, std::function<std::unique_ptr<State>()>> m_createStateMap;


	};
}


