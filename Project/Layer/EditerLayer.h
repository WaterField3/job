#pragma once

#include <memory>

#include "Layer.h"
#include "Imgui/imgui.h"
#include "Editer/Hierarchy.h"
#include "Editer/Inspector.h"

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
		std::unique_ptr<Hierarchy> m_pHirarchy;
		std::unique_ptr<Inspector> m_pInspector;
	};
}
