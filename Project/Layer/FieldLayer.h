#pragma once
#include "Layer.h"

namespace TMF
{
	class FieldLayer :
		public Layer
	{

		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnDraw() override;
	public:
	private:
	};
}

