#include "FollowMove.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Transform.h"

REGISTER_COMPONENT(TMF::FollowMove, "FollowMove");

namespace TMF
{
	void FollowMove::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTranform = pTransform.lock())
			{
				m_pTransform = pLockTranform;
				auto pParent = pLockTranform->GetParent();
				if (auto pLockParent = pParent.lock())
				{
					m_pFollowTarget = pLockParent;
				}
			}
		}
	}
	void FollowMove::OnFinalize()
	{

	}
	void FollowMove::OnUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (auto pLockTargetTransform = m_pFollowTarget.lock())
			{
				auto forward = pLockTargetTransform->GetForward();
				auto left = pLockTargetTransform->GetLeft();
				forward.Normalize();
				left.Normalize();
				auto up = pLockTargetTransform->GetRotation().ToEuler().y;
				left.y = 1.0f;
				auto pos = pLockTargetTransform->GetPosition() + left * m_offsetPos;
				pLockTransform->SetPosition(pos);
				auto rotation = DirectX::SimpleMath::Vector3(0.0f, up, m_rotationOffset);
				auto qua = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
				pLockTransform->SetRotation(qua);
			}
		}
	}
	void FollowMove::OnLateUpdate()
	{
	}
	void FollowMove::OnDraw()
	{
	}
	void FollowMove::OnDrawImGui()
	{
		auto offsetLabel = StringHelper::CreateLabel("Offset", m_uuID);
		if (ImGui::DragFloat3(offsetLabel.c_str(), &m_offsetPos.x, 0.1f))
		{

		}
		auto rotationOffsetLabel = StringHelper::CreateLabel("RotationOffset", m_uuID);
		if (ImGui::DragFloat(rotationOffsetLabel.c_str(), &m_rotationOffset, 0.1f))
		{

		}

	}
}