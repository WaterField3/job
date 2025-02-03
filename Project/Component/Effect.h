#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <SimpleMath.h>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class Effect : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void Play();
	private:
		DirectX::SimpleMath::Vector3 m_effectPos = DirectX::SimpleMath::Vector3::Zero;
		std::string m_effectPath = "";

		SERIALIZE_COMPONENT(m_effectPath, m_effectPos);
	};
}
