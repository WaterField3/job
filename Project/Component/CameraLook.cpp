#include "CameraLook.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Camera.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "ComponentRegister.h"

REGISTER_COMPONENT(TMF::CameraLook, "CameraLook");

namespace TMF
{
	void CameraLook::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTranform = pTransform.lock())
			{
				m_pTransform = pLockTranform;
				if (m_targetName != "")
				{
					auto pTargetObject = GameObjectManager::Instance().GetGameObject(m_targetName);
					if (auto pLockTargetObject = pTargetObject.lock())
					{
						auto playerPos = DirectX::SimpleMath::Vector3::Zero;
						m_pLockTarget = pLockTargetObject->GetComponent<Transform>();
					}
				}
			}
			auto pCamera = pLockOwner->GetComponent<Camera>();
			if (auto pLockCamera = pCamera.lock())
			{
				m_pCamera = pLockCamera;
			}
		}
	}
	void CameraLook::OnFinalize()
	{
	}
	void CameraLook::OnUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (auto pLockFollowTarget = m_pLockTarget.lock())
			{
				auto lookPos = pLockFollowTarget->GetWorldPosition();
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
				lookPos += m_offsetPos * moveDirection;
				if (lookPos == DirectX::SimpleMath::Vector3::Zero)
				{
					return;
				}
				if (auto pLockCamera = m_pCamera.lock())
				{
					pLockCamera->SetTargetPosition(lookPos);
				}
			}
		}
	}
	void CameraLook::OnLateUpdate()
	{
	}
	void CameraLook::OnDraw()
	{
	}
	void CameraLook::OnDrawImGui()
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
	std::shared_ptr<Component> CameraLook::OnClone() const
	{
		return std::shared_ptr<Component>();
	}
}