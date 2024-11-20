#include "Inspector.h"

#include "Imgui/imgui.h"

#include "Component/ComponentManager.h"

namespace TMF
{
	void Inspector::DrawImGui(std::weak_ptr<GameObject> pGameObject)
	{
		ImGui::Begin("Inspector");

		if (auto pLockGameObject = pGameObject.lock())
		{
			pLockGameObject->DrawImGui();
			if (ImGui::Button("AddComponent"))
			{
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				for (auto& componentName : ComponentManager::Instance().GetComponentNames())
				{
					if (ImGui::Button(componentName.c_str()))
					{
						ComponentManager::Instance().AddComponent(componentName, pLockGameObject);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}
