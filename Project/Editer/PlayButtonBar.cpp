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
	void PlayButtonBar::DrawImGui(std::weak_ptr<GameObject> pObject)
	{
		auto isPlay = ApplicationState::Instance().GetIsPlay();
		auto isPause = ApplicationState::Instance().GetIsPause();
		auto isNextFrame = ApplicationState::Instance().GetIsNextFrame();
		auto m_ButtonName = "Play";
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
				GameObjectManager::Instance().Finalize();
				PhysicsManager::Instance().Reset();
				GameObjectManager::Instance().Load(TEST_DATA);
				GameObjectManager::Instance().Initialize();
			}
			ApplicationState::Instance().SetIsPlay(!isPlay);
		}
		ImGui::SameLine();
		if (isPause)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.4f, 1.0f });
		}
		auto temp = ImGui::Button("Pause");

		if (temp)
		{
			ApplicationState::Instance().SetIsPause(!isPause);
		}
		ImGui::PopStyleColor(1);
		ImGui::SameLine();
		if (isNextFrame)
		{
			ApplicationState::Instance().SetIsNextFrame(false);
		}
		if (ImGui::Button("NextFrame"))
		{
			if (isPause)
			{
				ApplicationState::Instance().SetIsNextFrame(true);
			}
		}

		if (ImGui::Button("Save"))
		{
			if (!isPlay)
			{
				GameObjectManager::Instance().Save(MAIN_DATA);
			}
		}
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_saveObjectName.c_str());
		if (ImGui::InputText("saveName", buf, 256))
		{
			m_saveObjectName = buf;
		}
		if (ImGui::Button("ObjectSave"))
		{
			if (!isPlay)
			{
				GameObjectManager::Instance().SaveObject(m_saveObjectName, pObject);
			}
		}
		ImGui::Checkbox("DemoWindow", &m_isDemoWindow);
		if (m_isDemoWindow)
		{
			ImGui::ShowDemoWindow();
		}
		ImGui::End();

	}
}
