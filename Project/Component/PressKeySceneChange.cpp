#include "PressKeySceneChange.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Fade.h"
#include "Input.h"

REGISTER_COMPONENT(TMF::PressKeySceneChange, "PressKeySceneChange");

namespace TMF
{
	void PressKeySceneChange::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pFade = pLockOwner->GetComponent<Fade>();
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
			if (auto pLockFade = m_pFade.lock())
			{
				pLockFade->FadeStart();
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
	std::shared_ptr<Component> PressKeySceneChange::OnClone() const
	{
		auto pClone = std::make_shared<PressKeySceneChange>();
		return move(pClone);
	}
}
