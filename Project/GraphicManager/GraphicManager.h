#pragma once

#include "direct3d.h"
#include "Layer/Layer.h"
#include "Layer/CameraLayer.h"

namespace TMF
{
	class GraphicManager
	{
	public:
		GraphicManager() = default;
		~GraphicManager() = default;
		static GraphicManager* Get();
		static void CreateGraphicManager();
		void SetCameraLayer(CameraLayer* _camLayer);
		void CreatCameraLayer();
		void DeleteCameraLayer();
		void CameraSetOwner(Layer* _owner);
		DirectX::XMMATRIX GetViewMatrix();
		DirectX::XMMATRIX GetProjectionMatrix();
	private:
		static GraphicManager* instance;

	private:
		CameraLayer* cameraLayer;
	};
}