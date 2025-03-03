#pragma once
#include "Component.h"

#include <string>
#include <memory>
#include <map>

#include "ComponentCerealHelper.h"
#include "BoneCombMatrix.h"
#include "AnimationMesh.h"
#include "AnimationData.h"
#include "Shader.h"

namespace TMF
{
	class AssimpAnimation : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

	private:

		// SRT���
		//DirectX::SimpleMath::Vector3 m_Position = { 0,0,0 };
		//DirectX::SimpleMath::Vector3 m_Rotation = { 0,0,0 };
		//DirectX::SimpleMath::Vector3 m_Scale = { 0.1f,0.1f,0.1f };

		// ���݃t���[��
		float m_CurrentFrame = 0;

		// �u�����h���C�g
		float m_BlendRate = 1.0f;									// ���[�V�����̃u�����h��

		Shader m_shader;

		// �{�[���R���r�l�[�V�����s��p�萔�o�b�t�@���e
		BoneCombMatrix m_BoneCombMatrix{};							// 20240723

		// �A�j���[�V�������b�V��
		std::unique_ptr<AnimationMesh> m_AnimMesh;

		// �A�j���[�V�����f�[�^
		std::vector<std::shared_ptr<AnimationData>> m_animationData;

		// ���f���t�@�C��
		std::string m_currentFile = "asset/model/03.fbx";

		// �e�N�X�`���t�@�C��
		std::string m_textureDirectory = "asset/model";

		std::string m_motionFile = "asset/model/03Idle.fbx";

		// ���݂̃A�j���[�V����
		std::shared_ptr<aiAnimation> m_CurrentAnimation;
		std::shared_ptr<aiAnimation> m_FromAnimation;
		std::shared_ptr<aiAnimation> m_ToAnimation;

	public:

		void SetAnimationMesh(std::unique_ptr<AnimationMesh> animmesh) { m_AnimMesh = std::move(animmesh); }

		void SetCurrentAnimation(std::shared_ptr<aiAnimation> anim) { m_CurrentAnimation = anim; }

		void SetFromAnimation(std::shared_ptr<aiAnimation> anim) { m_FromAnimation = anim; }

		void SetToAnimation(std::shared_ptr<aiAnimation> anim) { m_ToAnimation = anim; }

		void SetBlendRate(float rate) { m_BlendRate = rate; }
	};
}

