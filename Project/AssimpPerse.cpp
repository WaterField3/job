#include "AssimpPerse.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <vector>
#include <iostream>
#include <unordered_map>
#include <cassert>

#include "AssimpTexture.h"

namespace TMF
{
	void AssimpPerse::GetModelData(std::string filename, std::string texturedirectory)
	{
		// シーン情報構築
		Assimp::Importer importer;

		// シーン情報を構築
		const aiScene* pScene = importer.ReadFile(
			filename.c_str(),
			//			aiProcess_ConvertToLeftHanded |	// 左手座標系に変換する
			//			aiProcess_Triangulate);			// 三角形化する
			aiProcessPreset_TargetRealtime_MaxQuality |
			aiProcess_ConvertToLeftHanded |
			aiProcess_PopulateArmatureData);		// 20231225追加

		if (pScene == nullptr)
		{
			std::cout << "load error" << filename.c_str() << importer.GetErrorString() << std::endl;
		}
		assert(pScene != nullptr);

		// 読み込み領域をクリア
		m_vertices.clear();				//20240908
		m_indices.clear();				//20240908	
		m_materials.clear();			//20240908
		m_diffuseTextures.clear();		//20240908
		m_subsets.clear();				//20240908
		m_BoneDictionary.clear();		//20240908
		m_BonesPerMeshes.clear();		//20240908

		// マテリアル情報取得
		GetMaterialData(pScene, texturedirectory);

		m_vertices.resize(pScene->mNumMeshes);

		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			aiMesh* mesh = pScene->mMeshes[m];

			// メッシュ名取得
			std::string meshname = std::string(mesh->mName.C_Str());

			//　頂点数分ループ
			for (unsigned int vidx = 0; vidx < mesh->mNumVertices; vidx++)
			{
				// 頂点データ
				VERTEX	v{};
				v.meshname = meshname;		// メッシュ名セット

				// 座標		
				v.pos = mesh->mVertices[vidx];

				// この頂点が使用しているマテリアルのインデックス番号（メッシュ内の）
				// を使用してマテリアル名をセット
				v.materialindex = mesh->mMaterialIndex;

				v.mtrlname = m_materials[mesh->mMaterialIndex].mtrlname;

				// 法線あり？
				if (mesh->HasNormals()) {
					v.normal = mesh->mNormals[vidx];
				}
				else
				{
					v.normal = aiVector3D(0.0f, 0.0f, 0.0f);
				}

				// 頂点カラー？（０番目）
				if (mesh->HasVertexColors(0)) {
					v.color = mesh->mColors[0][vidx];
				}
				else
				{
					v.color = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
				}

				// テクスチャあり？（０番目）
				if (mesh->HasTextureCoords(0)) {
					v.texcoord = mesh->mTextureCoords[0][vidx];
				}
				else
				{
					v.texcoord = aiVector3D(0.0f, 0.0f, 0.0f);
				}

				// 頂点データを追加
				m_vertices[m].push_back(v);
			}
		}

		// メッシュ数文ループ
		// インデックスデータ作成
		m_indices.resize(pScene->mNumMeshes);
		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			aiMesh* mesh = pScene->mMeshes[m];

			// メッシュ名取得
			std::string meshname = std::string(mesh->mName.C_Str());

