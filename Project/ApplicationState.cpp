#include "ApplicationState.h"

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
		auto m_ButtonName = "Play";
		ImGui::Begin("PlayButtonBar");
		if (m_isPlay)
		{
			m_ButtonName = "Stop";
		}
		if (ImGui::Button(m_ButtonName))
		{
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
		ImGui::End();
	}
}