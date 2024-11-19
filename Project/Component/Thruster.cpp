#include "Thruster.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Timer.h"
#include "Transform.h"
#include "Rigidbody.h"

REGISTER_COMPONENT(TMF::Thruster, "Thruster");

namespace TMF
{
	Thruster::Thruster()
	{
	}
	Thruster::~Thruster()
	{
	}
	void Thruster::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			m_pRigidBody = pLockOwner->GetComponent<Rigidbody>();
		}
	}
	void Thruster::OnFinalize()
	{
	}
	void Thruster::OnUpdate()
	{
		if (m_isOverHeat == true || m_isThruster == false)
		{
			m_thrusterValue -= Timer::Instance().deltaTime.count() * 1.5f;;
			if (0 >= m_thrusterValue)
			{
				m_isOverHeat = false;
				m_thrusterValue = 0.0f;
			}
		}

	}
	void Thruster::OnLateUpdate()
	{
	}
	void Thruster::OnDraw()
	{
	}
	void Thruster::OnDrawImGui()
	{
		auto thrusterValueLabel = StringHelper::CreateLabel("ThrusterValue", m_uuID);
		if (ImGui::SliderFloat(thrusterValueLabel.c_str(), &m_thrusterValue, 0.0f, m_maxThrusterValue))
		{

		}
		auto firstUseMagnificationLabel = StringHelper::CreateLabel("FirstUseMagnification", m_uuID);
		if (ImGui::DragFloat(firstUseMagnificationLabel.c_str(), &m_firstUseMagnification, 0.1f, 0.0f, 1.0f))
		{

		}
		auto thrusterMoveSpeedLabel = StringHelper::CreateLabel("ThrusterMoveSpeed", m_uuID);
		if (ImGui::DragFloat(thrusterMoveSpeedLabel.c_str(), &m_thrusterMoveSpeedMagnification))
		{

		}
		auto isDontUseThrusterLabel = StringHelper::CreateLabel("IsUseThruster", m_uuID);
		if (ImGui::Checkbox(isDontUseThrusterLabel.c_str(), &m_isDontUseThruster))
		{

		}
	}
	void Thruster::UseThruster(MoveDirection moveDirection)
	{
		if (m_isThruster == false)
		{
			// ‰‰ñ
			if (auto pLockTransform = m_pTransform.lock())
			{
				switch (moveDirection)
				{
				case TMF::NEUTRAL:
				case TMF::FOWARD:
					m_moveVector = pLockTransform->GetForward();
					break;
				case TMF::RIGHT:
					m_moveVector = pLockTransform->GetRight();
					break;
				case TMF::LEFT:
					m_moveVector = pLockTransform->GetLeft();
					break;
				case TMF::BACK:
					m_moveVector = pLockTransform->GetBack();
					break;
				default:
					break;
				}
			}
			else
			{
				return;
			}

			if (m_maxThrusterValue > m_thrusterValue && m_isOverHeat == false)
			{
				if (auto pLockRigdBody = m_pRigidBody.lock())
				{
					m_moveVector *= m_thrusterMoveSpeedMagnification;
					auto currentVelocity = pLockRigdBody->GetLinearVelocity();
					if (currentVelocity.y < 0)
					{
						m_moveVector.y = currentVelocity.y;
					}
					else
					{
						m_moveVector.y = -1.0f;
					}
					pLockRigdBody->SetLinearVelocity(m_moveVector);
				}
				else
				{
					return;
				}

				m_isThruster = true;
				m_moveDirection = moveDirection;

				if (m_isDontUseThruster == false)
				{
					// ‰ŠúÁ”ï
					m_thrusterValue += m_maxThrusterValue * m_firstUseMagnification;
				}
				if (m_thrusterValue >= m_maxThrusterValue)
				{
					m_isOverHeat = true;
					m_thrusterValue = m_maxThrusterValue;
					m_isThruster = false;
				}
				return;
			}
		}
		else
		{
			if (auto pLockTransform = m_pTransform.lock())
			{
				switch (moveDirection)
				{
				case TMF::FOWARD:
					if (m_moveDirection != moveDirection)
					{
						m_moveVector += pLockTransform->GetForward();
						m_moveDirection = moveDirection;
					}
					break;
				case TMF::RIGHT:
					if (m_moveDirection != moveDirection)
					{
						m_moveVector += pLockTransform->GetRight();
						m_moveDirection = moveDirection;
					}
					break;
				case TMF::LEFT:
					if (m_moveDirection != moveDirection)
					{
						m_moveVector += pLockTransform->GetLeft();
						m_moveDirection = moveDirection;
					}
					break;
				case TMF::BACK:
					if (m_moveDirection != moveDirection)
					{
						m_moveVector += pLockTransform->GetBack();
						m_moveDirection = moveDirection;
					}
					break;
				default:
					break;
				}
			}
			// Œp‘±Žž
			if (auto pLockRigdBody = m_pRigidBody.lock())
			{
				pLockRigdBody->SetLinearVelocity(m_moveVector);
				if (m_isDontUseThruster == false)
				{
					m_thrusterValue += 0.05f;
					if (m_thrusterValue >= m_maxThrusterValue)
					{
						m_isOverHeat = true;
						m_isThruster = false;
					}
				}
			}
		}
	}
	void Thruster::StopUseThruster()
	{
		if (auto pLockRigidBody = m_pRigidBody.lock())
		{
			auto velocity = pLockRigidBody->GetLinearVelocity();
			if (velocity != DirectX::SimpleMath::Vector3::Zero)
			{
				velocity.x *= 0.9f;
				velocity.z *= 0.9f;
				if (DirectX::SimpleMath::Vector3::Distance(velocity, DirectX::SimpleMath::Vector3::Zero) < 0.1f)
				{
					velocity = DirectX::SimpleMath::Vector3::Zero;
				}
				pLockRigidBody->SetLinearVelocity(velocity);
			}
		}
		if (m_isThruster == true)
		{
			m_isThruster = false;
		}
	}
	void Thruster::DodgeUseThruster(float useMagnification)
	{
		m_thrusterValue += m_maxThrusterValue * useMagnification;
		if (m_thrusterValue >= m_maxThrusterValue)
		{
			m_isOverHeat = true;
			m_thrusterValue = m_maxThrusterValue;
		}
	}
}