#include "PlayButtonBar.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Utility/CerealExtention.h"
#include "System/DataFileNames.h"
#include "PhysicsManager.h"
#include "ApplicationState.h"

namespace TMF
{
	void PlayButtonBar::DrawImGui()
	{
		auto m_ButtonName = "Play";

		auto isPlay = ApplicationState::Instance().GetIsPlay();
		auto isPause = ApplicationState::Instance().GetIsPause();

		// プレイボタン押したときのセーブをApplicationStateに移す

		ImGui::Begin("PlayButtonBar");
		if (isPlay)
		{
			m_ButtonName = "Stop";
		}
		if (ImGui::Button(m_ButtonName))
		{
			if (!isPlay)
			{
				GameObjectManager::Instance().Save(TEST_DATA);
			}
			else
			{
				PhysicsManager::Instance().AllRemoveRigidBody();
				GameObjectManager::Instance().Finalize();
				GameObjectManager::Instance().Load(TEST_DATA);
				GameObjectManager::Instance().Initialize();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			if (isPlay)
			{
				GameObjectManager::Instance().Save(MAIN_DATA);
				GameObjectManager::Instance().Finalize();
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("DemoWindow", &m_isDemoWindow);
		if (m_isDemoWindow)
		{
			ImGui::ShowDemoWindow();
		}
		ImGui::End();

	}
}
