#pragma once

#include <unordered_map>
#include <functional>

namespace TMF
{
	class EventSystem
	{
	public:
		inline void AddHandler(char key, const std::function<void()>& handler) { m_handler[key] = handler; }
		void TriggerEvent(char key);

	private:
		std::unordered_map<char, std::function<void()>> m_handler;
	};
}

