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
		float m_endTime = 0;
		float m_coolTime = 1.0f;
		float m_timer = 0.0f;
		bool m_isMelee = false;
		std::string m_meleeAnimation = "";
		std::string m_meleeObjectName = "";


		SERIALIZE_COMPONENT(m_meleeAnimation, m_endTime, m_meleeObjectName);
	};
}

