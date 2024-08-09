#include "ApplicationState.h"

#include "GameObject/GameObjectManager.h"
#include "PhysicsManager.h"
#include "System/DataFileNames.h"

namespace TMF
{
	void ApplicationState::Initialize()
	{

	}
	void ApplicationState::Update()
	{

	}
	void ApplicationState::DrawImGui()
	{

	}
	bool ApplicationState::GetIsUpdate()
	{
		return (m_isPlay && !m_isPause) || (m_isPlay && m_isNextFrame);
	}
}