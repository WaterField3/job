#include "EffectManager.h"

#include "direct3d.h"

#include "GameObject/GameObjectManager.h"
#include "Component/Camera.h"

namespace TMF
{
	void EffectManager::Initialize()
	{
		m_pEffekseerManager = Effekseer::Manager::Create(100);
		m_pEffectRenderer = EffekseerRendererDX11::Renderer::Create(D3D::Get()->GetDevice(), D3D::Get()->GetContext(), 100);

		m_pEffekseerManager->SetSpriteRenderer(m_pEffectRenderer->CreateSpriteRenderer());
		m_pEffekseerManager->SetRibbonRenderer(m_pEffectRenderer->CreateRibbonRenderer());
		m_pEffekseerManager->SetRingRenderer(m_pEffectRenderer->CreateRingRenderer());
		m_pEffekseerManager->SetModelRenderer(m_pEffectRenderer->CreateModelRenderer());
		m_pEffekseerManager->SetTrackRenderer(m_pEffectRenderer->CreateTrackRenderer());
		// テクスチャ、モデル、カーブ、マテリアルローダーの設定
		m_pEffekseerManager->SetTextureLoader(m_pEffectRenderer->CreateTextureLoader());
		m_pEffekseerManager->SetModelLoader(m_pEffectRenderer->CreateModelLoader());
		m_pEffekseerManager->SetMaterialLoader(m_pEffectRenderer->CreateMaterialLoader());
		m_pEffekseerManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
		auto camera = GameObjectManager::Instance().GetComponent<Camera>();
		Effekseer::Matrix44 projMatrix;
		Effekseer::Matrix44 viewMatrix;
		if (auto cam = camera.lock())
		{
			auto camProj = cam->GetProjectionMatrix();
			auto camView = cam->GetViewMatrix();
			CopyMatrix(camProj, projMatrix);
			CopyMatrix(camView, viewMatrix);
			m_pEffectRenderer->SetProjectionMatrix(projMatrix);
			m_pEffectRenderer->SetCameraMatrix(viewMatrix);
		}
	}
	void EffectManager::Finalize()
	{
		//if (m_pEffect != nullptr)
		//{
			//m_pEffect->Release();
		//}
		//m_pEffectRenderer->Release();
		//m_pEffectRenderer = nullptr;
		//m_pEffekseerManager->Release();
		//m_pEffekseerManager = nullptr;
	}
	void EffectManager::Update()
	{
		auto camera = GameObjectManager::Instance().GetComponent<Camera>();
		if (auto cam = camera.lock())
		{
			Effekseer::Matrix44 projMatrix;
			Effekseer::Matrix44 viewMatrix;
			auto camProj = cam->GetProjectionMatrix();
			auto camView = cam->GetViewMatrix();
			CopyMatrix(camProj, projMatrix);
			CopyMatrix(camView, viewMatrix);
			m_pEffectRenderer->SetProjectionMatrix(projMatrix);
			m_pEffectRenderer->SetCameraMatrix(viewMatrix);
		}
		m_pEffekseerManager->Update();
	}
	void EffectManager::Draw()
	{
		m_pEffectRenderer->BeginRendering();
		m_pEffekseerManager->Draw();
		m_pEffectRenderer->EndRendering();
	}
	void EffectManager::Play(std::string name, DirectX::SimpleMath::Vector3 pos)
	{
		auto uName = std::u16string(name.begin(), name.end());
		m_pEffect = Effekseer::Effect::Create(m_pEffekseerManager, uName.c_str());
		auto handle = m_pEffekseerManager->Play(m_pEffect, pos.x, pos.y, pos.z);
	}
	void EffectManager::CopyMatrix(const DirectX::SimpleMath::Matrix& tkMatrix, Effekseer::Matrix44& efMatrix)
	{
		efMatrix.Values[0][0] = tkMatrix._11;
		efMatrix.Values[0][1] = tkMatrix._12;
		efMatrix.Values[0][2] = tkMatrix._13;
		efMatrix.Values[0][3] = tkMatrix._14;

		efMatrix.Values[1][0] = tkMatrix._21;
		efMatrix.Values[1][1] = tkMatrix._22;
		efMatrix.Values[1][2] = tkMatrix._23;
		efMatrix.Values[1][3] = tkMatrix._24;

		efMatrix.Values[2][0] = tkMatrix._31;
		efMatrix.Values[2][1] = tkMatrix._32;
		efMatrix.Values[2][2] = tkMatrix._33;
		efMatrix.Values[2][3] = tkMatrix._34;

		efMatrix.Values[3][0] = tkMatrix._41;
		efMatrix.Values[3][1] = tkMatrix._42;
		efMatrix.Values[3][2] = tkMatrix._43;
		efMatrix.Values[3][3] = tkMatrix._44;
	}
}