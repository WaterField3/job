#pragma once

#include "Application/Application.h"

namespace TMF
{
	class TestApplication : public Application
	{
		void OnInitialize() override;

		void OnUpdate() override;

		void OnFinalize() override;
	};
}
