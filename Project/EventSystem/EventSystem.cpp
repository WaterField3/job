#include "EventSystem.h"

namespace TMF
{
	void EventSystem::TriggerEvent(char key)
	{
		if (m_handler.find(key) != m_handler.end())
		{
			m_handler[key]();
		}
	}

}
