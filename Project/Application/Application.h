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
		void AddLayer(const char name[64])
		{
			m_layers.push_back(std::make_unique<T>());
			auto size = m_layers.size();
			if (size == 0)
			{
				return;
			}
			m_layers[size - 1]->OnInitialize();
			m_layers[size - 1]->SetName(name);
		}

		template <typename T>
		T* GetLayer(const char name[64])
		{
			auto size = m_layers.size();
			for (int i = 0; i < size; i++)
			{
				if (m_layers[i]->GetName() == name)
				{
					return dynamic_cast<T*> (m_layers[i].get());
				}
			}
			return nullptr;
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
