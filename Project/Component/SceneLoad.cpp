#include "SceneLoad.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Fade.h"
#include "GameObject/GameObjectManager.h"
#include "PhysicsManager.h"

REGISTER_COMPONENT(TMF::SceneLoad, "SceneLoad");

namespace TMF
{
	void SceneLoad::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			switch (m_trigger)
			{
			case TMF::SceneLoad::FADEIN:
			case TMF::SceneLoad::FADEOUT:
				m_pFade = pLockOwner->GetComponent<Fade>();
				break;
			default:
				break;
			}
		}
	}
	void SceneLoad::OnFinalize()
	{
	}
	void SceneLoad::OnUpdate()
	{
		switch (m_trigger)
		{
		case TMF::SceneLoad::FADEIN:
			if (auto pLockFade = m_pFade.lock())
			{
				if (pLockFade->GetIsFadeInEnd() == true)
				{
					Load();
				}
			}
			break;
		case TMF::SceneLoad::FADEOUT:
			if (auto pLockFade = m_pFade.lock())
			{
				if (pLockFade->GetIsFadeOutEnd() == true)
				{
					Load();
				}
			}
			break;
		default:
			break;
		}
	}
	void SceneLoad::OnLateUpdate()
	{
	}
	void SceneLoad::OnDraw()
	{
	}
	void SceneLoad::OnDrawImGui()
	{
		auto loadFileLabel = StringHelper::CreateLabel("LoadFile", m_uuID);
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_loadFile.c_str());
		if (ImGui::InputText(loadFileLabel.c_str(), buf, 256))
		{
			m_loadFile = buf;
		}

		auto triggerTypeLabel = StringHelper::CreateLabel("Trigger", m_uuID);
		const char* types[] = { "FadeIn", "FadeOut"};
		int selectIndex = (int)m_trigger;
		if (ImGui::BeginCombo(triggerTypeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_trigger == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_trigger = SceneChangeTrigger(i);
					selectIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	void SceneLoad::Load()
	{
		GameObjectManager::Instance().Finalize();
		PhysicsManager::Instance().Reset();
		GameObjectManager::Instance().Load(m_loadFile);
		GameObjectManager::Instance().Initialize();
	}
}