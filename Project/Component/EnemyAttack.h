#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class EnemyAttack : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void SelectShot();
		void SelectMelee();
		void Play();
		bool GetIsMeleeEnd();

	private:
		int m_selectIndex = 0;
		std::vector<std::weak_ptr<Component>> m_pWepons;
		std::vector<int> m_shotIndices;

		//SERIALIZE_COMPONENT();
	};
}

