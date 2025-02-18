#pragma once
#include "WeaponBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Sword : public WeaponBase
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		float GetMeleeTime() override;
		void OnAttack() override;
		void OnSelect() override;

	private:
		float m_meleeTimer = 0.0f;
		float m_animationSpeed = 1.0f;
		std::string m_meleeAnimation = "";

		SERIALIZE_COMPONENT(m_meleeAnimation, m_endTime, m_animationSpeed, m_cancelTime);
	};
}

