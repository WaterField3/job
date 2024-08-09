#pragma once

#include "Application/Application.h"

namespace TMF
{
	class TestApplication : public Application
	{
	public:
		void OnInitialize() override;
		void OnUpdate() override;
		void OnFinalize() override;
	};
}
