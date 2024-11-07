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
		void Play();

	private:
		float m_damage = 0;
		std::string m_meleeAnimation = "";
		float endTime = 0;

		SERIALIZE_COMPONENT(m_damage, m_meleeAnimation, endTime);
	};
}

