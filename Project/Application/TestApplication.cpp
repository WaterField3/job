#include "TestApplication.h"

#include "GraphicManager/GraphicManager.h"
#include "Utility/Log.h"
#include "Layer/GameLayer.h"
#include "Layer/EditerLayer.h"
#include "Layer/PhysicsLayer.h"

TMF::Application* CreateApplication()
{
	return new TMF::TestApplication();
}

void TMF::TestApplication::OnInitialize()
{
	//m_console.Initialize();
	AddLayer<GameLayer>();
	AddLayer<PhysicsLayer>();
	AddLayer<EditerLayer>();
}	

void TMF::TestApplication::OnUpdate()
{
	Application::OnUpdate();
}

void TMF::TestApplication::OnFinalize()
{
	//m_console.Finalize();
}
