#pragma once

#include "Layer.h"
#include "Vender/Imgui/imgui.h"


namespace TMF
{
	class EditerLayer : public Layer
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		bool m_show_demo_window = true;
		bool m_show_another_window = false;
		ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	};
}
