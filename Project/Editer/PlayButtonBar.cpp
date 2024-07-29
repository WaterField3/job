#include "PlayButtonBar.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Utility/CerealExtention.h"

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
				GameObjectManager::Instance().Initialize();
				std::ofstream ss("test.json", std::ios::out);
				{
					cereal::JSONOutputArchive oArchive(ss);
					oArchive(GameObjectManager::Instance());
				}
			}
			else
			{
				GameObjectManager::Instance().Finalize();
				std::ifstream iS("test.json", std::ios::in);
				{
					cereal::JSONInputArchive inArchive(iS);
					inArchive(GameObjectManager::Instance());
				}
			}
			m_isPlay = !m_isPlay;
		}

		// ボタンの色を変更
		auto currentColor = m_isPause ? normalColor : pressedColor;
		ImGui::PushStyleColor(ImGuiCol_Button, currentColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, currentColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, currentColor);

		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			m_isPause = !m_isPause;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (ImGui::Button("NextFrame"))
		{
			m_isNextFrame = !m_isNextFrame;
		}
		ImGui::End();
		if (m_isPlay && !m_isPause)
		{
			GameObjectManager::Instance().Update();
			GameObjectManager::Instance().LateUpdate();
		}
		GameObjectManager::Instance().Draw();
	}
}
