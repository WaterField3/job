#pragma once

#include "Layer.h"

#include <memory>

#include "Editer/Hierarchy.h"
#include "Editer/Inspector.h"
#include "Editer/PlayButtonBar.h"

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
		std::unique_ptr<PlayButtonBar> m_pPlayButtonBar;
	};
}
