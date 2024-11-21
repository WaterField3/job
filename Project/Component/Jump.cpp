#include "Jump.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Timer.h"

REGISTER_COMPONENT(TMF::Jump, "Jump");

namespace TMF
{
	void Jump::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			m_pTransform = pLockOwner->GetComponent<Transform>();
		}
	}
	void Jump::OnFinalize()
	{
	}
	void Jump::OnUpdate()
	{
		if (m_isChageEnd == true)
		{
			m_flightTime -= 0.1f;
			if (m_flightTime <= 0)
			{
				m_flightTime = m_maxFlightTime;
				m_isChageEnd = false;
				m_chageTime = 0.0f;
				m_moveSpeed = m_maxMoveSpeed;
				m_isChage = true;
				return;
			}
			if (auto pLockRigidbody = m_pRigidbody.lock())
			{
				auto jumpPower = m_jumpVector * m_impact * m_moveSpeed;
				pLockRigidbody->SetLinearVelocity(jumpPower);
			}
			m_moveSpeed = m_moveSpeed * 0.7f;
			if (m_moveSpeed <= 1.0f)
			{
				m_moveSpeed = 1.0f;
			}
		}
	}
	void Jump::OnLateUpdate()
	{
	}
	void Jump::OnDraw()
	{
	}
	void Jump::OnDrawImGui()
	{
		auto impactLabel = StringHelper::CreateLabel("Impact", m_uuID);
		if (ImGui::DragFloat(impactLabel.c_str(), &m_impact))
		{

		}

		auto maxFlightTimeLabel = StringHelper::CreateLabel("MaxFlightTime", m_uuID);
		if (ImGui::DragFloat(maxFlightTimeLabel.c_str(), &m_maxFlightTime))
		{

		}

		auto maxMoveSpeedLabel = StringHelper::CreateLabel("MaxMoveSpeed", m_uuID);
		if (ImGui::DragFloat(maxMoveSpeedLabel.c_str(), &m_maxMoveSpeed))
		{

		}
		auto isChageLabel = StringHelper::CreateLabel("IsChage", m_uuID);
		if (ImGui::Checkbox(isChageLabel.c_str(), &m_isChage))
		{

		}
	}
	void Jump::OnCollisionEnter(GameObject* pGameObject)
	{
		if (pGameObject->GetTag() == GameObject::Tag::Ground)
		{
			m_isChage = false;
		}
	}
	void Jump::Chage(MoveDirection moveDirection)
	{
		if (m_isChageEnd == true || m_isChage == true)
		{
			return;
		}
		m_chageTime += Timer::Instance().deltaTime.count();
		if (m_chageTime >= 1)
		{
			m_chageTime = 1.0f;
			m_isChageEnd = true;
			m_flightTime = m_maxFlightTime;
			m_moveSpeed = m_maxMoveSpeed;
			if (auto pLockTransform = m_pTransform.lock())
			{
				switch (moveDirection)
				{
				case TMF::NEUTRAL:
					m_jumpVector = pLockTransform->GetUp();
					break;
				case TMF::FOWARD:
					m_jumpVector = pLockTransform->GetForward();
					break;
				case TMF::RIGHT:
					m_jumpVector = pLockTransform->GetRight();
					break;
				case TMF::LEFT:
					m_jumpVector = pLockTransform->GetLeft();
					break;
				case TMF::BACK:
					m_jumpVector = pLockTransform->GetBack();
					break;
				default:
					break;
				}
				m_jumpVector.y = pLockTransform->GetUp().y;
			}
			m_moveDirection = moveDirection;
		}
	}
	void Jump::ChageStop(MoveDirection moveDirection)
	{

		if (m_chageTime < 0.1f)
		{
			return;
		}
		m_isChageEnd = true;
		if (auto pLockTransform = m_pTransform.lock())
		{
			switch (moveDirection)
			{
			case TMF::NEUTRAL:
				m_jumpVector = pLockTransform->GetUp();
				break;
			case TMF::FOWARD:
				m_jumpVector = pLockTransform->GetForward();
				break;
			case TMF::RIGHT:
				m_jumpVector = pLockTransform->GetRight();
				break;
			case TMF::LEFT:
				m_jumpVector = pLockTransform->GetLeft();
				break;
			case TMF::BACK:
				m_jumpVector = pLockTransform->GetBack();
				break;
			default:
				break;
			}
			m_jumpVector.y = pLockTransform->GetUp().y;
		}
		m_flightTime = m_maxFlightTime * m_chageTime;
		m_moveSpeed = m_maxMoveSpeed;
		m_moveDirection = moveDirection;
	}
	void Jump::Fall()
	{
		if (m_isChageEnd == true)
		{
			if (auto pLockRigidbody = m_pRigidbody.lock())
			{
				pLockRigidbody->SetLinearVelocity(DirectX::SimpleMath::Vector3::Zero);
			}
		}
	}
}