#include "Shot.h"

#include <Imgui/imgui.h>

#include "Component/ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"

REGISTER_COMPONENT(TMF::Shot, "Shot");

namespace TMF
{
	void Shot::OnInitialize()
	{
	}
	void Shot::OnFinalize()
	{
	}
	void Shot::OnUpdate()
	{
	}
	void Shot::OnLateUpdate()
	{
	}
	void Shot::OnDraw()
	{
	}
	void Shot::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_objectFilePath.c_str());
		auto label = StringHelper::CreateLabel("FileName", m_uuID);
		if (ImGui::InputText(label.c_str(), buf, 256))
		{
			m_objectFilePath = buf;
		}

		if (ImGui::Button("Play"))
		{
			Play();
		}
	}

	void Shot::Play()
	{
		GameObjectManager::Instance().LoadObject(m_objectFilePath);
	}


}