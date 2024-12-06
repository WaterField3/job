#pragma once

#include <SimpleMath.h>
#include <memory>

#include "MoveInfo.h"

namespace TMF
{
	class Transform;
	class Rigidbody;
	class Thruster;
	class PlayerJump
	{
	public:

		void Chage();
		void Flight();
	private:

		float m_chageTime = 0;
		float m_impact = 1.0f;
		float m_maxFlightTime = 10.0f;
		float m_flightTime = 1.0f;
		float m_maxMoveSpeed = 2.0f;
		float m_moveSpeed = 1.0f;
		bool m_isChage = false;
		bool m_isChageEnd = false;
		DirectX::SimpleMath::Vector3 m_jumpVector = DirectX::SimpleMath::Vector3::Zero;
		MoveDirection m_moveDirection = MoveDirection::NEUTRAL;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Thruster> m_pThruster;
	};
}

