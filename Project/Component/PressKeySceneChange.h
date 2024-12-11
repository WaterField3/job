#pragma once
#include "Component.h"

#include "Input.h"

namespace TMF
{
	class Fade;
	class PressKeySceneChange : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		std::weak_ptr<Fade> m_pFade;
		//enum Key : unsigned char
		//{
		//	A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,Enter = 0x20,SPACE,
		//};

		//Key m_key = Key::Enter;
	};
}

