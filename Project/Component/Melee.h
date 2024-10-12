#pragma once

#include "Component.h"

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class Melee : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		float m_damage = 0;

		SERIALIZE_COMPONENT(m_damage);
	};
}

