#pragma once
#include <Imgui/imgui.h>

namespace TMF
{
	class PlayButtonBar
	{
	public:
		PlayButtonBar() = default;
		virtual ~PlayButtonBar() = default;

		void DrawImGui();

	private:

		bool m_isDemoWindow = false;
		ImVec4 normalColor = ImVec4(0.0f, 0.7f, 0.0f, 1.0f); // óŒêF
	};
}
