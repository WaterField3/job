#include "AnimationMesh.h"

// 作成中

namespace TMF
{
	void AnimationMesh::Load(std::string filename, std::string texturedirectory)
	{
		// メッシュ読み込み
		StaticMesh::Instance().Load(filename, texturedirectory);

		// アニメーションデータ(ASSIMP用）
		std::unordered_map<std::string, AssimpPerse::BONE> assimp_BoneDictionary{};

		// ボーン辞書取得（ボーン名をキーにしてボーン情報が取れる）
		assimp_BoneDictionary = AssimpPerse::Instance().GetBoneDictionary();

		for (auto& asimpbone : assimp_BoneDictionary) {
			D3D::BONE dxbone;

			dxbone.meshname = asimpbone.second.meshname;
			dxbone.armaturename = asimpbone.second.armaturename;
			dxbone.bonename = asimpbone.second.bonename;
			dxbone.idx = asimpbone.second.idx;

			dxbone.OffsetMatrix = AiMatrixToSimpleMatrix(asimpbone.second.OffsetMatrix);
			dxbone.AnimationMatrix = DirectX::SimpleMath::Matrix::Identity;
			dxbone.Matrix = DirectX::SimpleMath::Matrix::Identity;

			dxbone.weights.clear();
			for (auto& asimpweight : asimpbone.second.weights)
			{
				D3D::WEIGHT dxweight;
				dxweight.bonename = asimpweight.bonename;
				dxweight.meshname = asimpweight.meshname;
				dxweight.vertexindex = asimpweight.vertexindex;
				dxweight.weight = asimpweight.weight;
				dxbone.weights.push_back(dxweight);
			}

			m_BoneDictionary[asimpbone.first] = dxbone;
		}

		m_AssimpNodeNameTree = std::make_shared<TreeNode<std::string>>();

		// ボーン名ツリー取得
		m_AssimpNodeNameTree = AssimpPerse::Instance().GetBoneNameTree();

		// レンダラ初期化
		m_StaticMeshRenderer.Init(StaticMesh::Instance());
	}
	void AnimationMesh::UpdateBoneMatrix(std::weak_ptr<TreeNode<std::string>> pTree, DirectX::SimpleMath::Matrix matrix)
	{

		if (auto pLockTree = pTree.lock())
		{
			// ノード名からボーン辞書を使ってボーン情報を取得
			D3D::BONE* bone = &m_BoneDictionary[pLockTree->m_nodeData];

			DirectX::SimpleMath::Matrix bonecombination;

			// ボーンオフセット行列×ボーンアニメメーション行列×逆ボーンオフセット行列
			bonecombination = bone->OffsetMatrix * bone->AnimationMatrix * matrix;
			bone->Matrix = bonecombination;

			// 自分の姿勢を表す行列を作成
			DirectX::SimpleMath::Matrix mybonemtx;
			mybonemtx = bone->AnimationMatrix * matrix;
			// 子ノードに対して再帰的に処理	
			for (unsigned int n = 0; n < pLockTree->m_children.size(); n++)
			{
				UpdateBoneMatrix(pLockTree->m_children[n], mybonemtx);
			}
		}
	}

	void AnimationMesh::Update(BoneCombMatrix& bonecombarray, int& CurrentFrame)
	{
		// アニメーションデータ取得
		auto animation = m_CurrentAnimation;

		// ボーン数分ループしてボーン行列を作成
		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];

			// ノード名からボーン辞書を使ってassimpのボーン情報を取得
			D3D::BONE* bone = &(m_BoneDictionary[nodeAnim->mNodeName.C_Str()]);	// 20240714 DX化	

			int f;

			f = CurrentFrame % nodeAnim->mNumRotationKeys;
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = CurrentFrame % nodeAnim->mNumPositionKeys;
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			// SRTから行列を生成
			auto s = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
			auto t = DirectX::SimpleMath::Vector3(pos.x, pos.y, pos.z);
			DirectX::SimpleMath::Quaternion r{};

			r.x = rot.x;
			r.y = rot.y;
			r.z = rot.z;
			r.w = rot.w;

			auto scalemtx = DirectX::SimpleMath::Matrix::CreateScale(s.x, s.y, s.z);
			auto rotmtx = DirectX::SimpleMath::Matrix::CreateFromQuaternion(r);
			auto transmtx = DirectX::SimpleMath::Matrix::CreateTranslation(t.x, t.y, t.z);

