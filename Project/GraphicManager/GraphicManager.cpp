#include "GraphicManager.h"

#include "Utility/Log.h"

TMF::GraphicManager* TMF::GraphicManager::instance = nullptr;


void TMF::GraphicManager::CreateGraphicManager()
{
	if (instance == nullptr)
	{
		instance = new GraphicManager();
	}
}

TMF::GraphicManager* TMF::GraphicManager::Get()
{
	if (instance == nullptr)
	{
		instance = new GraphicManager();
	}
	return instance;
}

void TMF::GraphicManager::SetCameraLayer(CameraLayer* _camLayer)
{
	cameraLayer = _camLayer;
}

void TMF::GraphicManager::CreatCameraLayer()
{
	cameraLayer = new CameraLayer();
}

void TMF::GraphicManager::DeleteCameraLayer()
{
	delete cameraLayer;
}

void TMF::GraphicManager::CameraSetOwner(Layer* _owner)
{
	cameraLayer->SetCameraOwner(_owner);
}

DirectX::XMMATRIX TMF::GraphicManager::GetViewMatrix()
{
	return cameraLayer->GetViewMatrix();
}

DirectX::XMMATRIX TMF::GraphicManager::GetProjectionMatrix()
{
	return cameraLayer->GetProjectionMatrix();
}
