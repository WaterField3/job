#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class WeaponBase : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		bool Play();
		void Select();
		inline float GetChangeTime() const { return m_initChangeTime; }
		inline float GetCurrentChangeTime() { return m_changeTime; }
		inline float GetCoolTime() const { return m_coolTime; }
		inline float GetCurrentCollTime() const { return m_timer; }
		inline void SetLateTimer(float lateTimer) { m_lateTimer = lateTimer; }
		inline bool GetIsCancel() const { return m_isCancel; }

		// ������
		inline int GetBulletNum() const { return m_bulletNum; }
		inline int GetBulletMaxNum() const { return m_bulletMaxNum; }
		inline float GetReloadTime() const { return m_reloadTime; }
		inline float GetReloadMaxTime() const { return m_reloadMaxTime; }

		// �ߋ���
		void Cancel();
		inline float GetEndTime() const { return m_endTime; }
		inline float GetCancelTime() const { return m_cancelTime; }
		virtual float GetMeleeTime();
		virtual float GetMeleeStopTime();
		inline bool GetIsMeleeEnd() const { return m_isMeleeEnd; }
		inline bool GetIsMelee() const { return m_isMelee; }
	public:
		enum WeaponType
		{
			MELEE,
			SHOT,
		};

		inline WeaponType GetWeaponType() const { return m_weaponType; }

	protected:
		virtual bool OnAttack();
		virtual void OnSelect();
		virtual void OnCancel();
		void OwnerWeponCheck();

	protected:

		float m_timer = 0.0f;
		float m_lateTimer = 0.0f;
		float m_coolTime = 1.0f;
		float m_changeTime = 1.0f;
		float m_initChangeTime = 1.0f;
		bool m_isCancel = false;
		// �ߋ���
		// �A�j���[�V��������
		float m_cancelTime = 0.0f;
		float m_endTime = 0.0f;
		bool m_isMelee = false;
		bool m_isMeleeEnd = false;

		// ������
		int m_bulletNum = -1;
		int m_bulletMaxNum = -1;
		float m_reloadTime = 0.0f;
		float m_reloadMaxTime = 10.0f;

		WeaponType m_weaponType;

		SERIALIZE_COMPONENT(m_coolTime, m_initChangeTime, m_weaponType);

	};
}

