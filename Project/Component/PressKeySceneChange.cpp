#include "PressKeySceneChange.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "SceneLoad.h"
#include "Input.h"

REGISTER_COMPONENT(TMF::PressKeySceneChange, "PressKeySceneChange");

namespace TMF
{
	void PressKeySceneChange::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pSceneLoad = pLockOwner->GetComponent<SceneLoad>();
		}
	}
	void PressKeySceneChange::OnFinalize()
	{

	}
	void PressKeySceneChange::OnUpdate()
	{
		auto keyState = Input::Instance().GetKeyState();
		auto keyTracker = Input::Instance().GetKeyboardTracker();
		keyTracker->Update(keyState);
		if (keyTracker->pressed.Enter == true)
		{
			if (auto pLockSceneLoad = m_pSceneLoad.lock())
			{
				pLockSceneLoad->Load();
			}
		}
	}
	void PressKeySceneChange::OnLateUpdate()
	{

	}
	void PressKeySceneChange::OnDraw()
	{

	}
	void PressKeySceneChange::OnDrawImGui()
	{

	}
}
