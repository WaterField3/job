#pragma once
#include "WeponBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Gun : public WeponBase
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnAttack() override;
		void OnSelect() override;
		inline int GetBulletNum() const { return m_bulletNum; }
		inline int GetBulletMaxNum() const { return m_bulletMaxNum; }
		inline float GetChangeTime() const { return m_initChangeTime; }
		inline float GetCurrentChangeTime() const { return m_changeTime; }
		inline float GetCoolTime() const { return m_coolTime; }
		inline float GetCurrentCollTime() const { return m_timer; }
		inline float GetReloadTime() const { return m_reloadTime; }
		inline float GetReloadMaxTime() const { return m_reloadMaxTime; }

	private:
		bool m_isShot = false;
		bool m_isUsePlayer = false;
		SERIALIZE_COMPONENT(m_coolTime, m_initChangeTime, m_reloadMaxTime, m_bulletMaxNum);

	};
}

