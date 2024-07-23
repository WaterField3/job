#include "Application.h"
#include "direct3d.h"

namespace TMF
{
	Application::Application()
	{

	}
	Application::~Application()
	{

	}
	void Application::OnInitialize()
	{
	}
	void Application::OnFinalize()
	{
		RemoveAllLayer();
	}
	void Application::OnUpdate()
	{
		for (auto& layer : m_layers)
		{
			layer->OnUpdate();
		}
	}

	void Application::OnDraw()
	{
		ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

		D3D::Get()->ClearScreen();
		for (auto& layer : m_layers)
		{
			layer->OnDraw();
		}
		D3D::Get()->UpdateScreen();
	}

	void Application::OnDrawImGui()
	{
		D3D::Get()->ClearScreen();
		for (auto& layer : m_layers)
		{
			layer->OnDrawImGui();
		}
		D3D::Get()->UpdateScreen();
	}

	void Application::RemoveAllLayer()
	{
		for (auto& layer : m_layers)
		{
			layer->OnFinalize();
		}
		m_layers.clear();
	}
}
