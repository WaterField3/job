#include "Animation.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Model.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject/GameObjectManager.h"
#include "Timer.h"

REGISTER_COMPONENT(TMF::Animation, "Animation");

namespace TMF
{
	void Animation::OnInitialize()
	{
		if (auto pOwner = m_pOwner.lock())
		{
			auto pModel = pOwner->GetComponent<Model>();
			if (auto pLockModel = pModel.lock())
			{
				auto model = pLockModel->GetModel();
				m_pModel = model;
				if (auto lockModel = model.lock())
				{
					try
					{
						auto wideString = std::wstring(m_fileName.begin(), m_fileName.end());
						m_boneSize = lockModel->bones.size();

						//m_animation.Load(wideString.c_str(), m_animOffset);
						//m_pAnimation->Bind(*lockModel);
						//m_pDrawBone = DirectX::ModelBone::MakeArray(m_boneSize);
					}
					catch (const std::exception&)
					{

					}
				}
			}
		}
	}
	void Animation::OnFinalize()
	{

	}
	void Animation::OnUpdate()
	{

		auto deltaTime = Timer::Instance().deltaTime.count();
		//m_pAnimation->Update(deltaTime);
	}
	void Animation::OnLateUpdate()
	{
	}
	void Animation::OnDraw()
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
			//m_pAnimation->Apply(*pLockModel, m_boneSize, m_pDrawBone.get());
			//pLockModel->DrawSkinned(context, *state, m_boneSize, m_pDrawBone.get(), world, view, proj);

		}

	}
	void Animation::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_fileName.c_str());
		if (ImGui::InputText("SoundName", buf, 256))
		{
			m_fileName = buf;
		}

		if (ImGui::Button("Load"))
		{
			auto pOwner = m_pOwner.lock();
			auto pModel = pOwner->GetComponent<Model>();
			if (auto pLockModel = pModel.lock())
			{
				auto model = pLockModel->GetModel();
				m_pModel = model;
				if (auto lockModel = model.lock())
				{
					try
					{
						auto wideString = std::wstring(m_fileName.begin(), m_fileName.end());
						m_boneSize = lockModel->bones.size();
						//m_pAnimation = std::make_unique<DX::AnimationCMO>();
						//m_pAnimation->Load(wideString.c_str(), m_animOffset);
						//m_pAnimation->Bind(*lockModel);
						//m_pDrawBone = DirectX::ModelBone::MakeArray(m_boneSize);
					}
					catch (const std::exception&)
					{

					}
				}
			}
		}
	}
}