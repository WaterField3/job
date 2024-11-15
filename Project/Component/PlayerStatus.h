#pragma once

#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Animater;
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

	private:
		bool m_isInvincible = false;
		float m_hp = 100;
		float m_staggerAnimEndTime = 1.0f;
		float m_invertAnimEndTime = 1.0f;
		float m_timer = 0.0f;
		float m_invincibleTime = 1.0f;
		std::string m_staggerAnimPath = "";
		std::string m_invertAnimPath = "";
		std::string m_standUpAnimPath = "";
		std::weak_ptr<Animater> m_pAnimater;
		
		SERIALIZE_COMPONENT(m_hp, m_staggerAnimPath, m_invertAnimPath, m_invertAnimEndTime, m_staggerAnimEndTime, m_standUpAnimPath);
	};
}

