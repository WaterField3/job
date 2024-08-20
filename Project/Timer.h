#pragma once

#include <Windows.h>
#include <chrono>
#include <thread>

namespace TMF
{
	class Timer
	{
	public:
		Timer();
		~Timer();
		void Initialize();
		void Update();

		static Timer& Instance()
		{
			static Timer instance;
			return instance;
		}

		std::chrono::duration<float> deltaTime;
	private:
		// 前のフレームの終了時間を記憶する変数
		std::chrono::steady_clock::time_point m_previousTime;
	};
}
