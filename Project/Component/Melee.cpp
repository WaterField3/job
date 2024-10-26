#include "Melee.h"

#include <Imgui/imgui.h>

#include "Animater.h"

REGISTER_COMPONENT(TMF::Melee, "Melee");

namespace TMF
{
	void Melee::OnInitialize()
	{
	}
	void Melee::OnFinalize()
	{
	}
	void Melee::OnUpdate()
	{
	}
	void Melee::OnLateUpdate()
	{
	}
	void Melee::OnDraw()
	{
	}
	void Melee::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_meleeAnimation.c_str());
		if (ImGui::InputText("EffectPath", buf, 256))
		{
			m_meleeAnimation = buf;
		}
	}
	void Melee::Play()
	{

		if (auto pLockOwner = m_pOwner.lock())
		{
			
		}
	}
}