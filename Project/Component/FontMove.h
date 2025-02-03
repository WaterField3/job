#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Font;
	class FontMove : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

	private:
		float m_maxMoveSpeed = 1.0f;
		float m_moveTime = 1.0f;
		DirectX::SimpleMath::Vector2 m_targetPosition = DirectX::SimpleMath::Vector2::Zero;
		std::weak_ptr<Font> m_pFont;

		SERIALIZE_COMPONENT(m_maxMoveSpeed, m_moveTime, m_targetPosition);

	};
}

