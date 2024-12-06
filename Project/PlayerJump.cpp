#include "PlayerJump.h"

#include "Component/Rigidbody.h"
#include "Component/Transform.h"
#include "Component/Thruster.h"
#include "Input.h"
#include "Timer.h"

namespace TMF
{
	void PlayerJump::Chage()
	{
		// チャージ済みなら行わない
		if (m_isChageEnd == true)
		{
			return;
		}
		if (auto pLockThruster = m_pThruster.lock())
		{
			// スタミナが無い状態なら行わない
			if (pLockThruster->GetIsOverHeat() == true)
			{
				return;
			}
		}

		m_chageTime += Timer::Instance().deltaTime.count();
		if (m_chageTime >= 1)
		{
			m_chageTime = 1.0f;
			m_isChageEnd = true;
			m_flightTime = m_maxFlightTime;
			m_moveSpeed = m_maxMoveSpeed;
			auto keyState = Input::Instance().GetKeyState();
			if (keyState.W)
			{
				m_moveDirection = MoveDirection::FOWARD;
			}
			else if (keyState.A)
			{
				m_moveDirection = MoveDirection::LEFT;
			}
			else if (keyState.S)
			{
				m_moveDirection = MoveDirection::BACK;
			}
			else if (keyState.D)
			{
				m_moveDirection = MoveDirection::RIGHT;
			}
		}
	}
	void PlayerJump::Flight()
	{
		if (m_chageTime != 0 && m_isChageEnd == true)
		{
			return;
		}

		m_chageTime -= Timer::Instance().deltaTime.count();


	}
}