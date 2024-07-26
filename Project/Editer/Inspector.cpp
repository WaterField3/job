#include "Inspector.h"

#include "Imgui/imgui.h"

#include "GameObject/GameObject.h"
#include "Component/Transform/Transform.h"

namespace TMF
{
	void Inspector::DrawImGui(std::weak_ptr<GameObject> pGameObject)
	{
		ImGui::Begin("Inspector");

		if (auto pObject = pGameObject.lock())
		{
			pObject->DrawImGui();
			if (ImGui::Button("AddComponent"))
			{
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				if (ImGui::Button("Transfrom"))
				{
					pObject->AddComponent<Transform>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}
