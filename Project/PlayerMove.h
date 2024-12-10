#pragma once

#include <memory>
#include <SimpleMath.h>

namespace TMF
{
	class Transform;
	class Rigidbody;
	class Thruster;
	class PlayerMove
	{
	public:
		PlayerMove();
		PlayerMove(std::weak_ptr<Transform> pTransform, std::weak_ptr<Rigidbody> pRigidbody, std::weak_ptr<Thruster> pThruster, float moveSpeed);
		~PlayerMove();

		void MoveForward();
		void MoveDirection(DirectX::SimpleMath::Vector3& vector);
		void MoveBack();
		void MoveLeft();
		void MoveRight();
		void FastMoveForward();
		void FastMoveBack();
		void FastMoveLeft();
		void FastMoveRight();
		void StopFastMove();

	private:
		float m_moveSpeed = 1.0f;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::weak_ptr<Thruster> m_pThruster;
	};
}

