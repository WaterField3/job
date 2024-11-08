#pragma once

#include <Imgui/imgui.h>
#include <memory>
#include <string>

namespace TMF
{
	class GameObject;
	class PlayButtonBar
	{
	public:
		PlayButtonBar() = default;
		virtual ~PlayButtonBar() = default;

		void DrawImGui(std::weak_ptr<GameObject> pObject);

	private:
		std::string m_saveObjectName;
		std::string m_loadObjectName;
		bool m_isDemoWindow = false;
		ImVec4 normalColor = ImVec4(0.0f, 0.7f, 0.0f, 1.0f); // óŒêF
	};
}
