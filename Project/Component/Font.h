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

	private:
		DirectX::SimpleMath::Vector2 m_spriteFontPos;
		std::string m_text = "";
		std::string m_fontPath = "asset/myfile.spritefont";
		std::unique_ptr<DirectX::SpriteFont> m_pSpriteFont;
		std::weak_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
		SERIALIZE_COMPONENT(m_spriteFontPos, m_text, m_fontPath);
	};
}

