#include "PlayerStatus.h"

#include <Imgui/imgui.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Timer.h"
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
		if (m_isInvincible == true)
		{
			if (m_timer < m_invincibleTime)
			{
				m_timer += Timer::Instance().deltaTime.count();
			}
			else
			{
				m_timer = 0.0f;
				m_isInvincible = false;
			}
		}
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
		char standUpBuf[256] = "";
		strcpy_s(standUpBuf, sizeof(standUpBuf), m_standUpAnimPath.c_str());
		auto standUpAnimPathLabel = StringHelper::CreateLabel("StandUpAnimPath", m_uuID);
		if (ImGui::InputText(standUpAnimPathLabel.c_str(), standUpBuf, 256))
		{
			m_standUpAnimPath = standUpBuf;
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
		// 無敵時間の調整
		auto invincibleTimeLabel = StringHelper::CreateLabel("InvincibleTime", m_uuID);
		if (ImGui::DragFloat(invincibleTimeLabel.c_str(), &m_invincibleTime))
		{

		}

	}
	void PlayerStatus::Stagger()
	{
		if (auto pLockAnimater = m_pAnimater.lock())
		{
			pLockAnimater->SetFileName(m_staggerAnimPath, m_staggerAnimEndTime);
			pLockAnimater->SetFileName(m_standUpAnimPath, 3);
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
		if (m_isInvincible == false)
		{
			m_isInvincible = true;
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
}