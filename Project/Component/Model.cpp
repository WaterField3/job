#include "Model.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"
#include "direct3d.h"
#include "Utility/Log.h"
#include "Utility/StringHelper.h"
#include "FreeCamera.h"

REGISTER_COMPONENT(TMF::Model, "Model");

namespace TMF
{
	Model::Model()
	{

	}

	Model::~Model()
	{

	}

	void Model::OnInitialize()
	{
		auto wideFileName = std::wstring(m_loadFile.begin(), m_loadFile.end());
		auto device = D3D::Get()->GetDevice();
		m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(device);
		auto wstringDirectry = std::wstring(m_loadDirectory.begin(), m_loadDirectory.end());
		m_pEffectFactory->SetDirectory(wstringDirectry.c_str());
		switch (m_loadType)
		{
		case TMF::Model::DEFAULT:
			break;
		case TMF::Model::CMO:
			LoadCMO();
			break;
		case TMF::Model::SDKMESH:
			LoadSdkMesh();
			break;
		default:
			break;
		}

	}

	void Model::LoadCMO()
	{
		auto device = D3D::Get()->GetDevice();
		auto wideFileName = std::wstring(m_loadFile.begin(), m_loadFile.end());
		try
		{
			m_pModel = DirectX::Model::CreateFromCMO(device, wideFileName.c_str(), *m_pEffectFactory, DirectX::ModelLoader_Clockwise | DirectX::ModelLoader_IncludeBones, &animOffset);
		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
	}

	void Model::LoadSdkMesh()
	{
		auto device = D3D::Get()->GetDevice();
		auto wideFileName = std::wstring(m_loadFile.begin(), m_loadFile.end());
		try
		{
			m_pModel = DirectX::Model::CreateFromSDKMESH(device, wideFileName.c_str(), *m_pEffectFactory, DirectX::ModelLoader_Clockwise | DirectX::ModelLoader_IncludeBones);
		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
	}

	void Model::OnFinalize()
	{

	}

	void Model::OnUpdate()
	{

	}

	void Model::OnLateUpdate()
	{

	}

	void Model::OnDraw()
	{

		if (m_isDraw)
		{
			ModelDraw();
		}
	}

	void Model::OnDrawImGui()
	{
		auto activeLabel = StringHelper::CreateLabel("Active", m_uuID);
		if (ImGui::Checkbox(activeLabel.c_str(), &m_isDraw))
		{

		}
		auto useWorldMatrixLabel = StringHelper::CreateLabel("IsUseWorldMatrix", m_uuID);
		if (ImGui::Checkbox(useWorldMatrixLabel.c_str(), &m_isUseWorldMatrix))
		{

		}
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_loadFile.c_str());
		auto label = StringHelper::CreateLabel("FileName", m_uuID);
		if (ImGui::InputText(label.c_str(), buf, 256))
		{
			m_loadFile = buf;
		}
		const char* types[] = { "Default","CMO","SDKMESH" };
		int selectIndex = (int)m_loadType;
		auto loadTypeCombo = StringHelper::CreateLabel("LoadType", m_uuID);
		if (ImGui::BeginCombo(loadTypeCombo.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_loadType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_loadType = LOADTYPE(i);
					selectIndex = i;

				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		char directoryBuf[256] = "";
		strcpy_s(directoryBuf, sizeof(directoryBuf), m_loadDirectory.c_str());
		auto directorylabel = StringHelper::CreateLabel("LoadDirectory", m_uuID);
		if (ImGui::InputText(directorylabel.c_str(), directoryBuf, sizeof(directoryBuf)))
		{
			m_loadDirectory = directoryBuf;
		}

		auto loadCmoLabel = StringHelper::CreateLabel("LoadCMO", m_uuID);
		if (ImGui::Button(loadCmoLabel.c_str()))
		{
			LoadCMO();
		}
		auto loadSdkMeshLabel = StringHelper::CreateLabel("LoadSDKMESH", m_uuID);
		if (ImGui::Button(loadSdkMeshLabel.c_str()))
		{
			LoadSdkMesh();
		}
	}

	void Model::ModelDraw()
	{
		if (!m_pModel)
		{
			return;
		}
		auto commonStates = D3D::Get()->GetCommonStates();
		auto d3dContext = D3D::Get()->GetContext();
		auto matrixWorld = DirectX::SimpleMath::Matrix::Identity;

		auto view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 5.0f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
		auto proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(1024) / float(576), 0.1f, 10.f);

		// camera‚ðŽæ“¾
		auto pCamera = GameObjectManager::Instance().GetComponent<Camera>();
		//auto pCamera = GameObjectManager::Instance().GetComponent<FreeCamera>();
		if (auto pLockCamera = pCamera.lock())
		{
			view = pLockCamera->GetViewMatrix();
			proj = pLockCamera->GetProjectionMatrix();
		}

		// Transform‚ðŽæ“¾
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				if (m_isUseWorldMatrix == true)
				{
					matrixWorld = pLockTransform->GetWorldMatrix();
				}
				else
				{
					matrixWorld = pLockTransform->GetLocalMatrix();
				}
			}
		}

		for (auto bone : m_pModel->bones)
		{
			if (bone.name == L"aaa")
			{
				bone;
			}
		}

		// ƒ‚ƒfƒ‹‚Ì•`‰æ
		m_pModel->Draw(d3dContext, *commonStates.get(), matrixWorld, view, proj);
		//DirectX::XMMATRIX blend;
		//m_pModel->Draw(d3dContext,*m_pCommonState,m_pModel->bones.size(), &blend, matrixWorld, view, proj)
	}

}
