#pragma once
#include "Component.h"

#include <string>

#include "Component/ComponentCerealHelper.h"

namespace TMF
{
	class Shot : public Component
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
		float m_coolTime = 1.0f;
		float m_timer = 0.0f;
		bool m_isShot = false;
		std::string m_objectFilePath = "";
		SERIALIZE_COMPONENT(m_objectFilePath, m_coolTime);
	};
}

