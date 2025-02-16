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
		// チャージ済みなら行わない
		if (m_isChageEnd == true || m_max_jump < m_jump)
		{
			return;
		}
		if (auto pLockThruster = m_pThruster.lock())
		{
			// スタミナが無い状態なら行わない
			if (pLockThruster->GetIsOverHeat() == true)
			{
				return;
			}
		}
		// 溜めている時間を計測
		m_chageTime += Timer::Instance().deltaTime.count();
		// 溜めているフラグに変更
		m_isCharging = true;
		// ジャンプ終了のフラグを変更
		m_isJumpingEnd = false;
		if (m_chageTime >= 1)
		{
			// ジャンプの回数を増やす　ジャンプの回数を制限するため
			m_jump++;
			// 溜め時間を最大値に
			m_chageTime = 1.0f;
			// 溜め終わりのフラグを変更
			m_isChageEnd = true;
			// ジャンプを行うための調整
			JumpSetting();
		}
	}
	void PlayerJump::JumpSetting()
	{
		// 飛ぶ時間を最大値に
		m_flightTime = m_maxFlightTime;
		// ジャンプ中のフラグを変更
		m_isJumping = true;
		// 溜め中のフラグを変更
		m_isCharging = false;
		auto keyState = Input::Instance().GetKeyState();
		// ジャンプの方向を取得
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
		// スタミナを溜めた時間(1秒までの割合で)消費する
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
		// ジャンプ可能でなければ行わない
		if (m_isJumping == false)
		{
			return;
		}
		// 飛行時間を減らす
		m_flightTime -= Timer::Instance().deltaTime.count();
		// 飛行時間が0になれば終わり
		if (m_flightTime <= 0)
		{
			// 飛行時間を最大飛行時間に変更
			m_flightTime = m_maxFlightTime;
			// 飛ぶために溜めるフラグを変更
			m_isChageEnd = false;
			// 溜めた時間をリセット
			m_chageTime = 0.0f;
			// ジャンプのフラグを変更
			m_isJumping = false;
			// ジャンプの終わりフラグを変更
			m_isJumpingEnd = true;
			return;
		}
		if (auto pLockRigidbody = m_pRigidbody.lock())
		{
			// 飛ぶための力を計算　最大ジャンプパワーを最大溜め時間から実際に溜めた時間で計算
			auto jumpForce = float(std::lerp(0, m_maxJumpForce, m_chageTime / m_maxChageTime));
			// ジャンプ用のパラメーター　移動の方向を設定
			auto velocity = m_jumpVector;
			// 高さを変更
			velocity.y = jumpForce;
			// 剛体に移動速度を設定
			pLockRigidbody->SetLinearVelocity(velocity);
		}
	}
}