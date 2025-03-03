#include "AssimpAnimation.h"

#include "ComponentRegister.h"
#include "Timer.h"
#include "Utility/Log.h"
#include "Transform.h"
#include "GameObject/GameObjectManager.h"
#include "Camera.h"

REGISTER_COMPONENT(TMF::AssimpAnimation, "AssimpAnimation");

namespace TMF
{
	void AssimpAnimation::OnInitialize()
	{
		m_BoneCombMatrix.Create();

		m_AnimMesh = std::make_unique<AnimationMesh>();

		// アニメーションメッシュ読み込み
		m_AnimMesh->Load(m_currentFile, m_textureDirectory);

		m_shader.Create("Shader/vertexLightingOneSkinVS.hlsl", "Shader/vertexLightingPS.hlsl");

		auto anim = std::make_shared<AnimationData>();
		try
		{
			anim->LoadAnimation(m_motionFile, "Idle");
			m_animationData.push_back(move(anim));
		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
		try
		{
			auto anim2 = std::make_shared<AnimationData>();
			anim2->LoadAnimation("asset/model/03Run.fbx", "Run");
			m_animationData.push_back(move(anim2));
		}
		catch (const std::exception&)
		{

		}

		m_CurrentAnimation = m_animationData[0]->GetAnimation("Idle");
		m_ToAnimation = m_CurrentAnimation;
		m_FromAnimation = m_animationData[1]->GetAnimation("Run");

		// アニメーションメッシュのカレントアニメーションをセット
		m_AnimMesh->SetCurentAnimation(m_CurrentAnimation);
	}
	void AssimpAnimation::OnFinalize()
	{

	}
	void AssimpAnimation::OnUpdate()
	{
		if (m_CurrentAnimation == nullptr)
		{
			return;
		}

		auto frame = static_cast<int>(m_CurrentFrame);

		// アニメーションメッシュ更新
		m_AnimMesh->Update(m_BoneCombMatrix, frame);

		if (m_FromAnimation == nullptr || m_ToAnimation == nullptr)
		{
			return;
		}
		// アニメーションメッシュのブレンドアニメーションをセット
		m_AnimMesh->SetFromAnimation(m_FromAnimation);
		m_AnimMesh->SetToAnimation(m_ToAnimation);

		m_AnimMesh->BlendUpdate(m_BoneCombMatrix, frame, frame, m_BlendRate);
		m_CurrentFrame += Timer::Instance().deltaTime.count();


	}
	void AssimpAnimation::OnLateUpdate()
	{

	}
	void AssimpAnimation::OnDraw()
	{
		if (m_CurrentAnimation == nullptr)
		{
			return;
		}

		m_shader.SetGPU();

		// ボーンコンビネーション行列用定数バッファ更新
		m_BoneCombMatrix.Update();

		// 定数バッファGPUへセット
		m_BoneCombMatrix.SetGPU();

		// メッシュ描画
		m_AnimMesh->Draw();
	}
	void AssimpAnimation::OnDrawImGui()
	{

	}
	std::shared_ptr<Component> AssimpAnimation::OnClone() const
	{
		return std::shared_ptr<Component>();
	}
}
