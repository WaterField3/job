#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class WeponBase : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void Attack();
		void Select();
		inline float GetChangeTime() const { return m_initChangeTime; }
		inline float GetCurrentChangeTime() { return m_changeTime; }
		inline float GetCoolTime() const { return m_coolTime; }
		inline float GetCurrentCollTime() const { return m_timer; }
	protected:
		virtual void OnAttack();
		virtual void OnSelect();

	protected:

		float m_endTime = 0;
		float m_coolTime = 1.0f;
		float m_changeTime = 1.0f;
		float m_initChangeTime = 1.0f;
		float m_timer = 0.0f;

		SERIALIZE_COMPONENT(m_coolTime, m_initChangeTime);

	};
}

