#include "BulletStraightMove.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::BulletStraightMove, "BulletStraightMove");

namespace TMF
{
	void BulletStraightMove::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
		}
	}
	void BulletStraightMove::OnFinalize()
	{
	}
	void BulletStraightMove::OnUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto movePos = pLockTransform->GetPosition() + m_moveVector * m_moveSpeed;
			pLockTransform->SetPosition(movePos);
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
	}
}