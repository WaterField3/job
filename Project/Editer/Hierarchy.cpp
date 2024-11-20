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
		m_pTransformsCache = GameObjectManager::Instance().GetComponents<Transform>();

		// ���[�g�̕\��
		ImGui::Text("Root");
		auto dropLabel = "HierarchyDragDropTransform";
		TransformDropTarget(dropLabel, std::make_shared<Transform>());
		
		//GameObject��Component�̕\��
		for (auto& pTransform : m_pTransformsCache)
		{
			// �e���ݒ肳��ĂȂ���Ε\��
			if (auto pLockedTransform = pTransform.lock())
			{
				if (pLockedTransform->GetParent().expired() == false)
				{
					continue;
				}
			}
			DrawTree(pTransform);
		}

		if (ImGui::Button("CreateGameObject"))
		{
			GameObjectManager::Instance().CreateGameObject();
		}
		ImGui::End();
	}
	void Hierarchy::DrawTree(std::weak_ptr<Transform> pTransform)
	{
		if (auto pLockTransform = pTransform.lock())
		{
			auto pGameObject = pLockTransform->GetOwner();
			if (auto pObject = pGameObject.lock())
			{
				auto name = pObject->GetName();
				auto uuID = pLockTransform->GetUUID();
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

				// �|�b�v�A�b�v�̃��x���쐬
				auto popupLabel = StringHelper::CreateLabel("GameObjectMenuPopup", uuID);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup(popupLabel.c_str());
				}

				// DeletePopup�`��
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
				// �h���b�O��Transform�o�^
				auto dragDropLabel = "HierarchyDragDropTransform";
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload(dragDropLabel, &pLockTransform, sizeof(std::weak_ptr<Transform>));
					ImGui::Text("%s", name.c_str());
					ImGui::EndDragDropSource();
				}

				// �h���b�v����f�[�^�̎��o��
				TransformDropTarget(dragDropLabel, pTransform);

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
	}
	void Hierarchy::TransformDropTarget(const char* dragDropLabel, std::weak_ptr<TMF::Transform> pTransform)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const auto payload = ImGui::AcceptDragDropPayload(dragDropLabel))
			{
				auto pDropTransform = static_cast<std::weak_ptr<Transform>*>(payload->Data);
				if (auto pLockedTransform = pDropTransform->lock())
				{
					pLockedTransform->SetParent(pTransform);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	std::vector<std::weak_ptr<Transform>> Hierarchy::GetTransformChildren(std::weak_ptr<Transform> pTransform)
	{
		auto children = std::vector<std::weak_ptr<Transform>>();
		for (auto& pCurrentTransform : m_pTransformsCache)
		{
			if (auto pLockedCurrentTransform = pCurrentTransform.lock())
			{
				auto pParentTransform = pLockedCurrentTransform->GetParent();
				if (auto pLockedParentTransform = pParentTransform.lock())
				{
					if (auto pLockedTransform = pTransform.lock())
					{
						if (pLockedTransform.get() == pLockedParentTransform.get())
						{
							children.push_back(pCurrentTransform);
						}
					}
				}
			}
		}
		return children;
	}
}
