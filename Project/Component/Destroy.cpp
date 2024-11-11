#include "Destroy.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::Destroy, "Destroy");

namespace TMF
{
	void Destroy::OnInitialize()
	{
	}
	void Destroy::OnFinalize()
	{
	}
	void Destroy::OnUpdate()
	{
	}
	void Destroy::OnLateUpdate()
	{
	}
	void Destroy::OnDraw()
	{
	}
	void Destroy::OnDrawImGui()
	{
		auto destroyTimeLabel = StringHelper::CreateLabel("DestroyTime", m_uuID);
		if (ImGui::DragFloat(destroyTimeLabel.c_str(), &m_destroyTime))
		{

		}

		auto destroyButtonLabel = StringHelper::CreateLabel("Destroy", m_uuID);
		if (ImGui::Button("destory"))
		{
			Play(m_destroyTime);
		}
	}

	void Destroy::Play(float time)
	{
		std::this_thread::sleep_for(std::chrono::seconds((int)time));
		auto pOwner = m_pOwner.lock().get();
		pOwner->SetActive(false);
		GameObjectManager::Instance().DestroyGameObject(pOwner);
		return;
	}
}