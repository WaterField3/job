#pragma once

#include <SimpleMath.h>
#include <memory>

#include "MoveInfo.h"

namespace TMF
{
	class Transform;
	class Rigidbody;
	class Thruster;
	class Jump;
	class PlayerJump
	{
	public:
		PlayerJump();
		PlayerJump(std::weak_ptr<Transform> pTransform, std::weak_ptr<Rigidbody> pRigidbody, std::weak_ptr<Thruster> pThruster, std::weak_ptr<Jump> pJump);
		~PlayerJump();

		void Chage();
		void JumpSetting();
		void ChageEnd();
		void Flight();

		inline bool GetChageEnd() { return m_isChageEnd; }
		inline bool GetIsJumpingEnd() { return m_isJumpingEnd; }
	private:

		float m_chageTime = 0;
		float m_impact = 1.0f;
		float m_maxFlightTime = 2.0f;
		float m_flightTime = 1.0f;
		float m_maxMoveSpeed = 2.0f;
		float m_moveSpeed = 1.0f;
		float m_maxJumpForce = 7.0f;
		float m_maxChageTime = 1.0f;
		bool m_isChage = false;
		bool m_isChageEnd = false;
		bool m_isCharging = false;
		bool m_isJumping = false;
		bool m_isJumpingEnd = false;
		DirectX::SimpleMath::Vector3 m_jumpVector = DirectX::SimpleMath::Vector3::Zero;
		MoveDirection m_moveDirection = MoveDirection::NEUTRAL;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Thruster> m_pThruster;
		std::weak_ptr<Jump> pJump;

	};
}

