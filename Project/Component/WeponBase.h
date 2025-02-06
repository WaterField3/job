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
		void Play();
		void Select();
		inline float GetChangeTime() const { return m_initChangeTime; }
		inline float GetCurrentChangeTime() { return m_changeTime; }
		inline float GetCoolTime() const { return m_coolTime; }
		inline float GetCurrentCollTime() const { return m_timer; }

		// 遠距離
		inline int GetBulletNum() const { return m_bulletNum; }
		inline int GetBulletMaxNum() const { return m_bulletMaxNum; }
		inline float GetReloadTime() const { return m_reloadTime; }
		inline float GetReloadMaxTime() const { return m_reloadMaxTime; }

		// 近距離
		inline float GetEndTime() const { return m_endTime; }
	protected:
		virtual void OnAttack();
		virtual void OnSelect();

	protected:

		float m_timer = 0.0f;
		float m_coolTime = 1.0f;
		float m_changeTime = 1.0f;
		float m_initChangeTime = 1.0f;
		// 近距離
		// アニメーション時間
		float m_endTime = 0;

		// 遠距離
		int m_bulletNum = -1;
		int m_bulletMaxNum = -1;
		float m_reloadTime = 0.0f;
		float m_reloadMaxTime = 10.0f;

		SERIALIZE_COMPONENT(m_coolTime, m_initChangeTime);

	};
}