			bone->AnimationMatrix = scalemtx * rotmtx * transmtx;
		}

		UpdateBoneMatrix(m_AssimpNodeNameTree, DirectX::SimpleMath::Matrix::Identity);		// 20240714 DX化	

		// ボーンコンビネーション行列の配列をセット
		for (const auto& bone : m_BoneDictionary)
		{
			bonecombarray.ConstantBufferMemory.BoneCombMtx[bone.second.idx] = bone.second.Matrix.Transpose();	// 20240714 DX化
		}
	}

	void AnimationMesh::BlendUpdate(BoneCombMatrix& bonecombarray, int fromframe, int toframe, float blendarate)
	{
		// アニメーションデータ取得
		auto fromanimation = m_FromAnimation;

		// ボーン数分ループしてボーン行列を作成
		for (unsigned int c = 0; c < fromanimation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = fromanimation->mChannels[c];

			// ノード名からボーン辞書を使ってassimpのボーン情報を取得
			D3D::BONE* bone = &(m_BoneDictionary[nodeAnim->mNodeName.C_Str()]);

			int f;

			f = fromframe % nodeAnim->mNumRotationKeys;				//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = fromframe % nodeAnim->mNumPositionKeys;				//簡易実装
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->fromPos.x = pos.x;
			bone->fromPos.y = pos.y;
			bone->fromPos.z = pos.z;

			bone->fromScale.x = 1.0f;
			bone->fromScale.y = 1.0f;
			bone->fromScale.z = 1.0f;

			bone->fromQuat.x = rot.x;
			bone->fromQuat.y = rot.y;
			bone->fromQuat.z = rot.z;
			bone->fromQuat.w = rot.w;
		}

		// アニメーションデータ取得
		auto toanimation = m_ToAnimation;

		// ボーン数分ループしてボーン行列を作成
		for (unsigned int c = 0; c < toanimation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = toanimation->mChannels[c];

			// ノード名からボーン辞書を使ってassimpのボーン情報を取得
			D3D::BONE* bone = &(m_BoneDictionary[nodeAnim->mNodeName.C_Str()]);

			int f;

			f = toframe % nodeAnim->mNumRotationKeys;				//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = toframe % nodeAnim->mNumPositionKeys;				//簡易実装
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->toPos.x = pos.x;
			bone->toPos.y = pos.y;
			bone->toPos.z = pos.z;

			bone->toScale.x = 1.0f;
			bone->toScale.y = 1.0f;
			bone->toScale.z = 1.0f;

			bone->toQuat.x = rot.x;
			bone->toQuat.y = rot.y;
			bone->toQuat.z = rot.z;
			bone->toQuat.w = rot.w;
		}

		// ブレンドする
		// ボーン数分ループしてボーン行列を作成
		for (unsigned int c = 0; c < toanimation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = toanimation->mChannels[c];

			// ノード名からボーン辞書を使ってassimpのボーン情報を取得
			D3D::BONE* bone = &(m_BoneDictionary[nodeAnim->mNodeName.C_Str()]);

			auto r = DirectX::SimpleMath::Quaternion::Slerp(bone->fromQuat, bone->toQuat, blendarate);
			auto t = DirectX::SimpleMath::Vector3::Lerp(bone->fromPos, bone->toPos, blendarate);
			auto s = DirectX::SimpleMath::Vector3::Lerp(bone->fromScale, bone->toScale, blendarate);

			auto scalemtx = DirectX::SimpleMath::Matrix::CreateScale(s.x, s.y, s.z);
			auto rotmtx = DirectX::SimpleMath::Matrix::CreateFromQuaternion(r);
			auto transmtx = DirectX::SimpleMath::Matrix::CreateTranslation(t.x, t.y, t.z);

			bone->AnimationMatrix = scalemtx * rotmtx * transmtx;
		}

		// ボーンコンビネーション行列をすべて再帰を使用して更新する
		UpdateBoneMatrix(m_AssimpNodeNameTree, DirectX::SimpleMath::Matrix::Identity);

		// ボーンコンビネーション行列の配列をセット
		for (const auto& bone : m_BoneDictionary)
		{
			bonecombarray.ConstantBufferMemory.BoneCombMtx[bone.second.idx] = bone.second.Matrix.Transpose();
		}
	}
	void AnimationMesh::Draw()
	{
		m_StaticMeshRenderer.Draw();
	}
	DirectX::SimpleMath::Matrix AnimationMesh::AiMatrixToSimpleMatrix(aiMatrix4x4 asimpmtx)
	{
		auto matrix = DirectX::SimpleMath::Matrix::Identity;
		matrix._11 = asimpmtx.a1; matrix._12 = asimpmtx.b1; matrix._13 = asimpmtx.c1; matrix._14 = asimpmtx.d1;
		matrix._21 = asimpmtx.a2; matrix._22 = asimpmtx.b2; matrix._23 = asimpmtx.c2; matrix._24 = asimpmtx.d2;
		matrix._31 = asimpmtx.a3; matrix._32 = asimpmtx.b3; matrix._33 = asimpmtx.c3; matrix._34 = asimpmtx.d3;
		matrix._41 = asimpmtx.a4; matrix._42 = asimpmtx.b4; matrix._43 = asimpmtx.c4; matrix._44 = asimpmtx.d4;
		return matrix;
	}
}