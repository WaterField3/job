#include "Inspector.h"

#include "Imgui/imgui.h"

#include "GameObject/GameObject.h"

namespace TMF
{
	void Inspector::DrawImGui(std::weak_ptr<GameObject> pGameObject)
	{
		if (pGameObject.expired())
		{
			return;
		}

		ImGui::Begin("Inspector");

		if (auto pObject = pGameObject.lock())
		{
			pObject->DrawImGui();
		}

		ImGui::End();
	}
}
