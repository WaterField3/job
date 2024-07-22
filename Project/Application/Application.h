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
		// �����擾
		static uint32_t GetWidth() {
			return m_Width;
		}

		// �������擾
		static uint32_t GetHeight() {
			return m_Height;
		}

		// �E�C���h�E�n���h����Ԃ�
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
		static HINSTANCE   m_hInst;        // �C���X�^���X�n���h���ł�.
		static HWND        m_hWnd;         // �E�B���h�E�n���h���ł�.
		static uint32_t    m_Width;        // �E�B���h�E�̉����ł�.
		static uint32_t    m_Height;       // �E�B���h�E�̏c���ł�.

	};
}
