#include "Jump.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Timer.h"
#include "Thruster.h"

REGISTER_COMPONENT(TMF::Jump, "Jump");

namespace TMF
{
	void Jump::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			m_pTransform = pLockOwner->GetComponent<Transform>();
			m_pThruster = pLockOwner->GetComponent<Thruster>();
		}
	}
	void Jump::OnFinalize()
	{
	}
	void Jump::OnUpdate()
	{
		if (m_isFlight == true)
		{
			if (m_flightTime > 0.0f)
			{
				m_flightTime -= Timer::Instance().deltaTime.count();
			}
			auto fall = false;
			if (m_flightTime <= 0)
			{
				m_jumpVector = DirectX::SimpleMath::Vector3::Zero;
				m_isChageEnd = false;
				//m_chageTime = 0.0f;
				//m_isChage = true;
				fall = true;
			}
			m_moveSpeed = m_moveSpeed * 0.7f;
			if (m_moveSpeed <= 1.0f)
			{
				m_moveSpeed = 1.0f;
			}
			if (auto pLockTransfrom = m_pTransform.lock())
			{
				auto start = pLockTransfrom->GetPosition();
				auto end = start - DirectX::SimpleMath::Vector3(0.0f, 0.5f, 0.0f);
				if (PhysicsManager::Instance().RayCastHit(start, end) && fall == true)
				{
					m_isFlight = false;
					m_isFall = false;
					m_IsLanding = true;
					m_jumpNum = 0;
				}
			}
		}
		else
		{
			m_IsLanding = false;
		}
	}
	void Jump::OnLateUpdate()
	{
		if (m_flightTime > 0.0f && m_isFlight == true)
		{
			if (auto pLockRigidbody = m_pRigidbody.lock())
			{
				auto jumpPower = m_jumpVector * m_impact * m_moveSpeed;
				pLockRigidbody->SetLinearVelocity(jumpPower);
			}
		}
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

		auto maxJumpNumLabel = StringHelper::CreateLabel("MaxJumpNum", m_uuID);
		if (ImGui::DragInt(maxJumpNumLabel.c_str(), &m_maxJumpNum, 0))
		{

		}

		auto chageTimeLabel = StringHelper::CreateLabel("ChageTime", m_uuID);
		if (ImGui::DragFloat(chageTimeLabel.c_str(), &m_chageTime))
		{

		}

		auto jumpNumLabel = StringHelper::CreateLabel("JumpNum", m_uuID);
		if (ImGui::DragInt(jumpNumLabel.c_str(), &m_jumpNum))
		{

		}

		auto isChageLabel = StringHelper::CreateLabel("IsChage", m_uuID);
		if (ImGui::Checkbox(isChageLabel.c_str(), &m_isChage))
		{

		}
	}
	std::shared_ptr<Component> Jump::OnClone() const
	{
		// m_impact, m_maxFlightTime, m_maxMoveSpeed
		auto pClone = std::make_shared<Jump>();
		pClone->m_impact = this->m_impact;
		pClone->m_maxFlightTime = this->m_maxFlightTime;
		pClone->m_maxMoveSpeed = this->m_maxMoveSpeed;
		return move(pClone);
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
		//if (m_isChageEnd == true)
		//{
		//	return;
		//}

		if (m_jumpNum >= m_maxJumpNum)
		{
			return;
		}

		if (auto pLockThruster = m_pThruster.lock())
		{
			if (pLockThruster->GetIsOverHeat() == true)
			{
				return;
			}
		}

		m_chageTime += Timer::Instance().deltaTime.count();
		m_isChage = true;
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
			m_isFlight = true;
			if (auto pLockThruster = m_pThruster.lock())
			{
				pLockThruster->UseThruster(m_chageTime);
				m_chageTime = 0.0f;
			}
		}
	}
	void Jump::ChageStop(MoveDirection moveDirection)
	{

		if (m_chageTime < 0.1f || m_isChage == false)
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
		m_flightTime += m_maxFlightTime * m_chageTime;
		m_moveSpeed = m_maxMoveSpeed;
		m_moveDirection = moveDirection;
		m_isFlight = true;
		m_isChage = false;
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->UseThruster(m_chageTime);
		}
		m_chageTime = 0.0f;
		m_jumpNum++;
	}
	void Jump::Fall()
	{
		if (m_isFlight == true && m_isFall == false)
		{
			if (auto pLockRigidbody = m_pRigidbody.lock())
			{
				pLockRigidbody->SetLinearVelocity(DirectX::SimpleMath::Vector3::Zero);
				pLockRigidbody->ClearForces();
				m_jumpVector = DirectX::SimpleMath::Vector3::Zero;
				m_isFall = true;
				//m_isChageEnd = false;
			}
		}
	}
}