#include "GameObject.h"

#include <Imgui/imgui.h>
#include <string.h>

#include "Component/ComponentManager.h"
#include "GameObject/GameObjectManager.h"
#include "Component/Transform.h"
#include "Utility/StringHelper.h"

namespace TMF
{
	void GameObject::Initialize()
	{
		if (m_uuID.is_nil())
		{
			m_uuID = boost::uuids::random_generator()();
		}
		m_pTransform = GetComponent<Transform>();
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Initialize(shared_from_this());
		}
	}

	void GameObject::Finalize()
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Finalize();
		}
	}

	void GameObject::Update()
	{
		if (m_isActive)
		{
			for (auto& component : m_pComponents)
			{
				component->Update();
			}
		}
	}

	void GameObject::Draw()
	{
		for (auto& component : m_pComponents)
		{
			component->Draw();
		}
	}

	void GameObject::DrawImGui()
	{
		auto label = std::string("## ");
		label += boost::uuids::to_string(m_uuID);
		ImGui::Checkbox(label.c_str(), &m_isActive);
		ImGui::SameLine(30);

		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_name.c_str());
		if (ImGui::InputText("Name", buf, 256))
		{
			m_name = buf;
		}

		const char* types[] = { "Default", "Player", "Ground" };
		static int selectIndex = (int)m_tag;
		auto shapeLabel = StringHelper::CreateLabel("Tag", m_uuID);
		if (ImGui::BeginCombo(shapeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_tag == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_tag = Tag(i);
					selectIndex = i;

				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		auto index = 0;
		for (auto& component : m_pComponents)
		{

			index++;
			auto componentName = ComponentManager::Instance().GetComponentName(typeid(*component));

			auto uuID = component->GetUUID();
			auto uuIDStr = boost::uuids::to_string(uuID);
			auto label = "## " + componentName + uuIDStr;
			auto enable = component->GetIsEnable();
			ImGui::Checkbox(label.c_str(), &enable);
			{
				component->SetIsEnable(enable);
			}
			ImGui::SameLine(30);
			label = componentName;
			label += "## " + uuIDStr;
			if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("ComponentMenuPopup");
					m_selectComponentName = componentName.c_str();
					m_selectIndex = index;
				}
				component->DrawImGui();
			}
		}

		if (ImGui::BeginPopup("ComponentMenuPopup"))
		{
			if (ImGui::Button("RemoveComponent"))
			{
				ComponentManager::Instance().RemoveComponent(m_selectComponentName, shared_from_this(), m_selectIndex);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void GameObject::CollisionEnter(GameObject* pGameObject)
	{
		for (auto& component : m_pComponents)
		{
			component->CollisionEnter(pGameObject);
		}
	}

	void GameObject::CollisionStay(GameObject* pGameObject)
	{
		for (auto& component : m_pComponents)
		{
			component->CollisionStay(pGameObject);
		}
	}

	void GameObject::CollisionExit(GameObject* pGameObject)
	{
		for (auto& component : m_pComponents)
		{
			component->CollisionExit(pGameObject);
		}
	}

	void GameObject::TrigerEnter(GameObject* pGameObject)
	{
		for (auto& component : m_pComponents)
		{
			component->TrigerEnter(pGameObject);
		}
	}

	void GameObject::TrigerStay(GameObject* pGameObject)
	{
		for (auto& component : m_pComponents)
		{
			component->TrigerStay(pGameObject);
		}
	}

	void GameObject::TrigerExit(GameObject* pGameObject)
	{
		for (auto& component : m_pComponents)
		{
			component->TrigerExit(pGameObject);
		}
	}

	std::vector<std::weak_ptr<GameObject>> GameObject::GetChildren()
	{
		auto children = std::vector<std::weak_ptr<GameObject>>();
		for (auto& pCurrentTransform : GameObjectManager::Instance().GetComponents<Transform>())
		{
			if (auto pLockedCurrentTransform = pCurrentTransform.lock())
			{
				auto pParentTransform = pLockedCurrentTransform->GetParent();
				if (auto pLockedParentTransform = pParentTransform.lock())
				{
					auto pTransform = GetComponent<Transform>();
					if (auto pLockedTransform = pTransform.lock())
					{
						if (pLockedTransform.get() == pLockedParentTransform.get())
						{
							children.push_back(pLockedCurrentTransform->GetOwner());
						}
					}
				}
			}
		}
		return children;
	}

	void GameObject::LateUpdate()
	{
		for (auto& component : m_pComponents)
		{
			component->LateUpdate();
		}
	}
}