#pragma once

#include "Imgui/imgui.h"
namespace TMF
{
	class ApplicationState
	{
	public:
		ApplicationState() = default;
		~ApplicationState() = default;
		void Initialize();
		void Update();
		void DrawImGui();
		static ApplicationState& Instance()
		{
			static ApplicationState instance;
			return instance;
		}

		inline bool GetIsPause() const { return m_isPause; }
		inline bool GetIsPlay() const { return m_isPlay; }
		inline bool GetIsNextFrame() const { return m_isNextFrame; }

	private:
		bool m_isPlay = false;
		bool m_isPause = false;
		bool m_isNextFrame = false;

		ImVec4 normalColor = ImVec4(0.0f, 0.7f, 0.0f, 1.0f); // óŒêF
		//// í èÌÇÃêF
		//ImVec4 pressedColor = ImVec4(0.7f, 0.0f, 0.0f, 1.0f); // ê‘êF
	};
}