			// インデックス数分ループ
			for (unsigned int fidx = 0; fidx < mesh->mNumFaces; fidx++)
			{
				aiFace face = mesh->mFaces[fidx];

				//				assert(face.mNumIndices == 3);	// 三角形のみ対応   car000.x　対応
				assert(face.mNumIndices <= 3);	// 三角形以下であればOK（縮退ポリゴン）

				// インデックスデータを追加
				for (unsigned int i = 0; i < face.mNumIndices; i++)
				{
					m_indices[m].push_back(face.mIndices[i]);
				}
			}
		}

		// サブセット情報を生成
		m_subsets.resize(pScene->mNumMeshes);
		for (unsigned int m = 0; m < m_subsets.size(); m++)
		{
			m_subsets[m].IndexNum = m_indices[m].size();
			m_subsets[m].VertexNum = m_vertices[m].size();
			m_subsets[m].VertexBase = 0;
			m_subsets[m].IndexBase = 0;
			m_subsets[m].meshname = m_vertices[m][0].meshname;
			m_subsets[m].mtrlname = m_vertices[m][0].mtrlname;
			m_subsets[m].materialindex = m_vertices[m][0].materialindex;
		}

		// サブセット情報を相対的なものにする	
		for (int m = 0; m < m_subsets.size(); m++)
		{
			// 頂点バッファのベースを計算
			m_subsets[m].VertexBase = 0;
			for (int i = m - 1; i >= 0; i--) {
				m_subsets[m].VertexBase += m_subsets[i].VertexNum;
			}

			// インデックスバッファのベースを計算
			m_subsets[m].IndexBase = 0;
			for (int i = m - 1; i >= 0; i--) {
				m_subsets[m].IndexBase += m_subsets[i].IndexNum;
			}
		}

		// ボーン情報取得	
		GetBone(pScene);
	}
	std::vector <AssimpPerse::SUBSET > AssimpPerse::GetSubsets()
	{
		return m_subsets;
	}
	std::vector<std::vector<AssimpPerse::VERTEX>> AssimpPerse::GetVertices()
	{
		return m_vertices;
	}
	std::vector<std::vector<unsigned int>> AssimpPerse::GetIndices()
	{
		return m_indices;
	}
	std::vector<AssimpPerse::MATERIAL> AssimpPerse::GetMaterials()
	{
		return m_materials;
	}
	std::vector<std::unique_ptr<AssimpTexture>> AssimpPerse::MoveDiffuseTextures()
	{
		return std::move(m_diffuseTextures);
	}
	std::unordered_map<std::string, AssimpPerse::BONE> AssimpPerse::GetBoneDictionary()
	{
		return m_BoneDictionary;
	}
	std::shared_ptr<TreeNode<std::string>> AssimpPerse::GetBoneNameTree()
	{
		return std::move(m_AssimpNodeNameTree);
	}
	void AssimpPerse::GetMaterialData(const aiScene* pScene, std::string texturedirectory)
	{
		// マテリアル数分テクスチャ格納エリアを用意する
		m_diffuseTextures.resize(pScene->mNumMaterials);

		// マテリアル数文ループ
		for (unsigned int m = 0; m < pScene->mNumMaterials; m++)
		{
			aiMaterial* material = pScene->mMaterials[m];

			// マテリアル名取得
			std::string mtrlname = std::string(material->GetName().C_Str());
			std::cout << mtrlname << std::endl;

			// マテリアル情報
			aiColor4D ambient;
			aiColor4D diffuse;
			aiColor4D specular;
			aiColor4D emission;
			float shiness;

			// アンビエント
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
			}
			else {
				ambient = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
			}

			// ディフューズ
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
			}
			else {
				diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// スペキュラ
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
			}
			else {
				specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
			}

			// エミッション
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission)) {
			}
			else {
				emission = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
			}

			// シャイネス
			if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shiness)) {
			}
			else {
				shiness = 0.0f;
			}

			// このマテリアルに紐づいているディフューズテクスチャ数分ループ
			std::vector<std::string> texpaths{};

			for (unsigned int t = 0; t < material->GetTextureCount(aiTextureType_DIFFUSE); t++)
			{
				aiString path{};

				// t番目のテクスチャパス取得
				if (AI_SUCCESS == material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, t), path))
				{
					// テクスチャパス取得
					std::string texpath = std::string(path.C_Str());
					std::cout << texpath << std::endl;

					texpaths.push_back(texpath);
					// 内蔵テクスチャかどうかを判断する
					if (auto tex = pScene->GetEmbeddedTexture(path.C_Str())) {

						auto texture = std::make_unique<AssimpTexture>();

						// 内蔵テクスチャの場合
						bool sts = texture->LoadFromFemory(
							(unsigned char*)tex->pcData,			// 先頭アドレス
							tex->mWidth);			// テクスチャサイズ（メモリにある場合幅がサイズ）	
						if (sts) {
							m_diffuseTextures[m] = std::move(texture);
						}
						std::cout << "Embedded" << std::endl;

					}
					else {
						// 外部テクスチャファイルの場合
						auto texture = std::make_unique<AssimpTexture>();

						std::string texname = texturedirectory + "/" + texpath;

						bool sts = texture->Load(texname);
						if (sts) {
							m_diffuseTextures[m] = std::move(texture);
						}

						std::cout << "other Embedded" << std::endl;
					}
				}
				// ディフューズテクスチャがなかった場合
				else
				{
					// 外部テクスチャファイルの場合
					auto texture = std::make_unique<AssimpTexture>();
					m_diffuseTextures[m] = std::move(texture);
				}
			}

			// マテリアル情報を保存
			AssimpPerse::MATERIAL mtrl{};

			mtrl.mtrlname = mtrlname;
			mtrl.Ambient = ambient;
			mtrl.Diffuse = diffuse;
			mtrl.Specular = specular;
			mtrl.Emission = emission;
			mtrl.Shiness = shiness;

			if (texpaths.size() == 0)
				mtrl.diffusetexturename = "";
			else
				mtrl.diffusetexturename = texpaths[0];

			m_materials.push_back(mtrl);
		}
	}
	std::vector<AssimpPerse::BONE> AssimpPerse::GetBonesPerMesh(const aiMesh* mesh)
	{
		std::vector<BONE> bones;		// このサブセットメッシュで使用されているボーンコンテナ

		// ボーン数分ループ
		for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {

			BONE bone{};

			// ボーン名取得
			bone.bonename = std::string(mesh->mBones[bidx]->mName.C_Str());

			// メッシュノード名
			bone.meshname = std::string(mesh->mBones[bidx]->mNode->mName.C_Str());

			// アーマチュアノード名
			bone.armaturename = std::string(mesh->mBones[bidx]->mArmature->mName.C_Str());

			// デバッグ用
			std::cout << bone.bonename
				<< "(" << bone.meshname << ")"
				<< "(" << bone.armaturename << ")"
				<< std::endl;

			// ボーンオフセット行列取得
			bone.OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;

			// ウェイト情報抽出
			bone.weights.clear();
			for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {

				WEIGHT w;
				w.meshname = bone.meshname;										// メッシュ名
				w.bonename = bone.bonename;										// ボーン名

				w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;			// 重み
				w.vertexindex = mesh->mBones[bidx]->mWeights[widx].mVertexId;	// 頂点インデックス
				bone.weights.emplace_back(w);
			}

			// コンテナに登録
			bones.emplace_back(bone);

			// ボーン辞書にも反映させる
			m_BoneDictionary[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;
		}

		return bones;
	}
	void AssimpPerse::CreateEmptyBoneDictionary(aiNode* node)
	{
		BONE bone{};

		// ボーン名で参照できるように空のボーン情報をセットする
		m_BoneDictionary[node->mName.C_Str()] = bone;

		std::cout << node->mName.C_Str() << std::endl;

		for (unsigned int n = 0; n < node->mNumChildren; n++)
		{
			CreateEmptyBoneDictionary(node->mChildren[n]);
		}
	}
	void AssimpPerse::SetBoneDataToVertices()
	{

		// ボーンインデックスを初期化
		for (auto& vtbl : m_vertices) {
			for (auto& v : vtbl) {
				v.bonecnt = 0;
				for (int b = 0; b < 4; b++) {
					v.BoneIndex[b] = -1;
					v.BoneWeight[b] = 0.0f;
					v.BoneName[b] = "";
				}
			}
		}

		// メッシュ毎のボーンコンテナ
		int subsetid = 0;
		for (auto& bones : m_BonesPerMeshes) {

			// このスタティックメッシュ内の頂点データのスタート位置を取得
//			int vertexbase = g_subsets[subsetid].VertexBase;

			// このサブセット内のボーンをひとつづつ取り出す
			for (auto& bone : bones)
			{
				for (auto& w : bone.weights) {
					int& idx = m_vertices[subsetid][w.vertexindex].bonecnt;

					m_vertices[subsetid][w.vertexindex].BoneName[idx] = w.bonename;	// ボーン名をセット
					m_vertices[subsetid][w.vertexindex].BoneWeight[idx] = w.weight;	// weight値をセット
					m_vertices[subsetid][w.vertexindex].BoneIndex[idx] = m_BoneDictionary[w.bonename].idx;

					//ボーンの配列番号をセット
					idx++;
					assert(idx <= 4);
				}
			}
			subsetid++;				// 次のメッシュへ
		}
	}

	void AssimpPerse::CreateNodeTree(aiNode* node, std::weak_ptr<TreeNode<std::string>> ptree)
	{
		if (auto pLockTree = ptree.lock())
		{
			pLockTree->m_nodeData = std::string(node->mName.C_Str());
			//		std::cout << node->mName.C_Str() << std::endl;

			for (unsigned int n = 0; n < node->mNumChildren; n++)
			{
				std::shared_ptr<TreeNode<std::string>> pchild = std::make_shared<TreeNode<std::string>>();
				pchild->m_pParent = pLockTree;
				pLockTree->AddChild(std::move(pchild));
				CreateNodeTree(node->mChildren[n], pLockTree->m_children[n]);
			}
		}
	}
	void AssimpPerse::GetBone(const aiScene* pScene)
	{
		// 空のボーン辞書を作成する（キー（ボーン名）だけの取り出し）
		CreateEmptyBoneDictionary(pScene->mRootNode);

		// ボーンの配列位置（インデックス値）を格納する
		unsigned int num = 0;
		for (auto& data : m_BoneDictionary)
		{
			data.second.idx = num;
			num++;
		}

		// メッシュ数分ループ
		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			aiMesh* mesh = pScene->mMeshes[m];

			// サブセットに紐づいているボーン情報を取得する
			std::vector<BONE> BonesPerMesh = GetBonesPerMesh(mesh);
			m_BonesPerMeshes.emplace_back(BonesPerMesh);
		}

		// 頂点データにボーン情報をセットする
		SetBoneDataToVertices();

		m_AssimpNodeNameTree = std::make_shared<TreeNode<std::string>>();

		// ボーンのノード名ツリーを生成する
		CreateNodeTree(pScene->mRootNode, m_AssimpNodeNameTree);
	}
}