#include "FollowMove.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"

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
				if (m_targetName != "")
				{
					auto pTargetObject = GameObjectManager::Instance().GetGameObject(m_targetName);
					if (auto pLockTargetObject = pTargetObject.lock())
					{
						auto playerPos = DirectX::SimpleMath::Vector3::Zero;
						m_pFollowTarget = pLockTargetObject->GetComponent<Transform>();
					}
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
			if (auto pLockFollowTarget = m_pFollowTarget.lock())
			{
				auto movePos = pLockFollowTarget->GetWorldPosition();
				auto moveDirection = pLockFollowTarget->GetForward();
				switch (m_moveDirection)
				{
				case TMF::NEUTRAL:
					break;
				case TMF::FOWARD:
					break;
				case TMF::RIGHT:
					moveDirection = pLockFollowTarget->GetRight();
					break;
				case TMF::LEFT:
					moveDirection = pLockFollowTarget->GetLeft();
					break;
				case TMF::BACK:
					moveDirection = pLockFollowTarget->GetBack();
					break;
				default:
					break;
				}
				moveDirection.y = 1.0f;

				movePos = movePos + pLockFollowTarget->GetRight() * m_offsetPos.x + pLockFollowTarget->GetUp() * m_offsetPos.y + pLockFollowTarget->GetForward() * m_offsetPos.z;

				pLockTransform->SetPosition(movePos);
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
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_targetName.c_str());
		auto targetNameLabel = StringHelper::CreateLabel("TargetName", m_uuID);
		if (ImGui::InputText(targetNameLabel.c_str(), buf, 256))
		{
			m_targetName = buf;
		}

		const char* types[] = { "NEUTRAL","FOWARD","RIGHT","LEFT","BACK" };
		int selectIndex = (int)m_moveDirection;
		auto loadTypeCombo = StringHelper::CreateLabel("MoveDirection", m_uuID);
		if (ImGui::BeginCombo(loadTypeCombo.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_moveDirection == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_moveDirection = MOVEDIRECTION(i);
					selectIndex = i;

				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		auto offsetPosLabel = StringHelper::CreateLabel("OffsetPos", m_uuID);
		if (ImGui::DragFloat3(offsetPosLabel.c_str(), &m_offsetPos.x))
		{

		}
	}
	std::shared_ptr<Component> FollowMove::OnClone() const
	{
		return std::shared_ptr<Component>();
	}
}