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
		std::string m_objectFilePath = "";
		SERIALIZE_COMPONENT(m_objectFilePath);
	};
}

