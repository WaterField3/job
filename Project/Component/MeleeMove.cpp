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
			}
			else
			{
				if (auto pLockOwner = m_pOwner.lock())
				{
					//GameObjectManager::Instance().DestroyGameObject(pLockOwner.get());
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
	void MeleeMove::OnLateUpdate()
	{
	}
	void MeleeMove::OnDraw()
	{
	}
	void MeleeMove::OnDrawImGui()
	{
		auto moveSpeedLabel = StringHelper::CreateLabel("MoveSpeed", m_uuID);
		if (ImGui::DragFloat(moveSpeedLabel.c_str(), &m_moveSpeed))
		{

		}
		auto rotateLabel = StringHelper::CreateLabel("Rotate", m_uuID);
		if (ImGui::DragFloat3(rotateLabel.c_str(), &m_rotate.x, 0.1f))
		{

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
					auto startPos = position + forward * 3 + left + up * 5;
					pLockTransform->SetPosition(startPos);
					m_endPosition = position + forward * 3 + right + down;
					m_moveVector = m_endPosition - startPos;
					m_moveVector.Normalize();
					auto normalForward = forward;
					normalForward.Normalize();
					//m_rotate *= normalForward;
					auto kari = m_rotate * normalForward;
					auto rotate = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(kari.y, kari.x, kari.z);
					//rotation.z = rotate.z + 2.0f;
					//forward.Normalize();
					//rotate = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(forward.y, forward.x, forward.z);
					rotation *= rotate /** DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_rotate.y, m_rotate.x, m_rotate.z)*/;
					//rotation = rotate;
					pLockTransform->SetRotation(rotation);
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
		}
	}
	void MeleeMove::OnTrigerEnter(GameObject* pGameObject)
	{
		//if (pGameObject->GetTag() == )
	}
}