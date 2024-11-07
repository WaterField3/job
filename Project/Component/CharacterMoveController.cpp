#include "CharacterMoveController.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Input.h"
#include "Transform.h"
#include "Melee.h"

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

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto rigidBodyComponent = pLockOwner->GetComponent<Rigidbody>();
			if (auto rb = rigidBodyComponent.lock())
			{
				if (movePos != DirectX::SimpleMath::Vector3::Zero)
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