#pragma once
#include "Component.h"

namespace TMF
{
	class Melee;
	class Shot;
	class Attack : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
	private:
		int m_previousScrollValue = 0;
		int m_selectIndex = 0;
		std::vector < std::weak_ptr<Component>> m_pWepons;
	};
}

