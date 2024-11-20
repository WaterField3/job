#include "Effect.h"

#include <Imgui/imgui.h>

#include "EffectManager.h"
#include "Utility/StringHelper.h"
#include "Transform.h"

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
		auto label = StringHelper::CreateLabel("EffectPos", m_uuID);
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
		auto pOwner = m_pOwner.lock();
		auto pTransform = pOwner->GetComponent<Transform>();
		if (auto pLockTransform = pTransform.lock())
		{
			auto parent = pLockTransform->GetParent();
			auto worldMatrix = pLockTransform->GetWorldMatrix();
			if (auto pLockParentTransform = parent.lock())
			{
				pLockTransform->SetPosition(m_effectPos);
				auto rotation = pLockParentTransform->GetRotation();
				pLockTransform->SetRotation(rotation);
				worldMatrix = pLockTransform->GetWorldMatrix();
				auto parentPos = pLockParentTransform->GetWorldPosition();
				auto effectPos = m_effectPos + parentPos;
				EffectManager::Instance().Play(m_effectPath, effectPos, worldMatrix);
			}
			else
			{
				EffectManager::Instance().Play(m_effectPath, m_effectPos, worldMatrix);
			}
		}
	}
}