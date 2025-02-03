#include "FontMove.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Font.h"

REGISTER_COMPONENT(TMF::FontMove, "FontMove");

namespace TMF
{
	void FontMove::OnInitialize()
	{
		if (auto pLockOwer = m_pOwner.lock())
		{
			m_pFont = pLockOwer->GetComponent<Font>();
		}
	}
	void FontMove::OnFinalize()
	{

	}
	void FontMove::OnUpdate()
	{
		if (auto pLockFont = m_pFont.lock())
		{
			auto currentFontPos = pLockFont->GetFontPosition();
			auto distance = DirectX::SimpleMath::Vector2::Distance(currentFontPos, m_targetPosition);
			if (distance < 5.0f)
			{
				return;
			}
			auto moveVector = m_targetPosition- currentFontPos;
			moveVector.Normalize();
			auto moveSpeed = std::lerp(1.0f, m_maxMoveSpeed, m_moveTime);
			auto movePos = currentFontPos + moveVector * moveSpeed;
			pLockFont->SetFonstPosition(movePos);

		}
	}
	void FontMove::OnLateUpdate()
	{

	}
	void FontMove::OnDraw()
	{

	}
	void FontMove::OnDrawImGui()
	{
		auto maxMoveSpeedLabel = StringHelper::CreateLabel("MaxMoveSpeed", m_uuID);
		if (ImGui::DragFloat(maxMoveSpeedLabel.c_str(), &m_maxMoveSpeed, 0.1f))
		{

		}
		auto moveTimeLabel = StringHelper::CreateLabel("MoveTime", m_uuID);
		if (ImGui::DragFloat(moveTimeLabel.c_str(), &m_moveTime, 0.1f))
		{

		}
		auto targetPositionLabel = StringHelper::CreateLabel("TargetPosition", m_uuID);
		if (ImGui::DragFloat2(targetPositionLabel.c_str(), &m_targetPosition.x, 0.1f))
		{

		}
	}
	std::shared_ptr<Component> FontMove::OnClone() const
	{
		// m_maxMoveSpeed, m_moveTime, m_targetPosition
		auto pClone = std::make_shared<FontMove>();
		pClone->m_maxMoveSpeed = this->m_maxMoveSpeed;
		pClone->m_moveTime = this->m_moveTime;
		pClone->m_targetPosition = this->m_targetPosition;
		return move(pClone);
	}
}