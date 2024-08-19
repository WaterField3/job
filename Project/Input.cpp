#include "Input.h"

namespace TMF
{
	void Input::Initialize(HWND hwnd)
	{
		m_pKeyboard = std::make_unique<DirectX::Keyboard>();
		m_pMouse = std::make_unique<DirectX::Mouse>();
		m_pMouse->SetWindow(hwnd);
		m_pTracker = std::make_shared<DirectX::Keyboard::KeyboardStateTracker>();
	}
}