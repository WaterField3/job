#pragma once

#include "Component.h"

#include "ComponentCerealHelper.h"
#include "MoveInfo.h"

namespace TMF
{
	class Rigidbody;
	class Transform;
	class Thruster;
	class Jump : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnCollisionEnter(GameObject* pGameObject) override;
		void Chage(MoveDirection moveDirection);
		void ChageStop(MoveDirection moveDirection);
		void Fall();

		inline int GetMaxJumpNum() { return m_maxJumpNum; }
		inline bool GetIsChageEnd() { return m_isChageEnd; }
		inline bool GetIsLanding() { return m_IsLanding; }
		inline float GetImpact() { return m_impact; }
		inline float GetMoveSpeed() { return m_moveSpeed; }
		inline float GetMaxMoveSpeed() { return m_maxMoveSpeed; }
		inline float GetMaxFlightTime() { return m_maxFlightTime; }

	private:
		int m_maxJumpNum = 1;
		int m_jumpNum = 0;
		// �`���[�W�������ԁi�����j
		float m_chageTime = 0;
		float m_impact = 1.0f;
		float m_maxFlightTime = 10.0f;
		float m_flightTime = 1.0f;
		float m_maxMoveSpeed = 2.0f;
		float m_moveSpeed = 1.0f;
		bool m_isChage = false;
		bool m_isChageEnd = false;
		bool m_isFlight = false;
		bool m_isFall = false;
		bool m_IsLanding = false;
		DirectX::SimpleMath::Vector3 m_jumpVector = DirectX::SimpleMath::Vector3::Zero;
		MoveDirection m_moveDirection = MoveDirection::NEUTRAL;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Thruster> m_pThruster;
		SERIALIZE_COMPONENT(m_impact, m_maxFlightTime, m_maxMoveSpeed, m_maxJumpNum)
	};
}
