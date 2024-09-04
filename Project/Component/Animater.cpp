#include "Animater.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Model.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject/GameObjectManager.h"
#include "Timer.h"
#include "System/Animation.h"
#include "Utility/Log.h"

REGISTER_COMPONENT(TMF::Animater, "Animater");

namespace TMF
{
	void Animater::OnInitialize()
	{
		//LoadCMO();
	}
	void Animater::OnFinalize()
	{

	}
	void Animater::OnUpdate()
	{

		auto deltaTime = Timer::Instance().deltaTime.count();
		if (m_pAnimationCMO)
		{
			m_pAnimationCMO->Update(deltaTime);
		}
		if (m_pAnimationSDKMESH)
		{
			m_pAnimationSDKMESH->Update(deltaTime);
		}
	}
	void Animater::OnLateUpdate()
	{
	}
	void Animater::OnDraw()
	{

		auto world = DirectX::SimpleMath::Matrix::Identity;
		auto view = DirectX::SimpleMath::Matrix::Identity;
		auto proj = DirectX::SimpleMath::Matrix::Identity;
		auto camera = GameObjectManager::Instance().GetComponent<Camera>();
		if (auto pLockCamera = camera.lock())
		{
			view = pLockCamera->GetViewMatrix();
			proj = pLockCamera->GetProjectionMatrix();
		}

		if (auto pOnwer = m_pOwner.lock())
		{
			auto transform = pOnwer->GetComponent<Transform>();
			if (auto pLockTransform = transform.lock())
			{
				world = pLockTransform->GetWorldMatrix();
			}
		}

		if (auto pLockModel = m_pModel.lock())
		{
			auto context = D3D::Get()->GetContext();
			auto state = D3D::Get()->GetCommonStates();
			if (m_pAnimationCMO)
			{
				m_pAnimationCMO->Apply(*pLockModel, m_boneSize, m_drawBone.get());
			}
			if (m_pAnimationSDKMESH)
			{
				m_pAnimationSDKMESH->Apply(*pLockModel, m_boneSize, m_drawBone.get());
			}
			pLockModel->DrawSkinned(context, *state, m_boneSize, m_drawBone.get(), world, view, proj);

		}

	}
	void Animater::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_fileName.c_str());
		if (ImGui::InputText("FileName", buf, 256))
		{
			m_fileName = buf;
		}

		if (ImGui::Button("LoadCMO"))
		{
			LoadCMO();
		}
		if (ImGui::Button("LoadSDKMESH"))
		{
			LoadSDKMESH();
		}
	}
	void Animater::LoadCMO()
	{
		auto pOwner = m_pOwner.lock();
		auto pModel = pOwner->GetComponent<Model>();
		if (auto pLockModel = pModel.lock())
		{
			auto model = pLockModel->GetModel();
			if (m_fileName == "")
			{
				return;
			}
			m_pModel = model;
			m_animOffset = pLockModel->GetAnimOffset();
			if (auto lockModel = model.lock())
			{
				try
				{
					auto wideString = std::wstring(m_fileName.begin(), m_fileName.end());
					m_boneSize = lockModel->bones.size();
					m_pAnimationCMO = std::make_unique<DX::AnimationCMO>();
					m_pAnimationCMO->Load(wideString.c_str(), m_animOffset);
					m_pAnimationCMO->Bind(*lockModel);
					m_drawBone = DirectX::ModelBone::MakeArray(m_boneSize);
				}
				catch (const std::exception& e)
				{
					Log::Info("%s", e.what());
				}
			}
		}
	}
	void Animater::LoadSDKMESH()
	{
		auto pOwner = m_pOwner.lock();
		auto pModel = pOwner->GetComponent<Model>();
		if (auto pLockModel = pModel.lock())
		{
			auto model = pLockModel->GetModel();
			if (m_fileName == "")
			{
				return;
			}
			m_pModel = model;
			m_animOffset = pLockModel->GetAnimOffset();
			if (auto lockModel = model.lock())
			{
				try
				{
					auto wideString = std::wstring(m_fileName.begin(), m_fileName.end());
					m_boneSize = lockModel->bones.size();
					m_pAnimationSDKMESH = std::make_unique<DX::AnimationSDKMESH>();
					m_pAnimationSDKMESH->Load(wideString.c_str());
					m_pAnimationSDKMESH->Bind(*lockModel);
					m_drawBone = DirectX::ModelBone::MakeArray(m_boneSize);
				}
				catch (const std::exception& e)
				{
					Log::Info("%s", e.what());
				}
			}
		}
	}
}