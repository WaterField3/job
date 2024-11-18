#include "CharacterMoveController.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Input.h"
#include "Transform.h"
#include "Melee.h"
#include "Shot.h"
#include "Dodge.h"
#include "PlayerStatus.h"

REGISTER_COMPONENT(TMF::CharacterMoveController, "CharacterMoveController");

namespace TMF
{
	CharacterMoveController::CharacterMoveController()
	{
	}
	CharacterMoveController::~CharacterMoveController()
	{
	}
	void CharacterMoveController::OnInitialize()
	{
	}
	void CharacterMoveController::OnFinalize()
	{
	}
	void CharacterMoveController::OnUpdate()
	{
		auto kb = Input::Instance().GetKeyState();
		auto tracker = Input::Instance().GetTracker();
		tracker->Update(kb);
		auto movePos = DirectX::SimpleMath::Vector3::Zero;
		auto torque = DirectX::SimpleMath::Vector3::Zero;
		auto isJump = false;
		auto isRotate = false;
		auto isAttack = false;
		auto isShot = false;
		auto isDodge = false;
		auto moveDirection = Dodge::Direction::FOWARD;
		if (kb.W == true)
		{
			auto pOwner = m_pOwner.lock();
			auto transform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				auto pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetWorldRotation();
				auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, rotate);

				movePos += forward * m_moveSpeed;
			}
		}
		if (kb.S == true)
		{
			auto pOwner = m_pOwner.lock();
			auto transform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				auto pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetWorldRotation();
				auto backward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, rotate);
				movePos += backward * m_moveSpeed;
				moveDirection = Dodge::Direction::BACK;
			}
		}
		if (kb.A == true)
		{
			auto pOwner = m_pOwner.lock();
			auto transform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				auto pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetWorldRotation();
				auto left = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Left, rotate);
				movePos += left * m_moveSpeed;
				torque = DirectX::SimpleMath::Vector3(abs(checkvec3.x), abs(checkvec3.y), abs(checkvec3.y));
				isRotate = true;
				moveDirection = Dodge::Direction::LEFT;
			}
		}
		if (kb.D == true)
		{
			auto pOwner = m_pOwner.lock();
			auto transform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				auto pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetWorldRotation();
				auto right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, rotate);
				movePos += right * m_moveSpeed;
				torque = DirectX::SimpleMath::Vector3(abs(checkvec3.x), -abs(checkvec3.y), abs(checkvec3.y));
				isRotate = true;
				moveDirection = Dodge::Direction::RIGHT;
			}
		}
		if (tracker->pressed.Space == true)
		{
			isJump = true;
		}
		else if (tracker->pressed.Enter == true)
		{
			isAttack = true;
		}
		else if (tracker->pressed.Back == true)
		{
			isShot = true;
		}
		// åªç›éûä‘
		auto now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		if (tracker->pressed.B == true)
		{
			// àÍíËéûä‘ì‡Ç…ï°êîâÒâüÇ≥ÇÍÇƒÇ¢ÇÈÇ©
			if (Input::Instance().PluralGetKeyDiwn(now) == true)
			{
				isDodge = true;
				movePos = DirectX::SimpleMath::Vector3::Zero;
			}
		}

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pPlayerStatus = pLockOwner->GetComponent<PlayerStatus>();
			auto isInvincible = false;
			if (auto pLockPlayerStatus = pPlayerStatus.lock())
			{
				isInvincible = pLockPlayerStatus->GetIsInvincible();
			}

			auto rigidBodyComponent = pLockOwner->GetComponent<Rigidbody>();
			if (auto rb = rigidBodyComponent.lock())
			{
				if (movePos != DirectX::SimpleMath::Vector3::Zero && isInvincible == false)
				{
					//rb->SetLinearVelocity(movePos);
					//rb->ApplyCentralForce(movePos);
					rb->ApplyForce(movePos, DirectX::SimpleMath::Vector3::Zero);
					if (isRotate == true)
					{
						//rb->SetAngularFactor(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
					}
					else
					{
						rb->SetAngularFactor(DirectX::SimpleMath::Vector3::Zero);
					}
					rb->ApplyTorque(torque);

				}
				if (isJump == true)
				{
					auto jumpPos = DirectX::SimpleMath::Vector3::Up * m_jumpPower;
					rb->ApplyImpulse(jumpPos, DirectX::SimpleMath::Vector3::Zero);
					rb->ApplyTorque(torque);
				}
				if (kb.N == true)
				{
					rb->SetAngularFactor(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
					auto torque = checkvec3;
					rb->ApplyTorque(torque);
					//rb->GetTotalTorque();
					//rb->SetRotation(torque);
				}
				if (tracker->pressed.O == true)
				{
					rb->SetAngularVelocity(DirectX::SimpleMath::Vector3::Zero);
					//rb->ClearForces();
					//rb->GetAngularFactor();

				}
			}
			if (isAttack == true)
			{
				auto pMelee = pLockOwner->GetComponent<Melee>();
				if (auto pLockMelee = pMelee.lock())
				{
					pLockMelee->Play();
				}
			}
			else if (isShot == true)
			{
				auto pShot = pLockOwner->GetComponent<Shot>();
				if (auto pLockShot = pShot.lock())
				{
					pLockShot->Play();
				}
			}
			else if (isDodge == true)
			{
				auto pDodge = pLockOwner->GetComponent<Dodge>();
				if (auto pLockDodge = pDodge.lock())
				{
					pLockDodge->DodgeStart(moveDirection);
				}
			}
		}
	}
	void CharacterMoveController::OnLateUpdate()
	{
	}
	void CharacterMoveController::OnDraw()
	{
	}
	void CharacterMoveController::OnDrawImGui()
	{
		if (ImGui::DragFloat("MoveSpeed", &m_moveSpeed, 0.1f, 0.0f))
		{

		}
		if (ImGui::DragFloat("JumpPower", &m_jumpPower, 0.1f, 0.0f))
		{

		}
		if (ImGui::DragFloat3("checkVec3", &checkvec3.x))
		{

		}
	}
}