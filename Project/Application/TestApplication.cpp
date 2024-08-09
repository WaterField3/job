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
		//m_console.Initialize();
		AddLayer<GameLayer>();
		AddLayer<PhysicsLayer>();
		AddLayer<EditerLayer>();
	}

	void TestApplication::OnUpdate()
	{
		Application::OnUpdate();
	}

	void TestApplication::OnFinalize()
	{
		//m_console.Finalize();
		Application::OnFinalize();
	}
}