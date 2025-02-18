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
		void Play();
		void WeaponsUpdate();
		bool GetIsMeleeEnd();

	private:
		void CheckWepons();

	private:
		int m_selectIndex = 0;
		//std::vector<std::weak_ptr<Component>> m_pWepons;
		std::vector<std::weak_ptr<WeaponBase>> m_pWeapons;
		std::vector<int> m_shotIndices;

		//SERIALIZE_COMPONENT();
	};
}

