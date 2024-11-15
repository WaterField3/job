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

	bool Input::PluralGetKeyDiwn(std::chrono::milliseconds currentTime)
	{
		if (currentTime - m_lastTapTime < m_tapIntertval)
		{
			m_tapCount++;
			m_lastTapTime = currentTime;
			return true;
		}
		m_tapCount = 1;
		m_lastTapTime = currentTime;
		return false;
	}
}