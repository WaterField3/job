#include "Effect.h"

#include <Imgui/imgui.h>

#include "EffectManager.h"
#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::Effect, "Effect");

namespace TMF
{
	void Effect::OnInitialize()
	{
	}
	void Effect::OnFinalize()
	{
	}
	void Effect::OnUpdate()
	{
	}
	void Effect::OnLateUpdate()
	{
	}
	void Effect::OnDraw()
	{
	}
	void Effect::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_effectPath.c_str());
		if (ImGui::InputText("EffectPath", buf, 256))
		{
			m_effectPath = buf;
		}
		auto label = StringHelper::Instance().ChangeLabel("EffectPos", m_uuID);
		if (ImGui::DragFloat3(label.c_str(), &m_effectPos.x, 0.1f))
		{

		}
		if (ImGui::Button("PlayEffect"))
		{
			Play();
		}
	}
	void Effect::Play()
	{
		EffectManager::Instance().Play(m_effectPath, m_effectPos);
	}
	boost::uuids::uuid Effect::OnGetUUID()
	{
		return m_uuID;
	}
}