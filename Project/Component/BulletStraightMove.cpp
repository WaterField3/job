#include "BulletStraightMove.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Utility/StringHelper.h"
#include "Timer.h"
#include "GameObject/GameObjectManager.h"
#include "GhostObject.h"

REGISTER_COMPONENT(TMF::BulletStraightMove, "BulletStraightMove");

namespace TMF
{
	void BulletStraightMove::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = m_pTransform.lock())
			{
				m_pParent = pLockTransform->GetParent();
			}
		}
	}
	void BulletStraightMove::OnFinalize()
	{
	}
	void BulletStraightMove::OnUpdate()
	{
		if (m_endTime > m_timer)
		{
			auto deltaTime = Timer::Instance().deltaTime.count();
			m_timer += deltaTime;
			auto movePos = DirectX::SimpleMath::Vector3::Zero;
			auto rotation = DirectX::SimpleMath::Quaternion::Identity;
			if (auto pLockTransform = m_pTransform.lock())
			{
				movePos = pLockTransform->GetPosition() + m_moveVector * m_moveSpeed;
				rotation = pLockTransform->GetRotation();
				pLockTransform->SetPosition(movePos);
			}
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pGhostObject = pLockOwner->GetComponent<GhostObject>();
				if (auto pLockGhostObject = pGhostObject.lock())
				{
					pLockGhostObject->SetGhostObjectTransform(movePos, rotation);
				}
			}
		}
		else
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				pLockOwner->SetActive(false);
			}
			if (auto pLockTransform = m_pTransform.lock())
			{
				pLockTransform->SetParent(m_pParent);
			}

			//GameObjectManager::Instance().DestroyGameObject(pLockOwner.get());
		}
	}
	void BulletStraightMove::OnLateUpdate()
	{

	}
	void BulletStraightMove::OnDraw()
	{
	}
	void BulletStraightMove::OnDrawImGui()
	{
		auto moveVectorLabel = StringHelper::CreateLabel("moveVector", m_uuID);
		if (ImGui::DragFloat3(moveVectorLabel.c_str(), &m_moveVector.x))
		{

		}

		auto moveSpeedLabel = StringHelper::CreateLabel("moveSpeed", m_uuID);
		if (ImGui::DragFloat(moveSpeedLabel.c_str(), &m_moveSpeed))
		{

		}
		auto endTimeLabel = StringHelper::CreateLabel("endTime", m_uuID);
		if (ImGui::DragFloat(endTimeLabel.c_str(), &m_endTime))
		{

		}
	}
	void BulletStraightMove::MoveStart(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation)
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				pLockOwner->SetActive(true);
				ResetTimer();
				auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, rotation);
				auto right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, rotation);
				auto left = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Left, rotation);
				auto down = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Down, rotation);
				auto up = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Up, rotation);
				auto startPos = position + forward + up * 2;
				m_moveVector = forward;
				pLockTransform->SetPosition(startPos);
				pLockTransform->SetRotation(rotation);

			}
		}
	}
	void BulletStraightMove::MoveStart(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 moveVector)
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				pLockOwner->SetActive(true);
				ResetTimer();
				m_moveVector = moveVector;
				moveVector.Normalize();
				auto rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(moveVector.y, moveVector.x, moveVector.z);
				auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, rotation);
				forward.Normalize();
				auto quaternionForward = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(forward.y, forward.x, forward.z);
				//rotation = DirectX::SimpleMath::Quaternion::LookRotation(moveVector, DirectX::SimpleMath::Vector3::Up);
				pLockTransform->SetPosition(position);
				pLockTransform->SetRotation(rotation);

			}
		}
	}
}