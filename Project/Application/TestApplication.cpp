#include "TestApplication.h"

#include "Layer/TestLayer.h"
#include "Layer/CameraLayer.h"
#include "Layer/FieldLayer.h"
#include "GraphicManager/GraphicManager.h"
#include "Utility/Log.h"

TMF::Application* CreateApplication()
{
	return new TMF::TestApplication();
}

void TMF::TestApplication::OnInitialize()
{
	//m_console.Initialize();
	GraphicManager::CreateGraphicManager();
	AddLayer<CameraLayer>("camera");
	AddLayer<TestLayer>("test");
	AddLayer<FieldLayer>("Field");
	//GraphicManager::Get()->CreatCameraLayer();
	GraphicManager::Get()->SetCameraLayer(GetLayer<CameraLayer>("camera"));
	GraphicManager::Get()->CameraSetOwner(GetLayer<TestLayer>("test"));
	//Log::Info(GetLayer<TestLayer>("test")->GetName());
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
