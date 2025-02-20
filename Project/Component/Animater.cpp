#include "Animater.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Model.h"
#include "Transform.h"
#include "Camera.h"
#include "FreeCamera.h"
#include "GameObject/GameObjectManager.h"
#include "Timer.h"
#include "System/Animation.h"
#include "Utility/Log.h"
#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::Animater, "Animater");

namespace TMF
{
	void Animater::OnInitialize()
	{
		m_initAnimationSpeed = m_animationSpeed;
		// アニメーションの読み込み
		LoadAnimation();
	}

	void Animater::OnFinalize()
	{

	}

	void Animater::OnUpdate()
	{
		auto deltaTime = Timer::Instance().deltaTime.count();

		auto isUpdate = true;
		if (m_timer > m_animEndTime)
		{
			if (m_isNextAnimSet == true)
			{
				m_timer = 0;
				m_fromAnimName = m_nextPath;
				m_animEndTime = m_nextAnimEnd;
				m_animationSpeed = m_nextAnimationSpeed;
				m_isNextAnimSet = false;
				LoadAnimation();
			}
			else if (m_fromAnimName != m_idlePath)
			{
				m_animEndTime = 1.0f;
				m_fromAnimName = m_idlePath;
				m_animationSpeed = m_initAnimationSpeed;
				LoadAnimation();
			}
			else
			{
				m_timer = 0;
			}
		}
		else
		{
			if (m_animEndTime != 0.0f)
			{
				if (m_timer > m_animStopTime && m_animStopEndTime > 0.0f)
				{
					m_animStopTime -= deltaTime;
					m_animStopEndTime -= deltaTime;
					if (m_animStopEndTime > 0.0f)
					{
						isUpdate = false;
					}
				}
			}
		}

		if (isUpdate == true)
		{
			m_timer += deltaTime * m_animationSpeed;
			if (m_pAnimationCMO)
			{
				m_pAnimationCMO->Update(deltaTime * m_animationSpeed);
			}
			else if (m_pAnimationSDKMESH)
			{
				m_pAnimationSDKMESH->Update(deltaTime * m_animationSpeed);
			}
		}
		//m_animationBlendRate += deltaTime;
		if (m_pAnimationSDKMESH)
		{
			m_pAnimationSDKMESH->SetBlendRate(m_animationBlendRate);
		}
		if (m_animationBlendRate > 1)
		{
			m_animationBlendRate = 0.0f;
		}
	}

	void Animater::OnLateUpdate()
	{

	}

	void Animater::OnDraw()
	{
		if (m_isAnimation == false)
		{
			return;
		}

		auto world = DirectX::SimpleMath::Matrix::Identity;
		auto view = DirectX::SimpleMath::Matrix::Identity;
		auto proj = DirectX::SimpleMath::Matrix::Identity;
		auto pCamera = GameObjectManager::Instance().GetComponent<Camera>();
		if (auto pLockCamera = pCamera.lock())
		{
			view = pLockCamera->GetViewMatrix();
			proj = pLockCamera->GetProjectionMatrix();
		}

		if (auto pLockOnwer = m_pOwner.lock())
		{
			auto pTransform = pLockOnwer->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
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
			else if (m_pAnimationSDKMESH)
			{
				m_pAnimationSDKMESH->Apply(*pLockModel, m_boneSize, m_drawBone.get());
			}
			pLockModel->DrawSkinned(context, *state, m_boneSize, m_drawBone.get(), world, view, proj);

		}

	}

