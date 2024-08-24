#include "Hierarchy.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Component/Transform.h"
#include "Utility/StringHelper.h"

namespace TMF
{
	void Hierarchy::DrawImGui()
	{
		ImGui::Begin("Hierarchy");                          // Create a window called "Hello, world!" and append into it.

		m_pTransformsCache.clear();
		auto pTransforms = GameObjectManager::Instance().GetComponents<Transform>();

		for (auto& pTransform : pTransforms)
		{
			if (auto pTrans = pTransform.lock())
			{
				m_pTransformsCache.push_back(pTrans.get());
			}
		}
		//GameObject‚ÌComponent‚Ì•\Ž¦
		for (auto& pTransform : m_pTransformsCache)
		{
			// e‚ªÝ’è‚³‚ê‚Ä‚È‚¯‚ê‚Î•\Ž¦
			if (pTransform->GetParent().expired() == true)
			{
				DrawTree(pTransform);
			}
		}

		if (ImGui::Button("CreateGameObject"))
		{
			GameObjectManager::Instance().CreateGameObject();
		}
		ImGui::End();
	}
	void Hierarchy::DrawTree(const Transform* pTransform)
	{
		auto pGameObject = pTransform->GetOwner();
		if (auto pObject = pGameObject.lock())
		{
			auto name = pObject->GetName();
			auto uuID = pTransform->GetUUID();
			auto label = StringHelper::CreateLabel(name.c_str(), uuID);
			auto flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			auto pChildren = GetTransformChildren(pTransform);
			auto isOpen = false;

			if (ImGui::TreeNodeEx(label.c_str(), flags))
			{
				isOpen = true;
			}

			if (ImGui::IsItemClicked())
			{
				m_pSelectGameObject = pGameObject;
			}
			auto popupLabel = StringHelper::CreateLabel("GameObjectMenuPopup", uuID);
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup(popupLabel.c_str());
			}
			if (ImGui::BeginPopup(popupLabel.c_str()))
			{
				if (ImGui::Button("DeleteGameObject"))
				{
					if (m_pSelectGameObject.expired() == false)
					{
						auto pGameObject = m_pSelectGameObject.lock();
						GameObjectManager::Instance().DestroyGameObject(pGameObject.get());
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (isOpen == true)
			{
				for (auto& pChild : pChildren)
				{
					DrawTree(pChild);
				}
				ImGui::TreePop();
			}
		}
	}
	std::vector<Transform*> Hierarchy::GetTransformChildren(const Transform* pTransform)
	{
		auto children = std::vector<Transform*>();
		for (auto& pCurrentTransform : m_pTransformsCache)
		{
			auto pParentTransform = pCurrentTransform->GetParent();
			if (auto pLockedParentTransform = pParentTransform.lock())
			{
				if (pTransform == pLockedParentTransform.get())
				{
					children.push_back(pCurrentTransform);
				}
			}
		}
		return children;
	}
}
