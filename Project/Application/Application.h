#pragma once

#include <memory>
#include <vector>

#include "Layer/Layer.h"
#include "Utility/Console.h"

namespace TMF
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnDraw();
		virtual void OnDrawImGui();
		// 幅を取得
		static uint32_t GetWidth() {
			return m_Width;
		}

		// 高さを取得
		static uint32_t GetHeight() {
			return m_Height;
		}

		// ウインドウハンドルを返す
		static HWND GetWindow() {
			return m_hWnd;
		}
	protected:
		template <typename T>
		void AddLayer()
		{
			m_layers.push_back(std::make_unique<T>());
			auto size = m_layers.size();
			if (size == 0)
			{
				return;
			}
			m_layers[size - 1]->OnInitialize();

		}

		void RemoveAllLayer();

	protected:
		std::vector<std::unique_ptr<Layer>> m_layers;
		Console m_console;

	private:
		static HINSTANCE   m_hInst;        // インスタンスハンドルです.
		static HWND        m_hWnd;         // ウィンドウハンドルです.
		static uint32_t    m_Width;        // ウィンドウの横幅です.
		static uint32_t    m_Height;       // ウィンドウの縦幅です.

	};
}
