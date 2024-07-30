#include "PlayButtonBar.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Utility/CerealExtention.h"
#include "System/DataFileNames.h"

namespace TMF
{
	void PlayButtonBar::DrawImGui()
	{
		auto m_ButtonName = "Play";

		ImGui::Begin("PlayButtonBar");
		if (m_isPlay)
		{
			m_ButtonName = "Stop";
		}
		if (ImGui::Button(m_ButtonName))
		{
			if (!m_isPlay)
			{
				GameObjectManager::Instance().Save(TEST_DATA);
			}
			else
			{
				GameObjectManager::Instance().Load(TEST_DATA);
				GameObjectManager::Instance().Initialize();
			}
			m_isPlay = !m_isPlay;
		}

		ImGui::SameLine();
		if (m_isPause)
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
			m_isPause = !m_isPause;
		}
		ImGui::PopStyleColor(1);
		ImGui::SameLine();
		if (m_isNextFrame)
		{
			m_isNextFrame = false;
		}
		if (ImGui::Button("NextFrame"))
		{
			if (m_isPause)
			{
				m_isNextFrame = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			if (!m_isPlay)
			{
				GameObjectManager::Instance().Finalize();
				GameObjectManager::Instance().Save(MAIN_DATA);
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("DemoWindow", &m_isDemoWindow);
		if (m_isDemoWindow)
		{
			ImGui::ShowDemoWindow();
		}
		ImGui::End();
		if ((m_isPlay && !m_isPause) || (m_isPlay && m_isNextFrame))
		{
			GameObjectManager::Instance().Update();
			GameObjectManager::Instance().LateUpdate();
		}
		GameObjectManager::Instance().Draw();
	}
}
