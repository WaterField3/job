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
	};
}
