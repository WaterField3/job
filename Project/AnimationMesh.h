#pragma once
#include "StaticMesh.h"

#include "AssimpPerse.h"
#include "TreeNode.h"
#include "direct3d.h"
#include "BoneCombMatrix.h"
#include "StaticMeshRenderer.h"

namespace TMF
{
	class AnimationMesh : public StaticMesh
	{
		// �{�[������
		std::unordered_map<std::string, D3D::BONE> m_BoneDictionary{};	// 20240714 DX��

		// �J�����g�̃A�j���[�V�����f�[�^
		std::shared_ptr<aiAnimation> m_CurrentAnimation{};

		std::shared_ptr<aiAnimation> m_FromAnimation{};
		std::shared_ptr<aiAnimation> m_ToAnimation{};

		// assimp �m�[�h���c���[�i�e�q�֌W���킩��j
		std::shared_ptr<TreeNode<std::string>> m_AssimpNodeNameTree;

		// �����_��
		StaticMeshRenderer m_StaticMeshRenderer{};

	public:

		static AnimationMesh& Instance()
		{
			static AnimationMesh instance;
			return instance;
		}

		inline void SetCurentAnimation(std::shared_ptr<aiAnimation> currentanimation) { m_CurrentAnimation = std::move(currentanimation); }
		inline void SetFromAnimation(std::shared_ptr<aiAnimation> animation) { m_FromAnimation = std::move(animation); }
		inline void SetToAnimation(std::shared_ptr<aiAnimation> animation) { m_ToAnimation = std::move(animation); }

		void Load(std::string filename, std::string texturedirectory = "");

		// �K�w�\�����l�������{�[���R���r�l�[�V�����s����X�V
		void UpdateBoneMatrix(std::weak_ptr<TreeNode<std::string>> pTree, DirectX::SimpleMath::Matrix matrix);		// 20240714 DX��	

		// �A�j���[�V�����̍X�V
		void Update(BoneCombMatrix& bonecombarray, int& CurrentFrame);

		// �A�j���[�V�����̍X�V�i���[�V�����u�����h�j
		void BlendUpdate(
			BoneCombMatrix& bonecombarray,			// �{�[���R���r�l�[�V�����s��ƌ��݂̃t���[��	
			int fromframe, 							// FROM���̃��[�V�������ƃt���[��
			int toframe,							// TO���̃��[�V�������ƃt���[��
			float blendarate);						// ����

		// �`��
		void Draw();

	private:
		DirectX::SimpleMath::Matrix AiMatrixToSimpleMatrix(aiMatrix4x4 asimpmtx);
	};
}

