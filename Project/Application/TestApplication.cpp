#include "TestApplication.h"

#include "Layer/TestLayer.h"
#include "Layer/CameraLayer.h"
#include "Layer/FieldLayer.h"
#include "GraphicManager/GraphicManager.h"
#include "Utility/Log.h"
#include "Layer/GameLayer.h"
#include "Layer/EditerLayer.h"


TMF::Application* CreateApplication()
{
	return new TMF::TestApplication();
}

void TMF::TestApplication::OnInitialize()
{
	//m_console.Initialize();
	GraphicManager::CreateGraphicManager();
	AddLayer<GameLayer>();
	AddLayer<EditerLayer>();
}	

void TMF::TestApplication::OnUpdate()
{
	Application::OnUpdate();
}

void TMF::TestApplication::OnFinalize()
{
	//m_console.Finalize();
	GraphicManager::Get()->DeleteCameraLayer();
}
