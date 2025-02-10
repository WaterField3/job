#include "Timer.h"

namespace TMF
{
	Timer::Timer()
	{
	}
	Timer::~Timer()
	{
	}
	void Timer::Initialize()
	{
		m_previousTime = std::chrono::high_resolution_clock::now();

	}
	void Timer::Update()
	{
		// 現在の時間を取得
		auto currentTime = std::chrono::high_resolution_clock::now();

		// 前のフレームからの時間差を計算（デルタタイム）
		deltaTime = currentTime - m_previousTime;
		m_previousTime = currentTime;

		fps = 1.0f / deltaTime.count();
	}
}