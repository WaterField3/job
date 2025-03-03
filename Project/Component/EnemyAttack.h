#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class WeaponBase;
	class EnemyAttack : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void SelectShot();
		void SelectMelee();
		bool Play();
		void WeaponsUpdate();
		bool GetIsMeleeEnd();
		float GetEndTime();
		inline float GetShotDinstance() const { return m_shotDistance; }

	private:
		void CheckWepons();

	private:
		int m_selectIndex = 0;
		float m_shotDistance = 3.0f;
		std::vector<std::weak_ptr<WeaponBase>> m_pWeapons;
		std::vector<int> m_shotIndices;
		//SERIALIZE_COMPONENT();
	};
}

