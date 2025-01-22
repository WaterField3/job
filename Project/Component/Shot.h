#pragma once
#include "Component.h"

#include <string>

#include "Component/ComponentCerealHelper.h"

namespace TMF
{
	class Shot : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void Play();
		void Select();
		inline int GetBulletNum() const { return m_bulletNum; }
		inline int GetBulletMaxNum() const { return m_bulletMaxNum; }
		inline float GetChangeTime() const { return m_initChangeTime; }
		inline float GetCurrentChangeTime() const { return m_changeTime; }
		inline float GetCoolTime() const { return m_coolTime; }
		inline float GetCurrentCollTime() const { return m_timer; }
		inline std::string GetShotObjectName() const { return m_shotObjectName; }

	private:
		int m_bulletNum = 1;
		int m_bulletMaxNum = 10;
		float m_coolTime = 1.0f;
		float m_timer = 0.0f;
		float m_changeTime = 1.0f;
		float m_initChangeTime = 1.0f;
		float m_reloadTime = 1.0f;
		float m_reloadMaxTime = 10.0f;
		bool m_isShot = false;
		bool m_isUsePlayer = false;
		std::string m_shotObjectName = "";
		SERIALIZE_COMPONENT(m_shotObjectName, m_coolTime, m_initChangeTime, m_reloadMaxTime, m_bulletMaxNum, m_isUsePlayer);
	};
}

