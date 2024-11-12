#include "TestApplication.h"

#include "Utility/Log.h"
#include "Layer/GameLayer.h"
#include "Layer/EditerLayer.h"
#include "Layer/PhysicsLayer.h"

TMF::Application* CreateApplication()
{
	return new TMF::TestApplication();
}
namespace TMF
{
	void TestApplication::OnInitialize()
	{
		AddLayer<GameLayer>();
		AddLayer<EditerLayer>();
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