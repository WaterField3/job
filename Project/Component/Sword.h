#pragma once
#include "WeaponBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class MeleeMove;
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
		float GetMeleeStopTime() override;
		bool OnAttack() override;
		void OnSelect() override;
		void OnCancel() override;

	private:
		float m_meleeTimer = 0.0f;
		float m_animationSpeed = 1.0f;
		float m_animationStopTime = 0.0f;
		std::string m_meleeAnimation = "";
		std::weak_ptr<GameObject> m_pParent;
		std::weak_ptr<MeleeMove> m_pMeleeMove;

		SERIALIZE_COMPONENT(m_meleeAnimation, m_endTime, m_animationSpeed, m_cancelTime, m_animationStopTime);
	};
}

