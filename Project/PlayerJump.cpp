#include "PlayerJump.h"

#include "Component/Rigidbody.h"
#include "Component/Transform.h"
#include "Component/Thruster.h"
#include "Component/Jump.h"
#include "Input.h"
#include "Timer.h"
#include <cmath>

namespace TMF
{
	PlayerJump::PlayerJump()
	{
		m_jump = 0;
	}
	PlayerJump::PlayerJump(std::weak_ptr<Transform> pTransform, std::weak_ptr<Rigidbody> pRigidbody, std::weak_ptr<Thruster> pThruster, std::weak_ptr<Jump> pJump)
	{
		m_pTransform = pTransform;
		m_pRigidbody = pRigidbody;
		m_pThruster = pThruster;
		if (auto pLockJump = pJump.lock())
		{
			m_impact = pLockJump->GetImpact();
			m_moveSpeed = pLockJump->GetMoveSpeed();
			m_maxFlightTime = pLockJump->GetMaxFlightTime();
			m_maxMoveSpeed = pLockJump->GetMaxMoveSpeed();
			m_max_jump = pLockJump->GetMaxJumpNum();
		}
		m_jumpVector = DirectX::SimpleMath::Vector3::Up;
	}
	PlayerJump::~PlayerJump()
	{

	}
	void PlayerJump::Chage()
	{
		// �`���[�W�ς݂Ȃ�s��Ȃ�
		if (m_isChageEnd == true || m_max_jump < m_jump)
		{
			return;
		}
		if (auto pLockThruster = m_pThruster.lock())
		{
			// �X�^�~�i��������ԂȂ�s��Ȃ�
			if (pLockThruster->GetIsOverHeat() == true)
			{
				return;
			}
		}
		// ���߂Ă��鎞�Ԃ��v��
		m_chageTime += Timer::Instance().deltaTime.count();
		// ���߂Ă���t���O�ɕύX
		m_isCharging = true;
		// �W�����v�I���̃t���O��ύX
		m_isJumpingEnd = false;
		if (m_chageTime >= 1)
		{
			// �W�����v�̉񐔂𑝂₷�@�W�����v�̉񐔂𐧌����邽��
			m_jump++;
			// ���ߎ��Ԃ��ő�l��
			m_chageTime = 1.0f;
			// ���ߏI���̃t���O��ύX
			m_isChageEnd = true;
			// �W�����v���s�����߂̒���
			JumpSetting();
		}
	}
	void PlayerJump::JumpSetting()
	{
		// ��Ԏ��Ԃ��ő�l��
		m_flightTime = m_maxFlightTime;
		// �W�����v���̃t���O��ύX
		m_isJumping = true;
		// ���ߒ��̃t���O��ύX
		m_isCharging = false;
		auto keyState = Input::Instance().GetKeyState();
		// �W�����v�̕������擾
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (keyState.W)
			{
				m_jumpVector = pLockTransform->GetForward();
			}
			else if (keyState.A)
			{
				m_jumpVector = pLockTransform->GetLeft();
			}
			else if (keyState.S)
			{
				m_jumpVector = pLockTransform->GetBack();
			}
			else if (keyState.D)
			{
				m_jumpVector = pLockTransform->GetRight();
			}
		}
		// �X�^�~�i�𗭂߂�����(1�b�܂ł̊�����)�����
		if (auto pLockThruster = m_pThruster.lock())
		{
			pLockThruster->UseThruster(m_chageTime);
		}
	}
	void PlayerJump::ChageEnd()
	{
		if (m_isCharging == true)
		{
			m_isJumpingEnd = false;
			m_jump++;
			JumpSetting();
		}
	}
	void PlayerJump::Flight()
	{
		// �W�����v�\�łȂ���΍s��Ȃ�
		if (m_isJumping == false)
		{
			return;
		}
		// ��s���Ԃ����炷
		m_flightTime -= Timer::Instance().deltaTime.count();
		// ��s���Ԃ�0�ɂȂ�ΏI���
		if (m_flightTime <= 0)
		{
			// ��s���Ԃ��ő��s���ԂɕύX
			m_flightTime = m_maxFlightTime;
			// ��Ԃ��߂ɗ��߂�t���O��ύX
			m_isChageEnd = false;
			// ���߂����Ԃ����Z�b�g
			m_chageTime = 0.0f;
			// �W�����v�̃t���O��ύX
			m_isJumping = false;
			// �W�����v�̏I���t���O��ύX
			m_isJumpingEnd = true;
			return;
		}
		if (auto pLockRigidbody = m_pRigidbody.lock())
		{
			// ��Ԃ��߂̗͂��v�Z�@�ő�W�����v�p���[���ő嗭�ߎ��Ԃ�����ۂɗ��߂����ԂŌv�Z
			auto jumpForce = float(std::lerp(0, m_maxJumpForce, m_chageTime / m_maxChageTime));
			// �W�����v�p�̃p�����[�^�[�@�ړ��̕�����ݒ�
			auto velocity = m_jumpVector;
			// ������ύX
			velocity.y = jumpForce;
			// ���̂Ɉړ����x��ݒ�
			pLockRigidbody->SetLinearVelocity(velocity);
		}
	}
}