#pragma once

#include <memory>
#include <vector>

#include "Layer/Layer.h"

namespace TMF
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnDraw();
		virtual void OnDrawImGui();
	protected:
		template <typename T>
		void AddLayer()
		{
			m_layers.push_back(std::make_unique<T>());
			auto size = m_layers.size();
			if (size == 0)
			{
				return;
			}
			m_layers[size - 1]->OnInitialize();

		}

		void RemoveAllLayer();

	protected:
		std::vector<std::unique_ptr<Layer>> m_layers;

	private:
	};
}
