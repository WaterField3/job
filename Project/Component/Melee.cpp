#include "Melee.h"

#include <Imgui/imgui.h>

#include "Animater.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Transform.h"
#include "PrimitiveMesh.h"
#include "BulletStraightMove.h"
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
		auto meleeAnimPath = StringHelper::CreateLabel("AnimPath", m_uuID);
		if (ImGui::InputText(meleeAnimPath.c_str(), buf, 256))
		{
			m_meleeAnimation = buf;
		}
		auto endTimeLabel = StringHelper::CreateLabel("EndTime", m_uuID);
		if (ImGui::DragFloat(endTimeLabel.c_str(), &endTime))
		{

		}
		auto playLabel = StringHelper::CreateLabel("Play", m_uuID);
		if (ImGui::Button(playLabel.c_str()))
		{
			Play();
		}
	}
	void Melee::Play()
	{
		// objファイルのGameObjectを生成
		//GameObjectManager::Instance().LoadObject("obj");

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pAnimater = pLockOwner->GetComponent<Animater>();
			if (auto pLockAnimater = pAnimater.lock())
			{
				// アニメーションのパスの変更
				pLockAnimater->SetFileName(m_meleeAnimation, endTime);
			}
		}
	}
}