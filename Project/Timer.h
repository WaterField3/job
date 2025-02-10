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
		float fps = 0.0f;
	private:
		// �O�̃t���[���̏I�����Ԃ��L������ϐ�
		std::chrono::steady_clock::time_point m_previousTime;
	};
}
