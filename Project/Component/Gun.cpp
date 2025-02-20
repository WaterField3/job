#include "Gun.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "BulletStraightMove.h"
#include "Timer.h"
#include "Attack.h"

REGISTER_COMPONENT(TMF::Gun, "Gun");

namespace TMF
{
	void Gun::OnInitialize()
	{
		OwnerWeponCheck();
		m_changeTime = m_initChangeTime;
		m_bulletNum = m_bulletMaxNum;
		m_weaponType = WeaponType::SHOT;
	}
	void Gun::OnFinalize()
	{
	}
	void Gun::OnUpdate()
	{
		auto deltaTime = Timer::Instance().deltaTime.count();
		if (m_isShot == true)
		{
			m_timer += deltaTime;
			if (m_timer > m_coolTime)
			{
				m_isShot = false;
				m_timer = 0.0f;
			}
		}
		if (m_lateTimer > 0.0f)
		{
			m_lateTimer -= deltaTime;
		}
		else
		{
			if (m_lateTimer <= 0.0f)
			{
				if (m_changeTime < m_initChangeTime)
				{
					m_changeTime += deltaTime;
				}
			}
		}
		if (m_bulletNum == 0)
		{
			m_reloadTime += deltaTime;
			if (m_reloadTime > m_reloadMaxTime)
			{
				m_bulletNum = m_bulletMaxNum;
				m_reloadTime = 0.0f;
			}
		}
	}
	void Gun::OnLateUpdate()
	{
	}
	void Gun::OnDraw()
	{
	}
	void Gun::OnDrawImGui()
	{
		auto coolTimeLabel = StringHelper::CreateLabel("CoolTime", m_uuID);
		if (ImGui::DragFloat(coolTimeLabel.c_str(), &m_coolTime))
		{

		}
		auto changeTimeLabel = StringHelper::CreateLabel("ChangeTime", m_uuID);
		if (ImGui::DragFloat(changeTimeLabel.c_str(), &m_initChangeTime))
		{

		}
		auto maxBulletNumLabel = StringHelper::CreateLabel("MaxBulletNum", m_uuID);
		if (ImGui::DragInt(maxBulletNumLabel.c_str(), &m_bulletMaxNum))
		{

		}
		auto reloadTimeLabel = StringHelper::CreateLabel("ReloadTime", m_uuID);
		if (ImGui::DragFloat(reloadTimeLabel.c_str(), &m_reloadMaxTime))
		{

		}
		auto isUsePlayerLabel = StringHelper::CreateLabel("IsUsePlayer", m_uuID);
		if (ImGui::Checkbox(isUsePlayerLabel.c_str(), &m_isUsePlayer))
		{

		}
		if (ImGui::Button("Play"))
		{
			OnAttack();
		}
	}
	std::shared_ptr<Component> Gun::OnClone() const
	{
		auto pClone = std::make_shared<Gun>();
		pClone->m_coolTime = this->m_coolTime;
		pClone->m_initChangeTime = this->m_initChangeTime;
		pClone->m_reloadMaxTime = this->m_reloadMaxTime;
		pClone->m_bulletMaxNum = this->m_bulletMaxNum;
		pClone->m_isUsePlayer = this->m_isUsePlayer;
		return move(pClone);
	}
	bool Gun::OnAttack()
	{
		if (m_isUsePlayer == false)
		{
			if (m_changeTime < m_initChangeTime)
			{
				return false;
			}
		}
		if (m_isShot == true || m_bulletNum == 0)
		{
			return false;
		}

		// Ž©•ª‚ÅŽ©•ª‚ð”ò‚Î‚·‚æ‚¤‚É
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pBulletMove = pLockOwner->GetComponent<BulletStraightMove>();
			if (auto pLockBulletMove = pBulletMove.lock())
			{
				auto pCameraObject = GameObjectManager::Instance().GetGameObject("CameraObject");
				auto cameraPosition = DirectX::SimpleMath::Vector3::Zero;
				if (auto pLockCameraObject = pCameraObject.lock())
				{
					auto pCameraTransform = pLockCameraObject->GetComponent<Transform>();
					if (auto pLockCameraTransform = pCameraTransform.lock())
					{
						cameraPosition = pLockCameraTransform->GetWorldPosition();
					}
				}

				auto pTransform = pLockOwner->GetComponent<Transform>();
				if (auto pLockTrancform = pTransform.lock())
				{
					auto pParent = pLockTrancform->GetParent();
					if (auto pLockParent = pParent.lock())
					{
						m_isShot = true;
						m_bulletNum--;
						auto nowPosition = pLockParent->GetPosition();
						nowPosition.y += 3;
						auto nowRotation = pLockParent->GetRotation();
						auto moveVector = nowPosition - cameraPosition;
						pLockTrancform->SetParent(std::weak_ptr<Transform>());
						pLockBulletMove->MoveStart(nowPosition, moveVector);
						m_isCancel = false;
						return true;
					}
				}
			}
		}
		return false;
	}
	void Gun::OnSelect()
	{
		m_changeTime = 0.0f;
	}
	void Gun::OnCancel()
	{
		m_lateTimer = 0.0f;
		m_timer = 0.0f;
		m_isCancel = true;
	}
}