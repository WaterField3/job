#include "Dodge.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Timer.h"
#include "Animater.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "PlayerStatus.h"
#include "Thruster.h"

REGISTER_COMPONENT(TMF::Dodge, "Dodge");

namespace TMF
{
	void Dodge::OnInitialize()
	{
	}
	void Dodge::OnFinalize()
	{
	}
	void Dodge::OnUpdate()
	{
		// ������Ă��邩
		if (m_isDodgeMove == true)
		{
			if (m_timer > m_dodgeTime)
			{
				m_isDodgeMove = false;
				m_timer = 0.0f;

						m_isDodgeEnd = true;
				return;
			}
			m_timer += Timer::Instance().deltaTime.count();
		}
	}
	void Dodge::OnLateUpdate()
	{
		if (m_isDodgeMove == true)
		{
			if (m_timer > m_dodgeTime)
			{
				if (auto pLockOwner = m_pOwner.lock())
				{
					// �ړ����~�߂�@�������Ȃ���
					auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
					if (auto pLockRigidbody = pRigidbody.lock())
					{
						pLockRigidbody->SetLinearVelocity(DirectX::SimpleMath::Vector3::Zero);
					}
					auto pPlayerStatus = pLockOwner->GetComponent<PlayerStatus>();
					if (auto pLockPlayerStatus = pPlayerStatus.lock())
					{
						pLockPlayerStatus->SetIsInvincible(false);
					}
				}
				return;
			}
			// ���W�̈ړ�
			if (auto pLockOwner = m_pOwner.lock())
			{
				// �����ō��W���ړ�����
				auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
				if (auto pLockRigidbody = pRigidbody.lock())
				{
					pLockRigidbody->ApplyImpulse(m_dodgeMoveVector, DirectX::SimpleMath::Vector3::Zero);
					pLockRigidbody->SetAngularVelocity(DirectX::SimpleMath::Vector3::Zero);
				}
			}
		}
	}
	void Dodge::OnDraw()
	{
	}
	void Dodge::OnDrawImGui()
	{
		// �ړ����Ȃ��A�j���[�V�������g�p���Ă�������
		// �O�����̉���A�j���[�V�����̃p�X�̐ݒ�
		char forwardBuf[256] = "";
		strcpy_s(forwardBuf, sizeof(forwardBuf), m_fowardDodge.c_str());
		auto forwardDodgeLabel = StringHelper::CreateLabel("ForwardDodge", m_uuID);
		if (ImGui::InputText(forwardDodgeLabel.c_str(), forwardBuf, 256))
		{
			m_fowardDodge = forwardBuf;
		}
		// �E�����̉���A�j���[�V�����̃p�X�̐ݒ�
		char rightBuf[256] = "";
		strcpy_s(rightBuf, sizeof(rightBuf), m_rightDodge.c_str());
		auto rightDodgeLabel = StringHelper::CreateLabel("rightDodge", m_uuID);
		if (ImGui::InputText(rightDodgeLabel.c_str(), rightBuf, 256))
		{
			m_rightDodge = rightBuf;
		}
		// �������̉���A�j���[�V�����̃p�X�̐ݒ�
		char leftBuf[256] = "";
		strcpy_s(leftBuf, sizeof(leftBuf), m_leftDodge.c_str());
		auto leftDodgeLabel = StringHelper::CreateLabel("LeftDodge", m_uuID);
		if (ImGui::InputText(leftDodgeLabel.c_str(), leftBuf, 256))
		{
			m_leftDodge = leftBuf;
		}
		// �������̉���A�j���[�V�����̃p�X�̐ݒ�
		char backBuf[256] = "";
		strcpy_s(backBuf, sizeof(backBuf), m_backDodge.c_str());
		auto backDodgeLabel = StringHelper::CreateLabel("BackDodge", m_uuID);
		if (ImGui::InputText(backDodgeLabel.c_str(), backBuf, 256))
		{
			m_backDodge = backBuf;
		}
		// ����A�j���[�V�����̍Đ�����
		auto dodgeTimeLabl = StringHelper::CreateLabel("DodgeTime", m_uuID);
		if (ImGui::DragFloat(dodgeTimeLabl.c_str(), &m_dodgeTime))
		{

		}
		// �ړ��̑��x�{��
		auto moveMagnificationLabel = StringHelper::CreateLabel("MoveMagnification", m_uuID);
		if (ImGui::DragFloat(moveMagnificationLabel.c_str(), &m_moveMagnification, 0.1f))
		{

		}
		// ������Ɏg�p����X���X�^�[�̊���
		auto dodgeUseThrusterLabel = StringHelper::CreateLabel("DodgeUseThruster", m_uuID);
		if (ImGui::DragFloat(dodgeUseThrusterLabel.c_str(), &m_dodgeUseThrusterMagnification, 0.1f, 0.0f, 1.0f))
		{

		}
	}
	std::shared_ptr<Component> Dodge::OnClone() const
	{
		auto pClone = std::make_shared<Dodge>();
		pClone->m_fowardDodge = this->m_fowardDodge;
		pClone->m_rightDodge = this->m_rightDodge;
		pClone->m_leftDodge = this->m_leftDodge;
		pClone->m_backDodge = this->m_backDodge;
		pClone->m_moveMagnification = this->m_moveMagnification;
		pClone->m_dodgeUseThrusterMagnification = this->m_dodgeUseThrusterMagnification;
		return move(pClone);
	}
	bool Dodge::DodgeStart(MoveDirection direction)
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pThruster = pLockOwner->GetComponent<Thruster>();
			if (auto pLockThruster = pThruster.lock())
			{
				if (pLockThruster->GetIsOverHeat() == false)
				{
					pLockThruster->UseThruster(m_dodgeUseThrusterMagnification);
				}
				else
				{
					return false;
				}
			}

			auto pAnimater = pLockOwner->GetComponent<Animater>();
			auto dodgePath = m_fowardDodge;

			if (auto pLockAnimater = pAnimater.lock())
			{
				auto pTransform = pLockOwner->GetComponent<Transform>();
				if (auto pLockTransform = pTransform.lock())
				{
					switch (direction)
					{
					case MoveDirection::NEUTRAL:
					case MoveDirection::FOWARD:
						m_dodgeDirection = MoveDirection::FOWARD;
						m_dodgeMoveVector = pLockTransform->GetForward();
						break;
					case MoveDirection::RIGHT:
						m_dodgeDirection = MoveDirection::RIGHT;
						m_dodgeMoveVector = pLockTransform->GetRight();
						dodgePath = m_rightDodge;
						break;
					case MoveDirection::LEFT:
						m_dodgeDirection = MoveDirection::LEFT;
						m_dodgeMoveVector = pLockTransform->GetLeft();
						dodgePath = m_leftDodge;
						break;
					case MoveDirection::BACK:
						m_dodgeDirection = MoveDirection::BACK;
						m_dodgeMoveVector = pLockTransform->GetBack();
						dodgePath = m_backDodge;
						break;
					default:
						break;
					}
					m_dodgeMoveVector *= m_moveMagnification;
					pLockAnimater->SetFileName(dodgePath, m_dodgeTime);
					m_isDodgeMove = true;
					m_isDodgeEnd = false;
				}
			}

			if (m_isDodgeMove == true)
			{
				auto pPlayerStatus = pLockOwner->GetComponent<PlayerStatus>();
				if (auto pLockPlayerStatus = pPlayerStatus.lock())
				{
					pLockPlayerStatus->SetIsInvincible(true);
				}
			}
			return true;
		}
		return false;
	}
}