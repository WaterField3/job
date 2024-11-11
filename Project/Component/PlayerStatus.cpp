#include "PlayerStatus.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Animater.h"

REGISTER_COMPONENT(TMF::PlayerStatus, "PlayerStatus");

namespace TMF
{
	PlayerStatus::PlayerStatus()
	{

	}
	PlayerStatus::~PlayerStatus()
	{
	}
	void PlayerStatus::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pAnimater = pLockOwner->GetComponent<Animater>();
		}
	}
	void PlayerStatus::OnFinalize()
	{
	}
	void PlayerStatus::OnUpdate()
	{
	}
	void PlayerStatus::OnLateUpdate()
	{
	}
	void PlayerStatus::OnDraw()
	{
	}
	void PlayerStatus::OnDrawImGui()
	{
		char staggerBuf[256] = "";
		strcpy_s(staggerBuf, sizeof(staggerBuf), m_staggerAnimPath.c_str());
		auto staggerAnimPathLabel = StringHelper::CreateLabel("StaggerAnimPath", m_uuID);
		if (ImGui::InputText(staggerAnimPathLabel.c_str(), staggerBuf, 256))
		{
			m_staggerAnimPath = staggerBuf;
		}
		char invertBuf[256] = "";
		strcpy_s(invertBuf, sizeof(invertBuf), m_invertAnimPath.c_str());
		auto invertAnimPathLabel = StringHelper::CreateLabel("InvertAnimPath", m_uuID);
		if (ImGui::InputText(invertAnimPathLabel.c_str(), invertBuf, 256))
		{
			m_invertAnimPath = invertBuf;
		}
		// よろけるアニメーションの終了時間
		auto staggerEndTimeLabel = StringHelper::CreateLabel("StaggerEndTime", m_uuID);
		if (ImGui::DragFloat(staggerEndTimeLabel.c_str(), &m_staggerAnimEndTime))
		{

		}
		//　転倒するアニメーションの終了時間
		auto invertEndTimeLabel = StringHelper::CreateLabel("InvertEndTime", m_uuID);
		if (ImGui::DragFloat(invertEndTimeLabel.c_str(), &m_invertAnimEndTime))
		{

		}
	}
	void PlayerStatus::Stagger()
	{
		if (auto pLockAnimater = m_pAnimater.lock())
		{
			pLockAnimater->SetFileName(m_staggerAnimPath, m_staggerAnimEndTime);
		}
	}
	void PlayerStatus::Invert()
	{
		if (auto pLockAnimater = m_pAnimater.lock())
		{
			pLockAnimater->SetFileName(m_invertAnimPath, m_invertAnimEndTime);
		}
	}
	void PlayerStatus::Damage(float damage)
	{
		m_hp -= damage;
		// 体力がなくなった時の処理
		if (m_hp < 0)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				pLockOwner->SetActive(false);
			}
		}
	}
}