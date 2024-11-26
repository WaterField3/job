#pragma once

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

		bool GetIsUpdate();
		inline bool GetIsPlay() const { return m_isPlay; }
		inline bool GetIsPause() const { return m_isPause; }
		inline bool GetIsNextFrame() const { return m_isNextFrame; }
		inline bool GetIsDrawDebug() { return m_isDrawDebug; }
		inline void SetIsPlay(bool set) { m_isPlay = set; }
		inline void SetIsPause(bool set) { m_isPause = set; }
		inline void SetIsNextFrame(bool set) { m_isNextFrame = set; }
		inline void SetIsDrawDebug(bool set) { m_isDrawDebug = set; }

	private:
		bool m_isPlay = false;
		bool m_isPause = false;
		bool m_isNextFrame = false;
		bool m_isDrawDebug = true;
		//// í èÌÇÃêF
		//ImVec4 pressedColor = ImVec4(0.7f, 0.0f, 0.0f, 1.0f); // ê‘êF
	};
}
