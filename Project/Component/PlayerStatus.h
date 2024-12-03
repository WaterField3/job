#pragma once

#include "Component.h"

#include "ComponentCerealHelper.h"
#include "State/StateMachine.h"

namespace TMF
{
	class Animater;
	class Collider;
	class PlayerStatus : public Component
	{
	public:
		PlayerStatus();
		~PlayerStatus();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void Stagger();
		void Invert();
		void Damage(float damage);
		inline bool GetIsInvincible() const { return m_isInvincible; }
		inline void SetIsInvincible(bool isInvincible) { m_isInvincible = isInvincible; }

	private:
		bool m_isInvincible = false;
		float m_hp = 100;
		float m_staggerAnimEndTime = 1.0f;
		float m_invertAnimEndTime = 1.0f;
		float m_timer = 0.0f;
		float m_invincibleTime = 1.0f;
		float m_standUpTime = 1.0f;
		DirectX::SimpleMath::Vector3 m_initCenter = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_initScale = DirectX::SimpleMath::Vector3::One;
		DirectX::SimpleMath::Vector3 m_staggerCenter = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_staggerScale = DirectX::SimpleMath::Vector3::One;
		std::string m_staggerAnimPath = "";
		std::string m_invertAnimPath = "";
		std::string m_standUpAnimPath = "";
		std::weak_ptr<Animater> m_pAnimater;
		std::weak_ptr<Collider> m_pCollider;
		std::unique_ptr<StateMachine> m_pStateMachine;
		// 9  10
		SERIALIZE_COMPONENT(m_hp, m_staggerAnimPath, m_invertAnimPath, m_invertAnimEndTime, m_staggerAnimEndTime, m_standUpAnimPath, m_standUpTime, m_staggerCenter, m_staggerScale);
	};
}
