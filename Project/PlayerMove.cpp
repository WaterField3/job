#include "PlayerMove.h"

#include "Component/Transform.h"
#include "Component/Rigidbody.h"
#include "Component/Thruster.h"
#include "Input.h"

namespace TMF
{
	PlayerMove::PlayerMove()
	{

	}
	PlayerMove::PlayerMove(std::weak_ptr<Transform> pTransform, std::weak_ptr<Rigidbody> pRigidbody,std::weak_ptr<Thruster> pThruster, float moveSpeed)
	{
		m_pTransform = pTransform.lock();
		m_pRigidbody = pRigidbody.lock();
		m_pThruster = pThruster.lock();
		m_moveSpeed = moveSpeed;
	}

	PlayerMove::~PlayerMove()
	{

	}

	void PlayerMove::MoveForward()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto forward = pLockTransform->GetForward();
			MoveDirection(forward);
		}
	}


	void PlayerMove::MoveBack()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto back = pLockTransform->GetBack();
			MoveDirection(back);
		}
	}

	void PlayerMove::MoveLeft()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto left = pLockTransform->GetLeft();
			MoveDirection(left);
		}
	}

	void PlayerMove::MoveRight()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto right = pLockTransform->GetRight();
			MoveDirection(right);
		}
	}

	void PlayerMove::FastMoveForward()
	{
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->FastMovement(MoveDirection::FOWARD);
		}
	}

	void PlayerMove::FastMoveBack()
	{
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->FastMovement(MoveDirection::BACK);
		}
	}

	void PlayerMove::FastMoveLeft()
	{
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->FastMovement(MoveDirection::LEFT);
		}
	}

	void PlayerMove::FastMoveRight()
	{
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->FastMovement(MoveDirection::RIGHT);
		}
	}

	void PlayerMove::StopFastMove()
	{
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->StopFastMovement();
		}
	}

	void PlayerMove::MoveDirection(DirectX::SimpleMath::Vector3& vector)
	{
		if (auto pLockRigidbody = m_pRigidbody.lock())
		{
			auto movePos = vector * m_moveSpeed;
			auto velocity = pLockRigidbody->GetLinearVelocity();
			movePos.y = velocity.y;
			pLockRigidbody->SetLinearVelocity(movePos);
		}
	}

}
