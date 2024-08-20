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
		// ���݂̎��Ԃ��擾
		auto currentTime = std::chrono::high_resolution_clock::now();

		// �O�̃t���[������̎��ԍ����v�Z�i�f���^�^�C���j
		deltaTime = currentTime - m_previousTime;
		m_previousTime = currentTime;
	}
}