#pragma once

#include "Component.h"

#include <SpriteFont.h>

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Font : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		inline DirectX::SimpleMath::Vector2 GetFontPosition() const { return m_spriteFontPos; }
		inline void SetFonstPosition(DirectX::SimpleMath::Vector2 position) { m_spriteFontPos = position; }
		inline void SetText(std::string text) { m_text = text; }

	private:
		float m_fontScale = 1.0f;
		DirectX::SimpleMath::Vector2 m_spriteFontPos;
		DirectX::SimpleMath::Color m_fontColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
		std::string m_text = "";
		std::string m_fontPath = "asset/myfile.spritefont";
		std::unique_ptr<DirectX::SpriteFont> m_pSpriteFont;
		std::weak_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
		SERIALIZE_COMPONENT(m_spriteFontPos, m_fontColor, m_fontScale, m_text, m_fontPath);
	};
}

