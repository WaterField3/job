#include "CharacterMoveController.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Input.h"
#include "Melee.h"
#include "Shot.h"
#include "Dodge.h"
#include "PlayerStatus.h"
#include "MoveInfo.h"
#include "Thruster.h"
#include "Jump.h"
#include "Camera.h"
#include "GameObject/GameObjectManager.h"
#include "FreeCamera.h"
#include "CameraMove.h"
#include "EventSystem/EventSystem.h"
#include "PlayerMove.h"
#include "State/StateMachine.h"

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
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			m_pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			m_pStateMachine = std::make_shared<StateMachine>(pLockOwner);
			if (m_pStateMachine)
			{
				m_pStateMachine->ChangeState("PlayerIdleState");
			}
		}
	}
	void CharacterMoveController::OnFinalize()
	{
	}
	void CharacterMoveController::OnUpdate()
	{
		auto kb = Input::Instance().GetKeyState();
		auto tracker = Input::Instance().GetKeyboardTracker();
		tracker->Update(kb);
		auto movePos = DirectX::SimpleMath::Vector3::Zero;
		auto torque = DirectX::SimpleMath::Vector3::Zero;
		auto isJump = false;
		auto isRotate = false;
		auto isAttack = false;
		auto isShot = false;
		auto isDodge = false;
		auto isThruster = false;
		auto moveDirection = MoveDirection::NEUTRAL;
		//if (kb.W == true)
		//{
		//	auto pLockOwner = m_pOwner.lock();
		//	auto pTransform = pLockOwner->GetComponent<Transform>();
		//	if (auto pLockTransform = pTransform.lock())
		//	{
		//		auto pos = pLockTransform->GetWorldPosition();
		//		auto rotate = pLockTransform->GetWorldRotation();
		//		auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, rotate);

		//		movePos += forward * m_moveSpeed;
		//	}
		//}
		//if (kb.S == true)
		//{
		//	auto pLockOwner = m_pOwner.lock();
		//	auto pTransform = pLockOwner->GetComponent<Transform>();
		//	if (auto pLockTransform = pTransform.lock())
		//	{
		//		auto pos = pLockTransform->GetWorldPosition();
		//		auto rotate = pLockTransform->GetWorldRotation();
		//		auto backward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, rotate);
		//		movePos += backward * m_moveSpeed;
		//		moveDirection = MoveDirection::BACK;
		//	}
		//}
		//if (kb.A == true)
		//{
		//	auto pLockOwner = m_pOwner.lock();
		//	auto pTransform = pLockOwner->GetComponent<Transform>();
		//	if (auto pLockTransform = pTransform.lock())
		//	{
		//		auto pos = pLockTransform->GetWorldPosition();
		//		auto rotate = pLockTransform->GetWorldRotation();
		//		auto left = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Left, rotate);
		//		movePos += left * m_moveSpeed;
		//		torque = DirectX::SimpleMath::Vector3(abs(checkvec3.x), abs(checkvec3.y), abs(checkvec3.y));
		//		isRotate = true;
		//		moveDirection = MoveDirection::LEFT;
		//	}
		//}
		//if (kb.D == true)
		//{
		//	auto pLockOwner = m_pOwner.lock();
		//	auto pTransform = pLockOwner->GetComponent<Transform>();
		//	if (auto pLockTransform = pTransform.lock())
		//	{
		//		auto pos = pLockTransform->GetWorldPosition();
		//		auto rotate = pLockTransform->GetWorldRotation();
		//		auto right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, rotate);
		//		movePos += right * m_moveSpeed;
		//		torque = DirectX::SimpleMath::Vector3(abs(checkvec3.x), -abs(checkvec3.y), abs(checkvec3.y));
		//		isRotate = true;
		//		moveDirection = MoveDirection::RIGHT;
		//	}
		//}

		m_pStateMachine->Update();

		if (kb.V == true)
		{
			isThruster = true;
		}
		else if (kb.Space == true)
		{
			isJump = true;
		}

		// èuä‘ÇéÊìæ
		if (tracker->pressed.Space == true)
		{
			//isJump = true;
		}
		else if (tracker->pressed.Enter == true)
		{
			isAttack = true;
		}
		else if (tracker->pressed.Back == true)
		{
			isShot = true;
		}
		else if (tracker->pressed.Y == true)
		{
			auto pCamera = GameObjectManager::Instance().GetComponent<Camera>();
			//auto pCamera = GameObjectManager::Instance().GetComponent<FreeCamera>();
			if (auto pLockCamera = pCamera.lock())
			{
				if (auto pLockOwner = m_pOwner.lock())
				{
					auto pTransform = pLockOwner->GetComponent<Transform>();
					if (auto pLockTransform = pTransform.lock())
					{
						pLockCamera->SetTargetTransform(pLockTransform);
					}
				}

				auto pCameraMove = GameObjectManager::Instance().GetComponent<CameraMove>();
				if (auto pLockOwner = m_pOwner.lock())
				{
					if (auto pLockCameraMove = pCameraMove.lock())
					{
						pLockCameraMove->SetTargetRigidbody(pLockOwner->GetComponent<Rigidbody>());
					}
				}

			}
		}
		else if (tracker->pressed.U == true)
		{
			auto pCamera = GameObjectManager::Instance().GetComponent<Camera>();
			//auto pCamera = GameObjectManager::Instance().GetComponent<FreeCamera>();
			if (auto pLockCamera = pCamera.lock())
			{

				pLockCamera->SetTargetTransform(std::shared_ptr<Transform>());
			}
		}

		// åªç›éûä‘
		auto now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		if (tracker->pressed.V == true)
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

			auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			if (auto pLockRigidbody = pRigidbody.lock())
			{
				if (movePos != DirectX::SimpleMath::Vector3::Zero && isInvincible == false)
				{
					auto velocity = pLockRigidbody->GetLinearVelocity();
					movePos.y = velocity.y;

					pLockRigidbody->SetLinearVelocity(movePos);
					//rb->ApplyCentralForce(movePos);
					//rb->ApplyForce(movePos, DirectX::SimpleMath::Vector3::Zero);
					if (isRotate == true)
					{
						//rb->SetAngularFactor(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
					}
					else
					{
						pLockRigidbody->SetAngularFactor(DirectX::SimpleMath::Vector3::Zero);
					}
					pLockRigidbody->ApplyTorque(torque);

				}
				//if (isJump == true)
				//{
				//	auto jumpPos = DirectX::SimpleMath::Vector3::Up * m_jumpPower;
				//	pLockRigidbody->ApplyImpulse(jumpPos, DirectX::SimpleMath::Vector3::Zero);
				//	pLockRigidbody->ApplyTorque(torque);
				//}
				if (kb.N == true)
				{
					pLockRigidbody->SetAngularFactor(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
					auto torque = checkvec3;
					pLockRigidbody->ApplyTorque(torque);
					//rb->GetTotalTorque();
					//rb->SetRotation(torque);
				}
				if (tracker->pressed.O == true)
				{
					pLockRigidbody->SetAngularVelocity(DirectX::SimpleMath::Vector3::Zero);
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
			if (isThruster == true)
			{
				auto pThruster = pLockOwner->GetComponent<Thruster>();
				if (auto pLockThruster = pThruster.lock())
				{
					pLockThruster->FastMovement(moveDirection);
				}
			}
			else if (isThruster == false)
			{
				auto pThruster = pLockOwner->GetComponent<Thruster>();
				if (auto pLockThruster = pThruster.lock())
				{
					pLockThruster->StopFastMovement();
				}
			}
			if (isJump == true)
			{
				auto pJump = pLockOwner->GetComponent<Jump>();
				if (auto pLockJump = pJump.lock())
				{
					pLockJump->Chage(moveDirection);
				}
			}
			else if (isJump == false && m_oldIsJump == true)
			{
				auto pJump = pLockOwner->GetComponent<Jump>();
				if (auto pLockJump = pJump.lock())
				{
					pLockJump->ChageStop(moveDirection);
				}
			}
		}
		m_oldIsJump = isJump;
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