#include "CharacterMoveController.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Input.h"
#include "Transform.h"

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
		auto jump = false;
		if (kb.W)
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
		if (kb.S)
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
		if (kb.A)
		{
			auto pOwner = m_pOwner.lock();
			auto transform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				auto pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetWorldRotation();
				auto left = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Left, rotate);
				movePos += left * m_moveSpeed;
			}
		}
		if (kb.D)
		{
			auto pOwner = m_pOwner.lock();
			auto transform = pOwner->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				auto pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetWorldRotation();
				auto right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, rotate);
				movePos += right * m_moveSpeed;
			}
		}
		if (tracker->pressed.Space)
		{
			jump = true;
		}

		if (auto owner = m_pOwner.lock())
		{
			auto rigidBodyComponent = owner->GetComponent<Rigidbody>();
			if (auto rb = rigidBodyComponent.lock())
			{
				if (movePos != DirectX::SimpleMath::Vector3::Zero)
				{
					//rb->SetLinearVelocity(movePos);
					//rb->ApplyCentralForce(movePos);
					rb->ApplyForce(movePos, DirectX::SimpleMath::Vector3::Zero);
				}
				if (jump)
				{
					auto jumpPos = DirectX::SimpleMath::Vector3::Up * m_jumpPower;
					rb->ApplyImpulse(jumpPos, DirectX::SimpleMath::Vector3::Zero);
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
	}
}