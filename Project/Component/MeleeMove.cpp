#include "MeleeMove.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "GhostObject.h"
#include "Audio.h"
#include "Effect.h"

REGISTER_COMPONENT(TMF::MeleeMove, "MeleeMove");

// UŒ‚Žž‚Ì‰ñ“]‚ðC³

namespace TMF
{
	MeleeMove::MeleeMove()
	{
	}
	MeleeMove::~MeleeMove()
	{
	}
	void MeleeMove::OnInitialize()
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
	void MeleeMove::OnFinalize()
	{
	}
	void MeleeMove::OnUpdate()
	{
		if (m_isPlay == true)
		{

			auto nowPosition = DirectX::SimpleMath::Vector3::One;
			auto nowRotation = DirectX::SimpleMath::Quaternion::Identity;
			if (auto pLockTransform = m_pTransform.lock())
			{
				nowPosition = pLockTransform->GetPosition();
				nowRotation = pLockTransform->GetRotation();
				auto distance = DirectX::SimpleMath::Vector3::Distance(nowPosition, m_endPosition);
				if (distance > 0.2f)
				{
					auto movePos = nowPosition + m_moveVector * m_moveSpeed;
					pLockTransform->SetPosition(movePos);
					auto Up = m_playerUp;
					if (m_time < 1.0f)
					{
						m_time += 0.01f * m_rotationSpeed;
					}
					auto rotateOffsetX = std::lerp(0.0f, m_targetRotationOffset.x, m_time);
					auto rotateOffsetZ = std::lerp(m_rotationOffset, m_targetRotationOffset.z, m_time);
					auto rotation = DirectX::SimpleMath::Vector3(rotateOffsetX, Up, rotateOffsetZ);
					auto qua = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
					pLockTransform->SetRotation(qua);
				}
				else
				{
					if (auto pLockOwner = m_pOwner.lock())
					{
						m_isPlay = false;
						pLockTransform->SetParent(m_pParent);
						pLockOwner->SetActive(false);
						return;
					}
				}
			}
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pGhostObjct = pLockOwner->GetComponent<GhostObject>();
				if (auto pLockGhostObject = pGhostObjct.lock())
				{
					auto movePos = nowPosition + m_moveVector * m_moveSpeed;
					pLockGhostObject->SetGhostObjectTransform(movePos, nowRotation);
				}
			}
		}
	}
	void MeleeMove::OnLateUpdate()
	{
	}
	void MeleeMove::OnDraw()
	{
	}
	void MeleeMove::OnDrawImGui()
	{
		auto moveSpeedLabel = StringHelper::CreateLabel("MoveSpeed", m_uuID);
		if (ImGui::DragFloat(moveSpeedLabel.c_str(), &m_moveSpeed, 0.01f))
		{

		}
		auto moveOfsetLabel = StringHelper::CreateLabel("MoveOfset", m_uuID);
		if (ImGui::DragFloat3(moveOfsetLabel.c_str(), &m_moveOfset.x, 0.1f))
		{

		}
		auto rotateLabel = StringHelper::CreateLabel("Rotate", m_uuID);
		if (ImGui::DragFloat3(rotateLabel.c_str(), &m_rotate.x, 0.1f))
		{

		}
		auto rotationOffsetLabel = StringHelper::CreateLabel("RotationOffset", m_uuID);
		if (ImGui::DragFloat(rotationOffsetLabel.c_str(), &m_rotationOffset, 0.1f))
		{

		}
		auto rotationOffsetXLabel = StringHelper::CreateLabel("TargetRotationOffset", m_uuID);
		if (ImGui::DragFloat3(rotationOffsetXLabel.c_str(), &m_targetRotationOffset.x, 0.1f))
		{

		}
		auto rotationSpeed = StringHelper::CreateLabel("RotationSpeed", m_uuID);
		if (ImGui::DragFloat(rotationSpeed.c_str(), &m_rotationSpeed, 0.1f))
		{

		}
	}
	std::shared_ptr<Component> MeleeMove::OnClone() const
	{
		auto pClone = std::make_shared<MeleeMove>();
		pClone->m_moveSpeed = this->m_moveSpeed;
		pClone->m_moveOfset = this->m_moveOfset;
		pClone->m_rotate = this->m_rotate;
		pClone->m_rotationOffset = this->m_rotationOffset;
		pClone->m_rotationSpeed = this->m_rotationSpeed;
		pClone->m_targetRotationOffset = this->m_targetRotationOffset;
		return move(pClone);
	}
	void MeleeMove::Cancel()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			pLockOwner->SetActive(false);
			if (auto pLockTransform = m_pTransform.lock())
			{
				m_isPlay = false;
				pLockTransform->SetParent(m_pParent);
				pLockOwner->SetActive(false);
			}
		}
	}
	void MeleeMove::Play(MoveType type, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation)
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			pLockOwner->SetActive(true);
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, rotation);
				auto right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, rotation);
				auto left = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Left, rotation);
				auto down = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Down, rotation);
				auto up = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Up, rotation);
				switch (type)
				{
				case TMF::MeleeMove::DEFAULT:
				{
					auto startPos = position + forward * m_moveOfset.z + left * m_moveOfset.x + up * m_moveOfset.y;
					pLockTransform->SetPosition(startPos);
					m_endPosition = position + forward * m_moveOfset.z + right * m_moveOfset.x + down;
					m_moveVector = m_endPosition - startPos;
					m_moveVector.Normalize();
					auto Up = rotation.ToEuler().y;
					m_playerUp = Up;
					auto rotation = DirectX::SimpleMath::Vector3(0.0f, Up, m_rotationOffset);
					auto qua = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
					pLockTransform->SetRotation(qua);
					break;
				}
				case TMF::MeleeMove::SPECIAL:
					break;
				default:
					break;
				}
			}
			auto pAudio = pLockOwner->GetComponent<Audio>();
			if (auto pLockAudio = pAudio.lock())
			{
				pLockAudio->Play();
			}
			auto pEffect = pLockOwner->GetComponent<Effect>();
			if (auto pLockEffect = pEffect.lock())
			{
				pLockEffect->Play();
			}
			m_isPlay = true;
			m_time = 0.0f;
		}
	}
	void MeleeMove::CheckParent()
	{
		if (auto pLockTransform = m_pTransform.lock())
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
		else
		{
			if (auto pLockTransform = m_pTransform.lock())
			{
				m_pParent = pLockTransform->GetParent();
			}
		}
	}
}