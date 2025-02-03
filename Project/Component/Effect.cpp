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
		if (m_effectPath != "")
		{
			Play();
		}
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
		auto effectPathLabel = StringHelper::CreateLabel("EffectPath", m_uuID);
		strcpy_s(buf, sizeof(buf), m_effectPath.c_str());
		if (ImGui::InputText(effectPathLabel.c_str(), buf, 256))
		{
			m_effectPath = buf;
		}
		auto effectPosLabel = StringHelper::CreateLabel("EffectPos", m_uuID);
		if (ImGui::DragFloat3(effectPosLabel.c_str(), &m_effectPos.x, 0.1f))
		{

		}
		if (ImGui::Button("PlayEffect"))
		{
			Play();
		}
	}
	std::shared_ptr<Component> Effect::OnClone() const
	{
		auto pClone = std::make_shared<Effect>();
		pClone->m_effectPath = this->m_effectPath;
		pClone->m_effectPos = this->m_effectPos;
		return move(pClone);
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