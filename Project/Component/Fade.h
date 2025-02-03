#pragma once
#include "Component.h"

#include <wrl/client.h>

#include "ComponentCerealHelper.h"
#include "SpriteBatch.h"

namespace TMF
{
	class Fade : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void FadeStart();
		inline bool GetIsFadeInEnd() const { return m_isFadeInEnd; }
		inline bool GetIsFadeOutEnd() const { return m_isFadeOutEnd; }

	private:
		enum FadeType
		{
			// èôÅXÇ…ï`âÊ
			FADEIN,
			//Å@èôÅXÇ…ìßâﬂ
			FADEOUT,
		};
		bool m_isFade = false;
		bool m_isFadeOutEnd = false;
		bool m_isFadeInEnd = false;
		float m_fadeSpeed = 1.0f;
		float m_fadeAlpha = 1.0f;
		FadeType m_fadeType = FADEIN;
		std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_pFadeTexture;
		SERIALIZE_COMPONENT(m_fadeSpeed, m_isFade, m_fadeType);
	};
}
