#pragma once

#include "Component.h"
#include "ComponentCerealHelper.h"

namespace TMF
{
	class Thruster : public Component
	{
	public:
		Thruster();
		~Thruster();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void StartThruster();

	private:

		bool m_isThruster = false;
		bool m_isOverHeat = false;
		float m_firstUseMagnification = 1.0f;
		float m_maxThrusterValue = 10.0f;
		float m_thrusterValue = 0.0;
		SERIALIZE_COMPONENT(m_thrusterValue, m_firstUseMagnification);
	};
}