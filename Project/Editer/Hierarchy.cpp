#include "Hierarchy.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"

namespace TMF
{
	void Hierarchy::DrawImGui()
	{
		ImGui::Begin("Hierarchy");                          // Create a window called "Hello, world!" and append into it.

		auto pGameObjects = GameObjectManager::Instance().GetGameObjects();

		for (auto& pGameObject : pGameObjects)
		{
			auto name = pGameObject->GetName();

			name += "##" + pGameObject->GetStrUUID();

			if (ImGui::TreeNodeEx(name.c_str()))
			{
				ImGui::TreePop();
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("GameObjectMenuPopup");
				}
			}

			if (ImGui::IsItemClicked())
			{
				m_pSelectGameObject = pGameObject;
			}
		}
		if (ImGui::BeginPopup("GameObjectMenuPopup"))
		{
			if (ImGui::Button("DeleteGameObject"))
			{
				if (!m_pSelectGameObject.expired())
				{
					auto pGameObject = m_pSelectGameObject.lock();
					GameObjectManager::Instance().DestroyGameObject(pGameObject.get());
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::Button("CreateGameObject"))
		{
			GameObjectManager::Instance().CreateGameObject();
		}
		ImGui::End();
	}
}
