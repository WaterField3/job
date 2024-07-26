#include "EditerLayer.h"

#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <tchar.h>
#include <iostream>

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Component/ComponentManager.h"
#include "Component/Transform/Transform.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace TMF
{
	void EditerLayer::OnInitialize()
	{
		m_pHirarchy = std::make_unique<Hierarchy>();
		m_pInspector = std::make_unique<Inspector>();

		// Show the window
		::ShowWindow(D3D::Get()->GetHwnd(), SW_SHOWDEFAULT);
		::UpdateWindow(D3D::Get()->GetHwnd());

		// imguiSetUp
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(D3D::Get()->GetHwnd());
		ImGui_ImplDX11_Init(D3D::Get()->GetDevice(), D3D::Get()->GetContext());

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}

	void EditerLayer::OnFinalize()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void EditerLayer::OnUpdate()
	{

	}

	void EditerLayer::OnDraw()
	{

	}
	void EditerLayer::OnDrawImGui()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		m_pHirarchy->DrawImGui();

		m_pInspector->DrawImGui(m_pHirarchy->GetSelectGameObject());

		ImGui::Render();


		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		D3D::Get()->UpdateScreen();
	}
}
