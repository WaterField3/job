#pragma once

#include <memory>
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <chrono>

namespace TMF
{
	class Input
	{
	public:
		Input() = default;
		~Input() = default;
		void Initialize(HWND hwnd);

		inline DirectX::Keyboard::State GetKeyState() const { return m_pKeyboard->GetState(); }
		inline DirectX::Mouse::State GetMouseState() const { return m_pMouse->GetState(); }
		inline DirectX::Keyboard::KeyboardStateTracker* GetTracker() const { return m_pTracker.get(); }
		// ˆê’èŠÔ“à‚É•¡”‰ñ‰Ÿ‚³‚ê‚½‚©‚ğŠm”F‚·‚é
		bool PluralGetKeyDiwn(std::chrono::milliseconds currentTime);
		static Input& Instance()
		{
			static Input instance;
			return instance;
		}

	private:
		std::unique_ptr<DirectX::Keyboard> m_pKeyboard;
		std::unique_ptr<DirectX::Mouse> m_pMouse;
		std::shared_ptr<DirectX::Keyboard::KeyboardStateTracker> m_pTracker;
		int m_tapCount = 0;
		std::chrono::milliseconds m_lastTapTime{ 0 };
		const std::chrono::milliseconds m_tapIntertval{ 200 };
	};
}

