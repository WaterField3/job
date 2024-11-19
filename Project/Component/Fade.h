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

	private:
		enum FadeType
		{
			// èôÅXÇ…ï`âÊ
			FADEIN,
			//Å@èôÅXÇ…ìßâﬂ
			FADEOUT,
		};
		bool m_isFade = false;
		float m_fadeSpeed = 1.0f;
		float m_timer = 0.0f;
		FadeType m_fadeType = FADEIN;
		std::weak_ptr<PrimitiveMesh> m_pPrimitiveMesh;
		SERIALIZE_COMPONENT(m_fadeSpeed, m_isFade, m_fadeType);
	};
}
