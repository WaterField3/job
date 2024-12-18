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
		// ボーン辞書
		std::unordered_map<std::string, D3D::BONE> m_BoneDictionary{};	// 20240714 DX化

		// カレントのアニメーションデータ
		std::shared_ptr<aiAnimation> m_CurrentAnimation{};

		std::shared_ptr<aiAnimation> m_FromAnimation{};
		std::shared_ptr<aiAnimation> m_ToAnimation{};

		// assimp ノード名ツリー（親子関係がわかる）
		std::shared_ptr<TreeNode<std::string>> m_AssimpNodeNameTree;

		// レンダラ
		StaticMeshRenderer m_StaticMeshRenderer{};

	public:

		//static AnimationMesh& Instance()
		//{
		//	static AnimationMesh instance;
		//	return instance;
		//}

		inline void SetCurentAnimation(std::shared_ptr<aiAnimation> currentanimation) { m_CurrentAnimation = std::move(currentanimation); }
		inline void SetFromAnimation(std::shared_ptr<aiAnimation> animation) { m_FromAnimation = std::move(animation); }
		inline void SetToAnimation(std::shared_ptr<aiAnimation> animation) { m_ToAnimation = std::move(animation); }

		void Load(std::string filename, std::string texturedirectory = "");

		// 階層構造を考慮したボーンコンビネーション行列を更新
		void UpdateBoneMatrix(std::weak_ptr<TreeNode<std::string>> pTree, DirectX::SimpleMath::Matrix matrix);		// 20240714 DX化	

		// アニメーションの更新
		void Update(BoneCombMatrix& bonecombarray, int& CurrentFrame);

		// アニメーションの更新（モーションブレンド）
		void BlendUpdate(
			BoneCombMatrix& bonecombarray,			// ボーンコンビネーション行列と現在のフレーム	
			int fromframe, 							// FROM側のモーション名とフレーム
			int toframe,							// TO側のモーション名とフレーム
			float blendarate);						// 割合

		// 描画
		void Draw();

	private:
		DirectX::SimpleMath::Matrix AiMatrixToSimpleMatrix(aiMatrix4x4 asimpmtx);
	};
}

