#include "Fade.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "PrimitiveMesh.h"

REGISTER_COMPONENT(TMF::Fade, "Fade");

namespace TMF
{
	void Fade::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pPrimitiveMesh = pLockOwner->GetComponent<PrimitiveMesh>();
		}
	}
	void Fade::OnFinalize()
	{
	}
	void Fade::OnUpdate()
	{
		if (m_isFade == true)
		{
			if (auto pLockPrimitiveMesh = m_pPrimitiveMesh.lock())
			{
				switch (m_fadeType)
				{
				case TMF::Fade::FADEIN:
					m_timer += 0.1f * m_fadeSpeed;
					pLockPrimitiveMesh->SetColor(DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, m_timer));
					break;
				case TMF::Fade::FADEOUT:
					m_timer += 0.1f * m_fadeSpeed;
					pLockPrimitiveMesh->SetColor(DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f - m_timer));
					break;
				default:
					break;
				}
				if (m_timer > 1.0f)
				{
					m_isFade = false;
					m_timer = 0.0f;
				}
			}
		}
	}
	void Fade::OnLateUpdate()
	{
	}
	void Fade::OnDraw()
	{
	}
	void Fade::OnDrawImGui()
	{
		// フェードの速度
		auto fadeSpeedLabel = StringHelper::CreateLabel("FadeSpeed", m_uuID);
		if (ImGui::DragFloat(fadeSpeedLabel.c_str(), &m_fadeSpeed))
		{

		}
		// フェードインかフェードアウトか選択
		auto fadeTypeLabel = StringHelper::CreateLabel("FadeType", m_uuID);
		const char* types[] = { "FadeIn","FadeOut" };
		int selectIndex = (int)m_fadeType;
		if (ImGui::BeginCombo(fadeTypeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_fadeType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_fadeType = FadeType(i);
					selectIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		// 最初からフェードを始めるか
		auto isFadeLabel = StringHelper::CreateLabel("IsFade", m_uuID);
		if (ImGui::Checkbox(isFadeLabel.c_str(), &m_isFade))
		{

		}
	}
	void Fade::FadeStart()
	{
		m_isFade = true;
	}
}