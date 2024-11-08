#include "Melee.h"

#include <Imgui/imgui.h>

#include "Animater.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Transform.h"
#include "PrimitiveMesh.h"
#include "Animater.h"
#include "MeleeMove.h"

REGISTER_COMPONENT(TMF::Melee, "Melee");

namespace TMF
{
	void Melee::OnInitialize()
	{
	}
	void Melee::OnFinalize()
	{
	}
	void Melee::OnUpdate()
	{
	}
	void Melee::OnLateUpdate()
	{
	}
	void Melee::OnDraw()
	{
	}
	void Melee::OnDrawImGui()
	{
		char animBuf[256] = "";
		strcpy_s(animBuf, sizeof(animBuf), m_meleeAnimation.c_str());
		auto meleeAnimPath = StringHelper::CreateLabel("AnimPath", m_uuID);
		if (ImGui::InputText(meleeAnimPath.c_str(), animBuf, 256))
		{
			m_meleeAnimation = animBuf;
		}

		char meleeObjectNameBuf[256] = "";
		strcpy_s(meleeObjectNameBuf, sizeof(meleeObjectNameBuf), m_meleeObjectName.c_str());
		auto meleeObjectNameLabel = StringHelper::CreateLabel("MeleeObjectName", m_uuID);
		if (ImGui::InputText(meleeObjectNameLabel.c_str(), meleeObjectNameBuf, 256))
		{
			m_meleeObjectName = meleeObjectNameBuf;
		}

		auto endTimeLabel = StringHelper::CreateLabel("EndTime", m_uuID);
		if (ImGui::DragFloat(endTimeLabel.c_str(), &endTime))
		{

		}
		auto playLabel = StringHelper::CreateLabel("Play", m_uuID);
		if (ImGui::Button(playLabel.c_str()))
		{
			Play();
		}
	}
	void Melee::Play()
	{
		if (m_meleeObjectName == "")
		{
			return;
		}

		auto nowPosition = DirectX::SimpleMath::Vector3::Zero;
		auto nowRotation = DirectX::SimpleMath::Quaternion::Identity;

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pAnimater = pLockOwner->GetComponent<Animater>();
			if (auto pLockAnimater = pAnimater.lock())
			{
				// �A�j���[�V�����̃p�X�̕ύX
				pLockAnimater->SetFileName(m_meleeAnimation, endTime);
			}

			// ���W�̎擾
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				nowPosition = pLockTransform->GetPosition();
				nowRotation = pLockTransform->GetRotation();
			}
			auto pChildren = pLockOwner->GetChildren();
			for (auto& pChild : pChildren)
			{
				if (auto pLockChild = pChild.lock())
				{
					if (pLockChild->GetName() == m_meleeObjectName)
					{
						auto pMeleeMove = pLockChild->GetComponent<MeleeMove>();
						if (auto pLockMeleemove = pMeleeMove.lock())
						{
							pLockMeleemove->Play(MeleeMove::DEFAULT, nowPosition, nowRotation);
						}
					}
				}
			}

		}

		// �����@�������s����update�̎��ɏ��������I�u�W�F�N�g���Q�Ƃ���̂Ŏg�p���Ȃ�
		// �w��t�@�C����GameObject�𐶐�
		//auto pGenerateObj = GameObjectManager::Instance().LoadObject(m_generatePath);
		//if (auto pLockGenerateObj = pGenerateObj.lock())
		//{
		//	auto pGenerateObjMeleeMove = pLockGenerateObj->GetComponent<MeleeMove>();
		//	if (auto pLockGenerateObjMeleeMove = pGenerateObjMeleeMove.lock())
		//	{
		//		pLockGenerateObjMeleeMove->Play(MeleeMove::DEFAULT, nowPosition, nowRotation);
		//	}
		//}
	}
}