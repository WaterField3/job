#include "BoneBind.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Transform.h"
#include "Model.h"
#include "Animater.h"

REGISTER_COMPONENT(TMF::BoneBind, "BoneBind");

namespace TMF
{
	void BoneBind::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = m_pTransform.lock())
			{
				m_pParent = pLockTransform->GetParent();
			}

			if (auto pLockParent = m_pParent.lock())
			{
				auto pParentOwner = pLockParent->GetOwner();
				if (auto pLockParentOwner = pParentOwner.lock())
				{
					m_pAnimater = pLockParentOwner->GetComponent<Animater>();
					auto pParentModel = pLockParentOwner->GetComponent<Model>();
					if (auto pLockParentModel = pParentModel.lock())
					{
						m_pModel = pLockParentModel->GetModel();
						m_boneSize = m_pModel.lock()->bones.size();

					}
				}
			}
		}
	}
	void BoneBind::OnFinalize()
	{
	}
	void BoneBind::OnUpdate()
	{
		if (m_isBindBone == true)
		{
			if (auto pLockTransform = m_pTransform.lock())
			{
				auto bonePosition = pLockTransform->GetWorldPosition();
				auto boneRotation = pLockTransform->GetRotation();
				if (auto pLockAnimater = m_pAnimater.lock())
				{
					bonePosition = pLockAnimater->GetBonePosition(m_bindName);
					boneRotation = pLockAnimater->GetBoneRotation(m_bindName);
					pLockTransform->SetPosition(bonePosition);
					pLockTransform->SetRotation(boneRotation);
				}
			}
		}
	}
	void BoneBind::OnLateUpdate()
	{
	}
	void BoneBind::OnDraw()
	{
	}
	void BoneBind::OnDrawImGui()
	{
		char bindNameBuf[128] = {};
		strcpy_s(bindNameBuf, sizeof(bindNameBuf), m_bindName.c_str());
		auto bindNameLabel = StringHelper::CreateLabel("BindName", m_uuID);
		if (ImGui::InputText(bindNameLabel.c_str(), bindNameBuf, 128))
		{
			m_bindName = bindNameBuf;
		}
		auto isBindLabel = StringHelper::CreateLabel("IsBind", m_uuID);
		if (ImGui::Checkbox(isBindLabel.c_str(), &m_isBindBone))
		{

		}

		auto testLabel = StringHelper::CreateLabel("test", m_uuID);
		if (ImGui::DragFloat(testLabel.c_str(), &m_scale, 0.1f))
		{

		}
	}
	std::shared_ptr<Component> BoneBind::OnClone() const
	{
		auto pClone = std::make_shared<BoneBind>();
		return move(pClone);
	}
	DirectX::SimpleMath::Matrix BoneBind::GetBoneMatrix()
	{
		//if (boneMatrix != DirectX::SimpleMath::Matrix::Identity)
		//{
		//	return boneMatrix;
		//}
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto bonePosition = pLockTransform->GetWorldPosition();
			auto boneRotation = pLockTransform->GetRotation();
			auto scale = pLockTransform->GetScale();
			if (auto pLockAnimater = m_pAnimater.lock())
			{
				bonePosition = pLockAnimater->GetBonePosition(m_bindName);
				boneMatrix = pLockAnimater->GetBoneMatrix(m_bindName);
				//boneMatrix = boneMatrix * pLockTransform->GetWorldMatrix();
			}
			auto parentRotation = DirectX::SimpleMath::Quaternion::Identity;
			auto pParent = pLockTransform->GetParent();
			if (auto pLockParent = pParent.lock())
			{
				bonePosition *= /*pLockParent->GetScale()*/ m_scale;
				bonePosition += pLockParent->GetWorldPosition();
				parentRotation = pLockParent->GetRotation();
				boneRotation *= parentRotation;
			}

			auto rotateMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(boneRotation);
			// ägèkçsóÒ
			auto scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale);
			// à⁄ìÆçsóÒ
			auto transformMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(bonePosition);

			boneMatrix = scaleMatrix * rotateMatrix * transformMatrix;

		}
		return boneMatrix;
	}
}