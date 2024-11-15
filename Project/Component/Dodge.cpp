#include "Dodge.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Animater.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Timer.h"

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
		// 回避しているか
		if (m_isDodgeMove == true)
		{
			if (m_timer > m_dodgeTime)
			{
				m_isDodgeMove = false;
				m_timer = 0.0f;
				if (auto pLockOwner = m_pOwner.lock())
				{
					// 移動を止める　慣性をなくす
					auto pRigidBody = pLockOwner->GetComponent<Rigidbody>();
					if (auto pLockRigidBody = pRigidBody.lock())
					{
						pLockRigidBody->SetLinearVelocity(DirectX::SimpleMath::Vector3::Zero);
					}
				}
				return;
			}
			m_timer += Timer::Instance().deltaTime.count();
			// 座標の移動
			if (auto pLockOwner = m_pOwner.lock())
			{
				// 移動のために向きを取得
				auto pTransform = pLockOwner->GetComponent<Transform>();
				if (auto pLockTransform = pTransform.lock())
				{
					auto moveDirection = DirectX::SimpleMath::Vector3::Zero;
					// 物理で座標を移動する
					auto pRigidBody = pLockOwner->GetComponent<Rigidbody>();
					if (auto pLockRigidBody = pRigidBody.lock())
					{
						switch (m_dodgeDirection)
						{
						case TMF::Dodge::FOWARD:
							moveDirection = pLockTransform->GetForward();
							break;
						case TMF::Dodge::RIGHT:
							moveDirection = pLockTransform->GetRight();
							break;
						case TMF::Dodge::LEFT:
							moveDirection = pLockTransform->GetLeft();
							break;
						case TMF::Dodge::BACK:
							moveDirection = pLockTransform->GetBack();
							break;
						default:
							break;
						}
						moveDirection *= m_moveMagnification;
						pLockRigidBody->ApplyImpulse(moveDirection, DirectX::SimpleMath::Vector3::Zero);
					}
				}

			}
		}
	}
	void Dodge::OnLateUpdate()
	{
	}
	void Dodge::OnDraw()
	{
	}
	void Dodge::OnDrawImGui()
	{
		// 移動しないアニメーションを使用してください
		// 前方向の回避アニメーションのパスの設定
		char forwardBuf[256] = "";
		strcpy_s(forwardBuf, sizeof(forwardBuf), m_fowardDodge.c_str());
		auto forwardDodgeLabel = StringHelper::CreateLabel("ForwardDodge", m_uuID);
		if (ImGui::InputText(forwardDodgeLabel.c_str(), forwardBuf, 256))
		{
			m_fowardDodge = forwardBuf;
		}
		// 右方向の回避アニメーションのパスの設定
		char rightBuf[256] = "";
		strcpy_s(rightBuf, sizeof(rightBuf), m_rightDodge.c_str());
		auto rightDodgeLabel = StringHelper::CreateLabel("rightDodge", m_uuID);
		if (ImGui::InputText(rightDodgeLabel.c_str(), rightBuf, 256))
		{
			m_rightDodge = rightBuf;
		}
		// 左方向の回避アニメーションのパスの設定
		char leftBuf[256] = "";
		strcpy_s(leftBuf, sizeof(leftBuf), m_leftDodge.c_str());
		auto leftDodgeLabel = StringHelper::CreateLabel("LeftDodge", m_uuID);
		if (ImGui::InputText(leftDodgeLabel.c_str(), leftBuf, 256))
		{
			m_leftDodge = leftBuf;
		}
		// 後ろ方向の回避アニメーションのパスの設定
		char backBuf[256] = "";
		strcpy_s(backBuf, sizeof(backBuf), m_backDodge.c_str());
		auto backDodgeLabel = StringHelper::CreateLabel("BackDodge", m_uuID);
		if (ImGui::InputText(backDodgeLabel.c_str(), backBuf, 256))
		{
			m_backDodge = backBuf;
		}
		// 回避アニメーションの再生時間
		auto dodgeTimeLabl = StringHelper::CreateLabel("DodgeTime", m_uuID);
		if (ImGui::DragFloat(dodgeTimeLabl.c_str(), &m_dodgeTime))
		{

		}
		// 移動の速度倍率
		auto moveMagnificationLabel = StringHelper::CreateLabel("MoveMagnification", m_uuID);
		if (ImGui::DragFloat(moveMagnificationLabel.c_str(), &m_moveMagnification, 0.1f))
		{

		}
	}
	void Dodge::DodgeStart(Direction direction)
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pAnimater = pLockOwner->GetComponent<Animater>();
			auto dodgePath = m_fowardDodge;

			if (auto pLockAnimater = pAnimater.lock())
			{
				switch (direction)
				{
				case TMF::Dodge::FOWARD:
					m_dodgeDirection = Direction::FOWARD;
					break;
				case TMF::Dodge::RIGHT:
					m_dodgeDirection = Direction::RIGHT;
					dodgePath = m_rightDodge;
					break;
				case TMF::Dodge::LEFT:
					m_dodgeDirection = Direction::LEFT;
					dodgePath = m_leftDodge;
					break;
				case TMF::Dodge::BACK:
					m_dodgeDirection = Direction::BACK;
					dodgePath = m_backDodge;
					break;
				default:
					break;
				}
				pLockAnimater->SetFileName(dodgePath, m_dodgeTime);
				m_isDodgeMove = true;
			}

		}

	}
}