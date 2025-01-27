#include "Input.h"

namespace TMF
{
	void Input::Initialize(HWND hwnd)
	{
		m_pKeyboard = std::make_unique<DirectX::Keyboard>();
		m_pMouse = std::make_unique<DirectX::Mouse>();
		m_pMouse->SetWindow(hwnd);
		m_pKeyboardTracker = std::make_shared<DirectX::Keyboard::KeyboardStateTracker>();
		m_pButtonTracker = std::make_shared<DirectX::Mouse::ButtonStateTracker>();
	}

	void Input::ResetScrollWheelValue()
	{
		if (m_pMouse->IsConnected() == true)
		{
			m_pMouse->ResetScrollWheelValue();
		}
	}

	void Input::Update()
	{
		if (m_pKeyboardTracker)
		{
			m_pKeyboardTracker->Update(m_pKeyboard->GetState());
		}
		if (m_pButtonTracker)
		{
			m_pButtonTracker->Update(m_pMouse->GetState());
		}
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