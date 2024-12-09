#include "PlayerJump.h"

#include "Component/Rigidbody.h"
#include "Component/Transform.h"
#include "Component/Thruster.h"
#include "Component/Jump.h"
#include "Input.h"
#include "Timer.h"
#include <cmath>

namespace TMF
{
	PlayerJump::PlayerJump()
	{

	}
	PlayerJump::PlayerJump(std::weak_ptr<Transform> pTransform, std::weak_ptr<Rigidbody> pRigidbody, std::weak_ptr<Thruster> pThruster, std::weak_ptr<Jump> pJump)
	{
		m_pTransform = pTransform;
		m_pRigidbody = pRigidbody;
		m_pThruster = pThruster;
		if (auto pLockJump = pJump.lock())
		{
			m_impact = pLockJump->GetImpact();
			m_moveSpeed = pLockJump->GetMoveSpeed();
			m_maxFlightTime = pLockJump->GetMaxFlightTime();
			m_maxMoveSpeed = pLockJump->GetMaxMoveSpeed();
		}
		m_jumpVector = DirectX::SimpleMath::Vector3::Up;
	}
	PlayerJump::~PlayerJump()
	{

	}
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
		m_isCharging = true;
		m_isJumpingEnd = false;
		if (m_chageTime >= 1)
		{
			m_chageTime = 1.0f;
			m_isChageEnd = true;
			JumpSetting();
		}
	}
	void PlayerJump::JumpSetting()
	{
		m_flightTime = m_maxFlightTime;
		m_isJumping = true;
		m_isCharging = false;
		auto keyState = Input::Instance().GetKeyState();
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (keyState.W)
			{
				m_jumpVector = pLockTransform->GetForward();
			}
			else if (keyState.A)
			{
				m_jumpVector = pLockTransform->GetLeft();
			}
			else if (keyState.S)
			{
				m_jumpVector = pLockTransform->GetBack();
			}
			else if (keyState.D)
			{
				m_jumpVector = pLockTransform->GetRight();
			}

		}
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->UseThruster(m_chageTime);
		}
	}
	void PlayerJump::ChageEnd()
	{
		if (m_isCharging == true)
		{
			m_isJumpingEnd = false;
			JumpSetting();
		}
	}
	void PlayerJump::Flight()
	{
		if (m_isJumping == false)
		{
			return;
		}

		//m_chageTime -= Timer::Instance().deltaTime.count();

		m_flightTime -= 0.1f;
		if (m_flightTime <= 0)
		{
			m_flightTime = m_maxFlightTime;
			m_isChageEnd = false;
			m_chageTime = 0.0f;
			m_isJumping = false;
			m_isJumpingEnd = true;
			return;
		}
		if (auto pLockRigidbody = m_pRigidbody.lock())
		{
			auto jumpForce = float(std::lerp(0, m_maxJumpForce, m_chageTime / m_maxChageTime));
			auto velocity = m_jumpVector;

			velocity.y = jumpForce;
			pLockRigidbody->SetLinearVelocity(velocity);
		}
	}
}