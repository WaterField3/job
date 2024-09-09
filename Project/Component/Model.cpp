#include "Model.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"
#include "direct3d.h"
#include "Utility/Log.h"
#include "Utility/StringHelper.h"

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
		m_pCommonState = std::make_unique<DirectX::CommonStates>(device);
		m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(device);
		m_pEffectFactory->SetDirectory(L"asset");
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
			m_pModel = DirectX::Model::CreateFromCMO(device, wideFileName.c_str(), *m_pEffectFactory, DirectX::ModelLoader_CounterClockwise | DirectX::ModelLoader_IncludeBones, &animOffset);
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
			m_pModel = DirectX::Model::CreateFromSDKMESH(device, wideFileName.c_str(), *m_pEffectFactory, DirectX::ModelLoader_CounterClockwise | DirectX::ModelLoader_IncludeBones);
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
		ImGui::Checkbox("Active", &m_isDraw);
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_loadFile.c_str());
		auto label = StringHelper::CreateLabel("FileName", m_uuID);
		if (ImGui::InputText(label.c_str(), buf, 256))
		{
			m_loadFile = buf;
		}
		const char* types[] = { "Default","CMO","SDKMESH" };
		static int selectIndex = (int)m_loadType;
		if (ImGui::BeginCombo("LoadType", types[selectIndex]))
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
			ImGui::SameLine();
		}
		if (ImGui::Button("LoadCmo"))
		{
			LoadCMO();
		}

		if (ImGui::Button("LoadSdkMesh"))
		{
			LoadSdkMesh();
		}
	}

	void Model::ModelDraw()
	{
		ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

		D3D::ConstBuffer cb;
		DirectX::SimpleMath::Matrix matrixWorld;

		auto view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 5.0f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
		auto proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(1024) / float(576), 0.1f, 10.f);
		for (auto pGameObject : GameObjectManager::Instance().GetGameObjects())
		{
			if (pGameObject->GetName() == "CameraObject")
			{
				auto cameraComponent = pGameObject->GetComponent<Camera>();
				if (auto camera = cameraComponent.lock())
				{
					view = camera->GetViewMatrix();
					proj = camera->GetProjectionMatrix();
				}
			}
		}

		// Transform���擾
		if (auto gameObject = m_pOwner.lock())
		{
			auto Component = gameObject->GetComponent<Transform>();
			if (auto pComponent = Component.lock())
			{
				matrixWorld = pComponent->GetWorldMatrix();
			}
		}

		// ���f���̕`��(�����)
		if (m_pModel)
		{
			m_pModel->Draw(d3dContext, *m_pCommonState, matrixWorld, view, proj);
		}
	}

}
