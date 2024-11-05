#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Destroy :	public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void Play(float time = 0.0f);

	private:
		float m_destroyTime = 0.0f;

		SERIALIZE_COMPONENT(m_destroyTime);
	};
}

