#include "Shot.h"

#include <Imgui/imgui.h>

#include "Component/ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "BulletStraightMove.h"
#include "Timer.h"
#include "Camera.h"

REGISTER_COMPONENT(TMF::Shot, "Shot");

namespace TMF
{
	void Shot::OnInitialize()
	{
		m_changeTime = m_initChangeTime;
		m_bulletNum = m_bulletMaxNum;
	}
	void Shot::OnFinalize()
	{
	}
	void Shot::OnUpdate()
	{
		auto deltaTime =Timer::Instance().deltaTime.count();
		if (m_isShot == true)
		{
			m_timer += deltaTime;
			if (m_timer > m_coolTime)
			{
				m_isShot = false;
				m_timer = 0.0f;
			}
		}
		if (m_changeTime < m_initChangeTime)
		{
			m_changeTime += Timer::Instance().deltaTime.count();
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
	void Shot::OnLateUpdate()
	{
	}
	void Shot::OnDraw()
	{

	}
	void Shot::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_shotObjectName.c_str());
		auto label = StringHelper::CreateLabel("FileName", m_uuID);
		if (ImGui::InputText(label.c_str(), buf, 256))
		{
			m_shotObjectName = buf;
		}
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
			Play();
		}
	}

	void Shot::Play()
	{
		//auto obj = GameObjectManager::Instance().LoadObject(m_objectFilePath);
		//if (auto lockObj = obj.lock())
		//{

		//}
		if (m_isUsePlayer == true)
		{
			if (m_changeTime < m_initChangeTime)
			{
				return;
			}
		}
		if (m_isShot == true || m_bulletNum == 0)
		{
			return;
		}
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pChildren = pLockOwner->GetChildren();
			for (auto& pChild : pChildren)
			{
				if (auto pLockChild = pChild.lock())
				{
					if (pLockChild->GetName() == m_shotObjectName)
					{
						auto pChildTransform = pLockChild->GetComponent<Transform>();
						if (auto pLockTransform = pChildTransform.lock())
						{
							pLockTransform->SetParent(std::weak_ptr<Transform>());
						}
						auto pBulletMove = pLockChild->GetComponent<BulletStraightMove>();
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
								m_isShot = true;
								m_bulletNum--;
								auto nowPosition = pLockTrancform->GetPosition();
								nowPosition.y += 3;
								auto nowRotation = pLockTrancform->GetRotation();
								auto moveVector = nowPosition - cameraPosition;
								pLockBulletMove->MoveStart(nowPosition, moveVector);
							}
						}
					}
				}
			}
		}
	}

	void Shot::Select()
	{
		m_changeTime = 0;
	}
}