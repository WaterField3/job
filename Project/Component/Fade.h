#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class PrimitiveMesh;
	class Fade : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
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
		float m_timer = 0.0f;
		FadeType m_fadeType = FADEIN;
		std::weak_ptr<PrimitiveMesh> m_pPrimitiveMesh;
		SERIALIZE_COMPONENT(m_fadeSpeed, m_isFade, m_fadeType);
	};
}