	void Animater::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_fromAnimName.c_str());
		auto label = StringHelper::CreateLabel("FromAnimName", m_uuID);
		if (ImGui::InputText(label.c_str(), buf, 256))
		{
			m_fromAnimName = buf;
		}
		char toBuf[256] = "";
		strcpy_s(toBuf, sizeof(toBuf), m_toAnimName.c_str());
		auto toAnimlabel = StringHelper::CreateLabel("ToAnimName", m_uuID);
		if (ImGui::InputText(toAnimlabel.c_str(), toBuf, 256))
		{
			m_toAnimName = toBuf;
		}

		char idleBuf[256] = "";
		strcpy_s(idleBuf, sizeof(idleBuf), m_idlePath.c_str());
		auto idleLabel = StringHelper::CreateLabel("IdlePath", m_uuID);
		if (ImGui::InputText(idleLabel.c_str(), idleBuf, 256))
		{
			m_idlePath = idleBuf;
		}
		auto animSpeedLabel = StringHelper::CreateLabel("AnimSpeed", m_uuID);
		if (ImGui::DragFloat(animSpeedLabel.c_str(), &m_animationSpeed, 0.1f))
		{

		}

		auto animBlendRateLabel = StringHelper::CreateLabel("BlendRate", m_uuID);
		if (ImGui::DragFloat(animBlendRateLabel.c_str(), &m_animationBlendRate, 0.1f))
		{
			if (m_pAnimationSDKMESH)
			{
				m_pAnimationSDKMESH->SetBlendRate(m_animationBlendRate);
			}
		}
		auto loadCMOLabel = StringHelper::CreateLabel("LoadCmo", m_uuID);
		if (ImGui::Button(loadCMOLabel.c_str()))
		{
			LoadCMO();
		}
		auto loadSDKMESHLabel = StringHelper::CreateLabel("LoadSdkMesh", m_uuID);
		if (ImGui::Button(loadSDKMESHLabel.c_str()))
		{
			LoadSDKMESH();
		}
		auto animButtonLabel = StringHelper::CreateLabel("IsAnimation", m_uuID);
		if (ImGui::Checkbox(animButtonLabel.c_str(), &m_isAnimation))
		{

		}
		auto endTimeLabel = StringHelper::CreateLabel("EndTime", m_uuID);
		if (ImGui::DragFloat(endTimeLabel.c_str(), &m_animEndTime))
		{

		}
	}

	std::shared_ptr<Component> Animater::OnClone() const
	{
		auto pClone = std::make_shared<Animater>();
		pClone->m_fromAnimName = this->m_fromAnimName;
		pClone->m_idlePath = this->m_idlePath;
		pClone->m_animationSpeed = this->m_animationSpeed;
		pClone->m_isAnimation = this->m_isAnimation;
		return move(pClone);
	}

	void Animater::SetFileName(std::string fileName, float endTime, float animSpeed)
	{
		if (m_timer > m_animEndTime || m_fromAnimName == m_idlePath)
		{
			m_fromAnimName = fileName;
			m_timer = 0.0f;
			m_animEndTime = endTime;
			m_animStopEndTime = 0.0f;
			m_animStopTime = 0.0f;
			if (animSpeed != 0.0f)
			{
				m_animationSpeed = animSpeed;
			}
			LoadAnimation();
		}
		else
		{
			m_nextPath = fileName;
			m_nextAnimEnd = endTime;
			m_isNextAnimSet = true;
			if (animSpeed != 0.0f)
			{
				m_nextAnimationSpeed = animSpeed;
			}
		}
	}

	void Animater::SetFileName(std::string fileName, float endTime, float animSpeed, float animStopTime)
	{
		if (m_timer > m_animEndTime || m_fromAnimName == m_idlePath)
		{
			m_fromAnimName = fileName;
			m_timer = 0.0f;
			m_animEndTime = endTime;
			m_animStopTime = animStopTime;
			m_animStopEndTime = endTime - animStopTime;
			if (animSpeed != 0.0f)
			{
				m_animationSpeed = animSpeed;
			}
			LoadAnimation();
		}
		else
		{
			m_nextPath = fileName;
			m_nextAnimEnd = endTime;
			m_isNextAnimSet = true;
			m_animStopTime = animStopTime;
			m_animStopEndTime = endTime - animStopTime;
			if (animSpeed != 0.0f)
			{
				m_nextAnimationSpeed = animSpeed;
			}
		}
	}

	void Animater::LoadAnimation()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pModel = pLockOwner->GetComponent<Model>();
			if (auto pLockModel = pModel.lock())
			{
				auto loadType = pLockModel->GetLoadType();
				switch (loadType)
				{
				case TMF::Model::DEFAULT:
					break;
				case TMF::Model::CMO:
					LoadCMO();
					break;
				case TMF::Model::SDKMESH:
					LoadSDKMESH();
					break;
				default:
					break;
				}
			}
		}
	}

	void Animater::LoadDefaltAnimation()
	{
		m_fromAnimName = m_idlePath;
		LoadAnimation();
	}

	DirectX::SimpleMath::Vector3 Animater::GetBonePosition(std::string findName)
	{
		auto position = DirectX::SimpleMath::Vector3::Zero;
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				if (auto pLockModel = m_pModel.lock())
				{
					auto data = m_pAnimationSDKMESH->GetFrameData(*pLockModel, m_boneSize, findName);
					position = data.pAnimationData->Translation;
				}
			}
		}
		return position;
	}

	DirectX::SimpleMath::Quaternion Animater::GetBoneRotation(std::string findName)
	{
		auto rotation = DirectX::SimpleMath::Quaternion::Identity;
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				if (auto pLockModel = m_pModel.lock())
				{
					auto data = m_pAnimationSDKMESH->GetFrameData(*pLockModel, m_boneSize, findName);
					rotation = data.pAnimationData->Orientation;
				}
			}
		}
		return rotation;
	}

	void Animater::LoadCMO()
	{
		auto pLockOwner = m_pOwner.lock();
		auto pModel = pLockOwner->GetComponent<Model>();
		if (auto pLockModel = pModel.lock())
		{
			auto model = pLockModel->GetModel();
			if (m_fromAnimName == "")
			{
				return;
			}
			m_pModel = model;
			m_animOffset = pLockModel->GetAnimOffset();
			if (auto lockModel = model.lock())
			{
				try
				{
					auto wideString = std::wstring(m_fromAnimName.begin(), m_fromAnimName.end());
					m_boneSize = lockModel->bones.size();
					m_pAnimationCMO = std::make_unique<DX::AnimationCMO>();
					m_pAnimationCMO->Load(wideString.c_str(), m_animOffset);
					m_pAnimationCMO->Bind(*lockModel);
					m_drawBone = DirectX::ModelBone::MakeArray(m_boneSize);
					m_timer = 0;

					if (m_pAnimationSDKMESH)
					{
						m_pAnimationSDKMESH.release();
					}
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
		auto pLockOwner = m_pOwner.lock();
		auto pModel = pLockOwner->GetComponent<Model>();
		if (auto pLockModel = pModel.lock())
		{
			auto model = pLockModel->GetModel();
			if (m_fromAnimName == "")
			{
				return;
			}
			m_pModel = model;
			m_animOffset = pLockModel->GetAnimOffset();
			if (auto lockModel = model.lock())
			{
				try
				{
					auto wideString = std::wstring(m_fromAnimName.begin(), m_fromAnimName.end());
					m_boneSize = lockModel->bones.size();
					m_pAnimationSDKMESH = std::make_unique<DX::AnimationSDKMESH>();
					if (m_toAnimName == "")
					{
						m_pAnimationSDKMESH->Load(wideString.c_str());
					}
					else
					{
						auto toAnimWideString = std::wstring(m_toAnimName.begin(), m_toAnimName.end());
						m_pAnimationSDKMESH->Load(wideString.c_str(), toAnimWideString.c_str());
					}
					m_pAnimationSDKMESH->Bind(*lockModel);
					m_drawBone = DirectX::ModelBone::MakeArray(m_boneSize);
					m_timer = 0;
					if (m_pAnimationCMO)
					{
						m_pAnimationCMO.release();
					}
				}
				catch (const std::exception& e)
				{
					Log::Info("%s", e.what());
				}
			}
		}
	}
}