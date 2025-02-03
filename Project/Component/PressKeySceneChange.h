#pragma once
#include "Component.h"

#include "Input.h"

namespace TMF
{
	class Fade;
	class PressKeySceneChange : public Component
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
		std::weak_ptr<Fade> m_pFade;
	};
}

