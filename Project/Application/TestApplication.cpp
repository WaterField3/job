#include "TestApplication.h"

#include "Utility/Log.h"
#include "Layer/GameLayer.h"
#include "Layer/EditerLayer.h"
#include "Layer/PhysicsLayer.h"

std::unique_ptr<TMF::Application> CreateApplication()
{
	auto app = std::make_unique<TMF::TestApplication>();
	return std::move(app);
}

namespace TMF
{
	void TestApplication::OnInitialize()
	{
		AddLayer<EditerLayer>();
		AddLayer<GameLayer>();
		AddLayer<PhysicsLayer>();
	}

	void TestApplication::OnUpdate()
	{
		Application::OnUpdate();
	}

	void TestApplication::OnFinalize()
	{
		Application::OnFinalize();
	}
}