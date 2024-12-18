#include "AssimpAnimation.h"

#include "ComponentRegister.h"
#include "Timer.h"
#include "Utility/Log.h"

//REGISTER_COMPONENT(TMF::AssimpAnimation, "AssimpAnimation");

namespace TMF
{
	void AssimpAnimation::OnInitialize()
	{
		m_BoneCombMatrix.Create();

		m_AnimMesh = std::make_unique<AnimationMesh>();

		// �A�j���[�V�������b�V���ǂݍ���
		m_AnimMesh->Load(m_currentFile, m_textureDirectory);

		m_shader.Create("shader/vertexLightingOneSkinVS.hlsl", "shader/vertexLightingPS.hlsl");

		auto anim = std::make_shared<AnimationData>();
		try
		{
			anim->LoadAnimation(m_motionFile, "Idle");
		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
		m_animationData.push_back(anim);

		m_CurrentAnimation = m_animationData[0]->GetAnimation("Idle");

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


		// �A�j���[�V�������b�V���̃J�����g�A�j���[�V�������Z�b�g
		m_AnimMesh->SetCurentAnimation(m_CurrentAnimation);

		// �A�j���[�V�������b�V���X�V
		m_AnimMesh->Update(m_BoneCombMatrix, frame);

		if (m_FromAnimation == nullptr || m_ToAnimation == nullptr)
		{
			return;
		}
		// �A�j���[�V�������b�V���̃u�����h�A�j���[�V�������Z�b�g
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

		// m_shader.SetGPU();

		// �{�[���R���r�l�[�V�����s��p�萔�o�b�t�@�X�V
		m_BoneCombMatrix.Update();

		// �萔�o�b�t�@GPU�փZ�b�g
		m_BoneCombMatrix.SetGPU();

		// ���b�V���`��
		m_AnimMesh->Draw();
	}
	void AssimpAnimation::OnDrawImGui()
	{

	}
}
